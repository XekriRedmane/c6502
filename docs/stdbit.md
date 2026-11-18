# &lt;stdbit.h&gt;

C23's `<stdbit.h>` (C23 §7.18) counts and finds bits in an unsigned
integer and rounds it to a power of 2. c6502 provides all 14 of its
operations, each in the form for one type, such as
`stdc_leading_zeros_uc`, and in the type-generic form, such as
`stdc_leading_zeros`. The header also defines the version and endian
macros.

The compiler implements each function itself, as a builtin
(builtin_functions.py). A call with a constant argument is folded to
its result, and any other call becomes a short loop written in place,
so no library is needed and none is linked.

```c
#include <stdbit.h>

uint8_t mask;

uint8_t lowest_free_slot(void) {
    /* The index of the lowest 0 bit, from 1, or 0 when all 8 are set. */
    return stdc_first_trailing_zero(mask);
}
```

## The two forms

The forms with a suffix take a parameter of one type. The suffixes are
`_uc`, `_us`, `_ui`, `_ul`, and `_ull`, for `unsigned char`,
`unsigned short`, `unsigned int`, `unsigned long`, and
`unsigned long long`. The argument is converted to that type as it
would be for any function with a prototype, so
`stdc_count_ones_uc(0x1FF)` counts the ones of `0xFF`, and a pointer
argument is an error.

The type-generic form takes the type of its argument, which has to be
one of those five types or an `unsigned _BitInt(N)` whose width N is
that of one of them, 8, 16, 32, or 64 (§7.18.3p2). No integer promotion is applied,
so `stdc_leading_zeros(c)` on a `uint8_t` counts within 8 bits. The
other side of that is that an expression such as `c + 1`, or a plain
constant such as `5`, has type `int` and is rejected. A cast, a `u`
suffix, or a form with a suffix gives it an unsigned type. `bool`,
plain `char`, and the signed types are rejected too, since none of
them is an unsigned integer type in C23's sense.

`stdc_bit_floor` and `stdc_bit_ceil` return a value of the argument's
type, which is an `unsigned _BitInt(16)` when the argument is one,
`stdc_has_single_bit` returns a `bool`, and the rest return an
`unsigned int`. C23 gives the forms with a suffix those types. For the
type-generic forms it specifies "a suitable large unsigned integer
type", and c6502 picks `unsigned int` for them, as GCC and Clang do.

## What each one returns

Each result is for a value of N bits, the width of its type. The *most
significant index* of a bit counts from the top bit, 0, down to N - 1,
and the *least significant index* counts the other way (§7.18.1p3-4).
Every function is defined for every value, 0 and the all-ones value
included.

| Function | Returns | For 0 | For all ones |
|---|---|---|---|
| `stdc_leading_zeros` | the number of 0 bits above the highest 1 | N | 0 |
| `stdc_leading_ones` | the number of 1 bits above the highest 0 | 0 | N |
| `stdc_trailing_zeros` | the number of 0 bits below the lowest 1 | N | 0 |
| `stdc_trailing_ones` | the number of 1 bits below the lowest 0 | 0 | N |
| `stdc_first_leading_zero` | the most significant index of the highest 0, plus 1, or 0 when there's none | 1 | 0 |
| `stdc_first_leading_one` | the most significant index of the highest 1, plus 1, or 0 | 0 | 1 |
| `stdc_first_trailing_zero` | the least significant index of the lowest 0, plus 1, or 0 | 1 | 0 |
| `stdc_first_trailing_one` | the least significant index of the lowest 1, plus 1, or 0 | 0 | 1 |
| `stdc_count_zeros` | the number of 0 bits | N | 0 |
| `stdc_count_ones` | the number of 1 bits | 0 | N |
| `stdc_has_single_bit` | `true` if and only if exactly one bit is 1 | `false` | `false` |
| `stdc_bit_width` | 0 for 0, and otherwise 1 + log2 of the value, the number of bits it needs | 0 | N |
| `stdc_bit_floor` | 0 for 0, and otherwise the largest power of 2 not above the value | 0 | 2^(N-1) |
| `stdc_bit_ceil` | the smallest power of 2 not below the value, or 0 when that doesn't fit in N bits | 1 | 0 |

So `stdc_first_leading_one(x)` is `stdc_leading_zeros(x) + 1` for any
`x` that isn't 0, and `stdc_first_trailing_one(x)` is
`stdc_trailing_zeros(x) + 1`. `stdc_bit_ceil` of 0 is 1, which is
2<sup>0</sup>, the smallest power of 2. Of an `unsigned char` above
128 it's 0 (§7.18.16p2).

| Call | Result |
|---|---|
| `stdc_leading_zeros_uc(0x10)` | 3 |
| `stdc_leading_zeros_ui(0x10)` | 11 |
| `stdc_trailing_ones((uint8_t)0x0F)` | 4 |
| `stdc_first_trailing_zero((uint8_t)0x0F)` | 5 |
| `stdc_bit_width(1000u)` | 10 |
| `stdc_bit_floor((uint8_t)200)` | 128 |
| `stdc_bit_ceil((uint8_t)200)` | 0 |
| `stdc_bit_ceil((uint16_t)200)` | 256 |

`builtin_functions.stdbit` computes the same results, for constant
folding, and `tests/test_stdbit.py` checks it against a second model
written from §7.18's wording one bit index at a time.

## Constants

When the argument is an integer constant expression, the compiler
computes the result itself, in both pipelines, and writes no loop. C23
§6.6p3 keeps function calls out of constant expressions, whatever
their arguments, but c6502 treats a call of one of these builtins with
a constant argument as an integer constant expression, as it does the
BCD builtins (docs/builtins.md). The result can then initialize a
static object, label a `case`, or size an array.

```c
static uint8_t table[stdc_bit_ceil(100u)];      /* 128 bytes */
static const unsigned bits = stdc_bit_width(1000u);   /* 10 */
```

A call whose argument becomes a constant only once the optimizer has
propagated a value into it isn't folded, and runs its loop.

## The code they compile to

Each call becomes a loop on a copy of the value, which shifts the copy
a bit at a time until it's zero, and counts the shifts in a byte. That
is the same shape for every width, and it takes less code than a table
of results, which would be 256 bytes for one byte of the value.

- `bit_width` counts right shifts, and `leading_zeros` counts them
  down from N. `trailing_zeros` counts left shifts down from N, since
  a value with k trailing zeros becomes zero after N - k of them.
- `leading_ones` and `trailing_ones` do the same on the complement of
  the value.
- Each `first_...` function counts down from N + 1, after a test that
  gives 0 for a value with no bit to find.
- `count_ones` and `count_zeros` test the low bit of each step before
  shifting it out.
- `bit_floor` and `bit_ceil` shift a result left from 1 once for each
  right shift of the value (of `v >> 1` for the floor and of `v - 1`
  for the ceiling). The ceiling's result shifts past the top bit, to
  0, when its power of 2 doesn't fit.
- `has_single_bit` of one or two bytes is `v != 0 && (v & (v - 1)) ==
  0`. Of four or eight bytes, it shifts the value right past its
  lowest 1, and the value had a single 1 bit if and only if nothing is
  left, which takes less code than the subtraction.

With `-O`, `r = stdc_leading_zeros(c);` for an `unsigned char` `c` and
an `unsigned int` `r` is this.

```
    LDA c
    STA t               ; the copy that's shifted
    LDA #$08
    STA n               ; the count, from N down
    JMP .test
.body:
    LSR t
    DEC n
.test:
    LDA t
    BNE .body
    LDA n
    STA r
    LDA #$00            ; the high byte of the unsigned int
    STA r+1
```

`t` and `n` are bytes of the function's zero-page locals. The loop
runs once for each bit from the lowest up to the highest 1, at 16
cycles a pass, so a byte takes at most 8 passes.

These are the sizes in bytes of the code for each call with `-O`, for
an argument and a result in absolute memory, beyond the code for the
plain assignment `r = g` of the argument to the result.

| Function | `unsigned char` | `unsigned int` | `unsigned long` | `unsigned long long` |
|---|---|---|---|---|
| `leading_zeros`, `trailing_zeros`, `bit_width` | 25 | 36 | 60 | 108 |
| `leading_ones`, `trailing_ones` | 27 | 40 | 68 | 124 |
| `first_leading_one`, `first_trailing_one` | 32 | 49 | 79 | 139 |
| `first_leading_zero`, `first_trailing_zero` | 34 | 50 | 87 | 155 |
| `count_ones`, `count_zeros` | 28 | 42 | 66 | 114 |
| `has_single_bit` | 16 | 45 | 79 | 139 |
| `bit_floor` | 36 | 65 | 115 | 215 |
| `bit_ceil` | 38 | 77 | 141 | 272 |

The loop of an N-bit value runs up to N times, so the cost in time is
O(N) passes of a shift over every byte. A function of an `unsigned
long long` can take a few thousand cycles. No call is a call of a
runtime helper. Each call writes its own loop, so a program that
calls one function in many places can save code by wrapping it in a
function of its own, or by calling the function itself as below.

## The functions themselves

C23 §7.1.4 lets a program take the address of a library function, or
call the function with its macro suppressed, as in
`(stdc_count_ones_uc)(x)` or after `#undef stdc_count_ones_uc`. So the
header declares each form with a suffix as a function, and when a unit
refers to one of them other than through its macro, the compiler
defines the function in that unit, as a function whose body is the
builtin (`passes/stdbit_functions.py`).

```c
#include <stdbit.h>

static unsigned (*const count[])(unsigned char) = {
    stdc_count_ones_uc, stdc_count_zeros_uc,
};
```

The definition has internal linkage, since c6502 has no library to
hold one copy. Each unit that refers to a function has its own copy,
so a pointer to it taken in one unit compares unequal to a pointer
taken in another. C23 has a single function with external linkage
there, and that's the one way this differs from the standard. A
program that never refers to a function gets no code for it, and the
declarations of the ones it doesn't use are dropped before they reach
the output.

The type-generic forms are macros only, which §7.18.1p5 allows. Their
address can't be taken, and `(stdc_count_ones)(x)` is an error.

## The other names in the header

`__STDC_VERSION_STDBIT_H__` is `202311L`. `__STDC_ENDIAN_LITTLE__` is
1234 and `__STDC_ENDIAN_BIG__` 4321, the values GCC and Clang give
their `__ORDER_LITTLE_ENDIAN__` and `__ORDER_BIG_ENDIAN__`. The 6502
stores the low byte of a value first, so `__STDC_ENDIAN_NATIVE__` is
`__STDC_ENDIAN_LITTLE__`.

§7.18.1p1 makes `size_t` and the exact-width and minimum-width integer
types, `uint8_t` and `int_least16_t` and the rest, available through
the header, so it declares them as `<stddef.h>` and `<stdint.h>` do.
C23 lets a typedef be declared again with the same type, so those
headers can come before or after it. It defines none of their other
names, such as `UINT8_MAX` or `NULL`.

## What the compiler rejects

| Mistake | Why it's an error |
|---|---|
| An argument of the type-generic form that isn't `unsigned char`, `unsigned short`, `unsigned int`, `unsigned long`, `unsigned long long`, or `unsigned _BitInt(N)` for N of 8, 16, 32, or 64 | §7.18 defines the functions on the unsigned integer types other than `bool`, and on the bit-precise unsigned types of a standard type's width. An `int`, such as `c + 1` or `5`, is one of the types rejected, and so is `unsigned _BitInt(12)`. |
| An argument of a form with a suffix that can't be converted to its parameter, such as a pointer | The same rule as a call of any function with a prototype. |
| A call with other than one argument | Each function takes one value. |
| A declaration of a builtin's name `__c6502_stdc_...`, or a use of it other than in a call | The name is the implementation's. The `stdc_...` names are the program's own when it doesn't include the header, as §7.1.3 allows. |

## Limits

In the unoptimized pipeline each call's temporaries get frame bytes of
their own, so a function with many calls on `unsigned long long`
values can pass the frame's limit of 253 bytes.
