int check(int v0, unsigned char v1, int v2, unsigned char v3, unsigned _BitInt(64) v4, unsigned int v5) {
    int fail = 0;
    { unsigned int r = (-v5); if (r != ((unsigned int)0u)) fail |= 1; }
    { int r = (v5 > ((((int)65535UL) & v3) >= ((unsigned char)2))); if (r != ((int)0)) fail |= 2; }
    { signed char r = ((signed char)7); if (r != ((signed char)7)) fail |= 4; }
    { int r = (v5 && v5); if (r != ((int)0)) fail |= 8; }
    { unsigned char r = v3; if (r != ((unsigned char)128)) fail |= 16; }
    { unsigned _BitInt(64) r = (v4 + ((v3 >> v1) > (v4 * v1))); if (r != ((unsigned _BitInt(64))0xFFFFFFFFFFFFFFFFULL)) fail |= 32; }
    { unsigned _BitInt(19) r = ((unsigned _BitInt(19))(((unsigned _BitInt(6))(v3)))); if (r != ((unsigned _BitInt(19))0x0ULL)) fail |= 64; }
    { unsigned _BitInt(6) r = ((unsigned _BitInt(6))(((((int)256) != v4) == (-v0)))); if (r != ((unsigned _BitInt(6))0x0ULL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((int)0), ((unsigned char)1), ((int)65535UL), ((unsigned char)128), ((unsigned _BitInt(64))0xFFFFFFFFFFFFFFFFULL), ((unsigned int)0u)); }
