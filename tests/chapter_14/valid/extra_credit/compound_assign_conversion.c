// Test compound assignment through pointers involving type conversions

int main(void) {
    // lval is pointer
    double d = 5.0;
    double *d_ptr = &d;
    // convert 1000 to double
    *d_ptr *= 1000u;
    if (d != 5000.0) {
        return 1; // fail
    }
    int i = -50;
    int *i_ptr = &i;
    // convert *i_ptr to unsigned long, perform operation, then convert back
    *i_ptr %= 65436UL;  // 65436 = 2^16 - 100; mod with -50 (ulong = 65486)
    /* (ulong)(-50) = 65486; 65486 % 65436 = 50; truncated to int = 50 */
    if (*i_ptr != 50) {
        return 2; // fail
    }

    // rval is pointer
    unsigned long ui = 65535UL; // 2^16 - 1
    ui /= *d_ptr;
    // convert ui to double (= 65535.0), divide by 5000.0 = 13.107, truncate to ulong = 13
    if (ui != 13ul) {
        return 3; // fail
    }

    // both operands are pointers
    i = -10;
    unsigned long long ul = 2147483647ull; // 2^31 - 1
    unsigned long long *ul_ptr = &ul;
    // convert i to common type (ull), perform operation, then
    // convert back to int
    *i_ptr -= *ul_ptr;
    /* i sign-extended to ull = 4294967286 (-10 as 32-bit unsigned).
     * 4294967286 - 2147483647 = 2147483639 = 0x7FFFFFF7.
     * Truncated to int (1B) = low byte 0xF7 = -9 signed. */
    if (i != -9) {
        return 4; // fail
    }

    // check neighbors
    if (ul != 2147483647ull) {
        return 5; // fail
    }

    return 0;
}