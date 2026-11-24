unsigned char v0 = ((unsigned char)15);
int v1 = ((int)32768UL);
unsigned char v2 = ((unsigned char)128);
unsigned char v3 = ((unsigned char)0);
unsigned _BitInt(21) v4 = ((unsigned _BitInt(21))0x1FFFFFULL);
unsigned _BitInt(18) v5 = ((unsigned _BitInt(18))0x0ULL);
int check(void) {
    int fail = 0;
    { unsigned _BitInt(21) r = ((unsigned _BitInt(21))((v1 ^ (v3 | v2)))); if (r != ((unsigned _BitInt(21))0x1F8080ULL)) fail |= 1; }
    { unsigned char r = v2; if (r != ((unsigned char)128)) fail |= 2; }
    { int r = ((((unsigned _BitInt(21))(v3)) >= (v0 || v3)) + (!(((unsigned long)255UL) || v2))); if (r != ((int)0)) fail |= 4; }
    { unsigned int r = ((unsigned int)128u); if (r != ((unsigned int)128u)) fail |= 8; }
    { int r = (((((int)7) > v5) << (v0 && v4)) >= v5); if (r != ((int)1)) fail |= 16; }
    { unsigned _BitInt(18) r = (((v2 / ((unsigned _BitInt(18))0x8000ULL)) * ((int)(((long)7L)))) >> (((int)(((_BitInt(61))0x7FFFULL))) != (v3 / ((unsigned char)1)))); if (r != ((unsigned _BitInt(18))0x0ULL)) fail |= 32; }
    { int r = (((_BitInt(61))0x80000000ULL) != ((unsigned _BitInt(21))0x80ULL)); if (r != ((int)1)) fail |= 64; }
    { unsigned char r = v2; if (r != ((unsigned char)128)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
