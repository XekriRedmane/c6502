#ifdef SUPPRESS_WARNINGS
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
/* Make sure we implicitly promote constants from unsigned long to unsigned long long
 * as needed, but don't promote long to unsigned long */

/* make this a global variable so we don't
 * optimize away these comparisons in Part III
 */
long long negative_one = 1ll; // can't use negative static initializers; negate this in main
long long zero = 0ll;

int main(void) {

    negative_one = -negative_one;
    /* 2^18 can't be represented as an unsigned long,
     * so it will be promoted to an unsigned long long;
     * when we compare this to -1ll, we'll convert -1ll to
     * an unsigned long long with value ULLONG_MAX
     */
    if (262144u >= negative_one) {
        return 1;
    }

    /* The integer constant with value 2^15 + 10
     * is promoted to signed long, not an unsigned long,
     * so negating it gives us a negative signed value.
     * If it were promoted to an unsigned long, comparing it to 0ll
     * would require us to zero-extend it and we'd get a positive value.
     */
    if (-32778 >= zero) {
        return 2;
    }

    /* constants with ull suffix are always treated as unsigned long long, not unsigned long
     * If these constants were interpreted as unsigned longs, addition would wrap around to 0
     */
    if (!(3ull + 65533ull)) {
        return 3;
    }

    return 0;
}