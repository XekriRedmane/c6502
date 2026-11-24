int check(unsigned _BitInt(19) v0, unsigned _BitInt(7) v1, signed char v2, unsigned int v3, int v4, unsigned _BitInt(19) v5) {
    int fail = 0;
    { unsigned _BitInt(19) r = v0; if (r != ((unsigned _BitInt(19))0x1ULL)) fail |= 1; }
    { unsigned _BitInt(19) r = (-((v0 >> ((unsigned _BitInt(7))0x7ULL)) + (v2 - ((unsigned _BitInt(19))0x2ULL)))); if (r != ((unsigned _BitInt(19))0x82ULL)) fail |= 2; }
    { long r = (v5 / ((long)2147483648UL)); if (r != ((long)0L)) fail |= 4; }
    { int r = ((((unsigned int)2u) & (v5 % v2)) || (~((signed char)2))); if (r != ((int)1)) fail |= 8; }
    { int r = (((unsigned char)((((signed char)69) % v5))) != ((((unsigned long)128UL) + v5) ^ (v1 && v1))); if (r != ((int)1)) fail |= 16; }
    { int r = (((((unsigned char)1) != v2) && v5) <= ((_BitInt(40))0xFFULL)); if (r != ((int)1)) fail |= 32; }
    { unsigned _BitInt(19) r = v5; if (r != ((unsigned _BitInt(19))0x19609ULL)) fail |= 64; }
    { int r = v4; if (r != ((int)65535UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned _BitInt(19))0x1ULL), ((unsigned _BitInt(7))0x0ULL), ((signed char)128), ((unsigned int)0u), ((int)65535UL), ((unsigned _BitInt(19))0x19609ULL)); }
