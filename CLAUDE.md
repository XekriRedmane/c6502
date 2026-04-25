# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project overview

c6502 is a C99 compiler written in Python that targets the MOS 6502. Dependencies
are managed with `uv`; `pyproject.toml` is the source of truth and `uv.lock` the
resolved set. `requirements.txt` is a hand-maintained `pip`-compatible fallback
and may lag.

## Common commands

```sh
uv sync                                         # create/update the project venv
uv run python -m unittest                       # run all tests
uv run python -m unittest tests.test_asm_emit   # run one module
uv run python -m unittest tests.test_parser.TestValidFiles.test_return_2   # run one test

uv run python compile.py <source.c> --codegen              # C → 6502 asm to stdout
uv run python compile.py <source.c> --codegen -o out.asm   # to a file (must end .asm)
uv run python compile.py - --tac < source.c                # read stdin, stop after TAC
```

`compile.py` is the only CLI; every other module is library-only. Flags it doesn't
recognize are forwarded to the preprocessor (pcpp), so `-D`, `-U`, `-I`,
`--passthru-*`, `--line-directive` etc. work the same as the `pcpp` CLI. pcpp's
own `-o` is not forwarded.

Stage-selection flags (mutually exclusive, one required with `compile.py`):
`--lex`, `--parse`, `--resolve`, `--tac`, `--codegen`. `--resolve` runs
the three name-resolution passes (identifier resolution, label resolution,
loop labeling) in that order.

## Regenerating AST modules

Each `*_ast.py` module is generated from its matching `*.asdl` by `asdl.py`.
After editing an ASDL file, regenerate:

```sh
uv run python asdl.py c99.asdl c99_ast.py
uv run python asdl.py tac.asdl tac_ast.py
uv run python asdl.py asm.asdl asm_ast.py
```

The generator emits one `@dataclass` per type. Sum-type bases are named
`Type_<name>` (to avoid colliding with Python builtins like `int`);
constructor classes keep their ASDL names. Fields become `int`, `str`,
`list[...]`, or `T | None` depending on the primitive / `*` / `?` markers.

## Compiler pipeline

`compile.py --codegen` chains ten passes, each a separate module that
takes one AST and returns another (or text for emit):

1. `parser.parse` (`parser.py`) — C source → `c99_ast`. Lark/LALR grammar
   lives in `c99.lark`. The top-level production is `function_definition*`:
   a translation unit is one or more `int NAME(<param_list>) <block>` forms.
   Every entry has a body, so the AST stores them as `Program(function_
   definition: list[Function])`. Forward declarations at file scope aren't
   accepted yet (`int foo(void);` only parses at *block* scope, not file
   scope). A `<param_list>` is `void` (empty params) or comma-separated
   `int IDENT` pairs. Parameter names are stored on the Function AST
   node (`Function(name, params, body)`) so identifier_resolution can
   rename them and the planned type-checking pass can validate calls
   against them. Block-scope `function_decl` records carry their own
   params on `Type_function_decl(name, params, body)`.
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
   `var_decl` is `int IDENT (= exp)? ;` and produces
   `Type_var_decl(name, init?)`; `function_decl` is `int IDENT
   (param_list) ;` (no body — C99 forbids nested function definitions
   at block scope) and produces `Type_function_decl(name, params,
   body=None)`. Iteration statements introduce a `for_init` rule
   covering a `var_decl` or `exp? ;` (function declarations aren't
   legal in for-init per C99 §6.8.5). The loop AST nodes (`WhileStmt`, `DoWhileStmt`, `ForStmt`,
   `BreakStmt`, `ContinueStmt`) carry an `identifier label` field that
   the parser leaves as the empty string — the loop_labeling pass
   fills it in later. The compound-
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
   identifiers, unary `-`/`~`/`!`, binary `+`/`-`/`*`/`/`/`%`/bitwise/shift/
   comparison/`&&`/`||`, parentheses, right-associative `=`, and the
   ternary `cond ? t : f`. The assignment LHS is loosened from C99's
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
   desugars `lval OP= rval` to `Assignment(lval, Binary(OP, lval, rval))`
   at parse time. The lval node is duplicated by reference, which is safe
   today because the only legal lval is a `Var` (no side effect when re-
   evaluated); when richer lvalues land (`*p`, `a[i]`, `s.f`), the rewrite
   has to materialize the address into a temp so it's evaluated once.
   Prefix `++a` / `--a` desugar the same way to `a = a ± 1`. Postfix
   `a++` / `a--` keep their own `Postfix(incdec_op, exp)` AST node
   because they evaluate to the *old* value of the operand while
   mutating it — a semantic that can't be expressed by reusing
   `Assignment` / `Binary` alone. Postfix sits at its own grammar
   level (`postfix_exp`) one tighter than `unary_exp`, so `-a++`
   parses as `-(a++)` and `++a++` as `++(a++)`.
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
   also gates `Postfix.operand`, so `1++` raises just like
   `1 = 2`. An `Assignment` additionally checks its lval is a `Var`
   (not a `Binary`, `Constant`, `Unary`, or nested `Assignment`)
   and raises "invalid lvalue" otherwise — `1+2=3`, `-a=5`,
   `(a=b)=c` all fail here. When richer lvalues (`*p`, `a[i]`,
   `s.f`) land, this check widens to an "is-lvalue" predicate.
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
   outer `a` is intact afterward. Labels, gotos, break, and
   continue pass through unchanged — they live in separate
   namespaces and are owned by later passes.
3. `passes.label_resolution.resolve_program` — `c99_ast` → `c99_ast`.
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
4. `passes.loop_labeling.label_program` — `c99_ast` → `c99_ast`.
   Mints a unique label `.loop@<N>` per iteration statement and
   stamps it onto that loop's `label` field. While walking the
   body, the same label is stamped onto every `BreakStmt` /
   `ContinueStmt` encountered, with nested loops pushing their own
   label as the current one for their body. A `break;` / `continue;`
   outside any iteration statement raises `LoopLabelingError`.
   (C99 §6.8.6.3 also allows `break` inside a switch; c6502 has no
   switch yet, so the loop pass is sole owner of break/continue
   targets right now — when switch lands its lowering will track
   its own break-target separately.) The pass runs *after*
   label_resolution: loop labels are translator-minted, not user-
   written, so they slot in only once user-defined goto / labeled-
   stmt names have already been resolved. The two namespaces are
   disjoint by construction — a user label is `.<funcname>@<orig>`
   where the part after `@` is a C identifier (starts with a letter
   or underscore), while a loop label is `.loop@<N>` where the part
   after `@` is digits, so the two forms can't ever match. Codegen
   derives concrete control-flow targets by appending suffixes
   (`_start`, `_continue`, `_break`) to the loop's base label.
5. `passes.type_checking.check_program` — `(c99_ast, SymbolTable)`.
   Walks the AST once and produces a `SymbolTable` (a `dict[str,
   Symbol]` keyed by resolved identifier name). Each `Symbol` has a
   `type` (`Int()` for variables and parameters today; `FunType
   (params: tuple[Type,...], ret: Type)` for functions, always
   `Int -> Int -> ... -> Int` right now) and a `defined` flag (True
   iff a function definition has been seen — irrelevant for
   variables today, will become meaningful for `extern` objects).
   Both `Type` subclasses are frozen dataclasses, so equality is
   structural and arity differences distinguish function types.
   The pass does NOT modify the AST; it returns the input program
   as-is alongside the populated symbol table, which `compile.py`
   currently discards (no later pipeline pass consumes it yet, but
   the API is built for future codegen passes that will).
   Errors raised (`TypeCheckError`):
   - **Function used as a variable**: `int foo(void); int x = foo;`
     — `Var(name)` lookup yields a `FunType`. Reachable because
     identifier_resolution's loosened cross-namespace lookup lets
     `Var(foo)` through when `foo` is in the function table; the
     type checker is what gives the precise diagnostic.
   - **Variable called as a function**: `int x; x();` — symmetric
     case, where `FunctionCall(name)` resolves to a non-`FunType`
     symbol via the variable-namespace fallback.
   - **Wrong arity**: `int foo(int a, int b); foo(1);` — argument
     count doesn't match `FunType.params` length.
   - **Argument type mismatch**: trivial today (every argument and
     every parameter is `Int`), nontrivial once richer types land.
   - **Incompatible redeclaration**: `int foo(int a); int foo(int
     a, int b);` — two declarations with different signatures.
     `SymbolTable.add_function` does the structural-equality check.
   - **Redefinition**: `int foo(void) { ... } int foo(void) { ... }`
     — second function definition for a name whose `defined` flag
     is already True.
   - **Initializer type mismatch**: `int x = some_func;` — exercises
     the var-init type-check path; today only fires when the
     initializer expression resolves to a `FunType` value.
   The function-name table from identifier_resolution and the
   variable-scope table both feed into the symbol table here:
   variable names arrive already unique (`@<N>.<orig>`) so a flat
   `dict` is enough — no nested scopes. Functions are pre-registered
   from their definitions before each body is checked, so a body
   can self-recurse without a forward declaration.
6. `c99_to_tac.translate_program` — `c99_ast` → `tac_ast` (three-address
   code). Both the c99 and TAC programs are now lists of function
   definitions (`Program(function_definition: list[Function])` on
   both sides); top-level c99 entries are walked in source order,
   each yielding one TAC `Function(name, params, instructions)`.
   Parameter names ride through unchanged — they were renamed to
   `@<N>.<orig>` by identifier_resolution and TAC `Var(@<N>.<orig>)`
   references in the body see the same names. Each TAC function
   gets an implicit `Ret(Constant(0))` appended if its body falls
   off without an explicit return (C99 §5.1.2.2.3 mandates this for
   `main`; we apply it generally so every TAC function terminates,
   even when some execution paths forgot a return). `FunctionDecl`
   block items lower to nothing — they're a name-binding artifact
   for `identifier_resolution`, not runtime state. `FunctionCall(
   name, args)` lowers to: evaluate each arg in source order
   (left-most temp first), collect the resulting TAC vals, mint a
   fresh dst temp, and emit a single `FunctionCall(name, args,
   dst)` TAC instruction. The dst temp is what the call expression
   returns, so chained uses (`x = f(); y = f() + 1`) thread cleanly
   through `Copy` / `Binary` / `Ret` etc.
   Compound expressions flatten into ops, materializing each
   intermediate into a fresh `Var(%n)`. `Binary(op, src1, src2,
   dst)` evaluates `src1` first so its temps get lower numbers.
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
7. `tac_to_asm.translate_program` — `tac_ast` → `asm_ast`. Each TAC
   instruction lowers into a sequence of atoms (`Mov` to/from `A`, atomic ops
   on `A`, carry setup if needed). Output is correct but redundant — every
   intermediate is materialized through a `Frame` slot. Optimization is
   deferred to TAC-level passes. **Asymmetry:** tac.asdl is now plural
   (`Program(function_definition*)`) but asm.asdl is still singular,
   so this pass currently asserts exactly one TAC function and
   translates it. Multi-function asm is gated on the calling-
   convention work that also enables lowering TAC `FunctionCall`
   instructions — until both land, the user-call form raises
   `NotImplementedError` here. The runtime helper calls (`mul8` /
   `divmod8` / `shl8` / `asr8`) emitted by the binary-op lowerings
   bypass TAC `FunctionCall` entirely; they go straight to
   `asm_ast.Call`, so they keep working.
8. `passes.replace_pseudoregisters.replace_program` — assigns each `Pseudo(name)` a
   `Frame(offset)` slot. Per function: walks instructions in order, mints
   offsets `args_bytes+1`, `args_bytes+2`, … for each new pseudo name; reuses
   the same offset for repeated names. `args_bytes` is `0` currently.
9. `passes.allocate_stack.allocate_program` — finds each function's `M` (highest
   `Frame` offset = local-byte count), prepends
   `FunctionPrologue(arg_bytes=0, local_bytes=M)`, and rewrites every
   `Ret(...)` to carry the same `arg_bytes`/`local_bytes`.
10. `asm_emit.emit_program` — `asm_ast` → 6502 assembly text. **Atomic IR**:
    every node maps to one 6502 instruction, except `Ret` and
    `FunctionPrologue`, which expand to the multi-instruction prelude/epilogue.

`Pseudo` operands at emit time are an error — they must have been resolved by
step 8 (`replace_pseudoregisters`). `Mul`/`Div`/`Mod`/`LeftShift`/`RightShift`
are TAC-only concepts;
`tac_to_asm` lowers each to a `Mov`/`Mov`/`Mov`/`Call`/`Mov` sequence
targeting one of the runtime helpers `mul8` / `divmod8` / `shl8` / `asr8`.
All take operands in `A` and `X`: `mul8` returns low/high in A/X, `divmod8`
returns quotient/remainder in A/X, `shl8` returns `A << X` (logical) in A,
`asr8` returns `A >> X` (arithmetic, sign-preserving) in A. Right shift
goes through the signed helper because c6502 currently treats all integers
as signed. The unary `LogicalNot` is lowered inline (no runtime helper):
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

## Function stack frame (soft stack)

Arguments and locals live on a **soft data stack** in main RAM, separate from
the 6502's hardware stack at `$0100`–`$01FF` (which is reserved for return
addresses and short-lived `PHA`/`PHP`). This dodges the 256-byte page-1 limit
and keeps return addresses out of the way during frame teardown.

Reserved zero-page: `$00`/`$01` = `SSP` (soft stack pointer, low/high),
`$02`/`$03` = `FP` (frame pointer). Both point at the **next-free byte** and
grow downward. Access is always indirect-indexed: `LDY #off; LDA (SSP),Y` or
`LDA (FP),Y`, so `Y` is scratch for any soft-stack access.

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
- Unknown reg combinations for `Mov` (e.g. `Reg(X) → Reg(Y)`, `Reg(A) → Reg(A)`)
  raise — there's no direct transfer instruction.
- `ArithmeticShiftLeft` (ASL), `LogicalShiftRight` (LSR), `RotateLeft`
  (ROL), and `RotateRight` (ROR) currently only accept `Reg(A)` as `dst`.
  The 6502's shift/rotate family has accumulator and absolute/zero-page
  modes but no indirect-Y, so soft-stack values can't be shifted in
  place — load to A, shift, store back. These atoms are present in the
  IR but `tac_to_asm` doesn't emit them yet (`<<`/`>>` go through the
  `shl8` / `asr8` runtime helpers); they'll be useful once 16-bit
  shifts land.
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

`tests/` holds sample programs from nlsandler/writing-a-c-compiler-tests
(chapter 1), checked in verbatim:

- `tests/invalid_lex/` — must fail at lex time (exercised by `TestInvalidLex` in `test_lexer.py`).
- `tests/invalid_parse/` — must lex cleanly but fail at parse time (`TestInvalidParseFiles` in `test_parser.py`).
- `tests/valid/` — must parse into `int main(void) { return N; }` (`TestValidFiles` in `test_parser.py`).

The file-based test classes skip themselves if `pcpp` isn't on `PATH`.

## Status (what works end-to-end through `--codegen`)

- `int main(void) { <block_item>* }`, where a block item is a
  declaration (`int x;` or `int x = exp;`) or a statement (`return
  exp;`, `exp;`, `goto label;`, `label: stmt`, a nested `{ ... }`
  block, or a null `;`). If the body has no `return`, the TAC
  translator appends an implicit `Ret(Constant(0))` (C99
  §5.1.2.2.3 for `main`; applied generally so every function
  terminates). Nested blocks open a new variable-resolution scope
  (per-block clone with outer-vs-inner flagging — see pass 2);
  shadowing across blocks is legal, redeclaration in the same
  block is not.
- `int main(void)` returning a single integer expression
- integer constants
- unary `-`, `~`, and `!` (`!` lowers inline to `Branch(EQ) + 0/1 select`
  — no runtime helper; the framing `LDA` already sets Z)
- binary `+`, `-`, `*`, `/`, `%` (the multiplicative ops emit `JSR mul8` /
  `JSR divmod8` against the runtime helpers — see below)
- binary `&`, `|`, `^` (lower to single 6502 `AND`/`ORA`/`EOR`)
- binary `<<` (logical) and `>>` (arithmetic; c6502 assumes signed
  integers right now). Both emit `JSR shl8` / `JSR asr8` against the
  runtime helpers
- binary `==` and `!=` (lower inline to `CMP` + `BEQ`/`BNE` + a 0/1
  select — no runtime helper)
- binary `<`, `>`, `<=`, `>=` (signed; lower inline to `SBC` with a
  V-flag correction `BVC novf; EOR #$80` and then `BMI`/`BPL` + a 0/1
  select. `>` and `<=` swap their operands so the same MI/PL branches
  work. c6502 assumes signed integers right now, so this matches C's
  relational semantics for `int`)
- binary `&&` and `||` (short-circuit; `c99_to_tac` lowers them to
  `JumpIfFalse`/`JumpIfTrue` + `Jump`/`Label`/`Copy`, then `tac_to_asm`
  lowers the conditional jumps to `Mov(cond, A); Branch(EQ|NE, target)`
  — LDA sets Z based on the loaded byte, so BEQ/BNE drives off C's
  falsy/truthy directly. Copy becomes a single `Mov`; Jump and Label
  are atom-for-atom. No runtime helper and no TAC binop — the control
  flow *is* the semantics)
- compound assignments `+=`, `-=`, `*=`, `/=`, `%=`, `&=`, `|=`, `^=`,
  `<<=`, `>>=` (desugared by the parser to `lval = lval OP rval`, so
  they reuse the same TAC/asm lowerings as their underlying binary op
  followed by a Copy back into the lval)
- prefix `++a` / `--a` (desugared by the parser to `a = a ± 1`, same
  shape as a compound assignment — returns the new value)
- postfix `a++` / `a--` (its own `Postfix(incdec_op, exp)` AST node;
  `c99_to_tac` lowers it to `Copy(a, %old); Binary(Add/Sub, a, 1, %new);
  Copy(%new, a)` and returns `%old` so the result is the operand's
  value *before* the mutation)
- `if (cond) stmt` and `if (cond) stmt else stmt` — `c99_to_tac`
  lowers to `JumpIfFalse(cond, end@N)` + body + `Label(end@N)` (no
  else); with else, `JumpIfFalse(cond, else@N)` + then-body +
  `Jump(end@N)` + `Label(else@N)` + else-body + `Label(end@N)`. Labels
  share the Translator's label counter (`.if_end@N`/`.if_else@N` —
  dasm local labels with a leading dot) with the short-circuit and
  inline-comparison lowerings, so each `if` gets globally unique
  numbers
- ternary `cond ? t : f` — `c99_to_tac` lowers it like an if/else
  that also produces a value: `<eval cond>; JumpIfFalse(cond,
  .cond_else@N); <eval t>; Copy(t, dst); Jump(.cond_end@N);
  Label(.cond_else@N); <eval f>; Copy(f, dst); Label(.cond_end@N)`
  and the Conditional expression returns `dst`. Labels
  (`.cond_else@N` / `.cond_end@N`) share the same Translator counter
  as the `if` / short-circuit / inline-comparison lowerings, so
  numbering stays globally unique across the program
- labeled statements `label: stmt` (C99 §6.8.1) and `goto label;`
  (§6.8.6). `passes.label_resolution` validates uniqueness within a
  function and that every goto target is declared in the same
  function, then rewrites both sides to `.<funcname>@<orig>` —
  dasm-style local labels (leading dot, scoped to the SUBROUTINE)
  with `@` as separator (illegal in C identifiers, so it can't
  collide with any user-written identifier). Translator-minted
  labels (`.if_end@N`, `.cond_else@N`, `.loop@N`, …) embed `@`
  too, but the part after `@` is digits there vs. a C identifier
  here, so the two forms stay disjoint. `c99_to_tac` lowers
  `Goto(L)` to `Jump(L)` and `LabeledStmt(L, s)` to `Label(L)`
  followed by lowering `s`. Labels are visible across the entire
  function body, so forward gotos are fine. Variably-modified-type
  scope-jump check (also §6.8.6) is vacuous because c6502 has no
  VLAs
- iteration statements `while (cond) stmt`, `do stmt while (cond);`,
  and `for (<for_init> exp? ; exp?) stmt`, plus the jump statements
  `break;` and `continue;`. The `for_init` slot is either a
  declaration (`for (int i = 0; ...)`) or an expression-or-empty
  (`for (i = 0; ...)`, `for (; ...)`). `passes.loop_labeling` mints
  a `.loop@<N>` per iteration statement and stamps it onto the loop
  AST node and onto every `break` / `continue` inside the body; a
  `break` or `continue` outside any loop raises
  `LoopLabelingError`. `c99_to_tac` derives `_start` / `_continue`
  / `_break` sub-labels from the base by suffix and lays out the
  three loop kinds as documented in pass 6 above. The for-header
  opens its own variable scope (C99 §6.8.5.3), so
  `int a; for (int a = 1; ...) ...` legally shadows the outer
  `a` for the duration of the loop. A missing `for` condition is
  treated as unconditionally true, so the test and its
  `JumpIfFalse` drop out of the lowered TAC entirely
- arbitrary parenthesisation

Lowered all the way through TAC (but **not yet to asm** — `tac_to_asm`
asserts a single TAC function and refuses TAC `FunctionCall` until
the calling convention is wired up):
- Function declarations at block scope: `int foo(void);` /
  `int foo(int a, int b);`. `identifier_resolution` registers the
  name in a per-program function-name set, leaves it unrenamed
  (external linkage — C99 §6.2.2), and accepts duplicate
  declarations of the same function as same-symbol redeclarations.
  `c99_to_tac` discards `FunctionDecl` block items (they're a
  name-binding artifact for earlier passes, not runtime state).
- Function calls: `f()`, `f(a, b + 1)`. Lowered to a single TAC
  `FunctionCall(name, args, dst)` instruction after evaluating
  each arg in source order. The dst temp is the call's value; the
  caller threads it through into `Copy` / `Binary` / `Ret`.
- Multiple top-level function definitions (`int foo(void) { ... }
  int main(void) { ... }`). `Program.function_definition` is a
  list on both c99 and TAC sides now; each c99 function yields one
  TAC function in source order.

Not yet in the pipeline at all: file-scope (forward) declarations,
calling-convention support that actually consumes parameters in
codegen (the IR threads `arg_bytes` everywhere but the asm
translator hardcodes 0 and `Function` definitions hand their param
names to identifier resolution / type checking only — codegen still
ignores them), `switch` statements (the loop-labeling pass is sole
owner of break-targets right now; once switch lands its lowering
will track its own break-target separately), types other than `int`
(so unsigned right shift and unsigned ordering aren't distinguishable
yet), and the runtime header that defines `SSP`/`FP`, initializes
`SSP`, sets the reset vector, and provides `mul8`/`divmod8`/`shl8`/
`asr8`.
