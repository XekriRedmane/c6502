/* Test that we correctly perform conversions "as if by assignment", including:
 * - actual assignment expressions
 * - initializers for automatic variables
 * - return statements
 * Implicit conversions of function arguments are in a separate test case, convert_function_arguments.c
 */

#ifdef SUPPRESS_WARNINGS
#ifdef __clang__
#pragma clang diagnostic ignored "-Wconstant-conversion"
#else
#pragma GCC diagnostic ignored "-Woverflow"
#endif
#endif

int return_truncated_long(long long l) {
    return l;
}

long long return_extended_int(int i) {
    return i;
}

int truncate_on_assignment(long long l, int expected) {
    int result = l; // implicit conversion truncates l
    return result == expected;
}

int main(void) {

    // return statements

    /* return_truncated_long will truncate 2^8 + 2 to 2 (mod 256);
     * assigning it to result converts this to a long long
     * but preserves its value.
     */
    long long result = return_truncated_long(258ll);
    if (result != 2ll) {
        return 1;
    }

    /* return_extended_int sign-extends its argument, preserving its value */
    result = return_extended_int(-10);
    if (result != -10) {
        return 2;
    }

    // initializer

    /* This is 2^8 + 2; it will be truncated to 2 by assignment */
    int i = 258ll;
    if (i != 2) {
        return 3;
    }

    // assignment expression

    // 256 (= 2^8) will be truncated to 0 when assigned to an int
    if (!truncate_on_assignment(256ll, 0)) {
        return 4;
    }

    return 0;
}