int check(_BitInt(16) v0, signed char v1, unsigned int v2, long v3, signed char v4, int v5) {
    int fail = 0;
    { _BitInt(36) r = ((-((_BitInt(36))(((int)32767)))) >> ((long)((v5 && v1)))); if (r != ((_BitInt(36))0xFFFFF8001ULL)) fail |= 1; }
    { int r = (((v5 >> v1) ^ (v2 - v1)) >= ((unsigned int)255u)); if (r != ((int)1)) fail |= 2; }
    { _BitInt(16) r = ((_BitInt(16))((((int)(v0)) >= (v5 < v1)))); if (r != ((_BitInt(16))0x1ULL)) fail |= 4; }
    { long r = ((long)256L); if (r != ((long)256L)) fail |= 8; }
    { _BitInt(16) r = ((_BitInt(16))0x43B6ULL); if (r != ((_BitInt(16))0x43B6ULL)) fail |= 16; }
    { int r = (!((v0 ^ ((long)0L)) == (v4 - ((unsigned _BitInt(26))0x1ULL)))); if (r != ((int)1)) fail |= 32; }
    { _BitInt(36) r = ((_BitInt(36))0x1ULL); if (r != ((_BitInt(36))0x1ULL)) fail |= 64; }
    { int r = (((((unsigned _BitInt(26))0x7ULL) * v0) >> ((_BitInt(16))(v5))) != ((~v3) > ((unsigned _BitInt(26))(v0)))); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(((_BitInt(16))0x1ULL), ((signed char)0), ((unsigned int)32768u), ((long)4294967295UL), ((signed char)0), ((int)10)); }
