int check(_BitInt(46) v0, signed char v1, signed char v2, _BitInt(45) v3, int v4, _BitInt(46) v5) {
    int fail = 0;
    { _BitInt(45) r = ((_BitInt(45))((((_BitInt(45))0x7ULL) && v1))); if (r != ((_BitInt(45))0x0ULL)) fail |= 1; }
    { long r = ((long)(((v3 >= v1) <= (v1 * v4)))); if (r != ((long)0L)) fail |= 2; }
    { signed char r = v1; if (r != ((signed char)0)) fail |= 4; }
    { int r = v4; if (r != ((int)65535UL)) fail |= 8; }
    { int r = (((signed char)128) & v4); if (r != ((int)65408UL)) fail |= 16; }
    { _BitInt(45) r = ((_BitInt(45))0x7FULL); if (r != ((_BitInt(45))0x7FULL)) fail |= 32; }
    { _BitInt(45) r = ((_BitInt(45))0x100ULL); if (r != ((_BitInt(45))0x100ULL)) fail |= 64; }
    { _BitInt(45) r = ((_BitInt(45))0x8000ULL); if (r != ((_BitInt(45))0x8000ULL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((_BitInt(46))0x5ULL), ((signed char)0), ((signed char)0), ((_BitInt(45))0x72B22B0919AULL), ((int)65535UL), ((_BitInt(46))0x6ULL)); }
