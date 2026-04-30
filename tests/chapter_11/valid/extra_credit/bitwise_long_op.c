/* Test bitwise &, |, and ^ operations on long long integers.
 * Make sure we:
 * - promote both operands to a common type;
 * - actually perform 4-byte (not narrower) operations
 * - use appropriate rewrite rules where one operand is an
 *   immediate that can't fit in a signed 16-bit integer
 */
int main(void) {
    // basic tests to make sure we're performing 4-byte operations
    long long l1 = 16711935ll;  // 0x00ff_00ff
    long long l2 = -65536ll;   // -2^16; upper 16 bits are 1, lower 16 bits are 0

    if ((l1 & l2) != 16711680ll /* 0x00ff_0000 */) {
        return 1;
    }

    if ((l1 | l2) != -65281ll /* 0xffff_00ff */) {
        return 2;
    }

    if ((l1 ^ l2) != -16776961ll /* 0xff00_00ff */) {
        return 3;
    }

    /* Rewrite rules: 4-byte AND $IMM, mem doesn't work if $IMM
     * can't fit in 16 bits. Ditto for OR and XOR */
    if ((-1ll & 65536ll) != 65536ll) {  // 65536 == 2^16
        return 4;
    }

    if ((0ll | 65536ll) != 65536ll) {
        return 5;
    }

    // 262144 == 2^18; 65536 ^ 262144 == 327680 (= 2^16 + 2^18,
    // since the two bits don't overlap)
    if ((65536ll ^ 262144ll) != 327680ll) {
        return 6;
    }

    /* Typechecking: promote both operands to common type */
    long long l = 1073741823ll;  // 0x3fff_ffff
    // if we try to use i in 4-byte bitwise op without sign-extending it
    // first, we may try to read neighboring values l and i2
    int i = -64;  // 1-byte int 0xc0
    int i2 = -1;

    // 1. sign-extend i to 32 bits; upper 24 bits are all 1s
    // 2. take bitwise AND of sign-extended value with l
    // 3. result is 0x3fff_ffc0; upper bits match l, low byte matches i
    if ((i & l) != 1073741760ll) {
        return 7;
    }

    // i is sign-extended so upper bytes are 1s; lower bytes of l are 1s
    if ((l | i) != -1) {
        return 8;
    }

    // 0x3fff_ffff ^ 0xffff_ffc0 = 0xc000_003f
    if ((l ^ i) != -1073741761ll) {
        return 9;
    }

    // 1. sign extend i2; value is still -1
    // 2. XOR result w/ 0x3fff_ffff (as a constant this time)
    // 3. result is the same as taking bitwise complement of 0x3fff_ffff
    if ((i2 ^ 1073741823ll) != ~1073741823ll) {
        return 10;
    }

    return 0;  // success
}