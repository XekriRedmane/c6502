# Retiring mem-to-mem at the IR level

A follow-up to
[r6502/018](https://reddit.com/r/6502). That post described a
narrow peephole I'd added to my C99-targeting-6502 compiler
([c6502](https://github.com/XekriRedmane/c6502)) to handle one
flavor of "the implicit LDA inside a mem-to-mem IR atom is
redundant." The follow-up to that post was the user asking
"isn't there something more general?" — and there was. This is
that.

## The setup

The compiler's asm IR has long allowed `Mov(mem_src, mem_dst)`
as a single atom. The 6502 has no `MOV mem, mem` opcode, so the
emitter lowered the atom to `LDA src; STA dst` using A as the
staging register. One IR atom → two 6502 opcodes, with A
silently clobbered.

That compound shape was opaque to every instruction-stream
peephole. The asc_floor case the user
[asked about](https://reddit.com/r/6502/comments/N/) was:

```c
beam_seed_floor = asc_floor;
floor_mirror    = asc_floor;
dsc_floor       = asc_floor;
```

Lowering to:

```asm
LDA __zpabi_do_ascend__asc_floor
STA beam_seed_floor
LDA __zpabi_do_ascend__asc_floor   ; redundant
STA floor_mirror
LDA __zpabi_do_ascend__asc_floor   ; redundant
STA dsc_floor
```

The redundant-load tracker (CFG-aware A-tracker, knows `A == M`
after `STA M`) couldn't see those LDAs as atoms — they were
hidden inside the mem-to-mem Movs the IR was emitting.

## Three options

1. **Add another narrow peephole** for this specific shape.
   Same approach as the previous post. Linear in the number of
   patterns I'd find. Doesn't scale.

2. **Extend the A-tracker to rewrite mem-to-mem sources.** The
   tracker already knows `state.a == M` at the right points;
   teach it to rewrite `Mov(M, dst_mem)` to `Mov(Reg(A), dst_mem)`
   when M is in state.a. General — catches arbitrary-distance
   and cross-block cases.

3. **Retire the compound shape entirely.** Split every
   `Mov(mem, mem)` into the `LDA src; STA dst` pair it emits as,
   at IR level. Every downstream pass then sees explicit atoms
   and applies its normal logic. No carve-outs.

I went with option 3.

## The pass

```python
def apply_split_mem_to_mem(prog):
    for fn in prog:
        for instr in fn.instructions:
            if isinstance(instr, Mov) and is_mem(instr.src) and is_mem(instr.dst):
                if instr.is_volatile:
                    keep                              # see below
                elif instr.src == instr.dst:
                    drop                              # mirrors emit-time peephole
                else:
                    emit Mov(instr.src, Reg(A))       # LDA src
                    emit Mov(Reg(A), instr.dst)       # STA dst
            else:
                keep
```

The whole pass is ~30 lines of actual logic.

## Two things that go wrong

I expected the split to drop straight in. It didn't — two
regressions surfaced on existing examples.

**Regression 1**: `sfx_tone.asm` grew by 1 line. The function
has `volatile uint8_t y = pitch;` inside an inner timing loop.
The split was turning the volatile mem-to-mem into two atoms,
both inheriting `is_volatile=True` because the IR's volatile
flag is one bit per Mov atom — it says "this Mov touches a
volatile cell" without saying which operand. The newly-explicit
LDA half got marked volatile and `redundant_load_elimination`
(which never drops volatile loads) couldn't elide it.

Fix: skip volatile mem-to-mem in the split. The existing
volatile branch in `redundant_load._update_for_mov` already
handles the compound form correctly (it adds the
presumed-non-volatile src to `state.a`).

**Regression 2**: `companion_update.asm` grew by 13 lines. Two
patterns broke:

(a) `STX __zpabi_callee__slot` became `TXA; STA __zpabi_callee__slot`
in 10+ places. Cause: `passes/x_save_slot_load.py`'s Pass 3 had
a quietly-mem-to-mem-aware rewrite — when M was an X-save slot,
it rewrote `Mov(M, D)` (mem-to-mem) directly to `Mov(Reg(X),
D)` (= STX D). After the split, only the LDA half (`Mov(M, A)`)
got rewritten to TXA; the orphaned STA stayed.

Fix: new peephole `apply_via_a_store_fold` — `Mov(Reg(X),
Reg(A)); Mov(Reg(A), Data|ZP) → Mov(Reg(X), Data|ZP)` when A
and flags are dead at the next instruction.

(b) An `LDA companion_state,X; BMI .lb_skip` became `LDA
companion_state,X; STA __local; AND #80; BNE .lb_skip`. Cause:
`apply_and_sign_bit_branch` looks for a 3-instruction window
`LDA M; AND #80; B(EQ|NE)` and rewrites to `LDA M; B(PL|MI)`.
Its `_is_lda_to_a` predicate explicitly accepted mem-to-mem as
an LDA-shape atom. After the split, the STA between the LDA and
the AND breaks the 3-instr adjacency.

Fix: extend `apply_and_sign_bit_branch` with a 4-instruction
variant that tolerates an intermediate `Mov(Reg(A), <mem>)` —
STA preserves both A and N/Z, so the fold's soundness argument
is unchanged.

After both fixes, both examples shrink relative to the original
gold: `do_ascend.asm` 83 → 81 lines, `companion_update.asm` 740
→ 739 lines. 2690 tests pass.

## The lesson

When the option-3 design hit "let's try it," the work I'd
estimated as "one pass, ~30 lines" actually involved two
additional peephole extensions, an investigation of the IR's
volatile-bit semantics, and re-blessing two gold files. None of
it was hard; the part I missed was that two existing passes had
silently been pattern-matching on the compound form for years.

Both dependencies were findable in advance with a grep for `mov.src
... mov.dst` patterns in `passes/`. I didn't run that grep. If
you're considering a similar IR-shape retirement on a similar
compiler, do.
