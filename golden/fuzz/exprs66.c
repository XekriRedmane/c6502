unsigned char v0 = ((unsigned char)6);
unsigned _BitInt(27) v1 = ((unsigned _BitInt(27))0x1ULL);
unsigned char v2 = ((unsigned char)255);
int v3 = ((int)8);
unsigned char v4 = ((unsigned char)0);
int v5 = ((int)0);
int check(void) {
    int fail = 0;
    { unsigned char r = v4; if (r != ((unsigned char)0)) fail |= 1; }
    { int r = v3; if (r != ((int)8)) fail |= 2; }
    { int r = v5; if (r != ((int)0)) fail |= 4; }
    { unsigned _BitInt(44) r = ((unsigned _BitInt(44))0x80000000ULL); if (r != ((unsigned _BitInt(44))0x80000000ULL)) fail |= 8; }
    { unsigned _BitInt(39) r = ((((long)0L) & (v3 && v4)) / (v2 & ((unsigned _BitInt(39))(v1)))); if (r != ((unsigned _BitInt(39))0x0ULL)) fail |= 16; }
    { signed char r = ((signed char)(((unsigned int)9029u))); if (r != ((signed char)69)) fail |= 32; }
    { int r = (v2 >= v2); if (r != ((int)1)) fail |= 64; }
    { int r = (((int)2) >> ((((unsigned char)255) & ((unsigned char)0)) < (((unsigned _BitInt(39))0x7FFFFFFFULL) < v0))); if (r != ((int)2)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
