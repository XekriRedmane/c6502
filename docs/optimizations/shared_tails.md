# Shared function tails

`apply_shared_tails` in
[passes/shared_tails.py](../../passes/shared_tails.py) moves the
instructions that several functions of a unit end with alike into a
function of their own, and makes each of them jump there. It runs
once, last in `compile.optimized_asm`, after
[shorter jumps](short_jumps.md), and only under `-O`.

Functions that hand their result to the same callee end the same way.
A Z-machine instruction stores a byte it found with `store_result`,
which takes a word, so the byte's high byte is stored as 0 and the
callee is tail-called. [Cross jumping](cross_jump.md) shares such
code inside one function, but a `JMP` can't go to a label in another
function: dasm and `binutils.assembler` scope a `.` label to its
function, and a label of any other kind starts an item, which the
linker may place anywhere. So the shared code becomes an item, the
function `F@tail<n>`.

## Example

In Zork's `objects.c` (examples/zork/interpreter), `instr_get_parent`,
`instr_get_prop`, `instr_get_next_prop`, and `instr_get_prop_len` each
end with the store of a byte.

### Before

```
instr_get_parent:
   LDA   OPERANDS
   JSR   get_object_addr
   LDY   #$04
   LDA   (get_object_addr@ret),Y
   STA   __zpabi_store_result__value_0
   LDA   #$00
   STA   __zpabi_store_result__value_1
   JMP   store_result
```

### After

```
instr_get_parent:
   LDA   OPERANDS
   JSR   get_object_addr
   LDY   #$04
   LDA   (get_object_addr@ret),Y
   JMP   instr_get_parent@tail0

instr_get_parent@tail0:
   STA   __zpabi_store_result__value_0
   LDA   #$00
   STA   __zpabi_store_result__value_1
   JMP   store_result
```

These are two complete compiles, before and after the pass was added.
The other three functions end with `JMP instr_get_parent@tail0` too.
The 9 bytes are in the program once, and each function pays a 3-byte
`JMP`, but the linker puts `instr_get_parent` right before its tail
and drops its `JMP` (`link_layout.fall_through`), as it does for any
function that ends by jumping to another of its output section. In
the linked interpreter the pass takes 79 bytes off with 15 tails. In
`decode.c` they're shared by `branch`, `return_value`, `instr_call`,
`inc_var` and `dec_var`, and cases of `do_0op`, `do_1op`, and
`do_2op`. The others are in the property and attribute routines of
`objects.c`, `mul`, `div`, `mod`, and `random` in `numbers.c`, `save`
and `restore`, and `quit` and the internal error, which print a
message and stop the same way. A tail can share a shorter tail with
other functions in turn: `instr_get_prop_addr@tail1`, `LDA #$00; STA
__zpabi_store_result__value_0`, ends by jumping to
`instr_get_prop@tail2`, which stores the high byte and jumps to
`store_result`.

## When it applies

- A *segment* is a run of instructions in a function that ends with
  a `JMP` or an `RTS` and doesn't start with a label. Every branch or
  jump in it to a `.` label goes to a label inside it, and no
  instruction outside it goes to a label inside it.
- Two segments match when they're the same instructions with their
  labels renamed in order. So `quit` and the internal error, which end
  in loops on `.loop@5_start` and `.loop@6_start`, match.
- The functions have no soft-stack frame (`FunctionPrologue`), since a
  `Ret` tears down its own function's frame, and a segment holds no
  `JumpTable`, whose table is named for its function.
- A segment of `s` bytes that `n` functions share takes `s + 3n`
  bytes in place of `n * s`. The pass shares it only when that's
  smaller before the link too, `s + 3n < n * s`, so a program that
  isn't linked doesn't grow. It takes the segment that saves the most
  first, with every function that ends with it, and repeats.

The instructions are the same, so they do the same thing in either
function. A function's own slots and labels are spelled with its
name, so matching instructions name only registers, globals, and
other functions' parameter slots. A `(zp),Y` pointer is the
exception. Its operand holds the pointer's address, and `asm_emit`
names it by the slot that the function it's emitted in keeps at that
address. Two functions that are never on the call stack together can
have slots at the same bytes in a unit, and the linker may place them
apart, so a segment doesn't read or write through a pointer kept in a
slot's bytes. The tail then starts after the last such access. The flags and registers reach the
tail across the `JMP`, which changes neither. The new function has no
parameters or locals, and the link metadata's call graph, built from
the TAC, already has its calls as calls of the functions it came
from. Its symbol is a copy of the first sharer's, with internal
linkage, so it goes in the same section.

## What it leaves alone

- Two segments of one function. Each function gives one segment to a
  share, and [cross jumping](cross_jump.md) handles a function's own.
- A segment that matches only the end of another function's longer
  shared segment. Each function joins a share with a segment as long
  as the share's.
- Functions in different units, whose code the compiler doesn't see
  together.

## Related

[Cross jumping](cross_jump.md) shares runs that end with a jump to the
same place inside one function. [Tail merge](tail_merge.md) shares a
return tail inside one function.
