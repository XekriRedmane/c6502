int check(unsigned long v0, unsigned long v1, long v2, unsigned int v3, unsigned long v4, unsigned int v5) {
    int fail = 0;
    { int r = ((v2 && (v4 - v5)) && (v4 > ((int)(v0)))); if (r != ((int)1)) fail |= 1; }
    { unsigned long r = ((unsigned long)58003336UL); if (r != ((unsigned long)58003336UL)) fail |= 2; }
    { long r = ((long)((v0 * (-v0)))); if (r != ((long)4294967295UL)) fail |= 4; }
    { unsigned long r = v1; if (r != ((unsigned long)0UL)) fail |= 8; }
    { int r = (v2 >= v1); if (r != ((int)1)) fail |= 16; }
    { int r = (((((int)1) == ((unsigned int)1u)) != (v4 - ((int)128))) < v5); if (r != ((int)1)) fail |= 32; }
    { unsigned long r = v4; if (r != ((unsigned long)2147483648UL)) fail |= 64; }
    { int r = ((int)65535UL); if (r != ((int)65535UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned long)1UL), ((unsigned long)0UL), ((long)2147483648UL), ((unsigned int)34604u), ((unsigned long)2147483648UL), ((unsigned int)65535u)); }
