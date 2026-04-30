int main(void) {

    unsigned long long ul = 0xfbfaf9f8ull; // 0xfbfa_f9f8
    ul &= -1000; // make sure we sign-extend -1000 to unsigned long long
    if (ul != 0xfbfaf818ull /* 0xfbfa_f818 */) {
        return 1; // fail
    }

    ul |= 65280ul; // 0x0000_ff00 - make sure we zero-extend this to unsigned long long

    if (ul != 0xfbfaff18ull /* 0xfbfa_ff18 */) {
        return 2; // fail
    }

    // make sure that we convert result _back_ to type of lvalue,
    // and that we don't clobber nearby values (e.g. by trying to assign
    // 4-byte result to 2-byte ui variable)
    int i = 100;
    unsigned long ui = 0xf0f0ul; // 0xf0f0
    long long l = -3856ll; // sign-extend to 4B = 0xffff_f0f0
    // 1. zero-extend ui to 4 bytes: 0x0000_f0f0
    // 2. l is already 4 bytes: 0xffff_f0f0
    // 3. XOR: 0xffff_0000
    // 4. truncate back to 2 bytes for ui: 0x0000
    // then check value of expression (i.e. value of ui)
    if (ui ^= l) {
        return 3; // fail
    }

    // check side effect (i.e. updating ui)
    if (ui) {
        return 4; // fail
    }
    // check neighbors
    if (i != 100) {
        return 5;
    }
    if (l != -3856ll) {
        return 6;
    }

    return 0; // success
}