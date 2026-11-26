/* stdbit.h — C23's bit and byte utilities for c6502 (C23 §7.18).
 *
 * Each function is a compiler builtin (builtin_functions.py,
 * docs/stdbit.md). `stdc_leading_zeros_uc(x)` and the other names
 * with a suffix are macros for the builtins `__c6502_stdc_..._uc`,
 * which take an argument of the parameter's type, converted as for
 * a call, and the type-generic `stdc_leading_zeros(x)` and the rest
 * are macros for the builtins `__c6502_stdc_...`, which take the type
 * of their argument. That has to be unsigned char, unsigned short,
 * unsigned int, unsigned long, or unsigned long long, with no integer
 * promotion. The compiler folds a call of a constant argument, which
 * is then an integer constant expression, and writes a short loop for
 * any other call in place.
 *
 *   bit_floor, bit_ceil   return the argument's type
 *   has_single_bit        returns bool
 *   the rest              return unsigned int
 *
 * Each function with a suffix is also declared, so a program can take
 * its address, or call it with the macro suppressed, as in
 * `(stdc_leading_zeros_uc)(x)` (§7.1.4). The compiler then defines
 * the function in the unit, with internal linkage, since c6502 has no
 * library (cpp/src/passes/stdbit_functions.h). The type-generic names are
 * macros only, which §7.18.1p5 allows.
 *
 * The 6502 is little-endian, so __STDC_ENDIAN_NATIVE__ is
 * __STDC_ENDIAN_LITTLE__. The values 1234 and 4321 are the ones GCC
 * and Clang give their __ORDER_LITTLE_ENDIAN__ and
 * __ORDER_BIG_ENDIAN__.
 *
 * §7.18.1p1 makes size_t and the exact-width and minimum-width
 * integer types available here too, so they're declared as
 * <stddef.h> and <stdint.h> declare them. C23 allows a typedef to be
 * declared again with the same type, so either header can come
 * before or after this one. No other name from those headers is
 * declared.
 */
#ifndef _STDBIT_H
#define _STDBIT_H

#define __STDC_VERSION_STDBIT_H__ 202311L

#define __STDC_ENDIAN_LITTLE__ 1234
#define __STDC_ENDIAN_BIG__ 4321
#define __STDC_ENDIAN_NATIVE__ __STDC_ENDIAN_LITTLE__

typedef unsigned int size_t;

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef int int16_t;
typedef unsigned int uint16_t;
typedef long int32_t;
typedef unsigned long uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

typedef int8_t int_least8_t;
typedef uint8_t uint_least8_t;
typedef int16_t int_least16_t;
typedef uint16_t uint_least16_t;
typedef int32_t int_least32_t;
typedef uint32_t uint_least32_t;
typedef int64_t int_least64_t;
typedef uint64_t uint_least64_t;


/* §7.18.3, count leading zeros */
unsigned int stdc_leading_zeros_uc(unsigned char __value);
unsigned int stdc_leading_zeros_us(unsigned short __value);
unsigned int stdc_leading_zeros_ui(unsigned int __value);
unsigned int stdc_leading_zeros_ul(unsigned long __value);
unsigned int stdc_leading_zeros_ull(unsigned long long __value);
#define stdc_leading_zeros_uc(x) __c6502_stdc_leading_zeros_uc(x)
#define stdc_leading_zeros_us(x) __c6502_stdc_leading_zeros_us(x)
#define stdc_leading_zeros_ui(x) __c6502_stdc_leading_zeros_ui(x)
#define stdc_leading_zeros_ul(x) __c6502_stdc_leading_zeros_ul(x)
#define stdc_leading_zeros_ull(x) __c6502_stdc_leading_zeros_ull(x)
#define stdc_leading_zeros(x) __c6502_stdc_leading_zeros(x)

/* §7.18.4, count leading ones */
unsigned int stdc_leading_ones_uc(unsigned char __value);
unsigned int stdc_leading_ones_us(unsigned short __value);
unsigned int stdc_leading_ones_ui(unsigned int __value);
unsigned int stdc_leading_ones_ul(unsigned long __value);
unsigned int stdc_leading_ones_ull(unsigned long long __value);
#define stdc_leading_ones_uc(x) __c6502_stdc_leading_ones_uc(x)
#define stdc_leading_ones_us(x) __c6502_stdc_leading_ones_us(x)
#define stdc_leading_ones_ui(x) __c6502_stdc_leading_ones_ui(x)
#define stdc_leading_ones_ul(x) __c6502_stdc_leading_ones_ul(x)
#define stdc_leading_ones_ull(x) __c6502_stdc_leading_ones_ull(x)
#define stdc_leading_ones(x) __c6502_stdc_leading_ones(x)

/* §7.18.5, count trailing zeros */
unsigned int stdc_trailing_zeros_uc(unsigned char __value);
unsigned int stdc_trailing_zeros_us(unsigned short __value);
unsigned int stdc_trailing_zeros_ui(unsigned int __value);
unsigned int stdc_trailing_zeros_ul(unsigned long __value);
unsigned int stdc_trailing_zeros_ull(unsigned long long __value);
#define stdc_trailing_zeros_uc(x) __c6502_stdc_trailing_zeros_uc(x)
#define stdc_trailing_zeros_us(x) __c6502_stdc_trailing_zeros_us(x)
#define stdc_trailing_zeros_ui(x) __c6502_stdc_trailing_zeros_ui(x)
#define stdc_trailing_zeros_ul(x) __c6502_stdc_trailing_zeros_ul(x)
#define stdc_trailing_zeros_ull(x) __c6502_stdc_trailing_zeros_ull(x)
#define stdc_trailing_zeros(x) __c6502_stdc_trailing_zeros(x)

/* §7.18.6, count trailing ones */
unsigned int stdc_trailing_ones_uc(unsigned char __value);
unsigned int stdc_trailing_ones_us(unsigned short __value);
unsigned int stdc_trailing_ones_ui(unsigned int __value);
unsigned int stdc_trailing_ones_ul(unsigned long __value);
unsigned int stdc_trailing_ones_ull(unsigned long long __value);
#define stdc_trailing_ones_uc(x) __c6502_stdc_trailing_ones_uc(x)
#define stdc_trailing_ones_us(x) __c6502_stdc_trailing_ones_us(x)
#define stdc_trailing_ones_ui(x) __c6502_stdc_trailing_ones_ui(x)
#define stdc_trailing_ones_ul(x) __c6502_stdc_trailing_ones_ul(x)
#define stdc_trailing_ones_ull(x) __c6502_stdc_trailing_ones_ull(x)
#define stdc_trailing_ones(x) __c6502_stdc_trailing_ones(x)

/* §7.18.7, first leading zero */
unsigned int stdc_first_leading_zero_uc(unsigned char __value);
unsigned int stdc_first_leading_zero_us(unsigned short __value);
unsigned int stdc_first_leading_zero_ui(unsigned int __value);
unsigned int stdc_first_leading_zero_ul(unsigned long __value);
unsigned int stdc_first_leading_zero_ull(unsigned long long __value);
#define stdc_first_leading_zero_uc(x) __c6502_stdc_first_leading_zero_uc(x)
#define stdc_first_leading_zero_us(x) __c6502_stdc_first_leading_zero_us(x)
#define stdc_first_leading_zero_ui(x) __c6502_stdc_first_leading_zero_ui(x)
#define stdc_first_leading_zero_ul(x) __c6502_stdc_first_leading_zero_ul(x)
#define stdc_first_leading_zero_ull(x) __c6502_stdc_first_leading_zero_ull(x)
#define stdc_first_leading_zero(x) __c6502_stdc_first_leading_zero(x)

/* §7.18.8, first leading one */
unsigned int stdc_first_leading_one_uc(unsigned char __value);
unsigned int stdc_first_leading_one_us(unsigned short __value);
unsigned int stdc_first_leading_one_ui(unsigned int __value);
unsigned int stdc_first_leading_one_ul(unsigned long __value);
unsigned int stdc_first_leading_one_ull(unsigned long long __value);
#define stdc_first_leading_one_uc(x) __c6502_stdc_first_leading_one_uc(x)
#define stdc_first_leading_one_us(x) __c6502_stdc_first_leading_one_us(x)
#define stdc_first_leading_one_ui(x) __c6502_stdc_first_leading_one_ui(x)
#define stdc_first_leading_one_ul(x) __c6502_stdc_first_leading_one_ul(x)
#define stdc_first_leading_one_ull(x) __c6502_stdc_first_leading_one_ull(x)
#define stdc_first_leading_one(x) __c6502_stdc_first_leading_one(x)

/* §7.18.9, first trailing zero */
unsigned int stdc_first_trailing_zero_uc(unsigned char __value);
unsigned int stdc_first_trailing_zero_us(unsigned short __value);
unsigned int stdc_first_trailing_zero_ui(unsigned int __value);
unsigned int stdc_first_trailing_zero_ul(unsigned long __value);
unsigned int stdc_first_trailing_zero_ull(unsigned long long __value);
#define stdc_first_trailing_zero_uc(x) __c6502_stdc_first_trailing_zero_uc(x)
#define stdc_first_trailing_zero_us(x) __c6502_stdc_first_trailing_zero_us(x)
#define stdc_first_trailing_zero_ui(x) __c6502_stdc_first_trailing_zero_ui(x)
#define stdc_first_trailing_zero_ul(x) __c6502_stdc_first_trailing_zero_ul(x)
#define stdc_first_trailing_zero_ull(x) __c6502_stdc_first_trailing_zero_ull(x)
#define stdc_first_trailing_zero(x) __c6502_stdc_first_trailing_zero(x)

/* §7.18.10, first trailing one */
unsigned int stdc_first_trailing_one_uc(unsigned char __value);
unsigned int stdc_first_trailing_one_us(unsigned short __value);
unsigned int stdc_first_trailing_one_ui(unsigned int __value);
unsigned int stdc_first_trailing_one_ul(unsigned long __value);
unsigned int stdc_first_trailing_one_ull(unsigned long long __value);
#define stdc_first_trailing_one_uc(x) __c6502_stdc_first_trailing_one_uc(x)
#define stdc_first_trailing_one_us(x) __c6502_stdc_first_trailing_one_us(x)
#define stdc_first_trailing_one_ui(x) __c6502_stdc_first_trailing_one_ui(x)
#define stdc_first_trailing_one_ul(x) __c6502_stdc_first_trailing_one_ul(x)
#define stdc_first_trailing_one_ull(x) __c6502_stdc_first_trailing_one_ull(x)
#define stdc_first_trailing_one(x) __c6502_stdc_first_trailing_one(x)

/* §7.18.11, count zeros */
unsigned int stdc_count_zeros_uc(unsigned char __value);
unsigned int stdc_count_zeros_us(unsigned short __value);
unsigned int stdc_count_zeros_ui(unsigned int __value);
unsigned int stdc_count_zeros_ul(unsigned long __value);
unsigned int stdc_count_zeros_ull(unsigned long long __value);
#define stdc_count_zeros_uc(x) __c6502_stdc_count_zeros_uc(x)
#define stdc_count_zeros_us(x) __c6502_stdc_count_zeros_us(x)
#define stdc_count_zeros_ui(x) __c6502_stdc_count_zeros_ui(x)
#define stdc_count_zeros_ul(x) __c6502_stdc_count_zeros_ul(x)
#define stdc_count_zeros_ull(x) __c6502_stdc_count_zeros_ull(x)
#define stdc_count_zeros(x) __c6502_stdc_count_zeros(x)

/* §7.18.12, count ones */
unsigned int stdc_count_ones_uc(unsigned char __value);
unsigned int stdc_count_ones_us(unsigned short __value);
unsigned int stdc_count_ones_ui(unsigned int __value);
unsigned int stdc_count_ones_ul(unsigned long __value);
unsigned int stdc_count_ones_ull(unsigned long long __value);
#define stdc_count_ones_uc(x) __c6502_stdc_count_ones_uc(x)
#define stdc_count_ones_us(x) __c6502_stdc_count_ones_us(x)
#define stdc_count_ones_ui(x) __c6502_stdc_count_ones_ui(x)
#define stdc_count_ones_ul(x) __c6502_stdc_count_ones_ul(x)
#define stdc_count_ones_ull(x) __c6502_stdc_count_ones_ull(x)
#define stdc_count_ones(x) __c6502_stdc_count_ones(x)

/* §7.18.13, single-bit check */
bool stdc_has_single_bit_uc(unsigned char __value);
bool stdc_has_single_bit_us(unsigned short __value);
bool stdc_has_single_bit_ui(unsigned int __value);
bool stdc_has_single_bit_ul(unsigned long __value);
bool stdc_has_single_bit_ull(unsigned long long __value);
#define stdc_has_single_bit_uc(x) __c6502_stdc_has_single_bit_uc(x)
#define stdc_has_single_bit_us(x) __c6502_stdc_has_single_bit_us(x)
#define stdc_has_single_bit_ui(x) __c6502_stdc_has_single_bit_ui(x)
#define stdc_has_single_bit_ul(x) __c6502_stdc_has_single_bit_ul(x)
#define stdc_has_single_bit_ull(x) __c6502_stdc_has_single_bit_ull(x)
#define stdc_has_single_bit(x) __c6502_stdc_has_single_bit(x)

/* §7.18.14, bit width */
unsigned int stdc_bit_width_uc(unsigned char __value);
unsigned int stdc_bit_width_us(unsigned short __value);
unsigned int stdc_bit_width_ui(unsigned int __value);
unsigned int stdc_bit_width_ul(unsigned long __value);
unsigned int stdc_bit_width_ull(unsigned long long __value);
#define stdc_bit_width_uc(x) __c6502_stdc_bit_width_uc(x)
#define stdc_bit_width_us(x) __c6502_stdc_bit_width_us(x)
#define stdc_bit_width_ui(x) __c6502_stdc_bit_width_ui(x)
#define stdc_bit_width_ul(x) __c6502_stdc_bit_width_ul(x)
#define stdc_bit_width_ull(x) __c6502_stdc_bit_width_ull(x)
#define stdc_bit_width(x) __c6502_stdc_bit_width(x)

/* §7.18.15, bit floor */
unsigned char stdc_bit_floor_uc(unsigned char __value);
unsigned short stdc_bit_floor_us(unsigned short __value);
unsigned int stdc_bit_floor_ui(unsigned int __value);
unsigned long stdc_bit_floor_ul(unsigned long __value);
unsigned long long stdc_bit_floor_ull(unsigned long long __value);
#define stdc_bit_floor_uc(x) __c6502_stdc_bit_floor_uc(x)
#define stdc_bit_floor_us(x) __c6502_stdc_bit_floor_us(x)
#define stdc_bit_floor_ui(x) __c6502_stdc_bit_floor_ui(x)
#define stdc_bit_floor_ul(x) __c6502_stdc_bit_floor_ul(x)
#define stdc_bit_floor_ull(x) __c6502_stdc_bit_floor_ull(x)
#define stdc_bit_floor(x) __c6502_stdc_bit_floor(x)

/* §7.18.16, bit ceiling */
unsigned char stdc_bit_ceil_uc(unsigned char __value);
unsigned short stdc_bit_ceil_us(unsigned short __value);
unsigned int stdc_bit_ceil_ui(unsigned int __value);
unsigned long stdc_bit_ceil_ul(unsigned long __value);
unsigned long long stdc_bit_ceil_ull(unsigned long long __value);
#define stdc_bit_ceil_uc(x) __c6502_stdc_bit_ceil_uc(x)
#define stdc_bit_ceil_us(x) __c6502_stdc_bit_ceil_us(x)
#define stdc_bit_ceil_ui(x) __c6502_stdc_bit_ceil_ui(x)
#define stdc_bit_ceil_ul(x) __c6502_stdc_bit_ceil_ul(x)
#define stdc_bit_ceil_ull(x) __c6502_stdc_bit_ceil_ull(x)
#define stdc_bit_ceil(x) __c6502_stdc_bit_ceil(x)

#endif /* _STDBIT_H */
