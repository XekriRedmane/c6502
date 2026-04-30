// Test out compound bitwise operations with dereferenced pointers, including
// ones that involve implicit type conversions
// Same operations as tests/chapter_12/valid/extra_credit/compound_bitwise.c

unsigned long long ul = 0xfbfaf9f8ull; // 0xfbfa_f9f8

int main(void) {

    unsigned long long *ul_ptr = &ul;
    *ul_ptr &= -1000;
    if (ul != 0xfbfaf818ull /* 0xfbfa_f818 */) {
        return 1; // fail
    }
    *ul_ptr |= 65280ul;

    if (ul != 0xfbfaff18ull /* 0xfbfa_ff18 */) {
        return 2; // fail
    }
    int i = 100;
    unsigned long ui = 0xf0f0ul; // 0xf0f0
    long long l = -3856ll; // sign-extended to 4B = 0xfffff0f0
    unsigned long *ui_ptr = &ui;
    long long *l_ptr = &l;
    if (*ui_ptr ^= *l_ptr) {
        return 3; // fail
    }
    if (ui) {
        return 4;
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