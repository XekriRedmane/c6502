unsigned long v0 = ((unsigned long)2183825372UL);
long v1 = ((long)2L);
long v2 = ((long)1L);
signed char v3 = ((signed char)1);
signed char v4 = ((signed char)1);
unsigned long v5 = ((unsigned long)4294967295UL);
int check(void) {
    int fail = 0;
    { int r = (v2 > v4); if (r != ((int)0)) fail |= 1; }
    { long r = (((v5 % v1) == v5) | v1); if (r != ((long)2L)) fail |= 2; }
    { int r = (((signed char)((v4 * ((unsigned _BitInt(16))0x7FULL)))) != ((v4 - v4) < (v2 | v2))); if (r != ((int)1)) fail |= 4; }
    { long r = (v1 << ((((unsigned _BitInt(16))0x7FULL) || v4) & (v5 * v5))); if (r != ((long)4L)) fail |= 8; }
    { int r = ((int)((-v5))); if (r != ((int)1)) fail |= 16; }
    { int r = (v1 <= (~(v0 - v3))); if (r != ((int)1)) fail |= 32; }
    { long r = v1; if (r != ((long)2L)) fail |= 64; }
    { int r = (((~((long)74565L)) * v2) != (v5 - ((unsigned _BitInt(16))(v1)))); if (r != ((int)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
