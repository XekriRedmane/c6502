/* limits.h — implementation limits for c6502 (C99 §7.10).
 *
 * c6502's integer model:
 *   char  / signed char  / unsigned char  : 1 byte
 *   int   / unsigned int                  : 2 bytes (the C99 §5.2.4.2.1
 *                                          minimum required widths)
 *   long  / unsigned long                 : 4 bytes
 *   long long / unsigned long long        : 8 bytes
 *
 * Plain `char` is unsigned in c6502 — an implementation choice
 * permitted by C99 §6.2.5.15 — so CHAR_MIN / CHAR_MAX match
 * UCHAR_MIN / UCHAR_MAX rather than SCHAR_MIN / SCHAR_MAX.
 *
 * `short` is not modeled as a distinct type: c6502's `int` is
 * already at the minimum-required width, so a separate `short`
 * would only repeat `int`. SHRT_MIN / SHRT_MAX / USHRT_MAX are
 * still defined (they're C99-mandated integer constants, useful
 * in portable constant-expression code) and alias INT_MIN /
 * INT_MAX / UINT_MAX.
 */
#ifndef _LIMITS_H
#define _LIMITS_H

#define CHAR_BIT 8

#define SCHAR_MIN (-128)
#define SCHAR_MAX 127
#define UCHAR_MAX 255

/* plain char is unsigned in c6502 */
#define CHAR_MIN 0
#define CHAR_MAX UCHAR_MAX

#define MB_LEN_MAX 1

/* int = 2 bytes signed */
#define INT_MAX 32767
#define INT_MIN (-INT_MAX - 1)
#define UINT_MAX 65535U

/* short is not a distinct type — alias int */
#define SHRT_MIN INT_MIN
#define SHRT_MAX INT_MAX
#define USHRT_MAX UINT_MAX

/* long = 4 bytes signed */
#define LONG_MAX 2147483647L
#define LONG_MIN (-LONG_MAX - 1L)
#define ULONG_MAX 4294967295UL

/* long long = 8 bytes signed */
#define LLONG_MAX 9223372036854775807LL
#define LLONG_MIN (-LLONG_MAX - 1LL)
#define ULLONG_MAX 18446744073709551615ULL

#endif /* _LIMITS_H */
