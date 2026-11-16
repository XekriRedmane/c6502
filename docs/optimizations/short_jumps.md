# Shorter jumps

`apply_short_jumps` in [passes/short_jumps.py](../../passes/short_jumps.py)
shortens jumps once the branches have their final reach. It runs
once, right after long-branch expansion, at the end of the
`--optimize` asm pipeline, and makes three changes, each of which
only removes bytes, so no branch falls out of reach.

- A `JMP .L`, or a [jump table](jump_table.md)'s entry for `.L`, where
  the first instruction at `.L` is a `JMP` to another function, goes
  to that function itself. A switch case that only calls a function
  and breaks is such a stub, since [tail call](tail_call.md) makes it
  `JMP f`, and so is a default that calls a function that doesn't
  return.
- A label that nothing names any more, after an instruction that
  doesn't fall through, goes, and so does the code after it up to the
  next label. That's each stub the first change leaves behind.
- `LDA #k`, `LDX #k`, or `LDY #k` followed by `JMP .L` becomes `BEQ
  .L` for `k` of 0 and `BNE .L` otherwise, when `.L` is within a
  branch's reach with a byte to spare each way. The load sets Z, so
  the branch is always taken. `tac_to_asm` lowers a comparison kept
  as a value as `Bcc t; LDA #0; JMP end; t: LDA #1; end:`, whose
  `JMP` is one of these.

A jump table's entry is the address the dispatch's `RTS` goes to
minus one, so an entry may name any function. The function returns
to the switch's caller, as it would from the stub's `JMP`.

## Example

From `do_1op` in `examples/zork/interpreter/decode.c`, a switch of 15
cases, 7 of which only call a function.

### Before

```
   CMP   #$10
   BCC   .lb_skip@0
   JMP   .default@33
.lb_skip@0:
   ASL
   ...
.jtab@1:
   DC.W  .case@18-1
   DC.W  .case@19-1
   ...
.case@18:
   LDA   OPERANDS+1
   ORA   OPERANDS
   BEQ   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@2:
   LDA   #$01
.cmp_end@1:
   JMP   branch
.case@19:
   JMP   instr_get_sibling
   ...
.default@33:
   JMP   internal_error
```

### After

```
   CMP   #$10
   BCC   .lb_skip@0
   JMP   internal_error
.lb_skip@0:
   ASL
   ...
.jtab@1:
   DC.W  .case@18-1
   DC.W  instr_get_sibling-1
   ...
.case@18:
   LDA   OPERANDS+1
   ORA   OPERANDS
   BEQ   .cmp_true@2
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@2:
   LDA   #$01
.cmp_end@1:
   JMP   branch
```

Each stub that goes saves 3 bytes, and each `JMP` that becomes a
branch saves 1. Together with the jump table's handling of a case for
0 after an `ORA #$00`, the change that made this listing's first
line, `decode.c`'s four dispatch switches are 112 bytes shorter, and
the rest of Zork's interpreter 9.

## When it applies

- A jump or table entry goes to the function a stub jumps to when
  neither the `JMP` to the stub nor the stub's `JMP` passes arguments
  in registers (`reg_args`).
- A stub goes only when no branch, jump, jump table, or operand names
  its label, and the instruction before the label is a `JMP`, `RTS`,
  or jump table.
- The load before a `JMP` that becomes a branch is a `Mov` of an
  immediate into A, X, or Y, and the `JMP`'s target is a label in the
  function. The displacement is measured on the code as it stands,
  less a byte, since the `JMP` itself loses one.

## What it leaves alone

- A branch to a stub, since a branch can't leave its function. The
  stub stays for it.
- A `JMP` after anything but a load of a constant, since the flags
  aren't known there.
- The unoptimized pipeline.

## Related

[Jump tables](jump_table.md) make the tables whose entries this
redirects, and [long-branch expansion](../../passes/long_branches.py)
fixes the branches' reach that this relies on.
