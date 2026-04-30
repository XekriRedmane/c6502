/* Test basic arithmetic operations on long long integers
 * when one or both operands and the result are outside the range of int */

long long a;
long long b;

int addition(void) {
    // a == 2147483640ll, i.e. 2^31 - 8
    // b = 5
    return (a + b == 2147483645ll);
}

int subtraction(void) {
    // a = -2000000000ll;
    // b = 90ll;
    return (a - b == -2000000090ll);
}

int multiplication(void) {
    // a = 500000000ll;
    return (a * 4ll == 2000000000ll);
}

int division(void) {
    /* The first operand can't fit in a long; the divide goes
    * through the 32-bit divmod32 helper.
    */
    // a = 100000000ll;
    b = a / 128ll;
    return (b == 781250ll);
}

int remaind(void) {
    // a = 1000000005ll
    b = -a % 1000000000ll;
    return (b == -5ll);
}

int complement(void) {
    // a = 2147483646ll, i.e. LONG_LONG_MAX - 1
    return (~a == -2147483647ll);
}

int main(void) {

    /* Addition */
    a = 2147483640ll; // 2^31 - 8
    b = 5ll;
    if (!addition()) {
        return 1;
    }

    /* Subtraction */
    a = -2000000000ll;
    b = 90ll;
    if (!subtraction()) {
        return 2;
    }

    /* Multiplication */
    a = 500000000ll;
    if (!multiplication()) {
        return 3;
    }

    /* Division */
    a = 100000000ll;
    if (!division()) {
        return 4;
    }

    /* Remainder */
    a = 1000000005ll;
    if (!remaind()) {
        return 5;
    }

    /* Complement */
    a = 2147483646ll; // LONG_LONG_MAX - 1
    if (!complement()) {
        return 6;
    }

    return 0;
}