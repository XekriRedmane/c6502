// Test access to static union members with . and ->
// (Locally adapted from upstream: upstream uses 8-byte `unsigned long`
// to fill all 8 bytes of the union; c6502's widest integer is 4-byte
// `unsigned long long`, so `l` is widened to that and the byte-check
// loops are split — bytes 0..3 are filled by `l`, bytes 4..7 are
// zero-padded.)
union u {
    unsigned long long l;
    double d;
    char arr[8];
};

static union u my_union = { 4294967295ULL };
static union u* union_ptr = 0;

int main(void) {
    union_ptr = &my_union;
    if (my_union.l != 4294967295ULL) {
        return 1; // fail
    }

    // bytes 0..3 of `l` are 0xFF; bytes 4..7 are zero-padded since
    // the union member is only 4 bytes.
    for (int i = 0; i < 4; i = i + 1) {
        if (my_union.arr[i] != -1) {
            return 2; // fail
        }
    }
    for (int i = 4; i < 8; i = i + 1) {
        if (my_union.arr[i]) {
            return 7; // fail (was: tail bytes should be zero-padded)
        }
    }

    union_ptr->d = -1.0;

    // -1.0 as IEEE 754 double (little-endian) = 00 00 00 00 00 00 F0 BF.
    // Reading the low 4 bytes as `unsigned long long` = 0.
    if (union_ptr->l != 0) {
        return 3; // fail
    }

    for (int i = 0; i < 6; i = i + 1) {
        // lower 6 bytes are 0
        if (my_union.arr[i]) {
            return 4; // fail
        }
    }
    if (union_ptr->arr[6] != -16) {  // 0xF0 = -16 signed
        return 5; // fail
    }

    if (union_ptr->arr[7] != -65) {  // 0xBF = -65 signed
        return 6; // fail
    }

    return 0; // success
}
