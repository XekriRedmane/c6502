int main(void) {
    int i = -20;
    int b = 127;
    int c = -100;

    /* This statement is evaluated as follows:
     * 1. sign-extend i to a long long with value -20
     * 2. add this long long to 100, resulting in the long long 80,
     * 3. convert this to an int with value 80 (this value
     * can be represented as an int)
     */
    i += 100ll;

    // make sure we got the right answer and didn't clobber b
    if (i != 80) {
        return 1;
    }
    if (b != 127) {
        return 2;
    }

    // b /= -2^31 + 1
    // if we try to perform int (rather than long long)
    // division, we'll interpret this value as 1 and
    // b's value won't change.
    b /= -2147483647ll;
    if (b) { // b's value should be 0
        return 3;
    }

    // make sure we didn't clobber i or c
    if (i != 80) {
        return 4;
    }
    if (c != -100) {
        return 5;
    }

    // this result will be outside the range of int; we'll
    // convert it to int in the usual implementation-defined way
    c *= 1000ll;
    if (c != 96) {
        return 6;
    }

    return 0;
}