# Five planned, two shipped: a c6502 optimization post-mortem

Another c6502 (C99-to-6502 compiler) session, this time on a 13-line
function called `step_pos`. The interesting thing about the session
wasn't what landed — two of five planned optimizations — but the
texture of why the other three are hard, and how much information
the compiler doesn't have at the moment it would need it.

## The function

`step_pos` is a leaf in the rescue-child state machine of a
Drol-style platformer. It decrements an animation counter, advances
a 16-bit world-X by 3 with carry, and tail-calls
`apply_bobble(slot, new_anim)`:

```c
__attribute__((zp_abi))
static void step_pos(uint8_t slot, uint8_t anim_in) {
    uint8_t new_anim = (uint8_t)(anim_in - 1);
    rescue_anim[slot] = new_anim;
    uint16_t world_x =
        ((uint16_t)entity_xoff_idx[slot] << 8) | entity_floor_col[slot];
    world_x = (uint16_t)(world_x + 3);
    entity_floor_col[slot] = (uint8_t)world_x;
    entity_xoff_idx[slot]  = (uint8_t)(world_x >> 8);
    apply_bobble(slot, new_anim);
}
```

The `__attribute__((zp_abi))` puts `slot` and `anim_in` in private
zero-page slots (so step_pos emits as bare-body + RTS, no frame
teardown). The hand-written reference is eleven instructions:

```asm
        SEC
        SBC #$01
        STA RESCUE_ANIM,Y               ; anim counter--
        TAX                              ; park new_anim in X
        CLC
        LDA ENTITY_FLOOR_COL,Y
        ADC #$03                         ; world-X += 3
        STA ENTITY_FLOOR_COL,Y
        LDA ENTITY_XOFF_IDX,Y
        ADC #$00                         ; carry into hi
        STA ENTITY_XOFF_IDX,Y
        ; falls through to apply_bobble
```

The compiler's first cut was 49 instructions. Four locals (`b0`,
`b1`, `b2`, `b3`), a `JSR apply_bobble; RTS` instead of the fall-
through, full LDA-then-STA marshalling for both call args. I made a
backlog of five gaps:

1. **Tail-call**: `JSR foo; RTS` → `JMP foo` (or fall-through).
2. **The 16-bit world_x temporary chain**: the `(uint16_t)hi << 8 |
   (uint16_t)lo` byte-construction idiom was producing four locals
   and a redundant `STA b2; LDA b2; STA b0` round-trip.
3. **ZP-arg coalescing**: at the call site to `apply_bobble`, both
   args (`slot`, `new_anim`) are forwarded directly from step_pos's
   own param/local slots. If callee's slots aliased the caller's,
   the marshalling LDA+STA pairs would collapse to self-Movs.
4. **Register calling convention for zp_abi**: pass the first 1-2
   args of zp_abi leaves in A/X/Y instead of ZP slots, skipping
   the inbound LDA entirely.
5. **Y as a holding register**: the hand version uses `TAX` to
   park `new_anim` across the world_x compute. Our regalloc spills
   it to ZP because slot is in X (for `,X` indexing) and `new_anim`
   has no HwReg hint.

#4 was off the table from the start — the user explicitly didn't
want a register-based ABI because registers are too scarce for
other things. Of the four that remained, two landed and two
deferred. Here's the texture.

## What shipped: tail-call

The peephole itself was 30 lines including the docstring:

```python
def apply_tail_call(prog):
    for fn in prog functions:
        i = 0
        while i < len(fn.instructions):
            if (i + 1 < len(fn.instructions)
                    and isinstance(fn.instructions[i], asm_ast.Call)
                    and isinstance(fn.instructions[i + 1], asm_ast.Return)):
                out.append(asm_ast.Jump(target=fn.instructions[i].name))
                i += 2
            else:
                ...
```

I only match the bare-RTS `Return` atom (no frame to tear down).
`Ret(arg_bytes, local_bytes, …)` carries an epilogue that has to
run before control leaves; I skip those.

What was NOT 30 lines was the fallout. Adding this peephole broke
80 chapter tests. The dead-store elimination pass treated `Call`
as opaque (it might read any memory) but `Jump` as ordinary
control flow. After the peephole, `JMP apply_bobble` no longer
looked opaque — its successor list was empty (no local label
named `apply_bobble`), so the DSE walk classified all four
upstream arg-marshalling `STA __zpabi_apply_bobble_p*` writes as
dead-at-exit and dropped them. `apply_bobble` then ran with
whatever stale bytes were already in its param slots.

The fix was one condition in `_is_dead_cfg`: treat a `Jump` to a
non-local label (i.e., a tail-called function name) as opaque,
same reasoning as `Call`. Caught by my sim-differential test on
the example.

The bigger lesson: a single-procedure assumption was baked into
the DSE pass — *every* `Jump` previously stayed inside the
function — and the peephole silently violated it. The dasm
output of both forms is `JMP`, but the liveness semantics
diverge sharply. Audit every flow analysis when you add inter-
procedural transfers.

## What shipped: world_x temporaries

The byte-construction idiom `(uint16_t)hi << 8 | (uint16_t)lo`
lowers to something like:

```
%9.lo = 0                    ; from (… << 8)
%9.hi = %8.lo = %7           ; from (… << 8)
%13.lo = %12                 ; from (uint16_t)lo
%13.hi = 0                   ; from (uint16_t)lo
%15.lo = %9.lo | %13.lo      ; from the OR
%15.hi = %9.hi | %13.hi
```

After the asm-SSA optimizer's forward + backward copy propagation
+ byte-DCE bracket settles, the OR is still there. Constant-fold
through `OR 0` doesn't happen at SSA level (only the post-coloring
`const_arith_fold` peephole knows the trick). And because the OR
sits in the middle of the copy chain, the asm-SSA coalescer can't
merge `%7` (the xoff byte) with `%15.hi` (the world_x high byte
that holds the same value) — there's an interfering op between
them.

The fix had two halves:

**Half 1**: a targeted asm-SSA-level pass `absorb_zero_load` that
folds `Mov(Imm(0), A); Or(X, A)` → `Mov(X, A)`. Same fold the
existing `const_arith_fold` peephole does, except `const_arith_fold`
ALSO drops `Or(Imm(0), A)` as an identity-after-write — and *that*
half is unsound at SSA level. The C99 `!` operator lowers to a
materialize-boolean-then-test where the BEQ has two CFG
predecessors (`LDA #1; .label:` fall-through vs `LDA #0; JMP
.label`), and the `ORA #0` between `.label:` and the BEQ re-sets
the Z flag for both paths. Dropping it at SSA-level broke 30
chapter_9 tests. Ship only the safe half at SSA, keep the unsafe-
in-multi-block-CFG half post-coloring.

**Half 2**: the asm-SSA coalescer's `_move_related_pairs` now
yields adjacent `Mov(P_a, A); Mov(A, P_b)` as a logical
Pseudo↔Pseudo copy. Before, the coalescer only saw explicit
`Mov(Pseudo, Pseudo)` copies (rare in the IR — almost everything
routes through A). With this extension, the OR-zero-absorbed copy
chain finally produces direct pairs the coalescer can merge.

Net effect on step_pos: one fewer ZP slot (`b3` → 3 locals
total), two atoms saved (the `LDA b2; STA b0` round-trip). The
function went 49 → 45 lines.

## What deferred: ZP-arg coalescing

At the call site, step_pos emits:

```asm
   LDA   __zpabi_step_pos_p0       ; slot at $80
   STA   __zpabi_apply_bobble_p0   ; bobble's slot at $82
   LDA   __local_step_pos_b2       ; new_anim at $86
   STA   __zpabi_apply_bobble_p1   ; bobble's anim at $83
   JMP   apply_bobble
```

If the ZP allocator could put `__zpabi_apply_bobble_p0` at `$80`
and `__zpabi_apply_bobble_p1` at `$86`, the four lines would
become self-Movs (LDA $80; STA $80) and drop at emit. Four atoms,
gone.

The deferral was about scope, not difficulty per se. The shape of
the change is well-defined: detect "direct forward" calls where
the caller's param X is dead after the call AND fed unchanged into
the callee's slot Y; record an aliasing preference; let the global
ZP allocator try to honor it. But:

* The callee's slot address is **global** — it has to satisfy
  every caller's preferences, not just one. If `apply_bobble` is
  called from N places, the aliasing decision has to be a single
  consistent choice across all of them.
* In single-TU compiles `apply_bobble` is declared `extern`. The
  compiler emits the EQU for its slot symbol so the linker resolves
  it consistently across TUs. If we choose a non-default address
  to satisfy step_pos's preference, every other TU's calls to
  `apply_bobble` go to the same address. Fine if no conflict —
  but we can't see the other TUs in this compile.
* The current allocator's invariant is "callee's slots are
  disjoint from caller's slots, because both are live on the
  stack simultaneously". Relaxing this to "disjoint UNLESS the
  caller's slot is dead at the call site, in which case alias"
  requires per-callsite liveness analysis, not just the static
  call graph the allocator currently uses.

It's a real piece of engineering. Not for this session.

## What deferred: Y as holding register

The hand version uses `TAX` to park `new_anim` across the world_x
compute. The compiler spills it to a ZP slot (`b2`) because slot
is pinned to X (the `,X`-indexing hint) and new_anim has no
HwReg hint.

I tried adding an opportunistic Y-pin loop: after the hinted
pin runs, walk every eligible-but-not-hinted Pseudo and try to
park it in Y (X if Y is taken). Worked on step_pos. Broke 100+
tests in functions that use a soft-stack frame.

The bug surfaces in `_can_pin`'s clobber-in-live-range check. It
looks only at *explicit* writes to X/Y in the IR — `Mov(_, Reg(Y))`,
`LDY #imm`, `INY`, `DEY`. It does NOT model the *implicit* `LDY
#off` that operand lowering emits for every `Frame` / `Stack` /
`Indirect` access. At regalloc time those accesses are still
Pseudos — the regalloc itself is what decides whether a Pseudo
becomes ZP-resident or Frame-spilled. So scanning operand types
at regalloc time can't predict which Pseudos will spill, and
therefore can't predict which ones will introduce implicit `LDY`
clobbers.

I tried gating the opportunistic pin on "the function uses a
private local pool" (= eligible zp_abi-like). That STILL didn't
work, because `continue.c`'s `main` has 6 bytes of locals into a
5-byte private pool — one byte spills to Frame, and the spill
introduces the (FP),Y access that corrupts the Y-pinned value.

The fix would be one of:
* A second pass after coloring that promotes only Pseudos whose
  live-range neighbors all got ZP/HwReg colors (no Frame
  neighbors). Re-runs pinning given perfect spill knowledge.
* A pre-coloring counting pass that statically proves "this
  function's Pseudos all fit in the private pool, no spills
  possible".

Both are more than I wanted to introduce in this session, given
that the gain is roughly "save one ZP byte per `new_anim`-style
short-lived intermediate". It'd add up across the example corpus
but each individual case is small.

Deferred. Memory note saved for the next session that ventures
near the regalloc.

## Final asm

Step_pos went from 49 lines to 45. Five lines below the
hand-written reference would have required the world_x compute
to also recognize "the source and destination are the same
memory cell" and emit an in-place `LDA col,X; CLC; ADC #3; STA
col,X` ADC chain. That's another deferred optimization —
rematerialization of indexed loads — and I didn't even start it
this session.

Two of five landed. The other two have memory notes that say
exactly why they're hard. That's a real outcome for an evening
of work, even if it doesn't look as good as the headline number
would have if I'd hit all five.

It's also a reminder that "I've sketched the optimization in my
head" is very different from "I've satisfied every invariant the
existing passes assumed in the absence of my change." The DSE
single-procedure assumption. The const_arith_fold's basic-block-
local flag assumption. The regalloc's "Frame appears post-
allocation" gotcha. None of them were documented as load-bearing
invariants. All of them broke when I introduced the obvious
extension. Future me, leaving notes for present me.

## Memory notes saved

* Post-coloring peepholes that drop flag-setting instructions can
  be unsound at asm-SSA level. Split into safe / unsafe halves
  before relocating.
* At asm-SSA / regalloc time, all locals are still Pseudos.
  Frame / Stack / Indirect operands appear only AFTER
  `replace_pseudoregisters` — so the regalloc can't predict
  spills by scanning operand types.

Both with `Why:` and `How to apply:` sections in
`memory/project_*.md` for future sessions.
