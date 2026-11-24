signed char v0 = ((signed char)1);
signed char v1 = ((signed char)255);
long v2 = ((long)12L);
signed char v3 = ((signed char)128);
unsigned char v4 = ((unsigned char)0);
unsigned long v5 = ((unsigned long)6UL);
int check(void) {
    int fail = 0;
    { long r = (v3 * ((long)((v1 != ((unsigned int)0u))))); if (r != ((long)4294967168UL)) fail |= 1; }
    { long r = (v2 << (!(v1 ^ ((unsigned _BitInt(53))0x100ULL)))); if (r != ((long)12L)) fail |= 2; }
    { signed char r = v0; if (r != ((signed char)1)) fail |= 4; }
    { int r = ((v2 * (v0 ^ v2)) == (((_BitInt(49))0x7ULL) != (v3 & v0))); if (r != ((int)0)) fail |= 8; }
    { int r = (~((v0 + ((unsigned _BitInt(53))0x80000000ULL)) == ((unsigned char)(v1)))); if (r != ((int)65535UL)) fail |= 16; }
    { _BitInt(28) r = ((_BitInt(28))0x100ULL); if (r != ((_BitInt(28))0x100ULL)) fail |= 32; }
    { int r = (((v5 - v4) >= (v5 >> v0)) & (((unsigned char)(v4)) > (v1 | v3))); if (r != ((int)1)) fail |= 64; }
    { signed char r = v1; if (r != ((signed char)255)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
