int main(void) {

    // c6502 currently always converts shift operands to a common type
    // (non-standard per C99 §6.5.7). Use a signed shift count so the
    // common type stays signed and we exercise arithmetic right shift.
    int i = -2;
    i >>= 3;
    if (i != -1) {
        return 1;
    }

    unsigned long long ul = 4294967295ULL;  // 2^32 - 1
    ul <<= 22;                              // 0 out lower 22 bits
    if (ul != 4290772992ull) {
        return 2;  // fail
    }
    return 0;  // success
}