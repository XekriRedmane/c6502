int check(unsigned char v0, unsigned char v1, signed char v2, unsigned char v3, unsigned int v4, unsigned int v5) {
    int fail = 0;
    { unsigned char r = v0; if (r != ((unsigned char)255)) fail |= 1; }
    { int r = (((v0 / v3) * (v3 % ((long)7L))) || ((int)((~((unsigned int)32768u))))); if (r != ((int)1)) fail |= 2; }
    { signed char r = v2; if (r != ((signed char)1)) fail |= 4; }
    { int r = (v1 ^ (((unsigned char)1) >= ((int)2))); if (r != ((int)49)) fail |= 8; }
    { unsigned char r = v3; if (r != ((unsigned char)102)) fail |= 16; }
    { long r = (((long)(v4)) & ((((signed char)0) / ((int)65535UL)) & (((signed char)0) - v0))); if (r != ((long)0L)) fail |= 32; }
    { int r = (v1 ^ (v4 == (((long)1L) > ((unsigned int)255u)))); if (r != ((int)49)) fail |= 64; }
    { unsigned char r = v0; if (r != ((unsigned char)255)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned char)255), ((unsigned char)49), ((signed char)1), ((unsigned char)102), ((unsigned int)65535u), ((unsigned int)65535u)); }
