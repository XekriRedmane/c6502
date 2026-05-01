/* Test that we can evaluate bitwise compound assignment expressions at compile time */

long long target(void) {
    long long v = -100;
    long long w = 100;
    long long x = 200;
    long long y = 300;
    long long z = 40000;

    v ^= 10; // -106
    w |= v; // -10
    x &= 30; // 8
    y <<= x; // 76800
    // include chained compound assignment
    z >>= (x |= 2); // z = 39 x = 10

    if (v == -106 && w == -10 && x == 10 && y == 76800 && z == 39) {
        return 0; // success
    }

    return 1; //fail
}

long long main(void) {
    return target();
}