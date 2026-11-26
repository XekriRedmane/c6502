# Byte-by-byte three-way compare

`split_three_way_compares` in
[cpp/src/passes/optimization/three_way_compare.h](../../cpp/src/passes/optimization/three_way_compare.h)
splits a 2-byte unsigned ordering compare and an equality compare of
the same two values into a compare of the high bytes and then of the
low bytes. It runs once, after SSA destruction, under `-O`.

A search of a sorted table tests the key against an entry twice:

```c
if (key < e) return 0;
if (key == e) return found;
```

Each compare is lowered on its own. The first is a 2-byte subtract
whose borrow is the answer, and the second compares both bytes for
equality. The high bytes give both results unless they're equal,
so a person writing the 6502 code compares the high bytes once,
branches on less and on greater, and compares the low bytes only when
the high bytes are equal. The pass writes those byte compares in TAC,
which `tac_to_asm` lowers to one `CMP` and two branches per byte.
`redundant_compare` drops the second compare of each byte, and
`tail_merge` sends both bytes' "less" branches to one copy of the
`return 0`.

Zork's `match_dictionary_word` (examples/zork/input.c)
compares the two 2-byte halves of an encoded word with a dictionary
entry this way. Its inner loop became the same code as Zork's own,
and the function went from 231 bytes to 181. Zork's is 190.

## Example

```c
#include <stdint.h>
uint16_t key;
uint16_t table[8];
uint8_t find(void) {
    for (uint8_t i = 0; i != 8; i++) {
        if (key < table[i])
            return 0;
        if (key == table[i])
            return i + 1;
    }
    return 0;
}
```

### Before

```
.find@rot@0:
   ASL
   TAX
   LDA   table,X
   STA   __local_find__1
   LDA   table+1,X
   STA   __local_find__0
   LDA   key
   SEC
   SBC   __local_find__1
   LDA   key+1
   SBC   __local_find__0
   BCC   .loop@0_break
   LDA   key+1
   CMP   __local_find__0
   BNE   .if_end@1
   LDA   key
   CMP   __local_find__1
   BNE   .if_end@1
   LDA   __local_find__i
   CLC
   ADC   #$01
   RTS
```

### After

```
.find@rot@0:
   ASL
   TAX
   LDA   table,X
   STA   __local_find__2
   LDA   key+1
   CMP   table+1,X
   BCC   .loop@0_break
   BNE   .if_end@1
   LDA   key
   CMP   __local_find__2
   BCC   .loop@0_break
   BNE   .if_end@1
   LDA   __local_find__i
   CLC
   ADC   #$01
   RTS
```

The function goes from 54 bytes to 43. An entry whose high byte
differs from the key's takes one compare. The low byte of `table[i]`
is still loaded into a temporary before the compare of the high
bytes, since the 2-byte load stays where it was.

## How the outcomes are routed

Each of the three outcomes, less, equal, and greater, goes where the
two compares sent it.

- When the two compares are in a row, `JumpIfCmp(op1, a, b, T1);
  JumpIfCmp(op2, a, b, T2)`, an outcome goes to T1 if `op1` is true
  of it, else to T2 if `op2` is true of it, else to the instruction
  after the second compare. Each byte gets a `<` to the less outcome's place and
  a `!=` to the greater one's, and the low bytes branch on the two
  outcomes that don't fall through.
- When the ordering compare jumps to the equality compare, as the two
  `if`s above do,

  ```
      JumpIfCmp(>=, a, b, T1)
      X                          the less outcome, ending in a Ret or Jump
    T1:
      JumpIfCmp(!=, a, b, T2)
  ```

  the first compare becomes a compare of the high bytes with jumps to
  `X` and to the greater outcome's place, and then a `>=` of the low
  bytes that branches over X to the low bytes' `!=` (or `==`). The
  branch over X lets `tail_merge` send it to another copy of X. The
  second compare goes when nothing else jumps to T1. `<=` is the same
  with the operands swapped.

## When it applies

- The ordering compare is unsigned, by the rule `tac_to_asm` uses (an
  operand is unsigned or a pointer), and both operands are 2 bytes.
  Neither is volatile, and they aren't both constants.
- The equality compare has the same operands in either order. An
  operand may be a copy of a temporary that has one definition, such
  as the `unsigned` copy of a widened byte, and copies at T1 that only
  the equality compare reads go with it.
- The three outcomes go to three different places.
- The ordering compare doesn't follow a subtract of its operands,
  which `tac_to_asm` branches on instead
  ([compare and subtract](compare_subtract.md)).

A byte of an operand is a byte constant for a constant. For a value
built as `(ZeroExtend(h) << 8) | ZeroExtend(l)` from two 1-byte
temporaries, the bytes are `h` and `l`, and for `ZeroExtend(l)` they
are 0 and `l`. The temporaries and the instructions that build the
value must each have one definition, all in one basic block, so the
bytes hold at the compare what they held when the value was built. A
load of such a byte moves down to its compare when nothing in between
writes memory or the load's operands, so that the compare reads the
entry with `CMP (entry),Y`. Any other operand's bytes come from new
`Truncate(a >> 8)` and `Truncate(a)` temporaries, which `tac_to_asm`
lowers to reads of one byte. A high byte of 0 makes the less outcome
`b_hi != 0` there and the greater one impossible.

## What it leaves alone

- Signed compares. A signed high byte would take the overflow
  correction, which costs more than it saves.
- Operands wider than 2 bytes.
- A lone ordering compare, which is shorter as a subtract.
- The less outcome when it falls into T1, since the block of the low
  bytes' second compare goes in front of T1.

## Related

[Redundant compare](redundant_compare.md) and [tail
merge](tail_merge.md) finish the code this pass writes, and the
[comparison and jump fold](cmp_zero_jump_fold.md) makes the
`JumpIfCmp`s it reads.
