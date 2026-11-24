int check(unsigned _BitInt(14) v0, unsigned long v1, unsigned _BitInt(14) v2, signed char v3, unsigned _BitInt(59) v4, unsigned _BitInt(59) v5) {
    int fail = 0;
    { unsigned int r = ((unsigned int)9890u); if (r != ((unsigned int)9890u)) fail |= 1; }
    { int r = (((v2 != v4) - (v3 > ((unsigned long)255UL))) - (v4 > (v4 && v1))); if (r != ((int)0)) fail |= 2; }
    { unsigned _BitInt(14) r = ((unsigned _BitInt(14))(((signed char)((v1 != v0))))); if (r != ((unsigned _BitInt(14))0x1ULL)) fail |= 4; }
    { int r = (((v2 - ((unsigned char)7)) >> (((unsigned long)128UL) * v3)) != (((unsigned int)(v5)) - ((int)127))); if (r != ((int)1)) fail |= 8; }
    { int r = (v5 > ((v1 && ((signed char)69)) < (v4 ^ v4))); if (r != ((int)1)) fail |= 16; }
    { unsigned int r = ((unsigned int)255u); if (r != ((unsigned int)255u)) fail |= 32; }
    { int r = ((v5 || (-v4)) != ((~v3) / (v4 + v0))); if (r != ((int)0)) fail |= 64; }
    { int r = (((signed char)255) >= (((unsigned _BitInt(59))(v0)) || (v4 > v2))); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned _BitInt(14))0x2000ULL), ((unsigned long)0UL), ((unsigned _BitInt(14))0x2A54ULL), ((signed char)0), ((unsigned _BitInt(59))0x400000000000000ULL), ((unsigned _BitInt(59))0x7FCD1C82335E9E2ULL)); }
