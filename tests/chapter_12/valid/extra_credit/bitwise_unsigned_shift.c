/* Tests for bit-shift operations on unsigned integers */

int main(void) {
    unsigned long ui = -1ul;  // 2^16 - 1, or 65535

    /* Shifting left by 2 wraps around; the result is
     * equivalent to (ui * 2^2) % (UINT_MAX + 1).
     * Note that we don't cast ui to a long long first.
     */
    if ((ui << 2ll) != 65532ul) {
        return 1;
    }

    /* Shifting right by 2 is like dividing by 4;
     * note that we need to use logical shift rather than
     * arithmetic shift  */
    if ((ui >> 2) != 16383) {
        return 2;
    }

    /* Test unsigned shift with variable shift counts, to make sure we handle
     * them correctly in codegen/code emission */
    static int shiftcount = 5;
    if ((1000ul >> shiftcount) != 31) {
        return 3;
    }

    if ((1000ul << shiftcount) != 32000) {
        return 4;
    }

    return 0;  // success
}