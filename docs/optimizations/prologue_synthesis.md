# Prologue synthesis

`synthesize_program` in
[passes/prologue_synthesis.py](../../passes/prologue_synthesis.py),
which `compile.py` imports as `synthesize_prologue`, adds a soft-stack
prologue and epilogue to a function only when the function ends up
needing a frame. It runs once, after `replace_pseudoregisters_bare_exit`
and the [LoadAddress split](lower_data_load_address.md), and before
[asm LICM](asm_licm.md) and the peephole fixed point. It runs only
under `--optimize`. The unoptimized pipeline gets its prologue from
`replace_pseudoregisters.replace_program`, which gives every function
one, and `asm_to_asm2` drops it for a function with no argument or
local bytes.

Under `--optimize`, `tac_to_asm` runs in bare-exit mode. It emits no
`FunctionPrologue`, and it ends each return path with a bare
`Return`, which lowers to `RTS`. Which functions get a frame isn't
known until register allocation has run and
`replace_pseudoregisters_bare_exit` has resolved every pseudoregister.
That pass returns the frame's dimensions for each function, which are
the soft-stack argument bytes, the frame-resident local bytes, and
the callee-saved zero-page bytes to preserve. When all three are
zero, this pass leaves the function as it is, a body that ends in
`RTS`. Otherwise it puts a `FunctionPrologue` at the top and turns
every `Return` into a `Ret` with the same dimensions. The prologue in
the example below is 32 bytes, and each epilogue is 29.

## Example

```c
unsigned char count_down(unsigned char n) {
    if (n == 0) return 0;
    return 1 + count_down(n - 1);
}

unsigned char main(void) { return count_down(5); }
```

`count_down` is recursive, so it uses the soft stack, and `n` arrives
at `FP+3`. The lines between `.if_end@0:` and the recursive `JSR` are
dropped from both listings.

### Before

```
count_down:
   SUBROUTINE

.count_down@asm_ssa_block@0:
   LDY   #$03
   LDA   (FP),Y
   BNE   .if_end@0
.count_down@asm_ssa_block@1:
   LDA   #$00
   RTS
.if_end@0:
...
   JSR   count_down
   STA   $80
   LDA   #$01
   CLC
   ADC   $80
   STA   $80
   LDA   $80
   RTS
```

### After

```
count_down:
   SUBROUTINE

   ; prologue: 1 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

.count_down@asm_ssa_block@0:
   LDY   #$03
   LDA   (FP),Y
   BNE   .if_end@0
.count_down@asm_ssa_block@1:
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$03
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@0:
...
   JSR   count_down
   STA   $80
   LDA   #$01
   CLC
   ADC   $80
   STA   $80
   LDA   $80

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$03
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
```

`main` uses the soft stack too, since it calls a soft-stack function,
but it has no parameters, no frame locals, and no callee-saved bytes,
so the pass leaves it unchanged.

## When it applies

A function gets a prologue and epilogue when any of these is
nonzero.

- Its argument bytes, which a soft-stack function's parameters take.
  A zp_abi function has none.
- Its frame local bytes. These hold a value the register allocator
  spilled, and an address-taken local that didn't get a byte in the
  private pool (see [Address-taken locals in zero
  page](address_taken_zp.md)).
- Its callee-saved bytes, which a function without a
  [private pool](zp_local_pools.md) uses for values that live across
  a call.

The prologue's `SSP` and `FP` arithmetic goes through A and Y. So
the stores of `register` parameters into their zp_abi slots, which
`tac_to_asm` puts at the start of the function, are moved in front
of the prologue. These are the leading non-volatile
`Mov(Reg(A|X|Y), Data(__zpabi_<function>__...))` instructions, with
any labels among them left where they are. If A is still read on
entry after that, because a parameter kept in A had its store
dropped, the pass wraps the prologue in `PHA` and `PLA`. No C program
tried for this page produced a zp_abi function with a `register`
parameter and a frame, since such a function's locals go in its
private pool. `test_y_stub_goes_before_the_prologue` in
[tests/test_param_registers.py](../../tests/test_param_registers.py)
covers the move.

## What it leaves alone

- A function with no soft-stack arguments, no frame locals, and no
  callee-saved bytes. This covers every zp_abi function whose locals
  fit its private pool, and a soft-stack function such as `main`
  above that takes no arguments and keeps nothing in the frame.

## Related

[Default zp_abi](zp_abi.md) removes the argument bytes,
[Private local pools](zp_local_pools.md) removes the callee-saved
bytes, and [Address-taken locals in zero page](address_taken_zp.md)
removes most frame locals, which together leave most functions with
no frame.
