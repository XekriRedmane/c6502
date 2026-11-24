unsigned int v0 = ((unsigned int)0u);
unsigned _BitInt(38) v1 = ((unsigned _BitInt(38))0x2000000000ULL);
signed char v2 = ((signed char)174);
unsigned _BitInt(38) v3 = ((unsigned _BitInt(38))0x1ULL);
long v4 = ((long)0L);
unsigned long v5 = ((unsigned long)10UL);
int check(void) {
    int fail = 0;
    { int r = (((long)((v0 * v0))) && ((v2 <= v5) || v2)); if (r != ((int)0)) fail |= 1; }
    { unsigned _BitInt(38) r = v3; if (r != ((unsigned _BitInt(38))0x1ULL)) fail |= 2; }
    { int r = (((_BitInt(5))0x0ULL) - ((v1 ^ v1) && (v1 < v4))); if (r != ((int)0)) fail |= 4; }
    { unsigned _BitInt(31) r = ((unsigned _BitInt(31))((((unsigned int)(v0)) < ((long)(v1))))); if (r != ((unsigned _BitInt(31))0x0ULL)) fail |= 8; }
    { unsigned int r = (v2 ^ ((-((unsigned int)32768u)) - (v5 > ((signed char)1)))); if (r != ((unsigned int)32849u)) fail |= 16; }
    { long r = ((long)((((unsigned int)(v5)) % v2))); if (r != ((long)10L)) fail |= 32; }
    { int r = (!v0); if (r != ((int)1)) fail |= 64; }
    { long r = v4; if (r != ((long)0L)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
