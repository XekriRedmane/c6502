int check(unsigned _BitInt(53) v0, unsigned _BitInt(53) v1, _BitInt(9) v2, unsigned char v3, signed char v4, unsigned int v5) {
    int fail = 0;
    { int r = ((!((int)255)) > (v2 >> (v2 > v4))); if (r != ((int)0)) fail |= 1; }
    { unsigned char r = v3; if (r != ((unsigned char)4)) fail |= 2; }
    { unsigned _BitInt(53) r = v0; if (r != ((unsigned _BitInt(53))0x0ULL)) fail |= 4; }
    { _BitInt(9) r = ((_BitInt(9))(v4)); if (r != ((_BitInt(9))0x1ULL)) fail |= 8; }
    { unsigned _BitInt(53) r = (v5 & (v0 / (((unsigned long)255UL) ^ ((unsigned _BitInt(53))0x2ULL)))); if (r != ((unsigned _BitInt(53))0x0ULL)) fail |= 16; }
    { unsigned int r = (((int)(v0)) | (v5 << (v2 && v3))); if (r != ((unsigned int)16930u)) fail |= 32; }
    { unsigned _BitInt(53) r = ((unsigned _BitInt(53))0x1ULL); if (r != ((unsigned _BitInt(53))0x1ULL)) fail |= 64; }
    { unsigned _BitInt(53) r = (v0 | v0); if (r != ((unsigned _BitInt(53))0x0ULL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned _BitInt(53))0x0ULL), ((unsigned _BitInt(53))0xDULL), ((_BitInt(9))0xEULL), ((unsigned char)4), ((signed char)1), ((unsigned int)41233u)); }
