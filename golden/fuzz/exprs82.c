int check(int v0, long v1, int v2, signed char v3, unsigned char v4, int v5) {
    int fail = 0;
    { _BitInt(52) r = (((_BitInt(52))((v5 > v4))) - ((v5 >> v2) >> v1)); if (r != ((_BitInt(52))0x1ULL)) fail |= 1; }
    { int r = v0; if (r != ((int)1)) fail |= 2; }
    { int r = (((_BitInt(60))((((unsigned _BitInt(26))0x80ULL) && v5))) > ((v3 | v2) % v4)); if (r != ((int)1)) fail |= 4; }
    { int r = (v5 <= ((!v0) == (v5 & v2))); if (r != ((int)0)) fail |= 8; }
    { int r = (((int)((v2 - v4))) >> ((((long)127L) ^ v3) % ((signed char)(((_BitInt(60))0x2ULL))))); if (r != ((int)3)) fail |= 16; }
    { _BitInt(52) r = (((((_BitInt(60))0x1ULL) + v2) && (v5 - v2)) | ((v3 | v0) - ((_BitInt(52))0x80000000ULL))); if (r != ((_BitInt(52))0xFFFFF80000001ULL)) fail |= 32; }
    { int r = v0; if (r != ((int)1)) fail |= 64; }
    { unsigned char r = ((unsigned char)(v3)); if (r != ((unsigned char)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(((int)1), ((long)13L), ((int)4), ((signed char)1), ((unsigned char)1), ((int)8144)); }
