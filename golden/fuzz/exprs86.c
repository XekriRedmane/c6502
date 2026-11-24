int check(signed char v0, signed char v1, unsigned _BitInt(42) v2, unsigned _BitInt(17) v3, unsigned char v4, _BitInt(58) v5) {
    int fail = 0;
    { signed char r = v1; if (r != ((signed char)0)) fail |= 1; }
    { int r = (((unsigned int)((~((unsigned long)255UL)))) && ((unsigned _BitInt(17))0x80ULL)); if (r != ((int)1)) fail |= 2; }
    { int r = (((v4 == ((_BitInt(58))0x21E3469AB1A23EAULL)) || (((int)65535UL) ^ v3)) && (v0 ^ (((unsigned _BitInt(42))0x2ULL) != v3))); if (r != ((int)1)) fail |= 4; }
    { int r = ((v5 ^ (((unsigned long)2147483647UL) > v2)) >= ((((signed char)1) >= v0) >= v0)); if (r != ((int)1)) fail |= 8; }
    { unsigned _BitInt(42) r = (((((_BitInt(58))0x7FFFULL) | v1) > v1) * ((unsigned _BitInt(42))0x1ULL)); if (r != ((unsigned _BitInt(42))0x1ULL)) fail |= 16; }
    { int r = (((v1 * v2) % v5) || v2); if (r != ((int)0)) fail |= 32; }
    { int r = (((unsigned _BitInt(42))0x1ULL) && ((v4 != v5) > (v1 || ((int)2)))); if (r != ((int)0)) fail |= 64; }
    { unsigned _BitInt(42) r = v2; if (r != ((unsigned _BitInt(42))0x0ULL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((signed char)128), ((signed char)0), ((unsigned _BitInt(42))0x0ULL), ((unsigned _BitInt(17))0x1ULL), ((unsigned char)255), ((_BitInt(58))0xCULL)); }
