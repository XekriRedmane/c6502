int check(unsigned int v0, signed char v1, signed char v2, int v3, unsigned int v4, unsigned long v5) {
    int fail = 0;
    { int r = (v2 == ((unsigned _BitInt(24))0x7ULL)); if (r != ((int)0)) fail |= 1; }
    { int r = (-(((unsigned _BitInt(24))(((unsigned _BitInt(24))0xFFULL))) > v4)); if (r != ((int)0)) fail |= 2; }
    { unsigned long r = v5; if (r != ((unsigned long)2147483648UL)) fail |= 4; }
    { long r = ((((long)2L) + v3) * ((v0 != ((int)256)) << (v3 == ((_BitInt(9))0x1FFULL)))); if (r != ((long)4294934530UL)) fail |= 8; }
    { _BitInt(7) r = ((_BitInt(7))((((long)(((long)256L))) > ((signed char)(v3))))); if (r != ((_BitInt(7))0x1ULL)) fail |= 16; }
    { int r = ((int)(((v4 + v2) < (v0 <= ((unsigned _BitInt(24))0x1ULL))))); if (r != ((int)0)) fail |= 32; }
    { unsigned int r = (((v4 == v1) <= (v4 > v2)) / v4); if (r != ((unsigned int)0u)) fail |= 64; }
    { int r = (((v4 < ((_BitInt(7))0x7FULL)) - v1) >= ((v2 * v5) > (v1 - v4))); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned int)0u), ((signed char)14), ((signed char)0), ((int)32768UL), ((unsigned int)65535u), ((unsigned long)2147483648UL)); }
