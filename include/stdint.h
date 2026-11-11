/* stdint.h — fixed-width integer types for c6502 (C99 §7.18).
 *
 * Mapping to c6502's integer model:
 *   int8_t   = signed char            (1 byte, -128..127)
 *   uint8_t  = unsigned char          (1 byte, 0..255)
 *   int16_t  = int                    (2 bytes, -32768..32767)
 *   uint16_t = unsigned int           (2 bytes, 0..65535)
 *   int32_t  = long                   (4 bytes, -2^31..2^31-1)
 *   uint32_t = unsigned long          (4 bytes, 0..2^32-1)
 *   int64_t  = long long              (8 bytes, -2^63..2^63-1)
 *   uint64_t = unsigned long long     (8 bytes, 0..2^64-1)
 *
 * `_least` and `_fast` types alias the matching exact-width types.
 * On the 6502 the "fastest" choice for each width is just the
 * exact-width type — wider types take more bytes and more cycles
 * regardless of the underlying ALU.
 *
 *   intptr_t  = int                   (6502 addresses are 16 bits)
 *   uintptr_t = unsigned int
 *   intmax_t  = long long             (widest integer modeled)
 *   uintmax_t = unsigned long long
 *
 * `size_t`, `sig_atomic_t`, `wchar_t`, `wint_t` aren't modeled, so
 * `SIZE_MAX`, `SIG_ATOMIC_*`, `WCHAR_*`, `WINT_*` aren't defined
 * here.
 */
#ifndef _STDINT_H
#define _STDINT_H

/* exact-width integer types — §7.18.1.1 */
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef int int16_t;
typedef unsigned int uint16_t;
typedef long int32_t;
typedef unsigned long uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

/* minimum-width — §7.18.1.2 */
typedef int8_t int_least8_t;
typedef uint8_t uint_least8_t;
typedef int16_t int_least16_t;
typedef uint16_t uint_least16_t;
typedef int32_t int_least32_t;
typedef uint32_t uint_least32_t;
typedef int64_t int_least64_t;
typedef uint64_t uint_least64_t;

/* fastest minimum-width — §7.18.1.3 */
typedef int8_t int_fast8_t;
typedef uint8_t uint_fast8_t;
typedef int16_t int_fast16_t;
typedef uint16_t uint_fast16_t;
typedef int32_t int_fast32_t;
typedef uint32_t uint_fast32_t;
typedef int64_t int_fast64_t;
typedef uint64_t uint_fast64_t;

/* integer types capable of holding object pointers — §7.18.1.4 */
typedef int intptr_t;
typedef unsigned int uintptr_t;

/* greatest-width integer types — §7.18.1.5 */
typedef long long intmax_t;
typedef unsigned long long uintmax_t;

/* limits of exact-width integer types — §7.18.2.1 */
#define INT8_MIN            (-128)
#define INT8_MAX            127
#define UINT8_MAX           255

#define INT16_MAX           32767
#define INT16_MIN           (-INT16_MAX - 1)
#define UINT16_MAX          65535U

#define INT32_MAX           2147483647L
#define INT32_MIN           (-INT32_MAX - 1L)
#define UINT32_MAX          4294967295UL

#define INT64_MAX           9223372036854775807LL
#define INT64_MIN           (-INT64_MAX - 1LL)
#define UINT64_MAX          18446744073709551615ULL

/* limits of minimum-width — §7.18.2.2 */
#define INT_LEAST8_MIN      INT8_MIN
#define INT_LEAST8_MAX      INT8_MAX
#define UINT_LEAST8_MAX     UINT8_MAX
#define INT_LEAST16_MIN     INT16_MIN
#define INT_LEAST16_MAX     INT16_MAX
#define UINT_LEAST16_MAX    UINT16_MAX
#define INT_LEAST32_MIN     INT32_MIN
#define INT_LEAST32_MAX     INT32_MAX
#define UINT_LEAST32_MAX    UINT32_MAX
#define INT_LEAST64_MIN     INT64_MIN
#define INT_LEAST64_MAX     INT64_MAX
#define UINT_LEAST64_MAX    UINT64_MAX

/* limits of fastest minimum-width — §7.18.2.3 */
#define INT_FAST8_MIN       INT8_MIN
#define INT_FAST8_MAX       INT8_MAX
#define UINT_FAST8_MAX      UINT8_MAX
#define INT_FAST16_MIN      INT16_MIN
#define INT_FAST16_MAX      INT16_MAX
#define UINT_FAST16_MAX     UINT16_MAX
#define INT_FAST32_MIN      INT32_MIN
#define INT_FAST32_MAX      INT32_MAX
#define UINT_FAST32_MAX     UINT32_MAX
#define INT_FAST64_MIN      INT64_MIN
#define INT_FAST64_MAX      INT64_MAX
#define UINT_FAST64_MAX     UINT64_MAX

/* limits of integer types capable of holding object pointers — §7.18.2.4 */
#define INTPTR_MIN          INT16_MIN
#define INTPTR_MAX          INT16_MAX
#define UINTPTR_MAX         UINT16_MAX

/* limits of greatest-width — §7.18.2.5 */
#define INTMAX_MIN          INT64_MIN
#define INTMAX_MAX          INT64_MAX
#define UINTMAX_MAX         UINT64_MAX

/* limits of `ptrdiff_t` — §7.18.3.
 * c6502 uses `long` (4 bytes signed) as the result type of
 * pointer subtraction. */
#define PTRDIFF_MIN         INT32_MIN
#define PTRDIFF_MAX         INT32_MAX

/* macros for integer constants — §7.18.4 */
#define INT8_C(c)           c
#define INT16_C(c)          c
#define INT32_C(c)          c ## L
#define INT64_C(c)          c ## LL

#define UINT8_C(c)          c ## U
#define UINT16_C(c)         c ## U
#define UINT32_C(c)         c ## UL
#define UINT64_C(c)         c ## ULL

#define INTMAX_C(c)         c ## LL
#define UINTMAX_C(c)        c ## ULL

#endif /* _STDINT_H */
