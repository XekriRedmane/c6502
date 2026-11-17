# Loop test at the bottom

`apply_rotate_loop_test` in
[passes/rotate_loop_test.py](../../passes/rotate_loop_test.py) moves
the test of a loop that tests at its top to the loop's bottom. It
runs once, after [the loop-invariant compare key in
A](loop_invariant_a.md) and before long-branch expansion, and only
under `-O`.

A `for` or `while` loop tests at the top and ends with a `JMP` back
to the test. Each pass runs the test's branch not taken (2 cycles)
and the `JMP` (3 cycles). With the test at the bottom, inverted to
branch back to the body, each pass runs the branch taken (3 cycles)
and no `JMP`, 2 cycles fewer. The loop is entered with a `JMP` to the
test, where the old layout fell into it, so the code is the same
size, and a loop that runs `n` times saves `2n - 2` cycles. A loop
that doesn't run at all costs 2 cycles more.

When the test is known to pass the first time, the entry `JMP` goes
too, which saves 3 bytes and 3 more cycles. That's the common
`for (uint8_t i = 0; i != 6; i++)`, whose test `CPX #$06; BEQ`
comes right after `LDX #$00`.

## Example

```c
#include <stdint.h>
uint8_t buf[8];
void clear(void) { for (uint8_t i = 0; i != 6; i++) buf[i] = 0; }
uint8_t count(void) { uint8_t c = 0; while (buf[c] != 0x8D) c++; return c; }
```

### Before

```
clear:
   LDX   #$00
.loop@0_start:
   CPX   #$06
   BEQ   .loop@0_break
   LDA   #$00
   STA   buf,X
   INX
   JMP   .loop@0_start
.loop@0_break:
   RTS
count:
   LDX   #$00
.loop@1_continue:
   LDA   buf,X
   CMP   #$8D
   BEQ   .loop@1_break
   INX
   JMP   .loop@1_continue
.loop@1_break:
   TXA
   RTS
```

### After

```
clear:
   LDX   #$00
.clear@rot@0:
   LDA   #$00
   STA   buf,X
   INX
   CPX   #$06
   BNE   .clear@rot@0
.loop@0_break:
   RTS
count:
   LDX   #$00
   JMP   .loop@1_continue
.count@rot@1:
   INX
.loop@1_continue:
   LDA   buf,X
   CMP   #$8D
   BNE   .count@rot@1
.loop@1_break:
   TXA
   RTS
```

`clear` loses its first test and its `JMP`, since X is 0 on entry and
`CPX #$06; BEQ` doesn't branch for 0. `count` reads `buf` in its
test, which isn't known on entry, so it's entered with a `JMP` to its
test. In Zork's interpreter (examples/zork/interpreter) this removes
the entry test of five loops, 15 bytes, and saves 84,602 of the first
test session's 38,476,066 cycles.

## When it applies

- A `JMP L` is followed, after any labels, by the label that the
  loop's test branches to, and `L` is a label earlier in the
  function.
- The test, the instructions after `L` up to the first branch, has
  no label, jump, or return in it, and that branch is a conditional
  one to the label after the `JMP`.
- The test and the body together are at most 112 bytes, so that the
  backward branch reaches the body even if long-branch expansion
  lengthens branches inside it.

The entry `JMP` is left out when control can't fall into `L`, since
the instruction before it is a jump or a return, and when the test is
known to pass on entry. The pass knows that by running the test on
the values that the straight-line code in front of `L` leaves in A, X,
Y, and memory: loads, stores, transfers, compares, increments and
decrements, and `SEC` and `CLC`, all of immediates or of bytes that
code stored. Anything else, a label, and a volatile access make the
values unknown, and then the `JMP` stays. A label at the start of the
function is where the function is entered, so control falls into it,
and the values there are unknown.

Entering at the body also skips what the test leaves in the registers
and the flags, which the body may read, since until now the body ran
only after the test. So the entry `JMP` is left out only when every
register the test writes, and N, Z, C, and V, are each dead at the
start of the body. After `LDA i; CMP #1; BCS exit`, C is clear in the
body, and [carry already known](carry_known.md) had dropped the `CLC`
of an increment there. Entered at the body, the first increment added
the carry from before the loop. `fuzz.stmts` found it with seed
1262.

Other jumps to `L`, such as from a `continue`, still go to `L`, which
moves to the bottom with the test.

## What it leaves alone

- A loop whose test branches out to anywhere other than right after
  the `JMP` back, such as a `return` from inside the loop's test,
  since the test at the bottom would then need a `JMP` of its own.
- A test with more than one branch, such as a 16-bit compare that
  branches on its high byte first.
- A loop that `loop_invariant_a` changed, whose reload stubs sit
  after the `JMP` back.
- A loop longer than 112 bytes.

## Related

[Loop rotation](loop_rotate.md) is the TAC-level rotation of signed
count-down loops, which runs before SSA and gives `DEX; BPL` loops.
