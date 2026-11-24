unsigned _BitInt(55) v0 = ((unsigned _BitInt(55))0x40000000000000ULL);
unsigned _BitInt(55) v1 = ((unsigned _BitInt(55))0x0ULL);
unsigned char v2 = ((unsigned char)10);
long v3 = ((long)0L);
long v4 = ((long)1L);
signed char v5 = ((signed char)10);
int check(void) {
    int fail = 0;
    { int r = (((((unsigned int)32768u) > v3) & (((signed char)128) + v0)) == ((v2 * v3) >= (v4 ^ v4))); if (r != ((int)0)) fail |= 1; }
    { signed char r = ((signed char)128); if (r != ((signed char)128)) fail |= 2; }
    { unsigned _BitInt(9) r = ((unsigned _BitInt(9))0x7FULL); if (r != ((unsigned _BitInt(9))0x7FULL)) fail |= 4; }
    { int r = ((-(-v1)) > (-(((unsigned _BitInt(9))0x7FULL) - v3))); if (r != ((int)0)) fail |= 8; }
    { int r = (v3 > ((int)((((unsigned long)2UL) << v3)))); if (r != ((int)0)) fail |= 16; }
    { unsigned _BitInt(51) r = (((unsigned _BitInt(51))0x2ULL) * ((unsigned _BitInt(51))((((unsigned _BitInt(55))0x80ULL) == v3)))); if (r != ((unsigned _BitInt(51))0x0ULL)) fail |= 32; }
    { long r = v4; if (r != ((long)1L)) fail |= 64; }
    { int r = (~((((unsigned int)255u) == v2) >> (v4 & v3))); if (r != ((int)65535UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
