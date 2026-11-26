# Builtins

A builtin is a name the compiler implements itself. A call of a
builtin looks like a function call, but there's no function behind
it. The compiler types the call by the builtin's own rules and
writes the code for it in place. The names begin with two
underscores, which C99 §7.1.3 reserves for the implementation, so a
program's own names can't collide with them. A program can't declare
one of them, and can't use one as a value, such as by taking its
address.

The three builtins this document describes work with packed BCD in
the 6502's decimal mode. The functions of C23's `<stdbit.h>` are
builtins too, named `__c6502_stdc_...`, and
[stdbit.md](stdbit.md) describes them.

| Builtin | Computes |
|---|---|
| `__c6502_bcd_add(a, b)` | `a + b` in packed BCD |
| `__c6502_bcd_sub(a, b)` | `a - b` in packed BCD |
| `__c6502_bin_to_bcd(x)` | the 16-bit `x` as five packed BCD digits |

The first two are described up to "Limits", and the third under
"Converting a number to packed BCD".

## Packed BCD

*Packed BCD* (binary-coded decimal) holds two decimal digits in each
byte, one in each nibble, with the higher digit in the high nibble.
The byte `0x47` holds 47. A value of several bytes keeps its low byte
first, like every c6502 integer, so the two bytes of the
`uint16_t` 0x0250 hold the digits 0250, and the number is 250. A
`uint32_t` holds eight digits.

The 6502 adds and subtracts such numbers directly. With its decimal
flag set (`SED`), `ADC` and `SBC` treat each byte as two decimal
digits and carry or borrow at 100. Games keep their scores this way,
since drawing a decimal digit is then a matter of shifting out a
nibble, where a binary score has to be divided by ten for each digit.

## Types

Each operand has one of the unsigned integer types `unsigned char`,
`unsigned short`, `unsigned int`, `unsigned long`, and
`unsigned long long` (so `uint8_t` through `uint64_t`). An integer
constant expression whose value isn't negative may have any integer
type, which lets a plain `0x75`, whose type is `int`, be an operand.

The result has the operand type of higher rank, which is what the
usual arithmetic conversions give without the integer promotions. Two
`uint8_t` operands give a `uint8_t`, and a `uint8_t` and a `uint16_t`
give a `uint16_t`, with the `uint8_t` zero-extended. A constant of
signed type takes the other operand's type, and has to fit in it. Two
such constants give the first of `unsigned int`, `unsigned long`, and
`unsigned long long` that holds both.

| Operands | Result type |
|---|---|
| `uint8_t`, `uint8_t` | `uint8_t` |
| `uint8_t`, `uint16_t` | `uint16_t` |
| `uint32_t`, `0x0250` | `uint32_t` |
| `uint8_t`, `0x75` | `uint8_t` |
| `uint8_t`, `0x75u` | `unsigned int`, since `0x75u` has that type |
| `0x99`, `1` | `unsigned int` |

The integer promotions are left out so that a byte of BCD stays a
byte. Under the usual rules a `uint8_t` operand would become a 16-bit
`int`, and the carry out of the byte would land in a second byte of
the result instead of being dropped.

The compiler rejects an operand of signed type (unless it's a
nonnegative constant), a floating operand, a pointer or array, a
structure or union, a `bool`, and a plain `char`. A `bool` can't
hold a BCD result. Plain `char` holds characters, and C99 §6.2.5
lists it among neither the signed nor the unsigned integer types, so
a byte of BCD is an `unsigned char`.

## What they compute

`__c6502_bcd_add(a, b)` is `a + b`, and `__c6502_bcd_sub(a, b)` is
`a - b`, each computed a byte at a time from the low byte up, as the
6502's `CLC` (or `SEC`) and a chain of `ADC` (or `SBC`) compute it. The
carry or borrow out of the top byte is dropped. For operands whose
digits are all 0-9, the result is the sum or difference modulo
100<sup>n</sup>, where n is the number of bytes in the result type.

| Call | Result |
|---|---|
| `__c6502_bcd_add((uint8_t)0x38, (uint8_t)0x45)` | `0x83` |
| `__c6502_bcd_add((uint8_t)0x99, (uint8_t)0x01)` | `0x00`, since 99 + 1 wraps at 100 |
| `__c6502_bcd_sub((uint8_t)0x00, (uint8_t)0x01)` | `0x99`, since 0 - 1 borrows |
| `__c6502_bcd_add((uint16_t)0x0999, 1)` | `0x1000` |
| `__c6502_bcd_sub((uint32_t)0, 1)` | `0x99999999` |

### Digits that aren't decimal

An operand with a nibble above 9 isn't valid BCD, and for such an
operand the result is whatever the NMOS 6502's decimal mode produces.
`__c6502_bcd_add((uint8_t)0x0F, (uint8_t)0x0F)` is `0x14`, for
example. The compiler folds constants, and the simulator and the TAC
interpreter compute, with the same model of the CPU, copied from
py65's `opADC` and `opSBC` (`adc_byte` and `sbc_byte` in
cpp/src/builtin_functions.h, with a Python copy in
builtin_functions.py). `tests/test_bcd_builtins.py` checks the Python
copy against py65 for every pair of bytes and both carries. For the accumulator and
the carry, py65 gives what the NMOS algorithm in Appendix A of
6502.org's "Decimal Mode" tutorial gives. That comes from comparing
the two algorithms, and it hasn't been checked on an NMOS 6502. The
same tutorial gives a different `SBC` for the 65C02, so on a 65C02 a
result for invalid digits may differ from the one the compiler
folds, which hasn't been tried either.

## Constants

When both operands are constants, the compiler computes the result
itself, in both pipelines, and no decimal-mode code is written. The
optimizer also folds a call whose operands become constants after
propagation. A call with constant operands is an integer constant
expression, so it can initialize a static object, label a `case`, or
size an array.

```c
static uint16_t bonus = __c6502_bcd_add(0x0999, 1);   /* 0x1000 */
```

## The code they compile to

Each call becomes one chain of decimal-mode arithmetic.

```
    SED
    CLC                 ; SEC for __c6502_bcd_sub
    LDA a               ; byte 0
    ADC b               ; SBC for __c6502_bcd_sub
    STA result
    LDA a+1             ; byte 1, and so on up
    ADC b+1
    STA result+1
    CLD
```

That's 3 bytes plus 6 per byte of the result when the operands are in
zero page. Each operand is read where it lives, as a zero-page or
absolute byte, an immediate (`ADC #$00` for the high bytes of a
zero-extended operand), or a soft-stack slot, whose access begins
with `LDY #off`. An array element at a variable index is copied into
a temporary byte first, and so is a value the optimizer would
otherwise keep in a register, since no operand of the chain is
addressed through X or Y. With `-O`, an assignment of the result to an
operand's own bytes, as in `score = __c6502_bcd_add(score, points)`,
writes them in place, for a global, a static object's member, or an
element at a constant index. Also with `-O`, the bytes of the result
that nothing reads are left out of the chain, from the top down.

## The decimal flag

The decimal flag is set only across the `ADC` or `SBC` chain. The
`SED` comes right before the `CLC` or `SEC`, and the `CLD` right after
the last `STA`. No other instruction runs between them. That leaves
out an address computation, which would use `ADC`, a call, and any
instruction the compiler adds for its own purposes, such as a spill.

The compiler arranges this by keeping the whole chain as one
instruction (`DecimalArith`) through every pass that works on
instructions, so none of those passes can put code inside the chain
or move part of it out. The chain is spelled out as single
instructions only on the way to the assembler's text. With
`C6502_CHECK_IR=1`, a check after that step fails the compile when a
`SED` isn't followed by exactly that shape and a `CLD`, or when a
`CLD` appears anywhere else.

Everywhere outside these chains, compiled code computes correctly
only with the decimal flag clear. The Apple II Monitor's reset
routine clears it.

### Interrupts

An NMOS 6502 doesn't clear the decimal flag when it takes an
interrupt. An interrupt that arrives during a chain runs its handler
with the flag set, and every `ADC` and `SBC` in the handler then
computes in decimal, which breaks ordinary arithmetic and address
computations. A program with interrupt handlers that do arithmetic
must clear the flag at the start of each handler, with `CLD` as its
first instruction (the `RTI` at the end restores the flag), or else
mask interrupts around each call of a builtin. In a handler that
calls compiled code, the `CLD` goes before the call, since the
compiled code computes in binary only with the flag clear. The 65C02
clears the flag itself when it takes an interrupt.

## Example: a score

Lode Runner keeps its score in four bytes of zero page, eight BCD
digits, and adds the points for each piece of gold or each guard as a
BCD constant. `examples/lode_runner/screen.c` does it this way.

```c
#include <stdint.h>

/* Eight BCD digits, the tens and units in SCORE[0]. */
extern uint8_t SCORE [[c6502::zeropage(0x8E)]][4];

/* Adds a BCD number of points, 0x0250 for 250, to SCORE. */
void add_to_score(uint16_t bcd_points) {
    uint32_t *score = (uint32_t *)SCORE;
    *score = __c6502_bcd_add(*score, bcd_points);
}
```

`SCORE` stays an array of bytes, because the code that draws the
score reads it a byte, two digits, at a time. The pointer gives the
addition all four bytes as one `uint32_t`. With `-O`, `bcd_points`
arrives in A and X, and `add_to_score` stores it in two bytes of its
own, since the chain reads no operand from a register. The rest is the
chain.

```
add_to_score:
    STA __local_add_to_score__bcd_points_0
    STX __local_add_to_score__bcd_points_1
    SED
    CLC
    LDA SCORE
    ADC __local_add_to_score__bcd_points_0
    STA SCORE
    LDA SCORE+1
    ADC __local_add_to_score__bcd_points_1
    STA SCORE+1
    LDA SCORE+2
    ADC #$00
    STA SCORE+2
    LDA SCORE+3
    ADC #$00
    STA SCORE+3
    CLD
    RTS
```

The score wraps from 99999999 to 0. To stop it at the top, a program
can compare the result with the old score as unsigned integers. Packed
BCD of valid digits orders the same way as the numbers it holds, so a
sum that wrapped is smaller than the score it started from.

## What the compiler rejects

| Mistake | Why it's an error |
|---|---|
| A call with other than two arguments | Each builtin takes two operands. |
| An operand of signed type that isn't a nonnegative constant | A BCD digit has no sign, and a negative value would be taken as its 2's complement bytes. |
| A floating operand, a pointer or array, or a structure or union | Only an integer holds packed BCD. |
| A `bool` or plain `char` operand | A `bool` can't hold the result, and a byte of BCD is an `unsigned char`. |
| An operand of a bit-precise type, such as `unsigned _BitInt(8)` | The builtins take the standard unsigned types, whose sizes are the whole bytes of BCD digits. A cast to one gives the operand that type. |
| A negative constant | It has no BCD form. |
| A constant that doesn't fit in the result type | Its high digits would be dropped without a word. Cast the other operand to a wider type to widen the result. |
| A declaration of a builtin's name, as a function, an object, or a parameter | The name is the implementation's, and a call would still reach the builtin. |
| A builtin's name used other than in a call | There's no function whose address could be taken. |
| `[[c6502::save_hardware_stack]]` on a statement whose only call is a builtin | A builtin isn't a call, so there's nothing to save around. |

## Limits

The operands are integers. There's no BCD type, so nothing stops a
program from mixing a BCD value with binary arithmetic, as C would
allow with any other encoding kept in an integer.

A `register` local can't be an operand under `-O`. It has to stay in
Y, which the chain can't read, and the compile stops with an error
that says the value can't be pinned to Y. A `register` parameter can
be one, since it's copied into a byte of zero page when its register
won't do.

`ADC` and `SBC` leave N, Z, and V unreliable in an NMOS 6502's decimal
mode, and the compiler reads no flag the chain leaves. A test of the
result, such as `if (__c6502_bcd_add(a, b) == 0)`, loads the result
again and tests it in binary.

## Converting a number to packed BCD

`__c6502_bin_to_bcd(x)` gives the five decimal digits of a 16-bit
value in packed BCD, which is what a program needs to print a binary
number. Each digit is then a nibble to shift or mask out, where
printing from binary takes a division by ten for each digit.

| Call | Result |
|---|---|
| `__c6502_bin_to_bcd(0)` | `0x00000` |
| `__c6502_bin_to_bcd(1234)` | `0x01234` |
| `__c6502_bin_to_bcd(65535)` | `0x65535` |
| `__c6502_bin_to_bcd(-1)` | `0x65535`, since -1 converts to 65535 |
| `__c6502_bin_to_bcd(70000L)` | `0x04464`, since 70000 converts to 4464 |

### Types

The argument may have any integer type, and it's converted to
`unsigned int` as an assignment would convert it. A 16-bit value
passes unchanged, a narrower one is extended, a negative value is
taken modulo 65536 (so -1 is 65535), and a wider one keeps its low 16
bits (so 70000 is 4464). A floating argument, a pointer or array, and
a structure or union are errors, as is a call with other than one
argument.

The result is an `unsigned long`. Bits 0-3 hold the ones, bits 4-7
the tens, and so on up to the ten-thousands in bits 16-19. Bits 20-31
are 0. Seen as bytes, low byte first, the result is the ones and tens,
then the hundreds and thousands, then the ten-thousands, and a zero
byte.

### Constants

A constant argument gives a constant result, in both pipelines, and
no code is written for the call. The optimizer also folds a call
whose argument becomes a constant. A call of a constant is an integer
constant expression, so it can initialize a static object, label a
`case`, or size an array.

```c
static uint32_t limit = __c6502_bin_to_bcd(9999);   /* 0x09999 */
```

### The helper

Any other call stores the argument in `HARGS` and calls the runtime
helper `bin16_to_bcd`, which the linker adds to the program from the
runtime library (`--defsym` can give another address for it).

| `HARGS` bytes | What they hold |
|---|---|
| `+0`-`+1` | the value, low byte first, unchanged by the call |
| `+2` | the tens (high nibble) and the ones (low nibble) |
| `+3` | the thousands and the hundreds |
| `+4` | the ten-thousands in the low nibble, and 0 in the high nibble |
| `+5`-`+6` | a copy of the value, which the helper shifts to 0 |

The helper doubles a BCD number once for each bit of the value, from
the top bit down, with that bit as the carry into the doubling. After
the top n bits the BCD number holds the value of those n bits, so
after all 16 it holds the value. Doubling is an `ADC` of each BCD byte
to itself in decimal mode.

```
    LDA HARGS       ; copy the value to HARGS+5..6
    STA HARGS+5
    LDA HARGS+1
    STA HARGS+6
    SED
    LDA #0
    STA HARGS+2     ; the BCD number := 0
    STA HARGS+3
    STA HARGS+4
    LDX #16
loop:
    ASL HARGS+5     ; the top bit of the copy into C
    ROL HARGS+6
    LDA HARGS+2     ; the BCD number := 2 * itself + C
    ADC HARGS+2
    STA HARGS+2
    LDA HARGS+3
    ADC HARGS+3
    STA HARGS+3
    LDA HARGS+4
    ADC HARGS+4
    STA HARGS+4
    DEX
    BNE loop
    CLD
    RTS
```

The value is at most 65535, so `HARGS+4` is at most `$06` before its
last doubling, and no carry leaves it. The helper is 46 bytes, and a
call takes 712 cycles with its `JSR` and `RTS`, whatever the value,
or 727 when the loop's `BNE` crosses a page. The caller's code for the
call is the stores of the argument, the `JSR`, and a load and store
for each result byte it keeps.

The helper sets the decimal flag for its loop and clears it before it
returns. Compiled code otherwise sets the flag only across one
`DecimalArith` chain, and with `C6502_CHECK_IR=1` the check after
`asm_to_asm2` holds the helper to its own rule, a `CLD` right before
each `RTS`. An interrupt during the loop runs its handler with the
flag set, as described under "Interrupts" above.

### Example: printing a number

The program below prints a number with the Apple II Monitor's `COUT`,
with no leading zeros. Ali Baba's `num_to_decimal`
(examples/ali_baba/text.c) is written the same way, and dropped from
125 bytes to 96 when it changed from dividing by each power of ten to
this.

```c
#include <stdint.h>
#include <apple2/monitor_io.h>

/* Prints `n` in decimal with no leading zeros. Digit k of the BCD
 * number, 0 for the ones, is in the low nibble of packed[k / 2] for
 * an even k and in the high nibble for an odd one. */
void print_number(uint16_t n) {
    uint32_t bcd = __c6502_bin_to_bcd(n);
    uint8_t packed[3] = {
        (uint8_t)bcd, (uint8_t)(bcd >> 8), (uint8_t)(bcd >> 16),
    };
    bool started = false;
    for (uint8_t k = 5; k-- != 0;) {
        uint8_t byte = packed[k / 2];
        uint8_t digit = (k & 1) ? byte >> 4 : byte & 0x0F;
        if (digit != 0 || started || k == 0) {
            COUT(0xB0 + digit);     /* '0' is $B0 in high-bit ASCII */
            started = true;
        }
    }
}
```

With `-O` it's 81 bytes. The three result bytes go to a local array,
which the loop indexes with Y, `LDA packed,Y`, after `LSR` puts the
low bit of `k` in C to pick the nibble.

```
print_number:
    STA   HARGS
    STX   HARGS+1
    JSR   bin16_to_bcd
    LDA   HARGS+2
    STA   __local_print_number__packed
    LDA   HARGS+3
    STA   __local_print_number__packed+1
    LDA   HARGS+4
    STA   __local_print_number__packed+2
    ...
.print_number@rot@0:
    LSR
    TAY
    LDA   __local_print_number__packed,Y
    BCC   .cond_else@0
    LSR
    LSR
    LSR
    LSR
    ...
```

Shifting the whole `uint32_t` left by 4 for each digit reads more
simply, but a 32-bit shift by 4 calls the helper `asl32`, and the
function grows to 107 bytes.
