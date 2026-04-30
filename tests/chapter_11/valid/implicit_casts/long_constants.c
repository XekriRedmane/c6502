int main(void) {

    /* A constant with an `ll` suffix always has long long type */

    // if we parse these as ints, this addition will overflow and be negative
    if (127ll + 127ll < 0ll) {
        return 1;
    }
    /* if a constant is too large to store as a long,
     * it's automatically converted to a long long, even if it
     * doesn't have an `ll` suffix.
     * if we parsed 100000 as a long, it would be negative
     * (100000 mod 65536 = 34464; signed long = -31072).
     */
    if (100000 < 100ll) {
        return 2;
    }
    return 0;
}