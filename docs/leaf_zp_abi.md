# Frame elimination via per-function ZP-passing ABI

A design for an optimization that gives small leaf functions a
calling convention based on zero-page argument passing instead
of the soft data stack. When a leaf function takes few enough
arguments to fit them in the caller-saved ZP region and uses no
frame-resident locals, its prologue and epilogue collapse
entirely to a bare `RTS` — no SSP/FP arithmetic, no saved-FP
slot, no per-byte `(FP),Y` reads in the body.

This document describes the design (what changes, why, and which
edge cases matter) and a step-by-step build plan that mirrors the
previous staged work in `docs/optimization.md`.

---

## Motivation

The current calling convention always allocates a soft-stack
frame for any non-empty function. Even a function as simple as

```c
int add(int a, int b) { return a + b; }
```

emits a 17-instruction prologue (subtract from SSP, save caller
FP, capture FP) plus matching epilogue, **even though** the body
is three useful instructions. The arg reads inside go through the
slow `LDY #off; LDA (FP),Y` indirect-Y sequence (8 cycles each)
when the same bytes could be read directly via `LDA $XX` (3
cycles).

Most of that overhead is unnecessary for a function that:
- doesn't make any other function calls (leaf), AND
- takes few enough parameters to fit in some ZP slots, AND
- has no values it needs Frame storage for.

For such functions the caller can write argument bytes directly
to the ZP slots the callee expects them in. The callee reads
them from there. No SSP/FP arithmetic is involved on either
side. The function body's only setup is whatever asm-level
regalloc has already placed in ZP; if it placed everything
there, M=0 and the function has no frame at all.

---

## Scope (what's in / what's out)

**In:**
- ABI is **chosen by the programmer** via
  `__attribute__((zp_abi))` on the function declaration and/or
  definition. Without the annotation, the function uses the
  soft-stack ABI (the existing convention). This makes the
  decision explicit, lets headers propagate the contract across
  translation units, and avoids the indirect-call ABI ambiguity
  that automatic selection can't safely resolve.
- The compiler **validates** the annotation: a `zp_abi`
  function must have an empty call set in its body (no
  `FunctionCall` / `IndirectCall`), must not have its address
  taken anywhere in the program, and must have a parameter byte
  total that fits the configured ZP window. Any of these
  violated → compile-time error with a clear message.
- Per-function ABI — the annotation rides on the function
  declaration, every caller in the program agrees by reading
  the same declaration.
- All-or-nothing per function — the function's parameters all
  go via ZP, or all go via the soft stack. No mixing.
- Sharing the whole caller-saved range ($80–$BF default). The
  caller-saved pool is used by both the caller's body
  (regalloc-assigned scratch) and outgoing-call argument
  writes; interference between the two is handled by liveness.

**Out (not in this design):**
- Automatic ABI selection. The programmer's annotation is
  authoritative. (We considered automatic leaf-detection but
  rejected it: it can't decide consistently for an indirect
  call site, and silently downgrading a leaf to soft-stack
  when its address gets taken later in the file leads to
  surprising re-compilations.)
- Mixed-ABI parameter passing within a single function. We
  don't pass the first 2 params in ZP and the rest on the
  stack. Either everything fits in ZP and the function is a
  ZP-ABI function, or it's soft-stack-ABI.
- Cross-TU enforcement beyond header propagation. The
  annotation must appear on the function declaration in any
  shared header so all TUs that see the declaration agree on
  the ABI. A definition in TU A annotated `zp_abi` paired
  with a declaration in TU B without the annotation will
  miscompile silently — the convention requires programmer
  discipline at the header level. (When separate compilation
  lands, name-mangling at link time is the natural belt-and-
  suspenders enforcement; see "Cross-TU and `extern`" below.)

---

## Definitions

### Leaf function

A `tac_ast.Function` is a **leaf** iff its `instructions` list
contains zero instances of `tac_ast.FunctionCall` and zero
instances of `tac_ast.IndirectCall`. Determined post-TAC,
post-optimizer (so that any opportunistic constant folding or
inlining that already eliminated calls counts in the function's
favor).

Conservative on uncertainty:
- **Address-taken function.** A function whose name is the
  operand of `GetAddress` may be called indirectly through any
  pointer in the program. We don't track the pointer's
  reachability (would need points-to analysis), so any function
  with `GetAddress(name=fn)` somewhere in the program is
  classified as **non-leaf** — even if its own body is leaf-shaped.
  Reason: a caller might invoke it via `IndirectCall` thinking
  the soft-stack ABI applies. Without points-to analysis we can't
  prove otherwise.
- **`extern` declaration without a body.** Treated as non-leaf
  (could be defined elsewhere with a call inside).

### ParamLayout

A per-function description of where each parameter byte lives
on entry. One of two shapes:

```
ParamLayout = SoftStackLayout | ZpLayout

SoftStackLayout         (the existing convention)
                         Each param byte j of param p (1-indexed)
                         lives at Frame(M + 2 + sum_prior_param_sizes
                         + byte_offset). N = total arg byte count.

ZpLayout(addrs)         (the new convention)
                         Each param byte j of param p (1-indexed)
                         lives at ZP(addrs[i]) where i is the
                         flat byte index across all params (low
                         byte of param 0 first). N = 0 — caller
                         does NOT allocate any soft-stack arg
                         space.
```

The layout is associated with the function's name in a
program-wide `dict[str, ParamLayout]` produced by an
**ABI-selection pass** (new) that runs after TAC optimization
and before `tac_to_asm`.

### ABI

The function's ABI is the combination of:
- its `ParamLayout` (where params live on entry), and
- its return-value convention (unchanged: A for 1B, HARGS for
  wider — same regardless of ParamLayout).

---

## Selection rules

Selection is **driven by the programmer's annotation**, not by
automatic body analysis. Default ABI is soft-stack. A function
becomes ZP-ABI only when explicitly marked.

| Function annotation | ParamLayout |
|---|---|
| (none) | `SoftStackLayout` |
| `__attribute__((zp_abi))` | `ZpLayout` *(after validation)* |

When a function carries the `zp_abi` annotation, the compiler
validates that:

1. **No nested calls.** The body contains zero `FunctionCall`
   and zero `IndirectCall` instructions.
2. **Address not taken.** No `GetAddress(operand=Var(name=fn))`
   or `Var(name=fn)` reference appears anywhere in the program.
3. **Params fit.** Total parameter byte count is no greater
   than the available ZP window.

Any violation is a compile-time error with a clear message
naming the specific check that failed (e.g. *"function `foo`
declared `__attribute__((zp_abi))` but its body contains a
call to `bar`"*).

The "available ZP window" is configurable but defaults to the
caller-saved region $80–$BF (64 bytes). At the design level
this is a per-program scalar; at the implementation level it
threads through the same `Pool` object the asm-level regalloc
uses, so the two stay in sync.

For declarations and definitions of the SAME function, the
annotation must appear on whichever the programmer wants to be
authoritative — typically both the header declaration and the
definition. If the annotation appears on EITHER, the compiler
treats the function as `zp_abi`. If it appears on BOTH, they
must agree (they trivially do — there's only one form). A
declaration WITHOUT the annotation paired with a definition
WITH it (or vice versa) is currently NOT diagnosed within the
single TU; it will silently miscompile if other TUs see only
the unannotated declaration. Header propagation is the
programmer's responsibility.

---

## Pipeline placement

The ABI decision affects both call-site lowering and callee-side
parameter access, so it must run before `tac_to_asm`. It does
NOT need TAC-level information beyond the body shape (call /
no-call) and the param types — both are available right after
`c99_to_tac` plus the optimizer pass.

```
parse → resolve → check → c99_to_tac
                                │
                                ▼
                         optimize_tac (TAC fixed-point;
                                       regalloc skipped under
                                       --optimize-asm)
                                │
                                ▼
                       [ABI-selection pass]   ← new
                                │
                                ▼ (TAC unchanged + abi: dict[name, ParamLayout])
                          tac_to_asm           ← consumes abi
                                │
                                ▼
                  asm-level SSA + opts + regalloc
                                │
                                ▼
              replace_pseudoregisters_bare_exit ← consumes abi
                                │
                                ▼
                        prologue_synthesis      ← consumes abi
                                │
                                ▼
                              ...
```

The TAC tree itself is unchanged by ABI selection — the dict
rides alongside through the rest of the pipeline. This keeps
the change additive: nothing in TAC has to know about the
calling convention; passes that need to know read the dict.

---

## Caller side: lowering a call

For a `FunctionCall(name, args, dst)` in TAC, `tac_to_asm`
looks up the callee's ParamLayout in the abi dict.

### SoftStackLayout (current behavior, unchanged)

```
emit AllocateStack(total_arg_bytes)
for each arg in source order:
    for each byte k:
        emit Mov(byte k of arg, Stack(running_offset + k))
emit Call(name)
emit return-value capture
```

### ZpLayout

```
for each arg in source order:
    for each byte k:
        emit Mov(byte k of arg, ZP(layout.addrs[i + k]))
        # i = flat byte index for this arg
emit Call(name)
emit return-value capture
```

No `AllocateStack`. The caller does NOT shift SSP. The bytes
written to ZP are clobbered by the callee (which reads them as
its params) but that's fine — those slots are caller-saved by
convention, so the caller wasn't supposed to have anything
live in them across the call anyway.

### Parallel-copy hazard at the call site

If the caller's regalloc has placed a value at ZP $80, and
that value is the SOURCE of arg 1 whose DESTINATION is $82,
and another value at $82 is the source of arg 2 whose
destination is $80, the two `Mov`s form a 2-cycle:

```
Mov(ZP $80, ZP $82)   ; arg 1 ← caller's $80
Mov(ZP $82, ZP $80)   ; arg 2 ← caller's $82  -- but $82 just got overwritten
```

The same hazard exists at Phi destruction in `from_ssa` and is
already solved there: storage-key-based topological sort plus a
fresh temp Pseudo for cycles. The call-site arg-write sequence
needs the same treatment. The implementation can reuse
`_order_parallel_copies` from `passes.optimization_asm.ssa_destruction`
verbatim — it's storage-key-driven and operates on `Mov`
sequences regardless of where they came from.

### Address-of-callee restriction (re-stated)

`GetAddress(name)` for any function `f` in the program forces
`f` into the SoftStackLayout — see Selection Rules. The reason
shows up here: an indirect call via a function pointer doesn't
know the target function's ParamLayout at compile time, so the
indirect-call site can only emit the stack-based convention.
Any function reachable through a pointer must therefore use
the soft-stack ABI so the indirect call site's lowering matches.

---

## Callee side: accessing parameters

For a `Function(name, params, instructions)` in TAC,
`tac_to_asm` looks up the function's own ParamLayout.

### SoftStackLayout (current behavior)

`tac_ast.Var(name=p)` references inside the body lower to
`asm_ast.Pseudo(name=p, offset=k)`. `replace_pseudoregisters`
later resolves the param's Pseudo to `Frame(M + 3 + j_offset
+ k)` based on the order in `fn.params`.

### ZpLayout

`tac_ast.Var(name=p)` references inside the body lower to
`asm_ast.Pseudo(name=p, offset=k)` exactly as today — but the
asm-level pipeline knows (via the abi dict, threaded into
`replace_pseudoregisters_bare_exit`) that this Pseudo
represents a ZP-resident param. The Pseudo is resolved to
`ZP(layout.addrs[flat_byte_index], 0)` — the same address the
caller wrote to.

The asm-level SSA construction needs to know that a ZP-ABI
param starts its life at a specific ZP address, NOT at a
soft-stack offset. The pre-push at SSA entry — which seeds the
stack with `Pseudo(p, offset=k)` — keeps the Pseudo form
through asm-SSA renaming; only the late `replace_pseudoregisters_bare_exit`
substitution differs. Renamed versions of the param (`p.bk.v1`,
etc.) are NOT pinned to the entry ZP address — they're regular
SSA names that asm-level regalloc colors freely.

### Interaction with the asm-level regalloc

The param's entry-time location is ZP(addr). The body's
regalloc-managed SSA names ALSO want ZP slots. The two pools
share the same physical ZP region, so the regalloc must
respect the param's pinned address.

Two options:
- **Pre-color the param.** Add the param's name to the
  interference graph as a pre-colored node at ZP(addr); body
  regalloc avoids that color for any name that interferes.
  (Mirrors classic "pre-colored register" handling.)
- **Treat the param like a static.** Don't add the param name
  to the graph at all; let `replace_pseudoregisters_bare_exit`
  resolve it to ZP at the end. Body regalloc may pick the same
  color for a body name, BUT — and this is the key — the param
  is dead after the first instruction that defines a body
  successor (since SSA renaming gives `p.bk.v1` a fresh name
  on first use), so the interference is only on whatever uses
  the original `Pseudo(p, k)`. Liveness handles this correctly
  if `p` is in the graph.

The second option is simpler and matches how today's code
treats statics. Recommended starting point.

---

## Frame elimination consequences

For a ZP-ABI leaf function, the soft-stack frame size is:

```
N = 0            (no soft-stack args)
M = (Frame-resident locals ∪ callee-saved area)
S = 0            (no callee-saved needed — no nested calls
                  whose caller-saved values we might clobber)
```

So `M == 0` iff every local fits in ZP via asm-level regalloc.
That's the existing `--optimize-asm` regalloc's job — nothing
new needed there.

When `N == 0 && M == 0 && S == 0`, `prologue_synthesis`
collapses the function to a bare `RTS`. No prologue. No
epilogue beyond the value-staging in HARGS (or A for 1-byte
returns) and the RTS.

The `--optimize-asm` collapse path already handles
`N == 0 && M == 0 && S == 0`. The new ABI just makes more
functions hit that case (any leaf with few enough params).

For leaf functions where some local DIDN'T fit in ZP (M > 0),
the frame still exists. The ZP-passing ABI still saves the
caller's `AllocateStack` and the callee's param-read overhead,
even though the prologue isn't fully empty.

---

## ZP pool partitioning

The caller-saved pool $80–$BF (64 bytes default) is shared
between three uses:
- Body-local regalloc-assigned scratch.
- Outgoing-arg writes immediately before a `Call`.
- Incoming-param locations of a ZP-ABI leaf function.

These three uses are time-disjoint within any single function:
- Body-local scratch is live during the body's regular flow.
- Outgoing args are live in the (typically 2-instruction)
  window between "first arg write" and "JSR".
- Incoming params are live from function entry to the param's
  first kill.

Liveness analysis already models this in the asm-level
interference graph. The new requirements:

1. **Outgoing args.** The arg-write Movs at a call site
   contribute interference: between "first arg write" and
   "JSR", the arg-destination ZP slots are LIVE (carrying
   half-built arguments). Any body value live across that
   window must not be in those slots. Today's interference
   builder doesn't model this — it currently treats
   `lives_across_call` as "live just before the Call
   instruction", which is the right concept but the wrong
   instruction window for arg-write hazards.

   Fix: when the arg sequence is a series of Movs followed by
   a Call, treat the arg destinations as LIVE for the entire
   window. The `lives_across_call` bit on body values already
   forces them to callee-saved or to spill, so the body's
   regalloc avoids the caller-saved pool for cross-call values
   automatically. The PARALLEL-COPY hazard at the arg writes
   themselves is handled separately by `_order_parallel_copies`.

2. **Incoming params.** The function's own params at ZP
   addresses must be reflected in the body's interference
   graph either as pre-colored nodes or as fixed external
   constraints. See "Interaction with asm-level regalloc" above.

3. **Pool exhaustion.** A ZP-ABI function's params occupy some
   bytes of the pool. The body's regalloc has the rest. If the
   function has MANY caller-saved-eligible body values that
   push the pool over capacity, some spill to Frame. M
   becomes > 0 and the frame is no longer eliminated. This is
   handled by the existing spill mechanism — no new code.

---

## Cross-TU and `extern`

c6502 currently compiles a single TU (the program is one
`Program` AST). For now:
- All function definitions in the program are visible.
- `extern int f(...)` declarations without a body are treated
  as non-leaf (conservative): the abi dict gets
  `f → SoftStackLayout`.
- A future c6502 with separate compilation will need an
  attribute syntax. The reserved spelling is `__attribute__((softstack))`
  / `__attribute__((zp_abi))`. The grammar / parser changes
  are not part of this design — the dict can carry forced
  layouts for any function whose layout was specified by
  annotation, with the auto-pick rule applying only to
  unannotated functions.

---

## Annotation syntax

Function declarations and definitions can carry the annotation:

```c
__attribute__((zp_abi)) int g(int x);

__attribute__((zp_abi))
int g(int x) {
    return x + 1;
}
```

The annotation slot accepts the GCC syntax `__attribute__((<name>))`
with `zp_abi` as the only currently-recognized name. Unknown
attribute names are rejected at parse time (a stricter posture
than GCC's "warn and ignore" — we'd rather catch typos than
miscompile).

Position: the annotation may appear before the declaration
specifiers (as shown above). Other GCC-supported positions
(after the declarator, after the parameter list) are not
parsed today; we can add them as needed.

The declaration form (`__attribute__((zp_abi)) int g(int x);`)
goes in headers shared between TUs. Including the header makes
every TU's view of `g` a ZP-ABI function. The definition
either repeats the annotation (recommended for clarity) or
relies on the declaration's annotation already in scope at the
time of definition.

---

## Build plan

Mirrors the previous staged work in `docs/optimization.md`. Each
step ends with a verification gate (chapter sim corpus + new
focused tests). The chapter sim corpus is the primary backstop;
each step must leave it green.

### Step F1a: parser support for `__attribute__((zp_abi))`

Extend the C99 grammar (`c99.lark`) to recognize the
`__attribute__((<name>))` prefix on declarations. Add an
`abi_annotation` field to `Type_function_decl` and
`Type_var_decl` in `c99.asdl`. Regenerate `c99_ast.py`. Update
the parser transformer to populate the field.

Reject unknown attribute names at parse time (stricter than
GCC's warn-and-ignore — typos shouldn't silently miscompile).

Verifiable: unit tests on synthetic C source with and without
the annotation, asserting the parsed AST carries the field
correctly. Existing test corpus stays green (no annotation in
any existing test source).

### Step F1b: ParamLayout type and ABI-selection / validation pass

Define `ParamLayout` as a discriminated union (`SoftStackLayout`
/ `ZpLayout(addrs)`). Add `passes/abi_selection.py` that takes
the TAC program, the c99 AST (for the `abi_annotation`), and a
Pool (for the ZP window). Returns `dict[str, ParamLayout]`.

Selection logic — driven entirely by the annotation:
- Function annotated `zp_abi` is validated:
  - body must contain zero `FunctionCall` / `IndirectCall`
    (otherwise compile-time error: *"function `foo` declared
    `zp_abi` but contains a call to `bar`"*),
  - function name must not appear as a Var anywhere in the
    program (otherwise: *"function `foo` declared `zp_abi`
    but its address is taken"*),
  - param byte total ≤ pool size (otherwise: *"parameters of
    `foo` exceed the ZP window"*).
  When all three pass: `ZpLayout(addrs sequentially from
  pool.start)`.
- No annotation → `SoftStackLayout`.

The body-shape checks (no-calls, not-address-taken) live
inside `passes/abi_selection.py` — they're not factored out
as a separate "leaf analysis" pass, because nothing else in
the pipeline needs them.

Verifiable: unit tests on the validation logic (each error
case + the happy path). Default pool gives ZpLayout to
annotated functions with up to 64 byte-size params.

### Step F2: tac_to_asm consumes the ABI for call-site lowering

Thread `abi: dict[str, ParamLayout]` into `tac_to_asm.Translator`.
The `_translate_function_call` path branches on the callee's
layout:
- SoftStackLayout: existing AllocateStack + Stack writes.
- ZpLayout: emit `Mov(arg_byte_k, ZP(layout.addrs[i]))` for each
  byte; no AllocateStack.

Use `_order_parallel_copies` (lifted from
`passes.optimization_asm.ssa_destruction`) to topologically sort
the arg writes when the source Pseudos / ZP slots could alias
the destinations. (Most call sites won't have this hazard, but
the path needs to handle it correctly.)

Verifiable: unit tests on TAC `FunctionCall` lowering. Hand-
crafted layouts confirm the right Movs are emitted in the right
order. Chapter sim corpus stays green (all functions still
default to SoftStackLayout because the F1 pass only fires on
leaves, and the corpus's leaves haven't been wired through yet
— see F3).

### Step F3: tac_to_asm lowers callee-side params per layout

`tac_to_asm` for a ZpLayout function emits its body's
`Var(name=p)` references as `Pseudo(name=p, offset=k)` — same
shape as today. The novelty is downstream: the
`replace_pseudoregisters_bare_exit` pass needs to know how to
resolve `Pseudo(p, k)` for a ZP-ABI param.

Add a `param_layouts: dict[str, ParamLayout]` parameter to
`replace_program_bare_exit` and `replace_function_bare_exit`.
Inside `Replacer.replace`, if the operand is a Pseudo and its
name is a param of the function and the function's layout is
ZpLayout, resolve to `ZP(layout.addrs[flat_byte_index], 0)`.
Otherwise fall through to the existing param→Frame path.

Verifiable: chapter sim corpus stays green. Inspection: hand-
craft a leaf C function and verify the asm reads its params from
the expected ZP addresses.

### Step F4: prologue_synthesis honors `arg_bytes == 0` from ZP-ABI

Today's `prologue_synthesis` already collapses to bare RTS when
`arg_bytes == 0 && local_bytes == 0 && callee_saved_addrs is
empty`. With ZP-ABI leaf functions emitting `arg_bytes == 0`
from `replace_program_bare_exit` (no Frame slots for params),
the existing collapse path fires.

This step is mostly verification: add a focused test that a
trivial leaf function (`int add(int a, int b) { return a + b;
}`) produces no prologue under `--optimize-asm`. Should pass
as a side effect of F1–F3 once they're wired through.

### Step F5: caller-side regalloc respects outgoing-arg windows

Body-local values must not be placed at ZP addresses currently
holding outgoing-call args. Already partially handled by
`lives_across_call`: cross-call values prefer callee-saved, so
they avoid the caller-saved pool entirely. The remaining hazard
is non-cross-call values that happen to live across the
arg-write window of a single call.

Strengthen the asm-level interference: at every Call
instruction, the outgoing-arg-byte ZP addresses are LIVE for
the duration of the arg writes. Concretely, for a Call at index
i with K preceding `Mov(_, ZP(addr_j))` arg writes, the
addresses {addr_0..addr_{K-1}} are added to live for those K
instructions. Body values whose interference range covers any
of those instructions must not pick those colors.

Verifiable: chapter sim corpus + a focused stress test (a
function that calls a leaf with many args while holding
caller-saved scratch).

### Step F6: leaf_zp ABI on by default in `--optimize-asm`; verify corpus

Wire ABI selection into the `--optimize-asm` pipeline.
Verification:
- Chapter sim corpus passes end-to-end.
- A diff-vs-baseline check confirms specific leaf functions
  collapse to bare RTS (e.g. functions in
  `tests/chapter_5/valid/` that take few args and no nested
  calls).
- A `tests/test_leaf_abi.py` that compiles representative
  programs and checks the output's prologue/epilogue
  presence.

### Step F7 (future): name-mangling for cross-TU enforcement

When c6502 grows multi-TU support, the annotation in headers
gives header-propagation enforcement: every TU including the
header sees the same annotation and uses the same ABI. To
add belt-and-suspenders link-time enforcement against a TU
that bypassed the header:

- A `zp_abi` function's symbol name encodes the ABI (e.g.,
  `foo` → `__zp_foo`).
- A reference to `foo` (without the annotation in scope) emits
  an unresolved external `foo`. The link fails. A reference
  to `foo` with the annotation in scope emits an unresolved
  external `__zp_foo`, matching the definition's mangled name.
- Address-of `foo` from a context that doesn't see the
  annotation references the unmangled name; the linker also
  fails to resolve it.

Out of scope for the current single-TU implementation.

---

## What this design deliberately leaves on the table

- **Inlining of leaf calls.** A small leaf function that's
  called from one site might as well be inlined; the ZP-ABI
  saves the call/return overhead but doesn't eliminate it.
  Inlining is a separate optimization; this design stacks
  cleanly with it.
- **Tail-call optimization.** A self-tail-call could replace
  the call with a jump back to the function's body, but the
  function would need to remain leaf-classified for that to
  fit this design — and self-tail-calls are non-leaf by
  construction. Out of scope.
- **Coalescing of arg-write Movs with the body's last def.**
  If the caller computes `arg1 = x + 1` and writes it to
  `ZP(addr1)`, the existing asm `Mov(Imm(1), Reg(A)); ADC
  ZP(x); STA ZP(addr1)` lowering already does this implicitly
  via the "Mov A → dst" terminal step of binary-op lowering.
  No new pass needed.
- **Variable-length arg fan-out.** Any leaf function whose
  total param byte count exceeds the pool falls back to
  SoftStackLayout. There's no "first 8 bytes in ZP, rest on
  stack" middle ground. Adding it would complicate the
  selection rule and the lowering, and the cliff doesn't seem
  costly enough to justify the complexity in the leaf-function
  case.

---

## Files at a glance (planned)

| File | Role |
|------|------|
| `passes/leaf_analysis.py` (F0) | Leaf classification over a TAC program. |
| `passes/abi_selection.py` (F1) | Computes per-function ParamLayout. |
| `tac_to_asm.py` (F2, F3) | Threading ABI through call-site lowering. |
| `passes/replace_pseudoregisters.py` (F3) | Resolves ZP-ABI params to ZP operands. |
| `passes/optimization_asm/interference.py` (F5) | Outgoing-arg-window liveness. |
| `compile.py`, `sim/harness.py` (F6) | Wiring. |
