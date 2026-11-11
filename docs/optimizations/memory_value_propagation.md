# Memory value propagation

`apply_memory_value_propagation` in
[passes/memory_value_propagation.py](../../passes/memory_value_propagation.py)
tracks what each zero-page byte holds across the whole control-flow
graph of a function, and rewrites reads of a copy to read the
original. It runs in the peephole fixed point, in both the optimized
and the unoptimized pipelines, though in the unoptimized pipeline
only its first rewrite fires, and only on a pointer in a zero-page
global.

The pass is a forward dataflow analysis. At each point it records,
for each zero-page byte, an expression that the byte is known to
equal on every path that reaches the point. The expression is
another zero-page byte, an immediate, the low or high byte of a
label's address, a byte of a static that the function never writes,
or an element of such a static indexed by an X or Y that hasn't
changed since. A join keeps only the facts that are the same at
the end of every predecessor. The pass then makes two kinds of rewrite.

1. When both bytes of `DPTR` are known to equal a pair of adjacent
   zero-page bytes at `N` and `N+1`, an access through `(DPTR),Y`
   becomes an access through `(N),Y`. The two forms cost the same,
   but `DPTR` is then no longer read, so later peepholes delete the
   four instructions that copied the pointer into it. With both pointers in zero page that's 8 bytes
   and 12 cycles.
2. A read of a zero-page byte whose value is known to be one of the
   other expressions becomes a read of that expression. The read
   can be the source of a load or a store, the operand of `ADC`,
   `SBC`, `AND`, `ORA`, or `EOR`, the right side of a compare, or a
   push. When every read of the byte has been rewritten, the store
   that filled it is dead, and later peepholes delete it along with
   the load in front of it.

## Example

```c
#include <stdint.h>
uint8_t sum(uint8_t *p, uint8_t n) {
    uint8_t s = 0;
    for (uint8_t i = 0; i < n; i++) s += p[i];
    return s;
}
```

### Before

```
   LDA   #$00
   STA   __local_sum__s
   LDY   #$00
   LDA   __zpabi_sum__p_0
   STA   DPTR
   LDA   __zpabi_sum__p_1
   STA   DPTR+1
.loop@0_start:
   CPY   __zpabi_sum__n
   BCS   .loop@0_break
.sum@asm_ssa_block@0:
   LDA   (DPTR),Y
   STA   __local_sum__0
   LDA   __local_sum__s
   CLC
   ADC   __local_sum__0
   STA   __local_sum__s
.loop@0_continue:
   INY
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_sum__s
   RTS
```

### After

```
   LDA   #$00
   STA   __local_sum__s
   LDY   #$00
   LDA   __zpabi_sum__p_0
   STA   DPTR
   LDA   __zpabi_sum__p_1
   STA   DPTR+1
.loop@0_start:
   CPY   __zpabi_sum__n
   BCS   .loop@0_break
.sum@asm_ssa_block@0:
   LDA   ($80),Y
   STA   __local_sum__0
   LDA   __local_sum__s
   CLC
   ADC   __local_sum__0
   STA   __local_sum__s
.loop@0_continue:
   INY
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_sum__s
   RTS
```

The copy into `DPTR` sits before the loop label and the read sits
inside the loop, so a pass that clears its facts at every label would
miss it. The `$80` is the address of `__zpabi_sum__p_0`, and the
final assembly prints it by name as `LDA (__zpabi_sum__p_0),Y`, with
the four staging instructions gone.

The second rewrite shows up in

```c
uint8_t table[8];
uint8_t pick(uint8_t i, uint8_t j) {
    uint8_t a = table[3];
    if (i) j++;
    return a + j;
}
```

where `LDA table+3; STA __local_pick__0` comes before the `if`, and
the `LDA __local_pick__0` after the join becomes `LDA table+3`. The
store and its load are then dead and get deleted, which saves 5
bytes and 7 cycles less the 1 byte and 1 cycle that the absolute
read costs over the zero-page one. With `table[i]` in place of
`table[3]`, the read becomes `LDA table,X`, since X still holds `i`
at the join.

## When it applies

- The function's jumps and branches all go to its own labels. A
  function holding a tail call (`JMP other_function`) is left
  unchanged, because the control-flow graph can't be built.
- A byte counts as zero page when it's a `ZP` operand, or a `Data`
  operand whose symbol has an address below `$100`. The symbols with
  known addresses are the runtime's (`DPTR`, `HARGS`, and the rest),
  the compiler's zp_abi and local slots, and user zero-page globals.
  The unoptimized pipeline has no slots, which is why only a pointer
  in a user zero-page global gets the `(N),Y` rewrite there.
- A fact is established by a load into A followed by a store from A,
  or by a memory-to-memory move. It's killed by a write to either
  byte it names, by a write to the static byte it names, or by a
  write to X or Y when it names an indexed element.
- An indexed element replaces only the source of a move, and not
  when the move loads the index register itself, since the 6502 has
  no `LDX abs,X` or `LDY abs,Y`. The other operand positions take
  only an immediate, a label byte, or a static byte.

## What it leaves alone

- A read of a byte that's known to equal another zero-page byte.
  Both reads cost the same, so the pass makes no change, except for
  the `DPTR` pair.
- Anything after a store through a pointer, a soft-stack store, an
  indexed store, a call, a prologue, or a `LoadAddress`. Each of
  these clears every fact. A loop that stores through `p`, such as
  `for (i = 0; i < n; i++) p[i] = 0;`, keeps `(DPTR),Y`, because
  the store clears the `DPTR` fact on the back edge and the join at
  the loop label drops it. The soft-stack stores of the unoptimized
  pipeline clear the facts for the same reason.
- A volatile move. Its `is_volatile` bit doesn't say which operand
  is volatile, so neither A nor the destination gets a fact.
- A byte of a static that the function writes anywhere, such as
  `table` in a function that also assigns `table[3]`.
- An operand that sets up Y with `LDY #off` (a soft-stack or
  `(DPTR),Y` access with an offset) counts as a write to Y, so a
  fact about `arr,Y` is cleared there.

The second rewrite doesn't check that the store it feeds becomes
dead. When the staged byte is also read at a point where no fact
about it is recorded, for example after a call, the store stays and the rewritten read
costs 1 byte and 1 cycle more than the zero-page read it replaced.

## Related

[Rematerialization](asm_remat.md) covers similar staging round trips
within a block, and [asm dead store](asm_dead_store.md) deletes the
stores this pass leaves unread.
