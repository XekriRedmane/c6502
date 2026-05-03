"""C99 lexer. Thin wrapper over a Lark grammar (c99.lark)."""

from __future__ import annotations

from dataclasses import dataclass
from enum import Enum
from pathlib import Path
from typing import Iterator

from lark import Lark
from lark.exceptions import UnexpectedCharacters, UnexpectedInput


KEYWORDS = frozenset({
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while",
    "_Bool", "_Complex", "_Imaginary",
    "__attribute__",
})

# Punctuator string → Lark terminal name in c99.lark. Keep in sync with
# the per-symbol terminal definitions in the grammar.
_SYMBOL_TERMINAL = {
    "...": "ELLIPSIS",
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
_LARK = Lark.open(
    str(_GRAMMAR_PATH),
    parser="lalr",
    lexer="basic",
    # `lex_only` is a secondary start that references every terminal, so
    # Lark doesn't drop unused ones while the real parser rules are still
    # partial. `.lex()` then sees the full terminal set.
    start=["start", "lex_only"],
)


# Each keyword has its own terminal in c99.lark. Terminal names are the
# keyword uppercased with leading AND trailing underscores stripped
# (e.g. `_Bool` → `BOOL`, `__attribute__` → `ATTRIBUTE`). Derived here
# so changes to KEYWORDS flow through automatically; c99.lark still
# has to be edited by hand to match.
def _keyword_terminal(kw: str) -> str:
    return kw.strip("_").upper()


_TERMINAL_TO_KIND = {_keyword_terminal(kw): TokenKind.KEYWORD for kw in KEYWORDS}
_TERMINAL_TO_KIND.update({name: TokenKind.SYMBOL for name in _SYMBOL_TERMINAL.values()})
_TERMINAL_TO_KIND.update({
    "IDENTIFIER": TokenKind.IDENTIFIER,
    "INTEGER_CONSTANT": TokenKind.CONSTANT,
    "LONG_INTEGER": TokenKind.CONSTANT,
    "UINT_INTEGER": TokenKind.CONSTANT,
    "ULONG_INTEGER": TokenKind.CONSTANT,
    "DOUBLE_CONSTANT": TokenKind.CONSTANT,
    "FLOAT_CONSTANT": TokenKind.CONSTANT,
    "LONG_DOUBLE_CONSTANT": TokenKind.CONSTANT,
    "CHAR_CONSTANT": TokenKind.CONSTANT,
    "STRING_LITERAL": TokenKind.STRING_LITERAL,
})


def tokenize(source: str) -> Iterator[Token]:
    prev: Token | None = None
    try:
        for lt in _LARK.lex(source):
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
