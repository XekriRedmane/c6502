int check(unsigned long v0, unsigned _BitInt(34) v1, signed char v2, int v3, unsigned char v4, unsigned long v5) {
    int fail = 0;
    { unsigned long r = (((((_BitInt(47))0x7807E88B663BULL) - ((_BitInt(47))0x2ULL)) > v1) / v5); if (r != ((unsigned long)0UL)) fail |= 1; }
    { unsigned _BitInt(34) r = (((v1 << ((unsigned _BitInt(34))0x0ULL)) | (v5 * v3)) << ((v2 & v4) == ((signed char)(v0)))); if (r != ((unsigned _BitInt(34))0x2FFFF8000ULL)) fail |= 2; }
    { unsigned long r = v5; if (r != ((unsigned long)1UL)) fail |= 4; }
    { int r = ((v1 < (((int)9029) != v4)) || ((long)(v0))); if (r != ((int)0)) fail |= 8; }
    { unsigned _BitInt(34) r = ((v0 * v1) % ((v4 && v3) | (v5 ^ ((long)3906764326UL)))); if (r != ((unsigned _BitInt(34))0x0ULL)) fail |= 16; }
    { int r = v3; if (r != ((int)32768UL)) fail |= 32; }
    { int r = ((((unsigned _BitInt(34))(v0)) - ((unsigned int)255u)) || ((((unsigned int)9029u) == v1) ^ (((unsigned _BitInt(34))0x2AC377946ULL) | v0))); if (r != ((int)1)) fail |= 64; }
    { unsigned _BitInt(34) r = (((((int)7) | v4) / v4) + v1); if (r != ((unsigned _BitInt(34))0x200000001ULL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned long)0UL), ((unsigned _BitInt(34))0x200000000ULL), ((signed char)178), ((int)32768UL), ((unsigned char)128), ((unsigned long)1UL)); }
