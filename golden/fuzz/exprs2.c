signed char v0 = ((signed char)128);
int v1 = ((int)0);
long v2 = ((long)9L);
signed char v3 = ((signed char)10);
_BitInt(17) v4 = ((_BitInt(17))0x0ULL);
_BitInt(14) v5 = ((_BitInt(14))0x1ULL);
int check(void) {
    int fail = 0;
    { int r = (((((_BitInt(17))0x1C898ULL) & v4) & v2) || v4); if (r != ((int)0)) fail |= 1; }
    { _BitInt(17) r = ((_BitInt(17))0x7FFFULL); if (r != ((_BitInt(17))0x7FFFULL)) fail |= 2; }
    { unsigned long r = (((v4 | v2) / (v0 | ((unsigned long)0UL))) ^ v2); if (r != ((unsigned long)9UL)) fail |= 4; }
    { int r = ((-(((signed char)108) > ((signed char)128))) * v0); if (r != ((int)128)) fail |= 8; }
    { signed char r = ((signed char)127); if (r != ((signed char)127)) fail |= 16; }
    { int r = ((int)9029); if (r != ((int)9029)) fail |= 32; }
    { int r = v1; if (r != ((int)0)) fail |= 64; }
    { long r = v2; if (r != ((long)9L)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
