int check(unsigned long v0, unsigned int v1, _BitInt(49) v2, unsigned _BitInt(18) v3, unsigned long v4, unsigned long v5) {
    int fail = 0;
    { unsigned long r = v4; if (r != ((unsigned long)4UL)) fail |= 1; }
    { unsigned long r = (v0 % v3); if (r != ((unsigned long)114760UL)) fail |= 2; }
    { unsigned long r = v5; if (r != ((unsigned long)3389672784UL)) fail |= 4; }
    { unsigned char r = ((unsigned char)69); if (r != ((unsigned char)69)) fail |= 8; }
    { _BitInt(49) r = ((_BitInt(49))(((unsigned _BitInt(18))0x2ULL))); if (r != ((_BitInt(49))0x2ULL)) fail |= 16; }
    { int r = (((unsigned int)((v3 && v3))) == v5); if (r != ((int)0)) fail |= 32; }
    { unsigned _BitInt(19) r = ((unsigned _BitInt(19))(v1)); if (r != ((unsigned _BitInt(19))0xE89BULL)) fail |= 64; }
    { long r = (~((long)2L)); if (r != ((long)4294967293UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned long)4294967295UL), ((unsigned int)59547u), ((_BitInt(49))0x1000000000000ULL), ((unsigned _BitInt(18))0x1F579ULL), ((unsigned long)4UL), ((unsigned long)3389672784UL)); }
