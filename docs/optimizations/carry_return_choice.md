# Carry-flag return without the annotation

`choose_carry_returns` in
[passes/carry_return_choice.py](../../passes/carry_return_choice.py)
gives a function the convention of
`[[c6502::return_bool_in_carry]]` without the annotation: it returns
its `bool` in C, and a caller branches on it with `BCC` or `BCS`. It
runs once on the optimized TAC, before ABI selection and translation
to asm, and only under `--optimize`.

A function that returns its `bool` in A leaves the flags as whatever
its last instruction set, so a caller that branches on the result
tests it first, with `ORA #$00` or `CMP #$00`, 2 bytes and 2 cycles.
With the result in C, the branch follows the `JSR`. A caller that
keeps the result as a value pays for the carry instead, `LDA #0; ROL
A`, 3 bytes, where the value in A costs nothing. So the compiler
makes the choice only for a function whose every call it can see and
whose every call uses the result in a way the carry suits.

## Example

```c
#include <stdint.h>
#include <stdbool.h>
uint8_t g;
static bool small(uint8_t x) { return x < 10; }
int main(void) { if (small(g)) g = 1; return g; }
```

### Before

```
small:
   ...
   BMI   .cmp_true@1
   LDA   #$00
   JMP   .cmp_end@2
.cmp_true@1:
   LDA   #$01
.cmp_end@2:
   RTS
main:
   ...
   JSR   small
   ORA   #$00
   BEQ   .if_end@0
```

### After

```
small:
   ...
   BMI   .cmp_true@1
   CLC
   RTS
.cmp_true@1:
   SEC
   RTS
main:
   ...
   JSR   small
   BCC   .if_end@0
```

These are two complete compiles, with the pass turned off and on,
and `...` marks the lines they share. `small`'s returns take 4 bytes
where they took 8, since each is a `CLC` or `SEC` and its own `RTS`
([carry_return.md](carry_return.md)), and the call takes 2 bytes
less.

In Zork's interpreter (examples/zork/interpreter) it chooses the
three separator tests of `input.c`, which the original also returns
in the carry, and `is_separator` becomes the original's `JSR
is_dict_separator; BCC ...; RTS` followed by a tail call of
`is_std_separator`.

## When it applies

- The function returns `bool`, has internal linkage (`static`), and
  is defined in the unit, so every call to it is there.
  `FunAttr.returns_bool` records the `bool`, which `c99_to_tac` sets
  before it turns `_Bool` into `unsigned char` in the types.
- It isn't already declared `return_bool_in_carry`, doesn't have an
  `address`, and its address is never taken, since an indirect call
  can't know the result is in C.
- Each call's result is unused, or is the condition of a
  `JumpIfTrue` / `JumpIfFalse` right after the call, or is the value
  of a `Ret` right after the call in a function that's chosen too,
  which passes the carry on. Only labels that nothing jumps to may
  sit between, since SSA leaves some there.
- At least one call's result is branched on or returned that way.

The last two conditions are decided for all the candidates together.
A candidate drops out when one of its calls uses the result another
way, and that can drop the candidates whose result it returns.

## What it leaves alone

- A function with external linkage, since a call from another unit
  would read the result in A.
- A call whose result is stored, used in arithmetic, or tested after
  other instructions, such as a copy that SSA destruction put
  between the call and the jump. The carry would have to be captured
  into a byte for those, which costs more than returning in A.
- A function returning a struct of a value and a `bool`, which only
  the annotation gives.

## Related

[The carry-flag return peepholes](carry_return.md) shorten the code
of a function that returns in C, chosen here or annotated, and of its
callers.
