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
uv run python compile.py - --codegen --optimize < src.c    # with TAC-level optimizer
uv run python compile.py - --codegen --optimize-asm < src.c  # alt: asm-level optimizer
```

`compile.py` is the only CLI; every other module is library-only. Flags it doesn't
recognize are forwarded to the preprocessor (pcpp), so `-D`, `-U`, `-I`,
`--passthru-*`, `--line-directive` etc. work the same as the `pcpp` CLI. pcpp's
own `-o` is not forwarded.

Stage-selection flags (mutually exclusive, one required with `compile.py`):
`--lex`, `--parse`, `--resolve`, `--tac`, `--codegen`. `--resolve` runs
the three name-resolution passes (identifier resolution, label resolution,
loop labeling) in that order.

Modifier flags (orthogonal to the stage flags; applies to `--tac` and
`--codegen`; mutually exclusive with each other):
- `--optimize` runs the SSA-bracketed optimizer pipeline (constant
  folding, unreachable-code elimination, copy propagation, dead-store
  elimination) plus TAC-level register allocation that maps promotable
  SSA values to zero-page slots.
- `--optimize-asm` runs the alternate pipeline: TAC fixed-point opts
  (no TAC regalloc) → asm-level SSA round-trip with forward + backward
  copy propagation + byte-granular DCE + regalloc → late prologue
  synthesis. Also enables the `__attribute__((zp_abi))` calling-
  convention optimization (frame elimination on annotated leaves).

See the "Optimization pipeline" / "Frame elimination via __attribute__"
sections below and `docs/optimization.md` / `docs/leaf_zp_abi.md`
for the full walk-throughs.

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

When `--optimize` is on, `compile.py` inserts `optimize_program`
(in `passes/optimization/optimizer.py`) between c99_to_tac and
tac_to_asm. The optimizer takes `(prog, symbols)` and returns
`(optimized_prog, colorings)` where `colorings` is a
`dict[func_name, Coloring]` consumed downstream by
`replace_pseudoregisters`. Per-function shape:

```
fn → to_ssa → [constant_fold → UCE → copy_propagate → DSE]* →
              build liveness → build interference → color_graph →
              from_ssa → fn'
```

`docs/optimization.md` is the from-scratch tour. The brief version:

1. **`to_ssa`** (`passes/optimization/ssa_construction.py`) renames
   promotable Vars (LocalAttr + scalar + non-address-taken) to
   `<orig>.<N>` so each name has exactly one definition. Inserts
   `Phi(dst, args=[(pred_label, source), ...])` nodes at iterated
   dominance frontiers (Cytron 1991), pruned by liveness — only
   blocks where the var is live-in get a Phi. Address-taken
   locals, statics, aggregates, and function names pass through
   unchanged. SSA-minted labels (preheader / block labels) are
   scoped per-function: `.<funcname>@ssa_block@N` and
   `.<funcname>@ssa_preheader@N`, so two functions in the same
   program don't collide in the asm namespace.

2. **Fixed-point loop** rotates four passes until no pass makes a
   structural change (`fn == prev`). Each pass enables the others:
   - `constant_fold` (`constant_folding.py`): folds Unary, Binary,
     comparison, integer-width / FP conversion casts, conditional
     jumps with constant conditions, and Phis whose every arg
     agrees. Width-correct: arithmetic wraps at the operand's
     declared width (Int 16-bit, Long 32-bit, LongLong 64-bit;
     unsigned variants the same widths). Wraparound matches what
     the 6502 lowering computes at runtime.
   - `eliminate_unreachable_code` (`unreachable_code_elimination.py`):
     drops unreachable blocks (forward DFS from ENTRY); prunes Phi
     args whose `pred_label` is no longer an actual predecessor
     (catches the case where constant folding dropped a conditional
     jump); folds singleton Phis to Copies; drops useless jumps and
     useless labels.
   - `copy_propagate` (`copy_propagation.py`): SSA-aware. For
     `Copy(src, dst)`, replaces every use of `dst` with `src`
     (chains too). Sound only in SSA where each name has one def.
   - `eliminate_dead_stores` (`dead_store_elimination.py`):
     SSA-aware. Drops pure defs whose dst doesn't appear as a use
     anywhere. `FunctionCall` / `IndirectCall` keep the call (side
     effects) but drop the dst when unused.

3. **Register allocation** runs while the function is in SSA form
   (the chordal property is what makes greedy coloring optimal):
   - `liveness.py` computes per-block live-in/live-out + lazy
     per-instruction queries. Phi sources contribute to the
     matching predecessor's live_out (the future de-SSA Copy reads
     them there); Phi dsts contribute to every predecessor's
     live_out (the future de-SSA Copy writes them, so they need
     to interfere with anything else live across the predecessor).
   - `interference.py` builds the chordal interference graph: each
     node carries a width (1/2/4/8 bytes from the symbol table)
     and a `lives_across_call` bit (true iff live at any
     `FunctionCall` / `IndirectCall`). Statics, function names,
     and address-taken locals are filtered out — regalloc only
     colors LocalAttr scalars.
   - `pool.py` carries the caller/callee-saved partition of the
     ZP register pool. Default `Pool(start=0x80)` splits
     `[0x80, 0xFF]` into caller-saved `[0x80, 0xC0)` and
     callee-saved `[0xC0, 0x100)` (64 bytes each). The starting
     address must be even and is configurable.
   - `register_allocation.py` colors via greedy width-aware
     allocation in dom-tree-PEO. Cross-call values prefer
     callee-saved (the function's prologue/epilogue save+restore
     them); non-cross-call values prefer caller-saved (no
     overhead). Falls back to the other pool when the preferred
     is full; spills to frame when neither fits.

4. **`from_ssa`** (`ssa_destruction.py`) lowers each Phi to one
   Copy per PhiArg in the matching predecessor block, before the
   terminator. Within each predecessor's parallel-copy set, Copies
   are **topologically sorted** so a Copy's dst isn't read by a
   later Copy — fixes the "lost copy" problem that arises when
   copy propagation makes one Phi's source equal to another Phi's
   dst at the same block. Cycles (e.g. `a, b = b, a`) are broken
   with a fresh `<funcname>.cycle_tmp@N` Var, registered in the
   symbol table with the cycle members' type. The temp gets a
   frame slot via `replace_pseudoregisters` later (regalloc
   already ran).

5. After `from_ssa`, the function is regular non-SSA TAC ready for
   `tac_to_asm` and the rest of the pipeline. The Coloring flows
   through `compile.py` into `replace_pseudoregisters`, which
   lowers colored Pseudos to `ZP(addr, offset)` operands and
   reserves frame slots at `FP+1..FP+S` for callee-saved bytes
   the function uses (saved by the prologue, restored by the
   epilogue).

`StaticVariable` top-levels pass through the optimizer unchanged
(their `init` is constant byte layout, not control flow).
`optimize_function` without `symbols` (legacy unit-test path)
skips both SSA and regalloc and returns `(fn, None)`.

The MVP doesn't yet do move coalescing (Phi src/dst sharing a
color is left to chance + the self-Mov peephole), and a few opts
remain future work — see the "What's not done yet" section at the
end of `docs/optimization.md`.

`compile.py` has a sibling flag `--optimize-asm` that selects an
**alternate optimizer pipeline** with byte-granular SSA / DCE /
regalloc operating on the asm IR, plus forward + backward copy
propagation in the SSA bracket. Same end-to-end correctness on
the chapter sim corpus; produces equivalent ZP usage to
`--optimize` plus a few byte-DCE wins (dead high-byte stores from
cast-then-truncate patterns drop) and the round-trip eliminations
that backward copy-prop catches (e.g. Long return values that
otherwise route through a ZP slot before being deposited in
HARGS). The asm-SSA bracket runs
`[copy_propagate → backward_copy_propagate → byte_dce]*` to a
fixed point. The two flags are mutually exclusive.
`--optimize-asm` also enables the `__attribute__((zp_abi))`
calling-convention optimization (see below).

## Frame elimination via `__attribute__((zp_abi))`

A function declared `__attribute__((zp_abi))` participates in a
**per-function ZP-passing calling convention** instead of the
soft-stack convention. Caller writes argument bytes directly to
fixed zero-page addresses (no `AllocateStack`); callee reads
params from those addresses (no `Frame(M+3+...)` reads). When
the callee additionally has no Frame-resident locals (asm-level
regalloc fit everything in ZP) and no callee-saved bytes
(leaves don't need any), the prologue / epilogue collapse to
nothing — the function emits as pure body + bare `RTS`.

Active under `--optimize-asm` only (the soft-stack convention
is unchanged in `--optimize` and unannotated `--optimize-asm`).
Selection is **manual**: a function uses ZP-passing if and only
if its declaration carries `__attribute__((zp_abi))`. Without
the annotation, soft-stack as today.

Validation (compile-time, error otherwise):
- The body must contain no `FunctionCall` / `IndirectCall` —
  leaf only. Recursion / mutual recursion would clobber the
  param ZP slots; indirect calls can't know the callee's ABI.
- The function's address must not be taken anywhere in the
  program. (Otherwise an indirect call site would assume the
  default soft-stack ABI.)
- The total parameter byte count must fit in the available ZP
  window (default 64 bytes, $80–$BF).

`passes/abi_selection.py` (`select_abi`) computes the per-
function `ParamLayout = SoftStackLayout | ZpLayout(addrs)` dict
and is threaded through:
- `tac_to_asm` (call-site lowering: ZP-ABI callees get
  `Mov(arg, ZP(addr))` writes with parallel-copy ordering, no
  `AllocateStack`).
- `replace_pseudoregisters_bare_exit` (callee-side: ZP-ABI
  param Pseudos resolve to `ZP(layout.addrs[k], 0)`;
  `arg_bytes` stays at 0).
- `passes/optimization_asm/optimizer.py`'s
  `_blocked_addrs_for`, which tells the body's regalloc to
  avoid (a) the function's own param addresses if it's ZP-ABI
  and (b) every ZP-ABI callee's param addresses — so locals
  can't collide with incoming or outgoing param bytes.

The annotation is parsed by the C99 grammar's `attribute_clause`
rule (just before declaration specifiers) and stored on
`Type_function_decl.abi_annotation` / `Type_var_decl.abi_annotation`
(the latter only ever None — annotations on object decls / tag-
only decls are rejected at parse time). Unknown attribute names
(anything other than `zp_abi` today) are also rejected at parse
time. Cross-TU correctness rides on header propagation: every
TU including the header sees the annotation and uses the
matching ABI.

Full design + build plan in `docs/leaf_zp_abi.md`.

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
- All nine integer types (`int`, `long`, `long long`,
  `unsigned int`, `unsigned long`, `unsigned long long`,
  `char`, `signed char`, `unsigned char`) and explicit casts
  among them parse, type-check, and lower all the way through
  to 6502 asm. 16- and 32-bit arithmetic (`+`, `-`,
  bitwise ops, comparisons, equality, sign-extension on widening
  signed casts, zero-extension on widening unsigned casts,
  truncation on narrowing casts) is expanded into byte-level
  sequences by `tac_to_asm` — each operand's type drives the
  dispatch via the symbol table. 2-byte-typed locals / params /
  temporaries occupy 2 contiguous frame bytes, 4-byte-typed ones
  occupy 4. Only 1-byte returns ride in a register (A); every wider
  return type lives in a fixed `HARGS` zero-page slot. Long / ULong
  / Pointer (2B) → `HARGS+0..1`, LongLong / Float (4B) →
  `HARGS+8..11`, Double (8B) → `HARGS+16..23`. The 4B and 8B
  offsets match the FP arithmetic helpers' output slots so a
  function ending in `return a OP b;` for FP operands needs no
  epilogue copy. LongLong shares the Float slot because types are
  exclusive per call and `mul32` / `divmod32` write their 4-byte
  results to that offset. The 2B slot at `HARGS+0..1` is the same
  byte range `mul8` / `divmod8` use for inputs — also fine, since
  return-setup happens after any helper call has consumed its
  inputs. All HARGS-resident returns flip `Ret(save_a=False)` so
  the epilogue skips the PHA/PLA pair — the SSP/FP arithmetic
  doesn't touch
  HARGS. See README's "Return-value convention" subsection.
  Mixed-type arithmetic goes through C99 §6.3.1.8's usual
  arithmetic conversions in `passes.type_checking` — `long +
  unsigned int` promotes the unsigned int to long via a
  `ZeroExtend`, `long long + unsigned long` promotes the
  unsigned long to long long the same way, `int + unsigned int`
  promotes the int to unsigned int via a same-width no-op cast,
  etc. Comparison ops still lower as if the operands are signed
  (`SBC` + V-correction); unsigned ordering would need a separate
  inline lowering that uses `BCC`/`BCS` instead of the V-corrected
  `BMI`/`BPL`. `*`, `/`, `%`, `<<`, `>>` on multi-byte operands
  lower to calls to `mul16` / `divmod16` / `asl16` / `asr16`
  (2-byte) or `mul32` / `divmod32` / `asl32` / `asr32` (4-byte)
  against HARGS — the marshaling is in place but the helpers
  themselves aren't in this repo yet, so a Long or LongLong
  arithmetic program will assemble but won't link until the
  runtime header lands.
- `int main(void)` returning a single integer expression
- integer constants of all six flavors per C99 §6.4.4.1
  paragraph 5: lex-time split into `INTEGER_CONSTANT` /
  `LONG_INTEGER` / `UINT_INTEGER` / `ULONG_INTEGER` by suffix
  (`LL`/`ll` shares the LONG terminal, `ULL` the ULONG terminal —
  `has_ll` then routes them into the long-long candidate rows),
  parser dispatches each (token-kind, base, value, has_ll) tuple
  to the first variant in C99's type list whose range fits the
  value (so e.g. `0x80` lex'd as INTEGER_CONSTANT becomes
  `ConstUInt(128)` because `int`'s range stops at 127 and the
  hex/octal type list passes through `unsigned int` next;
  `0x8000L` becomes `ConstULong(32768)` because the hex/octal `L`
  list goes `long → unsigned long → long long → unsigned long
  long` and ULong is the first that fits; `100000` becomes
  `ConstLongLong(100000)` because the unsuffixed-decimal list
  goes `int → long → long long`). Literals exceeding the widest
  type's range (`unsigned long long`, 2^32 - 1) are rejected.
- character constants (`'a'`, `'\n'`, `'\x41'`, `'\101'`) per
  C99 §6.4.4.4: lex-time as `CHAR_CONSTANT`, parser decodes
  the body via `_decode_escapes` (handles every simple escape
  in C99 plus `\xHH` and octal `\NNN` escapes; rejects multi-
  character constants and `\u`/`\U` Unicode escapes), and
  emits a `Constant(ConstInt(value))` carrying the byte
  value. (The const variant is `ConstInt` rather than
  `ConstChar` per C99 §6.4.4.4.10's "An integer character
  constant has type int".)
- string literals (`"abc"`, `"a\nb"`) per C99 §6.4.5: lex-
  time as `STRING_LITERAL+` (the grammar accepts adjacent
  literals), the parser concatenates per §6.4.5.5 ("In
  translation phase 6, the multibyte character sequences
  specified by any sequence of adjacent character and
  identically-prefixed string literal tokens are concatenated
  into a single multibyte character sequence"), and emits a
  single `String(str=joined_bytes)` AST node. The
  `passes.string_lifting` pass then hoists every String that
  ISN'T a direct char-array initializer into a fresh file-
  scope `static char[N+1] .str@<N>` declaration, replacing
  the original String node with a `Var(.str@<N>)`. After
  lifting, every other use of a string literal — `&"abc"`,
  `"abc"[1]`, `char *p = "abc"`, `return "abc";` — works
  through the same mechanisms as any other file-scope char
  array. `char arr[N] = "abc";` keeps its String inline; the
  type checker validates `len(s) <= N` per §6.7.8.14 (with
  the null terminator elided when `N == len(s)`); the static
  init lays down `s[0..len-1]` followed by zero-pad to N
  (block-scope auto-storage uses per-byte Stores, file-scope
  static lays the bytes down as `IntInit` items in the
  initializer list).
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
  `<<=`, `>>=` (each builds a `CompoundAssignment` AST node;
  `c99_to_tac._translate_compound_assign` evaluates the lval's address
  ONCE — same machinery as Prefix / Postfix — then emits Load + cast-to-
  intermediate-type + Binary + cast-back-to-lval-type + Store, reusing
  the underlying binop's TAC / asm lowering. Var lvals skip the Load /
  Store and read/write the var directly. The `intermediate_type` is the
  binop's working type — common-of-promoted for arithmetic / bitwise,
  promoted-left-only for shifts (§6.5.7.3); pointer arithmetic
  `ptr += int` routes through `translate_pointer_arithmetic` for
  sizeof-pointee scaling)
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
  deferral). Pre/postfix increment and compound assignment on
  subscripts (`++a[i]`, `a[i]++`, `a[i] += 1`, `arr[i++] *= 3`)
  all evaluate the subscript's address exactly once — the Prefix
  / Postfix / CompoundAssignment AST nodes route through
  `_translate_subscript_address` for the address and then Load +
  Binary + Store, so any side effects in the index subexpression
  fire only once.
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
- Struct and union types per C99 §6.7.2.1 / §6.5.2.3, including:
  declarations with bodies (`struct foo { int a; long b; };`) at
  file or block scope, forward declarations (`struct foo;`),
  member access via `.` and `->` (`s.m`, `p->m`, chained nested
  forms like `s_ptr->in_array->a`), compound initializers
  (`struct s x = {1, 2, {3, 4, 5}};`), struct = struct copy via
  `Copy(src, dst)` byte-fan-out (`s1 = s2`, `s1 = other.member`,
  `s_ptr->m = small`, etc.), pointer-to-struct
  (`struct s *p = &x;`), address-of struct member
  (`int *q = &x.field;`), `sizeof(struct s)` /
  `sizeof(union u)`, and unions (member access, copy, address-of).
  Layout follows c6502's "no padding, byte-aligned" rule:
  `_compute_layout` walks members in source order, accumulating
  `byte_offset = running_sum` for structs and pinning every
  member at offset 0 for unions; total size is the sum of member
  sizes (struct) or the max (union). The c99 data_type variants
  `Structure(tag)` and `Union(tag)` carry only the tag — full
  layout (members, offsets, total size) lives in a separate
  `TypeTable` produced by the type checker, parallel to the
  `SymbolTable`. Both ride through `c99_to_tac`,
  `tac_to_asm`, and `replace_pseudoregisters` — each consults
  the TypeTable via the extended `_sizeof` / `_size_of_name` /
  `_size_of` helpers when sizing struct-typed Vars, frame slots,
  and TAC operands. Tag visibility is per-block: a stack of
  visible-tag sets pushes on every Compound block, for-header,
  and function body, popping on exit. A `struct foo` reference
  with no prior declaration auto-introduces a forward declaration
  in the current scope (C99's "appearance of `struct foo` in any
  declaration introduces it" rule) — so
  `struct outer { struct inner *p; };` works without an explicit
  forward decl of `inner`. Member access lowers in `c99_to_tac`
  to `GetAddress` (or load through pointer) + optional
  `Binary(Add, ConstLong(member.offset))` + `Load` (rvalue) /
  `Store` (lvalue). Compound initializers walk the layout
  recursively, emitting one `Store` per scalar leaf at
  `base + member_offset`. Static-storage struct initializers
  flatten to a typed-init list in member-declaration order via
  `_flat_static_init_raw`'s Structure/Union arms; union statics
  pad out to the full union size after the first-member's bytes
  via a trailing `ZeroInit`. Struct-typed assignment / Copy
  works at any width through TAC's existing N-byte fan-out
  (`_translate_copy` reads `_size_of(dst)`).

  **Struct-by-value parameter passing** uses the existing soft-
  stack arg block: a struct param contributes `sizeof(struct)`
  bytes to the caller's arg-byte count. The caller writes each
  byte into `Stack(1)..Stack(N)`; the callee reads them via
  `Frame(M+3+offset)`. No new mechanism — the existing per-byte
  `Mov` emission with `_byte_at` and `_size_of` handles
  arbitrary-width vals uniformly.

  **Struct-by-value returns** use an sret-style convention.
  `c99_to_tac._translate_function` detects a Structure/Union
  return type and prepends a hidden first parameter
  `.sret.<funcname>` of type `Pointer(struct)` to the TAC
  function's params; the c99 symbol table gets a matching
  `LocalAttr` entry. `Return(e)` for a struct-returning function
  lowers to `Store(e, .sret.<funcname>) + Ret(None)` — the
  callee writes the return bytes through the caller's pointer
  and produces no scalar result. At call sites
  (`c99_to_tac.translate_exp`'s FunctionCall arm), if the callee
  returns Structure/Union the caller mints a fresh struct-typed
  local for the return slot, emits `GetAddress(slot, addr)`,
  prepends `addr` as the first arg, and the FunctionCall TAC
  instruction has `dst=None`. The "result" of the
  FunctionCall expression is the slot Var itself, which
  downstream consumers (Assignment Copy, Dot/Arrow chained
  member access, `f().m`) treat as any other addressable
  struct lvalue. `_translate_lvalue_address` falls back for
  rvalue struct expressions (FunctionCall, Conditional) by
  translating them to a Var (the slot) and `GetAddress`-ing
  that — uniform with the canonical Var case. The structural
  lvalue check (`_is_lvalue`) in identifier_resolution treats
  `Dot.operand` as an lvalue iff the operand is — so
  `(c?a:b).m = …` and `f().m = …` are rejected (the slot has
  temporary lifetime; you can read it but not assign through
  it).

  **Block-scope tag shadowing** is supported via per-scope tag
  renaming in `passes.identifier_resolution`. File-scope tags
  keep their source name; block-scope tag declarations get a
  fresh `@<N>.<source>` rename, recorded in a `_TagScope` that
  clones-and-flips on every Compound block / for-header /
  function body (parallel to the variable scope). Every
  Structure/Union AST node's tag is rewritten to its scope-
  resolved name, so the type checker's flat TypeTable keys on
  globally-unique names — two different block-scope `struct s`
  declarations end up as distinct `@N.s` and `@M.s` entries.
  Auto-introduction (per C99 §6.7.2.3 paragraph 5: "appearance
  of a struct/union specifier in a declarator introduces the
  tag with incomplete type into the current scope") happens
  inside `_resolve_type` — a Structure/Union reference whose
  tag isn't in any visible scope mints a fresh resolved name in
  the current scope. The type checker's error messages strip
  the `@<N>.` prefix so users see their source-level tag
  spelling.

  See `tests/STATUS.md` for the chapter\_18 file-by-file status.

Unsigned types (`unsigned int`, `unsigned long`, `unsigned long
long`) parse, type-check, and lower end-to-end. Values lay down
correctly, mixed-type arithmetic promotes per C99 §6.3.1.8,
explicit casts lower through `SignExtend` / `ZeroExtend` /
`Truncate` (or are elided for same-width casts), `<` / `>` /
`<=` / `>=` dispatch to the unsigned BCC/BCS-based per-byte SBC
sequence (no V-correction), and `>>` dispatches to `lsr8` /
`lsr16` / `lsr32` (logical right shift, zero-fill). Signedness
rides on the operand: the const variant for Constants
(Const{UInt,ULong,ULongLong} → unsigned), the symbol-table c99
type for Vars. The `lsr*` helpers themselves aren't in the repo
yet — see the runtime-header status note below; the lowerings
emit calls to them in advance of the runtime header landing,
same status as `mul*` / `divmod*` / `asl*` / `asr*`.

Long-long types (`long long`, `unsigned long long`) parse and
propagate through every pass — `LongLong` / `ULongLong` are
4-byte signed / unsigned integers (-2^31..2^31-1 / 0..2^32-1).
Static initialisers lay down 4 little-endian bytes via dasm's
`DC.L`. Locals / params / temporaries get 4 contiguous frame
bytes. Add / Sub / And / Or / Xor / equality / ordering /
sign-or-zero-extend / truncate / cond-jump / negate / complement
all fan out to per-byte sequences via the existing size-
parameterized loops with carry / borrow threading where
appropriate. Multiply / divide / modulo / shift dispatch to the
`mul32` / `divmod32` / `asl32` / `asr32` runtime helpers (not
in this repo yet — see status note below). LongLong return
values use the same convention as Float: caller and callee both
exchange the 4 bytes through `HARGS+8..11`, and `Ret` flips
`save_a=False` so the epilogue skips the PHA/PLA pair (HARGS
isn't touched by SSP/FP arithmetic). Conversions between
LongLong / ULongLong and Float / Double dispatch to `ll2f` /
`ull2f` / `ll2d` / `ull2d` / `f2ll` / `f2ull` / `d2ll` /
`d2ull` (also pending the runtime header).

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
`divmod8` / `asl8` / `asr8` / `lsr8` / `mul16` / `divmod16` /
`asl16` / `asr16` / `lsr16` / `mul32` / `divmod32` / `asl32` /
`asr32` / `lsr32` plus the 26
FP-conversion helpers (`i2f`/`u2f`/`l2f`/`ul2f`/`ll2f`/`ull2f`,
`i2d`/`u2d`/`l2d`/`ul2d`/`ll2d`/`ull2d`, `f2i`/`f2u`/`f2l`/`f2ul`/
`f2ll`/`f2ull`, `d2i`/`d2u`/`d2l`/`d2ul`/`d2ll`/`d2ull`, `f2d`,
`d2f`), the FP arithmetic helpers above, and the `icall`
trampoline (`JMP (DPTR)`) used by `IndirectCall`. `tac_to_asm`
already emits Calls to all of these, so a program that uses
`*` / `/` / `%` / `<<` / `>>`, any FP↔int or Float↔Double cast,
or any indirect call (`fp()` where fp is a function pointer)
assembles but won't link until those helpers exist.

`--optimize` runs end-to-end (see "Optimization pipeline"
section above): SSA construction → fixed-point loop (constant
folding, UCE, copy prop, DSE) → register allocation onto ZP
(default `$80-$FF`, configurable via `Pool(start=...)`, split
caller/callee at `$C0`) → SSA destruction with topologically
sorted parallel copies (cycles broken with a fresh temp).
Promotable SSA values lower to `ZP(addr, offset)` operands —
direct zero-page addressing, ~3× faster per access than the
unoptimized indirect-Y `(FP),Y` path. Spilled / address-taken /
parameter Pseudos continue to use Frame slots. Cross-call live
values get callee-saved colors and the function's
prologue/epilogue save+restore them around the body. Verified
end-to-end via the asm sim: `tests/test_sim_asm_optimized.py`
runs the chapter_1..12 corpus through `--optimize` and asserts
program return values match the unoptimized expectations. See
`docs/optimization.md` for the full walk-through.

`--optimize-asm` runs end-to-end as the alternate pipeline:
TAC fixed-point opts (no TAC regalloc), then asm-level SSA
round-trip (`passes/optimization_asm/`) with `[copy_propagate →
backward_copy_propagate → byte_dce]*` (fixed point) + byte-
granular regalloc; final stage is a post-regalloc
`prologue_synthesis` that elides the prologue/epilogue when no
frame is needed. The forward `copy_propagate` is the SSA-aware
asm equivalent of TAC's pass: substitutes uses of `Mov(src,
Pseudo)` with `src` for Imm / ImmLabel / SSA-Pseudo sources.
The backward `backward_copy_propagate` collapses the asm-level
round-trip pattern `Mov(Reg(A), P); ...; Mov(P, Reg(A));
Mov(Reg(A), D)` (where `P` is single-use, `D` is a non-Pseudo
memory destination, the relocation range is free of Calls /
aliasing writes / control flow, and `Reg(A)` + N/Z flags are
dead at the deletion point) into a single `Mov(Reg(A), D)` —
catches return-value temps that get ZP-colored only to be
immediately reloaded into HARGS. Same chapter sim corpus runs
through the alt path via
`tests.test_sim_asm_optimized.TestAsmSimChaptersOptimizeAsm`.

`__attribute__((zp_abi))` on a function declaration enables
**frame elimination via ZP-passing** under `--optimize-asm`
(see "Frame elimination via __attribute__((zp_abi))" section
above). Validated at compile time (function must be a leaf,
not address-taken, params fit the ZP window); rejected with a
clear error otherwise. Caller writes args directly to the
callee's pinned ZP addresses (no AllocateStack); callee's
params live at those ZP addresses (no Frame reads); when the
body also fits entirely in ZP and uses no callee-saved bytes,
the function emits as bare body + RTS — no prologue, no
epilogue. Tests in `tests/test_leaf_zp_abi.py` cover end-to-
end compilation + simulation; design and build plan in
`docs/leaf_zp_abi.md`.
