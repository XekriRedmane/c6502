int check(long v0, int v1, unsigned long v2, int v3, int v4, unsigned long v5) {
    int fail = 0;
    { int r = v1; if (r != ((int)0)) fail |= 1; }
    { unsigned long r = ((v5 / (((unsigned long)7UL) > v2)) << ((long)1L)); if (r != ((unsigned long)2UL)) fail |= 2; }
    { unsigned long r = (-(-((unsigned long)0UL))); if (r != ((unsigned long)0UL)) fail |= 4; }
    { unsigned long r = v5; if (r != ((unsigned long)1UL)) fail |= 8; }
    { unsigned long r = ((((signed char)255) == (v3 == v0)) + (v2 & (v5 != v3))); if (r != ((unsigned long)0UL)) fail |= 16; }
    { int r = v4; if (r != ((int)12)) fail |= 32; }
    { int r = (!((((unsigned int)256u) != v3) <= (v5 & ((int)65535UL)))); if (r != ((int)0)) fail |= 64; }
    { int r = (((v0 & v0) > ((signed char)69)) ^ (((long)(v2)) != ((long)1L))); if (r != ((int)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(((long)4294967295UL), ((int)0), ((unsigned long)0UL), ((int)20931), ((int)12), ((unsigned long)1UL)); }
