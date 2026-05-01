/* Test constant-folding with INT_MIN */
long long target(void) {
    return -2147483647 - 1;
}

long long main(void) {
    if (~target() != 2147483647) {
        return 1; // fail
    }
    return 0;
}