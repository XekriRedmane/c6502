// compound bitwise assignment on subscript expressions
int main(void) {
    unsigned long long arr[4] = {
        65536ll,                    // 2^16
        4294901760ull,              // 0xffff_0000
        2147483648ull,              // 2^31
        252645135ll                 // 0x0f0f_0f0f
    };

    // &=
    arr[1] &= arr[3];
    if (arr[1] != 252641280 /* 0x0f0f_0000 */) {
        return 1;
    }

    // |=
    arr[0] |= arr[1];
    if (arr[0] != 252706816ull) {
        return 2;
    }

    // ^=
    arr[2] ^= arr[3];
    if (arr[2] != 2400128783ull /* 0x8f0f_0f0f */) {
        return 3;
    }

    // >>=
    arr[3] >>= 12;
    if (arr[3] != 61680ll) {
        return 4;
    }

    // <<=
    arr[1] <<= 4;
    if (arr[1] != 4042620928ull) {
        return 5;
    }

    return 0; // success
}