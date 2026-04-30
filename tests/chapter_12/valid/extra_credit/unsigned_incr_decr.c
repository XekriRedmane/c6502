// test ++/-- with unsigned values (including wraparound)
int main(void) {
    unsigned long i = 0;

    // Postfix --, including wraparound
    if (i-- != 0) {
        return 1;
    }
    if (i != 65535UL) { // wraparound from 0 to ULONG_MAX (2-byte)
        return 2;
    }

    // Prefix --
    if (--i != 65534UL) {
        return 3;
    }
    if (i != 65534UL) {
        return 4;
    }

    unsigned long long l = 4294967294ULL;
    // Postfix ++
    if (l++ != 4294967294ULL) {
        return 5;
    }
    if (l != 4294967295ULL) {
        return 6;
    }
    if (++l != 0) { // wraparound from ULONG_LONG_MAX to 0
        return 7;
    }
    if (l != 0) {
        return 8;
    }
    return 0; // success
}