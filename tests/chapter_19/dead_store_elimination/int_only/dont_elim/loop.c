/* Test case where a block is its own predecessor
 * */

long long putchar(long long c);

long long fib(long long count) {
    long long n0 = 0;
    long long n1 = 1;
    long long i = 0;
    do {
        long long n2 = n0 + n1;
        n0 = n1;  // not a dead store b/c n0 is used again in the next loop
                  // iteration, in n2 = n0 + n1
        n1 = n2;
        i = i + 1;
    } while (i < count);
    return n1;
}

long long main(void) {
    return (fib(20) == 10946);
}