# Jump tables

`apply_jump_tables` in [passes/jump_table.py](../../passes/jump_table.py)
turns a chain of compares of A with constants, each followed by a
branch to its case, into a dispatch through a table of the cases'
addresses. It runs once, at the end of the `--optimize` asm pipeline,
after every pass that follows control flow, so none of those passes
has to know the table.

A `switch` on a byte becomes a chain of `CMP #k; BEQ case`, 4 bytes a
case, and 7 when the case is too far for a branch and the `BEQ` grows
into `BNE; JMP`. The table takes 2 bytes a case, from the smallest
value to the largest, and 15 for the bounds check and the dispatch:

```
   CMP   #max+1
   BCS   default
   ASL
   TAY
   LDA   table+1,Y
   PHA
   LDA   table,Y
   PHA
   RTS
table:
   DC.W  case0-1
   DC.W  case1-1
   ...
```

`RTS` pulls the address the two `PHA`s pushed and adds one, so each
entry is its case's address minus one. A value that no case names
gets an entry for the default. Every case takes the same 32 cycles or
so to reach, where the chain took 5 for each compare before the
right one. Zork's own interpreter dispatches its instructions through
four tables the same way.

## Example

From `do_2op` in `examples/zork/interpreter/decode.c`, a switch of 24
cases on the opcode's low five bits.

### Before

```
   LDA   __local_do_2op__equal
   CMP   #$01
   BNE   .lb_skip@28
   JMP   .case@35
.lb_skip@28:
   CMP   #$02
   BNE   .lb_skip@27
   JMP   .case@36
   ...
.lb_skip@5:
   JMP   .default@59
```

### After

```
   LDA   __local_do_2op__equal
   CMP   #$19
   BCC   .lb_skip@2
   JMP   .default@59
.lb_skip@2:
   ASL
   TAY
   LDA   .jtab@2+1,Y
   PHA
   LDA   .jtab@2,Y
   PHA
   RTS
.jtab@2:
   DC.W  .default@59-1
   DC.W  .case@35-1
   DC.W  .case@36-1
   ...
```

`do_2op` is 103 bytes shorter, and the four dispatch switches of
`decode.c` together 151.

## When it applies

- At least four compares of A with constants from 0 to 127, one
  after another, each followed by a `BEQ` to its case, with no label
  in the chain and no constant twice. The case for 0 may be a `BEQ`
  right after the load of A, with no compare.
- After the chain, a `JMP` to the default, or code that is the
  default, which gets a label (`.jtdef@N`).
- At every case and at the default, A, N, Z, and C are dead, since
  the dispatch leaves them as the table's load set them, and so is Y,
  the index, or else X, which takes its place.
- The table is shorter than the chain, counting each compare and
  branch as 2 bytes, as they are when the branch reaches its case.
  When the smallest constant is 2 or less, the table starts at 0,
  which costs at most two entries and saves the `CMP; BCC` of a lower
  bound.

The pass labels each table `.jtab@N`, numbered across the program.
The dispatch and the table are one `JumpTable` instruction in the asm
IRs, which `asm_emit` and the simulator's assembler write out.

## What it leaves alone

- A case that reads A, which still holds the value in the chain.
- A chain whose compares aren't all of A with constants, such as the
  2-byte compares of a switch on an `int`.
- The unoptimized pipeline.

## Related

[Tail call](tail_call.md) makes each case that ends in a call and a
`break` a `JMP` to the function it calls.
