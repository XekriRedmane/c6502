/* Test that we correctly find the common type of character types and other
 * types (it's always the other type - or, if both are character types, it's
 * int) */

#if defined SUPPRESS_WARNINGS && !defined __clang__
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif

long long ternary(int flag, signed char c) {
    // first we'll convert c to an unsigned long (2^32 - c), then to
    // a long long. (Plain `char` is unsigned in c6502 — use `signed
    // char` so a negative `c` stays negative through the promotion.
    // Use `1ul` because c6502's `unsigned int` is 2 bytes; the
    // test's 4-byte unsigned wrap target requires `unsigned long`.
    // Return `long long` (8B) so the 4-byte unsigned value
    // 4294967286 doesn't overflow the signed return type.)
    return flag ? c : 1ul;
}

int char_lt_int(char c, int i) {
    return c < i;  // common type is int
}

int uchar_gt_long(unsigned char uc, long l) {
    return uc > l;  // common type is long
}

/* On operations with two character types, both are promoted to int */
int char_lt_uchar(signed char c, unsigned char u) {
    // c6502 plain `char` is unsigned, so use `signed char` to
    // express the test's intent (a negative input survives as
    // negative through integer promotion to int).
    return c < u;
}

int signed_char_le_char(signed char s, char c) {
    return s <= c;
}

char ten = 10;
int multiply(void) {
    /* This should promote 10 to a double,
     * calculate 10.75 * 10.0, which is 107.5,
     * and then truncate back to an int, 107.
     * It should not truncate 10.75 to 10 before
     * performing the calculation.
     */
    char i = 10.75 * ten;

    return i == 107;
}

int main(void) {
    if (ternary(1, -10) != 4294967286ll) {
        // 1 ? -10 : 1ul
        // ==> (long long) (ULONG_MAX_4B - 10) where ULONG_MAX_4B
        // is c6502's 4-byte unsigned long max (0xFFFFFFFF).
        return 1;
    }

    if (!char_lt_int((char)1, 256)) {
        // 1 < 256 ; if we converted 256 to a char, its value would be 0,
        // so it would evaluate to less than 1
        return 2;
    }

    if (!uchar_gt_long((unsigned char)100, -2)) {
        // we should convert 100 to a long, preserving its type
        return 3;
    }

    signed char c = -1;
    unsigned char u = 2;
    if (!char_lt_uchar(c, u)) {
        // we convert both c and u to int; we DON'T convert c to an unsigned
        // char!
        return 4;
    }

    signed char s = -1;
    if (!signed_char_le_char(s, c)) {
        return 5;
    }

    if (!multiply()) {
        return 6;
    }

    return 0;
}