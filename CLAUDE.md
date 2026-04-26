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

`compile.py --codegen` chains nine passes, each a separate module that
takes one AST and returns another (or text for emit):

1. `parser.parse` (`parser.py`) — C source → `c99_ast`. Lark/LALR grammar
   lives in `c99.lark`. The top-level production is `declaration*`:
   a translation unit is a list of `var_decl` / `function_decl` forms.
   The AST stores them as `Program(declaration*)` where each declaration
   is `VarDecl(var_decl)` or `FunctionDecl(function_decl)`; a function
   *definition* is the FunctionDecl variant with `body=Block(...)`,
   while a forward declaration has `body=None`. Both `var_decl` and
   `function_decl` carry a required `data_type` (one of `Int()`,
   `Long()`, or — for functions — `FunType(params, ret)`) and an
   optional `storage_class` (`Static()` / `Extern()` / None). The
   parser builds the function's `FunType` from the per-param
   `type_specifier+` runs and the return-type specifiers. A
   `<param_list>` is `void` (empty params) or comma-separated
   `<type_specifier>+ IDENT` pairs. Parameter *names* live on the
   function_decl's `params` array; their *types* live in parallel
   on `data_type.params`.

   The type vocabulary is two integer types — `Int()` is 1-byte signed
   (-128..127), `Long()` is 2-byte signed (-32768..32767). The parser's
   `_make_const(value)` factory dispatches integer literals to the
   smallest-fitting variant: -128..127 → `ConstInt(int)`, the rest of
   the signed-2-byte range → `ConstLong(int)`, anything outside →
   `ParserError`. `Constant(const)` wraps the resulting `Type_const`.
   `_split_specifiers` validates the run of specifier tokens (`int`,
   `long`, `static`, `extern`) and splits it into the (data_type,
   storage_class) pair, rejecting multiple type specifiers, multiple
   storage classes, missing type, `long long`, and unsigned suffixes
   (`5U`).
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
   `INT` / `LONG` → cast; anything else → parenthesised exp. Each
   `Cast(target_type, exp)` carries a resolved `Int()` / `Long()`
   target type (built by the `type_name: type_specifier+` rule, which
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
   Symbol]` keyed by resolved identifier name). The data-type
   classes (`Int`, `Long`, `FunType`) live on `c99_ast` and are re-
   exported here under stable `passes.type_checking.<Name>` names so
   every consumer agrees on the type vocabulary; equality is
   structural via `@dataclass`. Each `Symbol` carries a `type` plus
   an `IdAttr` describing its runtime category:
   - `LocalAttr` — automatic-storage object (block-scope `int x;`
     / `long x;`, function parameter, or any TAC temporary
     introduced by `c99_to_tac`).
   - `StaticAttr(initial_value, is_global)` — every file-scope
     object plus block-scope `static`. `initial_value` is one of
     `Initial(c)`, `Tentative`, or `NoInitializer` per C99 §6.9.2.
   - `FunAttr(defined, is_global)` — a function name. `defined`
     flips True the first time a definition is seen.
   `is_global` is True iff the symbol has external linkage,
   materialized once here so the asm backend doesn't have to re-
   derive it from the three-way `Linkage` enum.

   The pass mutates each visited expression's `data_type?` field in
   place — every `Constant` / `Var` / `Cast` / `Unary` / `Binary` /
   `Assignment` / `Postfix` / `Conditional` / `FunctionCall` ends up
   tagged with its concrete result type. Constants pick from the
   const variant (ConstInt → Int, ConstLong → Long); Cast picks its
   target_type; Var picks the symbol's type; Unary / Postfix inherit
   the inner operand's type, except `!` which always yields Int per
   §6.5.3.3.5.

   **Implicit conversions** apply C99 §6.3.1.8's usual arithmetic
   conversions on a restricted two-type world: matching types pass
   through; an Int / Long mix promotes the Int operand to Long. The
   narrower operand is wrapped in an implicit `Cast(target=common,
   exp=…, data_type=common)` via `_convert_to(exp, target)`, so by
   the time TAC sees the tree every operand has its concrete
   data_type and any size-changing conversion is an explicit Cast
   node. The same `_convert_to` helper runs at every place C99
   specifies a conversion:
   - **Binary** operands (§6.3.1.8): both promoted to the common
     type before the op.
   - **Assignment** rval (§6.5.16.1): converted to lval's type.
     Compound assignments inherit this via parser desugaring.
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

   Static-storage initializers stay constant-expression-only:
   `_const_init_value` recursively drills through any number of
   Cast wrappers (the parser produces `Cast` for explicit casts,
   and the implicit-conversion rule wraps a mismatched literal in
   another Cast) to the underlying integer.

   Errors raised (`TypeCheckError`):
   - Function used as a variable / variable called as a function.
   - Wrong call arity.
   - Mismatched binary-operator operand types (only when neither is
     an object type — Int/Long mixes are handled by promotion now).
   - Initializer / cast / return-value types not assignable.
   - Cast target isn't an object type (no `FunType` casts).
   - Incompatible redeclaration of an object or function (signature
     differs, or linkage disagrees with prior).
   - Multiple definitions (function with `defined=True` already, or
     two distinct file-scope `Initial(c)` values for one object).
   - Static-storage initializer isn't a constant expression.

   The function-name table from identifier_resolution and the
   variable-scope table both feed into the symbol table here:
   variable names arrive already unique (`@<N>.<orig>`) so a flat
   `dict` is enough — no nested scopes. Functions are pre-registered
   from their definitions before each body is checked, so a body
   can self-recurse without a forward declaration.
6. `c99_to_tac.translate_program` — `(c99_ast, SymbolTable)` →
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
      declaration site; plain `int x [= e];` / `long x [= e];`
      lowers to a `Copy` from the evaluated initializer into the var.
   2. Iterate the symbol table once. Each `StaticAttr` entry whose
      `initial_value` is `Initial(c)` (use `c`) or `Tentative` (use
      `0`) becomes a TAC `StaticVariable`, with a typed `IntInit(v)`
      / `LongInit(v)` chosen by the variable's declared type;
      `NoInitializer` entries describe a reference to a definition
      elsewhere and emit nothing.

   The c99 and TAC ASDLs declare parallel `data_type` / `const`
   sums (`Int | Long | FunType` and `ConstInt(int) | ConstLong(int)`),
   so translation between them is a one-to-one rewrap
   (`_to_tac_data_type`, `_to_tac_const`); the helpers
   `_tac_const_for(t, v)` and `_tac_const_val(t, v)` build typed
   constants for the synthetic-constant call sites (postfix `+1`,
   short-circuit 0/1, implicit `return 0` per declared return type).

   **Cast lowering.** `Cast(target, exp)` lowers to the inner val
   plus one of:
   - `SignExtend(src, dst)` for Int → Long
   - `Truncate(src, dst)` for Long → Int
   - elide (just return inner's val) for matching types
   The source type comes from the inner node's `data_type` (set by
   the type checker); a `None` data_type — synthetic AST that
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
   sizing — both read `symbols['%N'].type` to decide between a
   1-byte and 2-byte plan. The `t=None` default is a unit-test
   backstop and resolves to Int.

   Parameter names ride through unchanged — they were renamed to
   `@<N>.<orig>` by identifier_resolution and TAC `Var(@<N>.<orig>)`
   references in the body see the same names. Each TAC function
   gets an implicit `Ret(_tac_const_val(ret_type, 0))` appended if
   its body falls off without an explicit return (C99 §5.1.2.2.3
   mandates this for `main`; we apply it generally so every TAC
   function terminates). The constant's variant matches the
   function's declared return type — Long-returning functions get
   `ConstLong(0)`, Int-returning ones get `ConstInt(0)`.
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
7. `tac_to_asm.translate_program` — `tac_ast` → `asm_ast`. The asm
   program shape mirrors TAC: `Program(top_level*)` with
   `Function(name, is_global, params, instructions)` and
   `StaticVariable(name, is_global, init)`. Each TAC `Function`
   lowers atom by atom; each TAC `StaticVariable` rides through to
   an asm `StaticVariable` unchanged, with the typed init
   (`IntInit | LongInit`) rewrapped 1-to-1. The asm side has no
   `data_type` field — the variant of the init alone determines
   the cell size at emit (DC.B for IntInit, DC.W for LongInit).

   **The asm IR is strictly 1:1 with 6502 opcodes** — no width
   tagging anywhere. The 6502 is an 8-bit machine, so every asm
   instruction is implicitly Byte-typed. That makes `tac_to_asm`
   the single home of all 16-bit lowering: for each TAC instruction
   whose operands are `Long` (per the symbol table), the translator
   emits a sequence of byte-level asm atoms — typically two passes
   (low byte, then high byte) with the 6502's carry flag threading
   naturally between them for arithmetic.

   **Per-byte addressing.** `Pseudo` and `Data` carry an `int
   offset` field that selects which byte of a multi-byte value the
   reference is — `offset=0` is the low byte (or the only byte of
   an Int), `offset=1` the high byte of a Long. The helper
   `_byte_at(operand, k)` produces the k-th byte of any operand:
   `Imm(v)` → `Imm((v >> 8*k) & 0xFF)` (using Python's arithmetic
   `>>` so a negative ConstLong folds to its two's-complement
   bytes); memory-shaped operands (Pseudo / Stack / Frame / Data)
   bump their `offset` by k.

   **Operand-size dispatch.** `Translator._size_of(val)` returns 1
   (Int) or 2 (Long) by reading the symbol table for Vars and the
   const variant for Constants. Each per-instruction lowering
   keys off this size — Int operands lower to today's single-byte
   sequences; Long operands lower to byte-pair sequences with
   carry threading where appropriate. Examples:
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
   - `Mul/Div/Mod/Shift` on Long: not implemented yet — the runtime
     helpers (mul8/divmod8/shl8/asr8) are 8-bit only and the
     16-bit equivalents (mul16/divmod16/shl16/asr16) aren't in
     this repo. `_require_byte_helper` raises `NotImplementedError`
     at TAC-translation time so the failure points at the source-
     level construct.

   **Cast lowering.**
   - `Truncate(src, dst)` (Long → Int): `Mov(_byte_at(src, 0),
     dst)` — memory is little-endian, so the source's offset-0
     byte is the low byte, and the high byte is just discarded.
   - `SignExtend(src, dst)` (Int → Long): inline byte sequence —
     `Mov(src, A); Mov(A, dst.lo); Branch(MI, sx_neg@N); LDA #$00;
     Jump(sx_done@N); Label(sx_neg@N); LDA #$FF; Label(sx_done@N);
     Mov(A, dst.hi)`. The framing LDA sets N based on the source
     byte's sign; STA preserves flags so BMI sees the right N.
     The two minted labels come from the Translator's program-
     global counter.

   Output is correct but redundant — every intermediate is
   materialized through a `Frame` slot. Optimization is deferred to
   TAC-level passes.

   **TAC `FunctionCall(name, args, dst)`** lowers to the caller-
   side soft-stack convention: `AllocateStack(total_arg_bytes)`
   (each Long arg contributes 2 bytes, each Int 1), one Mov per
   arg byte writing into `Stack(1)..Stack(total_arg_bytes)` in
   source order (low byte at the lower offset for Long args),
   `Call(name)`, then capture the return value. Int return: Mov A
   → dst. Long return: Mov A → dst.lo; Mov X → A; Mov A → dst.hi
   (return convention: A = low byte, X = high byte, matching the
   mul8/divmod8 helpers). The callee's epilogue rewinds SSP all
   the way back to the caller's pre-call value, so there's no
   per-call cleanup. The runtime helper calls (`mul8` / `divmod8` /
   `shl8` / `asr8`) emitted by the binary-op lowerings still go
   straight to `asm_ast.Call` (no `AllocateStack`); they take
   their operands in registers, not on the soft stack, so they
   bypass the user-function calling convention entirely.
8. `passes.replace_pseudoregisters.replace_program` — replaces every
   `Pseudo(name, offset)` operand with a `Frame(offset)` (or
   `Data(name, offset)` for static-storage references) and lays
   out the function's stack frame. Takes the type-checker's
   SymbolTable so it can size each pseudo by its declared type:
   `Long` → 2 contiguous bytes, `Int` (or unknown) → 1 byte.
   Walks each function twice:
   - **Pass 1 (discovery):** mint a *base* offset (the offset of
     byte 0) for every Pseudo name that *isn't* in the function's
     `params` and isn't in the program's static-storage set.
     Locals get sequential base offsets in source-encounter order,
     each advancing the cursor by `_size_of_name(name)`. After the
     walk, M = total local bytes.
   - **Finalize:** compute param base offsets analogously. The
     first param's first byte is at `Frame(M + 3)`; each subsequent
     param starts after the previous one's bytes. The 2-byte gap
     at M+1, M+2 holds the saved caller FP.
   - **Pass 2 (replacement):** rewrite each Pseudo operand. A
     name in the static set becomes `Data(name, offset=k)`
     (absolute addressing, asm_emit renders as `LDA name+k`). A
     name in the local or param maps becomes `Frame(base + k)`
     where `k` is the Pseudo's `offset` field — so `Pseudo(name,
     offset=1)` accesses the high byte of a Long that was
     allocated 2 contiguous bytes starting at `base`.
   The pass also prepends `FunctionPrologue(arg_bytes=N,
   local_bytes=M)` and patches every `Ret(...)` with the same N/M,
   so the emitter has the dimensions it needs for the prologue's
   space-allocation step and the epilogue's SSP-rewind.
9. `asm_emit.emit_program` — `asm_ast` → 6502 assembly text.
   **Atomic IR**: every node maps to one 6502 instruction, except
   `Ret` and `FunctionPrologue` (multi-step compound nodes
   documented above) and `AllocateStack(N)` which expands to the
   16-bit `SSP -= N` sequence (SEC; LDA SSP; SBC #lo; STA SSP; LDA
   SSP+1; SBC #hi; STA SSP+1). Multi-function programs emit each
   function's body in source order separated by a single blank
   line. `Data(name, offset)` operands render as `LDA name` for
   offset 0 (the common case) and `LDA name+offset` otherwise —
   the assembler resolves the symbol+offset to a fixed address.
   Top-level `StaticVariable(name, _, init)` emits as `<name>:`
   followed by `DC.B $XX` for `IntInit(int=v)` and `DC.W $XXXX`
   for `LongInit(int=v)` (the W form masks to 16 bits so signed-
   negative values render as two's-complement; dasm's `DC.W` lays
   the bytes down little-endian, matching the soft-stack memory
   model — so `Data(name, offset=1)` accesses the high byte of a
   Long static).

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
  declaration (`int x;` / `long x;` / `int x = exp;` / `long x = exp;`)
  or a statement (`return exp;`, `exp;`, `goto label;`, `label:
  stmt`, a nested `{ ... }` block, or a null `;`). If the body has
  no `return`, the TAC translator appends an implicit
  `Ret(_tac_const_val(ret_type, 0))` (C99 §5.1.2.2.3 for `main`;
  applied generally so every function terminates), with the
  constant variant chosen by the function's declared return type.
  Nested blocks open a new variable-resolution scope (per-block
  clone with outer-vs-inner flagging — see pass 2); shadowing
  across blocks is legal, redeclaration in the same block is not.
- The `long` type and explicit `(int)` / `(long)` casts parse,
  type-check, and lower all the way through to 6502 asm. 16-bit
  arithmetic (`+`, `-`, bitwise ops, comparisons, equality, sign-
  extension on Cast Int→Long, truncation on Cast Long→Int) is
  expanded into byte-level sequences by `tac_to_asm` — each
  operand's type drives the dispatch via the symbol table. Long-
  typed locals / params / temporaries occupy 2 contiguous frame
  bytes; Long return values come back with low byte in A and high
  byte in X (matching the mul8/divmod8 helper convention). The one
  remaining gap: `*`, `/`, `%`, `<<`, `>>` on Long operands raises
  `NotImplementedError` in `tac_to_asm` because the 16-bit runtime
  helpers (`mul16` / `divmod16` / `shl16` / `asr16`) aren't in
  this repo yet.
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

Lowered all the way to 6502 asm:
- Function declarations at block scope: `int foo(void);` /
  `int foo(int a, int b);`. `identifier_resolution` registers the
  name in a per-program function-name set, leaves it unrenamed
  (external linkage — C99 §6.2.2), and accepts duplicate
  declarations of the same function as same-symbol redeclarations.
  `c99_to_tac` discards `FunctionDecl` block items (they're a
  name-binding artifact for earlier passes, not runtime state).
- Function calls: `f()`, `f(a, b + 1)`. Lowered through TAC to
  the soft-stack calling convention: caller subtracts N from SSP,
  writes args at Stack(1)..Stack(N), JSRs, copies the return value
  out of A. The callee's prologue saves the caller's FP, captures
  its own FP, and the epilogue rewinds SSP all the way back to
  the caller's pre-call value — no per-call cleanup.
- Multiple top-level function definitions (`int foo(void) { ... }
  int main(void) { ... }`). c99's `Program.declaration` is a list
  of VarDecl/FunctionDecl entries; TAC's and asm's `Program.top_
  level` are parallel lists of Function/StaticVariable entries;
  each c99 function definition yields one TAC function yields one
  asm function, all emitted in source order separated by blank
  lines.
- Function parameters land at Frame(M+3)..Frame(M+2+N) in the
  callee's frame; locals at Frame(1)..Frame(M); saved caller FP
  at the 2-byte gap M+1, M+2.

Not yet in the pipeline at all: `switch` statements (the loop-
labeling pass is sole owner of break-targets right now; once switch
lands its lowering will track its own break-target separately),
unsigned types (so unsigned right shift and unsigned ordering
aren't distinguishable from signed yet), the 16-bit runtime
helpers (`mul16` / `divmod16` / `shl16` / `asr16`) needed to lower
`*` / `/` / `%` / `<<` / `>>` on Long operands, and the runtime
header that defines `SSP`/`FP`, initializes `SSP`, sets the reset
vector, and provides `mul8`/`divmod8`/`shl8`/`asr8`.
