/* When a long long is used in the controlling condition of a switch
 * statement, the constant in each case statement should be converted to
 * a long long.
 */

int switch_on_long(long long l) {
    switch (l) {
        case 0: return 0;
        case 100: return 1;
        case 1073741824ll: // 2^30
            return 2;
        default:
            return -1;
    }
}

int main(void) {
    if (switch_on_long(1073741824) != 2)
        return 1;
    if (switch_on_long(100) != 1)
        return 2;
    return 0; // success
}