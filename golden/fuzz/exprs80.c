int check(int v0, unsigned int v1, unsigned int v2, unsigned int v3, signed char v4, long v5) {
    int fail = 0;
    { int r = (-v4); if (r != ((int)65533UL)) fail |= 1; }
    { int r = (((((unsigned char)127) + v2) == (v3 - v4)) > v3); if (r != ((int)0)) fail |= 2; }
    { long r = ((long)((v1 + v3))); if (r != ((long)2L)) fail |= 4; }
    { int r = (((((long)2147483647L) || v3) > (v3 * v1)) >> v2); if (r != ((int)0)) fail |= 8; }
    { unsigned int r = ((v1 & (((int)0) != v5)) << ((unsigned char)((-v5)))); if (r != ((unsigned int)0u)) fail |= 16; }
    { int r = (((unsigned char)128) >> (((unsigned int)(v3)) % (((signed char)255) | v3))); if (r != ((int)64)) fail |= 32; }
    { int r = (((v5 / ((unsigned char)2)) <= ((unsigned char)(v2))) != (v0 != (v2 / v2))); if (r != ((int)0)) fail |= 64; }
    { long r = v5; if (r != ((long)0L)) fail |= 128; }
    return fail;
}
int main(void) { return check(((int)0), ((unsigned int)1u), ((unsigned int)1u), ((unsigned int)1u), ((signed char)3), ((long)0L)); }
