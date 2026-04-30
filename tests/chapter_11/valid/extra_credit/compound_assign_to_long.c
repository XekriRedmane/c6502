int main(void) {
    long long l = -1000000000ll; // -10^9
    int i = -10;
    /* We should convert i to a long long, then subtract from l */
    l -= i;
    if (l != -999999990ll) {
        return 1;
    }
    return 0;
}