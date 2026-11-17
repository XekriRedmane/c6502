# Outlined runs

`apply_outline_runs` in
[passes/outline_runs.py](../../passes/outline_runs.py) moves a run of
instructions that several places in a unit repeat into a function of
its own, ending in `RTS`, and puts a `JSR` to it in each place. It runs
once, last in `compile.optimized_asm`, after
[shared function tails](shared_tails.md), and only under `-O`.

Calls of one routine with the same arguments repeat the code that sets
the arguments up, and the code that stores the result often repeats
too. A function's own slots are spelled with its name, so code that
matches in two functions names only globals, registers, and other
functions' parameter slots, and does the same thing wherever it is.
`JSR` and `RTS` change no register and no flag, so the code after the
call sees what it saw after the run. Each place pays 12 cycles, and
the hardware stack holds two more bytes while the run executes.

## Example

Zork's `instr_mul`, `instr_div`, and `instr_mod`
(examples/zork/interpreter/numbers.c) each start by passing both
operands to `check_sign`.

### Before

```
instr_mod:
   LDA   OPERANDS+2
   STA   __zpabi_check_sign__b_0
   LDA   OPERANDS+3
   STA   __zpabi_check_sign__b_1
   LDA   OPERANDS
   STA   __zpabi_check_sign__a_0
   LDA   OPERANDS+1
   STA   __zpabi_check_sign__a_1
   JSR   check_sign
   STA   __zpabi_divu16__dividend_0
   ...
```

### After

```
instr_mod:
   JSR   instr_mul@run0
   JSR   instr_div@run1
   ...

instr_mul@run0:
   LDA   OPERANDS+2
   STA   __zpabi_check_sign__b_0
   LDA   OPERANDS+3
   STA   __zpabi_check_sign__b_1
   LDA   OPERANDS
   STA   __zpabi_check_sign__a_0
   LDA   OPERANDS+1
   STA   __zpabi_check_sign__a_1
   JMP   check_sign
```

These are two complete compiles, before and after the pass was added,
and `...` marks the lines they share. A run that ends with a call
ends with a `JMP` to the callee, which returns to the run's caller,
and the linker can then put the run right before the callee and drop
the `JMP` as well. `instr_mod`'s next run, which passes the result
and the divisor to `divu16`, is the same in `instr_div`, and so is
`instr_mul@run2`, which calls `instr_mul@run0` and stores its result
for `store_with_sign`: a run can call another. In the whole
interpreter the pass takes 75 bytes off, in the arithmetic
instructions, the page cache's disk reads, the disk routines, `save`
and `restore`, the printer and `[MORE]` code, the status line, the
tokenizer, and the three pushes of `instr_call`, whose run is `STA
__zpabi_push__value_1; JMP push`.

## When it applies

- A run is a sequence of loads, stores, arithmetic and logic
  instructions, compares, `SEC`, `CLC`, `BIT`, and calls, with no
  label, branch, jump, return, push, or pull.
- No instruction names a soft-stack operand or a function's own
  `__local_*` slot, and no call takes arguments in registers or keeps
  a register, since the registers are set or read around it.
- The function has no soft-stack frame.
- The run isn't inside a loop of at most 64 bytes, where 12 cycles a
  pass would be a large share of the loop's time.
- The places don't overlap. A run of `s` bytes at `n` places takes `s
  + 1 + 3n` bytes in place of `n * s`, or `s + 3n` when it ends with a
  call, and the change is made only when that's smaller, so two places
  need a run of 8 bytes or more, or 7 ending with a call.
  The sizes count a zero-page name's operand as one byte. The pass
  takes the run that saves the most first and repeats.

The new function has no parameters or locals. Its calls are ones the
functions it came from made, and the link metadata's call graph,
built from the TAC, has them as theirs, so the zero-page allocation is
the same. Its symbol is a copy of the first user's, with internal
linkage, so it goes in that function's section.

## What it leaves alone

- Code that ends in a `JMP` or `RTS` in every place, which the
  [shared function tails](shared_tails.md) share with a `JMP`, 3
  cycles and no stack.
- Runs in different units, whose code the compiler doesn't see
  together.

## Related

[Shared function tails](shared_tails.md) moves code that functions end
with alike. [Cross jumping](cross_jump.md) shares the end of two
blocks inside one function.
