int check(unsigned char v0, unsigned int v1, unsigned int v2, long v3, unsigned long v4, long v5) {
    int fail = 0;
    { long r = ((((int)127) ^ (v5 + v2)) ^ (~(v0 & ((unsigned int)32768u)))); if (r != ((long)1945033688L)) fail |= 1; }
    { long r = v5; if (r != ((long)1944953944L)) fail |= 2; }
    { int r = (((v0 == ((unsigned int)0u)) && (((signed char)0) ^ v3)) < (!(v2 / v0))); if (r != ((int)0)) fail |= 4; }
    { signed char r = ((signed char)7); if (r != ((signed char)7)) fail |= 8; }
    { int r = (!((~v2) && (-v4))); if (r != ((int)0)) fail |= 16; }
    { unsigned int r = v1; if (r != ((unsigned int)65535u)) fail |= 32; }
    { unsigned long r = ((unsigned long)(v4)); if (r != ((unsigned long)4294967295UL)) fail |= 64; }
    { long r = v3; if (r != ((long)6L)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned char)128), ((unsigned int)65535u), ((unsigned int)32768u), ((long)6L), ((unsigned long)4294967295UL), ((long)1944953944L)); }
