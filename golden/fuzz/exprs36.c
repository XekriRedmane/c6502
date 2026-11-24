int check(int v0, int v1, long v2, int v3, int v4, signed char v5) {
    int fail = 0;
    { unsigned int r = (((!v1) != (v5 <= ((unsigned char)0))) | ((unsigned int)128u)); if (r != ((unsigned int)129u)) fail |= 1; }
    { int r = v1; if (r != ((int)57348UL)) fail |= 2; }
    { unsigned char r = ((unsigned char)2); if (r != ((unsigned char)2)) fail |= 4; }
    { int r = (~((v4 - v4) & (v5 <= v3))); if (r != ((int)65535UL)) fail |= 8; }
    { int r = (v3 >> ((signed char)0)); if (r != ((int)65535UL)) fail |= 16; }
    { int r = (((v2 % v2) * (v3 <= v3)) > v4); if (r != ((int)0)) fail |= 32; }
    { unsigned long r = (-((unsigned long)127UL)); if (r != ((unsigned long)4294967169UL)) fail |= 64; }
    { unsigned long r = (((v0 >= v5) & v5) & (((unsigned long)74565UL) & v5)); if (r != ((unsigned long)1UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((int)3825), ((int)57348UL), ((long)2L), ((int)65535UL), ((int)1), ((signed char)255)); }
