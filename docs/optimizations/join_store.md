# Join store

`apply_join_store` in [passes/join_store.py](../../passes/join_store.py)
handles a value that each arm of a branch leaves in one of the
function's own zero-page slots, and that the code where the arms join
copies into memory with `LDA t; STA M`. Each arm stores the value
straight into `M`, and the copy at the join goes. That saves 4 or 5
bytes, and the copy's 6 or 7 cycles on every path through the join.

The slot is there because the asm register allocator gives a value
that comes out of a conditional expression one home on both arms,
and it doesn't know where the value goes after the join. A 2-byte
value usually keeps its low byte in A across the join, so the high
byte is the one left in a slot.

## Example

From `do_instruction` in `examples/zork/decode.c`, whose
statement is

```c
OPERANDS[0] = (op & 0x40) ? get_var_content() : get_next_code_byte();
```

### Before

```
   JSR   get_var_content
   STX   __local_do_instruction__v_1
   JMP   .cond_end@31
.cond_else@30:
   JSR   get_next_code_byte
   LDY   #$00
   STY   __local_do_instruction__v_1
.cond_end@31:
   STA   OPERANDS
   LDA   __local_do_instruction__v_1
   STA   OPERANDS+1
   LDA   __local_do_instruction__1
```

### After

```
   JSR   get_var_content
   STX   OPERANDS+1
   JMP   .cond_end@31
.cond_else@30:
   JSR   get_next_code_byte
   LDY   #$00
   STY   OPERANDS+1
.cond_end@31:
   STA   OPERANDS
   LDA   __local_do_instruction__1
```

`do_instruction` stores four of its operand bytes this way and is 16
bytes shorter. Its one-operand case has three arms, and it stores
both bytes of the operand this way.

## When it applies

- At a label, or a run of labels, within eight instructions, an
  `LDA t; STA M`, with `t` one of the function's own `__local_*`
  slots and `M` a byte of memory with no index, neither volatile.
- The instructions between the labels and the `LDA` neither read nor
  write `t` or `M`, and none is a call, a branch, or a label.
- `t` isn't read after the `LDA`, and A and N/Z aren't read after the
  `STA`, since they no longer hold `t`'s value.
- The labels are reached only by `JMP`s and by falling through, at
  most three ways in all. A store to `M` takes a byte more than one
  to `t` when `M` is an absolute address, and three arms keep the
  change smaller than the 4 bytes saved.
- On each way in, the last write to `t` stores A, X, Y, or a
  constant, and the code after it up to the join has no call, branch,
  or label and neither reads nor writes `t` or `M`. That store becomes
  a store to `M`.

`M` takes its value earlier than it did, which no instruction can
see, since none between an arm's store and the join reads `M`.

## What it leaves alone

- A join that a branch reaches, since the arm's store to `M` would
  reach the branch's other path too.
- A value the join stores through an index, such as `OPERANDS[n] =
  v` with `n` computed after the join.
- The unoptimized pipeline, where the slot is on the soft stack.
