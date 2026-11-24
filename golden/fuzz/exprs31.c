int check(signed char v0, unsigned char v1, signed char v2, unsigned char v3, signed char v4, unsigned char v5) {
    int fail = 0;
    { int r = (((unsigned char)((v1 ^ v2))) * (((int)(v1)) + v3)); if (r != ((int)0)) fail |= 1; }
    { int r = (((((long)256L) != ((signed char)0)) & ((unsigned int)(v2))) || (~(((unsigned char)7) || ((int)2)))); if (r != ((int)1)) fail |= 2; }
    { int r = (((v5 / ((int)128)) - (v2 & v4)) >> ((unsigned int)((-v2)))); if (r != ((int)1)) fail |= 4; }
    { long r = ((((int)(v2)) | (((unsigned char)0) ^ ((unsigned int)7u))) * (((long)(v1)) - (v5 >= v1))); if (r != ((long)4294967289UL)) fail |= 8; }
    { int r = (((int)(v2)) << ((((unsigned char)2) / ((long)128L)) && (v4 != v2))); if (r != ((int)0)) fail |= 16; }
    { unsigned int r = ((v4 < ((signed char)255)) ^ ((unsigned int)((((unsigned int)65535u) - v2)))); if (r != ((unsigned int)65535u)) fail |= 32; }
    { int r = (v3 | ((((unsigned long)128UL) == v0) <= ((long)(v2)))); if (r != ((int)1)) fail |= 64; }
    { int r = ((((unsigned int)255u) < ((unsigned long)2218379038UL)) < ((v4 | v5) << v1)); if (r != ((int)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(((signed char)255), ((unsigned char)0), ((signed char)0), ((unsigned char)0), ((signed char)0), ((unsigned char)255)); }
