# Carry-flag return without the annotation

`choose_carry_returns` in
[passes/carry_return_choice.py](../../passes/carry_return_choice.py)
gives a function the convention of
`[[c6502::return_bool_in_carry]]` without the annotation: it returns
its `bool` in C, and a caller branches on it with `BCC` or `BCS`.
`choose_global_carry_returns`, in the same file, does the same for a
function with external linkage ([a function with external
linkage](#a-function-with-external-linkage)). Both run once on the
optimized TAC, between ABI selection and translation to asm, and only
under `-O`. `choose_global_carry_returns` runs first, so that
a static function can pass its carry on through a function it
chose.

A function that returns its `bool` in A leaves the flags as whatever
its last instruction set, so a caller that branches on the result
tests it first, with `ORA #$00` or `CMP #$00`, 2 bytes and 2 cycles.
With the result in C, the branch follows the `JSR`. A caller that
keeps the result as a value pays for the carry instead, `LDA #0; ROL
A`, 3 bytes, where the value in A costs nothing. So for a `static`
function, whose every call it can see, the compiler makes the choice
only when every call uses the result in a way the carry suits.

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
  of a `Ret` right after the call in a function that returns in C
  too, chosen here, by the rule for external linkage below, or by
  the annotation, which passes the carry on. Only labels that nothing
  jumps to may sit between, since SSA leaves some there. The rule for
  external linkage runs first, so that its choices count here.
- At least one call's result is branched on or returned that way.

The last two conditions are decided for all the candidates together.
A candidate drops out when one of its calls uses the result another
way, and that can drop the candidates whose result it returns.

## A function with external linkage

Another unit can call a function with external linkage. The unit
that defines it can't see those calls, and each unit that calls it
can't see the others, so the choice can't depend on how the calls
use the result. Every unit makes it from the declaration instead. A
function returning `bool`, with external linkage, no `address`, and
no annotation, returns in C whenever it's zp_abi in the unit, which
also means its address isn't taken there. That covers the unit that
defines it and every unit that only declares and calls it.

A caller that keeps such a result as a value pays `LDA #0; ROL A`, 3
bytes, but one that branches on it saves its `ORA #$00`, and the
function itself usually gets shorter, since a comparison it returns
leaves the answer in C.

```c
bool big(uint8_t x) { return x > 5; }
```

```
big:                                big:
   LDA   #$05                          LDA   #$05
   SEC                                 SEC
   SBC   __zpabi_big__x                SBC   __zpabi_big__x
   BCC   .cmp_true@0          ->       BCC   .cmp_true@0
   LDA   #$00                          CLC
   JMP   .cmp_end@1                    RTS
.cmp_true@0:                        .cmp_true@0:
   LDA   #$01                          SEC
.cmp_end@1:                            RTS
   RTS
```

Each unit records the choice in its link metadata, as `carry=true` on
the function's `def` or `ext` line, for an annotated function too.
The linker rejects units that disagree
(`linker._carry_conflict`). That happens when one unit takes the
function's address, or when the unit that defines it can't make it
zp_abi. A function that returns `unsigned char` returns it in A in
every unit.

## What it leaves alone

- A function with external linkage that isn't zp_abi in the unit,
  such as one whose address is taken there.
- A call whose result is stored, used in arithmetic, or tested after
  other instructions, such as a copy that SSA destruction put
  between the call and the jump. The carry would have to be captured
  into a byte for those, which costs more than returning in A. The
  choice is made on TAC, where it can't tell which instructions
  will leave C alone. Zork's `write_to_sector`
  (examples/zork/interpreter/disk.c) stores `do_rwts_on_sector`'s
  result while it restores the motor's spin-up time, and the carry
  survives the `LDA` and `STA` in between, so `do_rwts_on_sector`
  keeps the annotation.
- A function returning a struct of a value and a `bool`, which only
  the annotation gives.

## Related

[The carry-flag return peepholes](carry_return.md) shorten the code
of a function that returns in C, chosen here or annotated, and of its
callers.
