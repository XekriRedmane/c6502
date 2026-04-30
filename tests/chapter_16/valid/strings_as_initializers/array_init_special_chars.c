/* Test that we can handle escape sequences in string literals.
 * Upstream's version of this file embeds literal VT / FF / TAB
 * bytes in the string body; pcpp (c6502's preprocessor) truncates
 * source lines at VT / FF, so we exercise the escape forms only.
 */
int main(void) {
    char special[6] = "\a\b\n\v\f\t";

    if (special[0] != '\a') {
        return 1;
    }

    if (special[1] != '\b') {
        return 2;
    }

    if (special[2] != '\n') {
        return 3;
    }
    if (special[3] != '\v') {
        return 4;
    }
    if (special[4] != '\f') {
        return 5;
    }

    if (special[5] != '\t') {
        return 6;
    }

    return 0;
}
