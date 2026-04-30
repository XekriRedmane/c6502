#ifdef SUPPRESS_WARNINGS
#ifdef __clang__
#pragma GCC diagnostic ignored "-Wimplicit-const-int-float-conversion"
#pragma GCC diagnostic ignored "-Wliteral-conversion"
#endif
#endif
/* Test that we correctly perform conversion as if by assignment */

int check_args(long long l, double d) {
    return l == 2 && d == -6.0;
}

double return_double(void) {
    /* Implicitly convert this integer to a double — the value
     * fits exactly in c6502's 4-byte unsigned long long.
     */
    return 4000000000ull;
}

int check_assignment(double arg) {
    // arg = 4.9
    int i = 0;
    /* truncate arg to 4 */
    i = arg;
    return i == 4;
}
int main(void) {

    /* function arguments: 2.4 should be truncated to 2, -6 should be converted to -6.0 */
    if (!check_args(2.4, -6)) {
        return 1;
    }

    /* return values */
    if (return_double() != 4000000000.0) {
        return 2;
    }

    /* assignment statement */
    if (!check_assignment(4.9)) {
        return 3;
    }

    /* initializer */
    double d = 4000000000ull; // implicitly convert constant to double

    if (d != 4000000000.) {
        return 4;
    }

    return 0;
}