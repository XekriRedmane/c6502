int check(long v0, _BitInt(32) v1, _BitInt(33) v2, _BitInt(33) v3, _BitInt(44) v4, _BitInt(33) v5) {
    int fail = 0;
    { _BitInt(33) r = v5; if (r != ((_BitInt(33))0x701CD368ULL)) fail |= 1; }
    { int r = (((((_BitInt(33))0x1ABD23827ULL) % v1) >= (((long)128L) >= v5)) >= (((_BitInt(44))0x2ULL) || (v5 | v2))); if (r != ((int)0)) fail |= 2; }
    { _BitInt(33) r = v5; if (r != ((_BitInt(33))0x701CD368ULL)) fail |= 4; }
    { int r = (!((((unsigned int)65535u) | ((_BitInt(33))0xFFFFULL)) - v1)); if (r != ((int)0)) fail |= 8; }
    { int r = ((int)((((unsigned char)69) || (v1 & v0)))); if (r != ((int)1)) fail |= 16; }
    { _BitInt(33) r = (-((v1 - v2) >> (((unsigned int)32767u) > v2))); if (r != ((_BitInt(33))0x40000001ULL)) fail |= 32; }
    { long r = v0; if (r != ((long)0L)) fail |= 64; }
    { _BitInt(44) r = (((long)32767L) ^ v4); if (r != ((_BitInt(44))0x7FFEULL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((long)0L), ((_BitInt(32))0x80000000ULL), ((_BitInt(33))0x2ULL), ((_BitInt(33))0x100000000ULL), ((_BitInt(44))0x1ULL), ((_BitInt(33))0x701CD368ULL)); }
