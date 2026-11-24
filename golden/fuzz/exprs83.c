int check(_BitInt(57) v0, int v1, unsigned int v2, unsigned char v3, unsigned _BitInt(7) v4, unsigned _BitInt(51) v5) {
    int fail = 0;
    { int r = ((v3 == (v2 < v4)) || ((unsigned _BitInt(7))0x7FULL)); if (r != ((int)1)) fail |= 1; }
    { _BitInt(57) r = v0; if (r != ((_BitInt(57))0x1B6A9C6D802DB89ULL)) fail |= 2; }
    { unsigned char r = ((unsigned char)(v3)); if (r != ((unsigned char)209)) fail |= 4; }
    { _BitInt(57) r = ((_BitInt(57))0xFFFFULL); if (r != ((_BitInt(57))0xFFFFULL)) fail |= 8; }
    { unsigned _BitInt(51) r = (((v4 == ((signed char)255)) & (v0 == ((unsigned char)69))) * v5); if (r != ((unsigned _BitInt(51))0x0ULL)) fail |= 16; }
    { int r = ((v3 >= (v4 >= v4)) & ((((long)7L) == v1) + (((unsigned long)7UL) > ((_BitInt(57))0x12F70B871F7F6E4ULL)))); if (r != ((int)1)) fail |= 32; }
    { int r = v1; if (r != ((int)24800)) fail |= 64; }
    { long r = ((long)((v2 & (v5 | v5)))); if (r != ((long)0L)) fail |= 128; }
    return fail;
}
int main(void) { return check(((_BitInt(57))0x1B6A9C6D802DB89ULL), ((int)24800), ((unsigned int)32768u), ((unsigned char)209), ((unsigned _BitInt(7))0x40ULL), ((unsigned _BitInt(51))0x0ULL)); }
