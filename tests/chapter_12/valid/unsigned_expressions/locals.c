int main(void) {
    /* Initialize and then update a mix of variables,
     * to check that we allocate enough stack space for each of them,
     * and writing to one doesn't clobber another.
     * Identical to chapter 11 long_and_int_locals but with some unsigned integers
     */

    unsigned long long a = 1000000000ull; // outside the range of long
    int b = -1;
    long long c = -1000000000ll; // outside the range of long
    unsigned long d = 10ul;

    /* Make sure every variable has the right value */
    if (a != 1000000000ull) {
        return 1;
    }
    if (b != -1){
        return 2;
    }
    if (c != -1000000000ll) {
        return 3;
    }
    if (d != 10ul) {
        return 4;
    }

    /* update every variable */
    a = -a;
    b = b - 1;
    c = c + 1000000002ll;
    d = d * 26214ul; // result is between LONG_MAX and ULONG_MAX (mod 2^16)

    /* Make sure the updated values are correct */
    if (a != 3294967296ull) {
        return 5;
    }
    if (b != -2) {
        return 6;
    }
    if (c != 2) {
        return 7;
    }
    // 10 * 26214 = 262140; mod 65536 = 65532
    if (d != 65532ul) {
        return 8;
    }

    return 0;
}