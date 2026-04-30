int main(void) {
    double d = 1000.5;
    /* When we perform compound assignment, we convert both operands
     * to their common type, operate on them, and convert the result to the
     * type of the left operand */
    d += 1000;
    if (d != 2000.5) {
        return 1;
    }

    unsigned long long ul = 4000000000ull;
    /* We'll promote ul to a double (= 4e9, exact),
     * subtract 1.5 * 10^9, resulting in 2.5 * 10^9,
     * then convert it back to an unsigned long long
     */
    ul -= 1.5E9;
    if (ul != 2500000000ull) {
        return 2;
    }
    /* We'll promote i to a double, add .99999,
     * then truncate it back to an int
     */
    int i = 10;
    i += 0.99999;
    if (i != 10) {
        return 3;
    }

    return 0;
}