/* stddef.h — C23's common definitions for c6502 (C23 §7.21).
 *
 *   ptrdiff_t   = int            (the type the subtraction of two
 *                                 pointers gives in c6502)
 *   size_t      = unsigned long  (the type `sizeof` gives in c6502)
 *   max_align_t = char           (c6502 puts every object and every
 *                                 struct member at any byte address,
 *                                 so the greatest alignment is 1)
 *   wchar_t     = int            (c6502 has no wide character
 *                                 literals, so nothing produces one)
 *   nullptr_t   = the type of `nullptr`
 *
 * C23 declares `nullptr_t` as `typedef typeof_unqual(nullptr)
 * nullptr_t;` (§7.21.2). c6502 has no `typeof`, so the header names
 * the type with c6502's keyword `__c6502_nullptr_t`, which is in the
 * namespace reserved for the implementation. A `nullptr_t` is 2
 * bytes, all zero, and converts to any pointer type and to `bool`.
 *
 * `NULL` is `((void *)0)`, the null pointer constant that C99
 * programs were written for. It compares equal to both `0` and `nullptr` (§6.5.10p6), it
 * converts to any object pointer and to `nullptr_t`, and it can't be
 * assigned to an integer.
 *
 * `offsetof(type, member)` takes the address of the member of an
 * object at address 0. The type checker folds that to a constant, so
 * it's an integer constant expression in a static initializer, a
 * case label, and code. An array size or an enumeration value can't
 * use it, since the parser folds those before it knows any struct's
 * layout, as it does for `sizeof(struct s)`.
 *
 * `unreachable()` expands to `((void)0)`. Reaching it is undefined
 * behavior (§7.21.1), and c6502 doesn't use it to optimize.
 */
#ifndef _STDDEF_H
#define _STDDEF_H

#define __STDC_VERSION_STDDEF_H__ 202311L

typedef int ptrdiff_t;
typedef unsigned long size_t;
typedef char max_align_t;
typedef int wchar_t;
typedef __c6502_nullptr_t nullptr_t;

#define NULL ((void *)0)
#define unreachable() ((void)0)
#define offsetof(type, member) ((size_t)&((type *)0)->member)

#endif /* _STDDEF_H */
