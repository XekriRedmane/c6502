int check(long v0, int v1, long v2, long v3, unsigned long v4, int v5) {
    int fail = 0;
    { unsigned long r = v4; if (r != ((unsigned long)4294967295UL)) fail |= 1; }
    { int r = ((int)(((((long)65535L) < v0) && (v0 ^ ((signed char)128))))); if (r != ((int)0)) fail |= 2; }
    { long r = (((((int)65535UL) * ((long)1L)) + v2) | v1); if (r != ((long)4294967295UL)) fail |= 4; }
    { int r = (((((unsigned int)0u) > v3) == ((signed char)255)) + ((((unsigned long)256UL) >> v0) <= (v2 != v4))); if (r != ((int)1)) fail |= 8; }
    { unsigned long r = v4; if (r != ((unsigned long)4294967295UL)) fail |= 16; }
    { signed char r = ((signed char)255); if (r != ((signed char)255)) fail |= 32; }
    { unsigned int r = (((unsigned int)1519u) + ((unsigned int)65535u)); if (r != ((unsigned int)1518u)) fail |= 64; }
    { int r = ((((signed char)(((unsigned int)127u))) < (v0 < v2)) % (v3 && (v1 >= v1))); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(((long)10L), ((int)1), ((long)0L), ((long)2147483648UL), ((unsigned long)4294967295UL), ((int)0)); }
