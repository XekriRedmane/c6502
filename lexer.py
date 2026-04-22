"""C99 lexer. Thin wrapper over a Lark grammar (c99.lark)."""

from __future__ import annotations

import sys
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
})

# Ordered longest-first for backwards compatibility with earlier tests.
SYMBOLS = (
    "...", "<<=", ">>=",
    "->", "++", "--", "<<", ">>", "<=", ">=", "==", "!=", "&&", "||",
    "*=", "/=", "%=", "+=", "-=", "&=", "^=", "|=", "##",
    "[", "]", "(", ")", "{", "}", ".", "&", "*", "+", "-", "~", "!",
    "/", "%", "<", ">", "^", "|", "?", ":", ";", "=", ",", "#",
)


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
_LARK = Lark.open(str(_GRAMMAR_PATH), parser="lalr", lexer="basic", start="start")


_TERMINAL_TO_KIND = {
    "INTEGER_CONSTANT": TokenKind.CONSTANT,
    "FLOATING_CONSTANT": TokenKind.CONSTANT,
    "CHARACTER_CONSTANT": TokenKind.CONSTANT,
    "STRING_LITERAL": TokenKind.STRING_LITERAL,
    "SYMBOL": TokenKind.SYMBOL,
}


def tokenize(source: str) -> Iterator[Token]:
    try:
        for lt in _LARK.lex(source):
            if lt.type == "INVALID_NUMBER":
                raise LexError(
                    f"malformed numeric token {str(lt)!r}",
                    lt.line, lt.column,
                )
            if lt.type == "IDENTIFIER":
                kind = (TokenKind.KEYWORD if str(lt) in KEYWORDS
                        else TokenKind.IDENTIFIER)
            else:
                kind = _TERMINAL_TO_KIND.get(lt.type)
                if kind is None:
                    raise LexError(
                        f"unrecognized token type {lt.type}",
                        lt.line, lt.column,
                    )
            yield Token(kind=kind, value=str(lt), line=lt.line, col=lt.column)
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


def main(argv: list[str]) -> int:
    if len(argv) != 2:
        print("usage: lexer.py <file>", file=sys.stderr)
        return 2
    with open(argv[1], "r", encoding="utf-8") as f:
        source = f.read()
    try:
        for tok in tokenize(source):
            print(f"{tok.line}:{tok.col}\t{tok.kind.value}\t{tok.value}")
    except LexError as e:
        print(e, file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
