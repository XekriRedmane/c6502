# A compound assignment's call first

`translate_compound_assign` in cpp/src/c99_to_tac_exp.cpp evaluates
the right operand of `lval OP= rval` before the left one's address
and value when `rval` calls a function and `lval` neither calls nor
writes anything and isn't volatile (`calls_first`). The left operand is then read after
the call, so neither its address nor its value has to be kept across
the call. This is part of the translation to TAC and happens in both
pipelines.

C leaves the evaluations of an assignment's two operands unsequenced
(C23 §6.5.17.1), and a call is indeterminately sequenced with the
evaluations around it (§6.5.3.3), so the left operand may be read
before the call or after it. If the call changes the pointer the
left operand goes through, or the member it names, either order is a
result C allows.

## Example

```c
#include <stdint.h>
struct mob { uint8_t pad[10]; uint8_t level; };
extern struct mob *E [[c6502::zeropage(0xF8)]];
uint8_t h(uint8_t x);
void f(uint8_t x) { E->level |= h(x); }
```

### Before

```
f:
   STA   __local_f__x
   LDA   E
   CLC
   ADC   #$0A
   STA   __local_f__1
   LDA   E+1
   ADC   #$00
   STA   __local_f__2
   LDY   #$00
   LDA   (__local_f__1),Y
   STA   __local_f__0
   LDA   __local_f__x
   JSR   h
   ORA   __local_f__0
   LDY   #$00
   STA   (__local_f__1),Y
   RTS
```

### After

```
f:
   JSR   h
   LDY   #$0A
   ORA   (E),Y
   STA   (E),Y
   RTS
```

After the call, the address `E + 10` is `(E),Y` with Y = 10.
[Commuted ADC, AND, and ORA](adc_commute.md) then reads the member in
the `ORA`, where the value from `h` was stored and reloaded.

## When it applies

- The left operand is a subscript, a dereference, or a member through
  `.` or `->`. A plain variable is read where the operation reads it,
  after the right operand, already.
- The right operand contains a function call.
- The left operand contains no call, assignment, increment, or
  decrement, so moving its evaluation after the call moves no side
  effect, and its type isn't `volatile`.

## What it leaves alone

- `f()->m += g()`, where both operands call, and `a[i++] += g()`,
  where the left operand writes `i`.
- A right operand with no call. Reading the left operand first then
  keeps nothing across a call.
