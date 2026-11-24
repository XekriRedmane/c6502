int check(signed char v0, signed char v1, _BitInt(41) v2, _BitInt(62) v3, unsigned _BitInt(21) v4, _BitInt(62) v5) {
    int fail = 0;
    { long r = (((!v3) && (((unsigned _BitInt(21))0x2ULL) && v2)) | ((long)65535L)); if (r != ((long)65535L)) fail |= 1; }
    { long r = ((long)(((signed char)((v0 >> ((unsigned _BitInt(21))0x0ULL)))))); if (r != ((long)1L)) fail |= 2; }
    { _BitInt(62) r = (((~v5) % (-v3)) ^ ((int)(((long)7L)))); if (r != ((_BitInt(62))0x3FFFFFFFFFFFFFF8ULL)) fail |= 4; }
    { _BitInt(62) r = ((-((_BitInt(41))0x2ULL)) + ((-v3) % (-v0))); if (r != ((_BitInt(62))0x3FFFFFFFFFFFFFFEULL)) fail |= 8; }
    { int r = ((v5 || (v5 <= v3)) << ((v5 != v5) * (-v3))); if (r != ((int)1)) fail |= 16; }
    { _BitInt(62) r = ((_BitInt(62))((v1 ^ (v0 & ((unsigned long)2147483647UL))))); if (r != ((_BitInt(62))0xFFFFFF81ULL)) fail |= 32; }
    { signed char r = v0; if (r != ((signed char)1)) fail |= 64; }
    { _BitInt(41) r = (((((int)127) + ((_BitInt(41))0x2ULL)) / v4) | ((v2 < v3) ^ (-v4))); if (r != ((_BitInt(41))0x100001ULL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((signed char)1), ((signed char)128), ((_BitInt(41))0x1FFFFFFFFFFULL), ((_BitInt(62))0xBULL), ((unsigned _BitInt(21))0x100000ULL), ((_BitInt(62))0x0ULL)); }
