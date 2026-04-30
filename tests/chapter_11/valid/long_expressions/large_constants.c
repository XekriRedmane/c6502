/* Make sure we can handle adding, subtracting,
 * and multiplying by constants that are outside
 * the range of int, but inside the range of unsigned int;
 * this tests several assembly rewrite rules.
 */


/* Make x a global variable so this test doesn't rely on
 * correct argument passing for longs but won't get optimized away in part III
 */
long long x = 5ll;

int add_large(void) {
    // x = 5ll
    x = x + 2147483640ll; // this constant is large (2^31 - 8)
    return (x == 2147483645ll);
}

int subtract_large(void) {
    // x = 2147483645ll
    x = x - 2147483640ll;
    return (x == 5ll);
}

int multiply_by_large(void) {
    // x = 5
    // 200_000_000 is well above 16-bit (65535), so this exercises
    // the rewrite rule for multi-byte multiply constants.
    x = x * 200000000ll;
    return (x == 1000000000ll);
}

int main(void) {

    if (!add_large()) {
        return 1;
    }

    if (!subtract_large()) {
        return 2;
    }

    if (!multiply_by_large()) {
        return 3;
    }

    return 0;
}