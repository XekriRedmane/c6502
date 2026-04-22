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
uv run python lexer.py <source.c>
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

## Tests

```sh
uv run python -m unittest
```
