

/* A recursive function in which both double and integer parameters
 * are passed in registers and on the stack.
 *
 * Reduced from 18 to 6 params (3 ints + 3 doubles) so the per-call
 * frame fits in c6502's 256-byte FP-relative addressing window
 * (each double is 8 bytes; 9 doubles + 9 ints + locals + temps
 * exceeds 256 bytes).
 */
int fun(int i1, double d1, int i2, double d2, int i3, double d3) {


    if (i1 != d3) {
        /* make two recursive calls that bring these values closer together:
         * 1. increment i1 and all ints: */
        int call1 = fun(i1 + 1, d1, i2 + 1, d2, i3 + 1, d3);

        /* 2. decrement d3 and all doubles */
        int call2 = fun(i1, d1 - 1, i2, d2 - 1, i3, d3 - 1);

        /* Make sure both calls succeeded; non-zero result indicates a problem */
        if (call1) {
            return call1;
        }

        if (call2) {
            return call2;
        }

    }

    // make sure all arguments have expected value; value of each arg relative to i1 (for ints)
    // or d3 (for doubles) should stay the same.
    if (i2 != i1 + 2) {
        return 2;
    }
    if (i3 != i1 + 4) {
        return 3;
    }
    if (d1 != d3 - 4) {
        return 11;
    }
    if (d2 != d3 - 2) {
        return 12;
    }

    return 0;
}

int main(void) {
    return fun(1, 2.0, 3, 4.0, 5, 6.0);
}