/* Test conversions from unsigned integer types to doubles */
double uint_to_double(unsigned long ui) {
    return (double) ui;
}

double ulong_to_double(unsigned long long ul) {
    return (double) ul;
}

int main(void) {

    // ulong (2B) that's smaller than LONG_MAX
    if (uint_to_double(1000ul) != 1000.0) {
        return 1;
    }

    // ulong (2B) that's larger than LONG_MAX
    if (uint_to_double(60000ul) != 60000.0) {
        return 2;
    }

    // ulong long (4B) that's smaller than LONG_LONG_MAX
    if (ulong_to_double(1000000000ull) != 1000000000.0) {
        return 3;
    }

    // ulong long that's larger than LONG_LONG_MAX
    if (ulong_to_double(3000000000ull) != 3000000000.0) {
        return 4;
    }

    /* All c6502 4-byte integer values fit exactly in a double's
     * 52-bit mantissa, so we don't need round-to-odd tests
     * (the upstream tests at this point exercise rounding for
     * 64-bit unsigned values that don't have exact double
     * representations). */

    return 0;
}