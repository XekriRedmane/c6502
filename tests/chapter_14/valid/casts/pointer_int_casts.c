#ifdef SUPPRESS_WARNINGS
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#endif
/* Test casting between pointers and ints.
 * The behavior we test here is implementation-defined.
 * We follow GCC's behavior, defined here:
 * https://gcc.gnu.org/onlinedocs/gcc/Arrays-and-pointers-implementation.html
 */

// NOTE: converting an integer to a pointer is undefined behavior
// if the resulting pointer is misaligned. These integers' values
// are divisible by 8, so it's safe to cast them to any pointer type.
// (c6502 adaptation: 128 doesn't fit in c6502's 1-byte signed int,
// so we use 8 — still 8-aligned, still meaningful for the
// int↔pointer round-trip exercise in cast_int_round_trip.)
int i = 8;
long l = 8l;

int int_to_pointer(void) {
    int *a = (int *) i;
    int *b = (int *) l;
    return a == b;
}

int pointer_to_int(void) {
    static long l;
    long *ptr = &l;
    unsigned long ptr_as_long = (unsigned long) ptr;
    /* Upstream checked `% 8 == 0` since long is 8-byte-aligned on
     * x86-64; c6502's long is 2 bytes and statics don't have any
     * fixed alignment, so we just check that the cast produces a
     * usable non-null pointer-as-int. */
    return (ptr_as_long != 0);
}

/* Casts between pointer types and 64-bit integer types should round-trip.
 * Casting a 32-bit int to a pointer and back should also round trip.
 * (Casting a pointer to a 32-bit int usually won't round trip since the
 * upper bits are discarded; we don't cover that case here.)
 */

// long to pointer and back
int cast_long_round_trip(void) {
    int *ptr = (int *) l;
    long l2 = (long) ptr;
    return (l == l2);
}


// pointer to ulong and back
int cast_ulong_round_trip(void) {
    long *ptr = &l;
    unsigned long ptr_as_ulong = (unsigned long) ptr;
    long *ptr2 = (long *) ptr_as_ulong;
    return (ptr == ptr2);
}

// int to pointer and back
int cast_int_round_trip(void) {
    double *a = (double *)i;
    int i2 = (int) a;
    return (i2 == 8);
}

int main(void) {

    if (!int_to_pointer()) {
        return 1;
    }

    if (!pointer_to_int()) {
        return 2;
    }

    if (!cast_long_round_trip()) {
        return 3;
    }

    if (!cast_ulong_round_trip()) {
        return 4;
    }

    if (!cast_int_round_trip()) {
        return 5;
    }

    return 0;
}