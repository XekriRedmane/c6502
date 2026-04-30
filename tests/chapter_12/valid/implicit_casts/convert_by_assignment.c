/* Test that we correctly perform conversions "as if by assignment", including:
 * - function arguments
 * - return statements
 * - actual assignment expressions
 * - initializers for automatic variables
 */

#ifdef SUPPRESS_WARNINGS
#ifdef __clang__
#pragma clang diagnostic ignored "-Wconstant-conversion"
#else
#pragma GCC diagnostic ignored "-Woverflow"
#endif
#endif

int check_int(long converted, long expected) {
    return (converted == expected);
}

int check_long(long long converted, long long expected) {
    return (converted == expected);
}

int check_ulong(unsigned long long converted, unsigned long long expected) {
    return (converted == expected);
}

long long return_extended_uint(unsigned long u) {
    return u;
}

unsigned long long return_extended_int(int i) {
    return i;
}

long return_truncated_ulong(unsigned long long ul) {
    return ul;
}

int extend_on_assignment(unsigned long ui, long long expected) {
    long long result = ui; // implicit conversion causes zero-extension
    return result == expected;
}

int main(void) {
    // function arguments

    /* truncate 2^31 + 5 to 5 */
    if (!check_int(2147483653ull, 5)) {
        return 1;
    }

    /* zero-extend 2^15+10, preserve its value */
    if (!check_long(32778ul, 32778ll)) {
        return 2;
    }

    /* sign-extend -1 to ULLONG_MAX */
    if (!check_ulong(-1, 4294967295ULL)) {
        return 3;
    }

    // return values

    /* zero-extend 2^15+10, preserve its value */
    if (return_extended_uint(32778ul) != 32778ll) {
        return 4;
    }

    /* sign-extend -1 to ULLONG_MAX */
    if (return_extended_int(-1) != 4294967295ULL) {
        return 5;
    }

    /* truncate 2^25 + 2^15 + 100 to long, -2^15 + 100
     * then sign-extend, preserving its value
     */
    long long l = return_truncated_ulong(33587812ul);
    if (l != -32668l) {
        return 6;
    }

    // assignment expressions
    if (!extend_on_assignment(32778ul, 32778ll)){
        return 7;
    }

    // local initializers
    long i = 65436ul; // unsigned long 2^16 - 100, will be converted to -100
    if (i != -100) {
        return 8;
    }


    return 0;
}