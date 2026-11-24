int check(int v0, unsigned long v1, _BitInt(14) v2, _BitInt(14) v3, long v4, unsigned long v5) {
    int fail = 0;
    { unsigned long r = (v4 & v5); if (r != ((unsigned long)8UL)) fail |= 1; }
    { _BitInt(14) r = v3; if (r != ((_BitInt(14))0x4ULL)) fail |= 2; }
    { long r = v4; if (r != ((long)3862178844UL)) fail |= 4; }
    { long r = ((long)2147483648UL); if (r != ((long)2147483648UL)) fail |= 8; }
    { int r = ((v1 || (v2 <= v5)) < ((~v0) % (v1 > v3))); if (r != ((int)0)) fail |= 16; }
    { long r = (v4 * ((v0 ^ v4) > v4)); if (r != ((long)3862178844UL)) fail |= 32; }
    { _BitInt(14) r = v2; if (r != ((_BitInt(14))0x2000ULL)) fail |= 64; }
    { _BitInt(14) r = v3; if (r != ((_BitInt(14))0x4ULL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((int)65535UL), ((unsigned long)4294967295UL), ((_BitInt(14))0x2000ULL), ((_BitInt(14))0x4ULL), ((long)3862178844UL), ((unsigned long)10UL)); }
