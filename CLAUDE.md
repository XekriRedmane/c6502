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

See `c99.lark` for the full grammar — it's the authoritative,
compact description of what parses. A few non-obvious AST-shape
notes that aren't visible from the grammar alone:

- Forward function declarations (`int foo(int x);`) parse as
  `var_decl` (the trailing `;` matches that rule); the transformer
  rewraps as `Type_function_decl` with `body=None` when the
  declarator composes to a FunType.
- Compound assignments build a `CompoundAssignment(op, lval, rval,
  intermediate_type?, data_type?)` node, NOT a parse-time desugar
  to `Assignment(lval, Binary(OP, lval, rval))`. The explicit node
  lets c99_to_tac evaluate the lval's address ONCE for side-effect-
  ful lvals like `arr[i++] += 1`. The type checker stamps
  `intermediate_type` (binop working type) and `data_type` (lval
  type, the result type).
- Prefix `++a`/`--a` → `Prefix(incdec_op, exp)`; postfix `a++`/`a--`
  → `Postfix(incdec_op, exp)`. Separate nodes (not desugars) for
  the same address-once reason and because prefix returns the new
  value, postfix the old.
- `FunctionCall(name, args)` carries the callee as a string, not an
  expression — no function-pointer call form yet.
- Loop / switch / case / default / labeled nodes carry an
  `identifier label` field the parser leaves empty; loop_labeling /
  label_resolution fill it in later. `SwitchStmt` also has `cases`
  (list of `(value, label)` pairs), `default_label`, and a
  `promoted_type` filled by the type checker.
- Case-label expressions go through a `constant_exp` wrapper around
  `conditional_exp` — the wrapper is a hook for the §6.6 validator
  in `passes.constant_expression`.
- Array declarators (`int a[10];`) and function-pointer declarators
  (`int (*fp)(int);`) parse but `_apply_direct_declarator` raises
  NotImplementedError on the array suffix — c99_ast has no Array
  variant yet.
- The assignment LHS is loosened from C99's `unary-expression` to
  `conditional_exp`, so `1+2=3+4` parses — identifier resolution
  rejects non-lvalue forms.
- LALR(1) shift-reduce resolutions worth knowing: dangling-else
  binds to the nearest `if`; `IDENTIFIER COLON` at statement-start
  picks the labeled-statement branch; `(` followed by a type-
  specifier token picks the cast branch.

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
`tac_ast`. See the module docstring at the top of `c99_to_tac.py`
for the program shape, the two-pass build (AST walk for function
definitions; symbol-table sweep for static-storage initializers),
the per-construct C99→TAC mapping table, the short-circuit
lowerings, and the cross-cutting invariants downstream passes depend
on (typed temporaries registered in the symbol table with the
surrounding expression's `data_type`; const variants that carry
width AND signedness; `@N.orig` parameter naming preserved from
identifier_resolution; implicit `Ret(0)` appended at fall-off;
pointer-arithmetic byte-scaling via `translate_pointer_arithmetic`).

## tac_to_asm

`tac_to_asm.translate_program` — `tac_ast` → `asm_ast`. See the
module docstring at the top of `tac_to_asm.py` for the per-construct
mapping, the runtime helper layout table (HARGS slots per helper),
the inline comparison / LogicalNot lowerings, the cast lowering
specifics, and the caller-/callee-side calling convention. Two
cross-cutting facts other docs / readers need at orientation level:

- **The asm IR is strictly 1:1 with 6502 opcodes** (the documented
  exceptions are `Ret`, `FunctionPrologue`, and `AllocateStack`,
  which `asm_to_asm2` expands into atoms before emit). No width
  tagging — the 6502 is an 8-bit machine, every operand is one
  byte. That makes `tac_to_asm` the single home of all multi-byte
  lowering: for each TAC instruction whose operands are wider than
  1 byte, the translator emits a sequence of byte-level asm atoms,
  typically one pass per byte with the 6502's carry flag threading
  naturally between them. (FP arithmetic isn't lowered inline; it
  dispatches to runtime helpers via the HARGS block.)
- **HARGS** is the runtime-helper exchange block — 24 bytes of
  zero-page at `$04`–`$1B`. Caller writes inputs at fixed offsets,
  JSRs the helper, reads outputs from later offsets. The same block
  doubles as the wide-return slot for `LongLong` / `Float` / `Double`
  return values (`HARGS+8..11` and `HARGS+16..23`). HARGS is
  caller-saved, so any HARGS-returned value MUST be captured
  immediately after the JSR.

## asm_emit

`asm_emit.emit_program` — `asm2_ast` → 6502 assembly text. See the
module docstring at the top of `asm_emit.py` for the formatting
rules, the per-atom emit table (Mov / Add / Sub / Compare / ASL /
LSR / ROL / ROR / Inc / Dec / Push / Pop / Xor / And / Or / Call /
Jump / Branch / Label / BitTest etc.), the Data / ZP operand
addressing modes, the self-Mov peephole, and the StaticVariable
init-variant directives (DC.B / DC.W / DC.L / two DC.Ls). One
cross-cutting fact worth keeping at orientation level: every
asm2_ast node maps to exactly one 6502 instruction (addressing-mode
setup like the `LDY` for indirect-Y counts as part of the opcode);
the compound nodes from `asm_ast` (`Ret`, `FunctionPrologue`,
`AllocateStack`) and any leftover `Pseudo` operands are
already-resolved by `asm_to_asm2` and `replace_pseudoregisters`
before emit runs, and emit raises if one slips through.

## Function stack frame (soft stack)

Arguments and locals live on a **soft data stack** in main RAM,
separate from the 6502's hardware stack at `$0100`–`$01FF` (which is
reserved for return addresses and short-lived `PHA`/`PHP`). This
dodges the 256-byte page-1 limit and keeps return addresses out of
the way during frame teardown.

Reserved zero-page: `$00`/`$01` = `SSP` (soft stack pointer, low/
high), `$02`/`$03` = `FP` (frame pointer), `$04`–`$1B` = `HARGS`
(24-byte runtime-helper exchange block — see `tac_to_asm.py`'s
module docstring for each helper's per-byte slot table; the block
is sized for the largest helper, `dadd`/`dsub`/`dmul`/`ddiv`, with
16 bytes of inputs + 8 of output). `SSP` and `FP` both point at the **next-free
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
