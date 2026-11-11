# Saves around a recursive call

`plan_saves` in
[passes/recursion_saves.py](../../passes/recursion_saves.py) decides
which values a recursive function saves on the hardware stack around
each call that can recurse, and `tac_to_asm` emits the `PHA` and
`PLA` instructions. It applies only to calls under `#pragma c6502
call save(hardware_stack)`, and only under `--optimize`.

A function on a cycle of calls normally can't have a [private local
pool](zp_local_pools.md): the call that recurses runs the same code
and would overwrite the bytes the outer call still needs. So it uses
the shared split of zero page instead. A value live across a call
goes in a callee-saved byte at `$C0`-`$FF`, and the function saves
that byte in its prologue and restores it in its epilogue. It pays
for this on every call, through a soft-stack frame it may not
otherwise need, even on the calls that don't recurse.

The pragma marks the calls in the statement or declaration after
it. It promises
that the hardware stack is deep enough for the bytes the caller
pushes there, which the compiler can't check, since it can't tell
how deep the recursion goes. When every call that stays on the cycle
is marked, the functions on the cycle get private pools, and the
caller saves each value live across a marked call itself:

```
   LDA   v          ; just after v's last definition in the call's
   PHA              ; block, or else just before the call
   JSR   f
   PLA              ; in the reverse order, after the call's result
   STA   v          ; is stored
```

Nothing in the pool is live across the call, so nothing needs saving
at entry or exit. Two refinements make the common case cheap.

- A value whose last definition copies it from somewhere is pushed
  from there. `v = g; LDA v; PHA` becomes `LDA g; PHA`, and the copy
  into `v` is then dead.
- A value whose first use after the call copies it to a global is
  pulled straight into the global, `PLA; STA g`, when nothing in
  between can see the global.

## Example

```c
unsigned char level;
unsigned char deepest;
unsigned char remaining;

void descend(void) {
    unsigned char here = level;
    if (here > deepest) deepest = here;
    if (remaining != 0) {
        remaining = remaining - 1;
        level = here + 1;
#pragma c6502 call save(hardware_stack)
        descend();
        level = here;
    }
}
```

The "Before" listing is the same `--optimize` compile without the
pragma.

### Before

```
descend:
   SUBROUTINE

   ; prologue: 0 arg bytes, 1 local bytes, 1 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$02
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1
   LDA   $C0
   LDY   #$01
   STA   (FP),Y

   LDA   level
   STA   $C0
   LDA   deepest
   CMP   $C0
   BCS   .if_end@0
   LDA   $C0
   STA   deepest
.if_end@0:
   LDA   remaining
   BEQ   .if_end@1
   DEC   remaining
   LDA   $C0
   CLC
   ADC   #$01
   STA   level
   JSR   descend
   LDA   $C0
   STA   level
.if_end@1:

   ; epilogue
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   CLC
   LDA   FP
   ADC   #$03
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
```

### After

```
__local_descend__here	EQU	$81

descend:
   SUBROUTINE

   LDA   level
   STA   __local_descend__here
   LDA   deepest
   CMP   __local_descend__here
   BCS   .if_end@0
   LDA   __local_descend__here
   STA   deepest
.if_end@0:
   LDA   remaining
   BEQ   .if_end@1
   DEC   remaining
   LDA   __local_descend__here
   CLC
   ADC   #$01
   STA   level
   LDA   __local_descend__here
   PHA
   JSR   descend
   PLA
   STA   level
.if_end@1:
   RTS
```

`here` is live across the call, so it's pushed just before it. Its
definition is in an earlier block, so it's pushed from its pool byte.
After the call it's pulled straight into `level`. `descend` goes from
111 bytes to 44.

In `examples/zork_print_string_literal.c`, `print_zstring` saves the
string decoder's state in locals before the call that prints an
abbreviation, and restores it after. With the pragma the locals
disappear, and the code is the same as the original Zork's: `LDA g;
PHA` for each byte before the call, and `PLA; STA g` after it.

## When it applies

- The call is under `#pragma c6502 call save(hardware_stack)`. The
  pragma has to come right before an expression statement, a
  `return`, or a declaration, and every call in it has to be one
  that can recurse, a direct call to a function on the caller's
  cycle. Anything else is an error.
- Every call from one function on the cycle to another is marked. A
  cycle with an unmarked call stays as it was, and its marks do
  nothing.
- The functions on the cycle are otherwise eligible for [private
  local pools](zp_local_pools.md): no call through a pointer, and
  every other function they call has a pool too.

A value is saved when it's live across the call and could be in the
function's pool. Globals, statics, parameters (which are in the
soft-stack frame, one copy per call), and locals whose address is
taken (which stay in the frame for the same reason) aren't saved.

## What it leaves alone

- Recursion without the pragma, which keeps its callee-saved bytes.
- The unoptimized compile, where every function saves its own
  callee-saved bytes. The pragma is accepted and ignored.
- Parameters. A recursive function still takes its parameters on the
  soft stack, since a zp_abi parameter would be overwritten by the
  arguments of the call that recurses.
- A marked call's own result. `PLA` changes A, so when there are
  values to pull, the result is stored before them, and a carry-flag
  result is no longer branched on straight after the `JSR`.

Each call in progress uses 2 bytes of the hardware stack for its
return address plus the bytes it saves. `print_zstring` saves 7, and
an abbreviation can't contain another, so it goes two calls deep.

## Related

[Pragmas](../pragmas.md) is the user guide to the pragma, with the
cautions about the hardware stack's depth.

[Private local pools](zp_local_pools.md) gives the functions on the
cycle their pools, and [byte-granular register
allocation](regalloc.md) colors from them. The asm optimizer checks
that nothing it colors is live across a marked call.
