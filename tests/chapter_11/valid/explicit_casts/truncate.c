int truncate(long long l, int expected) {
    int result = (int) l;
    return (result == expected);
}

int main(void)
{
    /* If a long long is already in the range of 'int',
     * truncation doesn't change its value.
     */
    if (!truncate(10ll, 10)) {
        return 1;
    }

    /* Truncating a negative int also preserves its value */
    if (!truncate(-10ll, -10)) {
        return 2;
    }
    /* If a long long is outside the range of int,
     * subtract 2^8 until it's in range
     */
    if (!truncate(261ll, // 2^8 + 5
                  5)) {
        return 3;
    }

    /* If a negative long long is outside the range of int,
     * add 2^8 until it's in range
     */
    if (!truncate(-251ll, // (-2^8) + 5
                  5)) {
        return 4;
    }

    /* truncate long long constant that can't
     * be expressed in 8 bits, to test rewrite rule
     */
    int i = (int)261ll; // 2^8 + 5
    if (i != 5)
        return 5;

    return 0;
}