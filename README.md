# c6502

A C99 compiler written in Python.

## Regenerating AST modules from `.asdl` sources

Each `*_ast.py` module is generated from its matching `*.asdl` source by
`asdl.py`. After editing any ASDL file, regenerate:

```sh
uv run python asdl.py c99.asdl c99_ast.py
uv run python asdl.py tac.asdl tac_ast.py
uv run python asdl.py asm.asdl asm_ast.py
```

The generator emits one `@dataclass` per type. Sum-type bases are named
`Type_<name>` to avoid collisions with Python builtins (`int`, etc.);
constructor classes keep their ASDL names. Fields use `int`, `str`, `list[...]`,
`T | None` depending on the primitive / optional / sequence markers in the
ASDL source.

## Top-level driver: `compile.py`

`compile.py` chains the whole pipeline. It pipes the input through pcpp
to strip comments, then runs to the stage requested by exactly one
mutually-exclusive flag:

| flag        | output                                                        |
| ----------- | ------------------------------------------------------------- |
| `--lex`     | one `line:col<TAB>kind<TAB>value` line per token              |
| `--parse`   | `pretty(c99_ast)` — the parsed AST                            |
| `--tac`     | `pretty(tac_ast)` — three-address-code IR                     |
| `--codegen` | 6502 assembly text                                            |

```sh
uv run python compile.py <source.c> --codegen              # to stdout
uv run python compile.py <source.c> --codegen -o out.asm   # to file
uv run python compile.py - --tac < source.c                # stdin
```

Notes:
- `-` as the input filename reads from stdin.
- With `--codegen`, the `-o` filename must end in `.asm`.
- The other stages (`--lex`, `--parse`, `--tac`) accept any output
  filename, or default to stdout.
- The comment-stripping step uses pcpp via the Python API (see
  `preprocessor.py`); pcpp is a project dependency, no shelling out.
- Any flag `compile.py` doesn't recognize is forwarded to the
  preprocessor — the full pcpp surface (`-D`, `-U`, `-N`, `-I`,
  `--passthru-*`, `--line-directive`, etc.) works the same as the
  `pcpp` CLI. pcpp's own `-o` is not forwarded; `compile.py`'s `-o`
  is the only output flag.

```sh
uv run python compile.py - --codegen -DMAX=42 -I include/ < src.c
```

The per-stage tools (`lexer.py`, `parser.py`, `tac_translator.py`,
`asm_translator.py`, `asm_emit.py`) below are the same building blocks,
exposed as standalone scripts and Python modules for debugging and
reuse.

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

## Translating a C99 AST to an asm AST

`asm_translator.py` walks a `c99_ast` tree and produces an `asm_ast` tree
(the tiny assembly IR declared in `asm.asdl`). Each `translate_*`
function handles one source-AST kind via a `match` statement:

| c99_ast node          | asm_ast result                                         |
| --------------------- | ------------------------------------------------------ |
| `Program(fn)`         | `Program(translate_function(fn))`                      |
| `Function(name,body)` | `Function(name, translate_statement(body))`            |
| `Return(exp)`         | `[Mov(translate_exp(exp), Register()), Ret()]`         |
| `Constant(value)`     | `Imm(value)`                                           |

Unknown variants raise `TypeError` so missing `case` clauses fail loudly.

CLI (runs parse then translate then pretty-prints the asm AST):

```sh
uv run python asm_translator.py <source.c>    # read from a file
uv run python asm_translator.py -              # read from stdin
```

## Emitting 6502 assembly

`asm_emit.py` takes an `asm_ast.Program` and produces 6502 assembly
text. Formatting rules:

- labels start in **column 1**
- opcodes (uppercase) and directives start in **column 4**
- operands start in **column 10**
- each function emits `<name>:` on one line, the `SUBROUTINE`
  directive on the next, then a blank line, then the instructions
- `Mov(src, Register())` → `LDA <src>`
- `Ret()` → `RTS`
- `Imm(value)` → `#$<02X>`; values outside 0..255 raise `ValueError`

The CLI chains the whole pipeline (parse → translate → emit):

```sh
uv run python asm_emit.py <source.c>              # stdout
uv run python asm_emit.py - -o out.asm            # stdin → file
```

If `-o` is supplied the filename must end in `.asm`. Piping through
pcpp first strips comments:

```sh
pcpp source.c --line-directive | uv run python asm_emit.py - -o source.asm
```

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

The `tests/` subdirectories hold sample programs from
[nlsandler/writing-a-c-compiler-tests](https://github.com/nlsandler/writing-a-c-compiler-tests)
(chapter 1), checked in verbatim:

- `tests/invalid_lex/` — sources that must fail at lex time (bad
  characters, malformed pp-numbers, …). Exercised by
  `TestInvalidLex` in `test_lexer.py`.
- `tests/invalid_parse/` — sources that lex cleanly but must fail at
  parse time (missing tokens, wrong keyword case, extra junk, …).
  Exercised by `TestInvalidParseFiles` in `test_parser.py`.
- `tests/valid/` — sources that must parse into `int main(void) {
  return N; }`. Exercised by `TestValidFiles` in `test_parser.py`.

Each file is run through `pcpp` (to strip comments) before being fed
to the lexer or parser; the file-based test classes are skipped if
`pcpp` isn't on `PATH`.
