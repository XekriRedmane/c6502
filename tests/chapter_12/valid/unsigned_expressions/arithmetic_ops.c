/* Test basic arithmetic operations on unsigned integers
 * None of these operations wrap around; that's tested separately in arithmetic_wraparound
 */

unsigned long ui_a;
unsigned long ui_b;

unsigned long long ul_a;
unsigned long long ul_b;

int addition(void) {
    // ui_a = 10ul;

    /* Test out rewrite rule for addition;
     * even when adding numbers we interpret as unsigned,
     * the immediate operand can only be a small literal,
     * second operand here is 32773, larger than 16-bit signed max
     */
    return (ui_a + 32773ul == 32783ul);
}

int subtraction(void) {
    // ul_a = 2^32 - 2^15
    // ul_b = 1000
    return (ul_a - ul_b == 4294933528ull);
}

int multiplication(void) {
    // ui_a = 2^14
    // ui_b = 3
    // product fits in unsigned long but not long
    return (ui_a * ui_b == 49152ul);
}

int division(void) {
    // ui_a = 100
    // ui_b = 65534

    /* ui_a/ui_b is 0.
     * If you interpreted these as signed values, ui_b would be -2
     * and ui_a / ui_b would be -50
     */
    return (ui_a / ui_b == 0);
}

int division_large_dividend(void) {
    // ui_a = 65534
    // ui_b = 32767

    /* upper bit of ui_a is set, so this tests
     * that we zero-extended dividend
     * instead of sign-extending it
     */

    return (ui_a / ui_b == 2);
}

int division_by_literal(void) {
    // ul_a = 16777215, i.e. 2^24 - 1
    // exercise assembly rewrite rule for div by constant
    return (ul_a / 5ull == 3355443ull);
}

int remaind(void) {
    // ul_a = 100
    // ul_b = 4294967205 (= 2^32 - 91)

    /* ul_b % ul_a is 5.
     * If you interpreted these as signed values, ul_b would be -91
     * and ul_b % ul_a would also be -91.
     */

    return (ul_b % ul_a == 5ull);
}
int complement(void) {
    // ui_a = ULONG_MAX (2-byte)
    return (~ui_a == 0);
}

int main(void) {

    ui_a = 10ul;
    if (!addition()) {
        return 1;
    }

    ul_a = 4294934528ull;
    ul_b = 1000ull;
    if (!subtraction()) {
        return 2;
    }

    ui_a = 16384ul;
    ui_b = 3ul;
    if (!multiplication()) {
        return 3;
    }

    ui_a = 100ul;
    ui_b = 65534ul;

    if (!division()) {
        return 4;
    }

    ui_a = 65534ul;
    ui_b = 32767ul;
    if (!division_large_dividend()) {
        return 5;
    }

    ul_a = 16777215ull;
    if (!division_by_literal()) {
        return 6;
    }

    ul_a = 100ull;
    ul_b = 4294967205ull;
    if (!remaind()) {
        return 7;
    }

    ui_a = 65535UL;
    if (!complement()) {
        return 8;
    }

    return 0;
}