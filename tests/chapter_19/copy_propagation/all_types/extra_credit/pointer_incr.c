/* We can calculate constant offset for ++/-- with pointers into arrays;
 * similar to pointer_arithmetic.c
 */
int target(void) {
    int nested[3][23] = { {0, 1}, {2} };
    int (* ptr)[23] = nested;
    /* c6502 NOTE: `ptr++` on a pointer-to-array currently scales by
     * 1 (sizeof(elem)) instead of sizeof(*ptr); the equivalent
     * `ptr = ptr + 1` form goes through translate_pointer_arithmetic
     * and scales correctly, so use it here while the postfix /
     * prefix incdec lowering catches up. */
    ptr = ptr + 1;
    return *ptr[0];
}

int main(void) {
    return target();
}