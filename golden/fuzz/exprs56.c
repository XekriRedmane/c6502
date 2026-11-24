int check(unsigned _BitInt(13) v0, unsigned _BitInt(13) v1, _BitInt(48) v2, unsigned char v3, unsigned int v4, unsigned int v5) {
    int fail = 0;
    { unsigned int r = (v1 | v4); if (r != ((unsigned int)1u)) fail |= 1; }
    { int r = (v0 && (v4 + (v5 + v2))); if (r != ((int)0)) fail |= 2; }
    { int r = (((!v3) == ((int)65535UL)) << v1); if (r != ((int)0)) fail |= 4; }
    { int r = (((unsigned _BitInt(13))0x1FFFULL) || ((unsigned _BitInt(60))((v3 && v0)))); if (r != ((int)1)) fail |= 8; }
    { unsigned int r = (((unsigned int)(((_BitInt(48))0x7ULL))) - ((v1 >= v5) ^ (v1 <= ((unsigned _BitInt(13))0xC0EULL)))); if (r != ((unsigned int)7u)) fail |= 16; }
    { _BitInt(48) r = (((v3 > v4) >= (!v0)) ^ v2); if (r != ((_BitInt(48))0x800000000001ULL)) fail |= 32; }
    { _BitInt(48) r = (v2 & ((((signed char)0) + ((unsigned int)0u)) >= (v4 > v1))); if (r != ((_BitInt(48))0x0ULL)) fail |= 64; }
    { unsigned _BitInt(60) r = (((unsigned _BitInt(60))0x1B90699BAD27C0ULL) + (((unsigned _BitInt(60))0xB29135D7F2E8959ULL) > v0)); if (r != ((unsigned _BitInt(60))0x1B90699BAD27C1ULL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned _BitInt(13))0x0ULL), ((unsigned _BitInt(13))0x1ULL), ((_BitInt(48))0x800000000000ULL), ((unsigned char)128), ((unsigned int)1u), ((unsigned int)1u)); }
