int main(void) {
    unsigned long x = -1ul; // 2^16 - 1
    /* 1. convert x to a signed long long, which preserves its value
     * 2. divide by -10, resulting in -6553 (truncated toward 0)
     * 3. convert -6553 to an unsigned long by adding 2^16
     */
    x /= -10ll;

    return (x == 58983ul);
}