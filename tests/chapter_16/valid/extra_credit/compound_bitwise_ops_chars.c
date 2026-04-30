#ifdef SUPPRESS_WARNINGS
#ifdef __clang__
#pragma clang diagnostic ignored "-Wshift-count-overflow"
#endif
#endif
// Test bitwise compound assignment operators with character types

/* Split into smaller helpers (with the arrays local to main so we can use
 * negative initializers — c6502's constant-expression evaluator doesn't
 * accept unary `-` at file-scope) so each function's frame fits in the
 * 256-byte FP-relative addressing window. */

static long x = 8;

int do_signed_ops(signed char *arr, unsigned char *u_arr) {
    arr[0] ^= 12345;
    arr[1] |= u_arr[3];
    arr[2] &= u_arr[1] - (unsigned char) 185;
    arr[3] <<= 7u; // wraps around to -128; well-defined b/c of integer promotions
    arr[4] >>= 7;
    return 0;
}

int do_unsigned_ops(unsigned char *u_arr) {
    u_arr[3] <<= 12;
    u_arr[2] >>= (x - 1);
    u_arr[1] |= -399; // doesn't overflow b/c of integer promotion
    x = -1224ll; // a number that doesn't fit in 1-byte int
    u_arr[0] ^= x;
    return 0;
}

int main(void) {
    signed char arr[5] = {-128, -120, -2, 1, 120};
    unsigned char u_arr[4] = {0, 170, 250, 255};
    do_signed_ops(arr, u_arr);
    do_unsigned_ops(u_arr);

    // validate them
    if (arr[0] != -71) {
        return 1; // fail
    }

    if (arr[1] != -1) {
        return 2; // fail
    }

    if (arr[2] != -16) {
        return 3; // fail
    }

    if (arr[3] != -128) {
        return 4; // fail
    }

    if (arr[4] != 0) {
        return 5;
    }

    if (u_arr[0] != 168) {
        return 6;
    }

    if (u_arr[1] != 251) {
        return 7;
    }

    if (u_arr[2] != 0) {
        return 8;
    }

    if (u_arr[3] != 0) {
        return 9;
    }

    return 0;
}