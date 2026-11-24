unsigned long v0 = ((unsigned long)2469463668UL);
unsigned long v1 = ((unsigned long)4294967295UL);
long v2 = ((long)0L);
unsigned _BitInt(52) v3 = ((unsigned _BitInt(52))0x7ULL);
_BitInt(44) v4 = ((_BitInt(44))0x0ULL);
unsigned long v5 = ((unsigned long)17267755UL);
int check(void) {
    int fail = 0;
    { int r = (((v4 && v1) || (v1 >= v1)) & ((int)255)); if (r != ((int)1)) fail |= 1; }
    { int r = (((v0 | ((_BitInt(44))0x80000000ULL)) >= ((unsigned char)(v4))) << ((v5 != v3) % (((_BitInt(44))0x100ULL) - ((signed char)44)))); if (r != ((int)2)) fail |= 2; }
    { _BitInt(44) r = ((~v4) - ((v5 + v1) < (((unsigned long)2147483647UL) & ((unsigned char)0)))); if (r != ((_BitInt(44))0xFFFFFFFFFFFULL)) fail |= 4; }
    { unsigned long r = (v1 * ((v1 / v0) + (!((unsigned int)1u)))); if (r != ((unsigned long)4294967295UL)) fail |= 8; }
    { unsigned _BitInt(52) r = ((unsigned _BitInt(52))(((unsigned int)32768u))); if (r != ((unsigned _BitInt(52))0x8000ULL)) fail |= 16; }
    { long r = v2; if (r != ((long)0L)) fail |= 32; }
    { int r = (((v3 - v4) >= ((unsigned int)65535u)) << ((unsigned _BitInt(56))0x2ULL)); if (r != ((int)0)) fail |= 64; }
    { unsigned _BitInt(56) r = (v3 & ((((_BitInt(44))0x38D9049D37DULL) < v2) * ((unsigned _BitInt(56))(v5)))); if (r != ((unsigned _BitInt(56))0x0ULL)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
