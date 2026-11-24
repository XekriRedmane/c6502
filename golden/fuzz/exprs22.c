int check(int v0, unsigned _BitInt(8) v1, unsigned long v2, unsigned int v3, unsigned long v4, _BitInt(16) v5) {
    int fail = 0;
    { _BitInt(16) r = v5; if (r != ((_BitInt(16))0xFFFFULL)) fail |= 1; }
    { int r = (((v3 <= v3) <= ((unsigned int)7u)) & v1); if (r != ((int)1)) fail |= 2; }
    { unsigned _BitInt(8) r = v1; if (r != ((unsigned _BitInt(8))0x2FULL)) fail |= 4; }
    { unsigned long r = (v4 / ((unsigned int)9029u)); if (r != ((unsigned long)0UL)) fail |= 8; }
    { unsigned _BitInt(8) r = v1; if (r != ((unsigned _BitInt(8))0x2FULL)) fail |= 16; }
    { int r = (((((unsigned _BitInt(8))0x45ULL) > v5) & ((signed char)(((long)1L)))) << ((!v3) || ((unsigned int)(((unsigned long)1UL))))); if (r != ((int)2)) fail |= 32; }
    { unsigned _BitInt(30) r = ((unsigned _BitInt(30))(((unsigned _BitInt(8))0x7ULL))); if (r != ((unsigned _BitInt(30))0x7ULL)) fail |= 64; }
    { int r = ((v4 <= v3) == v3); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(((int)1), ((unsigned _BitInt(8))0x2FULL), ((unsigned long)3404091302UL), ((unsigned int)17646u), ((unsigned long)9UL), ((_BitInt(16))0xFFFFULL)); }
