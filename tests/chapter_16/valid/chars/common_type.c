/* Test that we correctly find the common type of character types and other
 * types (it's always the other type - or, if both are character types, it's
 * int) */

#if defined SUPPRESS_WARNINGS && !defined __clang__
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif

long ternary(int flag, signed char c) {
    // first we'll convert c to an unsigned int (2^16 - c), then to a long
    return flag ? c : 1u;
}

int char_lt_int(signed char c, int i) {
    return c < i;  // common type is int
}

int uchar_gt_long(unsigned char uc, long long l) {
    return uc > l;  // common type is long long
}

/* On operations with two character types, both are promoted to int */
int char_lt_uchar(signed char c, unsigned char u) {
    return c < u;
}

int signed_char_le_char(signed char s, signed char c) {
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
    if (ternary(1, -10) != 65526l) {
        // 1 ? -10 : 1ul
        // ==> (long) (UINT_MAX - 10)
        return 1;
    }

    if (!char_lt_int((char)1, 100)) {
        // 1 < 100; if we converted 100 to a char, its value would be 100,
        // still > 1.
        return 2;
    }

    if (!uchar_gt_long((unsigned char)100, -2)) {
        // we should convert 100 to a long, preserving its type
        return 3;
    }

    signed char c = -1;
    char u = 2;
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