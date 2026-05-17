# Dual-index promotion (X + Y for two simultaneous indices)

Design proposal — not yet implemented. Motivated by hand-written
asm comparison on `examples/apply_bobble.c`. Documented here to be
picked up as a follow-up after the TAC sinker + ADC commutativity
peephole land (which they have, this session).

## Motivating shape

`apply_bobble` takes two uchar params:

- `slot` — index into `entity_floor_pos[20]`.
- `bobble_idx` — index into `rescue_bobble[7]`.

Both are used as IndexedData indices, never together at the same
instruction, but both alive across the JumpIfMasked.

Current asm (post-sinker, post-ADC-commute, this session's ship):

```
   LDX   __zpabi_apply_bobble_p1     ; X = bobble_idx
   LDA   rescue_bobble,X
   STA   __local_b0
   BPL   .if_else@1
.add_path:
   AND   #$7F
   LDX   __zpabi_apply_bobble_p0     ; X = slot   <-- reload
   CLC
   ADC   entity_floor_pos,X
   STA   entity_floor_pos,X
   JMP   .if_end@0
.if_else@1:
   LDA   __local_b0
   AND   #$7F
   STA   __local_b1
   LDX   __zpabi_apply_bobble_p0     ; X = slot   <-- reload
   LDA   entity_floor_pos,X
   SEC
   SBC   __local_b1
   STA   entity_floor_pos,X
.if_end@0:
   RTS
```

Hand-written equivalent pins `bobble_idx` to X and `slot` to Y at
function entry; the two `LDX __zpabi_apply_bobble_p0` reloads
become a single `LDY __zpabi_apply_bobble_p0` and the
`entity_floor_pos,X` becomes `entity_floor_pos,Y`:

```
   LDX   __zpabi_apply_bobble_p1     ; X = bobble_idx
   LDY   __zpabi_apply_bobble_p0     ; Y = slot       <-- ONCE at entry
   LDA   rescue_bobble,X
   STA   __local_b0
   BPL   .if_else@1
.add_path:
   AND   #$7F
   CLC
   ADC   entity_floor_pos,Y          ; abs,Y instead of abs,X
   STA   entity_floor_pos,Y
   JMP   .if_end@0
.if_else@1:
   LDA   __local_b0
   AND   #$7F
   STA   __local_b1
   LDA   entity_floor_pos,Y          ; abs,Y instead of abs,X
   SEC
   SBC   __local_b1
   STA   entity_floor_pos,Y
.if_end@0:
   RTS
```

Per-branch savings: one `LDX __zpabi_apply_bobble_p0` (3 bytes / 3
cycles per branch). On `apply_bobble` specifically the cycle math
is neutral (one LDY at entry replaces one LDX per branch — only
one branch runs per call), but **code size shrinks by 3 bytes**
and the pattern generalizes to functions where the same index is
used many times within a branch.

## Why it doesn't fall out of the existing infrastructure

The asm-SSA round-trip's HwReg coloring (`hwreg_eligibility.py` +
`regalloc.color_graph`) only sees **Pseudos**, not Data operands.
zp_abi params arrive at the asm IR pre-resolved as
`Data("__zpabi_<fn>_p<k>", 0)` — they never pass through the
regalloc that could pin them to X or Y.

`loop_counter_to_x.py` promotes Pseudos to `Reg(X)` for a specific
loop-iv shape; it doesn't apply to non-loop indices, and it
doesn't promote to Y.

## Where it should live

A new asm pass `passes/data_to_y_promotion.py`, running:

- AFTER `replace_pseudoregisters` (operands are concrete; we can
  see which Data references resolve to ZP via the slot symbols).
- AFTER `loop_counter_to_x` (so X-pinned ranges are already
  established and we know which IndexedData ops use X).
- BEFORE `expand_long_branches` (the pass shrinks code, never
  expands, so no new branches need long-range expansion).
- INSIDE the peephole fixed-point loop — the rewrite is purely
  local and asm_dead_store may clean up the now-orphaned LDX
  instructions on the next sweep.

## Algorithm sketch

For each function:

1. **Collect candidates.** Walk the function; for each
   `Mov(Data(<param>, 0), Reg(X))` instance, record `<param>`. A
   "candidate" is any `<param>` that appears as the source of two
   or more such LDX instances AND is a ZP-resolvable symbol
   (`__zpabi_*` / `__local_*`).

2. **Eligibility gate.** A candidate qualifies for Y-promotion iff:

   - The function uses Reg(Y) **nowhere** — no `LDY`, no
     `Mov(_, Reg(Y))`, no `IndexedData(_, _, Y)` index, no
     `Indirect`/`IndirectY` (those consume Y), no `Compare`
     with Reg(Y), etc. (Y is unused so we can pin it.)
   - The function has at least one **other** IndexedData index
     that uses X — i.e., X is already taken by some other live
     value. (Otherwise just leaving X for `<param>` is fine and
     this pass does nothing.)
   - Between any pair of LDX of `<param>` and the matching
     IndexedData use of X, X isn't modified by anything else —
     i.e., the LDX-then-use chain is local. (Conservative.)

3. **Rewrite.**

   - Insert `Mov(Data(<param>, 0), Reg(Y))` (i.e., `LDY <param>`)
     at the start of the function body, immediately after the
     `FunctionPrologue` (or at index 0 if there's none). For
     zp_abi/bare-exit functions this lands at the top.
   - For each occurrence of `Mov(Data(<param>, 0), Reg(X))`
     followed by IndexedData(_, _, X) uses, drop the LDX and
     rewrite the IndexedData operands' `index` from `X()` to
     `Y()`. The 6502 supports `abs,X` and `abs,Y` symmetrically
     for LDA / STA / ADC / SBC / AND / ORA / EOR / CMP, so the
     rewrite is encodable.

4. **Cleanup.** Subsequent peephole iterations (`asm_dead_store`,
   `redundant_load_elimination`) clean up any now-redundant
   loads.

## Eligibility refinements (post-prototype)

- Allow Y to be used in narrow ranges — e.g., a transient `LDY
  #<imm>` for an indirect-Y access could be re-staged before the
  access if the pinned-Y value is needed afterward. Adds
  bookkeeping; defer until a motivating case appears.
- Cost-model the choice: prefer the param with more uses to be
  Y-pinned. The Data operand that appears in the most
  IndexedData index positions wins. Today's prototype just picks
  the first candidate.
- Cross-function: if multiple eligible candidates exist (3+
  params used as indices), pin two to X and Y; pick the third
  through the normal reload path. The 6502 only has 2 index regs,
  so anything beyond 2 falls back.

## Why not extend `hwreg_eligibility`?

`hwreg_eligibility` operates on Pseudos and runs INSIDE the
asm-SSA round-trip. Extending it to also consider Data operands
would muddy the abstraction: Pseudos are renameable storage that
the regalloc assigns colors to; Data is link-time-named storage
with a fixed identity. A separate late pass keeps the concerns
clean.

It would also force ordering changes: zp_abi params today resolve
to Data BEFORE the SSA round-trip even sees them. To make them
Pseudos visible to regalloc, the whole pipeline would have to
defer the zp_abi resolution. That's a much bigger refactor than a
focused late pass.

## Tests

- `tests/test_data_to_y_promotion.py` — unit tests against
  hand-built asm IR shapes. Cover:
  - The apply_bobble shape: two zp_abi params, both used as
    IndexedData indices, one promoted to Y.
  - Y already in use: pass should bail.
  - Only one index used: pass should bail (no win).
  - Multi-use of a param: pass should pick the right one.
- Gold-file output diff on `examples/apply_bobble.asm` (the
  expected post-promotion shape is the hand-written reference
  above).
- Sim differential (`tests/test_apply_bobble_sim.py`) must
  continue to pass — opt vs unopt agreement is the soundness
  oracle.

## Estimated effort

~3 days, plus tests and a sim-differential pass over the full
example corpus to catch any function where Y was being implicitly
relied upon (the eligibility gate should catch these, but a
corpus sweep is the integration safety net).
