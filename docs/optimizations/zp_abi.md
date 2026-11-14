# Default zp_abi calling convention

`select_abi` in [passes/abi_selection.py](../../passes/abi_selection.py)
gives every function that qualifies the zp_abi calling convention,
and `allocate_zp_slots` in
[passes/zp_slot_allocation.py](../../passes/zp_slot_allocation.py)
then picks the zero-page bytes that hold each such function's
parameters. Both run once, under `--optimize` only, after the TAC
optimizer and before `tac_to_asm`. Without `--optimize` every
argument goes on the soft stack, except for a call to a zp_abi
routine at a fixed address. The convention itself is described for
users in [calling_conventions.md](../calling_conventions.md), and
its design is in [leaf_zp_abi.md](../leaf_zp_abi.md). This page
covers which functions get it, what it saves, and when a function
falls back to the soft stack.

A soft-stack call costs the caller a 16-bit subtract from `SSP` and
an `LDY` and a `STA (SSP),Y` per argument byte. In the example
below, the callee's prologue, which saves `FP` and points it at the
new frame, takes 32 bytes, and its epilogue takes 29. Each parameter
read is an `LDY #off` or `INY` followed by a 5-cycle `LDA (FP),Y`.
Under zp_abi each parameter byte has a fixed address named
`__zpabi_<function>__<parameter>`. The caller stores the argument
there, the callee reads it with a 2-byte, 3-cycle zero-page
instruction, and there's no prologue or epilogue. When the
function's locals also fit in zero page (see
[Private local pools](zp_local_pools.md)), the function is its body
followed by `RTS`.

`allocate_zp_slots` walks the call graph of the zp_abi functions,
callers before callees, and gives each function the lowest run of
bytes in `$80`-`$BF` that none of its transitive callers uses.
Two functions where neither can call the other may get the same
bytes, since they never run at the same time. When `$80`-`$BF` has
no run that's long enough, the slots go at `$0200` or above, and the
same `LDA` and `STA` instructions assemble in their 3-byte absolute
form. When every function in the unit has a [private local
pool](zp_local_pools.md), no function keeps a value in `$C0`-`$FF`
across a call, so the slots and pools come from all of `$80`-`$FF`,
as they do when `--link` allocates them for a whole program.

Not every parameter gets a slot:

- A `register` parameter arrives in A, X, or Y, and the callee's
  first instructions copy it into a local of its own
  (`tac_to_asm._register_param_locals`), which the register allocator
  keeps in the register when it can and in the function's pool
  otherwise. The copies from A come first, and the local is never put
  in a register another parameter arrives in, so no copy overwrites a
  register another copy still reads (`optimizer._entry_register_limits`).
- A parameter declared `[[c6502::zeropage(addr)]]` is at that
  address. Its bytes are kept off everything else of the compiler's.
  A function defined here copies such a parameter into a local as it
  starts when something it calls might store an argument there: a
  function with a `zeropage(addr)` parameter on the same bytes, or one
  defined in another unit that isn't a routine with a fixed address
  (`slot_sharing.pinned_param_copies`,
  `tac_to_asm._pinned_param_locals`).
- Parameters that [hold one value for several
  functions](slot_sharing.md) share one address.

## Example

```c
unsigned char add3(unsigned char a, unsigned char b, unsigned char c) {
    return a + b + c;
}

unsigned char main(void) { return add3(1, 2, 3); }
```

The "Before" listing is the soft-stack code that `--optimize`
produces for the same program with `fp = add3;` added at the start
of `main`, where `fp` is a global function pointer. That takes
`add3`'s address, which makes it ineligible. The lines that store
`fp` are dropped. The "After" listing is the program above, compiled
with `--optimize`.

### Before

```
add3:
   SUBROUTINE

   ; prologue: 3 arg bytes, 0 local bytes
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

   INY
   LDA   (FP),Y
   CLC
   INY
   ADC   (FP),Y
   CLC
   INY
   ADC   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$05
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

main:
   SUBROUTINE
...
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$01
   LDY   #$01
   STA   (SSP),Y
   LDA   #$02
   INY
   STA   (SSP),Y
   LDA   #$03
   INY
   STA   (SSP),Y
   JMP   add3
```

### After

```
__zpabi_add3__a	EQU	$80
__zpabi_add3__b	EQU	$81
__zpabi_add3__c	EQU	$82

add3:
   SUBROUTINE

   LDA   __zpabi_add3__a
   CLC
   ADC   __zpabi_add3__b
   CLC
   ADC   __zpabi_add3__c
   RTS

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_add3__a
   LDA   #$02
   STA   __zpabi_add3__b
   LDA   #$03
   STA   __zpabi_add3__c
   JMP   add3
```

`add3` shrinks from 72 bytes to 9, and the call in `main` from 32
bytes to 15.

## When it applies

A function defined in the translation unit gets zp_abi when all of
these are true.

- Its body has no call through a function pointer.
- It isn't on a cycle of direct calls among the functions defined in
  the translation unit.
- Its address isn't taken. That's any use of its name as a value in
  a function body, its own included, and any static initializer that
  names it (`int (*fp)(int) = f;`). A call through a pointer passes
  the arguments on the soft stack, so a function that can be called
  that way keeps the soft stack.
- It doesn't return a struct or union, unless it's
  `return_bool_in_carry`, whose struct comes back in A, X, and C.
- Its parameters take at most 64 bytes, the size of `$80`-`$BF`.
- Every function it calls directly uses zp_abi too, or has a
  [private local pool](zp_local_pools.md). A soft-stack callee
  without a pool uses `$80`-`$BF` as scratch, which is where the
  caller's own parameters are. One with a pool, such as a function
  whose address is taken or a recursive one under `#pragma c6502
  call save(hardware_stack)`, writes only its pool, which is kept
  apart from the caller's bytes. `_demote_soft_stack_callers`
  applies this last and repeats until no function changes, since
  demoting one function can disqualify its callers. The parameter
  slots of two zp_abi functions are kept apart when one can reach
  the other, even through soft-stack functions in between.

A function declared but not defined in the translation unit gets
zp_abi under the parameter-size and return-type rules, since there's
no body to check, unless its address is taken, which the same scan
finds for declared functions too. A routine with
`[[c6502::address(addr)]]` takes its convention from its
declaration alone.

A function marked `[[c6502::zp_abi]]`, or one with a `register`
parameter or local, gets a compile error in place of the fallback
when it doesn't qualify.

The asm regalloc keeps each function's body values off its own
parameter bytes and off the parameter bytes of every zp_abi function
it calls (`_blocked_addrs_for` in
[passes/optimization_asm/optimizer.py](../../passes/optimization_asm/optimizer.py)),
so an argument store before a call can't overwrite a value that's
still live.

## What it leaves alone

- A recursive function, directly or through other functions. Each
  activation would read the same parameter bytes, and the inner call
  would overwrite the outer one's arguments.
- A function whose address is taken in another function's code, or
  that calls through a pointer. The target of a call through a
  pointer isn't known when the program is compiled, so that call
  always uses the soft stack.
- A function that returns a struct or union. The hidden pointer to
  the result isn't one of the declared parameters, so the slot
  layout has no byte for it.
- A caller of a soft-stack function without a private pool, such as
  a recursive function without the pragma, a function that calls
  through a pointer, or a routine with a fixed address that's
  declared without `zp_abi`. With
  `[[c6502::address(0xFDED)]] void cout(unsigned char c);`, a
  function that calls `cout` gets a soft-stack prologue.

## Related

[Private local pools](zp_local_pools.md) puts the body locals of
the same functions in zero page, [Address-taken locals in zero
page](address_taken_zp.md) does the same for locals whose address is
taken, and [Prologue synthesis](prologue_synthesis.md) leaves out the
frame of a function that ends up needing none.
