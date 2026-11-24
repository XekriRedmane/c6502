int v0 = ((int)1);
unsigned char v1 = ((unsigned char)128);
_BitInt(12) v2 = ((_BitInt(12))0x0ULL);
unsigned char v3 = ((unsigned char)229);
long v4 = ((long)1655358153L);
_BitInt(11) v5 = ((_BitInt(11))0x0ULL);
int check(void) {
    int fail = 0;
    { _BitInt(12) r = v2; if (r != ((_BitInt(12))0x0ULL)) fail |= 1; }
    { long r = (((((_BitInt(11))0x7FFULL) > ((_BitInt(11))0x7FFULL)) / (v4 >> v0)) << (((_BitInt(11))(v2)) >= (v4 | ((signed char)0)))); if (r != ((long)0L)) fail |= 2; }
    { unsigned char r = v1; if (r != ((unsigned char)128)) fail |= 4; }
    { long r = v4; if (r != ((long)1655358153L)) fail |= 8; }
    { int r = (((-((signed char)255)) ^ v2) | (v0 >= ((unsigned long)2UL))); if (r != ((int)1)) fail |= 16; }
    { unsigned long r = ((unsigned long)((v0 / (v0 | ((int)2))))); if (r != ((unsigned long)0UL)) fail |= 32; }
    { _BitInt(11) r = ((_BitInt(11))((!(v4 ^ v5)))); if (r != ((_BitInt(11))0x0ULL)) fail |= 64; }
    { int r = (((v2 ^ ((_BitInt(12))0x98ULL)) != (v0 | v0)) == ((v2 != v1) || v0)); if (r != ((int)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
