int check(long v0, long v1, int v2, signed char v3, unsigned long v4, signed char v5) {
    int fail = 0;
    { long r = v0; if (r != ((long)1L)) fail |= 1; }
    { unsigned long r = (~((v5 | ((unsigned long)65535UL)) % ((long)(((unsigned int)255u))))); if (r != ((unsigned long)4294967295UL)) fail |= 2; }
    { signed char r = ((signed char)0); if (r != ((signed char)0)) fail |= 4; }
    { long r = (v0 / (v3 | ((unsigned char)(((int)0))))); if (r != ((long)4294967295UL)) fail |= 8; }
    { signed char r = v3; if (r != ((signed char)255)) fail |= 16; }
    { int r = (v3 & (v3 < v5)); if (r != ((int)0)) fail |= 32; }
    { unsigned int r = ((unsigned int)65535u); if (r != ((unsigned int)65535u)) fail |= 64; }
    { unsigned long r = ((v2 & v4) << ((unsigned char)((v2 >= v1)))); if (r != ((unsigned long)0UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((long)1L), ((long)1L), ((int)25750), ((signed char)255), ((unsigned long)2147483648UL), ((signed char)128)); }
