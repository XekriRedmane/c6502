# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project overview

c6502 is a C99 compiler written in Python that targets the MOS 6502. Dependencies
are managed with `uv`; `pyproject.toml` is the source of truth and `uv.lock` the
resolved set. `requirements.txt` is a hand-maintained `pip`-compatible fallback
and may lag.

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
`asm2.asdl`) shape the data passed between passes. `tac_ast`
carries width AND signedness on integer types; `asm_ast` is
byte-typed (one IR atom per 6502 opcode, with `Pseudo` operands
that `replace_pseudoregisters` resolves); `asm2_ast` is `asm_ast`
with the three compound nodes (`FunctionPrologue` / `AllocateStack`
/ `Ret`) already expanded into atoms.

Two key runtime conventions:

  * **Soft data stack** at `SSP`/`FP` (zero-page pointers, both grow
    downward); the 6502 hardware stack at `$0100-$01FF` is reserved
    for JSR return addresses and short-lived PHA/PLA. Reserved ZP:
    `$00-$01` SSP, `$02-$03` FP, `$04-$23` HARGS (runtime-helper
    exchange block), `$24-$25` DPTR (caller-saved indirect-pointer
    scratch).
  * **`__attribute__((zp_abi))` + call-graph-disjoint private pools**
    put params AND body locals in ZP for eligible leaf / non-
    recursive / non-indirect-calling functions; ineligible functions
    fall back to the default caller/callee partition (`$80..$BF` /
    `$C0..$FF`). Eligible functions emit as bare body + RTS. See
    the "Call-graph-disjoint ZP allocation" section.

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

`compile.py` is the only CLI; every other module is library-only. Flags it doesn't
recognize are forwarded to the preprocessor (pcpp), so `-D`, `-U`, `-I`,
`--passthru-*`, `--line-directive` etc. work the same as the `pcpp` CLI. pcpp's
own `-o` is not forwarded.

Stage-selection flags (mutually exclusive, one required with `compile.py`):
`--lex`, `--parse`, `--resolve`, `--tac`, `--codegen`. `--resolve` runs
the three name-resolution passes (identifier resolution, label resolution,
loop labeling) in that order.

Modifier flags (orthogonal to the stage flags; both apply to
`--tac` and `--codegen`):

- `--optimize` runs the optimizer pipeline:

  1. TAC SSA construction (`passes.optimization.ssa_construction`).
  2. One-shot scalar const-static read fold — replaces `Var(static
     const scalar)` USE positions with `Constant(value)`.
  3. TAC fixed-point loop: constant folding (incl. const-array-
     subscript fold), strength reduction, comparison-against-zero
     / jump fold, AND-zero-jump fold, unreachable-code elimination,
     copy propagation, dead-store elimination, copy folding,
     Add-with-Constant reassociation, loop rotation, sink-increment,
     IndexedLoad / IndexedStore / IndirectIndexed recognizers.
  4. TAC SSA destruction + post-destruction copy folding.
  5. Asm-level const-static fold (drops scalar `static T const`
     storage, replacing references with immediates).
  6. Asm-level SSA round-trip: move coalescing, forward + backward
     copy propagation, byte-granular DCE, byte-granular regalloc
     drawing from per-function private pools (when eligible) or
     the default caller/callee partition.
  7. `replace_pseudoregisters_bare_exit` resolves Pseudos.
  8. Late prologue / epilogue synthesis (collapses to bare body +
     RTS when no save / spill is needed).
  9. `loop_counter_to_x` (X-pivot promotion for hot loop counters).
  10. Peephole fixed-point loop — see the "Peephole catalog" section.
  11. `expand_long_branches`, `asm_to_asm2`, `emit_program`.

  Also enables `__attribute__((zp_abi))` and the call-graph-disjoint
  body-local allocator. The INC / DEC peepholes run in the
  unoptimized pipeline too (their win is addressing-mode-aware, not
  regalloc-dependent).

- `--unroll` (only meaningful with `--optimize`) runs `passes.
  optimization_ast.unroll.unroll_program` after parsing and before
  identifier resolution. Every for-loop carrying `#pragma c6502
  loop unroll(enable)` (with the canonical `init=const; cond=var<const;
  step=var=var+const` shape and a compile-time-known iteration
  count) is fully unrolled in place. Unrolling before name
  resolution means each unrolled body gets fresh per-iteration
  identifier renames; unrolling-then-typechecking sidesteps having
  to fix up SymbolTable entries.

Linker mode:

- `compile.py --link <a.asm> <b.asm> ... -o out.asm` — multi-TU
  globally re-allocates `__zpabi_*` and `__local_*` ZP slot
  symbols across the supplied per-TU outputs (each produced by
  `compile.py --codegen --optimize`). Reads each input's
  `; @zp-link-meta-begin` / `; @zp-link-meta-end` block (emitted
  by `passes.zp_link_metadata`) to recover the call-graph and
  param/local sizes needed for the global allocation. See
  `passes/linker.py`.

See the "Optimization pipeline" section below and
`docs/optimization.md` / `docs/leaf_zp_abi.md` for the full
walk-throughs.

## Regenerating AST modules

Each `*_ast.py` module is generated from its matching `*.asdl` by `asdl.py`.
After editing an ASDL file, regenerate:

```sh
uv run python asdl.py c99.asdl c99_ast.py
uv run python asdl.py tac.asdl tac_ast.py
uv run python asdl.py asm.asdl asm_ast.py
uv run python asdl.py asm2.asdl asm2_ast.py
```

The generator emits one `@dataclass` per type. Sum-type bases are named
`Type_<name>` (to avoid colliding with Python builtins like `int`);
constructor classes keep their ASDL names. Fields become `int`, `str`,
`list[...]`, or `T | None` depending on the primitive / `*` / `?` markers.

## Compiler pipeline

`compile.py --codegen` chains eleven passes, each a separate module that
takes one AST and returns another (or text for emit):

1. `parser.parse` (`parser.py`) — C source → `c99_ast`. Lark/LALR grammar
   lives in `c99.lark`. The top-level production is `declaration*`:
   a translation unit is a list of `var_decl` / `function_decl` forms.
   The AST stores them as `Program(declaration*)` where each declaration
   is `VarDecl(var_decl)` or `FunctionDecl(function_decl)`; a function
   *definition* is the FunctionDecl variant with `body=Block(...)`,
   while a forward declaration has `body=None`. Both `var_decl` and
   `function_decl` carry a required `data_type` (one of `Int()`,
   `Long()`, `UInt()`, `ULong()`, `Float()`, `Double()`, or — for
   functions — `FunType(params, ret)`) and an optional
   `storage_class` (`Static()` / `Extern()` / None). The parser
   builds the function's `FunType` from the per-param
   `type_specifier+` runs and the return-type specifiers. A
   `<param_list>` is `void` (empty params) or comma-separated
   `<type_specifier>+ IDENT` pairs. Parameter *names* live on the
   function_decl's `params` array; their *types* live in parallel
   on `data_type.params`.

   The type vocabulary is nine integer types, two floating types,
   plus pointers and function types. Widths follow C99's minimum
   ranges per §5.2.4.2.1 — Int is 16 bits, Long is 32 bits, LongLong
   is 64 bits. Integers: `Int()` is 2-byte signed (-32768..32767),
   `Long()` is 4-byte signed (-2^31..2^31-1), `LongLong()` is
   8-byte signed (-2^63..2^63-1), `UInt()` is 2-byte unsigned
   (0..65535), `ULong()` is 4-byte unsigned (0..2^32-1),
   `ULongLong()` is 8-byte unsigned (0..2^64-1), `Char()` is
   1-byte unsigned (0..255; plain `char` is unsigned in c6502 per
   C99 §6.2.5.15's implementation-defined choice — same byte
   semantics as `unsigned char`), `SChar()` is `signed char`
   (1-byte signed, -128..127), `UChar()` is `unsigned char`
   (1-byte unsigned, 0..255). Char/SChar/UChar are distinct types
   from Int/UInt — C99 §6.3.1.1.2 integer-promotes them all to
   `int` (because Int's 16-bit range covers both signed and
   unsigned char), so arithmetic always happens at Int width or
   wider.
   Floating: `Float()` is IEEE 754 single (4 bytes), `Double()`
   is IEEE 754 double (8 bytes). `long double` (16-byte IEEE 754
   quad / extended) isn't modelled — the parser rejects it.
   `Pointer(referenced_type)` is a 2-byte address (the 6502's
   address width); declared with `*` in the declarator, e.g. `int
   *p;`. Pointer is its own TAC variant (`tac_ast.Pointer`) because
   2-byte pointers no longer match any integer width — Int is now
   2 bytes too, but conceptually distinct, and the symbol table
   preserves the Pointer type for the rare codegen sites that
   inspect it (signedness checks for unsigned ordering, pointer-
   arithmetic scaling).

   The lexer splits integer literals into four terminals
   by suffix (`INTEGER_CONSTANT` for no suffix, `LONG_INTEGER` for
   `L`/`LL`, `UINT_INTEGER` for `U`-only, `ULONG_INTEGER` for `U+L`
   in any order — `LL` shares a terminal with `L`, and `ULL` with
   `UL`; the parser's `has_ll` flag then routes `LL`/`ULL` cases
   into separate candidate-list rows). Floating literals split
   into three (`DOUBLE_CONSTANT` for no suffix, `FLOAT_CONSTANT`
   for `f`/`F`, `LONG_DOUBLE_CONSTANT` for `l`/`L`). The parser's
   `_const_for_token` then maps each integer token + base (decimal
   vs. hex/octal) to a c99 const variant per the C99 §6.4.4.1
   paragraph 5 type-list rule (first type whose range fits the
   value):
   * unsuffixed decimal:    int → long → long long
   * unsuffixed hex/octal:  int → unsigned int → long → unsigned
                             long → long long → unsigned long long
   * `L` decimal:           long → long long
   * `L` hex/octal:         long → unsigned long → long long →
                             unsigned long long
   * `LL` decimal:          long long
   * `LL` hex/octal:        long long → unsigned long long
   * `U`:                   unsigned int → unsigned long → unsigned
                             long long
   * `UL` (any letter order): unsigned long → unsigned long long
   * `ULL` (any letter order): unsigned long long

   Picking the matching const variant from those lists. A literal
   whose value exceeds `unsigned long long` (the widest type c6502
   models) is rejected with "doesn't fit any supported type".
   Floating literals follow C99
   §6.4.4.2 — the suffix uniquely determines the type, no
   value-fitting rule:
   * unsuffixed: `ConstDouble`
   * `f` / `F`:  `ConstFloat`
   * `l` / `L`:  rejected ("long double not supported")

   Hex floating literals (`0x1.0p3`) lex but the parser rejects
   them — Python's `float()` doesn't parse the C hex-float syntax,
   so wiring up support would mean writing the conversion by hand;
   deferred until something in the corpus needs them.

   `Constant(const)` wraps the resulting `Type_const`.
   `_split_specifiers` validates the run of specifier tokens
   (`int`, `long`, `signed`, `unsigned`, `float`, `double`,
   `static`, `extern`) and splits it into the
   `(data_type, storage_class)` pair. `_resolve_data_type` decodes
   the C99 §6.7.2 combinations c6502 supports (`int` /
   `signed [int]` → Int; `unsigned [int]` → UInt; `long [int]` /
   `signed long [int]` → Long; `unsigned long [int]` → ULong;
   `long long [int]` / `signed long long [int]` → LongLong;
   `unsigned long long [int]` → ULongLong; `char` → Char;
   `signed char` → SChar; `unsigned char` → UChar; `float` →
   Float; `double` → Double), rejecting multiple type specifiers,
   multiple storage classes, missing type, three or more `long`s,
   `char` combined with `int` / `long` / `short`, `long double`,
   `signed unsigned`, and any FP/integer specifier mix (`int
   float`, `unsigned double`, etc.).
   A `<block>` is `{ <block_item>* }` (its own AST product type
   `Block(block_item*)` so a function body is `Function(name,
   Block([...]))`). A block item is a declaration (`var_decl` or
   `function_decl`) or a statement (`return exp;`, `exp;`, `if (exp)
   stmt (else stmt)?`, `goto label;`, `label: stmt`, a `<block>`
   (compound statement, `Compound(block)`), `break;`, `continue;`,
   `while (exp) stmt`, `do stmt while (exp);`,
   `for (<for_init> exp? ; exp?) stmt`, or a null `;`). The two
   declaration alternatives map to the AST sum
   `declaration = FunctionDecl(function_decl) | VarDecl(var_decl)`:
   `var_decl` is `<specifier>+ <declarator> (= exp)? ;` and
   `function_decl` is `<specifier>+ <declarator> <block>` (the
   function-definition path; forward declarations like `int foo(int
   x);` parse as `var_decl` because the trailing `;` matches that
   rule, and the var_decl transformer rewraps as a `Type_function_
   decl` with `body=None` whenever the declarator composes to a
   FunType). The transformer walks the declarator parse tree (per
   C99 §6.7.5: postfix array / function suffixes bind tighter than
   prefix `*`) via `_apply_declarator`, returning `(name,
   composed_type, outer_param_names)`. Composed type accumulates
   `Pointer` wrappers from the pointer prefix and `FunType`
   wrappers from function suffixes; the outermost function-suffix's
   param names ride along separately for the AST's `params` field
   (which holds names alongside the function's `data_type` =
   `FunType` carrying param types). `int *p;` → `Pointer(Int())`;
   `int *foo(int *x)` → `FunType(params=[Pointer(Int())],
   ret=Pointer(Int()))`. Array declarators (`int a[10];`) and
   function-pointer declarators (`int (*fp)(int);`) parse but
   `_apply_direct_declarator` raises NotImplementedError when it
   hits an array suffix — c99_ast has no Array variant yet. Iteration statements introduce a `for_init` rule
   covering a `var_decl` or `exp? ;` (function declarations aren't
   legal in for-init per C99 §6.8.5). The loop AST nodes (`WhileStmt`, `DoWhileStmt`, `ForStmt`,
   `BreakStmt`, `ContinueStmt`) carry an `identifier label` field that
   the parser leaves as the empty string — the loop_labeling pass
   fills it in later. Selection / case statements (`SwitchStmt`,
   `CaseStmt`, `DefaultStmt`) carry the same kind of `label` field;
   `SwitchStmt` additionally has `cases` (a list of
   `(value, label)` pairs collected from its body), an optional
   `default_label`, and an optional `promoted_type` (filled by the
   type checker — see pass 5). The case-label expression goes
   through a `constant_exp` non-terminal that's a one-child
   wrapper around `conditional_exp`; the wrapper exists so the
   site is self-documenting and shares a §6.6 validator across
   future call sites (enums, array sizes, etc. — see
   `passes.constant_expression`). The case / default rules
   introduce their own COLON shift-reduce situation analogous to
   labeled statements; LALR(1) shift resolves it correctly. The compound-
   statement rule reuses the same `block` rule the function body uses
   — the only difference is the transformer wraps the resulting
   `Block` in a `Compound`. The `IDENTIFIER COLON statement` rule
   for labeled statements introduces a shift-reduce conflict at
   statement-start on COLON lookahead — Lark's LALR(1) backend resolves
   it by shifting (same mechanism that handles dangling-else), which
   picks the labeled-statement branch. Inside an expression (e.g. a
   ternary's true-clause) the parser state is different, so `a ? b : c`
   continues to parse as a Conditional even though `b` is also an
   IDENTIFIER followed by COLON.
   The dangling-else ambiguity is resolved by Lark's LALR(1) backend
   preferring shift, which binds `else` to the nearest preceding
   unmatched `if` (the C99 §6.8.4.1 rule). `<exp>` covers integer constants,
   identifiers, casts (`(int)x` / `(long)x`), unary `-`/`~`/`!`,
   binary `+`/`-`/`*`/`/`/`%`/bitwise/shift/
   comparison/`&&`/`||`, parentheses, right-associative `=`, and the
   ternary `cond ? t : f`. Cast expressions sit at their own
   `cast_exp` level between `unary_exp` and `mul_exp` (C99 §6.5.4),
   right-recursive so `(int)(long)x` parses as `(int)((long)x)`. The
   `mul_exp` recursive RHS and the unary-operator alternative both
   take `cast_exp`, so `-(int)x` parses as `-((int)x)`; prefix `++`/
   `--` keep their `unary_exp` operand because the cast result isn't
   an lvalue (so `++(int)x` is a parse error). The LPAREN-vs-paren-
   expr ambiguity is resolved at LALR(1) by the next token after `(`:
   any type-specifier token (`INT`, `LONG`, `SIGNED`, `UNSIGNED`,
   `FLOAT`, `DOUBLE`) → cast; anything else → parenthesised exp.
   Each `Cast(target_type, exp)` carries a resolved object-type
   target (built by the `type_name: type_specifier+` rule, which
   reuses `_resolve_data_type`).
   The assignment LHS is loosened from C99's
   `unary-expression` to `conditional_exp`, so `1+2=3+4` and
   `(1?2:a)=5` both parse — identifier resolution rejects the non-lvalue
   forms. The ternary sits at its own `conditional_exp` level between
   assignment and logical-or (C99 §6.5.15): condition is
   `logical_or_exp`, true-clause is full `exp`, false-clause is
   `conditional_exp`. The right-recursion makes `?:` right-
   associative (`a ? 1 : b ? 2 : 3` is `a ? 1 : (b ? 2 : 3)`) and
   keeps assignments out of the false-clause slot, so
   `1 ? 2 : a = 5` parses as `(1 ? 2 : a) = 5` via the outer
   assignment rule (and then fails the lvalue check).
   The ten compound assignments (`+=`, `-=`, `*=`, `/=`, `%=`, `&=`, `|=`,
   `^=`, `<<=`, `>>=`) share a single `compound_assign` builder that
   builds a `CompoundAssignment(op, lval, rval, intermediate_type?,
   data_type?)` AST node — NOT a parse-time desugar to `Assignment(lval,
   Binary(OP, lval, rval))`. The explicit node is what lets c99_to_tac
   evaluate the lval's address ONCE before the read-modify-write, which
   matters for Subscript / Dereference / Dot / Arrow lvals whose address-
   computing subexpressions have side effects (`arr[i++] += 1`,
   `(*p++)++`, `ptr++[idx++] *= 3`); a desugar would duplicate the lval
   and fire those side effects twice. The type checker stamps two types
   on the node: `intermediate_type` is the binop's working type — common-
   of-promoted for arithmetic / bitwise per §6.3.1.8, promoted-left alone
   for shifts per §6.5.7.3 (right operand promotes independently),
   pointer-itself for `ptr += int` (which routes through
   `translate_pointer_arithmetic` in c99_to_tac for sizeof-pointee
   scaling) — and `data_type` is the lval's type, the result of the
   compound assign expression. c99_to_tac's `_translate_compound_assign`
   computes the lval's address once, Loads at the lval type, casts to
   intermediate_type, applies the binop, casts back to lval's type, and
   Stores. Var lvals skip the Load/Store (the Var IS the storage).
   Prefix `++a` / `--a` and postfix `a++` / `a--` each build their own
   AST node (`Prefix(incdec_op, exp)` / `Postfix(incdec_op, exp)`)
   instead of desugaring to assignment. Two reasons: (1) they have
   different result semantics — prefix returns the *new* value,
   postfix the *old* value, which can't be expressed by reusing
   `Assignment` / `Binary` alone; (2) direct nodes let `c99_to_tac`
   evaluate the operand's address ONCE before the read-modify-write,
   avoiding the side-effect duplication a desugared
   `arr[--i] = arr[--i] + 1` would cause for richer lvalues. Postfix
   sits at its own grammar level (`postfix_exp`) one tighter than
   `unary_exp`, so `-a++` parses as `-(a++)` and `++a++` as
   `++(a++)` (the inner `a++` isn't an lvalue, but identifier_
   resolution rejects that semantically — the grammar accepts it).
   Function calls `f(arg, ...)` sit at the atom level alongside
   constants, parenthesised expressions, and bare identifiers. The
   grammar uses `IDENTIFIER LPAREN arg_list? RPAREN -> function_call`
   (and `IDENTIFIER -> identifier` as a separate atom alternative);
   LALR(1) shifts on LPAREN to disambiguate between the two — bare
   `f` reduces to `Var("f")`, `f(x)` reduces to
   `FunctionCall(name="f", args=[...])`. The callee is a literal
   identifier, not an expression, because the AST node carries the
   name as a string (`FunctionCall(name, args)`) — no
   pointer-to-function call form yet. Arg expressions are full `exp`
   (assignment-level), separated by commas.
2. `passes.identifier_resolution.resolve_program` — `c99_ast` → `c99_ast`.
   Resolves every user-written identifier — variables and functions
   both — and tags it with its C99 §6.2.2 **linkage kind**, stored
   alongside the resolved name in the resolver's tables. Renaming
   is gated on linkage:
   - **`Linkage.NONE`** (block-scope automatic variables today —
     every `int x;` we accept) → mint a program-unique
     `@<N>.<orig>` (illegal in a C identifier, so it can't collide
     with user names) and record it in the per-block scope.
   - **`Linkage.EXTERNAL`** (every function declaration / definition
     today; later: `extern int x;` at file scope) → keep the source
     spelling, because the linker resolves these by name across
     translation units.
   - **`Linkage.INTERNAL`** (later: `static int x;` / `static int
     foo(void);` at file scope) → keep the source spelling, because
     later TU-local passes resolve these by name. Not produced
     today.
   The "rename only NONE-linkage names" rule replaces the older
   "rename variables, leave functions alone" heuristic — same
   behavior right now (every variable is NONE, every function is
   EXTERNAL), but the linkage-driven version slots in cleanly when
   `extern`/`static` land. A `VarDecl(Type_var_decl(name))` runs
   through `resolve_var_decl(... , linkage=Linkage.NONE)` today,
   which bumps the unique-name counter, mints `@<N>.<orig>`, and
   records `(resolved, inner=True, linkage=NONE)` in the per-block
   scope. Declaring the same variable name twice in the same block
   raises `IdentifierResolutionError`. A `FunctionDecl(Type_
   function_decl(name))` registers the name in a per-program
   `_functions: dict[str, Linkage]` (today always
   `Linkage.EXTERNAL`) without renaming — multiple declarations of
   the same function are legal and idempotent under dict-overwrite
   semantics. Top-level `Function(name, body)` definitions are
   pre-registered in the same dict before any body is walked, so a
   `FunctionCall` inside one function can resolve a target defined
   later in the file or be self-recursive. A `Var(name)` in any
   expression is rewritten to its mapped resolved name; referencing
   an undeclared variable raises (a function name on its own
   doesn't satisfy a `Var` lookup — c6502 has no function-pointer
   expressions yet). A `FunctionCall(name, args)` validates that
   `name` is in `_functions` (raises "call to undeclared function"
   if not), recursively resolves the args, and leaves `name` itself
   unchanged. The same lvalue check that gates `Assignment.lval`
   also gates `Postfix.operand` and `Prefix.operand`, so `1++` and
   `++1` raise just like `1 = 2`. The accepted lvalue forms are
   `Var`, `Dereference`, and `Subscript` (the three syntactic
   lvalues c6502 supports today); anything else raises "invalid
   lvalue" — `1+2=3`, `-a=5`, `(a=b)=c`, `++1` all fail here.
   **Parameters** are resolved exactly like NONE-linkage local
   variables: `_resolve_params` walks the parameter list, validating
   uniqueness within the list and minting a fresh `@<N>.<orig>` for
   each (the param scope built up is independent of the surrounding
   block scope, so `int a; int foo(int a);` is legal — the param
   `a` doesn't conflict with the outer variable `a`). For a
   `FunctionDecl` (no body), the renamed names are stored on the
   returned `Type_function_decl.params` and the param scope is
   discarded. For a function *definition* the param scope IS the
   body's outermost scope (C99 §6.9.1.7: "the parameters and the
   local variables of the function have the same scope"), so the
   body's block items resolve directly into it without the usual
   clone-flip — `int foo(int a) { int a = 3; ... }` raises
   duplicate-decl on the body's `int a`, while a nested
   `int foo(int a) { { int a = 3; ... } ... }` legally shadows via
   the inner Compound's own scope.
   Scope is per-block: each `Block` owns a `dict[str, tuple[str,
   bool, Linkage]]` mapping each visible user name to
   `(resolved_name, inner, linkage)`, where `inner` is True iff
   the name was declared in *this* block. Entering a nested block
   clones the parent's map and flips every entry's `inner` flag to
   False — linkage rides along unchanged, since linkage is fixed at
   the declaration site. A duplicate-decl error fires only when an
   already-inner-scoped entry would be overwritten; declaring a
   name that's currently outer-scoped legally shadows it
   (overwrite with a fresh entry, mint or reuse the spelling per
   the new entry's linkage, flag as inner). Exiting the inner
   block discards its dict — Python GC handles this since we
   cloned the parent's map rather than aliasing it. While/do-while bodies
   resolve in the parent scope (they don't introduce a scope of
   their own; a Compound body opens its own scope as usual). The
   for-header (`for (<init> ...) body`) opens a fresh scope per
   C99 §6.8.5.3, so `int a; for (int a = 1; a < 10; a++) ...`
   shadows the outer `a` for the duration of the loop and the
   outer `a` is intact afterward. `switch` doesn't introduce a
   scope of its own (a Compound body does, as usual); the
   controlling expression and `case` / `default` bodies all
   resolve in the surrounding scope. Labels, gotos, break,
   continue, and `case` / `default` labels themselves all pass
   through unchanged — they live in separate namespaces and are
   owned by later passes (label_resolution for user labels;
   loop_labeling for break / continue / case / default).
3. `passes.string_lifting.lift_program` — `c99_ast` → `c99_ast`.
   Hoists every `String` literal whose context is NOT a direct
   char-array initializer (`char arr[N] = "abc";` keeps its
   String inline) into a fresh file-scope `static char[N+1]`
   declaration, replacing the original `String` with a `Var`
   referencing the new declaration. The minted name is
   `.str@<N>` (leading `.` and `@` keep it disjoint from any
   user identifier and from translator-minted labels). After
   lifting, every other use of a string literal — `&"abc"`,
   `"abc"[1]`, `char *p = "abc"`, `return "abc";` — works
   through the same mechanisms as any other file-scope char
   array (decay to `char *`, AddressOf-of-array, subscript,
   ...) without per-pass special cases. Runs AFTER
   identifier_resolution so the lifted names use a disjoint
   character (`.`) and don't get re-renamed; runs BEFORE
   label_resolution / loop_labeling / type_checking so those
   passes see the rewritten AST.
4. `passes.label_resolution.resolve_program` — `c99_ast` → `c99_ast`.
   Validates labeled statements (C99 §6.8.1) and `goto` targets
   (§6.8.6). Two walks per function: (a) collect every `LabeledStmt`,
   minting a unique name `.<funcname>@<orig>` per label and rejecting
   duplicates; (b) rewrite the AST, replacing each label and matching
   `Goto` target with the unique name and raising
   `LabelResolutionError` for any goto whose target wasn't declared in
   the same function. Labels are visible across the whole function
   (forward gotos are fine), so both walks descend into the bodies
   of `if`, compound, while, do-while, and for statements. The
   leading `.` makes them dasm-style **local labels**, scoped only
   to the SUBROUTINE the asm emits — so two functions can both have
   a label `foo` without colliding in the global asm namespace. The
   `@` separator (illegal in a C identifier, so it can't appear in
   `<funcname>` or `<orig>`) keeps user labels disjoint from
   translator-minted labels (which all carry `@<digits>`, e.g.
   `.if_end@N`, `.loop@N`) and from any user-written identifier.
   C99 §6.8.6 also forbids jumping into the scope of a variably-
   modified-type identifier; c6502 has no VLAs, so that constraint
   is vacuously satisfied.
5. `passes.loop_labeling.label_program` — `c99_ast` → `c99_ast`.
   Mints a unique label per iteration statement (`.loop@<N>`) and
   per `switch` statement (`.switch@<N>`), stamping it onto that
   statement's `label` field. While walking the body, the pass
   threads two pieces of state per C99 §6.8.6:
   - `current_loop` — innermost iteration statement's label, used
     to resolve `continue` (§6.8.6.2 — only iteration statements).
   - `current_break_target` — innermost iteration *or* switch
     label, used to resolve `break` (§6.8.6.3 — both kinds).
   Iteration statements push to both; switch pushes only to
   `current_break_target` (so `continue` inside a switch inside a
   loop still finds the loop). A third bit of state,
   `current_switch`, holds the innermost enclosing switch's case-
   collector — `case <e>:` and `default:` nodes encountered during
   the walk mint their own labels (`.case@<N>` / `.default@<N>`),
   stamp them onto the AST node, and append to that switch's
   `cases` / `default_label` fields. Case labels can sit inside
   if / loop / compound bodies inside a switch (Duff's-device-
   style), so iteration / if / compound nodes preserve
   `current_switch`; only a nested SwitchStmt swaps in a fresh
   collector for its own body. Errors (`LoopLabelingError`):
   `break;` outside any iteration / switch; `continue;` outside
   any iteration; `case` / `default` outside any switch; duplicate
   `default:` within one switch (case-value uniqueness is checked
   later, in the type-checking pass). The pass runs *after*
   label_resolution: loop / switch / case / default labels are
   translator-minted, not user-written, so they slot in only once
   user-defined goto / labeled-stmt names have already been
   resolved. The namespaces are disjoint by construction — a user
   label is `.<funcname>@<orig>` where the part after `@` is a C
   identifier; a loop / switch / case / default label is
   `.{loop,switch,case,default}@<N>` where the part after `@` is
   digits, so the two forms can't ever match. Codegen derives
   concrete control-flow targets for iteration statements by
   appending suffixes (`_start`, `_continue`, `_break`) to the
   loop's base label; switches use only the `_break` suffix (the
   dispatch chain emits the case / default labels directly).
6. `passes.type_checking.check_program` — `(c99_ast, SymbolTable)`.
   Walks the AST once and produces a `SymbolTable` (a `dict[str,
   Symbol]` keyed by resolved identifier name). The data-type
   classes (`Int`, `Long`, `LongLong`, `UInt`, `ULong`, `ULongLong`,
   `Float`, `Double`, `FunType`) live on `c99_ast` and are
   re-exported here under stable `passes.type_checking.<Name>`
   names so every consumer agrees on the type vocabulary;
   equality is structural via `@dataclass`. Each `Symbol` carries
   a `type` plus an `IdAttr` describing its runtime category:
   - `LocalAttr` — automatic-storage object (block-scope `int x;`
     / `long x;` / `long long x;` / `unsigned int x;` / `unsigned
     long x;` / `unsigned long long x;` / `float x;` / `double x;`,
     function parameter, or any TAC temporary introduced by
     `c99_to_tac`).
   - `StaticAttr(initial_value, is_global)` — every file-scope
     object plus block-scope `static`. `initial_value` is one of
     `Initial(c)`, `Tentative`, or `NoInitializer` per C99 §6.9.2.
     `Initial.value` is `int` for integer types and `float` for
     floating types.
   - `FunAttr(defined, is_global)` — a function name. `defined`
     flips True the first time a definition is seen.
   `is_global` is True iff the symbol has external linkage,
   materialized once here so the asm backend doesn't have to re-
   derive it from the three-way `Linkage` enum.

   The pass mutates each visited expression's `data_type?` field in
   place — every `Constant` / `Var` / `Cast` / `Unary` / `Binary` /
   `Assignment` / `Postfix` / `Conditional` / `FunctionCall` ends up
   tagged with its concrete result type. Constants pick from the
   const variant (ConstInt → Int, ConstLong → Long, ConstLongLong
   → LongLong, ConstUInt → UInt, ConstULong → ULong, ConstULongLong
   → ULongLong, ConstFloat → Float, ConstDouble → Double); Cast
   picks its target_type; Var picks the symbol's type; Unary /
   Postfix inherit the inner operand's type, except `!` which
   always yields Int per §6.5.3.3.5.

   **Integer promotion** (C99 §6.3.1.1.2) runs FIRST at each
   operand position of an arithmetic / bitwise / comparison /
   shift operator (and on the operand of unary `-` / `~`).
   Char-typed operands promote to `Int` (when Int can represent
   the source's range) or `UInt` (otherwise, which in c6502
   means UChar — Int -128..127 doesn't cover UChar 0..255):
   * SChar / Char → Int (same range and signedness; same-width
     no-op Cast that c99_to_tac elides at lowering)
   * UChar       → UInt
   Other integer types already have rank ≥ Int and pass through
   unchanged.

   **Implicit conversions** apply C99 §6.3.1.8's usual arithmetic
   conversions to the post-promotion operand types. Floating
   types dominate per §6.3.1.8.1:
   * either operand `Double` → result `Double`
   * else either operand `Float` → result `Float`
   * else both operands integer → integer rules (below)

   Integer rules, keyed by C99 §6.3.1.1 conversion rank (`Int` and
   `UInt` are rank 1; `Long` and `ULong` are rank 2; `LongLong`
   and `ULongLong` are rank 3 — char types are below Int but never
   participate in the common-type computation, since integer
   promotion has already lifted them to Int / UInt):
   * matching types → that type
   * both signed (or both unsigned) → the higher-rank type wins
     (Int+Long → Long, Long+LongLong → LongLong, UInt+ULongLong →
     ULongLong)
   * mixed signedness, unsigned has rank ≥ signed → unsigned wins
     (Int+UInt → UInt, Int+ULong → ULong, Long+ULongLong →
     ULongLong)
   * mixed signedness, signed has higher rank and can represent
     all unsigned values → signed wins (Long+UInt → Long;
     LongLong+UInt → LongLong; LongLong+ULong → LongLong, since
     LongLong's -2^31..2^31-1 covers ULong's 0..65535)

   The narrower or signed-displaceable operand is wrapped in an
   implicit `Cast(target=common, exp=…, data_type=common)` via
   `_convert_to(exp, target)`, so by the time TAC sees the tree
   every operand has its concrete data_type and any size- or
   signedness-changing conversion is an explicit Cast node. The
   same `_convert_to` helper runs at every place C99 specifies a
   conversion:
   - **Binary** operands (§6.3.1.8): both promoted to the common
     type before the op (except shifts — see below).
   - **Shift operands** (§6.5.7.3): each operand integer-promotes
     independently; the result type is the promoted left operand's
     type. The right keeps its own promoted type — c99_to_tac's
     shift-helper path passes only its low byte to asl/asr/lsr.
   - **Assignment** rval (§6.5.16.1): converted to lval's type.
   - **CompoundAssignment** (§6.5.16.2): rval converted to the
     intermediate type stamped on the node (common-of-promoted, or
     promoted-left for shifts); the lval-load and binop-result
     casts to/from the lval's type are emitted by c99_to_tac.
   - **FunctionCall** args (§6.5.2.2.7): each arg converted to the
     corresponding parameter's type.
   - **Return** value (§6.8.6.4.3): converted to the enclosing
     function's declared return type (tracked on
     `self._return_type` while walking each body).
   - **Variable initializers** (§6.5.16.1): block-scope auto,
     block-scope `static`, file-scope, and for-init declarations
     all run through the same conversion.
   Comparisons (`==`/`!=`/`<`/`>`/`<=`/`>=`) and `&&`/`||` always
   yield Int regardless of operand type, but their operands still
   go through the promotion so the underlying op happens at one
   width. Conditional `?:` uses the rule on its true/false branches.

   **Pointer arithmetic** (C99 §6.5.6) takes its own path on
   `Binary(Add | Subtract)` when at least one operand is a Pointer,
   sidestepping `_common_type` (which can't construct a Pointer
   without a `referenced_type`). Four legal shapes:
   * `ptr + int` / `int + ptr` → result is the pointer type.
   * `ptr - int` → result is the pointer type.
   * `ptr - ptr` (matching pointer types) → result is `Long`,
     c6502's stand-in for the standard's `ptrdiff_t`.
   For the first three the integer operand is wrapped in an
   implicit `Cast(Long)` (matching the pointer's 2-byte width), so
   by the time TAC sees the Binary every operand is 2 bytes wide.
   The actual scaling by `sizeof(pointee)` lives in `c99_to_tac` —
   see step 7 below. Rejected at the type-check boundary:
   `ptr + ptr`, `int - ptr` (which catches `0 - p`), `ptr ±
   floating`, `ptr - ptr` with mismatched pointer types, and any
   additive op on a function pointer (§6.5.6.2 requires "pointer
   to an object type").
   Ordering comparisons on pointers (`<`/`>`/`<=`/`>=`, §6.5.8)
   take their own short-circuit path before `_common_type`, which
   would crash on Pointer for the same reason as equality. The
   constraint is stricter than equality's: both operands must be
   pointers to compatible object types — null pointer constants
   aren't accepted on the relational ops. Result is always Int per
   §6.5.8.6. `tac_to_asm` dispatches pointer ordering to its own
   unsigned-ordering lowering (per-byte SBC with carry threading,
   then BCC/BCS — no V-correction), so addresses above $8000 rank
   correctly. `>` / `<=` swap operands the same way the signed
   form does. Non-pointer ordering still uses the signed
   V-corrected sequence (a known limitation for `unsigned long`
   operands).

   **Array-to-pointer decay** (C99 §6.3.2.1.3) is reified by
   `_decay_if_array(exp)`: if `exp.data_type` is `Array(elem, N)`,
   wrap `exp` in an implicit `AddressOf` stamped with
   `Pointer(elem)` and return the wrapper. Each call site that
   consumes an expression — Binary / Conditional / Cast inner /
   Assignment rval / FunctionCall arg / Return value / var
   initializer / Subscript array operand / Dereference operand —
   is responsible for decaying its inputs before further type
   checking; the `_is_object_type` predicate excludes Array, so any
   missed decay site fails as a non-object-type error rather than
   silently producing nonsense. The wrapper type is narrower than
   the standard's `Pointer(Array(elem, N))` (we use `Pointer(elem)`,
   the address of the array's first element) — equivalent at the
   byte level since both are the same 2-byte address, and downstream
   pointer-arithmetic scaling reads the pointee from `Pointer.referenced_type`.
   User-written `&arr` for an array DOES yield the standard
   `Pointer(Array(elem, N))`; both forms work end-to-end because
   `_to_tac_data_type` collapses `Pointer` onto `Long` and
   `_pointee_size` recurses into `Array` for the scale factor.

   **Subscript** (`Subscript(array, index)`) is type-checked but
   left in the AST for `c99_to_tac` to lower (rather than rewritten
   here to `Dereference(Binary(Add, decayed, index))`, which would
   require every parent slot to reassign). Per C99 §6.5.2.1.2 the
   subscript operands are symmetric — `E1[E2]` is defined as
   `*((E1)+(E2))`, so either side may be the pointer/array and the
   other side the integer. The type checker accepts both `arr[3]`
   and `3[arr]`, swapping operands when needed so the canonical
   AST always has `Subscript.array` holding the pointer side. The
   array operand decays to Pointer; the index is widened to Long;
   the result type is the pointee. Pointer-typed array operands
   (`p[i]` where `p` is a pointer) skip the decay step but go
   through the same downstream lowering.

   **Variable declarations** distinguish two predicates:
   `_is_object_type` (the operand-allowed set: arithmetic types and
   Pointer) and `_is_complete_object_type` (adds Array). Var-decl
   sites use the broader predicate since `int a[10];` IS a legal
   declaration; arithmetic / operand sites use the narrower one
   because arrays must decay first.

   Static-storage initializers stay constant-expression-only:
   `_const_init_value` recursively drills through any number of
   Cast wrappers (the parser produces `Cast` for explicit casts,
   and the implicit-conversion rule wraps a mismatched literal in
   another Cast) to the underlying integer or float value.

   **Switch type-checking** (C99 §6.8.4.2). The control expression
   must have integer type — Int / Long / UInt / ULong; Float /
   Double / Pointer rejected per §6.8.4.2.1. After integer
   promotion (a no-op in c6502 since every integer type is already
   at promotion rank ≥ Int), the promoted type is stamped on
   `SwitchStmt.promoted_type` and each case value is funneled
   through `passes.constant_expression.evaluate_integer_constant_
   expression` to fold it to a Python `int`, converted to the
   promoted type modulo width via `_coerce_int_to_type`. Each
   case's `value` is then replaced by a single canonicalized
   `Constant` of the promoted type so c99_to_tac sees a uniform
   shape. Uniqueness is checked on the converted integer values
   (per §6.8.4.2.3), so e.g. `case 256:` in an Int (1-byte) switch
   wraps to 0 and conflicts with `case 0:`. The case body and any
   nested case / default nodes are type-checked normally.

   `passes.constant_expression` provides two entry points sharing
   the §6.6 vocabulary: `evaluate_integer_constant_expression`
   (returns `(value, type)` for §6.6.6 sites — case labels today;
   future enums / array sizes / bitfield widths) and
   `validate_constant_expression` (the §6.6.3 check without
   folding, for arbitrary constant-expression contexts — currently
   no consumers, kept for upcoming features). Today's integer
   evaluator accepts a `Constant` integer literal optionally
   wrapped in any number of integer Casts; expanding to Unary /
   Binary / Conditional folding drops in via additional match
   arms.

   Errors raised (`TypeCheckError`):
   - Function used as a variable / variable called as a function.
   - Wrong call arity.
   - Mismatched binary-operator operand types (only when neither
     is an object type — every Int/Long/UInt/ULong/Float/Double
     mix is handled by promotion now).
   - Initializer / cast / return-value types not assignable.
   - Cast target isn't an object type (no `FunType` casts).
   - Incompatible redeclaration of an object or function (signature
     differs, or linkage disagrees with prior).
   - Multiple definitions (function with `defined=True` already, or
     two distinct file-scope `Initial(c)` values for one object).
   - Static-storage initializer isn't a constant expression.
   - Switch control expression isn't integer-typed.
   - Case label isn't an integer constant expression.
   - Two case constants in the same switch share the same value
     after conversion to the switch's promoted control type.

   The function-name table from identifier_resolution and the
   variable-scope table both feed into the symbol table here:
   variable names arrive already unique (`@<N>.<orig>`) so a flat
   `dict` is enough — no nested scopes. Functions are pre-registered
   from their definitions before each body is checked, so a body
   can self-recurse without a forward declaration.
7. `c99_to_tac.translate_program` — `(c99_ast, SymbolTable)` →
   `tac_ast`. The TAC program shape is `Program(top_level*)` where
   each `top_level` is `Function(name, is_global, params,
   instructions)` or `StaticVariable(name, is_global, data_type,
   init)`. Two passes assemble the list:
   1. Walk c99 declarations in source order. Each `FunctionDecl`
      with a body lowers to a TAC `Function`; `is_global` rides
      through from the function's symbol-table entry. File-scope
      variable declarations and forward function declarations emit
      nothing here. Block-scope variable declarations with a storage
      class (`static` / `extern`) also skip TAC emission at the
      declaration site; plain `int x [= e];` / `long x [= e];` /
      `unsigned int x [= e];` / `unsigned long x [= e];` lowers to
      a `Copy` from the evaluated initializer into the var.
   2. Iterate the symbol table once. Each `StaticAttr` entry whose
      `initial_value` is `Initial(c)` (use `c`) or `Tentative` (use
      `0`) becomes a TAC `StaticVariable`, with a typed `IntInit(v)`
      / `LongInit(v)` / `UIntInit(v)` / `ULongInit(v)` /
      `FloatInit(v)` / `DoubleInit(v)` chosen by the variable's
      declared type; `NoInitializer` entries describe a reference
      to a definition elsewhere and emit nothing.

   The c99 and TAC ASDLs declare parallel `data_type` sums (Int /
   Long / LongLong / UInt / ULong / ULongLong / Float / Double /
   FunType), so translating data_type is a one-to-one rewrap
   (`_to_tac_data_type`). The TAC `const` sum carries each
   integer's full c99 type — width AND signedness — across six
   variants: ConstInt / ConstLong / ConstLongLong on the signed
   side, ConstUInt / ConstULong / ConstULongLong on the unsigned
   side. `_to_tac_const` is a 1-to-1 map per variant; `ConstChar`
   / `ConstUChar` collapse onto `ConstInt` / `ConstUInt`
   respectively (per C99 §6.3.1.1.2 char-types-promote-to-int).
   The 6502 doesn't care about signedness at the byte level for
   `+` / `-` / `&` / `|` / `^` / `<<` / `==` / `!=`, so those op
   lowerings dispatch only on width; the places where signedness
   matters at codegen — `<` / `>` / `<=` / `>=`, right shift,
   int↔FP conversion — read the operand variant's signedness for
   Constants and the symbol-table c99 type for Vars, and dispatch
   accordingly (`asr*` vs. `lsr*` for right shift; V-corrected
   MI/PL vs. BCC/BCS for ordering; i2f vs. u2f vs. l2f vs. ul2f
   etc. for FP conversion). The integer value passes through
   `_to_tac_const` unchanged; downstream `_byte_at` masks each
   byte with `& 0xFF`, so the bit pattern is preserved regardless
   of how the integer is interpreted. FP variants stay distinct
   (Float and Double have different IEEE 754 bit patterns). The
   TAC `static_init` sum likewise keeps signedness alongside width
   on the integer side (IntInit / LongInit / LongLongInit /
   UIntInit / ULongInit / ULongLongInit) and precision on the FP
   side (FloatInit / DoubleInit) — `_tac_static_init_for(t, v)`
   dispatches on the declared type and coerces the raw value
   (`int(v)` for integer variants, `float(v)` for FP variants),
   so an integer literal initializing a `double` static lays down
   as `3.0` and a Cast-wrapped FP initializer for an integer
   static lays down its truncated integer. The helpers
   `_tac_const_for(t, v)` and `_tac_const_val(t, v)` build typed
   constants for the synthetic-constant call sites (postfix `+1`,
   short-circuit 0/1, implicit `return 0`); they dispatch by type
   — `Int` → `ConstInt(v)`, `UInt` → `ConstUInt(v)`, `Long` →
   `ConstLong(v)`, `ULong` / `Pointer` → `ConstULong(v)`,
   `LongLong` → `ConstLongLong(v)`, `ULongLong` →
   `ConstULongLong(v)`, `Float` → `ConstFloat(v)`, `Double` →
   `ConstDouble(v)`.

   **Cast lowering.** `Cast(target, exp)` lowers based on the byte
   widths of the source and target c99 types; same-width casts
   are no-ops because the 6502 has no signedness distinction:
   - same width (`Int↔UInt`, `Long↔ULong`, `LongLong↔ULongLong`,
     plus matching types) → elide (just return inner's val)
   - narrower → wider, signed source (`Int → Long`, `Int →
     ULong`, `Int → LongLong`, `Long → LongLong`, `Long →
     ULongLong`, etc.) → `SignExtend(src, dst)`
   - narrower → wider, unsigned source (`UInt → Long`, `UInt →
     ULong`, `UInt → ULongLong`, `ULong → ULongLong`, etc.) →
     `ZeroExtend(src, dst)`
   - wider → narrower (any signedness combination) →
     `Truncate(src, dst)`
   - integer → Float / Double → `IntToFloat(src, dst)` /
     `IntToDouble(src, dst)`
   - Float / Double → integer → `FloatToInt(src, dst)` /
     `DoubleToInt(src, dst)`
   - Float ↔ Double cross-precision → `FloatToDouble(src, dst)` /
     `DoubleToFloat(src, dst)`
   The SignExtend / ZeroExtend / Truncate nodes themselves carry
   no width info — `tac_to_asm` reads the symbol-table widths of
   src and dst at lowering time to fan out per byte (so the same
   three nodes cover every 1B/2B/4B widening or narrowing pair).
   The six FP-conversion nodes are TAC-only (the asm IR is 1:1
   with 6502 opcodes); `tac_to_asm` lowers each to a runtime
   helper Call. The TAC nodes themselves carry no signedness or
   width info — `tac_to_asm` reads the symbol-table types of src
   and dst to pick the right helper (i2f vs. u2f vs. l2f vs.
   ul2f vs. ll2f vs. ull2f on the integer side, f2d / d2f on the
   FP side). To keep that
   dispatch simple, `c99_to_tac` compile-time-folds any FP cast
   whose operand is a TAC `Constant` — folding sidesteps the
   integer-signedness erasure baked into TAC's `const` sum (see
   `_fold_fp_cast_constant`). Static-storage initializers also
   bypass the runtime path: `_tac_static_init_for` does the
   int→float conversion in Python at static-init build time.
   The source type comes from the inner node's `data_type` (set
   by the type checker); a `None` data_type — synthetic AST that
   bypassed type-checking — falls back to the elide path so unit
   tests of pure Cast translation stay focused.

   **Typed temporaries.** `Translator.make_temporary_variable_name(t)`
   mints a fresh `%N`, registers it in the symbol table as a
   `LocalAttr` symbol with `type=t`, and returns the name. Every
   production call site passes the surrounding expression's
   `data_type` (which the type checker has stamped as the post-
   conversion / post-promotion result type), so each `%N` carries
   the right width. Downstream consumers — `tac_to_asm` for
   operand-size dispatch and `replace_pseudoregisters` for slot
   sizing — both read `symbols['%N'].type` to decide on the byte
   plan: 1 byte for Int / UInt, 2 for Long / ULong, 4 for LongLong
   / ULongLong / Float, 8 for Double. The `t=None` default is a
   unit-test backstop and resolves to Int.

   Parameter names ride through unchanged — they were renamed to
   `@<N>.<orig>` by identifier_resolution and TAC `Var(@<N>.<orig>)`
   references in the body see the same names. Each TAC function
   gets an implicit `Ret(_tac_const_val(ret_type, 0))` appended if
   its body falls off without an explicit return (C99 §5.1.2.2.3
   mandates this for `main`; we apply it generally so every TAC
   function terminates). The constant's variant matches the
   function's declared return type — 2-byte-returning functions
   (Long / ULong) get `ConstLong(0)`, 1-byte-returning ones
   (Int / UInt) get `ConstInt(0)`, FP-returning ones get
   `ConstFloat(0.0)` / `ConstDouble(0.0)`.
   `FunctionDecl` block items lower to nothing.
   `FunctionCall(name, args)` lowers to: evaluate each arg in
   source order (left-most temp first), collect the resulting TAC
   vals, mint a fresh typed dst temp, and emit a single
   `FunctionCall(name, args, dst)` TAC instruction. The dst temp is
   what the call expression returns, so chained uses (`x = f(); y =
   f() + 1`) thread cleanly through `Copy` / `Binary` / `Ret` etc.
   Compound expressions flatten into ops, materializing each
   intermediate into a fresh `Var(%n)`. `Binary(op, src1, src2,
   dst)` evaluates `src1` first so its temps get lower numbers.

   **Pointer arithmetic lowering.** When `Binary(Add | Subtract)`
   has at least one Pointer operand (the type checker stamped the
   operand types), `translate_pointer_arithmetic` takes over:
   - `ptr ± int` — multiply the int operand by `_pointee_size(ptr)`
     using a `Binary(Multiply, int, ConstLong(size))`, skipping the
     multiply when size == 1; then emit a normal `Binary(Add /
     Subtract)` on the pointer and the scaled int. The dst temp is
     pointer-typed (so codegen sizes it as 2 bytes). For `int + ptr`
     the lowering keeps the pointer on the lhs of the underlying
     Add (consistency, not semantics — Add is commutative).
   - `ptr - ptr` — emit `Binary(Subtract)` on the two 2-byte
     pointers to get a Long byte-difference, then divide by
     `_pointee_size(ptr)` via `Binary(Divide, diff,
     ConstLong(size))` to recover the element count, skipping the
     divide when size == 1. Result is Long.
   `_pointee_size` returns the recursive `_sizeof` of the pointee:
   1 for Int/UInt, 2 for Long/ULong/Pointer, 4 for Float, 8 for
   Double, `_sizeof(elem) × count` for Array (so multi-dim pointer
   arithmetic scales correctly — `int (*)[10]; q + 1` advances by
   10 bytes). Same widths as the symbol-table sizing in `tac_to_asm`
   and `replace_pseudoregisters`. The Multiply/Divide steps go
   through the existing `mul16` / `divmod16` runtime helpers (so a
   non-trivial pointer arithmetic program assembles but won't link
   until those helpers land — same status as `*` / `/` on Long).

   **Subscript lowering.** `Subscript(array, index)` reuses
   `translate_pointer_arithmetic` directly: compute
   `array_val + index*sizeof(elem)` (the Pointer-typed byte
   address), then `Load(src_ptr=addr, dst=fresh_elem_temp)` for
   rvalue context. On the lvalue path (Assignment with Subscript
   lval) the same address computation feeds a `Store(src=rval,
   dst_ptr=addr)`. Array decay was reified by the type checker as
   an `AddressOf` wrapper, which lowers to a `GetAddress` here, so
   `arr[i]` and `ptr[i]` go through the same TAC shape — the only
   difference is that `arr[i]` evaluates to `GetAddress(arr) +
   ...` while `ptr[i]` evaluates to `Load(ptr_var) + ...`.

   `Goto(label)` lowers to a TAC `Jump(label)`; `LabeledStmt(label,
   stmt)` lowers to a TAC `Label(label)` followed by the inner
   statement's lowering. Label names arrive pre-mangled by
   label_resolution and pass through unchanged. Iteration statements
   derive concrete control-flow targets from the base label set by
   loop_labeling, by suffix: `<base>_start` (top of loop),
   `<base>_continue` (continue target), `<base>_break` (break
   target). `BreakStmt(label)` → `Jump(<label>_break)`,
   `ContinueStmt(label)` → `Jump(<label>_continue)`. The three loop
   kinds lower to fixed sequences: `while` is `Label(_continue);
   <eval cond>; JumpIfFalse(_break); <body>; Jump(_continue);
   Label(_break)`; `do-while` is `Label(_start); <body>;
   Label(_continue); <eval cond>; JumpIfTrue(_start); Label(_break)`;
   `for` is `<init>; Label(_start); [<eval cond>;
   JumpIfFalse(_break);] <body>; Label(_continue); [<post>;]
   Jump(_start); Label(_break)`, with the bracketed sections omitted
   when the condition or post-clause slot is empty (a missing
   condition is treated as unconditionally true).

   **Switch lowering.** `SwitchStmt(control, body, label, cases,
   default_label)` lowers to: evaluate the control once into a
   typed temp `t`; for each `(case_value, case_label)` in `cases`
   emit `Binary(Equal, t, case_const, eq_temp)` followed by
   `JumpIfTrue(eq_temp, case_label)`; emit an unconditional
   `Jump(default_label or <label>_break)` past the dispatch chain;
   then translate `body` (which contains `CaseStmt` / `DefaultStmt`
   nodes that lower to `Label(...)` followed by their inner
   statement); finally emit `Label(<label>_break)`. Cases fall
   through unless `break;` (lowered via the regular BreakStmt path
   to `Jump(<switch>_break)`) is hit. Each case's `case.value` is
   already a canonicalized integer `Constant` of the switch's
   promoted control type (see pass 5), so the dispatch comparisons
   happen at one width. `CaseStmt` / `DefaultStmt` outside the
   dispatch context just emit their `Label` and recurse — the case-
   value itself was already consumed at the dispatch chain.
8. `tac_to_asm.translate_program` — `tac_ast` → `asm_ast`. The asm
   program shape mirrors TAC: `Program(top_level*)` with
   `Function(name, is_global, params, instructions)` and
   `StaticVariable(name, is_global, init)`. Each TAC `Function`
   lowers atom by atom; each TAC `StaticVariable` rides through to
   an asm `StaticVariable`. The asm-side init has five variants —
   the integer side carries only the three width variants (`IntInit
   | LongInit | LongLongInit`), so TAC's `UIntInit(v)` collapses to
   asm `IntInit(v)`, `ULongInit(v)` to `LongInit(v)`, and
   `ULongLongInit(v)` to `LongLongInit(v)`; the FP side keeps Float
   and Double distinct (`FloatInit | DoubleInit`) because their
   IEEE 754 byte patterns differ. The asm side has no `data_type`
   field — the variant of the init alone determines the cell size
   at emit (DC.B for IntInit, DC.W for LongInit, DC.L for
   LongLongInit, DC.L for FloatInit, two DC.Ls for DoubleInit
   since dasm has no native 8-byte directive).

   **The asm IR is strictly 1:1 with 6502 opcodes** — no width
   tagging anywhere. The 6502 is an 8-bit machine, so every asm
   instruction is implicitly Byte-typed. That makes `tac_to_asm`
   the single home of all multi-byte lowering: for each TAC
   instruction whose operands are wider than 1 byte (Long / ULong
   = 2 bytes, LongLong / ULongLong = 4, Float = 4, Double = 8 —
   per the symbol table), the translator emits a sequence of
   byte-level asm atoms — typically one pass per byte with the
   6502's carry flag threading
   naturally between them for arithmetic on 2-byte operands. (FP
   arithmetic isn't lowered inline; it dispatches to runtime
   helpers via the HARGS block — see below.)

   **Per-byte addressing.** `Pseudo` and `Data` carry an `int
   offset` field that selects which byte of a multi-byte value the
   reference is — `offset=0` is the low byte (or the only byte of
   an Int), `offset=k` the (k+1)-th byte (so `offset=7` is the
   high byte of a Double). The helper `_byte_at(operand, k)`
   produces the k-th byte of any operand: `Imm(v)` →
   `Imm((v >> 8*k) & 0xFF)` (using Python's arithmetic `>>` so a
   negative ConstLong folds to its two's-complement bytes; FP
   constants pre-fold to a non-negative IEEE 754 bit pattern at
   `translate_val` time, so the same shift-and-mask byte
   extraction works without special-casing FP); memory-shaped
   operands (Pseudo / Stack / Frame / Data) bump their `offset`
   by k.

   **Operand-size dispatch.** `Translator._size_of(val)` returns 1
   for 1-byte types (Int / UInt), 2 for 2-byte (Long / ULong), 4
   for 4-byte (LongLong / ULongLong / Float), 8 for Double — by
   reading the symbol table for Vars and the const variant for
   Constants (each TAC integer const variant carries width AND
   signedness; this helper only reads width). Each per-instruction
   lowering keys off this size; the size-parameterized loops
   naturally generalize across 1, 2, and 4 byte widths with carry
   threading where appropriate. Signedness only matters for
   ordering comparisons, right shift, and int↔FP conversion;
   everywhere else the byte sequences are identical. The signedness
   dispatch reads the operand: const variant for Constants,
   symbol-table c99 type for Vars (via `_is_unsigned_val` for
   ordering / right shift, `_int_type_of` for FP-conversion helper
   selection). Examples:
   - `Copy(src, dst)`: 1 Mov for Int, 2 Movs (lo, hi) for Long.
   - `Binary(Add, …)` Long: `Mov src1.lo→A; CLC; Add(src2.lo, A);
     Mov A→dst.lo; Mov src1.hi→A; Add(src2.hi, A); Mov A→dst.hi`.
     No CLC between the bytes — `LDA` only affects N/Z, so the
     carry from the low ADC is intact for the high ADC.
   - `Binary(Subtract, …)` Long: same shape with SetCarry/Sub,
     borrow threads via the carry register.
   - `Binary(Equal, …)` Long: high-byte CMP first; if differ, BNE
     short-circuits to a label (Z=0 there); else fall through to
     low-byte CMP whose Z is the final answer; then 0/1 select.
   - `Binary(LessThan, …)` Long: low-byte SBC then high-byte SBC
     (carry threads), V-correction on the high result, branch on
     MI/PL. Same operand-swap trick as the 8-bit form for `>` /
     `<=`.
   - `JumpIfFalse(Long_cond, target)`: `Mov(cond.lo, A);
     Or(cond.hi, A); Branch(EQ, target)` — the OR sets Z=1 iff
     both bytes are zero, i.e. the 16-bit value is zero.
   - `Mul/Div/Mod/Shift` (any operand width): runtime-helper Calls.
     Operands are exchanged through `HARGS`, a 24-byte zero-page
     block (`$04`–`$1B`) that the runtime header pins by name. The
     block is sized for the largest helper (`dadd`/`dsub`/`dmul`/
     `ddiv`, which need 16 bytes in + 8 bytes out); integer helpers
     use only the low 8 bytes.
     Caller writes inputs into `HARGS+0..N-1`, JSRs the helper
     (mul8 / udivmod8 / sdivmod8 / asl8 / asr8 / lsr8 for 1-byte
     operands; the 16-bit and 32-bit families have the same names
     with the suffix changed to 16 or 32), and reads the result
     from a fixed offset later in the block. Inputs survive the
     call. The signed/unsigned divmod split mirrors the asr/lsr
     right-shift split: signed `/` and `%` route to `sdivmod*`
     (trunc-toward-zero per C99 §6.5.5.6), unsigned to `udivmod*`
     (floor-divide). Per-helper layout (inputs → outputs):
       mul8       A:`+0`, B:`+1`              → product:`+2` (1 byte;
                                                 low byte of A*B,
                                                 high byte discarded
                                                 because int*int
                                                 wraps to int)
       udivmod8/  num:`+0`, den:`+1`          → quot:`+2`, rem:`+3`
        sdivmod8
       asl8/      val:`+0`, count:`+1`        → result:`+2`
        asr8/
        lsr8
       mul16      A:`+0..+1`, B:`+2..+3`      → product:`+4..+5` (2 bytes;
                                                 low half of A*B,
                                                 high half discarded)
       udivmod16/ num:`+0..+1`, den:`+2..+3`  → quot:`+4..+5`,
        sdivmod16                              rem:`+6..+7`
       asl16/     val:`+0..+1`, count:`+2`    → result:`+3..+4`
        asr16/     (1-byte count: shifts ≥16 are UB, so the high byte
        lsr16      of a promoted-to-Long count is dropped)
       mul32      A:`+0..+3`, B:`+4..+7`      → product:`+8..+11` (4 bytes;
                                                 low half of A*B,
                                                 high half discarded)
       udivmod32/ num:`+0..+3`, den:`+4..+7`  → quot:`+8..+11`,
        sdivmod32                              rem:`+12..+15`
       asl32/     val:`+0..+3`, count:`+4`    → result:`+5..+8`
        asr32/     (1-byte count: shifts ≥32 are UB)
        lsr32
     `RightShift` dispatches by operand signedness: signed operands
     route to `asr*` (arithmetic, sign-preserving), unsigned to
     `lsr*` (logical, zero-fill). Signedness for Constants comes
     from the const variant (Const{Int,Long,LongLong} → signed,
     Const{UInt,ULong,ULongLong} → unsigned); for Vars, from the
     symbol-table c99 type. The 16- and 32-bit helpers themselves
     aren't in the repo yet; the lowerings emit calls to them in
     advance of the runtime header landing. (8-bit signed `>>` of
     `signed char` is rare in practice — `signed char` integer-
     promotes to `int` before `>>`, so the 8-bit `asr8` helper is
     mostly a placeholder.)

   **Cast lowering.** SignExtend / ZeroExtend / Truncate read the
   source and destination operand widths from the symbol table at
   lowering time, so the same three TAC nodes cover every 1B/2B/4B
   widening or narrowing pair. The 6502 has no signedness
   distinction at the byte level, so same-width casts are no-ops.
   - `Truncate(src, dst)`: copy `_size_of(dst)` low bytes from src
     into dst — memory is little-endian, so byte 0 is the low byte,
     and the source's higher bytes are just discarded. Covers
     Long → Int, LongLong → Int, LongLong → Long, etc., for any
     signedness combination.
   - `SignExtend(src, dst)` (signed source widened): inline byte
     sequence — copy each source byte to the matching dst byte
     (the last LDA's N flag is the source's sign byte's), `Branch(MI,
     sx_neg@N); LDA #$00; Jump(sx_done@N); Label(sx_neg@N); LDA
     #$FF; Label(sx_done@N);` then STA into each remaining (high)
     dst byte. Covers Int → Long, Int → LongLong, Long → LongLong,
     Int → ULong, etc. Two minted labels per use; the Translator's
     program-global counter keeps them unique.
   - `ZeroExtend(src, dst)` (unsigned source widened): inline byte
     sequence — copy each source byte unchanged, then write a
     literal 0 into each remaining (high) dst byte. No branch
     needed. Covers UInt → ULong, UInt → ULongLong, ULong →
     ULongLong, etc.

   Output is correct but redundant — every intermediate is
   materialized through a `Frame` slot. Optimization is deferred to
   TAC-level passes.

   **TAC `FunctionCall(name, args, dst)`** lowers to the caller-
   side soft-stack convention: `AllocateStack(total_arg_bytes)`
   (each Long arg contributes 2 bytes, each LongLong / Float arg
   4, each Double 8, each Int 1), one Mov per arg byte writing
   into `Stack(1)..Stack(total_arg_bytes)` in source order (low
   byte at the lower offset for multi-byte args), `Call(name)`,
   then capture the return value. The convention is width-driven:
   Int (1B) ← A; Long (2B) ← A=low, X=high (with X routed through
   A for the high-byte store); LongLong (4B) / Float (4B) ← bytes
   read from `HARGS+8..11` byte-by-byte through A; Double (8B) ←
   bytes read from `HARGS+16..23`. LongLong shares the Float slot
   because types are exclusive per call and `mul32` / `divmod32`
   already write their 4-byte results to that offset, so a
   function ending `return a OP b;` for LongLong operands needs no
   epilogue copy. The FP slots are deliberately the same as the
   FP arithmetic helpers' output slots. Caller has to capture
   any HARGS-returned value *immediately* after the JSR, before
   any other helper Call, since HARGS is caller-saved. The
   callee's epilogue rewinds SSP all the way back to the caller's
   pre-call value, so there's no per-call cleanup. Runtime-helper
   calls (mul8/16/32, divmod8/16/32, asl8/16/32, asr8/16/32)
   emitted by the binary-op lowerings still go straight to
   `asm_ast.Call` (no `AllocateStack`); they exchange operands
   through the `HARGS` zero-page block instead of the soft stack,
   so they bypass the user-function calling convention entirely.
9. `passes.replace_pseudoregisters.replace_program` — replaces every
   `Pseudo(name, offset)` operand with a concrete addressing-mode
   operand and lays out the function's stack frame. Takes the
   type-checker's SymbolTable so it can size each pseudo by its
   declared type: 1 byte for `Char` / `SChar` / `UChar` /
   unknown, 2 for `Int` / `UInt` / `Pointer`, 4 for `Long` /
   `ULong` / `Float`, 8 for `LongLong` / `ULongLong` / `Double`.
   Optionally takes a `colorings: dict[func_name, Coloring]` from
   the optimizer when `--optimize` is on; without it, every
   pseudo goes to Frame as before.

   Walks each function twice:
   - **Pre-step:** if a `Coloring` is supplied, derive the set
     of zero-page byte addresses the function uses from the
     callee-saved pool (every byte of every colored value that
     falls in `coloring.pool.callee_saved()`). These bytes get
     reserved at the bottom of the frame (`FP+1..FP+S`); locals
     shift up by S to leave room. The prologue saves them; the
     epilogue restores them.
   - **Pass 1 (discovery):** mint a *base* offset (the offset of
     byte 0) for every Pseudo name that *isn't* in the function's
     `params`, isn't in the program's static-storage set, and
     isn't colored. Locals get sequential base offsets in source-
     encounter order, each advancing the cursor by `size_of_name
     (name)`. After the walk, M = total local bytes (including
     the S-byte callee-save area).
   - **Finalize:** compute param base offsets analogously. The
     first param's first byte is at `Frame(M + 3)`; each subsequent
     param starts after the previous one's bytes. The 2-byte gap
     at M+1, M+2 holds the saved caller FP.
   - **Pass 2 (replacement):** rewrite each Pseudo operand. The
     decision order is: static → `Data(name, offset)` (absolute,
     link-time address); param → `Frame(...)` (calling convention
     wins even if regalloc colored it); colored local →
     `ZP(addr, offset)` (the ZP byte from `coloring.assignments`
     plus the Pseudo's offset); ordinary local (uncolored,
     spilled, or address-taken) → `Frame(base + offset)`.
   The pass also prepends `FunctionPrologue(arg_bytes=N,
   local_bytes=M, callee_saved_addrs=[...])` and patches every
   `Ret(...)` with the same N/M and the same addrs list, so the
   emitter has the dimensions it needs for the prologue's space-
   allocation step, the save/restore sequences, and the
   epilogue's SSP-rewind.
10. `passes.asm_to_asm2.translate_program` — `asm_ast` → `asm2_ast`.
    Strictly-atomic-IR lowering: rewrites the three asm_ast
    compound nodes (`AllocateStack(N)` for caller-side soft-stack
    allocation, `FunctionPrologue` for the callee's frame setup,
    `Ret` for the matching teardown) into sequences of single-
    instruction asm2 atoms, and re-tags every other instruction /
    operand / static_init / reg / condition payload at the asm2
    type. The result has every node = one logical 6502
    instruction (where indirect-Y addressing setup counts as
    addressing-mode setup, per the asm-emit convention). Three
    asm2-only atoms join the existing instruction set: `Return`
    (RTS — what `Ret` collapsed to in the no-frame case),
    `Comment(text)` (block-level "; …" line at opcode column —
    what the prologue / epilogue used to emit inline), and
    `Blank` (a blank-line separator between prologue / body /
    epilogue; emit collapses runs of these). `LoadAddress` stays
    a single atom (its expansion is short enough to keep as one
    logical "compute the address into two bytes" step).

    The compound-node lowerings are deliberately naive: they drop
    the INY / TAX / STX byte-saving tricks that `asm_emit` used
    to use, in exchange for a uniform "each Mov is self-
    contained" model where the same `Mov(Reg(A), Stack(off))`
    atom always emits its own LDY setup. That costs +1 byte per
    `FunctionPrologue` save-FP step and +2 bytes per non-trivial
    `Ret` restore-FP step versus the old emit. `sim.assembler.
    _prologue_size` / `_ret_size` / `_emit_prologue` / `_emit_ret`
    mirror the same naive lowering so `instruction_size` (used by
    `passes.long_branches`) and `assemble` (the in-process binary
    assembler) stay byte-aligned with what `asm_emit` produces.

11. `asm_emit.emit_program` — `asm2_ast` → 6502 assembly text.
   **Atomic IR**: every node maps to one 6502 instruction. The
   compound nodes from asm_ast are gone here — they were
   expanded by step 10. The new `Return` atom emits `RTS`;
   `Comment(text)` emits `   ; <text>`; `Blank` emits `""` and
   `emit_function` collapses consecutive blanks.

   Multi-function programs emit each function's body in source
   order separated by a single blank line.

   `Data(name, offset)` operands render as `LDA name` for offset
   0 (the common case) and `LDA name+offset` otherwise — the
   assembler resolves the symbol+offset to a fixed address.
   `ZP(address, offset)` operands fold both at emit time into
   `LDA $XX` (where XX = address + offset), giving direct zero-
   page addressing for regalloc-assigned locals. `ZP` is legal
   everywhere `Data` is (Mov, Add/Sub, Compare, Inc/Dec, ASL/LSR,
   direct LDX/LDY shortcut). The self-Mov peephole inside
   `_emit_mov` returns `[]` when `src == dst` — drops the redundant
   `LDA $XX; STA $XX` pairs that arise when regalloc gives a Phi
   src and dst the same color.
   Top-level `StaticVariable(name, _, init)` emits as `<name>:`
   followed by `DC.B $XX` for `IntInit(int=v)`, `DC.W $XXXX` for
   `LongInit(int=v)`, `DC.L $WWWWWWWW` for `LongLongInit(int=v)`
   (4 bytes signed/unsigned integer; mask to 32 bits so negatives
   render as two's-complement), `DC.L $WWWWWWWW` for
   `FloatInit(float=v)` (4 bytes IEEE 754 single, packed via
   `struct.pack` at emit time), and two `DC.L`s — low half, high
   half — for `DoubleInit(float=v)` (8 bytes IEEE 754 double;
   dasm has no native 8-byte directive). The W form masks to 16
   bits so signed-negative values render as two's-complement;
   dasm's `DC.W` / `DC.L` both lay the bytes down little-endian,
   matching the soft-stack memory model — so `Data(name,
   offset=1)` accesses the high byte of a Long static,
   `Data(name, offset=3)` the high byte of a LongLong static, and
   `Data(name, offset=7)` the high byte of a Double static.

`Pseudo` operands aren't part of `asm2_ast` — they must have been resolved by
step 9 (`replace_pseudoregisters`); the asm_to_asm2 pass raises if one
slips through. `Mul`/`Div`/`Mod`/`LeftShift`/`RightShift`
are TAC-only concepts;
`tac_to_asm` lowers each to a sequence of `Mov`s into the shared
zero-page block `HARGS` (`$04`–`$1B`), a `Call` to the appropriate
runtime helper (`mul8`/`divmod8`/`asl8`/`asr8`/`lsr8` for 1-byte
operands, `mul16`/`divmod16`/`asl16`/`asr16`/`lsr16` for 2-byte
operands, `mul32`/`divmod32`/`asl32`/`asr32`/`lsr32` for 4-byte
operands), and `Mov`s reading the result back out at a helper-specific
offset within HARGS (see step 8's per-helper layout table). Right
shift dispatches to `asr*` (arithmetic, sign-preserving) for signed
operands and `lsr*` (logical, zero-fill) for unsigned, keyed off the
LEFT operand's signedness per C99 §6.5.7.5. Per §6.5.7.3 the result
type — and so the helper width — is the promoted left operand's
type; the right operand promotes independently and contributes only
its low byte to the count. The unary `LogicalNot` is lowered inline (no runtime helper):
`Mov src→A; Branch(EQ, true); Mov 0→A; Jump end; true: Mov 1→A; end:
Mov A→dst`. The framing `Mov(src, A)` already sets Z via `LDA`, so no
`Compare` is needed before the branch.

The six comparison ops
(`Equal`/`NotEqual`/`LessThan`/`GreaterThan`/`LessOrEqual`/`GreaterOrEqual`)
are also TAC-only but are lowered inline with `Compare`/`Sub` + `Branch`
atoms (no runtime helper). `Equal`/`NotEqual` emit `Mov src1→A; Compare(A,
src2); Branch(EQ|NE, true); LDA #0; Jump end; true: LDA #1; end: Mov A→dst`.
`LessThan`/`GreaterOrEqual` use `Mov src1→A; SEC; Sub(src2, A); BVC novf;
EOR #$80; novf:; Branch(MI|PL, true); … 0/1 select …`. CMP can't be used
for signed ordering because it leaves V alone, and the N flag lies when the
signed subtraction overflows — the `BVC novf; EOR #$80` pair corrects N.
`GreaterThan`/`LessOrEqual` reuse the same sequence with operands swapped
(`>` → `src2 < src1`, `<=` → `src2 >= src1`) because `Z` is unreliable after
the EOR correction, so asking for "not-less-than AND not-equal" directly
would need a second compare; swapping is cheaper. The asm IR itself has no
multiply/divide/shift/lnot primitives — every non-prologue/ret node is 1:1
with a 6502 opcode.

`tac_to_asm` is class-based (`Translator`) because the inline comparison
lowerings mint fresh labels per use and need a counter that persists across
the whole program. Module-level wrappers (`translate_program`, etc.) each
construct a fresh `Translator`.

## Optimization pipeline (`--optimize`)

TAC-level fixed-point opts → asm-level SSA round-trip with
byte-granular regalloc → late prologue / epilogue synthesis.
Per-function TAC shape:

```
fn → rotate_signed_countdown_loops (one-shot, pre-SSA)
   → to_ssa
   → fold_static_const_reads (one-shot)
   → [constant_fold → reduce_strength → fold_cmp_zero_jump
      → fold_narrow_and_jump → UCE → copy_propagate → DSE
      → fold_copies → reassoc_constants → recognize_indexed_store
      → recognize_indexed_load → sink_increments]*
   → recognize_indirect_indexed (post-fixedpoint, one-shot)
   → from_ssa
   → fold_copies                                  (post-from_ssa)
   → fn'
```

`docs/optimization.md` is the from-scratch tour. Brief:

1. **`rotate_signed_countdown_loops`** (`passes/optimization/
   loop_rotate.py`). Pre-SSA one-shot. Recognizes the canonical
   `c99_to_tac` for-loop shape where the test is at the top
   (`for (i = N; i >= 0; i--)`) and rotates to test-at-bottom
   (`do { ... } while (i >= 0);`), saving one unconditional jump
   per iteration. Operates pre-SSA because the rewrite is
   structural — Phis don't exist yet, so the rewrite needs only
   to fix up the def/use chain on the loop's single counter var,
   not parallel-copy a Phi web. `to_ssa` rebuilds Phis after.

2. **`to_ssa`** (`passes/optimization/ssa_construction.py`).
   Renames promotable Vars (LocalAttr + scalar + non-address-
   taken) to `<orig>.<N>`; inserts pruned Phi nodes at iterated
   dominance frontiers (Cytron 1991). Address-taken locals,
   statics, aggregates pass through unchanged. SSA-minted
   labels are scoped per-function (`.<funcname>@ssa_block@N`).

3. **`fold_static_const_reads`** (`passes/optimization/
   static_const_fold.py`). One-shot post-SSA. Replaces every
   USE-position `Var(name)` with `Constant(value)` when `name`
   is `static const` scalar with an `Initial(c)` initializer
   and a const-qualified type. See "Static-const reads + array-
   subscript folding" below for details.

4. **Fixed-point loop**. Twelve passes rotated to convergence:
   - `constant_fold`: folds Unary / Binary / comparison / cast /
     conditional-jump-with-constant-cond / Phis with agreeing
     args. Width-correct wraparound at the operand's declared
     width. Const-array-subscript fold (`_fold_indexed_load`) is
     part of this pass.
   - `reduce_strength`: `Multiply(x, 2^k)` → `LeftShift`,
     unsigned `Divide(x, 2^k)` → `RightShift`, unsigned
     `Modulo(x, 2^k)` → `BitwiseAnd`. Signed Divide / Modulo
     skipped (C99 truncation differs from arithmetic shift).
   - `fold_cmp_zero_jump`: fuses `Binary(cmp, ...); JumpIf*` to
     direct conditional jumps. `== 0` / `!= 0` traces through
     ZeroExtend; ordering ops emit `JumpIfCmp(op, src1, src2)`
     for the per-byte compare-chain lowering. Operand narrowing
     through ZeroExtend folds `(uint8_t)i < 105` to 3-instr
     `LDA / CMP / BCS`.
   - `fold_narrow_and_jump` (`passes/optimization/and_zero_jump_
     fold.py`): folds `(ZeroExtend(uchar); BitwiseAnd(_, 0x80);
     JumpIf*)` to `JumpIfMasked` when the operand can be
     narrowed to 1 byte — produces the direct `LDA / BPL/BMI`
     pattern at asm lowering instead of an 8-bit AND + 16-bit Z.
   - `eliminate_unreachable_code`: forward DFS from ENTRY; prunes
     dead Phi args; folds singleton Phis to Copies; drops
     useless jumps / labels.
   - `copy_propagate` (SSA-aware): replaces every use of a
     Copy's dst with its src; chains too.
   - `eliminate_dead_stores` (SSA-aware): drops pure defs with
     no reads. Calls keep the call (side effects) but drop
     unused dst.
   - `fold_copies` (`passes/optimization/copy_folding.py`):
     fuses `<producer dst=%t>; Copy(%t, X)` to `<producer
     dst=X>` for single-use `%t`. See its dedicated section
     below.
   - `reassoc_constants` (`passes/optimization/reassoc_const.py`):
     `Add(C2, V, %t); Add(C1, %t, dst)` → `Add(C1+C2, V, dst)`.
   - `recognize_indexed_store` / `recognize_indexed_load`
     (`passes/optimization/recognize_indexed_*.py`): collapses
     `ZeroExtend(uchar) + Binary(Add, C) + Store/Load` chains
     to `IndexedStore` / `IndexedLoad` (absolute,X addressing).
     See "IndexedStore recognizer" below.
   - `sink_increments` (`passes/optimization/sink_increment.py`):
     moves `Y = X + c` past the last in-line use of `X` when
     `Y`'s only consumer follows, exposing `recognize_indexed_*`
     patterns the original ordering hid.

5. **`recognize_indirect_indexed`** (`passes/optimization/
   recognize_indirect_indexed.py`). Post-fixedpoint one-shot.
   Collapses `ZeroExtend(uchar) + Binary(Add, ptr) + Load` to
   `IndirectIndexed` for the `(zp),Y` lowering. Deliberately
   runs LAST so any pointer that's going to fold to a Constant
   (via the const-static fold path) has already done so —
   otherwise this pass would prematurely lock in (zp),Y for a
   chain that would have qualified for the cheaper absolute,X
   form.

5a. **`dispatch_const_pointer_arrays`** (`passes/optimization/
    dispatch_pointer_array.py`). Runs at the program level
    after `optimize_tac` (post-from_ssa). Recognizes the
    `Binary(LeftShift|Multiply, i, 1|2) + IndexedLoad(arr, _,
    %ptr) + IndirectIndexedLoad(%ptr, j, %v)` chain when `arr`
    is a file-scope `static const T * const[N]` with N ≤ 8 and
    all-AddressInit elements; rewrites to a CMP/BEQ dispatch on
    `i` with per-case direct `IndexedLoad(target_k, j, %v)`.
    Eliminates DPTR staging and (zp),Y indirection at the cost
    of a small dispatch chain; frees X and Y from the
    dual-index conflict so loop counters can stay pinned to X
    across the dispatch.

6. **`from_ssa`** (`ssa_destruction.py`). One Copy per PhiArg
   in the matching predecessor, before the terminator. Parallel-
   copy ordering by topological sort fixes the "lost copy"
   problem; cycles break with a fresh `<funcname>.cycle_tmp@N`.

7. After `from_ssa` the function is non-SSA TAC, ready for
   `tac_to_asm` in bare-exit mode.

8. **Asm-level SSA round-trip** (`passes/optimization_asm/`).
   Prepass `fold_const_statics` (`const_static_fold.py`)
   replaces const-static reads with `Imm` operands (see "Const-
   static fold" below). Then `dead_static.py` drops any
   internal-linkage `StaticVariable` top-level nothing now
   references. Per function: `to_ssa` byte-versions every
   promotable `(name, offset)` pair; `hwreg_eligibility` marks
   Pseudos that can live in X/Y across their intra-block live
   range (saves the `LDX / LDY` setup for absolute,X / (zp),Y
   accesses where the index is the pinned Pseudo). Eligibility
   is per-HwReg (separate `eligible_x` and `eligible_y` sets) —
   `Mov(IndexedData(...,X), P)` makes P Y-eligible only (via
   `LDY abs,X`, since `LDX abs,X` doesn't exist), and vice
   versa for IndexedData(...,Y). `coalesce_
   moves` merges Mov/Phi-related non-interfering names; fixed-
   point `[copy_propagate → backward_copy_propagate →
   byte_dce]`. **Byte-granular regalloc** colors 1-byte SSA
   names to ZP from `Pool(start=0x80)` (default split: caller-
   saved `[0x80, 0xC0)`, callee-saved `[0xC0, 0x100)`); multi-
   byte names get contiguous width-N blocks. Cross-call values
   prefer callee-saved; non-cross-call values caller-saved.
   HwReg-eligible Pseudos are assigned `Reg(X)` / `Reg(Y)`
   instead of a ZP byte. `from_ssa` emits per-edge Movs with
   parallel-copy ordering.

   When the per-function private pool (see "Call-graph-disjoint
   ZP allocation") is in effect for an eligible function, the
   regalloc draws colors exclusively from that pool — the
   caller/callee partition collapses.

9. **`replace_pseudoregisters_bare_exit`** resolves Pseudos:
   colored → `ZP(addr, 0)`; spilled / address-taken / params →
   `Frame(off)` or `Data(slot_symbol, 0)` for zp_abi params.

10. **Late prologue synthesis** (`passes/prologue_synthesis.py`):
    when `arg_bytes == local_bytes == callee_saved_bytes == 0`,
    the bare `Return(save_a)` atoms stay and no
    `FunctionPrologue` is prepended. Otherwise prepend
    `FunctionPrologue(N, M, callee_saved_addrs)` and patch each
    `Return` to `Ret(N, M, save_a, callee_saved_addrs)`.

10a. **`apply_licm`** (`passes/asm_licm.py`). Asm-level LICM-
    lite for loop-invariant constant stores. Identifies natural
    loops by back-edge, hoists `Mov(Imm, Data|ZP)` and `LDA #c;
    STA M` pairs to the preheader when the dst isn't otherwise
    written in the body, no `Call` appears in the body
    (conservative — sidesteps zp_abi clobber questions), and
    the loop has a single entry through the header.

11. **`loop_counter_to_x`** (`passes/loop_counter_to_x.py`).
    Asm-level. Promotes a loop counter pseudo to `Reg(X)` when
    the live range fits the X pivot pattern: the counter is
    initialized once outside the loop, used as an `LDX` source
    inside, decremented at loop bottom, and not live across any
    JSR (saved/restored around them with `STX`/`LDX`). Also
    accepts `LDA M` body uses (rewritten to `TXA` since X = M is
    the promotion invariant). Y-pivot ranges within the loop
    reject ranges containing Indirect / IndirectY / IndirectZp /
    IndirectZpY operands — these read Y for addressing and the
    pivot's LDX→LDY rewrite would clobber that Y. The classic
    refresh_hit_entities winner — ~5× speedup on the hot loop.
    Composes with the Y-pivot path inside the promotion shape.

12. **Peephole fixed-point loop** — see "Peephole catalog"
    below. 19 passes; runs twice in the optimized pipeline
    (once before `loop_counter_to_x`, once after, so promotions
    the X-pivot exposes get a second round of cleanup).

13. **`expand_long_branches`** rewrites conditional branches
    whose target is out of the ±127-byte range into
    `Branch(inverted_cond, .skip); Jump(target); .skip:`.
    Iterative because rewrites can push other branches over
    the limit.

14. **`asm_to_asm2`** expands the three compound nodes
    (`FunctionPrologue` / `AllocateStack` / `Ret`) into
    sequences of asm2 atoms — see the `asm_to_asm2`
    description in the compiler-pipeline section.

`StaticVariable` top-levels pass through unchanged except for
the const-static fold (next section) and the `dead_static`
drop. `optimize_function` without `symbols` (legacy unit-test
path) skips SSA construction.

After `--optimize --codegen`, `passes.zp_link_metadata` emits a
`; @zp-link-meta-begin` / `; @zp-link-meta-end` block at the
top of the output: each `def <fn> param_bytes=N local_bytes=M
indirect=B in_cycle=B` and `ext <fn> param_bytes=N` and `call
<caller> -> <callee>` line carries what `compile.py --link`
needs to re-allocate `__zpabi_*` / `__local_*` symbols globally
across multiple TUs.

## Static-const reads + array-subscript folding

Three composable TAC-level passes that turn const-static reads
and const-array subscripts with constant indices into compile-
time Constants, exposing them to the rest of the constant
folder.

### `passes/optimization/static_const_fold.py` — scalar reads

One-shot pass that runs once after `to_ssa`, before the fixed-
point loop. Walks every TAC instruction; replaces every USE-
position `Var(name)` with `Constant(value)` when `name`'s symbol-
table entry is:

  * `StaticAttr(initial_value=Initial(c))` with `c` being `int`
    or `float` (NOT `AddressInit` — link-time symbol; NOT a
    tuple — aggregate);
  * type carries an outermost `Const(...)` wrapper (gates the
    fold on the C type system having already promised the
    storage's value is fixed at runtime);
  * underlying type is a foldable scalar (Char/SChar/UChar /
    Int/UInt / Long/ULong / LongLong/ULongLong / Float / Double
    / Pointer — not Array, Structure, Union).

The asm-level `fold_const_statics` already drops the
`StaticVariable` storage when nothing references it; this
TAC-level pass eliminates the runtime reads upstream so the
constant flows into the rest of the constant folder.

### Const-array-subscript fold (`_fold_indexed_load` in
`constant_folding.py`)

`IndexedLoad(name, Constant(byte_idx), dst)` collapses to
`Copy(Constant(value), dst)` when:

  * `name` is `StaticAttr(Initial(tuple_value))`,
  * the array's element type is const-qualified
    (`Array(Const(elem_t), N)`),
  * `byte_idx` is element-aligned,
  * the indexed element value is `int` or `float` (not a nested
    tuple, not `AddressInit`),
  * the dst's c99 width matches the element's width.

### Add-with-Constant reassociation (`passes/optimization/reassoc_const.py`)

Recognizes `Binary(Add, C2, V, %inner); Binary(Add, C1, %inner,
%outer)` (or any commutative variant) where `%inner` is single-
use and the two Constants share a const variant, and rewrites
to `Binary(Add, (C1+C2), V, %outer)` (dropping the inner def).
Wraps modulo the variant's bit width.

The headline composition: in code like

```c
static uint8_t * const buf = (uint8_t * const)0x2000;
static const uint16_t offsets[N] = {0x100, 0x200, ...};
buf[offsets[2] + col] = value;
```

the static-const reads turn `buf` into `Constant(0x2000)`, the
const-array fold turns `offsets[2]` into `Constant(0x300)`, and
reassociation collapses `0x2000 + (0x300 + col)` to `0x2300 +
col` — one runtime 16-bit Add instead of two. Then the
IndexedStore recognizer (next section) folds the whole thing
into a single absolute,X store.

## IndexedStore recognizer (`passes/optimization/recognize_indexed_store.py`)

A TAC pass that runs in the fixed-point loop. Detects the
canonical absolute,X-store pattern and rewrites it to the new
`IndexedStore(int address, val index, val src)` instruction.

Pattern (three adjacent instructions, with single-use temps):

```
ZeroExtend(uchar_var, %ext)
Binary(Add, Constant(C), %ext, %addr)   # or commutative
Store(val, %addr)
```

Eligibility:
  * `%ext` and `%addr` are single-use Pseudos.
  * `uchar_var`'s c99 type is 1 byte (Char / SChar / UChar).
  * `val` is 1-byte typed (Var or Constant).
  * `0 ≤ C ≤ 0xFF00` so `C + 255` fits in the 16-bit address
    space (the 6502's absolute,X addressing wraps modulo
    0x10000; capping the base prevents an unintended wrap into
    page zero).

The replacement `IndexedStore(C, uchar_var, val)` lowers in
`tac_to_asm` to:

```
LDA val           # Mov(val, A)
LDX uchar_var     # Mov(uchar_var, X) via A
STA $C,X          # Mov(A, IndexedData(name="", offset=C, index=X))
```

The asm IR's `IndexedData` operand has been extended: when its
`name` field is empty, the address is read directly from
`offset` (rendered as `$XXXX,X` instead of `name+offset,X`). The
existing static-array load path uses `name`-keyed `IndexedData`;
the IndexedStore lowering uses the empty-name variant for raw
numeric bases.

The end-to-end composition (`static T * const` + const subscript
+ reassoc + recognize) turns

```c
static uint8_t * const buf = (uint8_t * const)0x2000;
buf[100 + col] = value;
```

(where `col` is uchar) into a single

```
LDA value
LDX col
STA $2064,X
```

— 7 bytes / 11 cycles, vs the original ~19 bytes / ~30 cycles
with separate 16-bit pointer arithmetic + DPTR-staged indirect-Y
store.

## Copy folding (`passes/optimization/copy_folding.py`)

TAC-level pass that fuses adjacent `<producer dst=%t>; Copy(%t,
X)` pairs into `<producer dst=X>` when `%t` is single-use across
the function. Runs inside the TAC fixed-point loop (alongside
constant_fold / reduce_strength / cmp_zero_jump_fold / UCE /
copy_propagate / DSE) AND once more after `from_ssa` (the SSA
destruction pass emits Copies at predecessor block ends to feed
each Phi's source into the Phi's dst — those Copies are the
loop-counter `i++` shape, fusable but not yet present during the
fixed-point loop).

The fusion handles two distinct cases:

  1. **Non-SSA-promoted dst** (the unique contribution of this
     pass). c99_to_tac emits `Binary(Add, x, 1, %t); Copy(%t, x)`
     for `x += 1` where x is a static or address-taken local —
     names that aren't SSA-renamed. copy_propagation can't
     forward `Copy(%t, x)` because x isn't an SSA-renamed name;
     fusion redirects the producer's dst to x, eliminating the
     temp.
  2. **SSA-renamed dst**. After `from_ssa` lowers each Phi to
     `Copy(%phi_arg, %phi_dst)` at predecessor block tails,
     those Copies have an SSA-renamed dst. Fusion redirects the
     producer's dst to `%phi_dst` directly, dropping the round
     trip. (Inside the fixed-point loop this case is also
     handled by copy_propagation + DSE — fusion is just a
     faster equivalent.)

Eligible producers (any TAC instruction with a single Var dst):
SignExtend, ZeroExtend, Truncate, the six FP-conversion casts
(IntToFloat, IntToDouble, FloatToInt, DoubleToInt, FloatToDouble,
DoubleToFloat), Unary, Binary, Copy (chained-copy elimination),
GetAddress, Load, IndexedLoad, FunctionCall (when its dst is
non-None), IndirectCall (same).

Phi is deliberately excluded — Phi.dst is always an SSA-renamed
name in the IR shape this pass sees, and SSA construction's
invariant (one def per renamed name) keeps it that way until
SSA destruction. Redirecting Phi.dst would let the SSA
destruction emit Copies into a non-renamed name, which complects
a different concern with this pass's job.

Soundness gates:
  * The Copy is the immediately-next instruction (adjacency).
    Without intervening side effects, no other op observes `%t`
    or `X` between the producer and the Copy, so redirecting is
    semantically identical.
  * `%t` is used exactly once across the function. The use-count
    check makes the fusion sound regardless of `%t`'s SSA
    status — multi-def `%t` (uncommon outside non-SSA) is fine,
    since after fusion any remaining def writes a name nothing
    reads (DSE picks them up next iteration).
  * `X` doesn't have to be SSA-renamed. The fusion preserves
    SSA: if `X` was renamed, it had exactly one def (the Copy);
    after fusion it still has one def (the redirected
    producer). If `X` is non-renamed (static), it had multiple
    defs; after fusion it still has multiple defs (one redirected
    here).

The composition with the multi-byte INC peephole is the headline
win for the static-RMW case: `static int x; x += 1;` previously
lowered to ~25 bytes (read X to %t through ADC chain, then Copy
%t back to X). After copy folding it becomes in-place
`Binary(Add, x, 1, x)`; tac_to_asm emits `LDA x; CLC; ADC #1;
STA x; LDA x+1; ADC #0; STA x+1`; the INC peephole then collapses
to `INC x; BNE done; INC x+1; done:` — 8 bytes total.

What still doesn't fire:
  * `Op(... %t); ...; Copy(%t, X)` with intervening
    instructions. Could be lifted with a more thorough
    aliasing/liveness check in the gate, deferred until a
    motivating case appears.
  * Asm-SSA-internal Phi destruction copies. The TAC fusion
    fires before tac_to_asm, but tac_to_asm and the asm-level
    SSA round-trip introduce their OWN Phi destruction copies
    on Pseudos that asm regalloc didn't coalesce. Those would
    need an asm-level analog of this pass — backward_copy_
    propagation handles a related shape but explicitly defers
    Pseudo-to-Pseudo coalescing to regalloc.

## Move coalescing (`passes/optimization_asm/coalescing.py`)

Asm-level SSA-era pass that merges move-related Pseudo pairs in
the interference graph when they don't interfere. Runs between
`build_interference` and `color_graph`. The point: ensure the
two ends of every `Mov(Pseudo a, Pseudo b)` and every
`(Phi.dst, PhiArg.source)` pair get the SAME ZP color whenever
that's safe. After SSA destruction the corresponding Mov
becomes `Mov(ZP($X), ZP($X))` — a self-Mov that asm_emit's
self-Mov peephole drops, eliminating the temp-routing round
trip.

Move-related pairs come from two sources:
  * `Mov(Pseudo a, Pseudo b)` — explicit Pseudo-to-Pseudo copy
    in the asm IR.
  * `(Phi.dst, PhiArg.source)` — SSA destruction would emit a
    `Mov(source, dst)` for this pair at the predecessor block's
    tail.

Eligibility filters:
  * Both names must be in the interference graph (statics,
    address-taken, params are excluded upstream).
  * Same width (the coloring pool's slot search is width-aware;
    coalescing different widths would force one node into the
    other's slot layout).
  * No interference edge between them (coalescing two
    interfering nodes would force them to share a color, which
    can't be correct).
  * Both Pseudos have `offset == 0` (asm-SSA-renamed names; a
    non-zero offset marks an unrenamed multi-byte name needing
    contiguous bytes, not the same byte).

Algorithm: aggressive (Chaitin-style) — for each candidate pair
in instruction order, look up the union-find class
representatives, check eligibility, and merge by absorbing one
node's edges and `lives_across_call` flag into the other. The
spill check is implicit via the existing `Coloring.spilled`
fallback: if a coalesced node ends up with too-high degree to
fit in any pool, spilling kicks in. With the default 128-byte
ZP pool this hasn't been observed in practice on c6502
programs.

The `CoalesceResult.representative` map projects every coalesced
non-rep name to its rep. The optimizer driver expands the
post-coloring assignments through this map so `apply_coloring`
sees every original SSA name mapped to its merged color.

The headline win: a loop-counter `for (uint8_t i = 0; i < N;
i++) ...` previously routed the increment through a temp
because asm-SSA Phi destruction emitted a `Mov(.v_post_inc,
.v_phi)` with the two SSA names colored to different ZP slots.
With coalescing, .v_phi / .v_init / .v_post_inc all share one
slot; the inserted Mov is a self-Mov dropped at emit; and the
in-place ADC chain that remains is collapsed by the multi-byte
INC peephole. End result: `i++` becomes a single `INC $XX`
(uchar) or `INC $XX; BNE done; INC $YY; done:` (int).

## Const-static fold (`passes/optimization_asm/const_static_fold.py`)

Program-level prepass that runs first inside `optimize_program`.
A `static T const x = <const-init>` (file-scope, internal linkage,
const-qualified, single foldable scalar init) whose address is
never taken in the program is genuinely immutable in c6502's
single-TU model: `static` keeps the symbol invisible at link time,
`const` rejects writes to it, and "no address taken" means no
runtime path observes the storage location. Every reference to its
bytes can therefore be replaced with the corresponding immediate
at compile time, and the storage cells freed.

A `StaticVariable` top-level becomes a candidate when:
  * `is_global` is False (internal linkage — `static` at file
    scope or any block-scope `static`),
  * the symbol-table type carries an outermost `Const(...)`
    wrapper (not recursed — `Pointer(Const(Int))` is `const int *`
    pointee, not a `int * const` pointer; that wouldn't be us),
  * `init` is a single CharInit / IntInit / LongInit /
    LongLongInit / FloatInit / DoubleInit (one foldable scalar —
    arrays, AddressInit, StringInit, ZeroInit are skipped).

A candidate is then disqualified if it appears as:
  * `LoadAddress.src` — `&candidate` somewhere,
  * the dst of any write atom (Mov / Add / Sub / And / Or / Xor /
    Inc / Dec / ASL / LSR / ROL / ROR / Pop) — defensive; the
    type checker rejects writes to a const lvalue, but we don't
    silently fold past one if it slipped through,
  * an `IndexedData(name=candidate, ...)` operand (only relevant
    for static arrays in practice — defensive),
  * an `AddressInit(name=candidate, ...)` in another static's
    initializer.

For surviving candidates: every `Pseudo(name=cand, offset=k)` USE
in every function is rewritten to `Imm(byte_at(init, k))`, and
the candidate's `StaticVariable` top-level is dropped. The
asm-level `Mov(Imm, Pseudo)` shapes the rewrite leaves behind get
picked up by the existing forward-copy-prop / DCE bracket — the
fold is a setup for downstream cleanup, not a standalone pass.

The canonical case is a memory-mapped device pointer:
`static uint8_t * const hires_page1 = (uint8_t * const)0x2000;`
— every `LDA hires_page1` (3 bytes) collapses to `LDA #$00`
(2 bytes), every `LDA hires_page1+1` to `LDA #$20`, and the
2-byte storage of `hires_page1` itself disappears from the
output. External-linkage globals (without `static`) are skipped
even when const, because another TU might read the symbol.

## Peephole catalog

`compile._peephole_fixedpoint` runs the following passes in order
until a full sweep produces no change. Each pass is a separate
module under `passes/`; see the module docstring for the full
rationale and motivating shape. Order matters — each pass can
enable the next (e.g. INC chains shorten, freeing up
direct-LDX/LDY rewrites). The loop is capped at
`_PEEPHOLE_FIXEDPOINT_CAP = 16` iterations as a safety net.

Always-on (runs in both optimized AND unoptimized pipelines):

  * `apply_inc_peephole` — multi-byte ADC-#1 chains on stable
    memory → `INC + BNE` chains. See "Multi-byte INC peephole".
  * `apply_dec_peephole` — single-byte SBC-#1 chains on stable
    memory → `DEC`. (No multi-byte form: DEC sets N/Z off the
    post-decrement value, so the underflow check would have to
    sit BEFORE the DEC, not after, which doesn't match the chain
    shape.)
  * `apply_sub1_test_zero_peephole` — folds the
    `for (uint8_t i = N; i-- > 0; ) { ... }` shape's separate
    sub-and-test pair into a single `DEC M; BNE label` (or
    inverted variant) since DEC's flag side-effect IS the post-
    decrement zero test.
  * `apply_direct_index_load` — `LDA M; TAX` → `LDX M` when M
    is `Imm`/`Data`/`ZP` and `Reg(A)` is dead after. See
    "Direct-into-X/Y peephole".
  * `apply_cpx_cpy_peephole` — `Mov(X|Y, A); Compare(A, imm)` →
    `Compare(X|Y, imm)` (`CPX` / `CPY`) when the compare's left
    is already in X or Y. Loop-induction-variable test shape.
  * `apply_indirect_base_prop` — detects the 4-instruction DPTR
    stage from a ZP-resolvable pair `(N, N+1)` and rewrites
    subsequent `Indirect(off)` / `IndirectY()` operands to
    `IndirectZp(N, off)` / `IndirectZpY(N)`, bypassing DPTR.
    Composes with DSE to drop the now-dead `STA DPTR` / `STA
    DPTR+1` writes.

Only meaningful with `--optimize` (the unoptimized pipeline
skips them):

  * `apply_redundant_load_after_rmw` — drops `LDA M` after `INC
    M` / `DEC M` / shift-on-M when only the N/Z flag effect was
    needed (the RMW already set N/Z off M's new value).
  * `apply_redundant_load_elimination` — per-block A/X/Y
    tracker: if `LDA M` (or LDX/LDY M) is about to read M and
    the target register already mirrors M, drop the load.
    Heaviest after loop unrolling.
  * `apply_redundant_store_elimination` — drops STAs whose
    written cell is overwritten before any read. Memory-to-
    memory transfer redundancy (e.g. the unrolled DPTR-staging
    sequences `redundant_load`'s A-tracking can't see across an
    intervening A clobber).
  * `apply_asm_dead_store` — CFG-wide forward dead-store
    elimination on Mov-into-memory atoms. Drops or morphs (to
    LDA-only) STAs whose target byte isn't observed by any
    reachable instruction. Treats DPTR / pool ZP / local-pool
    slot symbols as dead-at-exit. `LoadAddress` is modeled
    precisely (read FP/FP+1 only for Frame src; bounded 2-byte
    write at dst) rather than opaque. `Call` /
    `FunctionPrologue` / `AllocateStack` are opaque.
  * `apply_dead_a_arith_elimination` — drops instructions whose
    only observable effect is a write to `Reg(A)` and the
    N/Z/C/V flags, when both A and the flags are dead afterward.
  * `apply_branch_invert` — `Branch(cond, L); Jump(target); L:`
    → `Branch(inverted_cond, target)` when L is the immediate
    next instruction.
  * `apply_mem_const_prop` — per-block memory-cell value tracker
    (`Data(name, off)` / `ZP(addr, off)`); substitutes the known
    immediate at any downstream operand slot that accepts `Imm`.
  * `apply_const_arith_fold` — folds `LDA #C1; ALU #C2` chains
    that produce a compile-time-known A value, replacing the
    sequence with `LDA #folded`. Most useful for the high-byte
    branch of an int-typed AND of a uchar value (`(uchar &
    0x80)` after promotion).
  * `apply_round_trip_load_drop` — drops `STA M; LDA M` where
    the LDA's only observable side effect is re-loading A with
    A's already-current value.
  * `apply_and_sign_bit_branch` — `Mov(M, A); And(Imm(0x80),
    A); Branch(EQ|NE, _)` → `BitTest(M); Branch(PL|MI, _)` when
    M is BIT-addressable (`Data` / `ZP`) AND A is dead after.
    Pays 1 byte / 3+ cycles per occurrence and preserves A for
    downstream use.
  * `apply_self_store_drop` — `Mov(M, A); ...; Mov(A, M)` where
    the body doesn't modify M and A reads M → drop the trailing
    self-store.
  * `apply_cmp_sbc_fusion` — fuses a `Compare; Branch; ...; SBC`
    pattern where the SBC's effect duplicates the Compare's
    flag set.
  * `apply_dec_inc_branch_fold` — `Dec(M)/Inc(M); Mov(M, A);
    Branch(EQ|NE, _)` → `Dec(M)/Inc(M); Branch(EQ|NE, _)` —
    drops the redundant LDA since INC/DEC already set N/Z off
    M's new value.

Two more asm-only peepholes run OUTSIDE the fixed-point loop:

  * `passes.y_peephole` (`apply_y_peephole`) — collapses
    adjacent `LDY #<off>` setups for indirect-Y accesses to the
    same or adjacent offsets into a single `LDY` plus `INY` /
    `DEY`.
  * `passes.long_branches` (`expand_program`) — rewrites
    conditional branches whose target is out of the ±127-byte
    range into `Branch(inverted_cond, .skip); Jump(target);
    .skip:`. Runs once, after every other peephole has settled.

## Direct-into-X/Y peephole (`passes/direct_index_load.py`)

Always-on asm-level peephole. `tac_to_asm` always stages a value
into `Reg(X)` or `Reg(Y)` via `Reg(A)`:

```
Mov(M, Reg(A))            ; LDA M
Mov(Reg(A), Reg(X))       ; TAX
```

This is conservatively right at lowering time — `M` is still a
`Pseudo` and could resolve to `Frame` / `Stack` / `Indirect`,
which use indirect-Y addressing that `LDX` / `LDY` don't support.
After `replace_pseudoregisters` resolves Pseudos to concrete
operand types, we can short-circuit the round trip when `M` is
addressable by `LDX` / `LDY` directly:

  * `Imm`  — `LDX #imm` (2 bytes vs `LDA #imm; TAX` = 3 bytes).
  * `Data` — `LDX abs` (3 bytes vs `LDA abs; TAX` = 4 bytes).
  * `ZP`   — `LDX zp` (2 bytes vs `LDA zp; TAX` = 3 bytes).

Saves 1 byte / 2 cycles per occurrence.

Eligibility:
  * Two consecutive instructions match `Mov(src=M, dst=Reg(A));
    Mov(src=Reg(A), dst=Reg(X|Y))`.
  * `M ∈ {Data, ZP, Imm}` — the addressing modes `LDX` / `LDY`
    support directly. `Frame` / `Stack` / `Indirect` skip.
  * `Reg(A)` is dead immediately after the second `Mov` — no
    subsequent read of A before A is redefined. Uses a forward
    liveness scan within the basic block (mirrors
    `backward_copy_propagation._a_dead_at`).

Flag soundness: `LDA M; TAX` sets N/Z based on M's value (LDA
sets, TAX overwrites with the same value); `LDX M` sets N/Z
based on M's value. Same flag state at the rewrite's exit, so
any subsequent `Branch` observes the same condition.

Runs after `replace_pseudoregisters` (operands concrete) and
before `expand_long_branches` (no new branches introduced — the
pass shrinks code, never expands). Active in both optimized and
unoptimized pipelines, like `inc_peephole`.

## Multi-byte INC peephole (`passes/inc_peephole.py`)

Always-on asm-level peephole that runs after
`replace_pseudoregisters` (so operands are concrete `Data`/`ZP`/
`Frame`/etc.) and before `expand_long_branches` (so any new BNE
displacements participate in long-branch checking). Detects the
multi-byte add-1 chain emitted by `tac_to_asm` and rewrites it
to an `INC + BNE` chain on the target memory operand.

The chain pattern (per byte, in order):
  * Byte 0: `Mov(M[0], A); ClearCarry; Add(Imm(1), A);
    Mov(A, M[0])` — 4 instructions, in-place RMW on M[0].
  * Each continuation byte k≥1: `Mov(M[k], A);
    Add(Imm(0), A); Mov(A, M[k])` — 3 instructions, in-place
    RMW on M[k]; no CLC since the carry threads from the prior
    ADC (LDA only sets N/Z, leaves C intact).

Eligibility (per-byte; failures break the chain at that byte):
  * `M[k]` is `Data(name, k)` or `ZP(addr, 0)`. The 6502's INC
    has zp / abs / zp,X / abs,X modes — no `(ind),Y` — so
    `Frame` / `Stack` / `Indirect` operands stay as ADC chains.
  * The pattern's per-byte LDA source equals the STA destination
    (in-place). After SSA destruction routes through a temp
    (common for parallel-copy ordering), `LDA $84; ... STA $82`
    isn't in-place on $84 and we skip — INC would corrupt $84
    instead of producing the right answer through the temp.

Bytes don't have to be at consecutive addresses. Byte-granular
asm SSA + regalloc may color the bytes of one logical multi-byte
value to non-adjacent ZP slots — the structural pattern (CLC-
ADC#1 first, ADC#0 continuations, every byte in-place RMW'd) is
only emitted by the multi-byte add-1 lowering, so wherever the
bytes live, INC + BNE preserves semantics.

Replacement for an N-byte chain:
  * N == 1: a bare `Inc(M[0])` — no branch needed (caller flow
    continues naturally).
  * N >= 2: `Inc(M[0]); Branch(NE, .inc_done@K); Inc(M[1]);
    Branch(NE, .inc_done@K); ...; Inc(M[N-1]); Label(.inc_done@K)`
    — each non-last byte's BNE skips the remaining INCs when its
    INC produced a non-zero result (no carry into the next byte).
    A fresh `.inc_done@<counter>` label is minted per chain;
    leading `.` keeps it dasm-local (per-SUBROUTINE), `@<digits>`
    keeps it disjoint from user labels and other translator-
    minted ones (`.if_end@<N>`, `.lb_skip@<N>`, …).

Byte / cycle savings (in-place add-1, vs the ADC chain):
  * 16-bit absolute: 17 → 8 bytes; 22 → 9 cycles (no overflow
    into high byte) or 14 cycles (with overflow).
  * 16-bit ZP: 13 → 6 bytes; 18 → 8 / 12 cycles.
  * 4-byte absolute: 33 → 18 bytes; cycle savings scale similarly.

The Z flag's value at the rewrite's exit is unreliable — it
depends on which BNE was the last to fire. The C and V flags are
left untouched by INC (the original ADC chain set them per the
final ADC). c6502's codegen never reads any of these across
separate operations (every comparison emits its own LDA that
resets N/Z, and SEC/CLC before each SBC/ADC), so the difference
is invisible to subsequent instructions.

Limitations / what doesn't fire:
  * `+= 2` and other small constants — INC only adds 1; chaining
    INCs would lose the win for n ≥ 2.
  * `-= 1` — needs a separate DEC peephole; DEC sets flags off
    its result, so detecting underflow needs an LDA + BNE BEFORE
    the DEC, not after. Not implemented yet.
  * In-place writes to a static when TAC routes through a temp.
    `static int counter; counter += 1;` lowers as `Binary(Add,
    counter, 1, %t); Copy(%t, counter)` — the ADC writes to %t,
    not counter, so the in-place check fails. A future TAC pass
    that fuses `Op(X, c, %t); Copy(%t, X)` to in-place `Op(X, c,
    X)` would unblock this case.

## Call-graph-disjoint ZP allocation

Under `--optimize`, c6502 hands each eligible function a private
range of ZP bytes for its params (zp_abi) and body locals,
allocated so no two functions on a common caller-callee path
share a byte. The "caller-saved vs callee-saved" partition
collapses for eligible functions — there's nothing to save in
the prologue because no caller's storage overlaps with the
callee's range. Eligible functions emit as bare body + RTS.

### `__attribute__((zp_abi))` (param passing)

Caller writes arg bytes directly to the callee's ZP slot
symbols (`STA __zpabi_<callee>_p<k>`); callee reads its params
from those same symbols. No `AllocateStack`, no Frame-resident
param storage. Compile-time validation (error otherwise):

- No `IndirectCall` in body (callee's ABI unknowable at the
  indirect site).
- Not on a cycle in the static direct call graph (a recursive
  call would overwrite the outer activation's still-live
  params).
- Address never taken (indirect call sites would assume the
  default soft-stack ABI).
- Param byte count fits the ZP window (default 64 bytes,
  $80–$BF). When the chain saturates, slots spill into a
  non-ZP fallback region (default $0200–$FFFF); dasm picks
  absolute addressing automatically, so the call-site / callee
  IR is unchanged.

### Body-local private pools (every eligible function)

`passes/zp_local_allocation.py` extends the same call-graph
allocation to body locals — the bytes the asm regalloc colors
for Pseudos that aren't params, statics, or spilled. Each
eligible function gets a private byte range disjoint from
every transitive caller's range AND every coexisting zp_abi
function's param slots. Eligibility:

- Defined in this TU (we need the body to size locals and
  enumerate callees).
- No `IndirectCall`.
- Not in any call-graph cycle (Tarjan SCC).
- Every direct callee is also eligible, OR is a zp_abi extern
  (treated as a bounded leaf via its declared param slots).
  A non-zp_abi extern callee disqualifies the caller.

Ineligible functions fall back to the conservative
caller/callee partition ($80..$BF caller-saved, $C0..$FF
callee-saved with the usual save/restore discipline).

### Allocation algorithm (shared between param and local passes)

Topological order over the call graph, parents first. For each
function `F`, compute the forbidden set as the union of every
already-allocated ancestor's range plus every coexisting zp_abi
function's param slots (ancestors AND descendants — both can
be on the call stack with F). Pick the lowest contiguous free
range of the required size in the ZP window; spill above $FF
on saturation. Siblings (non-comparable in the
caller-callee reachability relation) freely share addresses,
since their activations are never simultaneous on the stack.

### Pass roles

- `passes/abi_selection.py` (`select_abi`): decides which
  functions are zp_abi, mints `__zpabi_<fn>_p<k>` slot symbols.
- `passes/zp_slot_allocation.py` (`allocate_zp_slots`): binds
  the slot symbols to ZP addresses via call-graph topo.
- `passes/function_local_sizing.py` (`compute_local_bytes`):
  counts each function's regalloc-colored ZP byte footprint
  from a preliminary optimizer pass.
- `passes/zp_local_allocation.py` (`allocate_function_locals`):
  hands each eligible function a private body-local range,
  disjoint from coexisting footprints.
- `tac_to_asm` / `replace_pseudoregisters_bare_exit`: emit
  `Data(slot_symbol, 0)` operands for zp_abi param refs (both
  call-site and callee-side).
- `passes/optimization_asm/optimizer.py`: takes `local_pools`,
  passes each function's range to `color_graph` via
  `allowed_range`. When set, the regalloc draws colors
  exclusively from that range; `lives_across_call` no longer
  drives color choice.
- `replace_pseudoregisters` excludes private-pool addresses
  from the callee-save list — addresses in the private pool
  are by-construction safe across calls regardless of where
  they land in ZP.
- `asm_emit` prepends `<sym> EQU $<addr>` directives.

### Pipeline shape

```
tac → select_abi → allocate_zp_slots
    → tac_to_asm (bare_exit, abi)
    → optimize_program (preliminary, default pool)  # size only
    → compute_local_bytes
    → allocate_function_locals
    → optimize_program (final, local_pools)
    → replace_pseudoregisters_bare_exit (local_pools)
    → synthesize_prologue → peephole → long_branches
    → asm_to_asm2 → emit_program (zp_slot_symbols → EQU)
```

Two optimizer passes: the first sizes each function's local
byte demand; the second uses per-function private pools as the
regalloc's `allowed_range`.

### Future: cross-TU

Today the linker is dasm; per-TU compilation produces all the
slot-symbol `EQU` bindings inline. Phase 2 would split the EQU
emission into a separate `slots.inc` from a multi-TU linker
that runs the allocator globally. The IR shape (symbolic slot
refs in every `Data(__zpabi_*)` operand) is already prepared
for that; the per-TU compile doesn't need to change. See
`docs/leaf_zp_abi.md`.

## Function stack frame (soft stack)

Arguments and locals live on a **soft data stack** in main RAM, separate from
the 6502's hardware stack at `$0100`–`$01FF` (which is reserved for return
addresses and short-lived `PHA`/`PHP`). This dodges the 256-byte page-1 limit
and keeps return addresses out of the way during frame teardown.

Reserved zero-page: `$00`/`$01` = `SSP` (soft stack pointer, low/high),
`$02`/`$03` = `FP` (frame pointer), `$04`–`$1B` = `HARGS` (24-byte
runtime-helper exchange block — see step 8 of the pipeline for each
helper's per-byte layout; the block is sized for the largest helper,
`dadd`/`dsub`/`dmul`/`ddiv`, with 16 bytes of inputs + 8 of output). `SSP` and `FP` both point at the
**next-free byte** and grow downward. SSP/FP access is always
indirect-indexed: `LDY #off; LDA (SSP),Y` or `LDA (FP),Y`, so `Y`
is scratch for any soft-stack access. HARGS bytes are accessed
absolutely (`LDA HARGS+k` / `STA HARGS+k`); dasm picks zero-page
addressing automatically because the symbol resolves into page 0.

Inside a function `SSP` is unstable (any intra-function push shifts it). So
every function captures `FP` once in its prelude and addresses args/locals
via `FP` — codegen emits `Frame(off)` for those and the emitter lowers to
`LDY #off; LDA (FP),Y`. For `N` arg-bytes and `M` local-bytes:

- Caller subtracts `N` from `SSP`, writes args at `SSP+1…SSP+N`, `JSR`s.
- Callee prelude (skipped when `N+M == 0`): subtract `M+2` from `SSP`
  (locals + saved-FP slot), write caller `FP` into `SSP+M+1`/`SSP+M+2`,
  then `FP = SSP`. Smallest valid `FP` offset is `1` (same convention
  as `SSP`).
- Callee epilogue: `PHA` return value, `SSP = FP + M + N + 2` in one 16-bit
  add, reload caller `FP` via `(FP),Y` (with low byte routed through `X`
  so we don't corrupt the indirect base between the two reads), `PLA`, `RTS`.
- When `N+M == 0` the prelude emits nothing and the epilogue collapses to
  `RTS`.

Arg `j` is at offset `M + 3 + j` (not `M + 1 + j`) because the saved-FP slot
sits between locals and args. The README has a frame diagram and a fully
annotated sample prologue/epilogue.

## Emit atomicity conventions

- `Add`/`Sub` do **not** emit `CLC`/`SEC` themselves — the caller emits
  `ClearCarry`/`SetCarry` first. This keeps each atomic node 1:1 with a
  6502 opcode.
- The `LDY` that sets up an indirect-Y source counts as addressing-mode
  setup, not a separate logical step, so a single `Mov(Frame, Reg(A))`
  still emits `LDY #o; LDA (PTR),Y`.
- `PTR` is `SSP` for `Stack` operands, `FP` for `Frame` operands.
  Stack/Frame offsets and immediates are `0..255` (single byte).
- Unsupported reg combinations for `Mov` raise (e.g. `Reg(X) → Reg(Y)`,
  `Reg(Y) → Reg(X)` — no direct transfer instruction). Same-register
  pairs (`Reg(A) → Reg(A)` etc.) and same-operand `Mov(src, dst)`
  with `src == dst` go through the self-Mov peephole and emit `[]`
  (the peephole catches the self-copies that arise when regalloc
  gives a Phi src and dst the same color).
- `ArithmeticShiftLeft` (ASL), `LogicalShiftRight` (LSR), `RotateLeft`
  (ROL), and `RotateRight` (ROR) currently only accept `Reg(A)` as `dst`.
  The 6502's shift/rotate family has accumulator and absolute/zero-page
  modes but no indirect-Y, so soft-stack values can't be shifted in
  place — load to A, shift, store back. These atoms are present in the
  IR but `tac_to_asm` doesn't emit them yet (`<<`/`>>` go through the
  `asl` / `asr` runtime helpers); they're available for inlining
  inside the helpers themselves once those land.
- `BitTest(src)` emits NMOS 6502 `BIT src` (zp / abs addressing
  only — no `BIT #imm` on NMOS). Sets `N=bit7(src)`, `V=bit6(src)`,
  `Z=(A & src)==0`; does not modify A. Primary use is the sign-bit
  test: `BIT M; BPL target` reads bit 7 of M in 5 cycles / 3 bytes
  (zp) vs. `LDA M; AND #$80; BEQ target` at 8+ cycles / 6 bytes
  that also clobbers A. `src` must be `Data` / `ZP`; emit and the
  in-process sim assembler reject `Frame` / `Stack` / `Indirect` /
  `IndexedData` / `Reg`. Emitted by `passes.and_sign_bit_branch`
  when the optimizer recognizes a `Mov(M, A); And(Imm(0x80), A);
  Branch(EQ|NE, _)` triple.
- `Label(name)`, `Jump(target)`, and `Branch(cond, target)` are the
  control-flow atoms. `Label` emits `<name>:` at column 1 (same column
  as the function name); `Jump` is `JMP <target>`; `Branch` is one of
  `BCC`/`BCS`/`BEQ`/`BMI`/`BNE`/`BPL`/`BVC`/`BVS` per its `condition`.
  All branches/jumps are symbolic — emit doesn't compute displacements,
  the assembler does. `tac_to_asm` emits them for the inline comparison
  lowerings and for the short-circuit lowerings of `&&` / `||`
  (`JumpIfFalse` → `Mov(cond, A); Branch(EQ, target)`, `JumpIfTrue` →
  `Branch(NE, …)`; TAC `Jump`/`Label` are atom-for-atom).
- Output formatting: labels at column 1, opcodes at column 4, operands at
  column 10. Each function emits `<name>:`, then `SUBROUTINE`, blank line,
  then instructions.

## Lexer & preprocessor

The lexer treats comments as lex errors — it assumes a preprocessor has
already stripped them. `preprocessor.preprocess` wraps `pcpp` (installed as
a uv tool, used via its Python API, no shelling out). Malformed numeric
tokens (`0x` with no digits, `3e` with no exponent body) raise `LexError`
rather than being split.

`docs/*_grammar.txt` files are reference documentation for the spec grammars that
`c99.lark` implements — they aren't parsed by any tool.

## Tests

```sh
uv run python -m unittest
```

`tests/chapter_<N>/` holds sample programs from
nlsandler/writing-a-c-compiler-tests, checked in verbatim, with one
`tests/test_chapter_<N>.py` harness driving each chapter end-to-end
through `--codegen`. Each harness has the same shape: per-bucket
test methods (`valid` must compile, `invalid_lex` / `invalid_parse`
must reject at the named stage, `invalid_*` semantic buckets must be
rejected somewhere in the pipeline). Two filter sets thread through:
`_INCOMPATIBLE_VALID` for files c6502 can't compile under its narrow
integer / soft-stack model (e.g. literals beyond 16-bit Long, frames
beyond 253 bytes), and `_EXPECTED_FAILURES_CODEGEN` /
`_NOT_REJECTED_TODAY` for feature gaps that pin current behavior so
a regression OR a fix flips the test in either direction.

Multi-TU `libraries/` subdirs and platform-specific `.s` files aren't
applicable to c6502 and are skipped at import time. Every harness
class is `@unittest.skipUnless(shutil.which("pcpp"), …)`.

## Status

For a working-feature checklist (every C99 §6.x construct c6502
accepts end-to-end), see the README's `## Status` section and
`tests/STATUS.md` (chapter-by-chapter pass/fail). The chapter
test harnesses under `tests/test_chapter_<N>.py` are the
authoritative list of what compiles and runs.

This section captures only the **gaps and known imprecisions**
that an unwary contributor would otherwise discover by surprise.

### Not yet in the repo (programs assemble but won't link)

The runtime header isn't in this repo:

- Symbol pinning for `SSP` / `FP` / `HARGS` / `DPTR`, `SSP`
  initialization, reset vector.
- Integer helpers: `mul8/16/32`, `udivmod8/16/32`, `sdivmod8/16/32`,
  `asl8/16/32`, `asr8/16/32`, `lsr8/16/32`.
- FP conversion helpers: 26 functions covering
  `{i,u,l,ul,ll,ull}{2f,2d}` and `{f,d}2{i,u,l,ul,ll,ull}` plus
  `f2d` / `d2f`.
- FP arithmetic helpers: `fadd` / `fsub` / `fmul` / `fdiv` and
  the `d`-variants. The ordering helpers `flt` / `fle` / `dlt` /
  `dle` exist as Python hooks in the sim but not yet as 6502
  routines.
- `icall` trampoline (`JMP (DPTR)`).

Programs that hit any of `*` / `/` / `%` / `<<` / `>>` / FP↔int
cast / FP arithmetic / indirect call assemble cleanly through
dasm but won't link until the runtime header lands. Python-
implemented hooks in `sim/` cover all of these so simulation-
based tests still pass.

### Type-system limitations

- FP arithmetic / unary FP ops (`+`/`-`/`*`/`/` and unary `-`
  on `float` / `double`) raise `NotImplementedError` at TAC
  translation. FP conversions and static initialisers work; FP
  comparisons via `flt` / `fle` / `dlt` / `dle` work in sim
  through Python hooks.
- `long double` rejected at parse time (no 16-byte IEEE 754).
- Hex floating literals (`0x1.0p3`) lex but the parser rejects
  them — `float()` doesn't parse C hex-float syntax and the
  conversion isn't wired up.
- Function-pointer expressions don't exist yet — c6502 has no
  function-pointer call form beyond `IndirectCall` from the
  parser's restricted callee = identifier rule.
- `extern` arrays rejected.
- Some C99 init-list shapes rejected: scalar init for an array,
  brace init for a scalar, too many initializers, the C99
  subaggregate flat form (`int a[2][3] = {1,2,3,4,5,6};`).
- Constant-expression evaluator (`passes.constant_expression`)
  accepts only `Constant` literals optionally wrapped in casts;
  no Unary / Binary / Conditional folds yet. Affects
  `case <const-expr>:` and any future enum / array-size /
  bitfield-width consumer.

### Codegen imprecisions

- Comparisons on unsigned multi-byte operands use the signed
  V-corrected lowering. Correct for values whose high bit isn't
  set; incorrect for `unsigned long long` operands spanning the
  sign-bit boundary. Tracked but not fixed.
- The 8-bit signed `>>` helper (`asr8`) is mostly a placeholder
  — `signed char >>` integer-promotes to int before the shift,
  so `asr16` does the real work; `asr8` exists for completeness.
- Rvalue struct expressions used as lvalues (`f().m = …`,
  `(c?a:b).m = …`) are rejected: the sret slot has temporary
  lifetime, so assigning through it would be a memory-safety
  hole.

### Where to look for more

- `tests/STATUS.md` — chapter_18 file-by-file status.
- `tests/test_sim_differential.py` — opt-vs-unopt sim
  differential across the full chapter corpus; the
  `_OPT_DIVERGES` dict at the top is the live list of optimizer
  bugs (today empty as of 2026-05-14).
- `tests/test_sim_asm_optimized.py` — chapter_1..12 corpus run
  through `--optimize` with end-to-end return-value assertions.
