/* Test that unsigned arithmetic operations wrap around */

unsigned long ui_a;
unsigned long ui_b;

unsigned long long ul_a;
unsigned long long ul_b;

int addition(void) {
    // ui_a = ULONG_MAX (2-byte) - 2
    // ui_b = 3
    // result wraps around to 0
    return ui_a + ui_b == 0ul;
}

int subtraction(void) {
    // ul_a = 10
    // ul_b = 20
    /* ul_a - ul_b wraps around to 2^32 - 10 */
    return (ul_a - ul_b == 4294967286ull);
}

int neg(void) {
    // ul_a = 1ull
    // negating this wraps around to 2^32 - 1, or ULLONG_MAX (4-byte)
    return -ul_a == 4294967295ULL;
}

int main(void) {
    ui_a = 65533ul;
    ui_b = 3ul;
    if (!addition()) {
        return 1;
    }

    ul_a = 10ull;
    ul_b = 20ull;
    if (!subtraction()) {
        return 2;
    }

    ul_a = 1ull;
    if (!neg()) {
        return 3;
    }

    return 0;

}