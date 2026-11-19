# Loop-invariant compare key in A

`apply_loop_invariant_a` in
[passes/loop_invariant_a.py](../../passes/loop_invariant_a.py) keeps
a key that a loop compares against on every iteration in A, and
compares each loop value against it. It runs once under `-O`,
after the third peephole fixed point, just before
`expand_long_branches`. Within a function it repeats until no more
loops qualify. The unoptimized pipeline doesn't run it.

`tac_to_asm` lowers `if (tbl[i] == key)` as `LDA tbl,X; CMP key`,
loading the value that changes and comparing it with the one that
doesn't. Equality is symmetric, so `LDA key; CMP tbl,X` sets Z the
same way, and with `key` in A at the top of the loop the `LDA` drops
out of the loop. The pass loads `key` once before the loop, and again
only on the paths inside the loop that overwrite A before returning
to the top. In the loop below, `LDA abs,X; CMP zp` (5 bytes, 7
cycles) becomes `CMP abs,X` (3 bytes, 4 cycles), so each iteration
saves 3 cycles. `key` arrives in A, so no load of it is left before
the loop, the store of `key` at entry goes too, and the function is 4
bytes shorter. The hand-written
page-table search in Zork I keeps its key in A this way, and
`find_index_of_page_table` in `examples/zork/pages.c` is a C version
of that routine.

## Example

```c
#include <stdint.h>
extern uint8_t tbl[64];

uint8_t find(uint8_t key)
{
    uint8_t i = 0;
    do {
        if (tbl[i] == key)
            return i;
        i++;
    } while (i != 64);
    return 0xFF;
}
```

The listings are from `-O`, and the "Before" listing comes from the
same compile with `apply_loop_invariant_a` turned off.

### Before

```
find:
   SUBROUTINE

   STA   __local_find__key
   LDX   #$00
.loop@0_start:
   LDA   tbl,X
   CMP   __local_find__key
   BNE   .if_end@0
   TXA
   RTS
.if_end@0:
   INX
   CPX   #$40
   BNE   .loop@0_start
   LDA   #$FF
   RTS
```

### After

```
find:
   SUBROUTINE

   LDX   #$00
.loop@0_start:
   CMP   tbl,X
   BNE   .if_end@0
   TXA
   RTS
.if_end@0:
   INX
   CPX   #$40
   BNE   .loop@0_start
   LDA   #$FF
   RTS
```

In the Zork page-table search, a second compare inside the loop
overwrites A with `LDA (PAGE_H_TABLE),Y` before branching back toward
the top with `BNE .if_end@5`. The pass retargets that branch to a new
label, `.find_index_of_page_table@lia@1`, which it places after the
preceding `RTS`, where it reloads the key with `LDA
__local_find_index_of_page_table__page_0` and falls into `.if_end@5`.
The path that doesn't overwrite A reaches `.if_end@5` without the
reload.

## When it applies

- The loop is the range from a label `H` to the last branch or jump
  back to `H`, and no jump or branch from outside the range targets a
  label inside it.
- The loop starts `H: LDA x; CMP k; BEQ` or `BNE`, where the `LDA` is
  not volatile, `k` is a `Data` or `ZP` byte, and `x` is any operand
  `CMP` can address.
- Nothing in the loop writes a byte that may alias `k`, and the loop
  has no call.
- After the branch, on both of its successors, A is dead and neither
  N nor C is read before being overwritten. The swapped `CMP` sets Z
  the same way but leaves different N and C, and A holds `k` in place
  of `x`.
- A block that the reload has to pass through on its way back to `H`
  doesn't read or write A, so the reload can't change what it
  computes.
- A reload on a fall-through edge goes in front of the target block.
  A reload on a branch edge goes under a new label placed in front of
  the target's label, which is possible only when the instruction
  before that label is a `JMP` or a return, so that nothing falls into
  the reload.

## What it leaves alone

- A loop with a call in it. With `tick();` added after the `if` in
  the example, the callee may change `k` and A, and the loop keeps its
  `LDA tbl,X; CMP __local_find__key`.
- A key the loop writes, and a key that may alias a byte the loop
  writes, such as another byte reached through a pointer.
- A key that isn't a `Data` or `ZP` byte. A compare against an
  immediate, as in `tbl[i] == 5`, isn't matched.
- A compare followed by `BMI`, `BPL`, `BCC`, or `BCS`, or by code
  that reads A, N, or C after the branch.
- A loop where a reload has to go on a branch edge and the code
  before the target label can fall through into it.
- A volatile key read. With `extern volatile uint8_t key;` the loop
  reads `key` into a temporary on every iteration, and the header no
  longer has the `LDA x; CMP k` shape.

## Related

[Loop-carried value in A](promote_loop_carried_to_a.md) is the other
pass that keeps a value in A across a loop, and
[count-down counter in X or Y](counter_to_index_reg.md) puts the
counter of the Zork search in X.
