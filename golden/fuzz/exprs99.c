int check(unsigned int v0, unsigned char v1, signed char v2, long v3, signed char v4, unsigned char v5) {
    int fail = 0;
    { int r = (((v1 && ((int)9029)) || ((int)(v0))) * v4); if (r != ((int)65535UL)) fail |= 1; }
    { unsigned int r = (((((unsigned char)255) && v1) >= ((unsigned long)(v3))) % v0); if (r != ((unsigned int)0u)) fail |= 2; }
    { int r = (v1 - ((v3 > ((int)0)) + v2)); if (r != ((int)3)) fail |= 4; }
    { unsigned char r = v5; if (r != ((unsigned char)1)) fail |= 8; }
    { int r = (((v5 == ((unsigned char)255)) > v1) | v2); if (r != ((int)65535UL)) fail |= 16; }
    { unsigned int r = (((((unsigned int)0u) + v3) && v0) + ((~((unsigned int)7u)) | (v4 >> v5))); if (r != ((unsigned int)0u)) fail |= 32; }
    { int r = (v0 <= ((v2 <= v0) == (-v1))); if (r != ((int)0)) fail |= 64; }
    { int r = (v2 / ((!((unsigned int)37538u)) + (v0 && v3))); if (r != ((int)65535UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned int)1u), ((unsigned char)3), ((signed char)255), ((long)1L), ((signed char)255), ((unsigned char)1)); }
