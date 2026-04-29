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
uv run python -m unittest tests.test_chapter_1.TestChapter1Valid    # run one test

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
   `Long()`, `UInt()`, `ULong()`, `Float()`, `Double()`, or — for
   functions — `FunType(params, ret)`) and an optional
   `storage_class` (`Static()` / `Extern()` / None). The parser
   builds the function's `FunType` from the per-param
   `type_specifier+` runs and the return-type specifiers. A
   `<param_list>` is `void` (empty params) or comma-separated
   `<type_specifier>+ IDENT` pairs. Parameter *names* live on the
   function_decl's `params` array; their *types* live in parallel
   on `data_type.params`.

   The type vocabulary is four integer types, two floating types,
   plus pointers and function types. Integers: `Int()` is 1-byte
   signed (-128..127), `Long()` is 2-byte signed (-32768..32767),
   `UInt()` is 1-byte unsigned (0..255), `ULong()` is 2-byte
   unsigned (0..65535). Floating: `Float()` is IEEE 754 single
   (4 bytes), `Double()` is IEEE 754 double (8 bytes). `long
   double` (16-byte IEEE 754 quad / extended) isn't modelled — the
   parser rejects it. `Pointer(referenced_type)` is a 2-byte
   address (the 6502's address width); declared with `*` in the
   declarator, e.g. `int *p;`. At the byte level a pointer is
   indistinguishable from a `Long`, so `_to_tac_data_type` collapses
   `Pointer` onto TAC `Long` and the size-dispatch in `tac_to_asm`
   / `replace_pseudoregisters` treats Pointer as 2-byte; the c99
   symbol table preserves the Pointer type for later passes
   (cast dispatch, dereference / address-of lowering when those
   land).

   The lexer splits integer literals into four terminals
   by suffix (`INTEGER_CONSTANT` for no suffix, `LONG_INTEGER` for
   `L`/`LL`, `UINT_INTEGER` for `U`-only, `ULONG_INTEGER` for `U+L`
   in any order), and floating literals into three (`DOUBLE_CONSTANT`
   for no suffix, `FLOAT_CONSTANT` for `f`/`F`, `LONG_DOUBLE_CONSTANT`
   for `l`/`L`). The parser's `_const_for_token` then maps each
   integer token + base (decimal vs. hex/octal) to a c99 const
   variant per the C99 §6.4.4.1 paragraph 5 type-list rule (first
   type whose range fits the value):
   * unsuffixed decimal:    int → long
   * unsuffixed hex/octal:  int → unsigned int → long → unsigned long
   * `L` decimal:           long
   * `L` hex/octal:         long → unsigned long
   * `U`:                   unsigned int → unsigned long
   * `UL` (any order):      unsigned long

   Picking `ConstInt`/`ConstLong`/`ConstUInt`/`ConstULong` from those
   lists (and rejecting any literal whose only fitting type would be
   `long long` / `unsigned long long`, since c6502 doesn't model
   them). `LL`/`ll` suffixes parse but are rejected with a
   "long long not supported" error. Floating literals follow C99
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
   `float` → Float; `double` → Double), rejecting multiple type
   specifiers, multiple storage classes, missing type, `long long`,
   `long double`, `signed unsigned`, and any FP/integer specifier
   mix (`int float`, `unsigned double`, etc.).
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
   desugars `lval OP= rval` to `Assignment(lval, Binary(OP, lval, rval))`
   at parse time. The lval node is duplicated by reference, which is safe
   today because the only legal lval is a `Var` (no side effect when re-
   evaluated); when richer lvalues land in compound assignment (`*p +=
   1`, `a[i] += 1`, `s.f += 1`), the rewrite has to materialize the
   address into a temp so it's evaluated once.
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
5. `passes.type_checking.check_program` — `(c99_ast, SymbolTable)`.
   Walks the AST once and produces a `SymbolTable` (a `dict[str,
   Symbol]` keyed by resolved identifier name). The data-type
   classes (`Int`, `Long`, `UInt`, `ULong`, `Float`, `Double`,
   `FunType`) live on `c99_ast` and are re-exported here under
   stable `passes.type_checking.<Name>` names so every consumer
   agrees on the type vocabulary; equality is structural via
   `@dataclass`. Each `Symbol` carries a `type` plus an `IdAttr`
   describing its runtime category:
   - `LocalAttr` — automatic-storage object (block-scope `int x;`
     / `long x;` / `unsigned int x;` / `unsigned long x;` /
     `float x;` / `double x;`, function parameter, or any TAC
     temporary introduced by `c99_to_tac`).
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
   const variant (ConstInt → Int, ConstLong → Long, ConstUInt →
   UInt, ConstULong → ULong, ConstFloat → Float, ConstDouble →
   Double); Cast picks its target_type; Var picks the symbol's
   type; Unary / Postfix inherit the inner operand's type, except
   `!` which always yields Int per §6.5.3.3.5.

   **Implicit conversions** apply C99 §6.3.1.8's usual arithmetic
   conversions. Floating types dominate per §6.3.1.8.1:
   * either operand `Double` → result `Double`
   * else either operand `Float` → result `Float`
   * else both operands integer → integer rules (below)

   Integer rules, keyed by C99 §6.3.1.1 conversion rank (`Int` and
   `UInt` are rank 1; `Long` and `ULong` are rank 2):
   * matching types → that type
   * both signed (or both unsigned) → the higher-rank type wins
     (Int+Long → Long, UInt+ULong → ULong)
   * mixed signedness, unsigned has rank ≥ signed → unsigned wins
     (Int+UInt → UInt, Int+ULong → ULong, Long+ULong → ULong)
   * mixed signedness, signed has higher rank and can represent
     all unsigned values → signed wins (Long+UInt → Long, since
     Long's -32768..32767 covers UInt's 0..255)

   The narrower or signed-displaceable operand is wrapped in an
   implicit `Cast(target=common, exp=…, data_type=common)` via
   `_convert_to(exp, target)`, so by the time TAC sees the tree
   every operand has its concrete data_type and any size- or
   signedness-changing conversion is an explicit Cast node. The
   same `_convert_to` helper runs at every place C99 specifies a
   conversion:
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
   see step 6 below. Rejected at the type-check boundary:
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

   The c99 and TAC ASDLs declare parallel `data_type` sums
   (Int / Long / UInt / ULong / Float / Double / FunType), so
   translating data_type is a one-to-one rewrap
   (`_to_tac_data_type`). The TAC `const` sum collapses integer
   signedness onto width — `_to_tac_const` maps `ConstUInt(v) →
   ConstInt(v)` and `ConstULong(v) → ConstLong(v)` because the
   6502 doesn't care about signedness at the byte level — but
   keeps Float and Double distinct (their IEEE 754 byte patterns
   differ). The integer value passes through unchanged; downstream
   `_byte_at` masks each byte with `& 0xFF`, so the bit pattern is
   preserved regardless of how the integer is interpreted. The
   TAC `static_init` sum keeps signedness alongside width on the
   integer side (IntInit / LongInit / UIntInit / ULongInit) and
   precision on the FP side (FloatInit / DoubleInit) —
   `_tac_static_init_for(t, v)` dispatches on the declared type
   and coerces the raw value (`int(v)` for integer variants,
   `float(v)` for FP variants), so an integer literal initializing
   a `double` static lays down as `3.0` and a Cast-wrapped FP
   initializer for an integer static lays down its truncated
   integer. The helpers `_tac_const_for(t, v)` and
   `_tac_const_val(t, v)` build typed constants for the synthetic-
   constant call sites (postfix `+1`, short-circuit 0/1, implicit
   `return 0`); they dispatch by type — `Int`/`UInt` → `ConstInt(v)`,
   `Long`/`ULong` → `ConstLong(v)`, `Float` → `ConstFloat(v)`,
   `Double` → `ConstDouble(v)`.

   **Cast lowering.** `Cast(target, exp)` lowers based on the byte
   widths of the source and target c99 types; same-width casts
   are no-ops because the 6502 has no signedness distinction:
   - same width (`Int↔UInt`, `Long↔ULong`, plus matching types) →
     elide (just return inner's val)
   - 1B → 2B with a *signed* source (`Int → Long` /
     `Int → ULong`) → `SignExtend(src, dst)`
   - 1B → 2B with an *unsigned* source (`UInt → Long` /
     `UInt → ULong`) → `ZeroExtend(src, dst)`
   - 2B → 1B (any signedness combination) →
     `Truncate(src, dst)`
   - integer → Float / Double → `IntToFloat(src, dst)` /
     `IntToDouble(src, dst)`
   - Float / Double → integer → `FloatToInt(src, dst)` /
     `DoubleToInt(src, dst)`
   - Float ↔ Double cross-precision → `FloatToDouble(src, dst)` /
     `DoubleToFloat(src, dst)`
   The six FP-conversion nodes are TAC-only (the asm IR is 1:1
   with 6502 opcodes); `tac_to_asm` lowers each to a runtime
   helper Call. The TAC nodes themselves carry no signedness or
   width info — `tac_to_asm` reads the symbol-table types of src
   and dst to pick the right helper (i2f vs. u2f vs. l2f vs. ul2f
   on the integer side, f2d / d2f on the FP side). To keep that
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
   plan: 1 byte for Int / UInt, 2 for Long / ULong, 4 for Float,
   8 for Double. The `t=None` default is a unit-test backstop and
   resolves to Int.

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
7. `tac_to_asm.translate_program` — `tac_ast` → `asm_ast`. The asm
   program shape mirrors TAC: `Program(top_level*)` with
   `Function(name, is_global, params, instructions)` and
   `StaticVariable(name, is_global, init)`. Each TAC `Function`
   lowers atom by atom; each TAC `StaticVariable` rides through to
   an asm `StaticVariable`. The asm-side init has four variants —
   the integer side carries only the two width variants (`IntInit |
   LongInit`), so TAC's `UIntInit(v)` collapses to asm `IntInit(v)`
   and `ULongInit(v)` to `LongInit(v)`; the FP side keeps Float and
   Double distinct (`FloatInit | DoubleInit`) because their IEEE
   754 byte patterns differ. The asm side has no `data_type` field
   — the variant of the init alone determines the cell size at
   emit (DC.B for IntInit, DC.W for LongInit, DC.L for FloatInit,
   two DC.Ls for DoubleInit since dasm has no native 8-byte
   directive).

   **The asm IR is strictly 1:1 with 6502 opcodes** — no width
   tagging anywhere. The 6502 is an 8-bit machine, so every asm
   instruction is implicitly Byte-typed. That makes `tac_to_asm`
   the single home of all multi-byte lowering: for each TAC
   instruction whose operands are wider than 1 byte (Long / ULong
   = 2 bytes, Float = 4, Double = 8 — per the symbol table), the
   translator emits a sequence of byte-level asm atoms — typically
   one pass per byte with the 6502's carry flag threading
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
   for Float, 8 for Double — by reading the symbol table for Vars
   and the const variant for Constants (TAC `const` collapses
   integer sign onto width, but Float / Double stay distinct). Each per-instruction lowering
   keys off this size — 1-byte operands lower to today's single-
   byte sequences; 2-byte operands lower to byte-pair sequences
   with carry threading where appropriate. Signedness only matters
   for ordering comparisons and shifts; everywhere else the byte
   sequences are identical. Examples:
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
     (mul8/divmod8/asl8/asr8 for 1-byte operands, mul16/divmod16/
     asl16/asr16 for 2-byte), and reads the result from a fixed
     offset later in the block. Inputs survive the call. Per-helper
     layout (inputs → outputs):
       mul8     A:`+0`, B:`+1`              → product:`+2..+3` (16-bit)
       divmod8  num:`+0`, den:`+1`          → quot:`+2`, rem:`+3`
       asl8/    val:`+0`, count:`+1`        → result:`+2`
        asr8
       mul16    A:`+0..+1`, B:`+2..+3`      → product:`+4..+7` (32-bit)
       divmod16 num:`+0..+1`, den:`+2..+3`  → quot:`+4..+5`,
                                              rem:`+6..+7`
       asl16/   val:`+0..+1`, count:`+2`    → result:`+3..+4`
        asr16    (1-byte count: shifts ≥16 are UB, so the high byte
                  of a promoted-to-Long count is dropped)
     `RightShift` always dispatches to the signed `asr` variant —
     c6502 currently treats every integer as signed for shift
     purposes. The 16-bit helpers themselves aren't in the repo
     yet; the lowerings emit calls to them in advance of the runtime
     header landing.

   **Cast lowering.**
   - `Truncate(src, dst)` (any 2B → any 1B): `Mov(_byte_at(src,
     0), dst)` — memory is little-endian, so the source's offset-0
     byte is the low byte, and the high byte is just discarded.
     Used for Long → Int, ULong → Int, Long → UInt, ULong → UInt.
   - `SignExtend(src, dst)` (signed 1B → any 2B): inline byte
     sequence — `Mov(src, A); Mov(A, dst.lo); Branch(MI, sx_neg@N);
     LDA #$00; Jump(sx_done@N); Label(sx_neg@N); LDA #$FF;
     Label(sx_done@N); Mov(A, dst.hi)`. The framing LDA sets N
     based on the source byte's sign; STA preserves flags so BMI
     sees the right N. Used for Int → Long and Int → ULong; the
     two minted labels come from the Translator's program-global
     counter.
   - `ZeroExtend(src, dst)` (unsigned 1B → any 2B): inline byte
     sequence — `Mov(src, A); Mov(A, dst.lo); LDA #$00; Mov(A,
     dst.hi)`. No branch needed — the new high byte is
     unconditionally zero. Used for UInt → Long and UInt → ULong.

   Output is correct but redundant — every intermediate is
   materialized through a `Frame` slot. Optimization is deferred to
   TAC-level passes.

   **TAC `FunctionCall(name, args, dst)`** lowers to the caller-
   side soft-stack convention: `AllocateStack(total_arg_bytes)`
   (each Long arg contributes 2 bytes, each Int 1), one Mov per
   arg byte writing into `Stack(1)..Stack(total_arg_bytes)` in
   source order (low byte at the lower offset for Long args),
   `Call(name)`, then capture the return value. The convention is
   width-driven: Int (1B) ← A; Long (2B) ← A=low, X=high (with X
   routed through A for the high-byte store); Float (4B) ← bytes
   read from `HARGS+8..11` byte-by-byte through A; Double (8B) ←
   bytes read from `HARGS+16..23`. The FP slots are deliberately
   the same as the FP arithmetic helpers' output slots — see
   "Return-value convention" in the README. Caller has to capture
   the FP return *immediately* after the JSR, before any other
   helper Call, since HARGS is caller-saved. The callee's
   epilogue rewinds SSP all the way back to the caller's pre-call
   value, so there's no per-call cleanup. Runtime-helper calls
   (mul8/mul16/divmod8/divmod16/asl8/asl16/asr8/asr16) emitted by
   the binary-op lowerings still go straight to `asm_ast.Call`
   (no `AllocateStack`); they exchange operands through the
   `HARGS` zero-page block instead of the soft stack, so they
   bypass the user-function calling convention entirely.
8. `passes.replace_pseudoregisters.replace_program` — replaces every
   `Pseudo(name, offset)` operand with a `Frame(offset)` (or
   `Data(name, offset)` for static-storage references) and lays
   out the function's stack frame. Takes the type-checker's
   SymbolTable so it can size each pseudo by its declared type:
   1 byte for `Int` / `UInt` / unknown, 2 for `Long` / `ULong`,
   4 for `Float`, 8 for `Double`.
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
   followed by `DC.B $XX` for `IntInit(int=v)`, `DC.W $XXXX` for
   `LongInit(int=v)`, `DC.L $WWWWWWWW` for `FloatInit(float=v)`
   (4 bytes IEEE 754 single, packed via `struct.pack` at emit
   time), and two `DC.L`s — low half, high half — for
   `DoubleInit(float=v)` (8 bytes IEEE 754 double; dasm has no
   native 8-byte directive). The W form masks to 16 bits so signed-
   negative values render as two's-complement; dasm's `DC.W` /
   `DC.L` both lay the bytes down little-endian, matching the
   soft-stack memory model — so `Data(name, offset=1)` accesses
   the high byte of a Long static, and `Data(name, offset=7)` the
   high byte of a Double static.

`Pseudo` operands at emit time are an error — they must have been resolved by
step 8 (`replace_pseudoregisters`). `Mul`/`Div`/`Mod`/`LeftShift`/`RightShift`
are TAC-only concepts;
`tac_to_asm` lowers each to a sequence of `Mov`s into the shared
zero-page block `HARGS` (`$04`–`$1B`), a `Call` to the appropriate
runtime helper (`mul8`/`divmod8`/`asl8`/`asr8` for 1-byte operands,
`mul16`/`divmod16`/`asl16`/`asr16` for 2-byte operands), and `Mov`s
reading the result back out at a helper-specific offset within HARGS
(see step 7's per-helper layout table). Right shift always dispatches
to the signed `asr` variant because c6502 currently treats all
integers as signed. The unary `LogicalNot` is lowered inline (no runtime helper):
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
`$02`/`$03` = `FP` (frame pointer), `$04`–`$1B` = `HARGS` (24-byte
runtime-helper exchange block — see step 7 of the pipeline for each
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
- Unknown reg combinations for `Mov` (e.g. `Reg(X) → Reg(Y)`, `Reg(A) → Reg(A)`)
  raise — there's no direct transfer instruction.
- `ArithmeticShiftLeft` (ASL), `LogicalShiftRight` (LSR), `RotateLeft`
  (ROL), and `RotateRight` (ROR) currently only accept `Reg(A)` as `dst`.
  The 6502's shift/rotate family has accumulator and absolute/zero-page
  modes but no indirect-Y, so soft-stack values can't be shifted in
  place — load to A, shift, store back. These atoms are present in the
  IR but `tac_to_asm` doesn't emit them yet (`<<`/`>>` go through the
  `asl` / `asr` runtime helpers); they're available for inlining
  inside the helpers themselves once those land.
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

## Status (what works end-to-end through `--codegen`)

- `int main(void) { <block_item>* }`, where a block item is a
  declaration (`int x;` / `long x;` / `unsigned int x;` /
  `unsigned long x;` / `float x;` / `double x;` / any of those
  with `= exp;`) or a statement
  (`return exp;`, `exp;`, `goto label;`, `label: stmt`, a nested
  `{ ... }` block, or a null `;`). If the body has no `return`,
  the TAC translator appends an implicit
  `Ret(_tac_const_val(ret_type, 0))` (C99 §5.1.2.2.3 for `main`;
  applied generally so every function terminates), with the
  constant width chosen by the function's declared return type.
  Nested blocks open a new variable-resolution scope (per-block
  clone with outer-vs-inner flagging — see pass 2); shadowing
  across blocks is legal, redeclaration in the same block is not.
- All four integer types (`int`, `long`, `unsigned int`,
  `unsigned long`) and explicit casts among them parse, type-
  check, and lower all the way through to 6502 asm. 16-bit
  arithmetic (`+`, `-`, bitwise ops, comparisons, equality,
  sign-extension on widening signed casts, zero-extension on
  widening unsigned casts, truncation on narrowing casts) is
  expanded into byte-level sequences by `tac_to_asm` — each
  operand's type drives the dispatch via the symbol table.
  2-byte-typed locals / params / temporaries occupy 2 contiguous
  frame bytes; 2-byte return values come back with low byte in A
  and high byte in X (two-register Long return so the epilogue
  PHA/PLA only needs to save A; X isn't touched by the SSP/FP
  arithmetic). Float (4B) and Double (8B) returns come back
  through HARGS instead — `HARGS+8..11` for Float and
  `HARGS+16..23` for Double, matching the FP arithmetic helpers'
  output slots so a function ending in `return a OP b;` for FP
  operands needs no epilogue copy. FP returns also flip
  `Ret(save_a=False)` so the epilogue skips the PHA/PLA pair —
  the SSP/FP arithmetic doesn't touch HARGS. See README's
  "Return-value convention" subsection. Mixed-type arithmetic goes
  through C99 §6.3.1.8's usual arithmetic conversions in
  `passes.type_checking` — `long + unsigned int` promotes the
  unsigned int to long via a `ZeroExtend`, `int + unsigned int`
  promotes the int to unsigned int via a same-width no-op cast,
  etc. Comparison ops still lower as if the operands are signed
  (`SBC` + V-correction); unsigned ordering would need a separate
  inline lowering that uses `BCC`/`BCS` instead of the V-corrected
  `BMI`/`BPL`. `*`, `/`, `%`, `<<`, `>>` on 2-byte operands lower
  to calls to `mul16` / `divmod16` / `asl16` / `asr16` against
  HARGS — the marshaling is in place but the helpers themselves
  aren't in this repo yet, so a Long arithmetic program will
  assemble but won't link until the runtime header lands.
- `int main(void)` returning a single integer expression
- integer constants of all four flavors per C99 §6.4.4.1
  paragraph 5: lex-time split into `INTEGER_CONSTANT` /
  `LONG_INTEGER` / `UINT_INTEGER` / `ULONG_INTEGER` by suffix,
  then parser dispatches each (token-kind, base, value) triple to
  the first variant in C99's type list whose range fits the
  value (so e.g. `0x80` lex'd as INTEGER_CONSTANT becomes
  `ConstUInt(128)` because `int`'s range stops at 127 and the
  hex/octal type list passes through `unsigned int` next, while
  `0x8000L` becomes `ConstULong(32768)` because the hex/octal `L`
  list goes `long → unsigned long`). The `LL`/`ll` suffix lexes
  but the parser rejects it ("long long is not supported")
- floating constants per C99 §6.4.4.2: lex-time split into
  `DOUBLE_CONSTANT` (no suffix) / `FLOAT_CONSTANT` (`f`/`F`) /
  `LONG_DOUBLE_CONSTANT` (`l`/`L`); parser maps each to its
  c99_ast variant — `ConstDouble` / `ConstFloat` / rejected. Static
  initialisers lay down 4 IEEE 754 single-precision bytes (one
  `DC.L`) for `Float` and 8 IEEE 754 double-precision bytes (two
  `DC.L`s, low half then high) for `Double`; runtime FP literals
  in expressions pack to a non-negative bit-pattern `Imm` at
  TAC→asm so the existing `_byte_at` shift-and-mask byte
  extraction works without special-casing FP. Hex floating
  literals (`0x1.0p3`) lex but the parser rejects them
- explicit casts among any of the four integer types lower
  through `SignExtend` / `ZeroExtend` / `Truncate` / no-op as
  appropriate. FP-involving runtime casts (int↔float, int↔double,
  float↔double) lower through six TAC-only nodes (`IntToFloat` /
  `IntToDouble` / `FloatToInt` / `DoubleToInt` / `FloatToDouble`
  / `DoubleToFloat`); `tac_to_asm` marshals operands through HARGS
  and emits `JSR i2f` / `JSR u2f` / `JSR l2f` / `JSR ul2f` (and
  the d-variants for double, the f2*/d2* family for FP→int, plus
  `JSR f2d` / `JSR d2f` for cross-precision), with the helper name
  picked from the operand's symbol-table type so signed and
  unsigned conversions go to distinct helpers. Compile-time
  constant casts are folded in Python by `c99_to_tac` (see
  `_fold_fp_cast_constant`) — no helper call. Static initialisers
  also do the conversion in Python at build time, so e.g. `double
  x = 3;` lays down `3.0` correctly. The 18 conversion helpers
  themselves aren't in this repo yet — see the runtime-header
  status note below
- unary `-`, `~`, and `!` (`!` lowers inline to `Branch(EQ) + 0/1 select`
  — no runtime helper; the framing `LDA` already sets Z)
- binary `+`, `-`, `*`, `/`, `%` (the multiplicative ops marshal
  operands into the `HARGS` zero-page block and emit
  `JSR mul8` / `JSR divmod8` for 1-byte operands or
  `JSR mul16` / `JSR divmod16` for 2-byte operands — see below)
- binary `&`, `|`, `^` (lower to single 6502 `AND`/`ORA`/`EOR`)
- binary `<<` and `>>` (`>>` is arithmetic; c6502 assumes signed
  integers right now). Both marshal through HARGS and emit
  `JSR asl8`/`JSR asr8` for 1-byte operands or `JSR asl16`/
  `JSR asr16` for 2-byte operands
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
- prefix `++a` / `--a` and postfix `a++` / `a--` — each builds its
  own AST node (`Prefix` / `Postfix`); `c99_to_tac._translate_incdec`
  lowers both with a shared read-modify-write path that handles the
  three syntactic lvalues identifier_resolution accepts (`Var`,
  `Subscript`, `Dereference`). For `Var` operands the lowering is
  `Binary(Add/Sub, a, 1, %new); Copy(%new, a)` (postfix additionally
  prepends `Copy(a, %old)` to capture the pre-mutation value).
  For `Subscript` / `Dereference` operands the byte address is
  computed exactly ONCE — via `_translate_subscript_address` for
  Subscript, `translate_exp` of the pointer expression for
  Dereference — then `Load + Binary + Store` reuse that single
  address. Evaluating the address only once is what makes
  `++arr[--i]` / `(*p++)++` correct: any side effects in the
  address-computing subexpressions fire once. Postfix returns the
  old value, prefix the new value
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
  `break` or `continue` outside any loop / switch raises
  `LoopLabelingError`. `c99_to_tac` derives `_start` / `_continue`
  / `_break` sub-labels from the base by suffix and lays out the
  three loop kinds as documented in pass 6 above. The for-header
  opens its own variable scope (C99 §6.8.5.3), so
  `int a; for (int a = 1; ...) ...` legally shadows the outer
  `a` for the duration of the loop. A missing `for` condition is
  treated as unconditionally true, so the test and its
  `JumpIfFalse` drop out of the lowered TAC entirely
- `switch (cond) stmt` with `case <const>:` and `default:` labels
  per C99 §6.8.4.2. The control expression must have integer
  type (Int / Long / UInt / ULong; Float / Double / Pointer
  rejected at type-check). Case labels are integer constant
  expressions per §6.6.6 — `passes.constant_expression`
  evaluates them and the type checker converts each to the
  switch's promoted control type modulo width, then validates
  uniqueness on the converted values (so e.g. `case 256:` in an
  Int switch wraps to 0 and conflicts with `case 0:`).
  `passes.loop_labeling` mints `.switch@<N>` per switch and
  `.case@<N>` / `.default@<N>` per labeled case (collecting
  them into `SwitchStmt.cases` / `.default_label`), stamps the
  switch label onto every `break;` inside (preserving any
  enclosing loop's continue target — a `continue;` inside a
  switch inside a loop still finds the loop). Rejects: case /
  default outside any switch, duplicate `default:` within one
  switch (case-value uniqueness is the type checker's job).
  `c99_to_tac` lowers the switch to a compare-and-conditional-
  jump dispatch chain (`Binary(Equal,...)` + `JumpIfTrue` per
  case), then an unconditional `Jump` to the default label (or
  to `<switch>_break` if no default), then the body with case /
  default labels emitted inline at their source positions, and
  finally `Label(<switch>_break)`. Cases fall through unless a
  `break;` (lowered to `Jump(<switch>_break)`) is hit. Case
  bodies can sit inside if / loop / compound bodies (Duff's
  device) — the labeling pass descends through those preserving
  the current-switch pointer, only swapping it for nested
  switches. Today the constant-expression evaluator only
  accepts `Constant` literals optionally wrapped in integer
  Casts; a future expansion adds Unary / Binary / Conditional
  arms (so `case 1+2:` / `case (1<<3):` / `case enum_const:` —
  once enums land — would work)
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
- Pointer arithmetic per C99 §6.5.6: `ptr + int`, `int + ptr`,
  `ptr - int`, and `ptr - ptr` (matching pointer types). The
  integer operand is widened to Long by the type checker, then
  scaled by `sizeof(pointee)` in `c99_to_tac` before the underlying
  byte-level Add/Subtract; `ptr - ptr` subtracts and divides by
  `sizeof(pointee)` to recover an element count (result type
  Long, c6502's stand-in for `ptrdiff_t`). Pointer-to-Int has
  size 1 so no scaling is emitted; pointer-to-{Long,ULong,Pointer}
  scales by 2, pointer-to-Float by 4, pointer-to-Double by 8 —
  using `mul16` / `divmod16` runtime helpers, so a non-trivial
  pointer-arithmetic program assembles but won't link until those
  land. Rejected at type-check: `ptr + ptr`, `int - ptr` (which
  catches `0 - p`), `ptr ± floating`, `ptr - ptr` with mismatched
  types, and any additive op on a function pointer.
- Pointer ordering comparisons (`<`/`>`/`<=`/`>=`) per C99 §6.5.8:
  both operands must be pointers to compatible object types
  (matching pointer types in c6502); result is Int. `tac_to_asm`
  dispatches Pointer-typed operands to an unsigned-ordering
  lowering (per-byte SBC with carry threading; BCC for `<` and
  BCS for `>=`; `>` / `<=` swap operands), so addresses above
  $8000 rank correctly. Rejected at type-check: pointer vs.
  integer (no null-pointer-constant exception on the relational
  ops, unlike equality), pointer vs. floating, mismatched pointer
  types.
- Block-scope arrays with constant integer sizes: `int a[10]`,
  `long a[5]`, `int *a[10]`, `int a[3][4]` (multi-dim composes
  outer-first as `Array(Array(elem, inner), outer)`). Frame
  allocation reserves `sizeof(elem) × count` contiguous bytes via
  `replace_pseudoregisters._size_of_name`'s recursive `_sizeof`.
  Subscript `a[i]` and `p[i]` lower to address-arithmetic + Load
  on the rvalue side and address-arithmetic + Store on the lvalue
  side, sharing the pointer-arithmetic infrastructure (so a `long
  a[5]; a[3]` emits `JSR mul16` for the by-2 scale exactly like
  `long *p; p[3]` does). Array-to-pointer decay (C99 §6.3.2.1.3)
  reifies as an implicit `AddressOf(arr_var)` wrapper stamped with
  `Pointer(elem)` — narrower than the strict standard's
  `Pointer(Array(elem, N))`, but matches the runtime address. Decay
  fires in seven contexts: Subscript array operand, Binary operand,
  Conditional branch, Cast inner, Assignment rval, FunctionCall
  arg, Return value, var initializer. User-written `&arr` for an
  array yields `Pointer(Array(elem, N))` per §6.5.3.2.3; this works
  through the rest of the pipeline because Pointer collapses to Long
  in TAC and `_pointee_size` recurses into Array. Casts targeting
  pointer-to-array (`(int (*)[3])`) compose through the abstract
  declarator and are accepted; the parser still rejects casts whose
  composed top-level type is `Array(...)` itself. Rejected: array
  assignment (`a = b`), `extern` arrays (would need cross-TU init
  deferral). Pre-increment / compound assignment on subscripts
  (`++a[i]`, `a[i] += 1`) work via the parser's desugaring to
  `a[i] = a[i] + 1`; postfix on a subscript (`a[i]++`) isn't
  wired through.
- File-scope and block-scope `static` arrays with constant
  initializer lists: `int a[3] = {1,2,3};` at file scope, or
  `static int nested[3][2] = {{1,2},{3,4},{5,6}};` inside a
  function. The type checker validates the InitList via the same
  `_check_array_init_list` path as automatic-storage arrays, then
  builds a value tree (a tuple of element values; nested tuples
  for multi-dim) and stashes it on `Initial.value`. `c99_to_tac`
  flattens the tree row-major into a list of typed `static_init`
  items (`StaticVariable.init` is `static_init*`), then coalesces
  any run of zero-valued items into a single `ZeroInit(bytes)` —
  so `int a[5] = {1};` lays down as `IntInit(1) + ZeroInit(4)`,
  and `long a[3][2] = {{100}, {200, 300}};` lays down as
  `LongInit(100) + ZeroInit(2) + LongInit(200) + LongInit(300) +
  ZeroInit(4)`. `tac_to_asm` rewraps each item; `asm_emit`
  renders typed inits as `dc.b` / `dc.w` / `dc.l` and ZeroInits
  as `ds.b N` (dasm reserves N zero-initialized bytes). The
  coalescing is value-driven, so an explicit `{1, 0, 0, 0, 0}`
  folds the same as `{1}`. AddressInit (`&otherstatic`) never
  folds — its byte pattern is symbolic, resolved at link time.
  Missing trailing entries zero-pad per C99 §6.7.8.21; a no-init
  `static int a[N];` zero-fills via the same machinery
  (§6.7.8.10).
- Array initializer lists per C99 §6.7.8: `int a[3] = {1, 2, 3};`
  parses as `var_decl` with `init = InitList(items=[...])`. The
  type checker validates the count (≤ array size, with shorter
  lists allowed and the rest zero-padded at lowering time per
  §6.7.8.21), and converts each item to the element type via the
  same `_convert_to` rule as scalar init / Assignment rval — so
  `long a[2] = {1, 2};` wraps each `Int` literal in
  `Cast(Long, ...)`. Lowering in `c99_to_tac._emit_init_stores`
  emits a single `GetAddress` for the array's base, then walks the
  initializer tree recursively, accumulating constant byte offsets
  to each scalar leaf and emitting `Store(val, base + offset)` for
  it (the Add is skipped when offset==0). For multi-dim arrays
  (`int a[2][3] = {{1,2,3},{4,5,6}};`) each top-level item is
  itself an InitList; the recursion threads the byte offset
  through, so `a[1][0]` lands at `base + 1*sizeof(int[3]) +
  0*sizeof(int) = base + 3`. Missing items zero-pad at any depth:
  a missing inner sub-array is treated as an empty InitList so
  every leaf zeroes. Trailing commas (`{1, 2, 3,}`) parse per the
  standard. Rejected: scalar init for an array (`int a[3] = 5;`),
  brace-enclosed init for a scalar (`int x = {1, 2};`), too many
  initializers at any nesting level, and the C99 "subaggregate"
  flat form for multi-dim (`int a[2][3] = {1,2,3,4,5,6};` — would
  need a parsing-time pre-grouping pass we don't have).
- Multi-dim subscript (`a[i][j]` for a multi-dim or pointer-to-
  array operand): the type checker's existing decay logic produces
  the AST `Subscript(AddressOf(Subscript(AddressOf(Var(a)), i)), j)`
  — the inner `a[i]` yields `Array(elem_inner, M)` which decays
  to `Pointer(elem_inner)` for the outer subscript's pointer-
  arithmetic path. `c99_to_tac` handles the `AddressOf(Subscript)`
  shape via a dedicated case in `translate_exp`'s AddressOf branch:
  it dispatches to `_translate_subscript_address` (the rvalue
  Subscript path without the trailing Load), so the outer
  subscript's address computation chains naturally through the
  inner's. User-written `&a[i]` lowers through the same case —
  identifier_resolution accepts Subscript as a third syntactic
  lvalue (alongside Var and Dereference), and `&a[i]` is byte-
  identical to `a + i` per C99 §6.5.3.2.3 (no Load, just the
  scaled Add).
- Array parameters with the C99 §6.7.5.3.7 adjustment: a parameter
  declared as `T param[N]` (or `T param[]`) is adjusted at parse
  time to `T *param`, via `_adjust_param_type` in
  `parameter_declaration`. Only the OUTERMOST array suffix decays
  — `int foo(int a[3][4])` becomes `int foo(int (*a)[4])`, carrying
  type `Pointer(Array(Int, 4))`. The adjustment is single-source-of-
  truth: the function's FunType, the parameter's symbol-table
  entry, and every Var reference to the parameter all see the
  pointer type uniformly. Forward-declaration with
  `int foo(int a[3]);` is compatible with a definition
  `int foo(int *a) { ... }` because the type checker compares the
  post-adjustment FunTypes. At a call site, `foo(arr)` triggers
  the regular array-to-pointer decay on the argument so its type
  matches the parameter's adjusted type.

Partially supported: unsigned types (`unsigned int`, `unsigned
long`) parse and propagate through every pass — values lay down
correctly, mixed-type arithmetic promotes per C99 §6.3.1.8, and
explicit casts lower to `SignExtend` / `ZeroExtend` / `Truncate` /
no-op as appropriate. What still acts signed: `<` / `>` / `<=` /
`>=` use the V-corrected `SBC` + `BMI`/`BPL` sequence regardless
of operand signedness, and `>>` always emits `JSR asr8` / `JSR
asr16` (signed arithmetic right shift) — neither has the unsigned
variant (`BCC`/`BCS` ordering, `JSR lsr8` / `JSR lsr16` for
unsigned right shift) wired up yet.

Floating types (`float`, `double`) parse, type-check, and lower
through to byte-correct IEEE 754 static initialisers, block-scope
load sequences, and runtime conversions between FP and integer
types (the six TAC nodes IntToFloat / IntToDouble / FloatToInt /
DoubleToInt / FloatToDouble / DoubleToFloat all lower into HARGS-
marshaled helper Calls). What's still missing: arithmetic / unary
ops on FP operands (`+` / `-` / `*` / `/` / `<` / `>` / `==` /
…) raise `NotImplementedError` at TAC translation time pending
the FP arithmetic runtime helpers (`fadd` / `fsub` / `fmul` /
`fdiv` / `dadd` / `dsub` / `dmul` / `ddiv`), and the conversion
helpers themselves (`i2f` / `u2f` / `l2f` / `ul2f` and their
d-variants, `f2i` / `f2u` / `f2l` / `f2ul` and their d-variants,
plus `f2d` / `d2f`) aren't in this repo yet either — `tac_to_asm`
emits the Calls in advance of the runtime header landing.

Not yet in the pipeline at all: the runtime header that defines
`SSP` / `FP` / `HARGS` / `DPTR`, initializes `SSP`, sets the
reset vector, and provides the runtime helpers `mul8` /
`divmod8` / `asl8` / `asr8` / `mul16` / `divmod16` / `asl16` /
`asr16` plus the 18 FP-conversion helpers (`i2f`/`u2f`/`l2f`/
`ul2f`, `i2d`/`u2d`/`l2d`/`ul2d`, `f2i`/`f2u`/`f2l`/`f2ul`, `d2i`/
`d2u`/`d2l`/`d2ul`, `f2d`, `d2f`), the FP arithmetic helpers
above, and the `icall` trampoline (`JMP (DPTR)`) used by
`IndirectCall`. `tac_to_asm` already emits Calls to all of
these, so a program that uses `*` / `/` / `%` / `<<` / `>>`,
any FP↔int or Float↔Double cast, or any indirect call (`fp()`
where fp is a function pointer) assembles but won't link until
those helpers exist.
