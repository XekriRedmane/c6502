/* Converting a value to a different type, then back to the original type,
 * does not always recover its original value
 */

// start with a global variable so we can't optimize away casts in Part III
unsigned long long a = 100000ull; // 2^16 + 34464

int main(void) {

    /* because a is too large to fit in an unsigned long,
     * casting it to unsigned long and back is equivalent to taking mod 2^16,
     * resulting in 34464
     */
    unsigned long long b = (unsigned long long) (unsigned long) a;

    if (b != 34464ull)
        return 1;

    /* Casting a to signed long takes the low 16 bits (34464) and reinterprets
     * as signed (= -31072). Casting it back to unsigned long long gives
     * 2^32 - 31072 = 4294936224.
     */
    b = (unsigned long long) (signed long) a;
    if (b != 4294936224ull)
        return 2;

    return 0;
}