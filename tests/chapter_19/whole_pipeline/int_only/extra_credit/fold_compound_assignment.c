/* Test that we can evaluate compound assignment expressions at compile time */

long long target(void) {
    long long v = -100;
    long long w = 100;
    long long x = 200;
    long long y = 300;
    long long z = 400;

    v += 10;
    w -= 20;
    x *= 30;
    y /= 100;
    // include chained compound assignment
    z %= y += 6;

    if (v == -90 && w == 80 && x == 6000 && y == 9 && z == 4) {
        return 0; // success
    }

    return 1; //fail
}

long long main(void) {
    return target();
}