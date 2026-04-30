/* Test out different, equivalent ways to declare the same identifier  */

#ifdef SUPPRESS_WARNINGS
#ifndef __clang__
#pragma GCC diagnostic ignored "-Wold-style-declaration"
#endif
#endif

/* These declarations all look slightly different,
 * but they all declare 'a' as a static long long, so they don't conflict.
 */
static int long long a;
int static long long a;
long long static a;

/* These declarations all look slightly different,
 * but they all declare 'my_function' as a function
 * with three long long parameters and an int return value,
 * so they don't conflict.
 */
int my_function(long long a, long long int b, int long long c);
int my_function(long long int x, int long long y, long long z) {
    return x + y + z;
}

int main(void) {
    /* Several different ways to declare local long long variables */
    long long x = 1ll;
    long long int y = 2ll;
    int long long z = 3ll;

    /* This links to the file-scope declarations of 'a' above */
    extern long long a;
    a = 4;

    /* make sure we can use long long type specifier in for loop initializer
     * i is 2^30 so this loop should have 31 iterations
    */
   int sum = 0;
    for (long long i = 1073741824ll; i > 0; i = i / 2) {
        sum = sum + 1;
    }

    /* Make sure everything has the expected value */
    if (x != 1) {
        return 1;
    }

    if (y != 2) {
        return 2;
    }

    if (a != 4) {
        return 3;
    }

    if (my_function(x,  y, z) != 6) {
        return 4;
    }

    if (sum != 31) {
        return 5;
    }
    return 0;
}