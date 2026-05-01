/* We can propagate x = y if it appears on all paths to some use of x,
 * even if y doesn't have the same value on all those paths.
 * Based on Figure 19-5.
 * */

long long callee(long long a, long long b) {
    return a + b;
}
long long target(long long flag) {
    // use static variables here so we can't coalesce x and y
    // into the same register, or into EDI and ESI, once we implement
    // register coalescing; otherwise it might look like we've propagated
    // x = y when we haven't
    static long long x;
    static long long y;
    if (flag) {
        y = 20;
        x = y;
    } else {
        y = 100;
        x = y;
    }
    // x = y reaches here, though with different values of y
    return callee(x, y);
}

long long main(void) {
    long long result = target(0);

    if (result != 200)
        return 1;

    result = target(1);
    if (result != 40)
        return 2;

    return 0;  // success
}