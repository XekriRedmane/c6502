int check(signed char v0, int v1, unsigned int v2, signed char v3, signed char v4, int v5) {
    int fail = 0;
    { int r = (-((unsigned char)127)); if (r != ((int)65409UL)) fail |= 1; }
    { int r = (v5 - v0); if (r != ((int)65534UL)) fail |= 2; }
    { int r = (!v4); if (r != ((int)0)) fail |= 4; }
    { int r = ((v4 > (v5 % v4)) + (-v0)); if (r != ((int)65533UL)) fail |= 8; }
    { unsigned char r = ((unsigned char)255); if (r != ((unsigned char)255)) fail |= 16; }
    { long r = ((((long)2147483648UL) | v4) + (((unsigned char)(v5)) + v3)); if (r != ((long)4294967168UL)) fail |= 32; }
    { int r = (v2 < ((long)65535L)); if (r != ((int)1)) fail |= 64; }
    { int r = ((int)((((signed char)(v0)) && v5))); if (r != ((int)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(((signed char)3), ((int)14), ((unsigned int)1593u), ((signed char)128), ((signed char)255), ((int)1)); }
