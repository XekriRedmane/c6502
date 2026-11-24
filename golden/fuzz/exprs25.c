int check(unsigned int v0, unsigned char v1, unsigned long v2, signed char v3, signed char v4, long v5) {
    int fail = 0;
    { int r = ((v3 | (((unsigned long)32768UL) > v3)) >= (v0 ^ (-((signed char)2)))); if (r != ((int)1)) fail |= 1; }
    { long r = (((((unsigned int)2u) <= v3) * (v1 > v4)) ^ (-v5)); if (r != ((long)4294967284UL)) fail |= 2; }
    { long r = (-((long)((v4 * v2)))); if (r != ((long)0L)) fail |= 4; }
    { int r = (((~v0) == (v0 + ((unsigned long)127UL))) < ((v1 - v3) % ((long)(v5)))); if (r != ((int)1)) fail |= 8; }
    { unsigned long r = (v4 % v2); if (r != ((unsigned long)2147483542UL)) fail |= 16; }
    { long r = ((long)(v1)); if (r != ((long)128L)) fail |= 32; }
    { signed char r = ((signed char)((!((unsigned long)127UL)))); if (r != ((signed char)0)) fail |= 64; }
    { unsigned int r = v0; if (r != ((unsigned int)50363u)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned int)50363u), ((unsigned char)128), ((unsigned long)2147483648UL), ((signed char)255), ((signed char)150), ((long)11L)); }
