unsigned _BitInt(29) v0 = ((unsigned _BitInt(29))0x10000000ULL);
unsigned int v1 = ((unsigned int)32768u);
unsigned _BitInt(4) v2 = ((unsigned _BitInt(4))0x8ULL);
unsigned _BitInt(33) v3 = ((unsigned _BitInt(33))0xEULL);
unsigned _BitInt(29) v4 = ((unsigned _BitInt(29))0x0ULL);
unsigned char v5 = ((unsigned char)255);
int check(void) {
    int fail = 0;
    { long r = (v0 & ((long)0L)); if (r != ((long)0L)) fail |= 1; }
    { int r = ((-(v3 > ((unsigned _BitInt(29))0x0ULL))) * (!(v1 != v5))); if (r != ((int)0)) fail |= 2; }
    { unsigned int r = v1; if (r != ((unsigned int)32768u)) fail |= 4; }
    { unsigned _BitInt(33) r = (v3 - (((unsigned long)0UL) > (v5 * v3))); if (r != ((unsigned _BitInt(33))0xEULL)) fail |= 8; }
    { long r = ((long)2L); if (r != ((long)2L)) fail |= 16; }
    { int r = (((v1 | v3) - (v1 < v1)) <= (-(v3 % v0))); if (r != ((int)1)) fail |= 32; }
    { int r = (((signed char)0) < ((v1 | v4) > v3)); if (r != ((int)1)) fail |= 64; }
    { int r = ((!(((unsigned _BitInt(33))0x1ULL) - v0)) != (~(v3 % v5))); if (r != ((int)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
