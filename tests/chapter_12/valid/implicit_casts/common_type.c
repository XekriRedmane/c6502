#ifdef SUPPRESS_WARNINGS
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
/* Test that we correctly find the common type of different integers */

int int_gt_uint(long i, unsigned long u) {
    // common type is unsigned long
    return i > u;
}

int int_gt_ulong(long i, unsigned long long ul) {
    // common type is unsigned long long
    return i > ul;
}

int uint_gt_long(unsigned long u, long long l) {
    // common type is long long
    return u > l;
}

int uint_lt_ulong(unsigned long u, unsigned long long ul) {
    // common type is unsigned long long
    return u < ul;
}

int long_gt_ulong(long long l, unsigned long long ul) {
    // common type is unsigned long long
    return l > ul;
}

int ternary_int_uint(int flag, long i, unsigned long ui) {
    /* flag = 1
     * i = -1
     * ui = 10ul
     * The common type of i and ui is unsigned long
     * (we don't consider the type of cond when we
     * determine the common type).
     * We therefore convert i to an unsigned long, 2^16 - 1,
     * which we then convert to a signed long long.
     * Therefore, result will be positive. If we didn't
     * convert i to an unsigned long, result would be negative.
     */
    long long result = flag ? i : ui;
    return (result == 65535ll);

}

int main(void) {

    // converting -100 from long to unsigned long gives us 2^16 - 100,
    // so -100 > 100ul
    if (!int_gt_uint(-100, 100ul)) {
        return 1;
    }

    // converting -1 to unsigned long long gives us 2^32-1
    // 4294967286 is 2^32 - 10
    if (!(int_gt_ulong(-1, 4294967286ull))) {
        return 2;
    }

    // converting 100ul to a signed long long won't change its value
    // Note that if we converted -100 to an unsigned long it would be
    // greater than 100
    if (!uint_gt_long(100ul, -100ll)) {
        return 3;
    }

    // converting an unsigned long to an unsigned long long won't change its value
    // if we converted 131072ull (2^17) to an unsigned long its value would be 0
    // Note: 16384ul is 2^14
    if (!uint_lt_ulong(16384ul, 131072ull)) {
        return 4;
    }

    // converting -1 from long long to unsigned long long gives us 2^32-1, so -1ll > 1000ull
    if (!long_gt_ulong(-1ll, 1000ull)) {
        return 5;
    }

    // make sure we convert the two branches of a ternary expression to the common type
    if (!ternary_int_uint(1, -1, 1ul)) {
        return 6;
    }

    return 0;

}