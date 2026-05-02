// compound bitwise assignment on subscript expressions
int main(void) {
    unsigned long long arr[4] = {
        32768ll,                    // 2^15 (the bit just below arr[1]'s 0x0F0F0000 mask)
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
    if (arr[0] != 252674048ull /* 0x0f0f_8000 */) {
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
    if (arr[1] != 4042260480ull /* 0xf0f0_0000 */) {
        return 5;
    }

    return 0; // success
}