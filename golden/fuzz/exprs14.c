int check(unsigned char v0, unsigned _BitInt(22) v1, long v2, unsigned _BitInt(32) v3, _BitInt(5) v4, long v5) {
    int fail = 0;
    { int r = ((v5 + ((unsigned long)(v4))) <= v3); if (r != ((int)0)) fail |= 1; }
    { unsigned _BitInt(32) r = (((unsigned _BitInt(32))((v5 / v1))) / (~(((unsigned _BitInt(22))0x1ULL) << v0))); if (r != ((unsigned _BitInt(32))0x400ULL)) fail |= 2; }
    { unsigned char r = ((unsigned char)255); if (r != ((unsigned char)255)) fail |= 4; }
    { unsigned _BitInt(32) r = ((~(v3 << v0)) >> ((v3 <= ((unsigned _BitInt(32))0x80000000ULL)) > (v4 << v3))); if (r != ((unsigned _BitInt(32))0xFFFFFFFFULL)) fail |= 8; }
    { unsigned int r = ((unsigned int)7u); if (r != ((unsigned int)7u)) fail |= 16; }
    { int r = (v5 >= ((v4 | ((int)65535UL)) ^ (((unsigned _BitInt(22))0x3FFFFFULL) - ((unsigned _BitInt(32))0x8000ULL)))); if (r != ((int)0)) fail |= 32; }
    { unsigned _BitInt(22) r = ((unsigned _BitInt(22))((!((unsigned long)4272093908UL)))); if (r != ((unsigned _BitInt(22))0x0ULL)) fail |= 64; }
    { unsigned _BitInt(22) r = ((((unsigned _BitInt(22))(v5)) - (v0 >> ((int)0))) % v4); if (r != ((unsigned _BitInt(22))0x0ULL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned char)0), ((unsigned _BitInt(22))0x200000ULL), ((long)1249904198L), ((unsigned _BitInt(32))0x0ULL), ((_BitInt(5))0xCULL), ((long)2147483648UL)); }
