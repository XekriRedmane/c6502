# passes/CLAUDE.md

Middle-end passes that consume and produce one of the four ASDL-defined
IRs (`c99_ast`, `tac_ast`, `asm_ast`, `asm2_ast`). The root pipeline
overview lives in `/project/c6502/CLAUDE.md`; this file documents the
middle-end passes that sit between the language front end
(`parser.parse`) and the back-end emit step (`asm_emit.emit_program`).

## Subdirectories

- [optimization/CLAUDE.md](optimization/CLAUDE.md) — TAC-level
  fixed-point optimizer (SSA construction / destruction, constant
  folding, copy folding, IndexedStore recognizer, dead-loop
  elimination, …).
- [optimization_asm/CLAUDE.md](optimization_asm/CLAUDE.md) — asm-level
  SSA round-trip with byte-granular regalloc, move coalescing,
  const-static fold.
- [optimization_ast/CLAUDE.md](optimization_ast/CLAUDE.md) — AST-level
  unroller (`#pragma c6502 loop unroll(enable)`).

## Module roster

The peephole / control-flow / allocation passes that sit directly in
this directory:

- `abi_selection.py` — `select_abi`. Decides which functions are
  `__attribute__((zp_abi))` and mints `__zpabi_<fn>_p<k>` slot symbols.
- `and_sign_bit_branch.py` — `apply_and_sign_bit_branch` peephole.
- `asm_aliasing.py` — asm-level aliasing helpers used by dead-store /
  liveness.
- `asm_dead_store.py` — `apply_asm_dead_store` CFG-wide DSE.
- `asm_licm.py` — `apply_licm` for loop-invariant constant stores.
- `asm_liveness.py` — per-block liveness used by several peepholes.
- `asm_to_asm2.py` — `asm_ast` → `asm2_ast` lowering (see below).
- `dead_pha_pla.py` — `apply_dead_pha_pla`. Drops `Push(Reg(A)) /
  body / Pop(Reg(A))` triples when the body preserves A and the
  PLA's N/Z flag effect is dead. Always-on. Headline case: the
  indirect-indexed-store lowering emits a conservative
  save/restore around the idx-into-Y stage, which becomes pure
  overhead once `direct_index_load` fuses the `LDA idx; TAY` into
  a flag-preserving `LDY idx`.
- `branch_invert.py` — `apply_branch_invert` peephole.
- `cmp_sbc_fusion.py` — `apply_cmp_sbc_fusion` peephole.
- `const_arith_fold.py` — `apply_const_arith_fold` peephole.
- `constant_expression.py` — C99 §6.6 evaluator (integer-constant
  folding; validation hook for future enum / array-size / bitfield-
  width consumers).
- `cpx_cpy_peephole.py` — `apply_cpx_cpy_peephole` peephole.
- `dead_a_arith.py` — `apply_dead_a_arith_elimination` peephole.
- `dec_inc_branch_fold.py` — `apply_dec_inc_branch_fold` peephole.
- `dec_peephole.py` — `apply_dec_peephole` peephole.
- `direct_index_load.py` — `apply_direct_index_load` peephole (see
  below).
- `function_local_sizing.py` — `compute_local_bytes`. Counts each
  function's regalloc-colored ZP byte footprint from a preliminary
  optimizer pass.
- `identifier_resolution.py` — pass 2 (see below).
- `inc_peephole.py` — multi-byte INC peephole (see below).
- `memory_value_propagation.py` —
  `apply_memory_value_propagation`. CFG-aware forward dataflow
  that tracks ZP-cell → recomputable-source equivalences and
  rewrites reads at use sites. Subsumes the former
  `indirect_base_prop` (DPTR-stage rewrite, now CFG-wide) and
  overlaps with `apply_remat` for Imm / Data / ImmLabel sources.
- `label_resolution.py` — pass 4 (see below).
- `linker.py` — `compile.py --link` driver; multi-TU re-allocates
  `__zpabi_*` and `__local_*` symbols across per-TU outputs.
- `long_branches.py` — `expand_program`. Rewrites conditional branches
  with out-of-range targets to `Branch(inverted_cond, .skip);
  Jump(target); .skip:`. Iterative.
- `loop_counter_to_x.py` — X-pivot promotion (see "Asm-level
  promotions" below).
- `loop_labeling.py` — pass 5 (see below).
- `mem_const_prop.py` — `apply_mem_const_prop` peephole.
- `prologue_synthesis.py` — late prologue / epilogue synthesis.
- `redundant_load.py` — `apply_redundant_load_elimination`.
- `redundant_load_after_rmw.py` — `apply_redundant_load_after_rmw`.
- `redundant_store.py` — `apply_redundant_store_elimination`.
- `replace_pseudoregisters.py` — pass 9 (see below).
- `round_trip_load.py` — `apply_round_trip_load_drop`.
- `split_mem_to_mem.py` — `apply_split_mem_to_mem`. Lowers `Mov(mem,
  mem)` to `Mov(mem, Reg(A)); Mov(Reg(A), mem)` so every downstream
  peephole sees the LDA + STA pair as separate atoms instead of one
  opaque compound. Volatile mem-to-mem is skipped (the conservative
  is_volatile bit doesn't tell which operand is volatile). Self-Movs
  `Mov(M, M)` are dropped entirely. See "Mem-to-mem splitting" below.
- `self_store_drop.py` — `apply_self_store_drop`.
- `string_lifting.py` — pass 3 (see below).
- `sub1_test_zero_peephole.py` — `apply_sub1_test_zero_peephole`.
- `type_checking.py` — pass 6 (see below).
- `via_a_store_fold.py` — `apply_via_a_store_fold`. Folds
  `Mov(Reg(X), Reg(A)); Mov(Reg(A), Data|ZP)` to `Mov(Reg(X),
  Data|ZP)` (TXA;STA → STX), same shape for Y. Recovers what
  `x_save_slot_load`'s Pass 3 mem-to-mem case used to do directly
  before `split_mem_to_mem` started breaking the mem-to-mem apart.
- `y_peephole.py` — `apply_y_peephole` (LDY collapse, outside
  fixedpoint).
- `zp_link_metadata.py` — emits the `; @zp-link-meta-begin` block at
  the top of `--codegen --optimize` output for the multi-TU linker.
- `zp_local_allocation.py` — `allocate_function_locals`. Hands each
  eligible function a private body-local ZP range disjoint from
  coexisting footprints.
- `zp_slot_allocation.py` — `allocate_zp_slots`. Binds zp_abi slot
  symbols to ZP addresses via call-graph topological order.

## Pass 2: `identifier_resolution.resolve_program`

`c99_ast` → `c99_ast`. Resolves every user-written identifier —
variables and functions both — and tags it with its C99 §6.2.2
**linkage kind**, stored alongside the resolved name in the resolver's
tables. Renaming is gated on linkage:

- **`Linkage.NONE`** (block-scope automatic variables today — every
  `int x;` we accept) → mint a program-unique `@<N>.<orig>` (illegal in
  a C identifier, so it can't collide with user names) and record it in
  the per-block scope.
- **`Linkage.EXTERNAL`** (every function declaration / definition
  today; later: `extern int x;` at file scope) → keep the source
  spelling, because the linker resolves these by name across
  translation units.
- **`Linkage.INTERNAL`** (later: `static int x;` / `static int
  foo(void);` at file scope) → keep the source spelling, because later
  TU-local passes resolve these by name. Not produced today.

The "rename only NONE-linkage names" rule replaces the older "rename
variables, leave functions alone" heuristic — same behavior right now
(every variable is NONE, every function is EXTERNAL), but the linkage-
driven version slots in cleanly when `extern`/`static` land. A
`VarDecl(Type_var_decl(name))` runs through `resolve_var_decl(... ,
linkage=Linkage.NONE)` today, which bumps the unique-name counter,
mints `@<N>.<orig>`, and records `(resolved, inner=True,
linkage=NONE)` in the per-block scope. Declaring the same variable
name twice in the same block raises `IdentifierResolutionError`. A
`FunctionDecl(Type_function_decl(name))` registers the name in a per-
program `_functions: dict[str, Linkage]` (today always
`Linkage.EXTERNAL`) without renaming — multiple declarations of the
same function are legal and idempotent under dict-overwrite semantics.
Top-level `Function(name, body)` definitions are pre-registered in the
same dict before any body is walked, so a `FunctionCall` inside one
function can resolve a target defined later in the file or be self-
recursive. A `Var(name)` in any expression is rewritten to its mapped
resolved name; referencing an undeclared variable raises (a function
name on its own doesn't satisfy a `Var` lookup — c6502 has no
function-pointer expressions yet). A `FunctionCall(name, args)`
validates that `name` is in `_functions` (raises "call to undeclared
function" if not), recursively resolves the args, and leaves `name`
itself unchanged. The same lvalue check that gates `Assignment.lval`
also gates `Postfix.operand` and `Prefix.operand`, so `1++` and `++1`
raise just like `1 = 2`. The accepted lvalue forms are `Var`,
`Dereference`, and `Subscript` (the three syntactic lvalues c6502
supports today); anything else raises "invalid lvalue" — `1+2=3`,
`-a=5`, `(a=b)=c`, `++1` all fail here.

**Parameters** are resolved exactly like NONE-linkage local variables:
`_resolve_params` walks the parameter list, validating uniqueness
within the list and minting a fresh `@<N>.<orig>` for each (the param
scope built up is independent of the surrounding block scope, so `int
a; int foo(int a);` is legal — the param `a` doesn't conflict with the
outer variable `a`). For a `FunctionDecl` (no body), the renamed names
are stored on the returned `Type_function_decl.params` and the param
scope is discarded. For a function *definition* the param scope IS the
body's outermost scope (C99 §6.9.1.7: "the parameters and the local
variables of the function have the same scope"), so the body's block
items resolve directly into it without the usual clone-flip — `int
foo(int a) { int a = 3; ... }` raises duplicate-decl on the body's
`int a`, while a nested `int foo(int a) { { int a = 3; ... } ... }`
legally shadows via the inner Compound's own scope.

Scope is per-block: each `Block` owns a `dict[str, tuple[str, bool,
Linkage]]` mapping each visible user name to `(resolved_name, inner,
linkage)`, where `inner` is True iff the name was declared in *this*
block. Entering a nested block clones the parent's map and flips every
entry's `inner` flag to False — linkage rides along unchanged, since
linkage is fixed at the declaration site. A duplicate-decl error fires
only when an already-inner-scoped entry would be overwritten;
declaring a name that's currently outer-scoped legally shadows it
(overwrite with a fresh entry, mint or reuse the spelling per the new
entry's linkage, flag as inner). Exiting the inner block discards its
dict — Python GC handles this since we cloned the parent's map rather
than aliasing it. While/do-while bodies resolve in the parent scope
(they don't introduce a scope of their own; a Compound body opens its
own scope as usual). The for-header (`for (<init> ...) body`) opens a
fresh scope per C99 §6.8.5.3, so `int a; for (int a = 1; a < 10; a++)
...` shadows the outer `a` for the duration of the loop and the outer
`a` is intact afterward. `switch` doesn't introduce a scope of its own
(a Compound body does, as usual); the controlling expression and
`case` / `default` bodies all resolve in the surrounding scope.
Labels, gotos, break, continue, and `case` / `default` labels
themselves all pass through unchanged — they live in separate
namespaces and are owned by later passes (label_resolution for user
labels; loop_labeling for break / continue / case / default).

## Pass 3: `string_lifting.lift_program`

`c99_ast` → `c99_ast`. Hoists every `String` literal whose context is
NOT a direct char-array initializer (`char arr[N] = "abc";` keeps its
String inline) into a fresh file-scope `static char[N+1]` declaration,
replacing the original `String` with a `Var` referencing the new
declaration. The minted name is `.str@<N>` (leading `.` and `@` keep
it disjoint from any user identifier and from translator-minted
labels). After lifting, every other use of a string literal — `&"abc"`,
`"abc"[1]`, `char *p = "abc"`, `return "abc";` — works through the
same mechanisms as any other file-scope char array (decay to `char *`,
AddressOf-of-array, subscript, ...) without per-pass special cases.
Runs AFTER identifier_resolution so the lifted names use a disjoint
character (`.`) and don't get re-renamed; runs BEFORE
label_resolution / loop_labeling / type_checking so those passes see
the rewritten AST.

## Pass 4: `label_resolution.resolve_program`

`c99_ast` → `c99_ast`. Validates labeled statements (C99 §6.8.1) and
`goto` targets (§6.8.6). Two walks per function: (a) collect every
`LabeledStmt`, minting a unique name `.<funcname>@<orig>` per label
and rejecting duplicates; (b) rewrite the AST, replacing each label
and matching `Goto` target with the unique name and raising
`LabelResolutionError` for any goto whose target wasn't declared in
the same function. Labels are visible across the whole function
(forward gotos are fine), so both walks descend into the bodies of
`if`, compound, while, do-while, and for statements. The leading `.`
makes them dasm-style **local labels**, scoped only to the SUBROUTINE
the asm emits — so two functions can both have a label `foo` without
colliding in the global asm namespace. The `@` separator (illegal in a
C identifier, so it can't appear in `<funcname>` or `<orig>`) keeps
user labels disjoint from translator-minted labels (which all carry
`@<digits>`, e.g. `.if_end@N`, `.loop@N`) and from any user-written
identifier. C99 §6.8.6 also forbids jumping into the scope of a
variably-modified-type identifier; c6502 has no VLAs, so that
constraint is vacuously satisfied.

## Pass 5: `loop_labeling.label_program`

`c99_ast` → `c99_ast`. Mints a unique label per iteration statement
(`.loop@<N>`) and per `switch` statement (`.switch@<N>`), stamping it
onto that statement's `label` field. While walking the body, the pass
threads two pieces of state per C99 §6.8.6:

- `current_loop` — innermost iteration statement's label, used to
  resolve `continue` (§6.8.6.2 — only iteration statements).
- `current_break_target` — innermost iteration *or* switch label, used
  to resolve `break` (§6.8.6.3 — both kinds).

Iteration statements push to both; switch pushes only to
`current_break_target` (so `continue` inside a switch inside a loop
still finds the loop). A third bit of state, `current_switch`, holds
the innermost enclosing switch's case-collector — `case <e>:` and
`default:` nodes encountered during the walk mint their own labels
(`.case@<N>` / `.default@<N>`), stamp them onto the AST node, and
append to that switch's `cases` / `default_label` fields. Case labels
can sit inside if / loop / compound bodies inside a switch (Duff's-
device-style), so iteration / if / compound nodes preserve
`current_switch`; only a nested SwitchStmt swaps in a fresh collector
for its own body. Errors (`LoopLabelingError`): `break;` outside any
iteration / switch; `continue;` outside any iteration; `case` /
`default` outside any switch; duplicate `default:` within one switch
(case-value uniqueness is checked later, in the type-checking pass).
The pass runs *after* label_resolution: loop / switch / case / default
labels are translator-minted, not user-written, so they slot in only
once user-defined goto / labeled-stmt names have already been
resolved. The namespaces are disjoint by construction — a user label
is `.<funcname>@<orig>` where the part after `@` is a C identifier; a
loop / switch / case / default label is
`.{loop,switch,case,default}@<N>` where the part after `@` is digits,
so the two forms can't ever match. Codegen derives concrete control-
flow targets for iteration statements by appending suffixes
(`_start`, `_continue`, `_break`) to the loop's base label; switches
use only the `_break` suffix (the dispatch chain emits the case /
default labels directly).

## Pass 6: `type_checking.check_program`

`(c99_ast, SymbolTable)`. Walks the AST once and produces a
`SymbolTable` (a `dict[str, Symbol]` keyed by resolved identifier
name). The data-type classes (`Int`, `Long`, `LongLong`, `UInt`,
`ULong`, `ULongLong`, `Float`, `Double`, `FunType`) live on `c99_ast`
and are re-exported here under stable `passes.type_checking.<Name>`
names so every consumer agrees on the type vocabulary; equality is
structural via `@dataclass`. Each `Symbol` carries a `type` plus an
`IdAttr` describing its runtime category:

- `LocalAttr` — automatic-storage object (block-scope `int x;` / `long
  x;` / `long long x;` / `unsigned int x;` / `unsigned long x;` /
  `unsigned long long x;` / `float x;` / `double x;`, function
  parameter, or any TAC temporary introduced by `c99_to_tac`).
- `StaticAttr(initial_value, is_global)` — every file-scope object
  plus block-scope `static`. `initial_value` is one of `Initial(c)`,
  `Tentative`, or `NoInitializer` per C99 §6.9.2. `Initial.value` is
  `int` for integer types and `float` for floating types.
- `FunAttr(defined, is_global)` — a function name. `defined` flips
  True the first time a definition is seen.

`is_global` is True iff the symbol has external linkage, materialized
once here so the asm backend doesn't have to re-derive it from the
three-way `Linkage` enum.

The pass mutates each visited expression's `data_type?` field in place
— every `Constant` / `Var` / `Cast` / `Unary` / `Binary` /
`Assignment` / `Postfix` / `Conditional` / `FunctionCall` ends up
tagged with its concrete result type. Constants pick from the const
variant (ConstInt → Int, ConstLong → Long, ConstLongLong → LongLong,
ConstUInt → UInt, ConstULong → ULong, ConstULongLong → ULongLong,
ConstFloat → Float, ConstDouble → Double); Cast picks its target_type;
Var picks the symbol's type; Unary / Postfix inherit the inner
operand's type, except `!` which always yields Int per §6.5.3.3.5.

**Integer promotion** (C99 §6.3.1.1.2) runs FIRST at each operand
position of an arithmetic / bitwise / comparison / shift operator (and
on the operand of unary `-` / `~`). Char-typed operands promote to
`Int` (when Int can represent the source's range) or `UInt`
(otherwise, which in c6502 means UChar — Int -128..127 doesn't cover
UChar 0..255):

* SChar / Char → Int (same range and signedness; same-width no-op Cast
  that c99_to_tac elides at lowering)
* UChar       → UInt

Other integer types already have rank ≥ Int and pass through
unchanged.

**Implicit conversions** apply C99 §6.3.1.8's usual arithmetic
conversions to the post-promotion operand types. Floating types
dominate per §6.3.1.8.1:

* either operand `Double` → result `Double`
* else either operand `Float` → result `Float`
* else both operands integer → integer rules (below)

Integer rules, keyed by C99 §6.3.1.1 conversion rank (`Int` and `UInt`
are rank 1; `Long` and `ULong` are rank 2; `LongLong` and `ULongLong`
are rank 3 — char types are below Int but never participate in the
common-type computation, since integer promotion has already lifted
them to Int / UInt):

* matching types → that type
* both signed (or both unsigned) → the higher-rank type wins (Int+Long
  → Long, Long+LongLong → LongLong, UInt+ULongLong → ULongLong)
* mixed signedness, unsigned has rank ≥ signed → unsigned wins
  (Int+UInt → UInt, Int+ULong → ULong, Long+ULongLong → ULongLong)
* mixed signedness, signed has higher rank and can represent all
  unsigned values → signed wins (Long+UInt → Long; LongLong+UInt →
  LongLong; LongLong+ULong → LongLong, since LongLong's -2^31..2^31-1
  covers ULong's 0..65535)

The narrower or signed-displaceable operand is wrapped in an implicit
`Cast(target=common, exp=…, data_type=common)` via
`_convert_to(exp, target)`, so by the time TAC sees the tree every
operand has its concrete data_type and any size- or signedness-
changing conversion is an explicit Cast node. The same `_convert_to`
helper runs at every place C99 specifies a conversion:

- **Binary** operands (§6.3.1.8): both promoted to the common type
  before the op (except shifts — see below).
- **Shift operands** (§6.5.7.3): each operand integer-promotes
  independently; the result type is the promoted left operand's type.
  The right keeps its own promoted type — c99_to_tac's shift-helper
  path passes only its low byte to asl/asr/lsr.
- **Assignment** rval (§6.5.16.1): converted to lval's type.
- **CompoundAssignment** (§6.5.16.2): rval converted to the
  intermediate type stamped on the node (common-of-promoted, or
  promoted-left for shifts); the lval-load and binop-result casts
  to/from the lval's type are emitted by c99_to_tac.
- **FunctionCall** args (§6.5.2.2.7): each arg converted to the
  corresponding parameter's type.
- **Return** value (§6.8.6.4.3): converted to the enclosing function's
  declared return type (tracked on `self._return_type` while walking
  each body).
- **Variable initializers** (§6.5.16.1): block-scope auto, block-scope
  `static`, file-scope, and for-init declarations all run through the
  same conversion.

Comparisons (`==`/`!=`/`<`/`>`/`<=`/`>=`) and `&&`/`||` always yield
Int regardless of operand type, but their operands still go through
the promotion so the underlying op happens at one width. Conditional
`?:` uses the rule on its true/false branches.

**Pointer arithmetic** (C99 §6.5.6) takes its own path on
`Binary(Add | Subtract)` when at least one operand is a Pointer,
sidestepping `_common_type` (which can't construct a Pointer without a
`referenced_type`). Four legal shapes:

* `ptr + int` / `int + ptr` → result is the pointer type.
* `ptr - int` → result is the pointer type.
* `ptr - ptr` (matching pointer types) → result is `Long`, c6502's
  stand-in for the standard's `ptrdiff_t`.

For the first three the integer operand is wrapped in an implicit
`Cast(Long)` (matching the pointer's 2-byte width), so by the time TAC
sees the Binary every operand is 2 bytes wide. The actual scaling by
`sizeof(pointee)` lives in `c99_to_tac` (see the root pipeline
section). Rejected at the type-check boundary: `ptr + ptr`, `int -
ptr` (which catches `0 - p`), `ptr ± floating`, `ptr - ptr` with
mismatched pointer types, and any additive op on a function pointer
(§6.5.6.2 requires "pointer to an object type").

Ordering comparisons on pointers (`<`/`>`/`<=`/`>=`, §6.5.8) take
their own short-circuit path before `_common_type`, which would crash
on Pointer for the same reason as equality. The constraint is stricter
than equality's: both operands must be pointers to compatible object
types — null pointer constants aren't accepted on the relational ops.
Result is always Int per §6.5.8.6. `tac_to_asm` dispatches pointer
ordering to its own unsigned-ordering lowering (per-byte SBC with
carry threading, then BCC/BCS — no V-correction), so addresses above
$8000 rank correctly. `>` / `<=` swap operands the same way the signed
form does. Non-pointer ordering still uses the signed V-corrected
sequence (a known limitation for `unsigned long` operands).

**Array-to-pointer decay** (C99 §6.3.2.1.3) is reified by
`_decay_if_array(exp)`: if `exp.data_type` is `Array(elem, N)`, wrap
`exp` in an implicit `AddressOf` stamped with `Pointer(elem)` and
return the wrapper. Each call site that consumes an expression —
Binary / Conditional / Cast inner / Assignment rval / FunctionCall arg
/ Return value / var initializer / Subscript array operand /
Dereference operand — is responsible for decaying its inputs before
further type checking; the `_is_object_type` predicate excludes Array,
so any missed decay site fails as a non-object-type error rather than
silently producing nonsense. The wrapper type is narrower than the
standard's `Pointer(Array(elem, N))` (we use `Pointer(elem)`, the
address of the array's first element) — equivalent at the byte level
since both are the same 2-byte address, and downstream pointer-
arithmetic scaling reads the pointee from `Pointer.referenced_type`.
User-written `&arr` for an array DOES yield the standard
`Pointer(Array(elem, N))`; both forms work end-to-end because
`_to_tac_data_type` collapses `Pointer` onto `Long` and
`_pointee_size` recurses into `Array` for the scale factor.

**Subscript** (`Subscript(array, index)`) is type-checked but left in
the AST for `c99_to_tac` to lower (rather than rewritten here to
`Dereference(Binary(Add, decayed, index))`, which would require every
parent slot to reassign). Per C99 §6.5.2.1.2 the subscript operands
are symmetric — `E1[E2]` is defined as `*((E1)+(E2))`, so either side
may be the pointer/array and the other side the integer. The type
checker accepts both `arr[3]` and `3[arr]`, swapping operands when
needed so the canonical AST always has `Subscript.array` holding the
pointer side. The array operand decays to Pointer; the index is
widened to Long; the result type is the pointee. Pointer-typed array
operands (`p[i]` where `p` is a pointer) skip the decay step but go
through the same downstream lowering.

**Variable declarations** distinguish two predicates: `_is_object_type`
(the operand-allowed set: arithmetic types and Pointer) and
`_is_complete_object_type` (adds Array). Var-decl sites use the
broader predicate since `int a[10];` IS a legal declaration;
arithmetic / operand sites use the narrower one because arrays must
decay first.

Static-storage initializers stay constant-expression-only:
`_const_init_value` recursively drills through any number of Cast
wrappers (the parser produces `Cast` for explicit casts, and the
implicit-conversion rule wraps a mismatched literal in another Cast)
to the underlying integer or float value.

**Switch type-checking** (C99 §6.8.4.2). The control expression must
have integer type — Int / Long / UInt / ULong; Float / Double /
Pointer rejected per §6.8.4.2.1. After integer promotion (a no-op in
c6502 since every integer type is already at promotion rank ≥ Int),
the promoted type is stamped on `SwitchStmt.promoted_type` and each
case value is funneled through
`passes.constant_expression.evaluate_integer_constant_expression` to
fold it to a Python `int`, converted to the promoted type modulo width
via `_coerce_int_to_type`. Each case's `value` is then replaced by a
single canonicalized `Constant` of the promoted type so c99_to_tac
sees a uniform shape. Uniqueness is checked on the converted integer
values (per §6.8.4.2.3), so e.g. `case 256:` in an Int (1-byte) switch
wraps to 0 and conflicts with `case 0:`. The case body and any nested
case / default nodes are type-checked normally.

`passes.constant_expression` provides two entry points sharing the
§6.6 vocabulary: `evaluate_integer_constant_expression` (returns
`(value, type)` for §6.6.6 sites — case labels today; future enums /
array sizes / bitfield widths) and `validate_constant_expression` (the
§6.6.3 check without folding, for arbitrary constant-expression
contexts — currently no consumers, kept for upcoming features).
Today's integer evaluator accepts a `Constant` integer literal
optionally wrapped in any number of integer Casts; expanding to Unary
/ Binary / Conditional folding drops in via additional match arms.

Errors raised (`TypeCheckError`):

- Function used as a variable / variable called as a function.
- Wrong call arity.
- Mismatched binary-operator operand types (only when neither is an
  object type — every Int/Long/UInt/ULong/Float/Double mix is handled
  by promotion now).
- Initializer / cast / return-value types not assignable.
- Cast target isn't an object type (no `FunType` casts).
- Incompatible redeclaration of an object or function (signature
  differs, or linkage disagrees with prior).
- Multiple definitions (function with `defined=True` already, or two
  distinct file-scope `Initial(c)` values for one object).
- Static-storage initializer isn't a constant expression.
- Switch control expression isn't integer-typed.
- Case label isn't an integer constant expression.
- Two case constants in the same switch share the same value after
  conversion to the switch's promoted control type.

The function-name table from identifier_resolution and the variable-
scope table both feed into the symbol table here: variable names
arrive already unique (`@<N>.<orig>`) so a flat `dict` is enough — no
nested scopes. Functions are pre-registered from their definitions
before each body is checked, so a body can self-recurse without a
forward declaration.

## Pass 9: `replace_pseudoregisters.replace_program`

Replaces every `Pseudo(name, offset)` operand with a concrete
addressing-mode operand and lays out the function's stack frame.
Takes the type-checker's SymbolTable so it can size each pseudo by its
declared type: 1 byte for `Char` / `SChar` / `UChar` / unknown, 2 for
`Int` / `UInt` / `Pointer`, 4 for `Long` / `ULong` / `Float`, 8 for
`LongLong` / `ULongLong` / `Double`. Optionally takes a `colorings:
dict[func_name, Coloring]` from the optimizer when `--optimize` is on;
without it, every pseudo goes to Frame as before.

Walks each function twice:

- **Pre-step:** if a `Coloring` is supplied, derive the set of zero-
  page byte addresses the function uses from the callee-saved pool
  (every byte of every colored value that falls in
  `coloring.pool.callee_saved()`). These bytes get reserved at the
  bottom of the frame (`FP+1..FP+S`); locals shift up by S to leave
  room. The prologue saves them; the epilogue restores them.
- **Pass 1 (discovery):** mint a *base* offset (the offset of byte 0)
  for every Pseudo name that *isn't* in the function's `params`, isn't
  in the program's static-storage set, and isn't colored. Locals get
  sequential base offsets in source-encounter order, each advancing
  the cursor by `size_of_name(name)`. After the walk, M = total local
  bytes (including the S-byte callee-save area).
- **Finalize:** compute param base offsets analogously. The first
  param's first byte is at `Frame(M + 3)`; each subsequent param
  starts after the previous one's bytes. The 2-byte gap at M+1, M+2
  holds the saved caller FP.
- **Pass 2 (replacement):** rewrite each Pseudo operand. The decision
  order is: static → `Data(name, offset)` (absolute, link-time
  address); param → `Frame(...)` (calling convention wins even if
  regalloc colored it); colored local → `ZP(addr, offset)` (the ZP
  byte from `coloring.assignments` plus the Pseudo's offset); ordinary
  local (uncolored, spilled, or address-taken) → `Frame(base +
  offset)`.

The pass also prepends `FunctionPrologue(arg_bytes=N, local_bytes=M,
callee_saved_addrs=[...])` and patches every `Ret(...)` with the same
N/M and the same addrs list, so the emitter has the dimensions it
needs for the prologue's space-allocation step, the save/restore
sequences, and the epilogue's SSP-rewind.

`replace_pseudoregisters_bare_exit` is the optimizer-mode variant:
colored → `ZP(addr, 0)`; spilled / address-taken / params →
`Frame(off)` or `Data(slot_symbol, 0)` for zp_abi params. Excludes
private-pool addresses from the callee-save list — addresses in the
private pool are by-construction safe across calls regardless of where
they land in ZP.

## Pass 10: `asm_to_asm2.translate_program`

`asm_ast` → `asm2_ast`. Strictly-atomic-IR lowering: rewrites the
three asm_ast compound nodes (`AllocateStack(N)` for caller-side soft-
stack allocation, `FunctionPrologue` for the callee's frame setup,
`Ret` for the matching teardown) into sequences of single-instruction
asm2 atoms, and re-tags every other instruction / operand /
static_init / reg / condition payload at the asm2 type. The result has
every node = one logical 6502 instruction (where indirect-Y addressing
setup counts as addressing-mode setup, per the asm-emit convention).
Three asm2-only atoms join the existing instruction set: `Return`
(RTS — what `Ret` collapsed to in the no-frame case), `Comment(text)`
(block-level "; …" line at opcode column — what the prologue /
epilogue used to emit inline), and `Blank` (a blank-line separator
between prologue / body / epilogue; emit collapses runs of these).
`LoadAddress` stays a single atom (its expansion is short enough to
keep as one logical "compute the address into two bytes" step).

The compound-node lowerings are deliberately naive: they drop the
INY / TAX / STX byte-saving tricks that `asm_emit` used to use, in
exchange for a uniform "each Mov is self-contained" model where the
same `Mov(Reg(A), Stack(off))` atom always emits its own LDY setup.
That costs +1 byte per `FunctionPrologue` save-FP step and +2 bytes
per non-trivial `Ret` restore-FP step versus the old emit.
`sim.assembler._prologue_size` / `_ret_size` / `_emit_prologue` /
`_emit_ret` mirror the same naive lowering so `instruction_size` (used
by `passes.long_branches`) and `assemble` (the in-process binary
assembler) stay byte-aligned with what `asm_emit` produces.

## Peephole catalog

`compile._peephole_fixedpoint` runs the following passes in order
until a full sweep produces no change. Each pass is a separate module
under `passes/`; see the module docstring for the full rationale and
motivating shape. Order matters — each pass can enable the next
(e.g. INC chains shorten, freeing up direct-LDX/LDY rewrites). The
loop is capped at `_PEEPHOLE_FIXEDPOINT_CAP = 16` iterations as a
safety net.

Always-on (runs in both optimized AND unoptimized pipelines):

- `apply_inc_peephole` — multi-byte ADC-#1 chains on stable memory →
  `INC + BNE` chains. See "Multi-byte INC peephole" below.
- `apply_dec_peephole` — single-byte SBC-#1 chains on stable memory →
  `DEC`. (No multi-byte form: DEC sets N/Z off the post-decrement
  value, so the underflow check would have to sit BEFORE the DEC, not
  after, which doesn't match the chain shape.)
- `apply_sub1_test_zero_peephole` — folds the `for (uint8_t i = N;
  i-- > 0; ) { ... }` shape's separate sub-and-test pair into a single
  `DEC M; BNE label` (or inverted variant) since DEC's flag side-
  effect IS the post-decrement zero test.
- `apply_direct_index_load` — `LDA M; TAX` → `LDX M` when M is
  `Imm`/`Data`/`ZP` and `Reg(A)` is dead after. See "Direct-into-X/Y
  peephole" below.
- `apply_dead_pha_pla` — drops `PHA / body / PLA` when the body
  preserves `Reg(A)` and the PLA's N/Z flag effect is dead. The
  indirect-indexed-store lowering emits a conservative save/restore
  around its idx-into-Y stage that this pass collapses once
  `direct_index_load` has fused the inner `LDA idx; TAY` into a
  flag-preserving `LDY idx`.
- `apply_cpx_cpy_peephole` — `Mov(X|Y, A); Compare(A, imm)` →
  `Compare(X|Y, imm)` (`CPX` / `CPY`) when the compare's left is
  already in X or Y. Loop-induction-variable test shape.
- `apply_memory_value_propagation` — CFG-aware forward dataflow
  tracking ZP-cell → recomputable-source equivalences. Two
  rewrite families: (1) `Indirect` / `IndirectY` operands rewrite
  to `IndirectZp(N)` / `IndirectZpY(N)` when DPTR's bytes are
  proven equal to a stable ZP pair at `N` (subsumes the former
  `apply_indirect_base_prop`); (2) `Mov(M, dst)` reads of a stage
  cell `M` whose tracked Expr is `Imm` / `Data` / `ImmLabelLow` /
  `ImmLabelHigh` rewrite to `Mov(<recomp>, dst)` directly,
  collapsing the staging round-trip (overlaps with `apply_remat`
  for these source kinds; the IndexedData source is still
  apply_remat's). The CFG-aware reach catches patterns that
  block-local passes miss (preheader stages used inside loop
  bodies, indirect bases that survive labels, etc.).

Only meaningful with `--optimize` (the unoptimized pipeline skips
them):

- `apply_split_mem_to_mem` — splits `Mov(mem, mem)` into the
  `Mov(mem, Reg(A)); Mov(Reg(A), mem)` pair it would emit as,
  exposing both halves to every downstream peephole. Volatile
  mem-to-mem skipped (see "Mem-to-mem splitting" below). Self-Movs
  dropped. Runs at the top of the fixedpoint so subsequent passes
  in the same iteration see the split form.
- `apply_via_a_store_fold` — folds `TXA;STA M` (and TYA;STA M)
  to STX M (STY M) when A and flags are dead at the next
  instruction. Recovers the STX/STY-direct form for the post-split
  shape of an X-save-slot mem-to-mem read.
- `apply_redundant_load_after_rmw` — drops `LDA M` after `INC M` /
  `DEC M` / shift-on-M when only the N/Z flag effect was needed (the
  RMW already set N/Z off M's new value).
- `apply_redundant_load_elimination` — per-block A/X/Y tracker: if
  `LDA M` (or LDX/LDY M) is about to read M and the target register
  already mirrors M, drop the load. Heaviest after loop unrolling.
- `apply_redundant_store_elimination` — drops STAs whose written cell
  is overwritten before any read. Memory-to-memory transfer redundancy
  (e.g. the unrolled DPTR-staging sequences `redundant_load`'s A-
  tracking can't see across an intervening A clobber).
- `apply_asm_dead_store` — CFG-wide forward dead-store elimination on
  Mov-into-memory atoms. Drops or morphs (to LDA-only) STAs whose
  target byte isn't observed by any reachable instruction. Treats
  DPTR / pool ZP / local-pool slot symbols as dead-at-exit.
  `LoadAddress` is modeled precisely (read FP/FP+1 only for Frame src;
  bounded 2-byte write at dst) rather than opaque. `Call` /
  `FunctionPrologue` / `AllocateStack` are opaque.
- `apply_dead_a_arith_elimination` — drops instructions whose only
  observable effect is a write to `Reg(A)` and the N/Z/C/V flags, when
  both A and the flags are dead afterward.
- `apply_branch_invert` — `Branch(cond, L); Jump(target); L:` →
  `Branch(inverted_cond, target)` when L is the immediate next
  instruction.
- `apply_mem_const_prop` — per-block memory-cell value tracker
  (`Data(name, off)` / `ZP(addr, off)`); substitutes the known
  immediate at any downstream operand slot that accepts `Imm`.
- `apply_const_arith_fold` — folds `LDA #C1; ALU #C2` chains that
  produce a compile-time-known A value, replacing the sequence with
  `LDA #folded`. Most useful for the high-byte branch of an int-typed
  AND of a uchar value (`(uchar & 0x80)` after promotion).
- `apply_round_trip_load_drop` — drops `STA M; LDA M` where the LDA's
  only observable side effect is re-loading A with A's already-current
  value.
- `apply_and_sign_bit_branch` — `Mov(M, A); And(Imm(0x80), A);
  Branch(EQ|NE, _)` → `BitTest(M); Branch(PL|MI, _)` when M is BIT-
  addressable (`Data` / `ZP`) AND A is dead after. Pays 1 byte / 3+
  cycles per occurrence and preserves A for downstream use.
- `apply_self_store_drop` — `Mov(M, A); ...; Mov(A, M)` where the body
  doesn't modify M and A reads M → drop the trailing self-store.
- `apply_cmp_sbc_fusion` — fuses a `Compare; Branch; ...; SBC` pattern
  where the SBC's effect duplicates the Compare's flag set.
- `apply_dec_inc_branch_fold` — `Dec(M)/Inc(M); Mov(M, A);
  Branch(EQ|NE, _)` → `Dec(M)/Inc(M); Branch(EQ|NE, _)` — drops the
  redundant LDA since INC/DEC already set N/Z off M's new value.

Two more asm-only peepholes run OUTSIDE the fixed-point loop:

- `passes.y_peephole` (`apply_y_peephole`) — collapses adjacent
  `LDY #<off>` setups for indirect-Y accesses to the same or adjacent
  offsets into a single `LDY` plus `INY` / `DEY`.
- `passes.long_branches` (`expand_program`) — rewrites conditional
  branches whose target is out of the ±127-byte range into
  `Branch(inverted_cond, .skip); Jump(target); .skip:`. Runs once,
  after every other peephole has settled.

## Mem-to-mem splitting (`split_mem_to_mem.py`)

The asm IR allows a `Mov` atom whose src AND dst are both memory
operands. The 6502 has no `MOV mem, mem` opcode, so `asm_emit`
lowers such an atom to `LDA src; STA dst` using A as the staging
register. Historically this compound form was opaque to every
instruction-stream peephole — `redundant_load_elimination`
couldn't see the implicit LDA, `apply_and_sign_bit_branch` lost
adjacency with the AND it was trying to fold, and so on. The
fix used to be a growing list of per-pass mem-to-mem-aware
carve-outs (`_update_for_mov`'s volatile branch,
`x_save_slot_load`'s Pass 3, `round_trip_load`'s Pattern B).

`apply_split_mem_to_mem` runs at the top of the asm-peephole
fixedpoint and rewrites every non-volatile `Mov(mem_src, mem_dst)`
into the explicit pair:

    Mov(mem_src, Reg(A))     # LDA src
    Mov(Reg(A), mem_dst)     # STA dst

Every downstream peephole then sees the LDA + STA as separate
atoms and applies its normal logic — `redundant_load_elimination`
drops the LDA if A already mirrors src, `asm_dead_store` drops
the STA if dst is dead, etc.

**Volatile mem-to-mem stays compound.** The `is_volatile` flag on
a Mov atom is conservative: it's True when *either* operand is a
volatile-typed cell, with no way to tell which. Splitting would
force both halves to inherit the bit, blocking
`redundant_load_elimination` (which never drops volatile loads)
and regressing on cases like the `volatile uint8_t y` inner loop
in `sfx_tone`. The existing volatile-aware branch in
`redundant_load._update_for_mov` already handles these correctly
without splitting.

**Self-Mov `Mov(M, M)` is dropped, not split.** Mirrors the
existing emit-time peephole at `asm_emit.py:513`.

**Other passes that produce mem-to-mem.** `apply_memory_value_
propagation` can substitute a tracked-Expr into an operand slot
and create a fresh mem-to-mem atom; the split runs at the top of
each fixedpoint iteration so any such mid-fixedpoint creation is
caught on the next pass through.

**Supporting peepholes.** Two follow-ups recover what the split
broke for specific patterns:

  * `apply_via_a_store_fold` — `TXA;STA M → STX M` (and Y).
    Recovers the STX/STY-direct form for an X-save-slot mem-to-
    mem read after `x_save_slot_load`'s Pass 3 rewrites the LDA
    half to TXA but leaves the STA in place.
  * `apply_and_sign_bit_branch`'s 4-instr variant — tolerates an
    intermediate STA between the LDA-shape atom and `AND #80;
    Branch`. The STA is what the split inserts when the original
    mem-to-mem-LDA was the source value.

## Direct-into-X/Y peephole (`direct_index_load.py`)

Always-on asm-level peephole. `tac_to_asm` always stages a value into
`Reg(X)` or `Reg(Y)` via `Reg(A)`:

```
Mov(M, Reg(A))            ; LDA M
Mov(Reg(A), Reg(X))       ; TAX
```

This is conservatively right at lowering time — `M` is still a
`Pseudo` and could resolve to `Frame` / `Stack` / `Indirect`, which
use indirect-Y addressing that `LDX` / `LDY` don't support. After
`replace_pseudoregisters` resolves Pseudos to concrete operand types,
we can short-circuit the round trip when `M` is addressable by `LDX` /
`LDY` directly:

- `Imm`  — `LDX #imm` (2 bytes vs `LDA #imm; TAX` = 3 bytes).
- `Data` — `LDX abs` (3 bytes vs `LDA abs; TAX` = 4 bytes).
- `ZP`   — `LDX zp` (2 bytes vs `LDA zp; TAX` = 3 bytes).

Saves 1 byte / 2 cycles per occurrence.

Eligibility:

- Two consecutive instructions match `Mov(src=M, dst=Reg(A));
  Mov(src=Reg(A), dst=Reg(X|Y))`.
- `M ∈ {Data, ZP, Imm}` — the addressing modes `LDX` / `LDY` support
  directly. `Frame` / `Stack` / `Indirect` skip.
- `Reg(A)` is dead immediately after the second `Mov` — no subsequent
  read of A before A is redefined. Uses a forward liveness scan within
  the basic block (mirrors `backward_copy_propagation._a_dead_at`).

Flag soundness: `LDA M; TAX` sets N/Z based on M's value (LDA sets,
TAX overwrites with the same value); `LDX M` sets N/Z based on M's
value. Same flag state at the rewrite's exit, so any subsequent
`Branch` observes the same condition.

Runs after `replace_pseudoregisters` (operands concrete) and before
`expand_long_branches` (no new branches introduced — the pass shrinks
code, never expands). Active in both optimized and unoptimized
pipelines, like `inc_peephole`.

## Multi-byte INC peephole (`inc_peephole.py`)

Always-on asm-level peephole that runs after `replace_pseudoregisters`
(so operands are concrete `Data`/`ZP`/`Frame`/etc.) and before
`expand_long_branches` (so any new BNE displacements participate in
long-branch checking). Detects the multi-byte add-1 chain emitted by
`tac_to_asm` and rewrites it to an `INC + BNE` chain on the target
memory operand.

The chain pattern (per byte, in order):

- Byte 0: `Mov(M[0], A); ClearCarry; Add(Imm(1), A); Mov(A, M[0])` — 4
  instructions, in-place RMW on M[0].
- Each continuation byte k≥1: `Mov(M[k], A); Add(Imm(0), A); Mov(A,
  M[k])` — 3 instructions, in-place RMW on M[k]; no CLC since the
  carry threads from the prior ADC (LDA only sets N/Z, leaves C
  intact).

Eligibility (per-byte; failures break the chain at that byte):

- `M[k]` is `Data(name, k)` or `ZP(addr, 0)`. The 6502's INC has zp /
  abs / zp,X / abs,X modes — no `(ind),Y` — so `Frame` / `Stack` /
  `Indirect` operands stay as ADC chains.
- The pattern's per-byte LDA source equals the STA destination (in-
  place). After SSA destruction routes through a temp (common for
  parallel-copy ordering), `LDA $84; ... STA $82` isn't in-place on
  $84 and we skip — INC would corrupt $84 instead of producing the
  right answer through the temp.

Bytes don't have to be at consecutive addresses. Byte-granular asm
SSA + regalloc may color the bytes of one logical multi-byte value to
non-adjacent ZP slots — the structural pattern (CLC-ADC#1 first,
ADC#0 continuations, every byte in-place RMW'd) is only emitted by
the multi-byte add-1 lowering, so wherever the bytes live, INC + BNE
preserves semantics.

Replacement for an N-byte chain:

- N == 1: a bare `Inc(M[0])` — no branch needed (caller flow continues
  naturally).
- N >= 2: `Inc(M[0]); Branch(NE, .inc_done@K); Inc(M[1]); Branch(NE,
  .inc_done@K); ...; Inc(M[N-1]); Label(.inc_done@K)` — each non-last
  byte's BNE skips the remaining INCs when its INC produced a non-zero
  result (no carry into the next byte). A fresh `.inc_done@<counter>`
  label is minted per chain; leading `.` keeps it dasm-local (per-
  SUBROUTINE), `@<digits>` keeps it disjoint from user labels and
  other translator-minted ones (`.if_end@<N>`, `.lb_skip@<N>`, …).

Byte / cycle savings (in-place add-1, vs the ADC chain):

- 16-bit absolute: 17 → 8 bytes; 22 → 9 cycles (no overflow into high
  byte) or 14 cycles (with overflow).
- 16-bit ZP: 13 → 6 bytes; 18 → 8 / 12 cycles.
- 4-byte absolute: 33 → 18 bytes; cycle savings scale similarly.

The Z flag's value at the rewrite's exit is unreliable — it depends on
which BNE was the last to fire. The C and V flags are left untouched
by INC (the original ADC chain set them per the final ADC). c6502's
codegen never reads any of these across separate operations (every
comparison emits its own LDA that resets N/Z, and SEC/CLC before each
SBC/ADC), so the difference is invisible to subsequent instructions.

Limitations / what doesn't fire:

- `+= 2` and other small constants — INC only adds 1; chaining INCs
  would lose the win for n ≥ 2.
- `-= 1` — needs a separate DEC peephole; DEC sets flags off its
  result, so detecting underflow needs an LDA + BNE BEFORE the DEC,
  not after. Not implemented yet.
- In-place writes to a static when TAC routes through a temp.
  `static int counter; counter += 1;` lowers as `Binary(Add, counter,
  1, %t); Copy(%t, counter)` — the ADC writes to %t, not counter, so
  the in-place check fails. (Copy folding handles the static-RMW case
  now — see `optimization/CLAUDE.md` for the composition.)

## Asm-level promotions

`prologue_synthesis.synthesize_prologue`: when `arg_bytes ==
local_bytes == callee_saved_bytes == 0`, the bare `Return(save_a)`
atoms stay and no `FunctionPrologue` is prepended. Otherwise prepend
`FunctionPrologue(N, M, callee_saved_addrs)` and patch each `Return`
to `Ret(N, M, save_a, callee_saved_addrs)`.

`asm_licm.apply_licm`: asm-level LICM-lite for loop-invariant constant
stores. Identifies natural loops by back-edge, hoists `Mov(Imm,
Data|ZP)` and `LDA #c; STA M` pairs to the preheader when the dst
isn't otherwise written in the body, no `Call` appears in the body
(conservative — sidesteps zp_abi clobber questions), and the loop has
a single entry through the header.

`loop_counter_to_x.loop_counter_to_x`: asm-level. Promotes a loop
counter pseudo to `Reg(X)` when the live range fits the X pivot
pattern: the counter is initialized once outside the loop, used as an
`LDX` source inside, decremented at loop bottom, and not live across
any JSR (saved/restored around them with `STX`/`LDX`). Also accepts
`LDA M` body uses (rewritten to `TXA` since X = M is the promotion
invariant). Y-pivot ranges within the loop reject ranges containing
Indirect / IndirectY / IndirectZp / IndirectZpY operands — these read
Y for addressing and the pivot's LDX→LDY rewrite would clobber that
Y. The classic refresh_hit_entities winner — ~5× speedup on the hot
loop. Composes with the Y-pivot path inside the promotion shape.

## Call-graph-disjoint ZP allocation

Under `--optimize`, c6502 hands each eligible function a private range
of ZP bytes for its params (zp_abi) and body locals, allocated so no
two functions on a common caller-callee path share a byte. The
"caller-saved vs callee-saved" partition collapses for eligible
functions — there's nothing to save in the prologue because no
caller's storage overlaps with the callee's range. Eligible functions
emit as bare body + RTS.

### zp_abi (param passing)

Caller writes arg bytes directly to the callee's ZP slot symbols
(`STA __zpabi_<callee>_p<k>`); callee reads its params from those same
symbols. No `AllocateStack`, no Frame-resident param storage. Eligibility:

- No `IndirectCall` in body (callee's ABI unknowable at the indirect
  site).
- Not on a cycle in the static direct call graph (a recursive call
  would overwrite the outer activation's still-live params).
- Address never taken (indirect call sites would assume the default
  soft-stack ABI).
- Param byte count fits the ZP window (default 64 bytes, $80–$BF).
  When the chain saturates, slots spill into a non-ZP fallback region
  (default $0200–$FFFF); dasm picks absolute addressing automatically,
  so the call-site / callee IR is unchanged.

Under `--optimize`, every function defaults to zp_abi when eligible.
The explicit `__attribute__((zp_abi))` annotation is a strict-mode
opt-in: an annotated function hard-errors on ineligibility, an
unannotated one silently falls back to soft-stack. Same eligibility
rules apply either way.

### Body-local private pools (every eligible function)

`zp_local_allocation.py` extends the same call-graph allocation to
body locals — the bytes the asm regalloc colors for Pseudos that
aren't params, statics, or spilled. Each eligible function gets a
private byte range disjoint from every transitive caller's range AND
every coexisting zp_abi function's param slots. Eligibility:

- Defined in this TU (we need the body to size locals and enumerate
  callees).
- No `IndirectCall`.
- Not in any call-graph cycle (Tarjan SCC).
- Every direct callee is also eligible, OR is a zp_abi extern (treated
  as a bounded leaf via its declared param slots). A non-zp_abi extern
  callee disqualifies the caller.

Ineligible functions fall back to the conservative caller/callee
partition ($80..$BF caller-saved, $C0..$FF callee-saved with the usual
save/restore discipline).

### Allocation algorithm (shared between param and local passes)

Topological order over the call graph, parents first. For each
function `F`, compute the forbidden set as the union of every already-
allocated ancestor's range plus every coexisting zp_abi function's
param slots (ancestors AND descendants — both can be on the call stack
with F). Pick the lowest contiguous free range of the required size
in the ZP window; spill above $FF on saturation. Siblings (non-
comparable in the caller-callee reachability relation) freely share
addresses, since their activations are never simultaneous on the
stack.

### Pass roles

- `abi_selection.py` (`select_abi`): decides which functions are
  zp_abi, mints `__zpabi_<fn>_p<k>` slot symbols.
- `zp_slot_allocation.py` (`allocate_zp_slots`): binds the slot
  symbols to ZP addresses via call-graph topo.
- `function_local_sizing.py` (`compute_local_bytes`): counts each
  function's regalloc-colored ZP byte footprint from a preliminary
  optimizer pass.
- `zp_local_allocation.py` (`allocate_function_locals`): hands each
  eligible function a private body-local range, disjoint from
  coexisting footprints.
- `tac_to_asm` / `replace_pseudoregisters_bare_exit`: emit
  `Data(slot_symbol, 0)` operands for zp_abi param refs (both call-
  site and callee-side).
- `optimization_asm/optimizer.py`: takes `local_pools`, passes each
  function's range to `color_graph` via `allowed_range`. When set, the
  regalloc draws colors exclusively from that range;
  `lives_across_call` no longer drives color choice.
- `replace_pseudoregisters` excludes private-pool addresses from the
  callee-save list — addresses in the private pool are by-construction
  safe across calls regardless of where they land in ZP.
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

Two optimizer passes: the first sizes each function's local byte
demand; the second uses per-function private pools as the regalloc's
`allowed_range`.

### Future: cross-TU

Today the linker is dasm; per-TU compilation produces all the slot-
symbol `EQU` bindings inline. Phase 2 would split the EQU emission
into a separate `slots.inc` from a multi-TU linker that runs the
allocator globally. The IR shape (symbolic slot refs in every
`Data(__zpabi_*)` operand) is already prepared for that; the per-TU
compile doesn't need to change. See `docs/leaf_zp_abi.md`.
