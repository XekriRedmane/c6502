"""The spelling of integer and floating constants: preprocessing
numbers, digit separators, binary constants, and suffixes.

C23 §6.4.8 defines a *preprocessing number* (pp-number) as the longest
run that begins with a digit, or a period and a digit, and continues
with identifier characters, periods, the pairs `e+`, `e-`, `E+`, `E-`,
`p+`, `p-`, `P+`, and `P-`, and a single quote followed by a digit or
a nondigit (a letter or `_`). Every integer and floating constant is a
pp-number, and a pp-number that isn't one of them is an error once it
reaches translation phase 7.

C23 §6.4.4.2 adds two things to the spelling of a constant.

  * A *binary constant* is `0b` or `0B` followed by the digits 0 and
    1. It's typed by the same list as octal and hexadecimal constants
    (the table in §6.4.4.2p6).
  * A *digit separator* is a single quote between two digits of an
    integer or floating constant, as in `1'000'000` or `0xFF'FF`.
    Separators are ignored for the value. The grammar puts one only
    between two digits of the same digit sequence, so a separator
    can't come first or last, can't be doubled, and can't touch the
    prefix (`0x'FF`), the period (`1'.5`, `1.'5`), the exponent's
    letter or sign (`1'e5`, `1e'5`), or the suffix (`1'u`). An octal
    constant's leading 0 is one of its digits, so `0'17` is octal 017.

`pp_number_at` finds a pp-number in a source text, and
`parse_integer` and `check_floating` read one, raising `NumberError`
with a message that names what's wrong. The lexer, the parser, and
the preprocessor's `#if` all use them, so the three accept the same
spellings and give the same messages.

The suffix grammar includes C23's `wb` and `uwb` for `_BitInt`
constants. `parse_integer` reports such a suffix in
`IntegerConstant.bit_precise`, and `bit_precise_width` gives the
width of the constant's type, or an error when that's wider than
BITINT_MAXWIDTH.
"""
from __future__ import annotations

import re
from dataclasses import dataclass

# A pp-number (C23 §6.4.8). The exponent pairs come first, so that
# the `+` or `-` after an `e` is taken with it.
PP_NUMBER_RE = re.compile(
    r"\.?[0-9](?:[eEpP][+-]|'[0-9A-Za-z_]|[0-9A-Za-z_.])*"
)

# The digits of each base. An octal constant's digits include its
# leading 0.
_DIGITS = {
    2: "01",
    8: "01234567",
    10: "0123456789",
    16: "0123456789abcdefABCDEF",
}

_BASE_NAMES = {
    2: "binary", 8: "octal", 10: "decimal", 16: "hexadecimal",
}

# An integer suffix (C23 §6.4.4.2p1): `u` or `U` with or without a
# length, and a length with or without `u` or `U` after it. The
# lengths are `l`, `L`, `ll`, `LL`, `wb`, and `WB`, so `lL` and `wB`
# aren't suffixes.
_INTEGER_SUFFIX_RE = re.compile(
    r"(?:(?P<u1>[uU])(?P<len1>ll|LL|l|L|wb|WB)?"
    r"|(?P<len2>ll|LL|l|L|wb|WB)(?P<u2>[uU])?)?"
)

# The floating suffixes (C23 §6.4.4.3p1). `df`, `dd`, and `dl` give
# the decimal floating types, which c6502 doesn't have.
_FLOATING_SUFFIXES = ("", "f", "F", "l", "L")
_DECIMAL_FLOATING_SUFFIXES = ("df", "dd", "dl", "DF", "DD", "DL")


class NumberError(ValueError):
    """A pp-number that isn't a valid constant. The message names the
    spelling and what's wrong with it."""


@dataclass(frozen=True)
class IntegerConstant:
    """An integer constant read by `parse_integer`. `value` ignores the
    digit separators. `base` is 2, 8, 10, or 16. `length` is "", "l",
    "ll", or "wb", lowercased, and `unsigned` is True when the suffix
    has a `u` or `U`."""
    value: int
    base: int
    unsigned: bool
    length: str

    @property
    def bit_precise(self) -> bool:
        """True for the `wb` and `uwb` suffixes of a `_BitInt`
        constant."""
        return self.length == "wb"


def bit_precise_width(constant: IntegerConstant, spelling: str) -> int:
    """The width N of the type of `constant`, whose suffix is `wb` or
    `uwb`: the `_BitInt(N)` or `unsigned _BitInt(N)` of the smallest N
    that holds its value, with a sign bit for `wb` (C23 §6.4.4.2p6).
    Raises NumberError when N is greater than BITINT_MAXWIDTH, since
    the constant then has no type (§6.4.4.2p7)."""
    import bit_precise
    width = bit_precise.constant_width(
        constant.value, signed=not constant.unsigned,
    )
    if width > bit_precise.BITINT_MAXWIDTH:
        kind = "unsigned _BitInt" if constant.unsigned else "_BitInt"
        raise NumberError(
            f"integer constant {spelling!r} needs a {kind}({width}), and "
            f"BITINT_MAXWIDTH is {bit_precise.BITINT_MAXWIDTH} (C23 "
            f"§6.4.4.2p6)"
        )
    return width


def pp_number_at(text: str, pos: int) -> str:
    """The pp-number that starts at `text[pos]`, or "" when none
    does."""
    m = PP_NUMBER_RE.match(text, pos)
    return m.group(0) if m else ""


def is_floating(spelling: str) -> bool:
    """True when the pp-number `spelling` has the form of a floating
    constant: a period, or an exponent (`e` in a decimal number, `p`
    in a hexadecimal one). A binary constant has neither, and its `b`
    isn't an exponent."""
    if spelling[:2] in ("0b", "0B"):
        return False
    if "." in spelling:
        return True
    if spelling[:2] in ("0x", "0X"):
        return any(c in "pP" for c in spelling)
    return any(c in "eE" for c in spelling)


def _scan_digits(spelling: str, i: int, base: int) -> int:
    """Reads the digit sequence of `base` that starts at `spelling[i]`,
    with digit separators between its digits, and returns the index
    after it. Raises NumberError for a separator that isn't between
    two digits of the sequence. The sequence may be empty."""
    digits = _DIGITS[base]
    n = len(spelling)
    while i < n:
        c = spelling[i]
        if c in digits:
            i += 1
            continue
        if c != "'":
            return i
        if i == 0 or spelling[i - 1] not in digits:
            raise NumberError(_misplaced(spelling, i))
        if i + 1 < n and spelling[i + 1] in digits:
            i += 1
            continue
        raise NumberError(_misplaced(spelling, i))
    return i


def _misplaced(spelling: str, i: int) -> str:
    """The message for the digit separator at `spelling[i]`, which
    isn't between two digits of one digit sequence."""
    before = spelling[i - 1] if i > 0 else ""
    after = spelling[i + 1] if i + 1 < len(spelling) else ""
    if after == "":
        why = "ends the number"
    elif after == "'":
        why = "is doubled"
    elif before in "xXbB" and i == 2 and spelling[0] == "0":
        why = f"comes right after the prefix 0{before}"
    elif before == "." or after == ".":
        why = "touches the period"
    elif before in "+-" or (
        before in "eEpP" and not _is_hex_digit_here(spelling, i - 1)
    ):
        why = "comes right after the exponent's letter or sign"
    elif after in "eEpP" and not _is_hex_digit_here(spelling, i + 1):
        why = "comes right before the exponent"
    elif after.isdigit():
        # `0'8` or `0b1'2`: a digit that the base doesn't have.
        return _bad_digit(spelling, i + 1)
    else:
        why = "comes right before the suffix"
    return (
        f"a digit separator has to be between two digits, and the one "
        f"in {spelling!r} {why}"
    )


def _is_hex_digit_here(spelling: str, i: int) -> bool:
    """True when `spelling[i]` is a digit of a hexadecimal number's
    digit sequence, where `e` is a digit, rather than an exponent
    letter."""
    if spelling[:2] not in ("0x", "0X"):
        return False
    if spelling[i] in "pP":
        return False
    rest = spelling[2:i]
    return not any(c in "pP" for c in rest)


def _bad_digit(spelling: str, i: int) -> str:
    """The message for the digit at `spelling[i]`, which the number's
    base doesn't have."""
    base = 2 if spelling[:2] in ("0b", "0B") else 8
    return (
        f"invalid digit {spelling[i]!r} in {_BASE_NAMES[base]} constant "
        f"{spelling!r}"
    )


def parse_integer(spelling: str) -> IntegerConstant:
    """Reads the integer constant `spelling`, a pp-number with no
    period and no exponent. Raises NumberError when it isn't a valid
    integer constant."""
    n = len(spelling)
    if spelling[:2] in ("0x", "0X"):
        base, i = 16, 2
    elif spelling[:2] in ("0b", "0B"):
        base, i = 2, 2
    elif spelling[:1] == "0":
        base, i = 8, 0
    else:
        base, i = 10, 0
    if base in (2, 16) and (i == n or spelling[i] not in _DIGITS[base]):
        if i < n and spelling[i] == "'":
            raise NumberError(_misplaced(spelling, i))
        if i < n and spelling[i].isdigit():
            raise NumberError(_bad_digit(spelling, i))
        raise NumberError(
            f"no digits after the prefix {spelling[:2]} in {spelling!r}"
        )
    end = _scan_digits(spelling, i, base)
    if end < n and spelling[end].isdigit():
        raise NumberError(_bad_digit(spelling, end))
    digits = spelling[i:end].replace("'", "")
    suffix = spelling[end:]
    if base == 2 and "." in suffix:
        raise NumberError(
            f"a binary constant is an integer, so {spelling!r} can't "
            f"have a period"
        )
    m = _INTEGER_SUFFIX_RE.fullmatch(suffix)
    if m is None:
        raise NumberError(
            f"invalid suffix {suffix!r} on integer constant {spelling!r}"
        )
    length = (m.group("len1") or m.group("len2") or "").lower()
    unsigned = bool(m.group("u1") or m.group("u2"))
    return IntegerConstant(
        value=int(digits, base), base=base, unsigned=unsigned,
        length=length,
    )


def check_floating(spelling: str) -> tuple[str, str]:
    """Checks the floating constant `spelling`, a pp-number with a
    period or an exponent, and returns `(body, suffix)`: the constant
    without its digit separators and suffix, and the suffix. Raises
    NumberError when it isn't a valid floating constant (C23
    §6.4.4.3p1)."""
    n = len(spelling)
    hexadecimal = spelling[:2] in ("0x", "0X")
    base = 16 if hexadecimal else 10
    i = 2 if hexadecimal else 0
    if hexadecimal and i < n and spelling[i] == "'":
        raise NumberError(_misplaced(spelling, i))
    whole_end = _scan_digits(spelling, i, base)
    has_digits = whole_end > i
    i = whole_end
    if i < n and spelling[i] == ".":
        i += 1
        if i < n and spelling[i] == "'":
            raise NumberError(_misplaced(spelling, i))
        frac_end = _scan_digits(spelling, i, base)
        has_digits = has_digits or frac_end > i
        i = frac_end
    if not has_digits:
        raise NumberError(
            f"floating constant {spelling!r} has no digits before or "
            f"after the period"
        )
    exponent_letters = "pP" if hexadecimal else "eE"
    if i < n and spelling[i] in exponent_letters:
        i += 1
        if i < n and spelling[i] in "+-":
            i += 1
        if i < n and spelling[i] == "'":
            raise NumberError(_misplaced(spelling, i))
        exp_end = _scan_digits(spelling, i, 10)
        if exp_end == i:
            raise NumberError(
                f"the exponent of {spelling!r} has no digits"
            )
        i = exp_end
    elif hexadecimal:
        raise NumberError(
            f"hexadecimal floating constant {spelling!r} has no binary "
            f"exponent (a p and a power of 2)"
        )
    body, suffix = spelling[:i], spelling[i:]
    if suffix in _DECIMAL_FLOATING_SUFFIXES:
        raise NumberError(
            f"the suffix {suffix!r} of {spelling!r} gives a decimal "
            f"floating type, which c6502 doesn't have"
        )
    if suffix not in _FLOATING_SUFFIXES:
        raise NumberError(
            f"invalid suffix {suffix!r} on floating constant {spelling!r}"
        )
    return body.replace("'", ""), suffix


def check(spelling: str) -> None:
    """Raises NumberError when the pp-number `spelling` isn't a valid
    integer or floating constant."""
    if is_floating(spelling):
        check_floating(spelling)
    else:
        parse_integer(spelling)
