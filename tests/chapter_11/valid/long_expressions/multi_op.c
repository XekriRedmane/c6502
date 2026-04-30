int target(long long a) {
    // a = 100000ll

    /* This expression produces an intermediate result that cannot
     * fit in a long, in order to test that we track the sizes
     * of intermediate results and allocate enough stack
     * space for them.
     */
    long long b = a * 5ll - 10ll;
    if (b == 499990ll) {
        return 1;
    }
    return 0;
}

int main(void) {
    return target(100000ll);
}