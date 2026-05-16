# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when
working with code in this repository.

## Project overview

c6502 is a C99 compiler written in Python that targets the MOS 6502.
Dependencies are managed with `uv`; `pyproject.toml` is the source of
truth and `uv.lock` the resolved set. `requirements.txt` is a hand-
maintained `pip`-compatible fallback and may lag.

## Subdirectories

Each subdirectory has its own CLAUDE.md with the details for the
files it contains. Read them as you navigate into them; this root
file holds the project overview and notes about root-level files
(parser, lexer, codegen, ASDL definitions, compile.py CLI).

- [passes/CLAUDE.md](passes/CLAUDE.md) — middle-end passes
  (identifier / label / loop / type-checking; replace_pseudoregisters;
  asm_to_asm2; peephole catalog; call-graph-disjoint ZP allocation).
- [passes/optimization/CLAUDE.md](passes/optimization/CLAUDE.md) —
  TAC-level fixed-point optimizer (SSA, constant folding, copy
  folding, IndexedStore recognizer, dead-loop elimination, …).
- [passes/optimization_asm/CLAUDE.md](passes/optimization_asm/CLAUDE.md)
  — asm-level SSA round-trip with byte-granular regalloc, move
  coalescing, const-static fold.
- [passes/optimization_ast/CLAUDE.md](passes/optimization_ast/CLAUDE.md)
  — AST-level unroller (`#pragma c6502 loop unroll(enable)`).
- [tests/CLAUDE.md](tests/CLAUDE.md) — test organization, chapter
  harnesses, where to look for status.
- [sim/CLAUDE.md](sim/CLAUDE.md) — in-process 6502 simulator
  (assembler, runtime stub, Python-implemented helpers).
- [docs/CLAUDE.md](docs/CLAUDE.md) — design notes, walkthroughs,
  reference grammars, drafts.
- [examples/CLAUDE.md](examples/CLAUDE.md) — real-world C sources
  with checked-in expected `.asm` outputs.
- [include/CLAUDE.md](include/CLAUDE.md) — c6502 flavors of
  `<stdint.h>` and `<limits.h>`.

## Pipeline at a glance

```
C source
  → preprocess (pcpp)
  → lex + parse (lark, LALR)            → c99_ast
  → identifier_resolution
  → string_lifting
  → label_resolution
  → loop_labeling
  → type_checking                       → c99_ast (+ SymbolTable, TypeTable)
  → (--unroll: unroll_program — #pragma c6502 loop unroll(enable))
  → c99_to_tac                          → tac_ast
  → (--optimize: TAC fixed-point opts)
  → (--optimize: select_abi + allocate_zp_slots)
  → tac_to_asm                          → asm_ast
  → (--optimize: asm-SSA round-trip + byte-granular regalloc)
  → replace_pseudoregisters             (Pseudo → Frame / ZP / Data)
  → (--optimize: synthesize_prologue, loop_counter_to_x, peephole fixedpoint)
  → expand_long_branches
  → asm_to_asm2                         → asm2_ast (1:1 with 6502 opcodes)
  → asm_emit                            → dasm-syntax 6502 assembly text
```

Four ASDL-defined IRs (`c99.asdl`, `tac.asdl`, `asm.asdl`,
`asm2.asdl`) shape the data passed between passes. `tac_ast` carries
width AND signedness on integer types; `asm_ast` is byte-typed (one
IR atom per 6502 opcode, with `Pseudo` operands that
`replace_pseudoregisters` resolves); `asm2_ast` is `asm_ast` with the
three compound nodes (`FunctionPrologue` / `AllocateStack` / `Ret`)
already expanded into atoms.

Two key runtime conventions:

- **Soft data stack** at `SSP`/`FP` (zero-page pointers, both grow
  downward); the 6502 hardware stack at `$0100-$01FF` is reserved for
  JSR return addresses and short-lived PHA/PLA. Reserved ZP: `$00-$01`
  SSP, `$02-$03` FP, `$04-$23` HARGS (runtime-helper exchange block),
  `$24-$25` DPTR (caller-saved indirect-pointer scratch). Full layout
  in "Function stack frame" below.
- **`__attribute__((zp_abi))` + call-graph-disjoint private pools**
  put params AND body locals in ZP for eligible leaf / non-recursive
  / non-indirect-calling functions; ineligible functions fall back to
  the default caller/callee partition (`$80..$BF` / `$C0..$FF`).
  Eligible functions emit as bare body + RTS. Details in
  [passes/CLAUDE.md](passes/CLAUDE.md) under "Call-graph-disjoint ZP
  allocation".

## Common commands

```sh
uv sync                                         # create/update the project venv
uv run python -m unittest                       # run all tests
uv run python -m unittest tests.test_asm_emit   # run one module
uv run python -m unittest tests.test_chapter_1.TestChapter1Valid    # run one test

uv run python compile.py <source.c> --codegen              # C → 6502 asm to stdout
uv run python compile.py <source.c> --codegen -o out.asm   # to a file (must end .asm)
uv run python compile.py - --tac < source.c                # read stdin, stop after TAC
uv run python compile.py - --codegen --optimize < src.c    # with the optimizer pipeline
```

`compile.py` is the only CLI; every other module is library-only.
Flags it doesn't recognize are forwarded to the preprocessor (pcpp),
so `-D`, `-U`, `-I`, `--passthru-*`, `--line-directive` etc. work the
same as the `pcpp` CLI. pcpp's own `-o` is not forwarded.

Stage-selection flags (mutually exclusive, one required with
`compile.py`): `--lex`, `--parse`, `--resolve`, `--tac`, `--codegen`.
`--resolve` runs the three name-resolution passes (identifier
resolution, label resolution, loop labeling) in that order.

Modifier flags (orthogonal to the stage flags; both apply to `--tac`
and `--codegen`):

- `--optimize` runs the optimizer pipeline:

  1. TAC SSA construction (`passes.optimization.ssa_construction`).
  2. One-shot scalar const-static read fold — replaces `Var(static
     const scalar)` USE positions with `Constant(value)`.
  3. TAC fixed-point loop: constant folding (incl. const-array-
     subscript fold), strength reduction, comparison-against-zero /
     jump fold, AND-zero-jump fold, unreachable-code elimination,
     copy propagation, dead-store elimination, copy folding, Add-
     with-Constant reassociation, loop rotation, sink-increment,
     IndexedLoad / IndexedStore / IndirectIndexed recognizers.
  4. TAC SSA destruction + post-destruction copy folding.
  5. Asm-level const-static fold (drops scalar `static T const`
     storage, replacing references with immediates).
  6. Asm-level SSA round-trip: move coalescing, forward + backward
     copy propagation, byte-granular DCE, byte-granular regalloc
     drawing from per-function private pools (when eligible) or the
     default caller/callee partition.
  7. `replace_pseudoregisters_bare_exit` resolves Pseudos.
  8. Late prologue / epilogue synthesis (collapses to bare body +
     RTS when no save / spill is needed).
  9. `loop_counter_to_x` (X-pivot promotion for hot loop counters).
  10. Peephole fixed-point loop — see the peephole catalog in
      [passes/CLAUDE.md](passes/CLAUDE.md).
  11. `expand_long_branches`, `asm_to_asm2`, `emit_program`.

  Also enables `__attribute__((zp_abi))` and the call-graph-disjoint
  body-local allocator. The INC / DEC peepholes run in the
  unoptimized pipeline too (their win is addressing-mode-aware, not
  regalloc-dependent).

- `--unroll` (only meaningful with `--optimize`) runs `passes.
  optimization_ast.unroll.unroll_program` after parsing and before
  identifier resolution. See
  [passes/optimization_ast/CLAUDE.md](passes/optimization_ast/CLAUDE.md).

Linker mode:

- `compile.py --link <a.asm> <b.asm> ... -o out.asm` — multi-TU
  globally re-allocates `__zpabi_*` and `__local_*` ZP slot symbols
  across the supplied per-TU outputs (each produced by `compile.py
  --codegen --optimize`). Reads each input's `; @zp-link-meta-begin`
  / `; @zp-link-meta-end` block (emitted by
  `passes.zp_link_metadata`) to recover the call-graph and param /
  local sizes needed for the global allocation. See
  `passes/linker.py`.

For the full optimizer walkthrough, see
[passes/optimization/CLAUDE.md](passes/optimization/CLAUDE.md) and
`docs/optimization.md` / `docs/leaf_zp_abi.md`.

## Regenerating AST modules

Each `*_ast.py` module is generated from its matching `*.asdl` by
`asdl.py`. After editing an ASDL file, regenerate:

```sh
uv run python asdl.py c99.asdl c99_ast.py
uv run python asdl.py tac.asdl tac_ast.py
uv run python asdl.py asm.asdl asm_ast.py
uv run python asdl.py asm2.asdl asm2_ast.py
```

The generator emits one `@dataclass` per type. Sum-type bases are
named `Type_<name>` (to avoid colliding with Python builtins like
`int`); constructor classes keep their ASDL names. Fields become
`int`, `str`, `list[...]`, or `T | None` depending on the primitive /
`*` / `?` markers.

## Root-level pipeline stages

`compile.py --codegen` chains eleven passes. The middle-end passes
(2–6, 9–10) live in `passes/`; their walkthroughs are in
[passes/CLAUDE.md](passes/CLAUDE.md). The four stages rooted in this
directory:

- **Pass 1: `parser.parse`** (`parser.py`, `c99.lark`, `lexer.py`,
  `preprocessor.py`) — C source → `c99_ast`. See "Parser" below.
- **Pass 7: `c99_to_tac.translate_program`** (`c99_to_tac.py`) —
  `(c99_ast, SymbolTable)` → `tac_ast`. See "c99_to_tac" below.
- **Pass 8: `tac_to_asm.translate_program`** (`tac_to_asm.py`) —
  `tac_ast` → `asm_ast`. See "tac_to_asm" below.
- **Pass 11: `asm_emit.emit_program`** (`asm_emit.py`) — `asm2_ast`
  → 6502 assembly text. See "asm_emit" below.

Other root-level files:

- `asdl.py` — code generator for `*_ast.py` from `*.asdl`.
- `c99.asdl` / `tac.asdl` / `asm.asdl` / `asm2.asdl` — the four ASDL
  IR definitions.
- `c99_ast.py` / `tac_ast.py` / `asm_ast.py` / `asm2_ast.py` —
  generated `@dataclass` modules.
- `tac_sim.py` — TAC-level interpreter, used by tests to validate
  expected behavior at the TAC stage.
- `fp_arith.py` — IEEE 754 Float / Double arithmetic in Python; used
  by `tac_sim.py` and `sim/runtime.py`'s FP traps.
- `pretty.py` — pretty-printer for ASTs (used in unit-test
  diagnostics).
- `compile.py` — the CLI driver.
- `main.py` — empty `hello world` stub; not part of the compiler.
- `knowledge_base.md` — engineering-knowledge running log.

## Parser

`parser.py` implements C source → `c99_ast`. Lark/LALR grammar lives
in `c99.lark`. The top-level production is `declaration*`: a
translation unit is a list of `var_decl` / `function_decl` forms.
The AST stores them as `Program(declaration*)` where each declaration
is `VarDecl(var_decl)` or `FunctionDecl(function_decl)`; a function
*definition* is the FunctionDecl variant with `body=Block(...)`,
while a forward declaration has `body=None`. Both `var_decl` and
`function_decl` carry a required `data_type` (one of `Int()`,
`Long()`, `UInt()`, `ULong()`, `Float()`, `Double()`, or — for
functions — `FunType(params, ret)`) and an optional `storage_class`
(`Static()` / `Extern()` / None). The parser builds the function's
`FunType` from the per-param `type_specifier+` runs and the return-
type specifiers. A `<param_list>` is `void` (empty params) or comma-
separated `<type_specifier>+ IDENT` pairs. Parameter *names* live on
the function_decl's `params` array; their *types* live in parallel on
`data_type.params`.

### Type vocabulary

Nine integer types, two floating types, plus pointers and function
types. Widths follow C99's minimum ranges per §5.2.4.2.1 — Int is 16
bits, Long is 32 bits, LongLong is 64 bits.

Integers:

- `Int()` is 2-byte signed (-32768..32767).
- `Long()` is 4-byte signed (-2^31..2^31-1).
- `LongLong()` is 8-byte signed (-2^63..2^63-1).
- `UInt()` is 2-byte unsigned (0..65535).
- `ULong()` is 4-byte unsigned (0..2^32-1).
- `ULongLong()` is 8-byte unsigned (0..2^64-1).
- `Char()` is 1-byte unsigned (0..255; plain `char` is unsigned in
  c6502 per C99 §6.2.5.15's implementation-defined choice — same
  byte semantics as `unsigned char`).
- `SChar()` is `signed char` (1-byte signed, -128..127).
- `UChar()` is `unsigned char` (1-byte unsigned, 0..255).

Char/SChar/UChar are distinct types from Int/UInt — C99 §6.3.1.1.2
integer-promotes them all to `int` (because Int's 16-bit range covers
both signed and unsigned char), so arithmetic always happens at Int
width or wider.

Floating:

- `Float()` is IEEE 754 single (4 bytes).
- `Double()` is IEEE 754 double (8 bytes).
- `long double` (16-byte IEEE 754 quad / extended) isn't modelled —
  the parser rejects it.

`Pointer(referenced_type)` is a 2-byte address (the 6502's address
width); declared with `*` in the declarator, e.g. `int *p;`. Pointer
is its own TAC variant (`tac_ast.Pointer`) because 2-byte pointers
no longer match any integer width — Int is now 2 bytes too, but
conceptually distinct, and the symbol table preserves the Pointer
type for the rare codegen sites that inspect it (signedness checks
for unsigned ordering, pointer-arithmetic scaling).

### Constants

The lexer splits integer literals into four terminals by suffix
(`INTEGER_CONSTANT` for no suffix, `LONG_INTEGER` for `L`/`LL`,
`UINT_INTEGER` for `U`-only, `ULONG_INTEGER` for `U+L` in any order
— `LL` shares a terminal with `L`, and `ULL` with `UL`; the parser's
`has_ll` flag then routes `LL`/`ULL` cases into separate candidate-
list rows). Floating literals split into three (`DOUBLE_CONSTANT` for
no suffix, `FLOAT_CONSTANT` for `f`/`F`, `LONG_DOUBLE_CONSTANT` for
`l`/`L`). The parser's `_const_for_token` then maps each integer
token + base (decimal vs. hex/octal) to a c99 const variant per the
C99 §6.4.4.1 paragraph 5 type-list rule (first type whose range fits
the value):

- unsuffixed decimal:    int → long → long long
- unsuffixed hex/octal:  int → unsigned int → long → unsigned long →
                          long long → unsigned long long
- `L` decimal:           long → long long
- `L` hex/octal:         long → unsigned long → long long → unsigned
                          long long
- `LL` decimal:          long long
- `LL` hex/octal:        long long → unsigned long long
- `U`:                   unsigned int → unsigned long → unsigned long
                          long
- `UL` (any letter order): unsigned long → unsigned long long
- `ULL` (any letter order): unsigned long long

Picking the matching const variant from those lists. A literal whose
value exceeds `unsigned long long` (the widest type c6502 models) is
rejected with "doesn't fit any supported type". Floating literals
follow C99 §6.4.4.2 — the suffix uniquely determines the type, no
value-fitting rule:

- unsuffixed: `ConstDouble`
- `f` / `F`:  `ConstFloat`
- `l` / `L`:  rejected ("long double not supported")

Hex floating literals (`0x1.0p3`) lex but the parser rejects them —
Python's `float()` doesn't parse the C hex-float syntax, so wiring
up support would mean writing the conversion by hand; deferred until
something in the corpus needs them.

`Constant(const)` wraps the resulting `Type_const`.
`_split_specifiers` validates the run of specifier tokens (`int`,
`long`, `signed`, `unsigned`, `float`, `double`, `static`, `extern`)
and splits it into the `(data_type, storage_class)` pair.
`_resolve_data_type` decodes the C99 §6.7.2 combinations c6502
supports (`int` / `signed [int]` → Int; `unsigned [int]` → UInt;
`long [int]` / `signed long [int]` → Long; `unsigned long [int]` →
ULong; `long long [int]` / `signed long long [int]` → LongLong;
`unsigned long long [int]` → ULongLong; `char` → Char; `signed char`
→ SChar; `unsigned char` → UChar; `float` → Float; `double` →
Double), rejecting multiple type specifiers, multiple storage
classes, missing type, three or more `long`s, `char` combined with
`int` / `long` / `short`, `long double`, `signed unsigned`, and any
FP/integer specifier mix (`int float`, `unsigned double`, etc.).

### Statement and expression grammar

A `<block>` is `{ <block_item>* }` (its own AST product type
`Block(block_item*)` so a function body is `Function(name,
Block([...]))`). A block item is a declaration (`var_decl` or
`function_decl`) or a statement (`return exp;`, `exp;`, `if (exp)
stmt (else stmt)?`, `goto label;`, `label: stmt`, a `<block>`
(compound statement, `Compound(block)`), `break;`, `continue;`,
`while (exp) stmt`, `do stmt while (exp);`, `for (<for_init> exp? ;
exp?) stmt`, or a null `;`). The two declaration alternatives map to
the AST sum `declaration = FunctionDecl(function_decl) |
VarDecl(var_decl)`: `var_decl` is `<specifier>+ <declarator> (= exp)?
;` and `function_decl` is `<specifier>+ <declarator> <block>` (the
function-definition path; forward declarations like `int foo(int x);`
parse as `var_decl` because the trailing `;` matches that rule, and
the var_decl transformer rewraps as a `Type_function_decl` with
`body=None` whenever the declarator composes to a FunType). The
transformer walks the declarator parse tree (per C99 §6.7.5: postfix
array / function suffixes bind tighter than prefix `*`) via
`_apply_declarator`, returning `(name, composed_type,
outer_param_names)`. Composed type accumulates `Pointer` wrappers
from the pointer prefix and `FunType` wrappers from function
suffixes; the outermost function-suffix's param names ride along
separately for the AST's `params` field (which holds names alongside
the function's `data_type` = `FunType` carrying param types). `int
*p;` → `Pointer(Int())`; `int *foo(int *x)` →
`FunType(params=[Pointer(Int())], ret=Pointer(Int()))`. Array
declarators (`int a[10];`) and function-pointer declarators (`int
(*fp)(int);`) parse but `_apply_direct_declarator` raises
NotImplementedError when it hits an array suffix — c99_ast has no
Array variant yet.

Iteration statements introduce a `for_init` rule covering a
`var_decl` or `exp? ;` (function declarations aren't legal in for-
init per C99 §6.8.5). The loop AST nodes (`WhileStmt`, `DoWhileStmt`,
`ForStmt`, `BreakStmt`, `ContinueStmt`) carry an `identifier label`
field that the parser leaves as the empty string — the loop_labeling
pass fills it in later. Selection / case statements (`SwitchStmt`,
`CaseStmt`, `DefaultStmt`) carry the same kind of `label` field;
`SwitchStmt` additionally has `cases` (a list of `(value, label)`
pairs collected from its body), an optional `default_label`, and an
optional `promoted_type` (filled by the type checker — see pass 5).
The case-label expression goes through a `constant_exp` non-terminal
that's a one-child wrapper around `conditional_exp`; the wrapper
exists so the site is self-documenting and shares a §6.6 validator
across future call sites (enums, array sizes, etc. — see
`passes.constant_expression`). The case / default rules introduce
their own COLON shift-reduce situation analogous to labeled
statements; LALR(1) shift resolves it correctly. The compound-
statement rule reuses the same `block` rule the function body uses —
the only difference is the transformer wraps the resulting `Block`
in a `Compound`. The `IDENTIFIER COLON statement` rule for labeled
statements introduces a shift-reduce conflict at statement-start on
COLON lookahead — Lark's LALR(1) backend resolves it by shifting
(same mechanism that handles dangling-else), which picks the
labeled-statement branch. Inside an expression (e.g. a ternary's
true-clause) the parser state is different, so `a ? b : c` continues
to parse as a Conditional even though `b` is also an IDENTIFIER
followed by COLON.

The dangling-else ambiguity is resolved by Lark's LALR(1) backend
preferring shift, which binds `else` to the nearest preceding
unmatched `if` (the C99 §6.8.4.1 rule). `<exp>` covers integer
constants, identifiers, casts (`(int)x` / `(long)x`), unary `-`/`~`/
`!`, binary `+`/`-`/`*`/`/`/`%`/bitwise/shift/comparison/`&&`/`||`,
parentheses, right-associative `=`, and the ternary `cond ? t : f`.
Cast expressions sit at their own `cast_exp` level between
`unary_exp` and `mul_exp` (C99 §6.5.4), right-recursive so
`(int)(long)x` parses as `(int)((long)x)`. The `mul_exp` recursive
RHS and the unary-operator alternative both take `cast_exp`, so
`-(int)x` parses as `-((int)x)`; prefix `++`/`--` keep their
`unary_exp` operand because the cast result isn't an lvalue (so
`++(int)x` is a parse error). The LPAREN-vs-paren-expr ambiguity is
resolved at LALR(1) by the next token after `(`: any type-specifier
token (`INT`, `LONG`, `SIGNED`, `UNSIGNED`, `FLOAT`, `DOUBLE`) →
cast; anything else → parenthesised exp. Each `Cast(target_type,
exp)` carries a resolved object-type target (built by the `type_name:
type_specifier+` rule, which reuses `_resolve_data_type`).

The assignment LHS is loosened from C99's `unary-expression` to
`conditional_exp`, so `1+2=3+4` and `(1?2:a)=5` both parse —
identifier resolution rejects the non-lvalue forms. The ternary sits
at its own `conditional_exp` level between assignment and logical-or
(C99 §6.5.15): condition is `logical_or_exp`, true-clause is full
`exp`, false-clause is `conditional_exp`. The right-recursion makes
`?:` right-associative (`a ? 1 : b ? 2 : 3` is `a ? 1 : (b ? 2 :
3)`) and keeps assignments out of the false-clause slot, so `1 ? 2
: a = 5` parses as `(1 ? 2 : a) = 5` via the outer assignment rule
(and then fails the lvalue check).

The ten compound assignments (`+=`, `-=`, `*=`, `/=`, `%=`, `&=`,
`|=`, `^=`, `<<=`, `>>=`) share a single `compound_assign` builder
that builds a `CompoundAssignment(op, lval, rval, intermediate_type?,
data_type?)` AST node — NOT a parse-time desugar to `Assignment(lval,
Binary(OP, lval, rval))`. The explicit node is what lets c99_to_tac
evaluate the lval's address ONCE before the read-modify-write, which
matters for Subscript / Dereference / Dot / Arrow lvals whose
address-computing subexpressions have side effects (`arr[i++] += 1`,
`(*p++)++`, `ptr++[idx++] *= 3`); a desugar would duplicate the lval
and fire those side effects twice. The type checker stamps two types
on the node: `intermediate_type` is the binop's working type —
common-of-promoted for arithmetic / bitwise per §6.3.1.8, promoted-
left alone for shifts per §6.5.7.3 (right operand promotes
independently), pointer-itself for `ptr += int` (which routes
through `translate_pointer_arithmetic` in c99_to_tac for sizeof-
pointee scaling) — and `data_type` is the lval's type, the result of
the compound assign expression. c99_to_tac's
`_translate_compound_assign` computes the lval's address once, Loads
at the lval type, casts to intermediate_type, applies the binop,
casts back to lval's type, and Stores. Var lvals skip the Load/Store
(the Var IS the storage).

Prefix `++a` / `--a` and postfix `a++` / `a--` each build their own
AST node (`Prefix(incdec_op, exp)` / `Postfix(incdec_op, exp)`)
instead of desugaring to assignment. Two reasons: (1) they have
different result semantics — prefix returns the *new* value, postfix
the *old* value, which can't be expressed by reusing `Assignment` /
`Binary` alone; (2) direct nodes let `c99_to_tac` evaluate the
operand's address ONCE before the read-modify-write, avoiding the
side-effect duplication a desugared `arr[--i] = arr[--i] + 1` would
cause for richer lvalues. Postfix sits at its own grammar level
(`postfix_exp`) one tighter than `unary_exp`, so `-a++` parses as
`-(a++)` and `++a++` as `++(a++)` (the inner `a++` isn't an lvalue,
but identifier_resolution rejects that semantically — the grammar
accepts it).

Function calls `f(arg, ...)` sit at the atom level alongside
constants, parenthesised expressions, and bare identifiers. The
grammar uses `IDENTIFIER LPAREN arg_list? RPAREN -> function_call`
(and `IDENTIFIER -> identifier` as a separate atom alternative);
LALR(1) shifts on LPAREN to disambiguate between the two — bare `f`
reduces to `Var("f")`, `f(x)` reduces to `FunctionCall(name="f",
args=[...])`. The callee is a literal identifier, not an expression,
because the AST node carries the name as a string (`FunctionCall(name,
args)`) — no pointer-to-function call form yet. Arg expressions are
full `exp` (assignment-level), separated by commas.

### Lexer & preprocessor

The lexer (`lexer.py`) treats comments as lex errors — it assumes a
preprocessor has already stripped them. `preprocessor.preprocess`
wraps `pcpp` (installed as a uv tool, used via its Python API, no
shelling out). Malformed numeric tokens (`0x` with no digits, `3e`
with no exponent body) raise `LexError` rather than being split.

`docs/*_grammar.txt` files are reference documentation for the spec
grammars that `c99.lark` implements — they aren't parsed by any tool.

## c99_to_tac

`c99_to_tac.translate_program` — `(c99_ast, SymbolTable)` →
`tac_ast`. The TAC program shape is `Program(top_level*)` where each
`top_level` is `Function(name, is_global, params, instructions)` or
`StaticVariable(name, is_global, data_type, init)`. Two passes
assemble the list:

1. Walk c99 declarations in source order. Each `FunctionDecl` with a
   body lowers to a TAC `Function`; `is_global` rides through from
   the function's symbol-table entry. File-scope variable
   declarations and forward function declarations emit nothing here.
   Block-scope variable declarations with a storage class (`static`
   / `extern`) also skip TAC emission at the declaration site; plain
   `int x [= e];` / `long x [= e];` / `unsigned int x [= e];` /
   `unsigned long x [= e];` lowers to a `Copy` from the evaluated
   initializer into the var.
2. Iterate the symbol table once. Each `StaticAttr` entry whose
   `initial_value` is `Initial(c)` (use `c`) or `Tentative` (use `0`)
   becomes a TAC `StaticVariable`, with a typed `IntInit(v)` /
   `LongInit(v)` / `UIntInit(v)` / `ULongInit(v)` / `FloatInit(v)` /
   `DoubleInit(v)` chosen by the variable's declared type;
   `NoInitializer` entries describe a reference to a definition
   elsewhere and emit nothing.

The c99 and TAC ASDLs declare parallel `data_type` sums (Int / Long /
LongLong / UInt / ULong / ULongLong / Float / Double / FunType), so
translating data_type is a one-to-one rewrap (`_to_tac_data_type`).
The TAC `const` sum carries each integer's full c99 type — width AND
signedness — across six variants: ConstInt / ConstLong / ConstLongLong
on the signed side, ConstUInt / ConstULong / ConstULongLong on the
unsigned side. `_to_tac_const` is a 1-to-1 map per variant;
`ConstChar` / `ConstUChar` collapse onto `ConstInt` / `ConstUInt`
respectively (per C99 §6.3.1.1.2 char-types-promote-to-int). The
6502 doesn't care about signedness at the byte level for `+` / `-` /
`&` / `|` / `^` / `<<` / `==` / `!=`, so those op lowerings dispatch
only on width; the places where signedness matters at codegen — `<` /
`>` / `<=` / `>=`, right shift, int↔FP conversion — read the
operand variant's signedness for Constants and the symbol-table c99
type for Vars, and dispatch accordingly (`asr*` vs. `lsr*` for right
shift; V-corrected MI/PL vs. BCC/BCS for ordering; i2f vs. u2f vs.
l2f vs. ul2f etc. for FP conversion). The integer value passes
through `_to_tac_const` unchanged; downstream `_byte_at` masks each
byte with `& 0xFF`, so the bit pattern is preserved regardless of how
the integer is interpreted. FP variants stay distinct (Float and
Double have different IEEE 754 bit patterns). The TAC `static_init`
sum likewise keeps signedness alongside width on the integer side
(IntInit / LongInit / LongLongInit / UIntInit / ULongInit /
ULongLongInit) and precision on the FP side (FloatInit /
DoubleInit) — `_tac_static_init_for(t, v)` dispatches on the
declared type and coerces the raw value (`int(v)` for integer
variants, `float(v)` for FP variants), so an integer literal
initializing a `double` static lays down as `3.0` and a Cast-wrapped
FP initializer for an integer static lays down its truncated
integer. The helpers `_tac_const_for(t, v)` and `_tac_const_val(t,
v)` build typed constants for the synthetic-constant call sites
(postfix `+1`, short-circuit 0/1, implicit `return 0`); they
dispatch by type — `Int` → `ConstInt(v)`, `UInt` → `ConstUInt(v)`,
`Long` → `ConstLong(v)`, `ULong` / `Pointer` → `ConstULong(v)`,
`LongLong` → `ConstLongLong(v)`, `ULongLong` → `ConstULongLong(v)`,
`Float` → `ConstFloat(v)`, `Double` → `ConstDouble(v)`.

**Cast lowering.** `Cast(target, exp)` lowers based on the byte
widths of the source and target c99 types; same-width casts are
no-ops because the 6502 has no signedness distinction:

- same width (`Int↔UInt`, `Long↔ULong`, `LongLong↔ULongLong`, plus
  matching types) → elide (just return inner's val)
- narrower → wider, signed source (`Int → Long`, `Int → ULong`,
  `Int → LongLong`, `Long → LongLong`, `Long → ULongLong`, etc.) →
  `SignExtend(src, dst)`
- narrower → wider, unsigned source (`UInt → Long`, `UInt → ULong`,
  `UInt → ULongLong`, `ULong → ULongLong`, etc.) →
  `ZeroExtend(src, dst)`
- wider → narrower (any signedness combination) → `Truncate(src,
  dst)`
- integer → Float / Double → `IntToFloat(src, dst)` /
  `IntToDouble(src, dst)`
- Float / Double → integer → `FloatToInt(src, dst)` /
  `DoubleToInt(src, dst)`
- Float ↔ Double cross-precision → `FloatToDouble(src, dst)` /
  `DoubleToFloat(src, dst)`

The SignExtend / ZeroExtend / Truncate nodes themselves carry no
width info — `tac_to_asm` reads the symbol-table widths of src and
dst at lowering time to fan out per byte (so the same three nodes
cover every 1B/2B/4B widening or narrowing pair). The six FP-
conversion nodes are TAC-only (the asm IR is 1:1 with 6502 opcodes);
`tac_to_asm` lowers each to a runtime helper Call. The TAC nodes
themselves carry no signedness or width info — `tac_to_asm` reads
the symbol-table types of src and dst to pick the right helper (i2f
vs. u2f vs. l2f vs. ul2f vs. ll2f vs. ull2f on the integer side,
f2d / d2f on the FP side). To keep that dispatch simple, `c99_to_tac`
compile-time-folds any FP cast whose operand is a TAC `Constant` —
folding sidesteps the integer-signedness erasure baked into TAC's
`const` sum (see `_fold_fp_cast_constant`). Static-storage
initializers also bypass the runtime path: `_tac_static_init_for`
does the int→float conversion in Python at static-init build time.
The source type comes from the inner node's `data_type` (set by the
type checker); a `None` data_type — synthetic AST that bypassed
type-checking — falls back to the elide path so unit tests of pure
Cast translation stay focused.

**Typed temporaries.** `Translator.make_temporary_variable_name(t)`
mints a fresh `%N`, registers it in the symbol table as a
`LocalAttr` symbol with `type=t`, and returns the name. Every
production call site passes the surrounding expression's `data_type`
(which the type checker has stamped as the post-conversion / post-
promotion result type), so each `%N` carries the right width.
Downstream consumers — `tac_to_asm` for operand-size dispatch and
`replace_pseudoregisters` for slot sizing — both read
`symbols['%N'].type` to decide on the byte plan: 1 byte for Int /
UInt, 2 for Long / ULong, 4 for LongLong / ULongLong / Float, 8 for
Double. The `t=None` default is a unit-test backstop and resolves to
Int.

Parameter names ride through unchanged — they were renamed to
`@<N>.<orig>` by identifier_resolution and TAC `Var(@<N>.<orig>)`
references in the body see the same names. Each TAC function gets an
implicit `Ret(_tac_const_val(ret_type, 0))` appended if its body
falls off without an explicit return (C99 §5.1.2.2.3 mandates this
for `main`; we apply it generally so every TAC function terminates).
The constant's variant matches the function's declared return type —
2-byte-returning functions (Long / ULong) get `ConstLong(0)`, 1-
byte-returning ones (Int / UInt) get `ConstInt(0)`, FP-returning
ones get `ConstFloat(0.0)` / `ConstDouble(0.0)`.

`FunctionDecl` block items lower to nothing. `FunctionCall(name,
args)` lowers to: evaluate each arg in source order (left-most temp
first), collect the resulting TAC vals, mint a fresh typed dst temp,
and emit a single `FunctionCall(name, args, dst)` TAC instruction.
The dst temp is what the call expression returns, so chained uses
(`x = f(); y = f() + 1`) thread cleanly through `Copy` / `Binary` /
`Ret` etc. Compound expressions flatten into ops, materializing each
intermediate into a fresh `Var(%n)`. `Binary(op, src1, src2, dst)`
evaluates `src1` first so its temps get lower numbers.

**Pointer arithmetic lowering.** When `Binary(Add | Subtract)` has at
least one Pointer operand (the type checker stamped the operand
types), `translate_pointer_arithmetic` takes over:

- `ptr ± int` — multiply the int operand by `_pointee_size(ptr)`
  using a `Binary(Multiply, int, ConstLong(size))`, skipping the
  multiply when size == 1; then emit a normal `Binary(Add /
  Subtract)` on the pointer and the scaled int. The dst temp is
  pointer-typed (so codegen sizes it as 2 bytes). For `int + ptr`
  the lowering keeps the pointer on the lhs of the underlying Add
  (consistency, not semantics — Add is commutative).
- `ptr - ptr` — emit `Binary(Subtract)` on the two 2-byte pointers
  to get a Long byte-difference, then divide by `_pointee_size(ptr)`
  via `Binary(Divide, diff, ConstLong(size))` to recover the element
  count, skipping the divide when size == 1. Result is Long.

`_pointee_size` returns the recursive `_sizeof` of the pointee: 1
for Int/UInt, 2 for Long/ULong/Pointer, 4 for Float, 8 for Double,
`_sizeof(elem) × count` for Array (so multi-dim pointer arithmetic
scales correctly — `int (*)[10]; q + 1` advances by 10 bytes). Same
widths as the symbol-table sizing in `tac_to_asm` and
`replace_pseudoregisters`. The Multiply/Divide steps go through the
existing `mul16` / `divmod16` runtime helpers (so a non-trivial
pointer arithmetic program assembles but won't link until those
helpers land — same status as `*` / `/` on Long).

**Subscript lowering.** `Subscript(array, index)` reuses
`translate_pointer_arithmetic` directly: compute
`array_val + index*sizeof(elem)` (the Pointer-typed byte address),
then `Load(src_ptr=addr, dst=fresh_elem_temp)` for rvalue context.
On the lvalue path (Assignment with Subscript lval) the same address
computation feeds a `Store(src=rval, dst_ptr=addr)`. Array decay was
reified by the type checker as an `AddressOf` wrapper, which lowers
to a `GetAddress` here, so `arr[i]` and `ptr[i]` go through the same
TAC shape — the only difference is that `arr[i]` evaluates to
`GetAddress(arr) + ...` while `ptr[i]` evaluates to `Load(ptr_var) +
...`.

`Goto(label)` lowers to a TAC `Jump(label)`; `LabeledStmt(label,
stmt)` lowers to a TAC `Label(label)` followed by the inner
statement's lowering. Label names arrive pre-mangled by
label_resolution and pass through unchanged. Iteration statements
derive concrete control-flow targets from the base label set by
loop_labeling, by suffix: `<base>_start` (top of loop),
`<base>_continue` (continue target), `<base>_break` (break target).
`BreakStmt(label)` → `Jump(<label>_break)`, `ContinueStmt(label)` →
`Jump(<label>_continue)`. The three loop kinds lower to fixed
sequences: `while` is `Label(_continue); <eval cond>;
JumpIfFalse(_break); <body>; Jump(_continue); Label(_break)`; `do-
while` is `Label(_start); <body>; Label(_continue); <eval cond>;
JumpIfTrue(_start); Label(_break)`; `for` is `<init>; Label(_start);
[<eval cond>; JumpIfFalse(_break);] <body>; Label(_continue);
[<post>;] Jump(_start); Label(_break)`, with the bracketed sections
omitted when the condition or post-clause slot is empty (a missing
condition is treated as unconditionally true).

**Switch lowering.** `SwitchStmt(control, body, label, cases,
default_label)` lowers to: evaluate the control once into a typed
temp `t`; for each `(case_value, case_label)` in `cases` emit
`Binary(Equal, t, case_const, eq_temp)` followed by
`JumpIfTrue(eq_temp, case_label)`; emit an unconditional
`Jump(default_label or <label>_break)` past the dispatch chain; then
translate `body` (which contains `CaseStmt` / `DefaultStmt` nodes
that lower to `Label(...)` followed by their inner statement);
finally emit `Label(<label>_break)`. Cases fall through unless
`break;` (lowered via the regular BreakStmt path to
`Jump(<switch>_break)`) is hit. Each case's `case.value` is already
a canonicalized integer `Constant` of the switch's promoted control
type (see pass 5 in [passes/CLAUDE.md](passes/CLAUDE.md)), so the
dispatch comparisons happen at one width. `CaseStmt` / `DefaultStmt`
outside the dispatch context just emit their `Label` and recurse —
the case-value itself was already consumed at the dispatch chain.

## tac_to_asm

`tac_to_asm.translate_program` — `tac_ast` → `asm_ast`. The asm
program shape mirrors TAC: `Program(top_level*)` with `Function(name,
is_global, params, instructions)` and `StaticVariable(name,
is_global, init)`. Each TAC `Function` lowers atom by atom; each TAC
`StaticVariable` rides through to an asm `StaticVariable`. The asm-
side init has five variants — the integer side carries only the
three width variants (`IntInit | LongInit | LongLongInit`), so TAC's
`UIntInit(v)` collapses to asm `IntInit(v)`, `ULongInit(v)` to
`LongInit(v)`, and `ULongLongInit(v)` to `LongLongInit(v)`; the FP
side keeps Float and Double distinct (`FloatInit | DoubleInit`)
because their IEEE 754 byte patterns differ. The asm side has no
`data_type` field — the variant of the init alone determines the
cell size at emit (DC.B for IntInit, DC.W for LongInit, DC.L for
LongLongInit, DC.L for FloatInit, two DC.Ls for DoubleInit since
dasm has no native 8-byte directive).

**The asm IR is strictly 1:1 with 6502 opcodes** — no width tagging
anywhere. The 6502 is an 8-bit machine, so every asm instruction is
implicitly Byte-typed. That makes `tac_to_asm` the single home of
all multi-byte lowering: for each TAC instruction whose operands are
wider than 1 byte (Long / ULong = 2 bytes, LongLong / ULongLong = 4,
Float = 4, Double = 8 — per the symbol table), the translator emits
a sequence of byte-level asm atoms — typically one pass per byte
with the 6502's carry flag threading naturally between them for
arithmetic on 2-byte operands. (FP arithmetic isn't lowered inline;
it dispatches to runtime helpers via the HARGS block — see below.)

**Per-byte addressing.** `Pseudo` and `Data` carry an `int offset`
field that selects which byte of a multi-byte value the reference
is — `offset=0` is the low byte (or the only byte of an Int),
`offset=k` the (k+1)-th byte (so `offset=7` is the high byte of a
Double). The helper `_byte_at(operand, k)` produces the k-th byte
of any operand: `Imm(v)` → `Imm((v >> 8*k) & 0xFF)` (using Python's
arithmetic `>>` so a negative ConstLong folds to its two's-
complement bytes; FP constants pre-fold to a non-negative IEEE 754
bit pattern at `translate_val` time, so the same shift-and-mask byte
extraction works without special-casing FP); memory-shaped operands
(Pseudo / Stack / Frame / Data) bump their `offset` by k.

**Operand-size dispatch.** `Translator._size_of(val)` returns 1 for
1-byte types (Int / UInt), 2 for 2-byte (Long / ULong), 4 for 4-byte
(LongLong / ULongLong / Float), 8 for Double — by reading the symbol
table for Vars and the const variant for Constants (each TAC integer
const variant carries width AND signedness; this helper only reads
width). Each per-instruction lowering keys off this size; the size-
parameterized loops naturally generalize across 1, 2, and 4 byte
widths with carry threading where appropriate. Signedness only
matters for ordering comparisons, right shift, and int↔FP conversion;
everywhere else the byte sequences are identical. The signedness
dispatch reads the operand: const variant for Constants, symbol-
table c99 type for Vars (via `_is_unsigned_val` for ordering / right
shift, `_int_type_of` for FP-conversion helper selection). Examples:

- `Copy(src, dst)`: 1 Mov for Int, 2 Movs (lo, hi) for Long.
- `Binary(Add, …)` Long: `Mov src1.lo→A; CLC; Add(src2.lo, A);
  Mov A→dst.lo; Mov src1.hi→A; Add(src2.hi, A); Mov A→dst.hi`. No
  CLC between the bytes — `LDA` only affects N/Z, so the carry from
  the low ADC is intact for the high ADC.
- `Binary(Subtract, …)` Long: same shape with SetCarry/Sub, borrow
  threads via the carry register.
- `Binary(Equal, …)` Long: high-byte CMP first; if differ, BNE
  short-circuits to a label (Z=0 there); else fall through to low-
  byte CMP whose Z is the final answer; then 0/1 select.
- `Binary(LessThan, …)` Long: low-byte SBC then high-byte SBC
  (carry threads), V-correction on the high result, branch on
  MI/PL. Same operand-swap trick as the 8-bit form for `>` / `<=`.
- `JumpIfFalse(Long_cond, target)`: `Mov(cond.lo, A); Or(cond.hi,
  A); Branch(EQ, target)` — the OR sets Z=1 iff both bytes are
  zero, i.e. the 16-bit value is zero.
- `Mul/Div/Mod/Shift` (any operand width): runtime-helper Calls.
  See "Runtime helper layout" below.

**Cast lowering** (matches the TAC node names from c99_to_tac).
SignExtend / ZeroExtend / Truncate read the source and destination
operand widths from the symbol table at lowering time, so the same
three TAC nodes cover every 1B/2B/4B widening or narrowing pair. The
6502 has no signedness distinction at the byte level, so same-width
casts are no-ops.

- `Truncate(src, dst)`: copy `_size_of(dst)` low bytes from src into
  dst — memory is little-endian, so byte 0 is the low byte, and the
  source's higher bytes are just discarded. Covers Long → Int,
  LongLong → Int, LongLong → Long, etc., for any signedness
  combination.
- `SignExtend(src, dst)` (signed source widened): inline byte
  sequence — copy each source byte to the matching dst byte (the
  last LDA's N flag is the source's sign byte's), `Branch(MI,
  sx_neg@N); LDA #$00; Jump(sx_done@N); Label(sx_neg@N); LDA #$FF;
  Label(sx_done@N);` then STA into each remaining (high) dst byte.
  Covers Int → Long, Int → LongLong, Long → LongLong, Int → ULong,
  etc. Two minted labels per use; the Translator's program-global
  counter keeps them unique.
- `ZeroExtend(src, dst)` (unsigned source widened): inline byte
  sequence — copy each source byte unchanged, then write a literal
  0 into each remaining (high) dst byte. No branch needed. Covers
  UInt → ULong, UInt → ULongLong, ULong → ULongLong, etc.

Output is correct but redundant — every intermediate is materialized
through a `Frame` slot. Optimization is deferred to TAC-level passes
(see [passes/optimization/CLAUDE.md](passes/optimization/CLAUDE.md)).

**TAC `FunctionCall(name, args, dst)`** lowers to the caller-side
soft-stack convention: `AllocateStack(total_arg_bytes)` (each Long
arg contributes 2 bytes, each LongLong / Float arg 4, each Double 8,
each Int 1), one Mov per arg byte writing into
`Stack(1)..Stack(total_arg_bytes)` in source order (low byte at the
lower offset for multi-byte args), `Call(name)`, then capture the
return value. The convention is width-driven: Int (1B) ← A; Long
(2B) ← A=low, X=high (with X routed through A for the high-byte
store); LongLong (4B) / Float (4B) ← bytes read from `HARGS+8..11`
byte-by-byte through A; Double (8B) ← bytes read from
`HARGS+16..23`. LongLong shares the Float slot because types are
exclusive per call and `mul32` / `divmod32` already write their
4-byte results to that offset, so a function ending `return a OP b;`
for LongLong operands needs no epilogue copy. The FP slots are
deliberately the same as the FP arithmetic helpers' output slots.
Caller has to capture any HARGS-returned value *immediately* after
the JSR, before any other helper Call, since HARGS is caller-saved.
The callee's epilogue rewinds SSP all the way back to the caller's
pre-call value, so there's no per-call cleanup. Runtime-helper calls
(mul8/16/32, divmod8/16/32, asl8/16/32, asr8/16/32) emitted by the
binary-op lowerings still go straight to `asm_ast.Call` (no
`AllocateStack`); they exchange operands through the `HARGS` zero-
page block instead of the soft stack, so they bypass the user-
function calling convention entirely.

### Runtime helper layout

Operands are exchanged through `HARGS`, a 24-byte zero-page block
(`$04`–`$1B`) that the runtime header pins by name. The block is
sized for the largest helper (`dadd`/`dsub`/`dmul`/`ddiv`, which
need 16 bytes in + 8 bytes out); integer helpers use only the low 8
bytes. Caller writes inputs into `HARGS+0..N-1`, JSRs the helper
(mul8 / udivmod8 / sdivmod8 / asl8 / asr8 / lsr8 for 1-byte
operands; the 16-bit and 32-bit families have the same names with
the suffix changed to 16 or 32), and reads the result from a fixed
offset later in the block. Inputs survive the call. The
signed/unsigned divmod split mirrors the asr/lsr right-shift split:
signed `/` and `%` route to `sdivmod*` (trunc-toward-zero per C99
§6.5.5.6), unsigned to `udivmod*` (floor-divide). Per-helper layout
(inputs → outputs):

```
  mul8       A:+0, B:+1               → product:+2 (1 byte; low byte of
                                          A*B, high byte discarded
                                          because int*int wraps to int)
  udivmod8/  num:+0, den:+1           → quot:+2, rem:+3
   sdivmod8
  asl8/      val:+0, count:+1         → result:+2
   asr8/
   lsr8
  mul16      A:+0..+1, B:+2..+3       → product:+4..+5 (2 bytes; low half
                                          of A*B, high half discarded)
  udivmod16/ num:+0..+1, den:+2..+3   → quot:+4..+5, rem:+6..+7
   sdivmod16
  asl16/     val:+0..+1, count:+2     → result:+3..+4 (1-byte count: shifts
   asr16/     ≥16 are UB, so the high byte of a promoted-to-Long count is
   lsr16      dropped)
  mul32      A:+0..+3, B:+4..+7       → product:+8..+11 (4 bytes; low half
                                          of A*B, high half discarded)
  udivmod32/ num:+0..+3, den:+4..+7   → quot:+8..+11, rem:+12..+15
   sdivmod32
  asl32/     val:+0..+3, count:+4     → result:+5..+8 (1-byte count: shifts
   asr32/     ≥32 are UB)
   lsr32
```

`RightShift` dispatches by operand signedness: signed operands route
to `asr*` (arithmetic, sign-preserving), unsigned to `lsr*` (logical,
zero-fill). Signedness for Constants comes from the const variant
(Const{Int,Long,LongLong} → signed, Const{UInt,ULong,ULongLong} →
unsigned); for Vars, from the symbol-table c99 type. The 16- and
32-bit helpers themselves aren't in the repo yet; the lowerings emit
calls to them in advance of the runtime header landing. (8-bit
signed `>>` of `signed char` is rare in practice — `signed char`
integer-promotes to `int` before `>>`, so the 8-bit `asr8` helper is
mostly a placeholder.)

### Comparisons and LogicalNot

`Mul`/`Div`/`Mod`/`LeftShift`/`RightShift` are TAC-only concepts;
`tac_to_asm` lowers each to a sequence of `Mov`s into `HARGS`, a
`Call` to the appropriate runtime helper, and `Mov`s reading the
result back out at a helper-specific offset within HARGS (see table
above).

The unary `LogicalNot` is lowered inline (no runtime helper): `Mov
src→A; Branch(EQ, true); Mov 0→A; Jump end; true: Mov 1→A; end: Mov
A→dst`. The framing `Mov(src, A)` already sets Z via `LDA`, so no
`Compare` is needed before the branch.

The six comparison ops
(`Equal`/`NotEqual`/`LessThan`/`GreaterThan`/`LessOrEqual`/`GreaterOrEqual`)
are also TAC-only but are lowered inline with `Compare`/`Sub` +
`Branch` atoms (no runtime helper). `Equal`/`NotEqual` emit `Mov
src1→A; Compare(A, src2); Branch(EQ|NE, true); LDA #0; Jump end;
true: LDA #1; end: Mov A→dst`. `LessThan`/`GreaterOrEqual` use `Mov
src1→A; SEC; Sub(src2, A); BVC novf; EOR #$80; novf:; Branch(MI|PL,
true); … 0/1 select …`. CMP can't be used for signed ordering
because it leaves V alone, and the N flag lies when the signed
subtraction overflows — the `BVC novf; EOR #$80` pair corrects N.
`GreaterThan`/`LessOrEqual` reuse the same sequence with operands
swapped (`>` → `src2 < src1`, `<=` → `src2 >= src1`) because `Z` is
unreliable after the EOR correction, so asking for "not-less-than
AND not-equal" directly would need a second compare; swapping is
cheaper. The asm IR itself has no multiply/divide/shift/lnot
primitives — every non-prologue/ret node is 1:1 with a 6502 opcode.

`tac_to_asm` is class-based (`Translator`) because the inline
comparison lowerings mint fresh labels per use and need a counter
that persists across the whole program. Module-level wrappers
(`translate_program`, etc.) each construct a fresh `Translator`.

## asm_emit

`asm_emit.emit_program` — `asm2_ast` → 6502 assembly text.

**Atomic IR**: every node maps to one 6502 instruction. The compound
nodes from asm_ast are gone here — they were expanded by step 10
(`asm_to_asm2`). The `Return` atom emits `RTS`; `Comment(text)`
emits `   ; <text>`; `Blank` emits `""` and `emit_function`
collapses consecutive blanks.

Multi-function programs emit each function's body in source order
separated by a single blank line.

`Data(name, offset)` operands render as `LDA name` for offset 0 (the
common case) and `LDA name+offset` otherwise — the assembler
resolves the symbol+offset to a fixed address. `ZP(address, offset)`
operands fold both at emit time into `LDA $XX` (where XX = address +
offset), giving direct zero-page addressing for regalloc-assigned
locals. `ZP` is legal everywhere `Data` is (Mov, Add/Sub, Compare,
Inc/Dec, ASL/LSR, direct LDX/LDY shortcut). The self-Mov peephole
inside `_emit_mov` returns `[]` when `src == dst` — drops the
redundant `LDA $XX; STA $XX` pairs that arise when regalloc gives a
Phi src and dst the same color.

Top-level `StaticVariable(name, _, init)` emits as `<name>:`
followed by `DC.B $XX` for `IntInit(int=v)`, `DC.W $XXXX` for
`LongInit(int=v)`, `DC.L $WWWWWWWW` for `LongLongInit(int=v)` (4
bytes signed/unsigned integer; mask to 32 bits so negatives render
as two's-complement), `DC.L $WWWWWWWW` for `FloatInit(float=v)` (4
bytes IEEE 754 single, packed via `struct.pack` at emit time), and
two `DC.L`s — low half, high half — for `DoubleInit(float=v)` (8
bytes IEEE 754 double; dasm has no native 8-byte directive). The W
form masks to 16 bits so signed-negative values render as two's-
complement; dasm's `DC.W` / `DC.L` both lay the bytes down little-
endian, matching the soft-stack memory model — so `Data(name,
offset=1)` accesses the high byte of a Long static, `Data(name,
offset=3)` the high byte of a LongLong static, and `Data(name,
offset=7)` the high byte of a Double static.

`Pseudo` operands aren't part of `asm2_ast` — they must have been
resolved by step 9 (`replace_pseudoregisters`); the asm_to_asm2 pass
raises if one slips through.

### Emit atomicity conventions

- `Add`/`Sub` do **not** emit `CLC`/`SEC` themselves — the caller
  emits `ClearCarry`/`SetCarry` first. This keeps each atomic node
  1:1 with a 6502 opcode.
- The `LDY` that sets up an indirect-Y source counts as addressing-
  mode setup, not a separate logical step, so a single `Mov(Frame,
  Reg(A))` still emits `LDY #o; LDA (PTR),Y`.
- `PTR` is `SSP` for `Stack` operands, `FP` for `Frame` operands.
  Stack/Frame offsets and immediates are `0..255` (single byte).
- Unsupported reg combinations for `Mov` raise (e.g. `Reg(X) →
  Reg(Y)`, `Reg(Y) → Reg(X)` — no direct transfer instruction).
  Same-register pairs (`Reg(A) → Reg(A)` etc.) and same-operand
  `Mov(src, dst)` with `src == dst` go through the self-Mov peephole
  and emit `[]` (the peephole catches the self-copies that arise
  when regalloc gives a Phi src and dst the same color).
- `ArithmeticShiftLeft` (ASL), `LogicalShiftRight` (LSR),
  `RotateLeft` (ROL), and `RotateRight` (ROR) currently only accept
  `Reg(A)` as `dst`. The 6502's shift/rotate family has accumulator
  and absolute/zero-page modes but no indirect-Y, so soft-stack
  values can't be shifted in place — load to A, shift, store back.
  These atoms are present in the IR but `tac_to_asm` doesn't emit
  them yet (`<<`/`>>` go through the `asl` / `asr` runtime helpers);
  they're available for inlining inside the helpers themselves once
  those land.
- `BitTest(src)` emits NMOS 6502 `BIT src` (zp / abs addressing only
  — no `BIT #imm` on NMOS). Sets `N=bit7(src)`, `V=bit6(src)`,
  `Z=(A & src)==0`; does not modify A. Primary use is the sign-bit
  test: `BIT M; BPL target` reads bit 7 of M in 5 cycles / 3 bytes
  (zp) vs. `LDA M; AND #$80; BEQ target` at 8+ cycles / 6 bytes
  that also clobbers A. `src` must be `Data` / `ZP`; emit and the
  in-process sim assembler reject `Frame` / `Stack` / `Indirect` /
  `IndexedData` / `Reg`. Emitted by `passes.and_sign_bit_branch`
  when the optimizer recognizes a `Mov(M, A); And(Imm(0x80), A);
  Branch(EQ|NE, _)` triple.
- `Label(name)`, `Jump(target)`, and `Branch(cond, target)` are the
  control-flow atoms. `Label` emits `<name>:` at column 1 (same
  column as the function name); `Jump` is `JMP <target>`; `Branch`
  is one of `BCC`/`BCS`/`BEQ`/`BMI`/`BNE`/`BPL`/`BVC`/`BVS` per its
  `condition`. All branches/jumps are symbolic — emit doesn't
  compute displacements, the assembler does. `tac_to_asm` emits them
  for the inline comparison lowerings and for the short-circuit
  lowerings of `&&` / `||` (`JumpIfFalse` → `Mov(cond, A);
  Branch(EQ, target)`, `JumpIfTrue` → `Branch(NE, …)`; TAC `Jump` /
  `Label` are atom-for-atom).
- Output formatting: labels at column 1, opcodes at column 4,
  operands at column 10. Each function emits `<name>:`, then
  `SUBROUTINE`, blank line, then instructions.

## Function stack frame (soft stack)

Arguments and locals live on a **soft data stack** in main RAM,
separate from the 6502's hardware stack at `$0100`–`$01FF` (which is
reserved for return addresses and short-lived `PHA`/`PHP`). This
dodges the 256-byte page-1 limit and keeps return addresses out of
the way during frame teardown.

Reserved zero-page: `$00`/`$01` = `SSP` (soft stack pointer, low/
high), `$02`/`$03` = `FP` (frame pointer), `$04`–`$1B` = `HARGS`
(24-byte runtime-helper exchange block — see "Runtime helper layout"
above for each helper's per-byte slot table; the block is sized for
the largest helper, `dadd`/`dsub`/`dmul`/`ddiv`, with 16 bytes of
inputs + 8 of output). `SSP` and `FP` both point at the **next-free
byte** and grow downward. SSP/FP access is always indirect-indexed:
`LDY #off; LDA (SSP),Y` or `LDA (FP),Y`, so `Y` is scratch for any
soft-stack access. HARGS bytes are accessed absolutely (`LDA
HARGS+k` / `STA HARGS+k`); dasm picks zero-page addressing
automatically because the symbol resolves into page 0.

Inside a function `SSP` is unstable (any intra-function push shifts
it). So every function captures `FP` once in its prelude and
addresses args/locals via `FP` — codegen emits `Frame(off)` for those
and the emitter lowers to `LDY #off; LDA (FP),Y`. For `N` arg-bytes
and `M` local-bytes:

- Caller subtracts `N` from `SSP`, writes args at `SSP+1…SSP+N`,
  `JSR`s.
- Callee prelude (skipped when `N+M == 0`): subtract `M+2` from
  `SSP` (locals + saved-FP slot), write caller `FP` into
  `SSP+M+1`/`SSP+M+2`, then `FP = SSP`. Smallest valid `FP` offset
  is `1` (same convention as `SSP`).
- Callee epilogue: `PHA` return value, `SSP = FP + M + N + 2` in
  one 16-bit add, reload caller `FP` via `(FP),Y` (with low byte
  routed through `X` so we don't corrupt the indirect base between
  the two reads), `PLA`, `RTS`.
- When `N+M == 0` the prelude emits nothing and the epilogue
  collapses to `RTS`.

Arg `j` is at offset `M + 3 + j` (not `M + 1 + j`) because the
saved-FP slot sits between locals and args. The README has a frame
diagram and a fully annotated sample prologue/epilogue.

## Status

For a working-feature checklist (every C99 §6.x construct c6502
accepts end-to-end), see the README's `## Status` section and
`tests/STATUS.md` (chapter-by-chapter pass/fail). The chapter test
harnesses under `tests/test_chapter_<N>.py` are the authoritative
list of what compiles and runs.

This section captures only the **gaps and known imprecisions** that
an unwary contributor would otherwise discover by surprise.

### Not yet in the repo (programs assemble but won't link)

The runtime header isn't in this repo:

- Symbol pinning for `SSP` / `FP` / `HARGS` / `DPTR`, `SSP`
  initialization, reset vector.
- Integer helpers: `mul8/16/32`, `udivmod8/16/32`, `sdivmod8/16/32`,
  `asl8/16/32`, `asr8/16/32`, `lsr8/16/32`.
- FP conversion helpers: 26 functions covering
  `{i,u,l,ul,ll,ull}{2f,2d}` and `{f,d}2{i,u,l,ul,ll,ull}` plus `f2d`
  / `d2f`.
- FP arithmetic helpers: `fadd` / `fsub` / `fmul` / `fdiv` and the
  `d`-variants. The ordering helpers `flt` / `fle` / `dlt` / `dle`
  exist as Python hooks in the sim but not yet as 6502 routines.
- `icall` trampoline (`JMP (DPTR)`).

Programs that hit any of `*` / `/` / `%` / `<<` / `>>` / FP↔int cast
/ FP arithmetic / indirect call assemble cleanly through dasm but
won't link until the runtime header lands. Python-implemented hooks
in `sim/` cover all of these so simulation-based tests still pass.

### Type-system limitations

- FP arithmetic / unary FP ops (`+`/`-`/`*`/`/` and unary `-` on
  `float` / `double`) raise `NotImplementedError` at TAC translation.
  FP conversions and static initialisers work; FP comparisons via
  `flt` / `fle` / `dlt` / `dle` work in sim through Python hooks.
- `long double` rejected at parse time (no 16-byte IEEE 754).
- Hex floating literals (`0x1.0p3`) lex but the parser rejects them
  — `float()` doesn't parse C hex-float syntax and the conversion
  isn't wired up.
- Function-pointer expressions don't exist yet — c6502 has no
  function-pointer call form beyond `IndirectCall` from the parser's
  restricted callee = identifier rule.
- `extern` arrays rejected.
- Some C99 init-list shapes rejected: scalar init for an array,
  brace init for a scalar, too many initializers, the C99
  subaggregate flat form (`int a[2][3] = {1,2,3,4,5,6};`).
- Constant-expression evaluator (`passes.constant_expression`)
  accepts only `Constant` literals optionally wrapped in casts; no
  Unary / Binary / Conditional folds yet. Affects `case <const-expr>:`
  and any future enum / array-size / bitfield-width consumer.

### Codegen imprecisions

- Comparisons on unsigned multi-byte operands use the signed V-
  corrected lowering. Correct for values whose high bit isn't set;
  incorrect for `unsigned long long` operands spanning the sign-bit
  boundary. Tracked but not fixed.
- The 8-bit signed `>>` helper (`asr8`) is mostly a placeholder —
  `signed char >>` integer-promotes to int before the shift, so
  `asr16` does the real work; `asr8` exists for completeness.
- Rvalue struct expressions used as lvalues (`f().m = …`, `(c?a:b).m
  = …`) are rejected: the sret slot has temporary lifetime, so
  assigning through it would be a memory-safety hole.

### Where to look for more

- `tests/STATUS.md` — chapter_18 file-by-file status.
- `tests/test_sim_differential.py` — opt-vs-unopt sim differential
  across the full chapter corpus; the `_OPT_DIVERGES` dict at the
  top is the live list of optimizer bugs.
- `tests/test_sim_asm_optimized.py` — chapter_1..12 corpus run
  through `--optimize` with end-to-end return-value assertions.
