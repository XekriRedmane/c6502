/* Test conversions from double to the signed integer types */

int double_to_int(double d) {
    return (int) d;
}

long double_to_long(double d) {
    return (long) d;
}

int main(void) {

    // when truncated, d will fit in a long
    // but not an int (c6502's long is 2B, int is 1B)
    long l = double_to_long(20000.3);
    // should be truncated towards 0
    if (l != 20000l) {
        return 1;
    }

    int i = double_to_int(-100.9999);
    // negative value should be truncated towards 0
    if (i != -100) {
        return 2;
    }

    return 0;
}