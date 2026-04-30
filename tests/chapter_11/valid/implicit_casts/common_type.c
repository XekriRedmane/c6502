/* Test that we correctly find the common type in binary expressions */

long long l;
int i;

int addition(void) {
    // l = 32898  (2^15 + 130; doesn't fit in long)
    // i = 10

    /* The common type of i and l is long long, so we should
     * promote i to a long long, then perform addition.
     * If we instead converted l to a long, its value would be
     * -32638 (32898 wraps to a negative signed long), and the
     * result of i + l would not be 32908.
     */
    long long result = i + l;
    return (result == 32908ll);
}

int division(void) {
    // l = 100000ll  (doesn't fit in long; long max is 32767)
    // i = 10

    /* The common type of i and l is long long.
     * Promote i to long long, divide (= 10000), then convert
     * back to long (which preserves the value, since 10000 is
     * within the range of long).
     *
     * If instead we truncated l to a long before performing
     * division, the result would be 34464 / 10 = 3446 (since
     * 100000 mod 65536 = 34464).
     */
    long long_result = l / i;
    return (long_result == 10000l);
}

int comparison(void) {
    // i = -100
    // l = 32898ll  (doesn't fit in long; as signed long = -32638)

    /* Make sure we convert i to long long instead of converting
     * l down to a smaller type. If we convert l to a long its
     * value will be -32638 (smaller than -100); to an int its
     * value will be -126 (also smaller than -100).
     */
    return (i <= l);
}

int conditional(void) {
    // l = 1073741824ll, i.e. 2^30
    // i = 10;

    /* When a conditional expression includes both int and long branches,
     * make sure the int type is promoted to a long long, rather
     * than the long long being converted to an int
     */
    long long result = 1 ? l : i;
    return (result == 1073741824ll);
}

int main(void) {
    // Addition
    l = 32898;
    i = 10;
    if (!addition()) {
        return 1;
    }

    // Division
    l = 100000ll;
    if (!division()) {
        return 2;
    }

    // Comparison
    i = -100;
    l = 32898; // 2^15 + 130
    if (!comparison()) {
        return 3;
    }

    // Conditional
    l = 1073741824ll; // 2^30
    i = 10;
    if (!conditional()) {
        return 4;
    }

    return 0;
}