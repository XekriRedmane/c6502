// Test bit shift operations on long integers; the main focus is making sure
// we type check them correctly
int main(void) {

    long long l = 131072ll; // 2^17
    int shiftcount = 2;

    if (l >> shiftcount != 32768ll /* 2 ^ 15 */) {
        return 1;
    }

    if (l << shiftcount != 524288ll /* 2 ^ 19 */) {
        return 2;
    }

    // test w/ immediate right operand too
    if (l << 2 != 524288ll /* 2 ^ 19 */) {
        return 3;
    }

    // try shift count > 16 (shift count between 16 and 32 is undefined when
    // shifting a long, well-defined when shifting a long long)
    if ((40ll << 20) != 41943040ll) {
        return 4;
    }

    // use long long as right shift operand
    // NOTE: we shouldn't perform usual arithmetic conversions here
    // (result has same type as left operand) but we won't be able to fully
    // validate that until chapter 12
    long long long_shiftcount = 3ll;

    // declare some variables near i; we'll make sure they aren't clobbered by
    // bit shift operations
    int i_neighbor1 = 0;
    int i = -125; // -2^7 + 3
    int i_neighbor2 = 0;

    // should be -16 (-125 >> 3, arithmetic right shift)
    if (i >> long_shiftcount != -16) {
        return 5;
    }

    i = -1;
    if (i >> 3ll != -1) {
        return 6;
    }

    // make sure we didn't shift any bits into i's neighbors
    if (i_neighbor1) {
        return 7;
    }

    if (i_neighbor2) {
        return 8;
    }

    return 0;
}