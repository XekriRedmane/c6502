/* The order in which multiple casts are applied matters */

// start with a global variable so we can't optimize away casts in Part III
unsigned long ui = 65440ul; // 2^16 - 96

int main(void) {


    /* In this case we
     * 1. convert ui to a signed long by computing ui - 2^16, producing -96
     * 2. sign-extend the result, which preserves the value of -96
     * Note that if we cast ui directly to a signed long long, its value wouldn't change
     */
    if ((long long) (signed) ui != -96ll)
        return 1;

    /* In this case we
     * 1. convert ui to a signed long by computing ui - 2^16, producing -96
     * 2. convert this signed long to an unsigned long long by computing -96 + 2^32
     * Note that if we converted ui directly to an unsigned long long, its value
     * wouldn't change
     */
    if ((unsigned long long) (signed) ui != 4294967200ull)
        return 2;

    return 0;
}