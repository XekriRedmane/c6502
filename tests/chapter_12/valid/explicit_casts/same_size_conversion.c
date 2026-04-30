/* Test conversions between signed and unsigned types of the same size */

int uint_to_int(unsigned long ui, long expected) {
    return (long) ui == expected;
}

int int_to_uint(long i, unsigned long expected) {
    return (unsigned long) i == expected;
}

int ulong_to_long(unsigned long long ul, signed long long expected) {
    return (signed long long) ul == expected;
}

int long_to_ulong(long long l, unsigned long long expected) {
    return (unsigned long long) l == expected;
}

int main(void) {

    /* Converting a positive signed long to an unsigned long preserves its value */
    if (!int_to_uint(10l, 10ul)) {
        return 1;
    }

    /* If an unsigned long is within the range of signed long,
     * converting it to a signed long preserves its value
     */
    if (!uint_to_int(10ul, 10l)) {
        return 2;
    }

    /* Converting a negative signed long long -x to an unsigned long long
     * results in 2^32 - x
     */
    if (!long_to_ulong(-1000ll, 4294966296ull)) {
        return 3;
    }

    /* If an unsigned long long is too large for a long long to represent,
     * reduce it modulo 2^32 until it's in range.
     */
    if (!ulong_to_long(4294966296ull, -1000ll)) {
        return 4;
    }

    return 0;
}