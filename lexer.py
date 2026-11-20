"""C99 lexer. Thin wrapper over a Lark grammar (c99.lark)."""

from __future__ import annotations

import re
from dataclasses import dataclass
from enum import Enum
from pathlib import Path
from typing import Iterator

from lark import Lark
from lark.exceptions import UnexpectedCharacters, UnexpectedInput

import pp_numbers
from lark_cache import open_lalr


KEYWORDS = frozenset({
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while",
    "_Bool", "_Complex", "_Imaginary",
    # C23 §6.4.1 adds these three. `_Bool` stays, as an alternative
    # spelling of `bool` that C23 makes obsolescent.
    "bool", "true", "false",
    # C23 §6.4.1 also adds `nullptr` (§6.4.4.6). Its type, `nullptr_t`,
    # isn't a keyword. <stddef.h> declares it as C23's does, as
    # `typeof_unqual(nullptr)`.
    "nullptr",
    # C23's bit-precise integer types (§6.7.3.1).
    "_BitInt",
    # C23's storage-class specifier for named constants (§6.7.2).
    "constexpr",
    # C23's `thread_local` (§6.7.2) and C11's spelling of it, which
    # the parser rejects, since c6502 has no threads.
    "thread_local", "_Thread_local",
    # C23's typeof operators (§6.7.3.6).
    "typeof", "typeof_unqual",
})

# Punctuator string → Lark terminal name in c99.lark. Keep in sync with
# the per-symbol terminal definitions in the grammar.
_SYMBOL_TERMINAL = {
    "...": "ELLIPSIS",
    "::": "COLONCOLON",
    "<<=": "LSHIFT_ASSIGN",
    ">>=": "RSHIFT_ASSIGN",
    "->": "ARROW",
    "++": "PLUSPLUS",
    "--": "MINUSMINUS",
    "<<": "LSHIFT",
    ">>": "RSHIFT",
    "<=": "LE",
    ">=": "GE",
    "==": "EQ",
    "!=": "NE",
    "&&": "ANDAND",
    "||": "OROR",
    "*=": "STAR_ASSIGN",
    "/=": "SLASH_ASSIGN",
    "%=": "PERCENT_ASSIGN",
    "+=": "PLUS_ASSIGN",
    "-=": "MINUS_ASSIGN",
    "&=": "AMP_ASSIGN",
    "^=": "CARET_ASSIGN",
    "|=": "PIPE_ASSIGN",
    "##": "HASHHASH",
    "[": "LBRACKET",
    "]": "RBRACKET",
    "(": "LPAREN",
    ")": "RPAREN",
    "{": "LBRACE",
    "}": "RBRACE",
    ".": "DOT",
    "&": "AMP",
    "*": "STAR",
    "+": "PLUS",
    "-": "MINUS",
    "~": "TILDE",
    "!": "BANG",
    "/": "SLASH",
    "%": "PERCENT",
    "<": "LT",
    ">": "GT",
    "^": "CARET",
    "|": "PIPE",
    "?": "QUESTION",
    ":": "COLON",
    ";": "SEMICOLON",
    "=": "ASSIGN",
    ",": "COMMA",
    "#": "HASH",
}

# Ordered longest-first for backwards compatibility with earlier tests.
SYMBOLS = tuple(_SYMBOL_TERMINAL.keys())


class TokenKind(Enum):
    KEYWORD = "keyword"
    IDENTIFIER = "identifier"
    SYMBOL = "symbol"
    CONSTANT = "constant"
    STRING_LITERAL = "string-literal"


@dataclass(frozen=True)
class Token:
    kind: TokenKind
    value: str
    line: int
    col: int


class LexError(Exception):
    def __init__(self, msg: str, line: int, col: int) -> None:
        super().__init__(f"{line}:{col}: {msg}")
        self.line = line
        self.col = col


_GRAMMAR_PATH = Path(__file__).parent / "c99.lark"
_LARK: Lark | None = None


def _lark() -> Lark:
    """Returns the Lark parser whose lexer `tokenize` runs, built (or
    loaded from its cache file, lark_cache.py) on the first call, since
    only `tokenize` uses it and most processes never call it."""
    global _LARK
    if _LARK is None:
        _LARK = open_lalr(
            _GRAMMAR_PATH,
            lexer="basic",
            # `lex_only` is a secondary start that references every
            # terminal, so Lark doesn't drop unused ones while the real
            # parser rules are still partial. `.lex()` then sees the
            # full terminal set.
            start=["start", "lex_only"],
        )
    return _LARK


# Each keyword has its own terminal in c99.lark. Terminal names are the
# keyword uppercased with leading AND trailing underscores stripped
# (e.g. `_Complex` → `COMPLEX`). `_Bool` would then collide with
# `bool`, so its terminal is `ALT_BOOL`. Derived here so changes to
# KEYWORDS flow through automatically; c99.lark still has to be edited
# by hand to match.
_TERMINAL_EXCEPTIONS = {
    "_Bool": "ALT_BOOL", "_Thread_local": "ALT_THREAD_LOCAL",
}


def _keyword_terminal(kw: str) -> str:
    return _TERMINAL_EXCEPTIONS.get(kw, kw.strip("_").upper())


_TERMINAL_TO_KIND = {_keyword_terminal(kw): TokenKind.KEYWORD for kw in KEYWORDS}
_TERMINAL_TO_KIND.update({name: TokenKind.SYMBOL for name in _SYMBOL_TERMINAL.values()})
_TERMINAL_TO_KIND.update({
    # C23's `[[`, which opens an attribute (a regex, since whitespace may
    # separate the two brackets).
    "ATTR_OPEN": TokenKind.SYMBOL,
    "IDENTIFIER": TokenKind.IDENTIFIER,
    "INTEGER_CONSTANT": TokenKind.CONSTANT,
    "LONG_INTEGER": TokenKind.CONSTANT,
    "UINT_INTEGER": TokenKind.CONSTANT,
    "ULONG_INTEGER": TokenKind.CONSTANT,
    "BITINT_INTEGER": TokenKind.CONSTANT,
    "UBITINT_INTEGER": TokenKind.CONSTANT,
    "DOUBLE_CONSTANT": TokenKind.CONSTANT,
    "FLOAT_CONSTANT": TokenKind.CONSTANT,
    "LONG_DOUBLE_CONSTANT": TokenKind.CONSTANT,
    "CHAR_CONSTANT": TokenKind.CONSTANT,
    "STRING_LITERAL": TokenKind.STRING_LITERAL,
})


# The terminals of numeric constants, which `check_number` looks at.
NUMBER_TERMINALS = frozenset({
    "INTEGER_CONSTANT", "LONG_INTEGER", "UINT_INTEGER", "ULONG_INTEGER",
    "BITINT_INTEGER", "UBITINT_INTEGER",
    "DOUBLE_CONSTANT", "FLOAT_CONSTANT", "LONG_DOUBLE_CONSTANT",
    "INVALID_NUMBER",
})


def check_number(source: str, lt) -> None:
    """Raises LexError for the numeric Lark token `lt` in `source`
    when the pp-number it begins (C23 §6.4.8) uses a C23 spelling
    wrongly: a digit separator that isn't between two digits, a binary
    constant with a digit other than 0 and 1 or no digits, or a `wb`
    or `uwb` constant too wide for any `_BitInt`. The message comes from
    pp_numbers.py. Any other number is left to the terminals and to
    the check in `tokenize`, as before. `tokenize` and the parser's
    lexer both call this.

    A `'` right after a number is a separator that isn't followed by a
    digit. The pp-number ends before it, as in `1'` or `1''000`, and
    the `'` would start a character constant, which can't follow a
    number in a valid program."""
    start = lt.start_pos
    spelling = pp_numbers.pp_number_at(source, start)
    end = start + len(spelling)
    if end < len(source) and source[end] == "'":
        shown = re.match(r"[0-9A-Za-z_.']*", source[start:]).group(0)
        raise LexError(
            f"a digit separator has to be between two digits, and the "
            f"one after {spelling!r} in {shown!r} isn't followed by a "
            f"digit",
            lt.line, lt.column,
        )
    if ("'" not in spelling and spelling[:2] not in ("0b", "0B")
            and lt.type not in ("BITINT_INTEGER", "UBITINT_INTEGER")):
        return
    try:
        pp_numbers.check(spelling)
        if not pp_numbers.is_floating(spelling):
            constant = pp_numbers.parse_integer(spelling)
            if constant.bit_precise:
                pp_numbers.bit_precise_width(constant, spelling)
    except pp_numbers.NumberError as e:
        raise LexError(str(e), lt.line, lt.column) from None
    if len(spelling) != len(str(lt)):
        raise LexError(
            f"invalid pp-number {spelling!r}", lt.line, lt.column,
        )


def tokenize(source: str) -> Iterator[Token]:
    prev: Token | None = None
    try:
        for lt in _lark().lex(source):
            if lt.type in NUMBER_TERMINALS:
                check_number(source, lt)
            if lt.type == "INVALID_NUMBER":
                raise LexError(
                    f"malformed numeric token {str(lt)!r}",
                    lt.line, lt.column,
                )
            kind = _TERMINAL_TO_KIND.get(lt.type)
            if kind is None:
                raise LexError(
                    f"unrecognized token type {lt.type}",
                    lt.line, lt.column,
                )
            tok = Token(kind=kind, value=str(lt), line=lt.line, col=lt.column)
            # C99 pp-numbers are greedy: a numeric constant can't abut an
            # identifier (e.g. `1foo`, `42ua`, `1e2e3`). If it does, the whole
            # thing would be a single invalid pp-number at phase 7. We detect
            # the split tokens and reject.
            if (prev is not None
                    and prev.kind == TokenKind.CONSTANT
                    and tok.kind == TokenKind.IDENTIFIER
                    and prev.line == tok.line
                    and prev.col + len(prev.value) == tok.col):
                raise LexError(
                    f"invalid pp-number: {prev.value}{tok.value!r} "
                    "(numeric constant may not abut an identifier)",
                    prev.line, prev.col,
                )
            yield tok
            prev = tok
    except UnexpectedCharacters as e:
        ch = source[e.pos_in_stream] if e.pos_in_stream < len(source) else ""
        raise LexError(f"unexpected character {ch!r}", e.line, e.column) from None
    except UnexpectedInput as e:
        raise LexError(
            str(e), getattr(e, "line", 0), getattr(e, "column", 0),
        ) from None


class Lexer:
    """Backwards-compatible thin wrapper."""

    def __init__(self, source: str) -> None:
        self.source = source

    def tokens(self) -> Iterator[Token]:
        return tokenize(self.source)
