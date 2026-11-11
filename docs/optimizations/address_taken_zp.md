# Address-taken locals in zero page

`compute_address_taken_assignments` in
[passes/address_taken_zp.py](../../passes/address_taken_zp.py) puts
each local whose address is taken in a byte of its function's
private zero-page pool, where it would otherwise go in a soft-stack
frame. It runs once in `compile.optimized_asm`, after the final run
of the asm optimizer and before `replace_pseudoregisters_bare_exit`,
under `--optimize` only.

A local whose address is taken can't take part in SSA renaming or
register allocation, since a pointer to it has to reach one fixed
location. Without this pass that location is in the frame, so the
function gets a prologue and an epilogue, `&x` is a 13-byte 16-bit
add of `FP` and the local's offset, and every access is
`LDY #off; LDA (FP),Y`. In a pool byte, `x` is named
`__local_<function>__x`, `&x` is the pair of immediates `#<` and
`#>` of that name, and an access is a 2-byte zero-page instruction.

The pool has room because `compute_address_taken_bytes` in
[passes/function_local_sizing.py](../../passes/function_local_sizing.py)
adds the size of every address-taken local to the function's demand
before [Private local pools](zp_local_pools.md) allocates it. After
the final coloring, the pass takes the pool bytes the coloring left
free, splits them into runs of consecutive addresses, and gives each
address-taken local, in the order the function first takes its
address, the start of the first run that's long enough.

## Example

```c
void read_pair(unsigned char *lo, unsigned char *hi) {
    *lo = 0x34;
    *hi = 0x12;
}

unsigned char sum_pair(void) {
    unsigned char a, b;
    read_pair(&a, &b);
    return a + b;
}

unsigned char main(void) { return sum_pair(); }
```

The "Before" listing comes from the same `--optimize` compile with
`compute_address_taken_assignments` replaced by one that assigns no
bytes, which leaves `a` and `b` in the frame. The "After" listing is
the normal `--optimize` compile.

### Before

```
sum_pair:
   SUBROUTINE

   ; prologue: 0 arg bytes, 2 local bytes
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$03
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   CLC
   LDA   FP
   ADC   #$01
   STA   __local_sum_pair__2
   LDA   FP+1
   ADC   #$00
   STA   __local_sum_pair__2+1
   CLC
   LDA   FP
   ADC   #$02
   STA   __local_sum_pair__0
   LDA   FP+1
   ADC   #$00
   STA   __local_sum_pair__0+1
   LDA   __local_sum_pair__2
   STA   __zpabi_read_pair__lo_0
   LDA   __local_sum_pair__3
   STA   __zpabi_read_pair__lo_1
   LDA   __local_sum_pair__0
   STA   __zpabi_read_pair__hi_0
   LDA   __local_sum_pair__1
   STA   __zpabi_read_pair__hi_1
   JSR   read_pair
   LDY   #$01
   LDA   (FP),Y
   CLC
   INY
   ADC   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$04
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
__local_sum_pair__a	EQU	$89
__local_sum_pair__b	EQU	$8A

sum_pair:
   SUBROUTINE

   LDA   #<__local_sum_pair__a
   STA   __zpabi_read_pair__lo_0
   LDA   #>__local_sum_pair__a
   STA   __zpabi_read_pair__lo_1
   LDA   #<__local_sum_pair__b
   STA   __zpabi_read_pair__hi_0
   LDA   #>__local_sum_pair__b
   STA   __zpabi_read_pair__hi_1
   JSR   read_pair
   LDA   __local_sum_pair__a
   CLC
   ADC   __local_sum_pair__b
   RTS
```

`sum_pair` goes from 113 bytes to 25. Its pool is `$85`-`$8A`. The
final coloring took `$85`-`$88` for the two pointers, and the pass
put `a` and `b` in the two bytes left over. Each pointer is a
constant once `a` and `b` have fixed addresses, so
[Split of LoadAddress](lower_data_load_address.md) and
[Memory value propagation](memory_value_propagation.md) later
replace the loads from `$85`-`$88` with immediates, and the stores
into those bytes are then dead.

## When it applies

- The function has a private local pool (see
  [Private local pools](zp_local_pools.md) for the conditions). It
  doesn't have to use zp_abi. A soft-stack function whose address is
  taken still has a pool, and its address-taken locals go in it.
- The local's name appears as the source of a `LoadAddress` in the
  function's asm.
- A run of consecutive pool bytes as long as the local is left free
  by the final coloring and by the address-taken locals placed before
  it. The bytes of a multi-byte colored value all count as used.

## What it leaves alone

- An address-taken local in a function without a pool, such as a
  recursive one. It stays in the frame.
- A parameter whose address is taken. It keeps its zp_abi slot or its
  frame slot.
- A static, which has its own fixed address.
- A local for which no free run is long enough. It falls back to a
  frame slot, and the function gets a prologue. The pool's size
  includes every address-taken byte, so this happens only when the
  final coloring uses more of the pool than the preliminary one did.

The pool bytes are disjoint from those of every function that can
run while this one is running, so a pointer to the local can be
passed to a callee and written through there, as `read_pair` does
here.

## Related

[Private local pools](zp_local_pools.md) sizes and places the pool,
and [Prologue synthesis](prologue_synthesis.md) leaves out the frame
once nothing is left in it.
