#ifdef SUPPRESS_WARNINGS
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
int test_sum(long long a, long long b, int c, int d, int e, int f, int g, int h, long long i) {
    /* Make sure the arguments passed in main weren't converted to ints */
    if (a + b < 100ll) {
        return 1;
    }
    /* Check an argument that was passed on the stack too */
    if (i < 100ll)
        return 2;
    return 0;
}

int main(void) {
    // passing a constant larger than 16-bit as our last argument
    // exercises the rewrite rule for push $large_constant
    return test_sum(1000000ll, 1000000ll, 0, 0, 0, 0, 0, 0, 1000000ll);
}