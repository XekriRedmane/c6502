int check(unsigned int v0, unsigned int v1, signed char v2, unsigned long v3, long v4, unsigned long v5) {
    int fail = 0;
    { long r = (v4 * ((v0 != ((signed char)7)) >= (((long)65535L) && v1))); if (r != ((long)2898220809UL)) fail |= 1; }
    { long r = ((long)2147483647L); if (r != ((long)2147483647L)) fail |= 2; }
    { int r = (((_BitInt(56))0x7FFFULL) >= v5); if (r != ((int)0)) fail |= 4; }
    { long r = v4; if (r != ((long)2898220809UL)) fail |= 8; }
    { unsigned int r = v1; if (r != ((unsigned int)0u)) fail |= 16; }
    { unsigned int r = v0; if (r != ((unsigned int)17017u)) fail |= 32; }
    { int r = (((v3 | v1) ^ (v4 != ((unsigned long)0UL))) > ((unsigned _BitInt(56))((((unsigned char)0) && v0)))); if (r != ((int)1)) fail |= 64; }
    { int r = (((_BitInt(56))0x2ULL) < ((((unsigned char)255) / v5) >> v1)); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned int)17017u), ((unsigned int)0u), ((signed char)128), ((unsigned long)3051111893UL), ((long)2898220809UL), ((unsigned long)4294967295UL)); }
