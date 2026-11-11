/* stdbool.h — boolean type and values for c6502 (C99 §7.16).
 *
 * C99 defines `_Bool` as a distinct integer type large enough to
 * store the values 0 and 1. c6502 doesn't model `_Bool` as a
 * separate type, so `bool` is aliased to `unsigned char` here —
 * one byte, holds 0..255, same storage and ABI as any `unsigned
 * char` variable. Code that assigns arbitrary integer expressions
 * to a `bool` (e.g. `bool b = x;` where `x` is wider than a byte)
 * will get the low byte rather than the C99-required normalized
 * 0/1; if you need normalization, write `b = !!x;`.
 *
 * C99 §7.16.2 makes `bool` a macro (one that expands to `_Bool`),
 * so it stays a macro here rather than a typedef. It can be
 * `#undef`'d, and diagnostics quote the expanded form (`unsigned
 * char`) rather than `bool`.
 */
#ifndef _STDBOOL_H
#define _STDBOOL_H

#define bool    unsigned char
#define true    1
#define false   0

#define __bool_true_false_are_defined 1

#endif /* _STDBOOL_H */
