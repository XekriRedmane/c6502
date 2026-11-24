int check(unsigned char v0, unsigned int v1, unsigned char v2, unsigned char v3, signed char v4, unsigned _BitInt(9) v5) {
    int fail = 0;
    { signed char r = v4; if (r != ((signed char)3)) fail |= 1; }
    { int r = (((-v2) / ((unsigned char)255)) | (((unsigned _BitInt(43))(v0)) > v5)); if (r != ((int)1)) fail |= 2; }
    { unsigned int r = ((unsigned int)(v0)); if (r != ((unsigned int)128u)) fail |= 4; }
    { int r = ((v5 & (v4 ^ v4)) == ((v2 - ((signed char)0)) <= (v4 != ((unsigned long)65535UL)))); if (r != ((int)0)) fail |= 8; }
    { unsigned _BitInt(9) r = v5; if (r != ((unsigned _BitInt(9))0x1ULL)) fail |= 16; }
    { int r = (((((unsigned _BitInt(9))0x1ULL) & ((unsigned char)255)) << (v3 && v3)) & (((unsigned _BitInt(9))0x7FULL) && (v1 << v5))); if (r != ((int)0)) fail |= 32; }
    { signed char r = ((signed char)(((!v1) ^ (((unsigned long)0UL) >> v2)))); if (r != ((signed char)1)) fail |= 64; }
    { unsigned char r = v0; if (r != ((unsigned char)128)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned char)128), ((unsigned int)0u), ((unsigned char)1), ((unsigned char)151), ((signed char)3), ((unsigned _BitInt(9))0x1ULL)); }
