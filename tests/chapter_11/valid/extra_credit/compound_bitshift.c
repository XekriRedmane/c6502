// Test compound bit shift operators with mix of types

int main(void) {

    // shift long long using long long shift count
    long long x = 100ll;
    x <<= 22ll;
    if (x != 419430400ll) {
        return 1; // fail
    }

    // try right shift; validate result of expression
    if ((x >>= 4ll) != 26214400ll) {
        return 2; // fail
    }

    // also validate side effect of updating variable
    if (x != 26214400ll) {
        return 3;
    }

    // now try shifting a long long with an int shift count
    long long l = 12345ll;
    if ((l <<= 17) != 1618083840ll) {
        return 4;
    }

    l = -l;
    if ((l >>= 10) != -1580160ll) {
        return 5;
    }

    return 0; // success
}