#ifdef SUPPRESS_WARNINGS
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
int main(void) {
    unsigned long ui = -1ul; // lower 16 bits set
    unsigned long long ul = 2147483648ull; // 2^31, only uppermost bit set

    /* this expression will:
     * 1. zero-extend ui. the result will have all 16 lower bits set to 1
     *    and all upper bits set to 0
     * 2. calculate the bitwise and of this zero-extended value and ul. the result is 0
     */
    if ((ui & ul) != 0)
        return 1;

    /* this expression will:
     * 1. zero-extend ui. the result will have all 16 lower bits set to 1
     *    and all upper bits set to 0
     * 2. calculate the bitwise or of this zero-extended value and ul.
     *    the result is 2^31 + 2^16 - 1
     */
    if ((ui | ul) != 2147549183ull)
        return 2;

    signed int i = -1;
    /* this expression will:
     * 1. sign-extend i. the result will have every bit set to 1.
     * 2. calculate the bitwise and of this sign-extended value and ul.
     *    the result is equal to ul.
     */
    if ((i & ul) != ul)
        return 3;


    /* this expression will:
     * 1. sign-extend i. the result will have every bit set to 1.
     * 2. calculate the bitwise or of this sign-extended value and ul.
     *    the result will have every bit set
     */
    if ((i | ul) != i)
        return 4;

    return 0; // success
}