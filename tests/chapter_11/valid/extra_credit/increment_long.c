// make sure we support prefix and postfix ++/-- on long long variables
int main(void) {
    long long x = -2147483647ll;

    // postfix ++
    if (x++ != -2147483647ll) {
        return 1;
    }
    if (x != -2147483646ll) {
        return 2;
    }

    // prefix --
    if (--x != -2147483647ll) {
        return 3;
    }
    if (x != -2147483647ll) {
        return 4;
    }

    return 0; // success
}