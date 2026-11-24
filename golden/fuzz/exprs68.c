int check(unsigned long v0, unsigned long v1, signed char v2, unsigned int v3, unsigned long v4, int v5) {
    int fail = 0;
    { int r = (((signed char)128) | ((unsigned char)255)); if (r != ((int)65535UL)) fail |= 1; }
    { unsigned int r = ((unsigned int)255u); if (r != ((unsigned int)255u)) fail |= 2; }
    { unsigned int r = (((v2 + v0) >= (((unsigned long)1UL) <= v3)) - ((unsigned int)65535u)); if (r != ((unsigned int)2u)) fail |= 4; }
    { unsigned int r = ((unsigned int)2u); if (r != ((unsigned int)2u)) fail |= 8; }
    { signed char r = ((signed char)(((v4 <= ((long)256L)) + (v3 >> v5)))); if (r != ((signed char)1)) fail |= 16; }
    { unsigned long r = v1; if (r != ((unsigned long)1UL)) fail |= 32; }
    { unsigned int r = v3; if (r != ((unsigned int)0u)) fail |= 64; }
    { int r = (((int)9029) * (((signed char)(v3)) / (((unsigned char)0) >= ((int)49447UL)))); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned long)0UL), ((unsigned long)1UL), ((signed char)9), ((unsigned int)0u), ((unsigned long)1UL), ((int)0)); }
