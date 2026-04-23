# c6502

A C99 compiler written in Python.

## Regenerating `c99_ast.py` from `c99.asdl`

`c99_ast.py` is the AST module, generated from `c99.asdl` by `asdl.py`. After
editing `c99.asdl`, regenerate with:

```sh
uv run python asdl.py c99.asdl c99_ast.py
```

The generator emits one `@dataclass` per type. Sum-type bases are named
`Type_<name>` to avoid collisions with Python builtins (`int`, etc.);
constructor classes keep their ASDL names. Fields use `int`, `str`, `list[...]`,
`T | None` depending on the primitive / optional / sequence markers in the
ASDL source.

## Using the lexer

`lexer.py` tokenizes C99 source using the Lark grammar in `c99.lark`. The
`*_grammar.txt` files are reference documentation for the spec grammars that
`c99.lark` implements — they are not parsed by any tool.

As a script:

```sh
uv run python lexer.py <source.c>    # read from a file
uv run python lexer.py -              # read from stdin
```

prints one token per line as `line:col  kind  value`.

As an API:

```python
from lexer import tokenize, TokenKind

for tok in tokenize(source):
    print(tok.kind, tok.value)
```

`TokenKind` is one of `KEYWORD`, `IDENTIFIER`, `SYMBOL`, `CONSTANT`,
`STRING_LITERAL`. Malformed numeric tokens (e.g. `0x` with no digits, `3e`
with no exponent body) raise `LexError` at lex time rather than being split
into pieces.

## Using the parser

`parser.py` parses C99 source into a `c99_ast` tree and pretty-prints it:

```sh
uv run python parser.py <source.c>    # read from a file
uv run python parser.py -              # read from stdin
```

As an API, `parse(source)` returns a `c99_ast` dataclass tree. The
pretty-printer in `pretty.py` works on any `@dataclass` tree and emits
valid Python, so round-tripping through `eval()` with the AST classes in
scope reconstructs the node.

## Stripping comments with pcpp

The lexer treats comments as lex errors (we expect a preprocessor to have
handled them already). [pcpp](https://github.com/ned14/pcpp) is installed
in the dev environment as a uv tool; use it to strip comments before
lexing or parsing:

```sh
pcpp input.c --line-directive | uv run python lexer.py -
pcpp input.c --line-directive | uv run python parser.py -
```

Notes:
- `-` as the input to `lexer.py` / `parser.py` reads from stdin.
- `--line-directive` with no form argument (trailing flag, or
  `--line-directive=`) suppresses the `#line N "file"` markers pcpp
  emits by default.
- pcpp replaces each block comment with a single space (C99 translation
  phase 3), not an empty string — harmless since our lexer ignores
  whitespace.
- Add `-D NAME=VAL` / `-U NAME` / `-I path` as needed for macro and
  include control. `--passthru-comments` keeps comments if you ever
  need the opposite behavior.

## Tests

```sh
uv run python -m unittest
```
