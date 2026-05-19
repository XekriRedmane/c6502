# Dead sign-extension residue: chasing weird code in a 6502 C compiler

Quick story about an inefficiency that looked like a miscompile.
I'm writing a C99 compiler (c6502) that targets the 6502. A
function I was inspecting had this asm for what looked like
trivial loop code:

```asm
.loop@0_start:
   TXA
   BMI   .loop@0_break
.find_active_entity@asm_ssa_block@0:
   TXA
   BMI   .sx_neg@0
.find_active_entity@asm_ssa_block@1:
   JMP   .sx_done@1
.sx_neg@0:
.sx_done@1:
   LDA   entity_hit_state,X
   ...
```

The duplicated `TXA; BMI` jumped out. So did the
`.find_active_entity@asm_ssa_block@N` labels — nothing in the
function targeted them. And the `.sx_neg@0` / `.sx_done@1` pair
had no body between them: both BMI and JMP eventually landed at
the same instruction.

The C source was nothing fancy:

```c
for (int8_t i = (int8_t)hit_max; i >= 0; i--) {
    if ((entity_hit_state[i] & 0x80) == 0) {
        ...
    }
}
```

Not a miscompile — residue from a dead sign-extension. Here's
where it came from.

## Where the dead code originated

C99 §6.3.1.1 integer-promotes a `signed char` (or `int8_t`) to
`int` whenever it participates in an arithmetic / subscript /
bitwise context. So `entity_hit_state[i]` (with `i` as `int8_t`)
requires sign-extending `i` to `int` (2 bytes on c6502) before
the indexed access.

The compiler's sign-extension lowering emits a 7-step sequence:

```asm
copy src.byte0 → dst.byte0   ; X has i already, dst.byte0 = i
Mov src.high → A             ; TXA — load N flag from i
Branch MI .sx_neg
LDA #$00                     ; positive case
JMP .sx_done
.sx_neg:
LDA #$FF                     ; negative case
.sx_done:
STA dst.byte_1               ; store high byte
```

The 6502's absolute,X addressing (`LDA arr,X`) reads only the
index's low byte; the high byte the sign-extension produced is
genuinely never observed by the array access.

Byte-granular dead-code elimination handled the data parts:

- `STA dst.byte_1` dropped (no reader).
- `LDA #$00` and `LDA #$FF` dropped by dead-A-arith (A's value
  is now dead).

What got left behind was the *control flow*:

```asm
TXA              ; flag setter — Branch reads N
BMI .sx_neg      ; both branches converge at sx_done
JMP .sx_done
.sx_neg:
.sx_done:
```

The N flag is "live" — the Branch reads it. So `dead_a_arith`
can't drop the `TXA`. The JMP targets `.sx_done` which is a
label with no body before the next code; the BMI targets
`.sx_neg` which is right next to `.sx_done`. Both paths reach
the same instruction. The branch is effectively dead, but no
peephole was catching that.

`apply_branch_invert` would have caught it (`Branch + Jump +
Label` → inverted Branch), but it requires the three to be
*consecutive*. The compiler's asm-SSA construction emits per-
block markers (`.find_active_entity@asm_ssa_block@N`) for SSA
reasoning; one of them sits between the Branch and the Jump,
preventing the consecutive-pattern match.

## The fix — two TAC-level changes

**1. Recognizer relaxation.** The c6502 optimizer has a
`recognize_indexed_load` pass that collapses
`ZeroExtend(uchar) + Add(C) + Load` into a direct
`IndexedConstLoad(C, uchar_index, dst)`. It only accepted
`ZeroExtend`, not `SignExtend`. Relaxed to also accept
`SignExtend` from a 1-byte source, under the C99 §6.5.6
soundness reasoning: negative array indices are undefined
behavior, and on the 6502 the absolute,X addressing mode
observes only the low byte — so treating the sign-extended
value as a zero-extended one is correctness-preserving for
well-defined C.

**2. New TAC peephole `fold_truncate_extend`.** After the
recognizer rewrites the absolute,X chain, it narrows the index
back to 1 byte via a `Truncate`. So the TAC ends up with

```
SignExtend(i, %ext)
Truncate(%ext, %narrow)
IndexedLoad(arr, %narrow, _)
```

— a sign-extend-then-truncate round-trip. The new peephole
recognizes this and rewrites the Truncate to read `i` directly
(as a Copy when widths match, narrower Truncate when narrower).
SSA-DCE then drops the SignExtend since `%ext` no longer has
readers.

**Soundness gate.** The fold compares `Var.name in ssa_dsts`
before using a def-idx lookup. Globals, statics, and address-
taken locals have multiple defs; without this gate I shipped a
sim-diff regression on `chained_casts.c` where the rewrite
dropped a read of a global because a *later* assignment to the
same global was treated as the "reaching def."

**3. (Bonus) `apply_dead_label_drop`.** The SSA-construction
labels were the third part of the residue. Added a ~70-line
peephole that scans for Labels whose name isn't in any
`Jump.target` / `Branch.target` / `PhiArg.pred_label` and drops
them. Pure noise removal; also unblocks `apply_branch_invert`
when an orphan label was the only thing keeping `Branch / Jump /
Label` non-consecutive.

## The result

```asm
find_active_entity:
   LDX   __zpabi_find_active_entity__hit_max
.loop@0_start:
   TXA
   BMI   .loop@0_break
   LDA   entity_hit_state,X
   BMI   .if_end@0
   LDA   entity_hit_row,X
   SEC
   SBC   #$08
   LDY   #$00
   STA   (__local_player_catch__1),Y
   LDA   #$01
   RTS
.if_end@0:
   DEX
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$00
   RTS
```

19 instructions, down from 35. Across the example corpus the
dead-label drop alone removed ~143 lines.

## What I'd tell past-me

If your compiler emits a cast lowering that produces an
expensive byte the consumer doesn't read, DCE will handle the
data part but the control flow can be stuck — especially if
intervening "marker" labels keep peephole patterns from
matching. Walking the asm with the source side-by-side surfaces
these quickly. Most look like miscompiles; almost always
they're residue.

Repo: <https://github.com/XekriRedmane/c6502>
