# Asm loop-invariant store hoisting

`apply_licm` in [passes/asm_licm.py](../../passes/asm_licm.py) moves
a store that writes the same value on every iteration of a loop to
just before the loop. It runs once, after
[prologue synthesis](prologue_synthesis.md) and before the first
peephole fixed point, under `--optimize` only. Inside each function
it repeats until nothing more moves, trying the innermost loops
first, so a store hoisted out of an inner loop can then leave the
loop around it.

The pass moves three shapes, each a store into a `Data` or `ZP`
cell. They are `Mov(Imm, M)`, `LDA #c; STA M`, and `LDA src; STA M`
where `src` is a `Data` or `ZP` cell the loop doesn't write. The
third is what `tac_to_asm` emits to stage a pointer in `DPTR` before
a `(DPTR),Y` access, which it repeats on every iteration even when
the pointer doesn't change. That staging is emitted by `tac_to_asm`,
after the TAC-level [LICM](licm.md) has run. Each `LDA abs; STA zp`
pair that leaves the loop saves 7 cycles per iteration.

## Example

```c
volatile unsigned char *port;

void blink(unsigned char n) {
    while (n--) {
        *port = 1;
    }
}
```

### Before

```
blink:
   SUBROUTINE

.blink@asm_ssa_preheader@0:
.blink@ssa_preheader@0:
   LDA   __zpabi_blink__n
   STA   __local_blink__n
.loop@0_continue:
   LDA   __local_blink__n
   SEC
   SBC   #$01
   STA   __local_blink__0
   LDA   __local_blink__n
   BEQ   .loop@0_break
.blink@ssa_block@0:
   LDA   port
   STA   DPTR
   LDA   port+1
   STA   DPTR+1
   LDA   #$01
   LDY   #$00
   STA   (DPTR),Y
   LDA   __local_blink__0
   STA   __local_blink__n
   JMP   .loop@0_continue
.loop@0_break:
   RTS
```

### After

```
blink:
   SUBROUTINE

.blink@asm_ssa_preheader@0:
.blink@ssa_preheader@0:
   LDA   __zpabi_blink__n
   STA   __local_blink__n
   LDA   port
   STA   DPTR
   LDA   port+1
   STA   DPTR+1
.loop@0_continue:
   LDA   __local_blink__n
   SEC
   SBC   #$01
   STA   __local_blink__0
   LDA   __local_blink__n
   BEQ   .loop@0_break
.blink@ssa_block@0:
   LDA   #$01
   LDY   #$00
   STA   (DPTR),Y
   LDA   __local_blink__0
   STA   __local_blink__n
   JMP   .loop@0_continue
.loop@0_break:
   RTS
```

The store through `port` is volatile and stays in the loop. The load
of `port` itself is an ordinary read, since `port` is a pointer to
volatile data and isn't volatile itself.

A constant store moves the same way. In a `do`-`while` loop that
starts with `mode = 3;`, the pass moves `LDA #$03; STA mode` from
just after the loop's label to just before it.

## When it applies

A loop is a `Jump` or `Branch` to a label earlier in the function,
and its body is every instruction from that label to the branch. A
store is moved when all of these are true.

- No instruction outside the body branches to a label inside it
  other than the loop's first label, so the loop has one entry.
- The body contains no `Call`. A callee could write the cell, and a
  zp_abi callee can use its parameter slots as scratch.
- The store is the only instruction in the body that writes its
  destination as a `Data` or `ZP` operand. For `LDA src; STA M`, no
  instruction in the body writes `src` that way either. Writes
  through a pointer or with an index register aren't counted, so a
  store through a pointer to the same cell doesn't stop the move.
  For `g = 5; out = g; *p = 7;` in a loop with `p == &g`, the pass
  moves `g = 5` out, and from the second iteration on `out` gets 7
  where it should get 5. That's a miscompile.
- No instruction in the body reads the destination before the store
  has run, on any path from the loop's first label.
- At every exit from the loop that can be taken before the store
  runs, the destination is dead, and no such path reaches a return.
- A and the N and Z flags are dead both at the loop's first label,
  where the moved `LDA` now runs, and just after the store's old
  position. A load or a store leaves C and V alone.
- Neither instruction is volatile.

## What it leaves alone

- A store in a loop whose test comes first, such as a `for` loop the
  TAC optimizer didn't rotate. The loop may run zero times, and the
  destination would then hold the hoisted value where it should hold
  the old one. `mode = 3;` at the top of
  `for (unsigned char i = 0; i < n; i++)` stays in the loop.
- Any store in a loop that calls a function. With `tick();` added to
  the loop above, the `DPTR` staging stays in the loop.
- A volatile access, which has to happen once per iteration.
- A store of a register other than A, and any store whose value
  comes from arithmetic.
- A loop with a second entry, such as a `goto` into its middle.

## Related

[LICM](licm.md) moves loop-invariant computations at the TAC level.
