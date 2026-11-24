int check(_BitInt(58) v0, long v1, unsigned char v2, int v3, unsigned long v4, int v5) {
    int fail = 0;
    { int r = (-((v1 >= ((unsigned char)128)) & ((int)32768UL))); if (r != ((int)0)) fail |= 1; }
    { int r = (-((v0 % ((_BitInt(42))0xFFULL)) != (v0 != v5))); if (r != ((int)0)) fail |= 2; }
    { int r = (v4 && ((v2 % v0) + (((unsigned char)1) / ((_BitInt(42))0x7FFFULL)))); if (r != ((int)0)) fail |= 4; }
    { unsigned long r = v4; if (r != ((unsigned long)266942703UL)) fail |= 8; }
    { unsigned int r = ((unsigned int)256u); if (r != ((unsigned int)256u)) fail |= 16; }
    { int r = (v5 - ((((int)256) < v4) <= v4)); if (r != ((int)8)) fail |= 32; }
    { int r = ((!(~v1)) > ((_BitInt(42))((((unsigned long)2UL) - ((_BitInt(58))0x1ULL))))); if (r != ((int)0)) fail |= 64; }
    { int r = (((((unsigned long)74565UL) | v2) == (v4 == ((_BitInt(58))0xFFULL))) << ((_BitInt(40))0x0ULL)); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(((_BitInt(58))0x1ULL), ((long)0L), ((unsigned char)255), ((int)54940UL), ((unsigned long)266942703UL), ((int)9)); }
