/* Test that we can handle the control-character escape sequences.
 * Upstream's version of this file embeds the literal control bytes
 * (TAB / VT / FF) in the source; pcpp (c6502's preprocessor)
 * truncates source lines at VT / FF, so we exercise the escape
 * forms instead.
 */

int main(void)
{
    int tab = '\t';
    int vertical_tab = '\v';
    int form_feed = '\f';
    if (tab != 9) {
        return 1;
    }
    if (vertical_tab != 11) {
        return 2;
    }

    if (form_feed != 12) {
        return 3;
    }

    return 0;
}
