int v0 = ((int)32768UL);
signed char v1 = ((signed char)77);
signed char v2 = ((signed char)123);
int v3 = ((int)32768UL);
unsigned long v4 = ((unsigned long)1UL);
signed char v5 = ((signed char)0);
int check(void) {
    int fail = 0;
    { unsigned long r = ((unsigned long)1UL); if (r != ((unsigned long)1UL)) fail |= 1; }
    { int r = (~v2); if (r != ((int)65412UL)) fail |= 2; }
    { long r = ((long)(((v3 >= ((unsigned int)7u)) && (v4 - v3)))); if (r != ((long)1L)) fail |= 4; }
    { unsigned int r = ((unsigned int)((((long)(v3)) == v5))); if (r != ((unsigned int)0u)) fail |= 8; }
    { unsigned long r = ((v0 >= v5) | v4); if (r != ((unsigned long)1UL)) fail |= 16; }
    { long r = ((((unsigned int)(((unsigned long)74565UL))) % (((signed char)128) || ((int)0))) % ((v4 >= v5) + (((long)2L) * v1))); if (r != ((long)0L)) fail |= 32; }
    { int r = (((~v2) - (v0 || v0)) > ((unsigned int)((v5 + ((int)127))))); if (r != ((int)1)) fail |= 64; }
    { signed char r = v1; if (r != ((signed char)77)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
