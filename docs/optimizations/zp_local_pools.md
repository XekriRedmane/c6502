# Private local pools

`allocate_function_locals` in
[passes/zp_local_allocation.py](../../passes/zp_local_allocation.py)
gives each eligible function a private run of zero-page bytes for
its body locals, chosen so that no function that can be running at
the same time uses any of them. The size of each run comes from
`compute_local_bytes` in
[passes/function_local_sizing.py](../../passes/function_local_sizing.py).
Both run once in `compile.optimized_asm`, between two runs of the asm
optimizer, under `--optimize` only.

Without a private pool, the byte-granular register allocator splits
zero page in two. A value that isn't live across a call goes in
`$80`-`$BF`, which is caller-saved, and a value that's live across a
call goes in `$C0`-`$FF`, which is callee-saved. A function that uses
a callee-saved byte has to save the caller's value in its prologue
and restore it in its epilogue, and that save goes through a
soft-stack frame, so even a zp_abi function picks up the whole
prologue and epilogue. A private pool is disjoint from the bytes of
every function that can be on the call stack with this one, so a
value in it survives any call the function makes, and there's
nothing to save.

The sizes come from a preliminary run of the asm optimizer. That run
uses the caller-saved and callee-saved split, and its output is
discarded once `compute_local_bytes` has counted the distinct `ZP`
bytes each function's coloring uses. `compute_address_taken_bytes`
adds the bytes of each local whose address is taken, so that
[Address-taken locals in zero page](address_taken_zp.md) has room in
the same pool. `allocate_function_locals` then walks the eligible
functions, callers before callees, and gives each one the lowest run
of that many bytes in `$80`-`$BF` that avoids all of these.

- The pools of its transitive callers.
- The parameter bytes of every zp_abi function that's a transitive
  caller or callee, including a zp_abi function declared `extern`.
- Its own parameter bytes.
- The bytes of the zero-page globals.

When no run in `$80`-`$BF` is long enough, the pool goes at `$0200`
or above, and its accesses assemble as absolute instructions. The
final run of the asm optimizer colors each function's values from
its pool alone, and the output names each pool byte
`__local_<function>__<name>` with an `EQU` line.

## Example

```c
unsigned char twice(unsigned char x) { return x + x; }

unsigned char sum2(unsigned char a, unsigned char b) {
    unsigned char t = twice(a);
    return t + twice(b);
}

unsigned char main(void) { return sum2(3, 4); }
```

The "Before" listing comes from the same `--optimize` compile with
`allocate_function_locals` replaced by one that returns no pools, so
every function uses the caller-saved and callee-saved split. The
"After" listing is the normal `--optimize` compile.

### Before

```
sum2:
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

   LDA   __zpabi_sum2__a
   STA   __zpabi_twice__x
   JSR   twice
   STA   $C0
   LDA   __zpabi_sum2__b
   STA   __zpabi_twice__x
   JSR   twice
   STA   $83
   LDA   $C0
   CLC
   ADC   $83

   ; epilogue
   PHA
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
   PLA
   RTS
```

### After

```
__local_sum2__0	EQU	$84
__local_sum2__1	EQU	$85

sum2:
   SUBROUTINE

   LDA   __zpabi_sum2__a
   STA   __zpabi_twice__x
   JSR   twice
   STA   __local_sum2__1
   LDA   __zpabi_sum2__b
   STA   __zpabi_twice__x
   JSR   twice
   STA   __local_sum2__0
   LDA   __local_sum2__1
   CLC
   ADC   __local_sum2__0
   RTS
```

`t` is live across the second call, so in the "Before" listing it's
in the callee-saved byte `$C0`, and saving that byte costs `sum2` a
frame. `sum2` goes from 95 bytes to 24. The pool starts at `$84`
because `main`, a caller of `sum2`, has a 1-byte pool at `$83`, and
`$80`-`$82` hold the parameters of `sum2` and `twice`. The byte at
`$83` comes from the preliminary run, and the final code for `main`
doesn't use it.

## When it applies

A function gets a pool when all of these are true.

- It's defined in the translation unit, so its body can be sized and
  its callees listed.
- Its body has no call through a function pointer, whose target
  could write anywhere.
- It isn't on a cycle of direct calls, unless every call from one
  function on the cycle to another is under `#pragma c6502 call
  save(hardware_stack)`. Then the caller saves what's live across
  each of those calls on the hardware stack ([saves around a
  recursive call](recursion_saves.md)), and the functions on the
  cycle may share bytes with each other.
- Every function defined in the translation unit that it calls
  directly has a pool too.
- Every function it calls that isn't defined in the translation unit
  uses zp_abi. Such a function is trusted to write only its own
  parameter bytes.

The function doesn't have to use zp_abi itself. A soft-stack function
whose only problem is that its address is taken still gets a pool.

## What it leaves alone

- A recursive function without the pragma, and every function that
  calls it, directly or through other functions. An inner activation
  would write the same bytes as the outer one.
- A function that calls through a pointer, and its callers.
- A function that calls an `extern` function that uses the soft
  stack, and its callers.
- `$C0`-`$FF`. Pools come only from `$80`-`$BF`, the same 64 bytes
  as the zp_abi parameters, and the register allocator uses the
  upper half only in functions without a pool.
- A function whose preliminary coloring used no zero-page bytes and
  that has no address-taken locals. Its pool is empty, and its final
  coloring uses the caller-saved and callee-saved split.

A value that doesn't fit the pool in the final coloring is spilled
to the soft-stack frame, which brings back the prologue. The pool is
sized from the preliminary coloring, so this happens only when the
final coloring uses more bytes than the preliminary one did.

## Related

[Default zp_abi](zp_abi.md) places the parameters by the same
call-graph rule, and [Register allocation](regalloc.md) colors each
function's values from its pool.
