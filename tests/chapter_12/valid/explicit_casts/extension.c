/* Test conversions from narrower to wider types */

int int_to_ulong(int i, unsigned long long expected) {
    unsigned long long result = (unsigned long long) i;
    return result == expected;
}

int uint_to_long(unsigned long ui, long long expected) {
    long long result = (long long) ui;
    return result == expected;
}

int uint_to_ulong(unsigned long ui, unsigned long long expected){
    return (unsigned long long) ui == expected;
}

int main(void) {
    /* Converting a positive int to an unsigned long long preserves its value */
    if (!int_to_ulong(10, 10ull)) {
        return 1;
    }

    /* When you convert a negative int to an unsigned long long,
     * add 2^32 until it's positive
     */
    if (!int_to_ulong(-10, 4294967286ull)) {
        return 2;
    }

    /* Extending an unsigned long to a signed long long preserves its value */
    if (!uint_to_long(65440ul, 65440ll)) {
        return 3;
    }

    /* Extending an unsigned long to an unsigned long long preserves its value */
    if (!uint_to_ulong(65440ul, 65440ull)) {
        return 4;
    }
    /* Zero-extend constant 65440
     * from an unsigned long to an unsigned long long
     * to test the assembly rewrite rule for zero-extension
     */
    if ((unsigned long long) 65440ul != 65440ull) {
        return 5;
    }
    return 0;
}
