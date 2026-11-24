int check(unsigned int v0, long v1, _BitInt(18) v2, long v3, unsigned char v4, signed char v5) {
    int fail = 0;
    { _BitInt(18) r = ((((int)65535UL) | v2) + v0); if (r != ((_BitInt(18))0xFFFEULL)) fail |= 1; }
    { long r = v1; if (r != ((long)2147483648UL)) fail |= 2; }
    { int r = (!v5); if (r != ((int)1)) fail |= 4; }
    { long r = (((signed char)255) & v3); if (r != ((long)2147483648UL)) fail |= 8; }
    { int r = (v3 > ((unsigned int)((v0 / v3)))); if (r != ((int)0)) fail |= 16; }
    { long r = v1; if (r != ((long)2147483648UL)) fail |= 32; }
    { int r = (!((v1 % ((long)2147483648UL)) >= (v2 / v2))); if (r != ((int)1)) fail |= 64; }
    { unsigned int r = v0; if (r != ((unsigned int)65535u)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned int)65535u), ((long)2147483648UL), ((_BitInt(18))0x20000ULL), ((long)2147483648UL), ((unsigned char)11), ((signed char)0)); }
