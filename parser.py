"""Parser for C99 — builds c99_ast nodes from the Lark parse tree.

Adding a grammar rule:
  1. Add/modify the rule in c99.lark.
  2. If the rule has alternatives that map to different AST constructors,
     give each alternative a name with `-> name`:
         statement: RETURN exp SEMICOLON            -> return_stmt
                  | IF LPAREN exp RPAREN statement  -> if_stmt
  3. Add a Transformer method in `_ASTBuilder` with the same name as the
     rule (or the alternative). The method receives the rule's items —
     subtrees already converted to AST nodes, terminals as Lark tokens —
     and returns the AST node for that rule.

@v_args(inline=True) spreads the rule's items into named parameters so
each method's signature mirrors the rule body. Unused items (the
punctuator terminals) are conventionally prefixed with `_`.
"""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path

from lark import Lark, Token, Transformer
from lark.exceptions import VisitError
from lark.visitors import v_args

import c99_ast
import fp_arith


_GRAMMAR_PATH = Path(__file__).parent / "c99.lark"
_LARK = Lark.open(
    str(_GRAMMAR_PATH),
    parser="lalr",
    lexer="basic",
    # `start` is the real translation-unit entry point; `lex_only`
    # keeps every terminal reachable so the lexer-only tests don't
    # see "unused terminal" trimming. `declarator` and `type_name`
    # are exposed so the §6.7.5 / §6.7.6 grammar can be unit-tested
    # in isolation, since nothing reachable from `start` exercises
    # the named-declarator side yet (var_decl / function_decl still
    # take a bare IDENTIFIER, not a full declarator).
    start=["start", "lex_only", "declarator", "type_name"],
)


class ParserError(Exception):
    """Raised for declaration-specifier constraint violations that the
    LALR grammar accepts but C99 §6.7.2 / §6.7.1 forbids — multiple
    type specifiers, multiple storage-class specifiers, or a missing
    type specifier (C99 dropped the C89 implicit-int rule)."""


# Compound-assignment operator tokens → AST binary-operator class. The
# parser desugars `lval OP= rval` into `lval = lval OP rval`, so each
# compound operator just needs to name the binary op it expands to.
_COMPOUND_ASSIGN_OPS = {
    "PLUS_ASSIGN":    c99_ast.Add,
    "MINUS_ASSIGN":   c99_ast.Subtract,
    "STAR_ASSIGN":    c99_ast.Multiply,
    "SLASH_ASSIGN":   c99_ast.Divide,
    "PERCENT_ASSIGN": c99_ast.Modulo,
    "AMP_ASSIGN":     c99_ast.BitwiseAnd,
    "PIPE_ASSIGN":    c99_ast.BitwiseOr,
    "CARET_ASSIGN":   c99_ast.BitwiseXor,
    "LSHIFT_ASSIGN":  c99_ast.LeftShift,
    "RSHIFT_ASSIGN":  c99_ast.RightShift,
}


# Storage-class specifier token type → AST node class. Used by
# `_split_specifiers` to map the parsed token to the AST node that
# rides on `Type_var_decl.storage_class` / `Type_function_decl.storage_class`.
_STORAGE_CLASSES = {
    "STATIC": c99_ast.Static,
    "EXTERN": c99_ast.Extern,
}

# Token types of every leaf the `specifier` rule can produce. After the
# `type_specifier` and `specifier` transformer methods unwrap their
# child trees, every entry on a specifier list is one of these tokens
# OR a `_TagSpecifier` wrapper for struct/union forms (which expose a
# matching `.type` attribute so the same predicate logic works on
# both shapes).
_SPECIFIER_TOKEN_TYPES = ("INT", "LONG", "SIGNED", "UNSIGNED",
                           "FLOAT", "DOUBLE", "CHAR", "VOID",
                           "STATIC", "EXTERN",
                           "STRUCT", "UNION",
                           "CONST", "VOLATILE", "RESTRICT")
_TYPE_SPECIFIER_TOKEN_TYPES = ("INT", "LONG", "SIGNED", "UNSIGNED",
                                "FLOAT", "DOUBLE", "CHAR", "VOID",
                                "STRUCT", "UNION")
# `const` and `volatile` are honored at parse time and wrapped into
# `Const(...)` / `Volatile(...)` type-AST nodes. `restrict` parses
# but is silently dropped — c6502 has no aliasing-analysis pass that
# could use it.
_TYPE_QUALIFIER_TOKEN_TYPES = ("CONST", "VOLATILE", "RESTRICT")


# A struct/union type specifier produced by the
# `struct_or_union_specifier` transformer. The wrapper carries the
# resolved Structure/Union type alongside an optional list of
# member-declaration pairs (the body, when present). The
# specifier-list consumers (`_split_specifiers`, `_consume_specifiers`)
# treat these like tokens for the purpose of pulling specifiers off
# the front of a parse tree, but `_resolve_data_type` reads the
# wrapped type directly. The `body` rides through into the
# enclosing var_decl / function_decl transformer, which surfaces it
# as a separate `StructDecl` declaration when present.
@dataclass
class _TagSpecifier:
    type: str  # "STRUCT" or "UNION" — same protocol as Token.type
    tag: str
    is_union: bool
    body: list[tuple[str, "c99_ast.Type_data_type"]] | None  # None = reference, [] = empty body (illegal)


def _resolve_data_type(type_specs):
    """Map a list of type-specifier tokens to a c99_ast object type.
    Valid combinations follow C99 §6.7.2 (subset c6502 models —
    no `short`, `_Bool`):
      * `char`                                     → Char (signed,
                                                     -128..127 in
                                                     c6502)
      * `signed char`                              → SChar
      * `unsigned char`                            → UChar
      * `int`, `signed`, `signed int`              → Int
      * `unsigned`, `unsigned int`                 → UInt
      * `long`, `long int`,
        `signed long`, `signed long int`           → Long
      * `unsigned long`, `unsigned long int`       → ULong
      * `long long`, `long long int`,
        `signed long long`, `signed long long int` → LongLong
      * `unsigned long long`,
        `unsigned long long int`                   → ULongLong
      * `float`                                    → Float
      * `double`                                   → Double
      * `long double`                              — rejected (no long
                                                      double type)
      * `long long long` (or more `long`s)         — rejected
      * `signed unsigned` / `unsigned signed`      — rejected
      * `float`/`double` mixed with any of `int` / `long` /
        `signed` / `unsigned`                      — rejected
      * duplicate `int` / `signed` / `unsigned` / `float` /
        `double`                                   — rejected
      * empty list                                 — rejected
      * `struct foo` / `union foo`                  — Structure(tag) /
                                                      Union(tag); cannot
                                                      combine with any
                                                      other type
                                                      specifier
    """
    # Struct/union specifiers don't combine with any other type
    # specifier (C99 §6.7.2 is exhaustive — the type-specifier list
    # has to be exactly one of the listed combinations, and a
    # struct-or-union-specifier IS one such whole combination).
    tag_specs = [t for t in type_specs if isinstance(t, _TagSpecifier)]
    if tag_specs:
        if len(tag_specs) > 1:
            raise ParserError(
                "at most one struct or union specifier in a "
                "declaration"
            )
        if len(type_specs) != 1:
            raise ParserError(
                "struct/union specifier cannot combine with another "
                "type specifier"
            )
        ts = tag_specs[0]
        return (c99_ast.Union(tag=ts.tag) if ts.is_union
                else c99_ast.Structure(tag=ts.tag))
    int_count = sum(1 for t in type_specs if t.type == "INT")
    long_count = sum(1 for t in type_specs if t.type == "LONG")
    signed_count = sum(1 for t in type_specs if t.type == "SIGNED")
    unsigned_count = sum(1 for t in type_specs if t.type == "UNSIGNED")
    float_count = sum(1 for t in type_specs if t.type == "FLOAT")
    double_count = sum(1 for t in type_specs if t.type == "DOUBLE")
    char_count = sum(1 for t in type_specs if t.type == "CHAR")
    void_count = sum(1 for t in type_specs if t.type == "VOID")
    is_fp = float_count > 0 or double_count > 0
    is_integer = (
        int_count > 0 or signed_count > 0 or unsigned_count > 0
        or char_count > 0
    )
    if void_count > 0:
        if void_count > 1:
            raise ParserError("'void' specified more than once")
        if is_fp or is_integer or long_count > 0:
            raise ParserError(
                "'void' cannot combine with any other type specifier"
            )
        return c99_ast.Void()
    if is_fp and is_integer:
        raise ParserError(
            "floating type cannot combine with 'int' / 'signed' / "
            "'unsigned' / 'char'"
        )
    if float_count > 1:
        raise ParserError("'float' specified more than once")
    if double_count > 1:
        raise ParserError("'double' specified more than once")
    if float_count == 1 and double_count == 1:
        raise ParserError(
            "'float' and 'double' cannot both appear in a declaration"
        )
    if double_count == 1 and long_count >= 1:
        raise ParserError(
            "'long double' is not supported (only 'float' and "
            "'double' are modeled today)"
        )
    if float_count == 1 and long_count >= 1:
        raise ParserError(
            "'long float' is not a valid type"
        )
    if signed_count > 0 and unsigned_count > 0:
        raise ParserError(
            "'signed' and 'unsigned' cannot both appear in a "
            "declaration"
        )
    if signed_count > 1:
        raise ParserError("'signed' specified more than once")
    if unsigned_count > 1:
        raise ParserError("'unsigned' specified more than once")
    if char_count > 1:
        raise ParserError("'char' specified more than once")
    if char_count == 1 and (int_count > 0 or long_count > 0):
        raise ParserError(
            "'char' cannot combine with 'int' / 'long' / 'short'"
        )
    if not is_fp and (int_count == 0 and long_count == 0
            and signed_count == 0 and unsigned_count == 0
            and char_count == 0):
        raise ParserError("missing type specifier")
    if int_count > 1:
        raise ParserError(
            "multiple type specifiers in a declaration "
            "(at most one 'int' is permitted)"
        )
    if long_count > 2:
        raise ParserError(
            "'long' specified more than twice (no integer type wider "
            "than 'long long')"
        )
    if float_count == 1:
        return c99_ast.Float()
    if double_count == 1:
        return c99_ast.Double()
    is_unsigned = unsigned_count == 1
    is_signed = signed_count == 1
    if char_count == 1:
        if is_unsigned:
            return c99_ast.UChar()
        if is_signed:
            return c99_ast.SChar()
        return c99_ast.Char()
    if long_count == 2:
        return c99_ast.ULongLong() if is_unsigned else c99_ast.LongLong()
    if long_count == 1:
        return c99_ast.ULong() if is_unsigned else c99_ast.Long()
    if is_unsigned:
        return c99_ast.UInt()
    return c99_ast.Int()


def _split_specifiers(specs):
    """Validate a `specifier+` token list and split it into
    `(data_type, storage_class)`.

    The grammar rule `specifier: type_specifier | type_qualifier |
    STATIC | EXTERN` accepts any interleaving of type, qualifier,
    and storage-class specifiers in a declaration. C99 §6.7.1.2 /
    §6.7.2 are stricter:
      * exactly one type (composed from `INT` and `LONG` per
        `_resolve_data_type`)
      * at most one storage-class specifier
      * type qualifiers (`const`, `volatile`, `restrict`) may
        repeat — duplicates have no effect (§6.7.3.4); only `const`
        is reflected in the data_type, the others are silently
        accepted-and-dropped

    Returns `(data_type, storage_class)` where data_type is wrapped
    in `Const(...)` if any `const` qualifier appeared. storage_class
    is a `c99_ast.Static() / .Extern() / None`.
    """
    type_specs = []
    qualifier_specs = []
    storage = None
    for spec in specs:
        if spec.type in _TYPE_SPECIFIER_TOKEN_TYPES:
            type_specs.append(spec)
        elif spec.type in _TYPE_QUALIFIER_TOKEN_TYPES:
            qualifier_specs.append(spec)
        else:
            cls = _STORAGE_CLASSES[spec.type]
            if storage is not None:
                raise ParserError(
                    "at most one storage-class specifier permitted "
                    "in a declaration"
                )
            storage = cls()
    return (
        _apply_qualifiers(_resolve_data_type(type_specs), qualifier_specs),
        storage,
    )


def _apply_qualifiers(data_type, qualifier_tokens):
    """Wrap `data_type` in `Const(...)` and/or `Volatile(...)` per
    `qualifier_tokens`. `RESTRICT` is silently dropped (c6502 doesn't
    model aliasing analysis).

    Canonical wrapping order is Volatile-innermost, Const-outermost
    when both are present — so `const volatile int` becomes
    `Const(Volatile(Int))`. The order doesn't affect semantics (the
    type checker's strip helpers peel either or both), it just keeps
    structural equality predictable so downstream consumers don't
    have to handle two equivalent shapes.

    Idempotent per C99 §6.7.3.4 ("If the same qualifier appears more
    than once in the same specifier-qualifier-list ... the behavior
    is the same as if it appeared only once") — wrapping `Const(T)`
    in another `Const` (or `Volatile(T)` in another `Volatile`) is a
    no-op."""
    has_const = any(t.type == "CONST" for t in qualifier_tokens)
    has_volatile = any(t.type == "VOLATILE" for t in qualifier_tokens)
    if has_volatile and not _has_outer_volatile(data_type):
        data_type = c99_ast.Volatile(referenced_type=data_type)
    if has_const and not _has_outer_const(data_type):
        data_type = c99_ast.Const(referenced_type=data_type)
    return data_type


def _has_outer_const(t):
    """True iff `t` is `Const(...)` directly, or `Volatile(Const(...))`
    — i.e. a Const qualifier sits at any position before encountering
    a non-qualifier type. The idempotence check uses this so
    `const volatile const int` reduces to `Const(Volatile(Int))`
    without an inner duplicate `Const`."""
    while isinstance(t, c99_ast.Volatile):
        t = t.referenced_type
    return isinstance(t, c99_ast.Const)


def _has_outer_volatile(t):
    """Mirror of `_has_outer_const` for the Volatile qualifier."""
    while isinstance(t, c99_ast.Const):
        t = t.referenced_type
    return isinstance(t, c99_ast.Volatile)


def _consume_specifiers(items, start):
    """Pull the leading specifier tokens off `items`. Returns
    `(specs, idx)` where `idx` is the position of the first non-
    specifier item (the IDENTIFIER for both var_decl and function_decl,
    by grammar)."""
    specs = []
    i = start
    while (
        i < len(items)
        and hasattr(items[i], "type")
        and items[i].type in _SPECIFIER_TOKEN_TYPES
    ):
        specs.append(items[i])
        i += 1
    return specs, i


# Recognized `__attribute__` spec shapes. Each entry maps the spec
# name to either None (bare-identifier form, no argument expected) or
# a frozenset of valid string-literal arguments (arg-form, must match
# one). The attribute_spec transformer checks against this table.
#
# `zp_abi`           — bare-identifier; opts a function into the
#                       leaf-ZP-ABI calling convention. Valid only as
#                       a function-level prefix.
# `reg("A"|"X"|"Y")` — arg-form; pins a parameter / local / return
#                       value to the named 6502 register. Valid as
#                       function-level prefix (return slot),
#                       parameter postfix (arg-passing slot), or
#                       init-declarator postfix (local-binding slot).
_KNOWN_ATTRIBUTES: dict[str, frozenset[str] | None] = {
    "zp_abi": None,
    "reg": frozenset({"A", "X", "Y"}),
}


@dataclass
class _AttributeClause:
    """Sentinel wrapper around a parsed `__attribute__((...))` clause.
    Holds the list of `(name, arg_or_None)` specs the clause carries.
    A dedicated class (rather than a bare list) lets
    `_consume_attribute_clause` distinguish it from `parameter_type_list`
    results (also lists) sharing item positions."""
    specs: list[tuple[str, str | None]]


def _consume_attribute_clause(items, start):
    """If `items[start]` is a transformed attribute_clause result
    (an `_AttributeClause`), return `(specs, start+1)`. Otherwise the
    optional `attribute_clause?` in the grammar didn't fire and we
    return `(None, start)`. `specs` is a list of `(name, arg_or_None)`
    tuples — the validator helpers (`_extract_function_attributes`,
    `_extract_register_class`) split it into the relevant fields."""
    if start < len(items) and isinstance(items[start], _AttributeClause):
        return items[start].specs, start + 1
    return None, start


def _validate_register_name(name: str) -> str:
    """Check `name` is one of "A"/"X"/"Y" and return it normalized.
    Used by every site that consumes a `reg(...)` arg."""
    if name not in _KNOWN_ATTRIBUTES["reg"]:
        raise ParserError(
            f"`__attribute__((reg({name!r})))`: register name must "
            f"be one of {sorted(_KNOWN_ATTRIBUTES['reg'])}; got "
            f"{name!r}"
        )
    return name


def _extract_function_attributes(specs, where: str):
    """Split a list of `(name, arg_or_None)` specs into the
    `(abi_annotation, return_register)` pair carried on a
    function_decl. `where` is a short label ("function declaration" /
    "extern function declaration") used in error messages.

    Rejects any spec name that isn't `zp_abi` or `reg`, any duplicate
    spec, any `zp_abi` carrying an argument, and any `reg` whose arg
    isn't a valid register name. Returns the two fields (each None if
    not present in the clause)."""
    abi_annotation = None
    return_register = None
    seen: set[str] = set()
    for name, arg in specs:
        if name not in _KNOWN_ATTRIBUTES:
            raise ParserError(
                f"unknown attribute name {name!r} on {where}; expected "
                f"one of {sorted(_KNOWN_ATTRIBUTES)}"
            )
        if name in seen:
            raise ParserError(
                f"duplicate `__attribute__(({name}...))` on {where}"
            )
        seen.add(name)
        if name == "zp_abi":
            if arg is not None:
                raise ParserError(
                    "`__attribute__((zp_abi))` doesn't take an "
                    "argument"
                )
            abi_annotation = "zp_abi"
        elif name == "reg":
            if arg is None:
                raise ParserError(
                    "`__attribute__((reg(...)))` requires a "
                    "string-literal register-name argument"
                )
            return_register = _validate_register_name(arg)
    return abi_annotation, return_register


def _extract_register_class(specs, where: str) -> str | None:
    """Pull a single `reg("...")` spec out of a postfix clause used in
    parameter or init-declarator position. `where` is a label for
    error messages. `zp_abi` isn't valid in this position; neither is
    any other unknown name. Returns the register name or None if the
    clause was empty."""
    register = None
    seen: set[str] = set()
    for name, arg in specs:
        if name not in _KNOWN_ATTRIBUTES:
            raise ParserError(
                f"unknown attribute name {name!r} on {where}; expected "
                f"one of {sorted(_KNOWN_ATTRIBUTES)}"
            )
        if name in seen:
            raise ParserError(
                f"duplicate `__attribute__(({name}...))` on {where}"
            )
        seen.add(name)
        if name == "zp_abi":
            raise ParserError(
                f"`__attribute__((zp_abi))` isn't valid on {where} "
                f"— it's a function-level attribute only"
            )
        if name == "reg":
            if arg is None:
                raise ParserError(
                    "`__attribute__((reg(...)))` requires a "
                    "string-literal register-name argument"
                )
            register = _validate_register_name(arg)
    return register


def _consume_pragma_clause(items, start):
    """If `items[start]` is a transformed pragma_clause result (a
    plain string — the canonical annotation name, e.g. "unroll"),
    return `(annotation, start+1)`. Otherwise the optional
    `pragma_clause?` in the grammar didn't fire, return `(None,
    start)`. Same Token-vs-str distinction as
    `_consume_attribute_clause`."""
    if (
        start < len(items)
        and isinstance(items[start], str)
        and not hasattr(items[start], "type")
    ):
        return items[start], start + 1
    return None, start


# Integer constant typing per C99 §6.4.4.1 paragraph 5: "the type of
# an integer constant is the first of the corresponding list in which
# its value can be represented." c6502 models six integer types —
# int / long / long long / unsigned int / unsigned long / unsigned
# long long — corresponding 1:1 with the six c99_ast `const`
# variants.
#
# Type ranges (literals are non-negative — unary minus comes from an
# operator, applied later):
#     int                 0..32767                       ConstInt
#     long                0..2147483647                  ConstLong
#     long long           0..9223372036854775807         ConstLongLong
#     unsigned int        0..65535                       ConstUInt
#     unsigned long       0..4294967295                  ConstULong
#     unsigned long long  0..18446744073709551615        ConstULongLong
_INT_MAX = 32767
_LONG_MAX = 2147483647
_LONG_LONG_MAX = 9223372036854775807
_UINT_MAX = 65535
_ULONG_MAX = 4294967295
_ULONG_LONG_MAX = 18446744073709551615

# Per-(token-kind, base) candidate-type list. Each entry is a tuple of
# (max_value, c99_ast Const class) — pick the first whose max accepts
# the literal's value. Bases: "decimal" for plain decimal literals,
# "hex_oct" for hex (0x...) and octal (0...) literals. The C99 table
# distinguishes the two for the unsuffixed / L / LL cases; the U-only
# and U+L / U+LL suffix cases share one list across bases.
#
# Token-kind splits: LONG_INTEGER and ULONG_INTEGER each cover both
# the single-L and double-L forms because the lexer doesn't split LL
# out from L; `_const_for_token` reads `has_ll` to pick the right
# row. For `LONG_INTEGER + has_ll` (i.e. plain `LL`) we fall through
# to the "long long" lists; for `ULONG_INTEGER + has_ll` (i.e. `ULL`
# in any letter order) to the "unsigned long long" lists.
_INT = c99_ast.ConstInt
_LONG = c99_ast.ConstLong
_LONG_LONG = c99_ast.ConstLongLong
_UINT = c99_ast.ConstUInt
_ULONG = c99_ast.ConstULong
_ULONG_LONG = c99_ast.ConstULongLong

_CANDIDATES = {
    # No suffix: int → long → long long (decimal); add unsigned
    # rungs at each width on hex/octal per C99.
    ("INTEGER_CONSTANT", "decimal"): [
        (_INT_MAX, _INT),
        (_LONG_MAX, _LONG),
        (_LONG_LONG_MAX, _LONG_LONG),
    ],
    ("INTEGER_CONSTANT", "hex_oct"): [
        (_INT_MAX, _INT), (_UINT_MAX, _UINT),
        (_LONG_MAX, _LONG), (_ULONG_MAX, _ULONG),
        (_LONG_LONG_MAX, _LONG_LONG), (_ULONG_LONG_MAX, _ULONG_LONG),
    ],
    # `L` suffix: long → long long (decimal); add unsigned rungs at
    # each width on hex/octal.
    ("LONG_INTEGER",    "decimal"): [
        (_LONG_MAX, _LONG),
        (_LONG_LONG_MAX, _LONG_LONG),
    ],
    ("LONG_INTEGER",    "hex_oct"): [
        (_LONG_MAX, _LONG), (_ULONG_MAX, _ULONG),
        (_LONG_LONG_MAX, _LONG_LONG), (_ULONG_LONG_MAX, _ULONG_LONG),
    ],
    # `LL` suffix: long long (decimal); long long → unsigned long
    # long (hex/octal).
    ("LONG_LONG_INTEGER",  "decimal"): [(_LONG_LONG_MAX, _LONG_LONG)],
    ("LONG_LONG_INTEGER",  "hex_oct"): [
        (_LONG_LONG_MAX, _LONG_LONG), (_ULONG_LONG_MAX, _ULONG_LONG),
    ],
    # `U` suffix: unsigned int → unsigned long → unsigned long long.
    # (Same list across bases — C99 doesn't split unsigned-only
    # forms by base.)
    ("UINT_INTEGER",    "decimal"): [
        (_UINT_MAX, _UINT), (_ULONG_MAX, _ULONG),
        (_ULONG_LONG_MAX, _ULONG_LONG),
    ],
    ("UINT_INTEGER",    "hex_oct"): [
        (_UINT_MAX, _UINT), (_ULONG_MAX, _ULONG),
        (_ULONG_LONG_MAX, _ULONG_LONG),
    ],
    # `UL` suffix (any letter order): unsigned long → unsigned
    # long long.
    ("ULONG_INTEGER",   "decimal"): [
        (_ULONG_MAX, _ULONG), (_ULONG_LONG_MAX, _ULONG_LONG),
    ],
    ("ULONG_INTEGER",   "hex_oct"): [
        (_ULONG_MAX, _ULONG), (_ULONG_LONG_MAX, _ULONG_LONG),
    ],
    # `ULL` suffix (any letter order): unsigned long long.
    ("ULONG_LONG_INTEGER", "decimal"): [(_ULONG_LONG_MAX, _ULONG_LONG)],
    ("ULONG_LONG_INTEGER", "hex_oct"): [(_ULONG_LONG_MAX, _ULONG_LONG)],
}


def _parse_integer_token(text):
    """Strip the C99 integer-constant suffix off `text` and return
    `(value, base, has_ll)`:
      * `value` — the non-negative integer (literals are always
        non-negative; negation is a separate unary-minus operator).
      * `base` — `"hex_oct"` for hex (`0x...`) and octal (`0...`)
        literals, `"decimal"` otherwise. The two bases share suffix
        rules but differ in their unsuffixed / L-suffixed type lists
        (C99 §6.4.4.1 table).
      * `has_ll` — True if the suffix contains `LL` or `ll`. Used to
        pick the long-long candidate row for LONG_INTEGER /
        ULONG_INTEGER tokens.
    """
    i = len(text)
    while i > 0 and text[i - 1] in "uUlL":
        i -= 1
    digits, suffix = text[:i], text[i:]
    has_ll = "LL" in suffix or "ll" in suffix
    if digits.startswith(("0x", "0X")):
        base = "hex_oct"
    elif len(digits) > 1 and digits.startswith("0"):
        base = "hex_oct"
    else:
        base = "decimal"
    return int(digits, 0), base, has_ll


# Single-character C99 escape sequences (§6.4.4.4 paragraph 4 / §5.2.2).
# Numeric escapes (\xNN, \NNN) are handled separately in `_decode_escape`.
_SIMPLE_ESCAPE_VALUES = {
    "'":  0x27,
    '"':  0x22,
    "?":  0x3F,
    "\\": 0x5C,
    "a":  0x07,
    "b":  0x08,
    "f":  0x0C,
    "n":  0x0A,
    "r":  0x0D,
    "t":  0x09,
    "v":  0x0B,
}


def _decode_escapes(body: str) -> bytes:
    """Decode a C99 character / string-literal body — that is, the
    raw text BETWEEN the surrounding `'` or `"` quotes, with the
    quotes already stripped — into the byte sequence the literal
    represents. Returns a `bytes` object whose values are 0..255.

    Handles every escape form the c99.lark grammar accepts:
      * `\\<simple>` for the simple escapes in `_SIMPLE_ESCAPE_VALUES`
      * `\\NNN` for octal escapes (1-3 digits per §6.4.4.4)
      * `\\xHH...` for hex escapes (one or more hex digits per
        §6.4.4.4 — c6502 truncates to the low 8 bits for char
        targets, since wider char isn't modelled)
    `\\u` / `\\U` escapes lex but raise here — c6502 has no wide-
    char model. Plain unescaped bytes pass through unchanged
    (the lexer already restricted them to printable ASCII minus
    the quote / backslash characters).
    """
    out = bytearray()
    i = 0
    n = len(body)
    while i < n:
        c = body[i]
        if c != "\\":
            out.append(ord(c) & 0xFF)
            i += 1
            continue
        # Escape: at least one character follows (the lexer
        # ensures every `\\` is part of a recognized escape).
        i += 1
        if i >= n:
            raise ParserError(
                f"trailing backslash in literal body {body!r}"
            )
        e = body[i]
        if e in _SIMPLE_ESCAPE_VALUES:
            out.append(_SIMPLE_ESCAPE_VALUES[e])
            i += 1
            continue
        if e == "x":
            # `\xH+` — one or more hex digits per §6.4.4.4.
            i += 1
            j = i
            while j < n and body[j] in "0123456789abcdefABCDEF":
                j += 1
            if j == i:
                raise ParserError(
                    f"empty \\x escape in literal body {body!r}"
                )
            value = int(body[i:j], 16) & 0xFF
            out.append(value)
            i = j
            continue
        if e in "01234567":
            # Octal escape: 1-3 octal digits per §6.4.4.4.
            j = i
            while j < min(i + 3, n) and body[j] in "01234567":
                j += 1
            value = int(body[i:j], 8) & 0xFF
            out.append(value)
            i = j
            continue
        if e in "uU":
            raise ParserError(
                "Unicode escapes (\\u / \\U) are not supported "
                "(c6502 has no wide-char model)"
            )
        raise ParserError(f"unrecognized escape \\{e} in literal")
    return bytes(out)


def _decode_char_constant(token) -> int:
    """Map a CHAR_CONSTANT token (text like `'a'`, `'\\n'`,
    `'\\x41'`) to the integer value of the contained character.
    Per the user's choice the result rides as a `ConstInt` rather
    than a typed `ConstChar` — the type checker promotes char-typed
    contexts itself, and `'a'` is conventionally an `int` in C
    anyway (§6.4.4.4.10: "An integer character constant has type
    int")."""
    text = str(token)
    # Strip the optional leading L (wide-char prefix; c6502 doesn't
    # model wide chars, so we accept the lex but treat as narrow).
    if text.startswith("L"):
        text = text[1:]
    # Strip surrounding single quotes.
    body = text[1:-1]
    decoded = _decode_escapes(body)
    if len(decoded) != 1:
        # Multi-character constants (e.g. `'ab'`) lex but their
        # value is implementation-defined per §6.4.4.4.10. c6502
        # rejects rather than picking an arbitrary encoding.
        raise ParserError(
            f"multi-character character constant {token!s} is not "
            f"supported"
        )
    return decoded[0]


def _decode_string_literal(token) -> bytes:
    """Map a single STRING_LITERAL token to its byte sequence
    (excluding the null terminator — that's added during static-
    storage layout, not at parse time, so adjacent literals can
    concatenate cleanly: `"ab" "cd"` → "abcd" before adding `\\0`)."""
    text = str(token)
    if text.startswith("L"):
        text = text[1:]
    body = text[1:-1]
    return _decode_escapes(body)


def _const_for_token(token):
    """Map a Lark constant token to a c99_ast `Type_const` node.
    Integer literals follow C99 §6.4.4.1 paragraph 5 (first variant
    in the per-(suffix, base) candidate list whose range fits the
    value); floating literals follow C99 §6.4.4.2 (suffix uniquely
    determines the type — no value-fitting rule); character
    literals lex via CHAR_CONSTANT and ride as a `ConstInt` carrying
    the byte value (§6.4.4.4.10)."""
    text = str(token)
    if token.type == "CHAR_CONSTANT":
        return c99_ast.ConstInt(value=_decode_char_constant(token))
    if token.type in ("DOUBLE_CONSTANT", "FLOAT_CONSTANT"):
        # Strip a trailing `f`/`F` before parsing — fp_arith doesn't
        # know about C suffixes.
        body = text[:-1] if token.type == "FLOAT_CONSTANT" else text
        if body.startswith(("0x", "0X")):
            # Hex floats (`0x1.0p3`) lex but neither Python nor
            # numpy parses the C hex-float syntax; rejected here
            # until we wire up a manual conversion.
            raise ParserError(
                f"hex floating literal {text!r} is not supported"
            )
        # Convert the source string directly to an IEEE 754 bit
        # pattern via fp_arith — no Python float intermediary, so
        # single-precision narrowing happens with correct
        # round-to-nearest-even at the source-text boundary.
        if token.type == "DOUBLE_CONSTANT":
            return c99_ast.ConstDouble(
                bits=fp_arith.double_string_to_bits(body),
            )
        return c99_ast.ConstFloat(
            bits=fp_arith.single_string_to_bits(body),
        )
    if token.type == "LONG_DOUBLE_CONSTANT":
        raise ParserError(
            f"`long double` is not supported (literal {text!r})"
        )
    value, base, has_ll = _parse_integer_token(text)
    # `has_ll` upgrades a LONG_INTEGER / ULONG_INTEGER token to its
    # long-long counterpart at dispatch time — the lexer doesn't
    # split LL out from L (see `_LSUFFIX` in c99.lark), so the
    # `_CANDIDATES` table keys on the wider name and we route here.
    kind = token.type
    if has_ll:
        if kind == "LONG_INTEGER":
            kind = "LONG_LONG_INTEGER"
        elif kind == "ULONG_INTEGER":
            kind = "ULONG_LONG_INTEGER"
    for max_value, cls in _CANDIDATES[(kind, base)]:
        if value <= max_value:
            return cls(value=value)
    raise ParserError(
        f"integer constant {text!r} doesn't fit any supported type "
        f"(value > 2^32 - 1)"
    )


# ---------------------------------------------------------------------------
# Declarator walking
# ---------------------------------------------------------------------------
#
# C99 declarators (§6.7.5) wrap a base type with a chain of modifiers
# — pointers, array bounds, function signatures — and name an
# identifier in the middle. The `_apply_declarator` helper walks a
# raw `declarator` parse Tree from the OUTER form inward, threading
# the base type and accumulating wrappers per the §6.7.5 precedence
# rule: postfix array / function suffixes bind tighter than a prefix
# `*`, so `int *foo(int)` is "function returning int*" not "pointer
# to function returning int".
#
# We don't add Lark transformer methods for `declarator` /
# `direct_declarator` / `pointer` — the walk needs the structural
# info (recursive nesting, suffix vs. prefix) and a single helper
# that owns the algorithm is clearer than splitting it across
# transformer methods.

def _is_token(x, ttype: str) -> bool:
    return hasattr(x, "type") and x.type == ttype


def _apply_declarator(decl_tree, base_type):
    """Walk a `declarator: pointer? direct_declarator` parse tree.

    Returns `(name, composed_type, outer_param_names,
    outer_param_registers)`:
      name              — identifier the declarator names.
      composed_type     — full c99_ast type for that name.
      outer_param_names — list[str | None] of parameter names from
                          the OUTERMOST direct_declarator's function
                          suffix (or None if the declarator has no
                          such suffix).
      outer_param_registers — parallel list[str | None] of per-
                          parameter `__attribute__((reg("...")))`
                          register names (None when the parameter
                          carried no postfix attribute). Same length
                          and ordering as outer_param_names. Also
                          None if the declarator has no outermost
                          function suffix.

    Caller checks composed_type to decide whether these are real
    function params: `composed_type is FunType` → they're the
    function's params; anything else (e.g. `Pointer(FunType)` for a
    function-pointer variable) → noise that belongs to the pointee,
    not the variable itself.
    """
    children = decl_tree.children
    if len(children) == 2:
        # `pointer direct_declarator` — pointer wraps the base type
        # before the inner direct_declarator threads its suffixes
        # through. Per §6.7.5 the pointer modifier applies AFTER
        # the postfix suffixes (the `*` is "outer" lexically but
        # "inner" in the type composition).
        pointer_tree, direct_tree = children
        base_type = _wrap_pointers(pointer_tree, base_type)
    else:
        # `direct_declarator` — pointer? matched empty.
        (direct_tree,) = children
    return _apply_direct_declarator(direct_tree, base_type)


def _wrap_pointers(pointer_tree, base_type):
    """`pointer: STAR type_qualifier* pointer?`. Walks the nested
    `pointer` chain (innermost-first in the parse tree, outermost-
    last). Each level wraps `base_type` in `Pointer(...)` and applies
    that level's `const` / `volatile` qualifiers via
    `_apply_qualifiers` — e.g. `int * const` becomes
    `Const(Pointer(Int))`, `volatile int *` becomes
    `Pointer(Volatile(Int))`, `int * volatile` becomes
    `Volatile(Pointer(Int))`. `restrict` is silently dropped."""
    # Collect the per-level qualifier lists, innermost first. The
    # parse tree's outermost `pointer` is the FIRST modifier, but we
    # apply modifiers innermost-first as we wrap, so we walk the
    # chain and reverse when applying.
    levels: list[list] = []
    cur = pointer_tree
    while True:
        # Each `pointer` Tree's direct children are: STAR token
        # (always first), then a mix of type_qualifier tokens (after
        # the type_qualifier transformer inlined them to bare CONST/
        # VOLATILE/RESTRICT tokens), and at most one nested `pointer`
        # sub-Tree.
        quals: list = []
        inner = None
        for c in cur.children:
            if hasattr(c, "type") and c.type in _TYPE_QUALIFIER_TOKEN_TYPES:
                quals.append(c)
            elif hasattr(c, "data") and c.data == "pointer":
                inner = c
        levels.append(quals)
        if inner is None:
            break
        cur = inner
    # Apply innermost-first: levels[-1] is the deepest `*`, applied
    # to the base type first.
    for quals in reversed(levels):
        base_type = c99_ast.Pointer(referenced_type=base_type)
        base_type = _apply_qualifiers(base_type, quals)
    return base_type


def _apply_direct_declarator(dd_tree, base_type):
    """Walk a `direct_declarator` Tree. Returns
    `(name, composed_type, outer_param_names,
    outer_param_registers)` — same shape as `_apply_declarator`. The
    outer-vs-inner distinction follows Lark's parse tree: the
    OUTERMOST direct_declarator is the FIRST one we recurse into
    (its suffix has already been parsed at parse time), and inner
    direct_declarators are deeper recursions."""
    children = dd_tree.children
    # 1) IDENTIFIER alone — base case.
    if len(children) == 1 and _is_token(children[0], "IDENTIFIER"):
        return str(children[0]), base_type, None, None
    # 2) `LPAREN declarator RPAREN` — recurse into the inner
    #    declarator with the same base_type.
    if (
        len(children) == 3
        and _is_token(children[0], "LPAREN")
        and _is_token(children[2], "RPAREN")
        and hasattr(children[1], "data")
        and children[1].data == "declarator"
    ):
        return _apply_declarator(children[1], base_type)
    inner_dd = children[0]
    # 3) Array suffix — `direct_declarator LBRACKET array_suffix RBRACKET`.
    #    Walked outer-first: the outermost suffix in source order is
    #    the rightmost `[N]`, and `int a[3][4]` ends up as
    #    Array(Array(Int, 4), 3) (a is array of 3 arrays of 4 ints).
    #    The grammar accepts four `array_suffix` shapes (plain /
    #    static / qualifiers-then-static / unspecified-VLA), but
    #    c6502 only supports the plain form with a constant integer
    #    size — no `[]`, no `static`, no qualifiers, no VLA `*`.
    if (
        len(children) == 4
        and _is_token(children[1], "LBRACKET")
        and _is_token(children[3], "RBRACKET")
    ):
        size = _array_size_from_suffix(children[2])
        # Wrap base_type in an Array; recurse with the wrapped type
        # so any further suffixes (deeper in inner_dd) compose on top.
        # size == 0 is the "incomplete array" sentinel from `[]`; the
        # type checker enforces that it only appears at the outermost
        # type of an `extern` declaration.
        new_base = c99_ast.Array(element_type=base_type, size=size)
        return _apply_direct_declarator(inner_dd, new_base)
    # 4) Function suffix — `inner_dd LPAREN body? RPAREN` where body
    #    is a parameter_type_list, VOID, an identifier_list, or
    #    empty (K&R `f()`).
    middle = children[2:-1]  # drop LPAREN at [1] and RPAREN at [-1]
    if not middle:
        # `f()` — K&R-style empty list. Treat as no params.
        param_pairs = []
    elif len(middle) == 1 and _is_token(middle[0], "VOID"):
        # `(void)` — explicit empty params.
        param_pairs = []
    elif len(middle) == 1 and isinstance(middle[0], list):
        # parameter_type_list transformer returns a list of
        # (name_or_None, type) tuples — by the time this helper
        # runs, Lark has already transformed the subtree.
        param_pairs = middle[0]
    elif len(middle) == 1 and hasattr(middle[0], "data"):
        if middle[0].data == "identifier_list":
            raise NotImplementedError(
                "K&R-style identifier-list functions aren't supported"
            )
        raise AssertionError(
            f"unexpected direct_declarator function-form child: {middle[0]!r}"
        )
    else:
        raise AssertionError(
            f"unexpected direct_declarator function-form children: {children!r}"
        )
    # C99 §6.7.5.3.1: a function declarator shall not specify a
    # return type that is a function type or an array type. Catch
    # this at composition time so e.g. `int foo(void)(void)` and
    # `int foo(void)[3]` are rejected at parse rather than producing
    # an unrepresentable FunType(ret=FunType/Array).
    _check_function_return_type(base_type)
    param_types = [t for (_n, t, _r) in param_pairs]
    new_base = c99_ast.FunType(params=param_types, ret=base_type)
    name, composed, _inner_outer_params, _inner_outer_regs = (
        _apply_direct_declarator(inner_dd, new_base)
    )
    # Our suffix is the OUTERMOST one for this declarator — propagate
    # our own param names and per-param register annotations up.
    # (Inner outer-params, if any, are from a deeper recursion and
    # represent inner function-types — they belong to the type, not
    # to this declarator's name.)
    param_names = [n for (n, _t, _r) in param_pairs]
    param_registers = [r for (_n, _t, r) in param_pairs]
    return name, composed, param_names, param_registers


def _check_function_return_type(ret_type):
    """C99 §6.7.5.3.1: a function declarator's return type can't be
    a function type or an array type. Used by both
    `_apply_direct_declarator` and `_apply_direct_abstract_declarator`
    just before they wrap a return type in a FunType."""
    if isinstance(ret_type, c99_ast.FunType):
        raise ParserError(
            "function declarator cannot specify a function-typed "
            "return (C99 §6.7.5.3.1)"
        )
    if isinstance(ret_type, c99_ast.Array):
        raise ParserError(
            "function declarator cannot specify an array return type "
            "(C99 §6.7.5.3.1)"
        )


def _adjust_param_type(t):
    """C99 §6.7.5.3.7 array-to-pointer adjustment for parameters:
    "A declaration of a parameter as 'array of type' shall be
    adjusted to 'qualified pointer to type'." So `int foo(int a[3])`
    is exactly equivalent to `int foo(int *a)`. Only the OUTERMOST
    array suffix decays — multi-dim params like `int a[3][4]` adjust
    to `int (*a)[4]` (pointer to inner array), not `int **a`. The
    adjustment happens at parse time so the FunType the type checker
    builds, the function's symbol-table entry, and every Var
    reference to the parameter all see the adjusted (pointer) type
    uniformly.

    C99 also adjusts function parameters of function type to
    "pointer to function" (§6.7.5.3.8), but the c6502 grammar only
    accepts named parameters via the `declarator` form which can't
    produce a bare FunType for a parameter (a function-typed
    declarator always wraps the FunType in a Pointer or names a
    function being declared, not a parameter), so that adjustment
    isn't needed here."""
    if isinstance(t, c99_ast.Array):
        # C99 §6.7.5.2.1: even though a parameter array adjusts to a
        # pointer, the original array declarator must still satisfy
        # the "element type shall be complete" constraint. After
        # adjustment the Array is gone, so the type checker can't see
        # it; reject pre-adjustment here. `void foo[3]` is the
        # canonical example — the adjusted type `void *foo` would be
        # valid on its own, but the declared form is not.
        if isinstance(t.element_type, c99_ast.Void):
            raise ParserError(
                "array parameter declarator has incomplete element "
                "type 'void' (C99 §6.7.5.2.1 — array element must "
                "be a complete object type)"
            )
        return c99_ast.Pointer(referenced_type=t.element_type)
    return t


def _array_size_from_suffix(suffix):
    """Decode an `array_suffix` parse tree into an int size. Returns 0
    for the unsized form `[]` (the incomplete-array sentinel — the
    type checker enforces that it only appears at the outermost type
    of an `extern` declaration). Used by both the named (`int a[3]`)
    and abstract (`int (*)[3]`) declarator paths."""
    if not hasattr(suffix, "data") or suffix.data != "array_size_plain":
        raise NotImplementedError(
            "only plain array sizes are supported "
            f"(got {getattr(suffix, 'data', suffix)!r})"
        )
    if len(suffix.children) == 0:
        return 0
    if len(suffix.children) > 1:
        raise NotImplementedError(
            "type-qualified array sizes aren't supported"
        )
    size_exp = suffix.children[0]
    if not isinstance(size_exp, c99_ast.Constant):
        raise NotImplementedError(
            "array size must be an integer constant literal"
        )
    c = size_exp.const
    if not isinstance(c, (
        c99_ast.ConstInt, c99_ast.ConstLong,
        c99_ast.ConstUInt, c99_ast.ConstULong,
    )):
        raise NotImplementedError(
            "array size must be an integer constant"
        )
    if c.value <= 0:
        raise ParserError("array size must be positive")
    return c.value


def _apply_abstract_declarator(adecl_tree, base_type):
    """Walk an `abstract_declarator` parse tree (used for type-names
    and unnamed parameter declarations) and return the composed
    c99_ast type. The grammar is `pointer | pointer?
    direct_abstract_declarator`. Same composition rule as
    `_apply_declarator`: the pointer prefix wraps the base type FIRST
    (so it ends up as the INNERMOST wrapper), then the direct
    abstract declarator's suffixes wrap around it. So `int *[3]`
    becomes `Array(Pointer(Int), 3)` — array of 3 pointers — while
    `int (*)[3]` becomes `Pointer(Array(Int, 3))` — pointer to array.
    """
    children = adecl_tree.children
    # `pointer` alone — no inner direct_abstract_declarator.
    if (
        len(children) == 1
        and hasattr(children[0], "data")
        and children[0].data == "pointer"
    ):
        return _wrap_pointers(children[0], base_type)
    # `pointer direct_abstract_declarator` — pointer wraps base FIRST,
    # then the dad applies its suffixes around it.
    if (
        len(children) == 2
        and hasattr(children[0], "data")
        and children[0].data == "pointer"
    ):
        base_type = _wrap_pointers(children[0], base_type)
        return _apply_direct_abstract_declarator(children[1], base_type)
    # `direct_abstract_declarator` alone (no leading pointer).
    if (
        len(children) == 1
        and hasattr(children[0], "data")
        and children[0].data == "direct_abstract_declarator"
    ):
        return _apply_direct_abstract_declarator(children[0], base_type)
    raise AssertionError(
        f"unexpected abstract_declarator children: {children!r}"
    )


def _apply_direct_abstract_declarator(dad_tree, base_type):
    """Walk a `direct_abstract_declarator`, applying the OUTERMOST
    suffix in source order first (it's the rightmost child here) and
    threading the composed type through the recursion. Mirrors
    `_apply_direct_declarator` but with no IDENTIFIER base case — the
    leaves are bare suffix forms (`[N]`, `(params)`) or a parenthesised
    inner abstract_declarator."""
    children = dad_tree.children
    # Leaf 1: `LPAREN abstract_declarator RPAREN` — recurse into the
    # inner abstract_declarator with the same base_type (parens just
    # group; no suffix here).
    if (
        len(children) == 3
        and _is_token(children[0], "LPAREN")
        and _is_token(children[2], "RPAREN")
        and hasattr(children[1], "data")
        and children[1].data == "abstract_declarator"
    ):
        return _apply_abstract_declarator(children[1], base_type)
    # Leaf 2: `LBRACKET array_suffix RBRACKET` — bare array suffix.
    if (
        len(children) == 3
        and _is_token(children[0], "LBRACKET")
        and _is_token(children[2], "RBRACKET")
    ):
        size = _array_size_from_suffix(children[1])
        return c99_ast.Array(element_type=base_type, size=size)
    # Leaf 3: `LPAREN parameter_type_list? RPAREN` / `LPAREN VOID
    # RPAREN` — bare function suffix.
    if (
        len(children) >= 2
        and _is_token(children[0], "LPAREN")
        and _is_token(children[-1], "RPAREN")
    ):
        _check_function_return_type(base_type)
        param_pairs = _parse_function_suffix_middle(children[1:-1])
        param_types = [t for (_n, t, _r) in param_pairs]
        return c99_ast.FunType(params=param_types, ret=base_type)
    # Recursive forms — first child is an inner direct_abstract_declarator,
    # followed by a postfix suffix.
    if (
        len(children) >= 3
        and hasattr(children[0], "data")
        and children[0].data == "direct_abstract_declarator"
    ):
        inner_dad = children[0]
        # Recursive array suffix: `inner LBRACKET array_suffix RBRACKET`.
        if (
            len(children) == 4
            and _is_token(children[1], "LBRACKET")
            and _is_token(children[3], "RBRACKET")
        ):
            size = _array_size_from_suffix(children[2])
            new_base = c99_ast.Array(element_type=base_type, size=size)
            return _apply_direct_abstract_declarator(inner_dad, new_base)
        # Recursive function suffix: `inner LPAREN body? RPAREN`.
        if (
            _is_token(children[1], "LPAREN")
            and _is_token(children[-1], "RPAREN")
        ):
            _check_function_return_type(base_type)
            param_pairs = _parse_function_suffix_middle(children[2:-1])
            param_types = [t for (_n, t, _r) in param_pairs]
            new_base = c99_ast.FunType(params=param_types, ret=base_type)
            return _apply_direct_abstract_declarator(inner_dad, new_base)
    raise AssertionError(
        f"unexpected direct_abstract_declarator children: {children!r}"
    )


def _parse_function_suffix_middle(middle):
    """Decode the middle children of a function-suffix declarator
    (between LPAREN and RPAREN) into a list of `(name_or_None, type,
    register_or_None)` triples. Shared between the named and abstract
    walkers; abstract-declarator paths drop the name and register
    fields since unnamed parameters can't carry either."""
    if not middle:
        # `f()` — K&R-style empty list. Treat as no params.
        return []
    if len(middle) == 1 and _is_token(middle[0], "VOID"):
        return []
    if len(middle) == 1 and isinstance(middle[0], list):
        # parameter_type_list transformer returns a list of
        # (name_or_None, type) tuples.
        return middle[0]
    if len(middle) == 1 and hasattr(middle[0], "data"):
        if middle[0].data == "identifier_list":
            raise NotImplementedError(
                "K&R-style identifier-list functions aren't supported"
            )
    raise AssertionError(
        f"unexpected function-suffix middle: {middle!r}"
    )


class _ASTBuilder(Transformer):
    def start(self, items):
        # `start: declaration*` — items is the list of declaration
        # values built by `declaration`. Each value is itself a list
        # of `Type_declaration` nodes (most have one element; an
        # inline `struct foo { ... } x;` produces two — the StructDecl
        # for the body and the VarDecl for `x`). Flatten here.
        flat = []
        for it in items:
            flat.extend(it)
        return c99_ast.Program(declaration=flat)

    @v_args(inline=True)
    def specifier(self, token):
        # `specifier: type_specifier | STATIC | EXTERN`. Either branch
        # contributes a single token (after `type_specifier` unwraps
        # its tree), so this method just passes the token through for
        # var_decl / function_decl to scan.
        return token

    @v_args(inline=True)
    def type_specifier(self, child):
        # `type_specifier: INT | LONG | ... | struct_or_union_specifier`.
        # The simple-keyword alternatives produce a Token; the
        # struct/union alternative produces a `_TagSpecifier`. Inline
        # the single child so the caller (`specifier`, `type_name`,
        # `specifier_qualifier_list`) sees the leaf directly.
        return child

    def type_name(self, items):
        # `type_name: (type_specifier | type_qualifier)+
        # abstract_declarator?` — used in cast expressions and
        # `sizeof (T)`.
        #
        # Each child is either a Token (primitive type-specifier OR
        # qualifier — disambiguated by `.type`), a `_TagSpecifier`
        # (struct/union), or an abstract_declarator parse Tree. We
        # split the leaves into type-specifiers and qualifiers, then
        # handle the optional abstract_declarator separately.
        leaf_items = [
            it for it in items
            if isinstance(it, (Token, _TagSpecifier))
        ]
        type_specs = [
            it for it in leaf_items
            if isinstance(it, _TagSpecifier)
            or (isinstance(it, Token)
                and it.type in _TYPE_SPECIFIER_TOKEN_TYPES)
        ]
        qualifiers = [
            it for it in leaf_items
            if isinstance(it, Token)
            and it.type in _TYPE_QUALIFIER_TOKEN_TYPES
        ]
        # Inline struct/union *definitions* in a type-name (`(struct
        # foo { int a; })x`) are legal C99 but semantically obscure
        # (the tag escapes to the surrounding scope). c6502 rejects
        # them — the surrounding context (cast / sizeof) has no place
        # to thread the resulting StructDecl to.
        for ts in type_specs:
            if isinstance(ts, _TagSpecifier) and ts.body is not None:
                raise ParserError(
                    "struct/union definition isn't permitted inside "
                    "a type-name (cast / sizeof)"
                )
        base = _apply_qualifiers(_resolve_data_type(type_specs), qualifiers)
        if len(leaf_items) == len(items):
            # No abstract_declarator — bare specifier-qualifier list.
            return base
        # Trailing abstract_declarator subtree.
        adecl_tree = items[-1]
        return _apply_abstract_declarator(adecl_tree, base)

    # `struct_or_union_specifier` rule alternatives. Two forms each:
    #   * `STRUCT IDENTIFIER` — bare reference (or forward decl when
    #     the surrounding declaration has no declarator).
    #   * `STRUCT IDENTIFIER LBRACE struct_declaration+ RBRACE` —
    #     defining form. The body is a list of (member_name,
    #     member_type) pairs; the surrounding var_decl /
    #     function_decl / tag_only_decl transformer drains it via
    #     `_drain_inline_struct_bodies` to surface a StructDecl
    #     declaration alongside the consuming declaration.
    def struct_specifier(self, items):
        return self._make_tag_specifier(items, is_union=False)

    def union_specifier(self, items):
        return self._make_tag_specifier(items, is_union=True)

    @staticmethod
    def _make_tag_specifier(items, *, is_union):
        # items: [STRUCT/UNION token, IDENTIFIER token, (LBRACE,
        # member_list_items..., RBRACE)?]
        kw_token = items[0]
        tag = str(items[1])
        body = None
        if len(items) > 2:
            # Body present. Children between LBRACE and RBRACE are
            # the lists returned by `struct_declaration`. Each
            # struct_declaration produces a list of (name, type)
            # pairs (one per declarator on that line).
            body = []
            seen = set()
            for child in items[3:-1]:
                # struct_declaration returns a list of pairs.
                for (mname, mtype) in child:
                    if mname in seen:
                        raise ParserError(
                            f"duplicate member {mname!r} in "
                            f"struct/union {tag!r}"
                        )
                    seen.add(mname)
                    body.append((mname, mtype))
            if not body:
                raise ParserError(
                    f"struct/union {tag!r} body is empty (C99 "
                    f"§6.7.2.1.7 — at least one member required)"
                )
        return _TagSpecifier(
            type=str(kw_token.type),
            tag=tag,
            is_union=is_union,
            body=body,
        )

    def specifier_qualifier_list(self, items):
        # `(type_specifier | type_qualifier)+`. Returns a list of
        # type-specifier leaves and qualifier tokens, intermixed —
        # callers (`struct_declaration`) sort them via
        # `_split_specifier_qualifier_list`.
        return list(items)

    @v_args(inline=True)
    def type_qualifier(self, child):
        # `type_qualifier: CONST | RESTRICT | VOLATILE`. Inline the
        # leaf token so consumers can read its `.type` attribute
        # alongside type_specifier tokens uniformly.
        return child

    def struct_declarator_list(self, items):
        # `declarator (COMMA declarator)*` — strip COMMA tokens; each
        # remaining item is a declarator parse Tree.
        return [it for it in items if not _is_token(it, "COMMA")]

    def struct_declaration(self, items):
        # `specifier_qualifier_list struct_declarator_list SEMICOLON`.
        # Returns a list of (member_name, member_type) tuples.
        leaf_items = items[0]
        decl_trees = items[1]
        # Split type-specifiers from type-qualifiers within the
        # specifier-qualifier list (the `specifier_qualifier_list`
        # transformer keeps them intermixed).
        type_specs = [
            it for it in leaf_items
            if isinstance(it, _TagSpecifier)
            or (isinstance(it, Token)
                and it.type in _TYPE_SPECIFIER_TOKEN_TYPES)
        ]
        qualifiers = [
            it for it in leaf_items
            if isinstance(it, Token)
            and it.type in _TYPE_QUALIFIER_TOKEN_TYPES
        ]
        # Reject inline struct/union definitions inside a member's
        # type-specifier — `struct outer { struct inner { int x; }
        # nested; };` is legal C99 but rarely-used and not modeled
        # here (the inner StructDecl would have nowhere to go in the
        # outer struct's member list).
        for ts in type_specs:
            if isinstance(ts, _TagSpecifier) and ts.body is not None:
                raise ParserError(
                    "nested struct/union definition inside a member "
                    "isn't supported — declare the inner type at "
                    "outer scope first"
                )
        base = _apply_qualifiers(_resolve_data_type(type_specs), qualifiers)
        # Per C99 §6.7.2.1.2 a struct member's declarator yields a
        # type that has to be a complete object type (or, if it's
        # a flexible array member, the very last member). c6502
        # rejects flexible array members and incomplete types here
        # via the type checker; the parser accepts the shape.
        out = []
        for decl_tree in decl_trees:
            name, composed, _outer, _outer_regs = _apply_declarator(
                decl_tree, base,
            )
            if isinstance(composed, c99_ast.FunType):
                raise ParserError(
                    f"struct/union member {name!r} cannot have "
                    f"function type"
                )
            out.append((name, composed))
        return out

    @v_args(inline=True)
    def member_dot(self, operand, _dot, identifier):
        # `postfix_exp DOT IDENTIFIER -> member_dot`. The IDENTIFIER
        # is a member name (own namespace per C99 §6.2.3) — never a
        # variable reference, so it doesn't go through Var.
        return c99_ast.Dot(operand=operand, member=str(identifier))

    @v_args(inline=True)
    def member_arrow(self, operand, _arrow, identifier):
        # `postfix_exp ARROW IDENTIFIER -> member_arrow`. Operand
        # must have pointer-to-struct/union type — the type checker
        # validates this.
        return c99_ast.Arrow(operand=operand, member=str(identifier))

    def parameter_declaration(self, items):
        # `parameter_declaration: specifier+ declarator attribute_clause?
        #                       | specifier+ abstract_declarator?`
        # Returns `(name_or_None, data_type, register_or_None)`. An
        # unnamed parameter — `int foo(int *)` — gives `name=None`
        # and `register=None`. An empty abstract_declarator gives
        # `name=None`, the bare specifier type, and `register=None`.
        # The postfix attribute_clause (when present) is only legal
        # on the named-declarator alternative; abstract-declarator
        # params can't be reg-annotated because there's no name to
        # bind. The clause's specs must be `reg("...")` shape (`zp_abi`
        # is a function-level attribute, rejected here).
        specs = []
        rest = []
        attr_clause: _AttributeClause | None = None
        for it in items:
            if isinstance(it, _AttributeClause):
                attr_clause = it
            elif (
                hasattr(it, "type")
                and it.type in _SPECIFIER_TOKEN_TYPES
            ):
                specs.append(it)
            else:
                rest.append(it)
        # Inline struct/union *definitions* in a parameter declaration
        # are legal C99 but obscure (the tag's scope would be the
        # enclosing function-prototype scope only). c6502 rejects.
        for s in specs:
            if isinstance(s, _TagSpecifier) and s.body is not None:
                raise ParserError(
                    "struct/union definition isn't permitted in a "
                    "parameter declaration"
                )
        base, storage = _split_specifiers(specs)
        # C99 §6.7.5.3.2: "The only storage-class specifier that shall
        # occur in a parameter declaration is register." c6502 doesn't
        # model `register` (the keyword lexes but no parser rule
        # accepts it), so any storage class reaching here is `static`
        # or `extern` and is a constraint violation.
        if storage is not None:
            raise ParserError(
                f"storage class "
                f"{type(storage).__name__.lower()!r} isn't permitted "
                f"on a function parameter (C99 §6.7.5.3.2)"
            )
        register = None
        if attr_clause is not None:
            register = _extract_register_class(
                attr_clause.specs, where="function parameter",
            )
        if not rest:
            if register is not None:
                raise ParserError(
                    "`__attribute__((reg(...)))` requires a named "
                    "parameter — saw it on an abstract-declarator "
                    "parameter"
                )
            return (None, base, None)
        decl_tree = rest[0]
        if decl_tree.data == "declarator":
            name, composed, _outer, _outer_regs = _apply_declarator(
                decl_tree, base,
            )
            return (name, _adjust_param_type(composed), register)
        if decl_tree.data == "abstract_declarator":
            if register is not None:
                raise ParserError(
                    "`__attribute__((reg(...)))` requires a named "
                    "parameter — saw it on an abstract-declarator "
                    "parameter"
                )
            t = _apply_abstract_declarator(decl_tree, base)
            return (None, _adjust_param_type(t), None)
        raise AssertionError(
            f"unexpected parameter_declaration child: {decl_tree.data}"
        )

    def parameter_or_ellipsis(self, items):
        # `parameter_or_ellipsis: parameter_declaration | ELLIPSIS`.
        # parameter_declaration has already been transformed into a
        # tuple; ELLIPSIS comes through as a Token. Reject ELLIPSIS
        # for now — c6502 has no variadic AST node.
        item = items[0]
        if hasattr(item, "type") and item.type == "ELLIPSIS":
            raise ParserError(
                "variadic functions ('...') aren't supported yet"
            )
        return item

    def parameter_type_list(self, items):
        # `parameter_type_list: parameter_declaration
        #                       (COMMA parameter_or_ellipsis)*`
        # — items alternate: tuple, COMMA, tuple, COMMA, ... .
        # Strip COMMAs and return the list of (name_or_None, type)
        # tuples. The transformer for parameter_or_ellipsis already
        # rejected ELLIPSIS, so by here every entry is a real param.
        return [it for it in items if not _is_token(it, "COMMA")]

    def block(self, items):
        # `block: LBRACE block_item* RBRACE`. Non-inline because
        # block_item* expands to a variable number of children. Each
        # block_item value is a list — `stmt_item` returns a
        # one-element list, `decl_item` returns one or more (a single
        # source-level declaration may produce more than one AST
        # declaration when a struct body is defined inline). Flatten.
        flat = []
        for it in items[1:-1]:
            flat.extend(it)
        return c99_ast.Block(block_item=flat)

    # Alternatives of `block_item` — wrap a statement / declaration.
    @v_args(inline=True)
    def stmt_item(self, statement):
        return [c99_ast.S(statement=statement)]

    @v_args(inline=True)
    def decl_item(self, declaration_list):
        # `declaration` already returns a list of `Type_declaration`
        # AST nodes (typically one element; two when a struct body is
        # defined inline alongside an object declaration). Wrap each
        # in `D` so they're block_item-compatible.
        # C99 §6.9.1: function-definitions are an external-declaration
        # form, only legal at file scope. A block_item may carry a
        # function *forward declaration* (FunctionDecl with body=None)
        # but never a definition (body present).
        out = []
        for d in declaration_list:
            if (
                isinstance(d, c99_ast.FunctionDecl)
                and d.function_decl.body is not None
            ):
                raise ParserError(
                    f"function definition for "
                    f"{d.function_decl.name!r} is only legal at "
                    f"file scope; nested function definitions aren't "
                    f"part of standard C (C99 §6.9.1)"
                )
            out.append(c99_ast.D(declaration=d))
        return out

    @v_args(inline=True)
    def declaration(self, child):
        # `declaration: function_decl | var_decl`. Each branch
        # returns a *list* of Type_declaration AST nodes — almost
        # always one element, but a `var_decl` whose specifier
        # contained an inline struct/union *definition*
        # (`struct foo { int a; } x;`) produces two: the StructDecl
        # for the body, then the VarDecl for `x`.
        return child

    def attr_name_only(self, items):
        # `attribute_spec: IDENTIFIER -> attr_name_only`. Returns
        # the bare attribute name, paired with `None` for the
        # missing argument slot, as a `(name, None)` tuple.
        return (items[0].value, None)

    def attr_name_arg(self, items):
        # `attribute_spec: IDENTIFIER LPAREN STRING_LITERAL RPAREN
        # -> attr_name_arg`. items[0] = IDENTIFIER, items[2] =
        # STRING_LITERAL. Strip the surrounding double-quotes from
        # the literal to produce a plain Python string. Returns a
        # `(name, arg)` tuple. Validation of which (name, arg)
        # combinations are legal happens in the caller (the per-
        # context `_extract_*` helpers).
        name = items[0].value
        lit = items[2].value
        # STRING_LITERAL grammar: optional `L` prefix + `"..."`. The
        # `L"..."` wide-char prefix isn't meaningful for an attribute
        # arg — strip it if present.
        if lit.startswith("L"):
            lit = lit[1:]
        if not (lit.startswith('"') and lit.endswith('"')):
            raise ParserError(
                f"expected double-quoted string literal in attribute "
                f"argument; got {items[2].value!r}"
            )
        return (name, lit[1:-1])

    def attribute_clause(self, items):
        # `attribute_clause: ATTRIBUTE LPAREN LPAREN attribute_spec
        # (COMMA attribute_spec)* RPAREN RPAREN`. Drop the punctuation
        # and gather the transformed `(name, arg)` tuples. Wrap the
        # list in `_AttributeClause` so `_consume_attribute_clause`
        # can identify it positionally (other rules also produce
        # lists). The validators in the calling rules (`var_decl` /
        # `function_decl` / `init_declarator` / `parameter_declaration`)
        # decide which attribute names are legal in each context.
        specs: list[tuple[str, str | None]] = [
            it for it in items
            if isinstance(it, tuple)
        ]
        return _AttributeClause(specs=specs)

    # `initializer` rule alternatives. `init_exp` is `assignment_exp`
    # (a single scalar initializer); the inner exp passes through
    # unchanged. `init_list` is `{ initializer (, initializer)* ,? }`
    # — strip the LBRACE / COMMA / RBRACE tokens and wrap the
    # remaining items in InitList. Items are themselves initializer
    # results (an exp or another InitList), supporting nested init
    # for future multi-dim use.
    @v_args(inline=True)
    def init_exp(self, exp):
        return exp

    def init_list(self, items):
        children = [
            it for it in items
            if not (
                hasattr(it, "type")
                and it.type in ("LBRACE", "RBRACE", "COMMA")
            )
        ]
        return c99_ast.InitList(items=children)

    def init_declarator(self, items):
        # `init_declarator: declarator attribute_clause?
        #                    (ASSIGN initializer)?`. Returns a
        # `(declarator_tree, init_or_None, register_class_or_None)`
        # triple so var_decl can iterate the list emitted by
        # `init_declarator (COMMA init_declarator)*`. The postfix
        # attribute_clause's specs must all be `reg("...")` shape —
        # `zp_abi` and unknown names are rejected here (a local
        # variable can't be `zp_abi`). The register name is validated
        # against {"A", "X", "Y"}.
        decl_tree = items[0]
        i = 1
        register_class: str | None = None
        if i < len(items) and isinstance(items[i], _AttributeClause):
            register_class = _extract_register_class(
                items[i].specs, where="object declaration",
            )
            i += 1
        init = None
        if i < len(items) and _is_token(items[i], "ASSIGN"):
            init = items[i + 1]
        return (decl_tree, init, register_class)

    def var_decl(self, items):
        # `var_decl: attribute_clause? specifier+ init_declarator
        # (COMMA init_declarator)* SEMICOLON`. The specifiers give the
        # BASE type; each init_declarator wraps it with pointer /
        # array / function modifiers and names the identifier. If a
        # declarator's composed type is FunType, that init-declarator
        # is a forward function declaration (`int foo(int);`) — rewrap
        # as Type_function_decl with body=None; function decls in this
        # form cannot carry an initializer.
        #
        # Returns a list of `Type_declaration` AST nodes. Inline
        # struct/union bodies in the specifier run (`struct foo {int
        # a;} x;`) come FIRST in the returned list, followed by one
        # node per init-declarator in source order.
        prefix_specs, i = _consume_attribute_clause(items, 0)
        specs, i = _consume_specifiers(items, i)
        struct_decls = _drain_inline_struct_bodies(specs)
        base_type, storage_class = _split_specifiers(specs)
        # Split the prefix attribute clause into the function-decl
        # fields (abi_annotation + return_register). On object decls
        # the prefix clause must be empty; we validate that below per
        # init-declarator (after we know the composed type).
        if prefix_specs is None:
            prefix_abi, prefix_return_reg = None, None
        else:
            prefix_abi, prefix_return_reg = _extract_function_attributes(
                prefix_specs, where="function declaration",
            )
        # Remaining items are init_declarator results (tuples) and
        # COMMA / SEMICOLON tokens. Pick out the tuples in order.
        init_decls = [
            it for it in items[i:]
            if isinstance(it, tuple)
        ]
        tails: list = []
        for decl_tree, init, register_class in init_decls:
            name, composed, outer_param_names, outer_param_regs = (
                _apply_declarator(decl_tree, base_type)
            )
            if isinstance(composed, c99_ast.FunType):
                if init is not None:
                    raise ParserError(
                        "function declaration cannot have an initializer"
                    )
                if register_class is not None:
                    # Postfix `reg(...)` on a function-typed
                    # init-declarator is ambiguous with the prefix
                    # return_register slot, so disallow it. Users
                    # should put the return-register attribute on the
                    # function-level prefix.
                    raise ParserError(
                        f"`__attribute__((reg(...)))` on function "
                        f"declaration {name!r} must appear as a "
                        f"prefix attribute (carrying the return "
                        f"register), not as a postfix attribute"
                    )
                param_names = outer_param_names or []
                param_regs = outer_param_regs or [None] * len(param_names)
                tails.append(c99_ast.FunctionDecl(
                    function_decl=c99_ast.Type_function_decl(
                        name=name,
                        params=param_names,
                        body=None,
                        data_type=composed,
                        storage_class=storage_class,
                        abi_annotation=prefix_abi,
                        return_register=prefix_return_reg,
                        param_registers=[r or "" for r in param_regs],
                    )
                ))
            else:
                if prefix_abi is not None or prefix_return_reg is not None:
                    raise ParserError(
                        f"`__attribute__((...))` is only valid on "
                        f"function declarations; saw it on object "
                        f"declaration `{name}`"
                    )
                tails.append(c99_ast.VarDecl(
                    var_decl=c99_ast.Type_var_decl(
                        name=name,
                        init=init,
                        data_type=composed,
                        storage_class=storage_class,
                        register_class=register_class,
                    )
                ))
        return struct_decls + tails

    def tag_only_decl(self, items):
        # `var_decl: attribute_clause? specifier+ SEMICOLON ->
        # tag_only_decl`. Used for struct/union forward declarations
        # (`struct foo;`) and struct/union definitions with no
        # declarator (`struct foo { int a; };`). Storage classes
        # aren't legal here — `static struct foo;` is meaningless.
        # Attribute clauses are also not legal here — there's no
        # function declaration to attach them to. Returns a list of
        # Type_declaration nodes (the StructDecl for the body, plus
        # any *nested* struct definitions encountered while parsing
        # the body).
        prefix_specs, i = _consume_attribute_clause(items, 0)
        if prefix_specs is not None:
            raise ParserError(
                "`__attribute__((...))` is only valid on function "
                "declarations; saw it on a struct/union declaration "
                "without a declarator"
            )
        specs, i = _consume_specifiers(items, i)
        # The semicolon is the last item; nothing else.
        if i != len(items) - 1:
            raise AssertionError(
                f"unexpected children in tag_only_decl: {items!r}"
            )
        struct_decls = _drain_inline_struct_bodies(specs)
        # Reject any storage-class specifier on a tag-only decl.
        if any(
            getattr(s, "type", None) in ("STATIC", "EXTERN")
            for s in specs
        ):
            raise ParserError(
                "storage-class specifier isn't permitted on a "
                "struct/union declaration without a declarator"
            )
        # The non-storage specifiers should compose to a single
        # Structure or Union type. Any other shape (e.g. `int;`,
        # `void;`) is meaningless and rejected per C99 §6.7.2 —
        # a declaration must declare at least a tag, a member of an
        # enumeration, or an object/function.
        non_storage = [s for s in specs if not (
            getattr(s, "type", None) in ("STATIC", "EXTERN")
        )]
        if not non_storage:
            raise ParserError("empty declaration")
        ts = _resolve_data_type(non_storage)
        if not isinstance(ts, (c99_ast.Structure, c99_ast.Union)):
            raise ParserError(
                "declaration does not declare anything (a tag, a "
                "member of an enumeration, or an object/function)"
            )
        # If the body had a definition and we already drained it
        # into struct_decls, we're done. Otherwise this is a forward
        # declaration — emit a StructDecl with empty members.
        if struct_decls and struct_decls[-1].struct_decl.tag == ts.tag:
            # The drained struct decl IS the body; nothing further.
            return struct_decls
        # Forward declaration: `struct foo;`. Append an empty-body
        # StructDecl.
        forward = c99_ast.StructDecl(struct_decl=c99_ast.Type_struct_decl(
            tag=ts.tag,
            is_union=isinstance(ts, c99_ast.Union),
            members=[],
        ))
        return struct_decls + [forward]

    def function_decl(self, items):
        # `function_decl: attribute_clause? specifier+ declarator
        # block` — function *definition* path (body present). Forward
        # declarations land in `var_decl` above (the grammar lets
        # `int foo(int);` parse as var_decl with a function-typed
        # declarator, since both rules share the `specifier+ declarator`
        # prefix and the trailing `(ASSIGN exp)? SEMICOLON` matches
        # the no-init, no-body case).
        #
        # The declarator must compose to a FunType — its outermost
        # direct_declarator form has to be the function-suffix shape.
        # Anything else (a plain identifier, a function-pointer
        # variable, ...) means the source isn't a function definition.
        # Returns a list of Type_declaration AST nodes (typically one
        # FunctionDecl, plus any inline struct definitions that
        # appeared in the return-type specifier).
        prefix_specs, i = _consume_attribute_clause(items, 0)
        if prefix_specs is None:
            prefix_abi, prefix_return_reg = None, None
        else:
            prefix_abi, prefix_return_reg = _extract_function_attributes(
                prefix_specs, where="function definition",
            )
        specs, i = _consume_specifiers(items, i)
        struct_decls = _drain_inline_struct_bodies(specs)
        base_type, storage_class = _split_specifiers(specs)
        decl_tree = items[i]
        name, composed, outer_param_names, outer_param_regs = (
            _apply_declarator(decl_tree, base_type)
        )
        block = items[i + 1]
        if not isinstance(composed, c99_ast.FunType):
            raise ParserError(
                f"function definition's declarator must compose to a "
                f"function type; got {type(composed).__name__}"
            )
        param_names = outer_param_names or []
        param_regs = outer_param_regs or [None] * len(param_names)
        tail = c99_ast.FunctionDecl(function_decl=c99_ast.Type_function_decl(
            name=name,
            params=param_names,
            body=block,
            data_type=composed,
            storage_class=storage_class,
            abi_annotation=prefix_abi,
            return_register=prefix_return_reg,
            param_registers=[r or "" for r in param_regs],
        ))
        return struct_decls + [tail]

    # Alternatives of `statement` — each named in c99.lark.
    def return_stmt(self, items):
        # `RETURN exp? SEMICOLON`. With `exp` present, items has 3
        # children; with the bare `return;` form, items has 2. The
        # latter is only legal in a void-returning function — type
        # checking enforces that constraint.
        if len(items) == 2:
            return c99_ast.Return(exp=None)
        return c99_ast.Return(exp=items[1])

    @v_args(inline=True)
    def expression_stmt(self, exp, _semi):
        return c99_ast.Expression(exp=exp)

    # `if (exp) stmt` (4 children) or `if (exp) stmt else stmt` (6
    # children). The else-branch is variable, so non-inline.
    def if_stmt(self, items):
        condition = items[2]
        then_clause = items[4]
        else_clause = items[6] if len(items) == 7 else None
        return c99_ast.IfStmt(
            condition=condition,
            then_clause=then_clause,
            else_clause=else_clause,
        )

    @v_args(inline=True)
    def goto_stmt(self, _goto, identifier, _semi):
        return c99_ast.Goto(label=str(identifier))

    @v_args(inline=True)
    def labeled_stmt(self, identifier, _colon, stmt):
        return c99_ast.LabeledStmt(label=str(identifier), statement=stmt)

    @v_args(inline=True)
    def compound_stmt(self, block):
        # `{ ... }` as a statement. `block` has already been built
        # into a `Block` by the `block` transformer; wrap it in a
        # `Compound` so it fits the `statement` sum.
        return c99_ast.Compound(block=block)

    @v_args(inline=True)
    def null_stmt(self, _semi):
        return c99_ast.Null()

    # Loop and jump statements. Loop labels are minted by the
    # loop_labeling pass that runs after identifier_resolution; the
    # parser leaves them as empty strings.
    @v_args(inline=True)
    def break_stmt(self, _break, _semi):
        return c99_ast.BreakStmt(label="")

    @v_args(inline=True)
    def continue_stmt(self, _continue, _semi):
        return c99_ast.ContinueStmt(label="")

    @v_args(inline=True)
    def while_stmt(self, _while, _lp, cond, _rp, body):
        return c99_ast.WhileStmt(condition=cond, body=body, label="")

    @v_args(inline=True)
    def do_stmt(self, _do, body, _while, _lp, cond, _rp, _semi):
        return c99_ast.DoWhileStmt(body=body, condition=cond, label="")

    # `for_init: var_decl | exp? SEMICOLON`. The var_decl alternative
    # already consumes its own SEMICOLON, so it arrives as the only
    # child — a `Type_var_decl` for object decls, or a
    # `Type_function_decl` if the var_decl rule's declarator composed
    # to a FunType (forward function declaration, which shares the
    # `specifier+ declarator ;` prefix with var_decl). Function
    # declarations aren't legal in for-init per C99 §6.8.5.3 ("only
    # objects having storage class auto or register"); reject those
    # explicitly so the function decl doesn't silently get dropped
    # and the loop parsed with an empty init clause. The exp-or-empty
    # alternative carries an explicit SEMICOLON token: zero or one
    # preceding exp child, then SEMICOLON.
    def for_init(self, items):
        if len(items) == 1:
            child = items[0]
            # var_decl returns a list of Type_declaration AST nodes:
            # one per init-declarator, with any inline struct/union
            # body for the specifier prepended. For for-init, none
            # of them may be an inline struct body (would smuggle the
            # tag into for-loop scope), none may be a function decl,
            # and none may carry a storage class (C99 §6.8.5.3 only
            # permits auto / register).
            if isinstance(child, list):
                if any(
                    isinstance(d, c99_ast.StructDecl)
                    for d in child
                ):
                    raise ParserError(
                        "struct/union definition isn't permitted in a "
                        "for-loop initializer"
                    )
                if not child:
                    raise AssertionError(
                        f"empty decl list in for_init: {child!r}"
                    )
                vds: list[c99_ast.Type_var_decl] = []
                for inner in child:
                    if isinstance(inner, c99_ast.FunctionDecl):
                        raise ParserError(
                            "function declarations aren't permitted "
                            "in a for-loop initializer (C99 §6.8.5.3)"
                        )
                    vd = inner.var_decl
                    if vd.storage_class is not None:
                        raise ParserError(
                            f"storage class "
                            f"{type(vd.storage_class).__name__.lower()!r} "
                            f"isn't permitted on a for-loop initializer "
                            f"(C99 §6.8.5.3 — only auto / register)"
                        )
                    vds.append(vd)
                return c99_ast.InitDecl(var_decls=vds)
            # Bare SEMICOLON — empty for-init clause.
            return c99_ast.InitExp(exp=None)
        # exp + SEMICOLON.
        return c99_ast.InitExp(exp=items[0])

    # `switch (exp) statement` — the controlling expression is a full
    # `exp` (per C99 §6.8.4.2.1 — wider than the §6.6 constraints
    # imposed on case labels). The labeling pass mints `.switch@<N>`
    # and stamps it onto `label`; the same pass walks the body to
    # collect `cases` / `default_label`. The type checker fills in
    # `promoted_type` after integer promotion of the control.
    @v_args(inline=True)
    def switch_stmt(self, _switch, _lp, control, _rp, body):
        return c99_ast.SwitchStmt(
            control=control,
            body=body,
            label="",
            cases=[],
            default_label=None,
            promoted_type=None,
        )

    # `case constant_exp : statement`. The grammar's `constant_exp`
    # non-terminal is just a one-child wrapper around `conditional_exp`
    # (C99 §6.6 — same syntax, additional semantic constraints). The
    # constness check lives in the labeling / type-checking passes,
    # not here.
    @v_args(inline=True)
    def case_stmt(self, _case, value, _colon, body):
        return c99_ast.CaseStmt(value=value, body=body, label="")

    @v_args(inline=True)
    def default_stmt(self, _default, _colon, body):
        return c99_ast.DefaultStmt(body=body, label="")

    @v_args(inline=True)
    def constant_exp(self, exp):
        # Pass-through wrapper. The grammar non-terminal exists so call
        # sites are self-documenting and share a §6.6 validator; the
        # AST shape is just the inner exp.
        return exp

    # `for (for_init exp? ; exp?) statement` — for_init contributes one
    # child that already swallowed the first SEMICOLON; the middle SEMI
    # between the condition and post_clause is in our items list. Each
    # of condition / post_clause is independently optional, so we scan
    # for the SEMICOLON to know which side each remaining child is on.
    def pragma_unroll(self, items):
        # `pragma_clause: PRAGMA_UNROLL -> pragma_unroll`. The token's
        # value is the literal sentinel; we hand back the canonical
        # annotation name that ForStmt.unroll_annotation expects.
        return "unroll"

    def for_stmt(self, items):
        # items: [pragma?, FOR, LPAREN, for_init, condition?, SEMICOLON,
        #         post_clause?, RPAREN, statement]
        unroll_annotation, idx = _consume_pragma_clause(items, 0)
        # idx now points at FOR. The rest of the parse is unchanged.
        init = items[idx + 2]
        body = items[-1]
        middle = items[idx + 3:-2]
        semi_idx = next(
            i for i, c in enumerate(middle)
            if hasattr(c, "type") and c.type == "SEMICOLON"
        )
        condition = middle[0] if semi_idx > 0 else None
        post_clause = (
            middle[semi_idx + 1] if semi_idx + 1 < len(middle) else None
        )
        return c99_ast.ForStmt(
            init=init,
            condition=condition,
            post_clause=post_clause,
            body=body,
            label="",
            unroll_annotation=unroll_annotation,
        )

    # Alternatives of `exp` — each named in c99.lark.
    @v_args(inline=True)
    def const(self, token):
        # `const: INTEGER_CONSTANT | LONG_INTEGER | UINT_INTEGER
        # | ULONG_INTEGER | DOUBLE_CONSTANT | FLOAT_CONSTANT
        # | LONG_DOUBLE_CONSTANT`. The lex split is by suffix presence
        # only. For integers, `_const_for_token` consults the C99
        # §6.4.4.1 type list (keyed by token-kind + base) and picks
        # the first variant whose range fits. For FP, the suffix
        # uniquely determines the type (§6.4.4.2). Long-double and
        # long-long both raise here.
        return _const_for_token(token)

    @v_args(inline=True)
    def constant(self, c):
        # `?atom: const -> constant`. The `const` handler already
        # built a ConstInt or ConstLong; wrap it in the Constant
        # expression node.
        return c99_ast.Constant(const=c)

    @v_args(inline=True)
    def identifier(self, token):
        return c99_ast.Var(name=str(token))

    @v_args(inline=True)
    def cast(self, _lp, target_type, _rp, exp):
        # `cast_exp: LPAREN type_name RPAREN cast_exp -> cast`. The
        # `type_name` transformer already resolved the type-specifier
        # run to a c99_ast type node; reject the shapes C99 §6.5.4.2
        # forbids for casts (array / function types) here, where the
        # type's role IS as a cast target. (Sizeof's other use of
        # `type_name` accepts both shapes, so the rejection lives at
        # the cast site rather than inside `type_name`.)
        if isinstance(target_type, c99_ast.Array):
            raise ParserError(
                "cannot cast to an array type (C99 §6.5.4.2 — cast "
                "target must be scalar or void)"
            )
        if isinstance(target_type, c99_ast.FunType):
            raise ParserError(
                "cannot cast to a function type (C99 §6.5.4.2 — cast "
                "target must be scalar or void)"
            )
        return c99_ast.Cast(target_type=target_type, exp=exp)

    @v_args(inline=True)
    def assignment(self, lval, _assign, rval):
        return c99_ast.Assignment(lval=lval, rval=rval)

    @v_args(inline=True)
    def comma_expression(self, left, _comma, right):
        return c99_ast.Comma(left=left, right=right)

    @v_args(inline=True)
    def compound_assign(self, lval, op_token, rval):
        # `lval OP= rval` builds a `CompoundAssignment` AST node
        # rather than desugaring to `lval = lval OP rval`. The
        # explicit node lets c99_to_tac evaluate the lval's address
        # ONCE before the read-modify-write — necessary for
        # Subscript / Dereference / Dot / Arrow lvals whose address
        # computation has side effects (e.g. `arr[i++] += 1`,
        # `(*p++)++`, `ptr++[idx++] *= 3`), where re-evaluating the
        # lval would fire those side effects twice.
        op_cls = _COMPOUND_ASSIGN_OPS[op_token.type]
        return c99_ast.CompoundAssignment(op=op_cls(), lval=lval, rval=rval)

    @v_args(inline=True)
    def unary(self, op, inner):
        return c99_ast.Unary(op=op, exp=inner)

    # `*e` and `&e` build their own AST nodes (not Unary variants).
    # The leading STAR / AMP token is discarded — the AST node itself
    # encodes the operator. The `data_type` field is left unset for
    # the type checker to fill in (Dereference yields the pointee
    # type; AddressOf yields a Pointer to the operand's type).
    @v_args(inline=True)
    def dereference(self, _star, inner):
        return c99_ast.Dereference(exp=inner)

    @v_args(inline=True)
    def address_of(self, _amp, inner):
        return c99_ast.AddressOf(exp=inner)

    # Prefix `++a` / `--a` build their own AST node analogous to
    # Postfix, instead of desugaring to `a = a ± 1`. A direct node
    # lets c99_to_tac evaluate the operand's address ONCE before
    # the read-modify-write — necessary for Subscript / Dereference
    # operands like `++arr[--i]`, where re-evaluating the operand
    # would fire `--i`'s side effect twice. The result of a prefix
    # increment is the *new* value (vs. Postfix which returns the
    # *old* value), so the two nodes need separate semantics.
    @v_args(inline=True)
    def pre_increment(self, _op, operand):
        return c99_ast.Prefix(op=c99_ast.Increment(), operand=operand)

    @v_args(inline=True)
    def pre_decrement(self, _op, operand):
        return c99_ast.Prefix(op=c99_ast.Decrement(), operand=operand)

    # `sizeof e` and `sizeof (T)` (C99 §6.5.3.4). The two grammar
    # alternatives feed separate AST variants — the type-name form is
    # a leaf, the expression form has a sub-expression that the rest
    # of the pipeline walks for identifier resolution / type
    # population. Per §6.5.3.4.2 the operand of sizeof is NOT
    # evaluated, so c99_to_tac will fold the node to a constant
    # without lowering the inner expression.
    @v_args(inline=True)
    def sizeof_exp(self, _sizeof, inner):
        return c99_ast.SizeOfExp(exp=inner)

    @v_args(inline=True)
    def sizeof_type(self, _sizeof, _lparen, type_name, _rparen):
        return c99_ast.SizeOfType(target_type=type_name)

    # Postfix `a++` / `a--` keep their own AST node because they have
    # to return the *old* value of the operand while also mutating
    # it. The lvalue check (operand must be a `Var`) lives in
    # identifier_resolution alongside the Assignment check.
    @v_args(inline=True)
    def post_increment(self, operand, _op):
        return c99_ast.Postfix(op=c99_ast.Increment(), operand=operand)

    @v_args(inline=True)
    def post_decrement(self, operand, _op):
        return c99_ast.Postfix(op=c99_ast.Decrement(), operand=operand)

    # Postfix subscript `e1[e2]` per C99 §6.5.2.1. Children are
    # `[postfix_exp, LBRACKET, exp, RBRACKET]` — keep the array and
    # index expressions; bracket tokens are framing.
    @v_args(inline=True)
    def subscript(self, array, _lb, index, _rb):
        return c99_ast.Subscript(array=array, index=index)

    @v_args(inline=True)
    def paren(self, _lp, inner, _rp):
        return inner

    # `STRING_LITERAL+` — one or more adjacent string-literal
    # tokens. Per C99 §6.4.5.5 adjacent string literals are
    # concatenated at translation phase 6 (compile-time), so we do
    # the join here at parse time and emit a single `String` AST
    # node carrying the concatenated raw bytes (each character a
    # value 0..255 in a Python str — c6502 doesn't model wide
    # chars). The string's null terminator is added later, during
    # static-storage layout — keeping it out of the parsed value
    # makes concatenation trivial.
    def str_literal(self, items):
        joined = bytearray()
        for token in items:
            joined.extend(_decode_string_literal(token))
        # Encode bytes as a Python str using Latin-1 so each byte
        # becomes one code point — round-trips byte-perfectly when
        # the type checker / lifter / codegen iterate the str.
        return c99_ast.String(str=bytes(joined).decode("latin-1"))

    # `IDENTIFIER LPAREN arg_list? RPAREN` — a function call. With
    # no arguments, items is [IDENT, LPAREN, RPAREN] (3); with an
    # arg_list, items is [IDENT, LPAREN, [arg, ...], RPAREN] (4).
    def function_call(self, items):
        name = str(items[0])
        args = items[2] if len(items) == 4 else []
        return c99_ast.FunctionCall(name=name, args=args)

    # `LPAREN exp RPAREN LPAREN arg_list? RPAREN` — function call
    # through a parenthesised callee. Children:
    #   [LPAREN, callee_exp, RPAREN, LPAREN, arg_list?, RPAREN]
    # — 5 if no args, 6 with an arg_list. Per C99 §6.3.2.1.4 the
    # callee auto-decays to a function pointer in any expression
    # context except the operands of `&` / `sizeof`, so several
    # surface forms boil down to "call through this name":
    #   `(fp)(args)`   — Var(fp): direct or indirect, depending on
    #                    fp's symbol-table type. Same as `fp(args)`.
    #   `(*fp)(args)`  — Dereference(Var(fp)): the dereference is
    #                    elided per the auto-decay rule. Same as
    #                    `fp(args)`.
    #   `(&foo)(args)` — AddressOf(Var(foo)): pointer to foo,
    #                    immediately called through. Same as
    #                    `foo(args)`.
    # Anything else (`(arr[i])(args)`, `(f())(args)`, ...) needs
    # the AST's name field replaced with a general callee
    # expression — separate scope; reject for now.
    def indirect_function_call(self, items):
        callee = items[1]
        args = items[4] if len(items) == 6 else []
        # Drill through the auto-decay shapes.
        if isinstance(callee, c99_ast.Var):
            name = callee.name
        elif (
            isinstance(callee, (c99_ast.Dereference, c99_ast.AddressOf))
            and isinstance(callee.exp, c99_ast.Var)
        ):
            name = callee.exp.name
        else:
            raise ParserError(
                "indirect call through complex expression not "
                "supported — callee must be a name, `*name`, or "
                "`&name`"
            )
        return c99_ast.FunctionCall(name=name, args=args)

    # `arg_list: exp (COMMA exp)*` — every other child is an exp;
    # the COMMA tokens are interleaved.
    def arg_list(self, items):
        return [items[i] for i in range(0, len(items), 2)]

    @v_args(inline=True)
    def conditional(self, condition, _q, true_clause, _c, false_clause):
        return c99_ast.Conditional(
            condition=condition,
            true_clause=true_clause,
            false_clause=false_clause,
        )

    # Binary alternatives of `exp` — tokens discarded, build a Binary node.
    @v_args(inline=True)
    def multiply(self, left, _star, right):
        return c99_ast.Binary(op=c99_ast.Multiply(), left=left, right=right)

    @v_args(inline=True)
    def divide(self, left, _slash, right):
        return c99_ast.Binary(op=c99_ast.Divide(), left=left, right=right)

    @v_args(inline=True)
    def modulo(self, left, _percent, right):
        return c99_ast.Binary(op=c99_ast.Modulo(), left=left, right=right)

    @v_args(inline=True)
    def add(self, left, _plus, right):
        return c99_ast.Binary(op=c99_ast.Add(), left=left, right=right)

    @v_args(inline=True)
    def subtract(self, left, _minus, right):
        return c99_ast.Binary(op=c99_ast.Subtract(), left=left, right=right)

    @v_args(inline=True)
    def bitwise_and(self, left, _amp, right):
        return c99_ast.Binary(op=c99_ast.BitwiseAnd(), left=left, right=right)

    @v_args(inline=True)
    def bitwise_or(self, left, _pipe, right):
        return c99_ast.Binary(op=c99_ast.BitwiseOr(), left=left, right=right)

    @v_args(inline=True)
    def bitwise_xor(self, left, _caret, right):
        return c99_ast.Binary(op=c99_ast.BitwiseXor(), left=left, right=right)

    @v_args(inline=True)
    def left_shift(self, left, _lshift, right):
        return c99_ast.Binary(op=c99_ast.LeftShift(), left=left, right=right)

    @v_args(inline=True)
    def right_shift(self, left, _rshift, right):
        return c99_ast.Binary(op=c99_ast.RightShift(), left=left, right=right)

    @v_args(inline=True)
    def equal(self, left, _eq, right):
        return c99_ast.Binary(op=c99_ast.Equal(), left=left, right=right)

    @v_args(inline=True)
    def not_equal(self, left, _ne, right):
        return c99_ast.Binary(op=c99_ast.NotEqual(), left=left, right=right)

    @v_args(inline=True)
    def less_than(self, left, _lt, right):
        return c99_ast.Binary(op=c99_ast.LessThan(), left=left, right=right)

    @v_args(inline=True)
    def greater_than(self, left, _gt, right):
        return c99_ast.Binary(op=c99_ast.GreaterThan(), left=left, right=right)

    @v_args(inline=True)
    def less_or_equal(self, left, _le, right):
        return c99_ast.Binary(op=c99_ast.LessOrEqual(), left=left, right=right)

    @v_args(inline=True)
    def greater_or_equal(self, left, _ge, right):
        return c99_ast.Binary(op=c99_ast.GreaterOrEqual(), left=left, right=right)

    @v_args(inline=True)
    def logical_and(self, left, _andand, right):
        return c99_ast.Binary(op=c99_ast.LogicalAnd(), left=left, right=right)

    @v_args(inline=True)
    def logical_or(self, left, _oror, right):
        return c99_ast.Binary(op=c99_ast.LogicalOr(), left=left, right=right)

    # Alternatives of `unop` — tokens discarded, just produce the AST op.
    @v_args(inline=True)
    def negate(self, _minus):
        return c99_ast.Negate()

    @v_args(inline=True)
    def complement(self, _tilde):
        return c99_ast.Complement()

    @v_args(inline=True)
    def logical_not(self, _bang):
        return c99_ast.LogicalNot()


def _drain_inline_struct_bodies(specs):
    """Walk a specifier list and produce StructDecl AST nodes for any
    struct/union specifier that carried an inline body. Mutates each
    `_TagSpecifier` in place to clear its body (so the same specifier
    list can be re-used to compute the type via `_resolve_data_type`).

    A specifier is a list of Tokens and `_TagSpecifier` wrappers.
    Bodies appear at most once per tag in the list; when a body is
    drained the resulting StructDecl is appended to the output.
    """
    out = []
    for s in specs:
        if isinstance(s, _TagSpecifier) and s.body is not None:
            members = [
                c99_ast.Type_member_decl(name=n, data_type=t)
                for (n, t) in s.body
            ]
            sd = c99_ast.Type_struct_decl(
                tag=s.tag,
                is_union=s.is_union,
                members=members,
            )
            out.append(c99_ast.StructDecl(struct_decl=sd))
            s.body = None  # consumed; further references treat as forward-ref
    return out


_BUILDER = _ASTBuilder()


def parse(source: str) -> c99_ast.Type_program:
    tree = _LARK.parse(source, start="start")
    try:
        return _BUILDER.transform(tree)
    except VisitError as e:
        # Lark wraps any exception raised by a transformer method in
        # `VisitError`. We raise our own ParserError from
        # `_split_specifiers`, so unwrap to give the caller a clean
        # ParserError instead of forcing it to dig through `.orig_exc`.
        if isinstance(e.orig_exc, ParserError):
            raise e.orig_exc
        raise
