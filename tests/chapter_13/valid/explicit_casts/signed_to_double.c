/* Test conversions from signed integer types to double */
double int_to_double(int i) {
    return (double) i;
}

double long_to_double(long long l) {
    return (double) l;
}
int main(void) {

    if (int_to_double(-100) != -100.0) {
        return 1;
    }

    /* c6502's long long is 4B, so all its values fit exactly in
     * a double's 52-bit mantissa — no rounding needed. */
    if (long_to_double(-2000000000ll) != -2000000000.0) {
        return 2;
    }

    // cast a constant to double to exercise rewrite rule
    double d = (double) 1000000000ll;
    if (d != 1000000000.0) {
        return 3;
    }

    return 0;
}