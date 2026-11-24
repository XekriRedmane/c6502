long v0 = ((long)3925865501UL);
_BitInt(2) v1 = ((_BitInt(2))0x0ULL);
unsigned long v2 = ((unsigned long)4294967295UL);
_BitInt(2) v3 = ((_BitInt(2))0x0ULL);
unsigned long v4 = ((unsigned long)1UL);
long v5 = ((long)4294967295UL);
int check(void) {
    int fail = 0;
    { _BitInt(29) r = ((_BitInt(29))0x100ULL); if (r != ((_BitInt(29))0x100ULL)) fail |= 1; }
    { int r = (-(((int)(v5)) >> (((int)2) || v2))); if (r != ((int)1)) fail |= 2; }
    { int r = (v3 < ((v2 > v2) << (v0 <= v1))); if (r != ((int)0)) fail |= 4; }
    { int r = (((int)(v0)) ^ ((int)256)); if (r != ((int)62749UL)) fail |= 8; }
    { int r = (v4 < ((unsigned long)2UL)); if (r != ((int)1)) fail |= 16; }
    { int r = (((v1 == v0) > (((unsigned long)0UL) > v5)) != ((((signed char)128) && v2) ^ ((long)(v2)))); if (r != ((int)1)) fail |= 32; }
    { _BitInt(2) r = v3; if (r != ((_BitInt(2))0x0ULL)) fail |= 64; }
    { int r = (((((_BitInt(2))0x1ULL) - v2) <= ((unsigned char)(v0))) > (v4 >> (((_BitInt(23))0x7ULL) < v4))); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
