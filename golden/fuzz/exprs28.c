int check(unsigned char v0, int v1, unsigned _BitInt(15) v2, unsigned int v3, int v4, signed char v5) {
    int fail = 0;
    { unsigned long r = ((unsigned long)2147483648UL); if (r != ((unsigned long)2147483648UL)) fail |= 1; }
    { _BitInt(11) r = ((_BitInt(11))0x345ULL); if (r != ((_BitInt(11))0x345ULL)) fail |= 2; }
    { unsigned int r = (((((long)65535L) || v4) >= (v3 - ((unsigned int)1u))) | ((v3 >> ((int)2)) ^ (-v3))); if (r != ((unsigned int)65531u)) fail |= 4; }
    { unsigned long r = ((unsigned long)74565UL); if (r != ((unsigned long)74565UL)) fail |= 8; }
    { int r = (((v3 >= v1) ^ ((long)7L)) > ((unsigned long)((v5 == v0)))); if (r != ((int)1)) fail |= 16; }
    { int r = v4; if (r != ((int)3)) fail |= 32; }
    { signed char r = v5; if (r != ((signed char)104)) fail |= 64; }
    { int r = ((((int)32767) <= (v4 <= v5)) >> (v4 <= (v4 / v2))); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned char)1), ((int)0), ((unsigned _BitInt(15))0x6ULL), ((unsigned int)6u), ((int)3), ((signed char)104)); }
