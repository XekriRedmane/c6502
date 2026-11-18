/* stdbool.h — C23's boolean header for c6502 (C23 §7.19).
 *
 * `bool`, `true`, and `false` are keywords in C23 (§6.4.1), so this
 * header doesn't define them as macros, as C99's did. `bool` is one
 * byte holding 0 or 1. Converting any scalar to it gives 0 if the
 * value compares equal to 0 and 1 otherwise (§6.3.1.2), so
 * `bool b = 256;` stores 1. `true` and `false` are constants of type
 * `bool` with the values 1 and 0 (§6.4.4.6).
 *
 * The one macro left is `__bool_true_false_are_defined`, which
 * expands to 1. C23 makes it obsolescent.
 */
#ifndef _STDBOOL_H
#define _STDBOOL_H

#define __bool_true_false_are_defined 1

#endif /* _STDBOOL_H */
