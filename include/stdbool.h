/* stdbool.h — boolean type and values for c6502 (C99 §7.16).
 *
 * `bool` is `_Bool`, one byte holding 0 or 1. Converting any scalar
 * to it gives 0 if the value compares equal to 0 and 1 otherwise
 * (C99 §6.3.1.2), so `bool b = 256;` stores 1.
 *
 * C99 §7.16.2 makes `bool` a macro (one that expands to `_Bool`),
 * so it stays a macro here rather than a typedef. It can be
 * `#undef`'d.
 */
#ifndef _STDBOOL_H
#define _STDBOOL_H

#define bool    _Bool
#define true    1
#define false   0

#define __bool_true_false_are_defined 1

#endif /* _STDBOOL_H */
