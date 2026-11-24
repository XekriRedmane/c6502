_BitInt(2) v0 = ((_BitInt(2))0x0ULL);
unsigned long v1 = ((unsigned long)0UL);
signed char v2 = ((signed char)128);
unsigned char v3 = ((unsigned char)128);
int v4 = ((int)21931);
unsigned _BitInt(55) v5 = ((unsigned _BitInt(55))0xAULL);
int check(void) {
    int fail = 0;
    { long r = (((long)127L) * ((v5 << v5) <= (v4 || v0))); if (r != ((long)0L)) fail |= 1; }
    { int r = (((unsigned long)(((unsigned long)(v4)))) <= v3); if (r != ((int)0)) fail |= 2; }
    { unsigned char r = v3; if (r != ((unsigned char)128)) fail |= 4; }
    { unsigned _BitInt(55) r = v5; if (r != ((unsigned _BitInt(55))0xAULL)) fail |= 8; }
    { unsigned _BitInt(55) r = (((unsigned int)128u) | ((((unsigned _BitInt(55))0x100ULL) | v0) ^ (~v5))); if (r != ((unsigned _BitInt(55))0x7FFFFFFFFFFEF5ULL)) fail |= 16; }
    { int r = ((((int)2) % (-((int)127))) != v1); if (r != ((int)1)) fail |= 32; }
    { int r = (v4 + ((v2 < v4) <= v2)); if (r != ((int)21931)) fail |= 64; }
    { int r = (((v5 - v1) < (v0 <= v4)) || v1); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
