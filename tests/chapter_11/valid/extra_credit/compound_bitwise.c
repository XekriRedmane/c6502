int main(void) {

    // bitwise compound operations on long long integers
    long long l1 = 16711935ll;  // 0x00ff_00ff
    long long l2 = -65536ll;   // -2^16; upper 16 bits are 1, lower 16 bits are 0

    l1 &= l2;
    if (l1 != 16711680ll) {
        return 1; // fail
    }

    l2 |= 100ll;
    if (l2 != -65436ll) {
        return 2;
    }

    l1 ^= -2147483647ll;
    if (l1 != -2130771967ll /* 0x80ff_0001 */ ) {
        return 3;
    }

    // if rval is int, convert to common type
    l1 = 1073741823ll;  // 0x3fff_ffff
    int i = -64;  // 1-byte int 0xc0
    // 1. sign-extend i to 32 bits; upper 24 bits are all 1s
    // 2. take bitwise AND of sign-extended value with l1
    // 3. result (stored in l1) is 0x3fff_ffc0;
    //    upper bits match l1, low byte matches i
    l1 &= i;
    if (l1 != 1073741760ll) {
        return 4;
    }

    // if lval is int, convert to common type, perform operation, then convert back
    i = -128ll; // int min, 0x80
    // check result and side effect
    // 1. sign extend 0x80 to 0xffff_ff80
    // 2. calculate 0xffff_ff80 | 0x00ff_00ff = 0xffff_ffff
    // 3. truncate to 0xff on assignment, which is -1 as signed 1-byte int
    if ((i |= 16711935ll) != -1) {
        return 5;
    }
    if (i != -1) {
        return 6;
    }

    return 0; // success

}