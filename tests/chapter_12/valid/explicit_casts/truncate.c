/* Test truncating wider to narrow types */
int ulong_to_int(unsigned long long ul, long expected) {
    long result = (long) ul;
    return (result == expected);
}

int ulong_to_uint(unsigned long long ul, unsigned long expected) {
    return ((unsigned long) ul == expected);
}

int long_to_uint(long long l, unsigned long expected) {
    return (unsigned long) l == expected;
}

int main(void) {
    /* truncate long long */

    /* 100 is in the range of unsigned long,
     * so truncating it to an unsigned long
     * will preserve its value
     */
    if (!long_to_uint(100ll, 100ul)) {
        return 1;
    }

    /* -2147482414 (i.e. -2^31 + 1234) is outside the range of unsigned long,
     * so add 2^16 to bring it within range */
    if (!long_to_uint(-2147482414ll, 1234ul)) {
        return 2;
    }

    /* truncate unsigned long long */

    /* 100 can be cast to a long or unsigned long without changing its value */
    if (!ulong_to_int(100ull, 100l)) {
        return 3;
    }

    if (!ulong_to_uint(100ull, 100ul)) {
        return 4;
    }

    /* 65440 (i.e. 2^16 - 96) can be cast to an unsigned long without changing its value,
     * but must be reduced modulo 2^16 to cast to a signed long
     */
    if (!ulong_to_uint(65440ull, 65440ul)) {
        return 5;
    }

    if (!ulong_to_int(65440ull, -96l)) {
        return 6;
    }

    /* 98304 (i.e. 2^16 + 2^15) must be reduced modulo 2^16
     * to represent as a signed or unsigned long
     */

    if (!ulong_to_uint(98304ull, 32768ul)) { // reduce to 2^15
        return 7;
    }

    if (!ulong_to_int(98304ull, -32768l)){ // reduce to -2^15
        return 8;
    }

    /* truncate unsigned long long constant that can't
     * be expressed in 16 bits, to test rewrite rule
     */
    unsigned long ui = (unsigned long)65541ull; // 2^16 + 5
    if (ui != 5)
        return 9;

    return 0;
}