int check(long v0, int v1, int v2, int v3, unsigned int v4, unsigned _BitInt(48) v5) {
    int fail = 0;
    { int r = v3; if (r != ((int)1)) fail |= 1; }
    { int r = (((int)(((unsigned _BitInt(12))0x7FULL))) > ((v5 % ((unsigned long)74565UL)) - (v3 - v4))); if (r != ((int)0)) fail |= 2; }
    { int r = (!(((signed char)(v2)) || v3)); if (r != ((int)0)) fail |= 4; }
    { int r = ((v1 > (v1 ^ v4)) < ((v2 * v0) <= (v2 || v3))); if (r != ((int)1)) fail |= 8; }
    { int r = v1; if (r != ((int)0)) fail |= 16; }
    { int r = v2; if (r != ((int)0)) fail |= 32; }
    { unsigned long r = ((unsigned long)715711533UL); if (r != ((unsigned long)715711533UL)) fail |= 64; }
    { int r = v2; if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(((long)0L), ((int)0), ((int)0), ((int)1), ((unsigned int)48804u), ((unsigned _BitInt(48))0x1ULL)); }
