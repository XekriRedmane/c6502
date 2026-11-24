int check(unsigned char v0, signed char v1, signed char v2, int v3, unsigned long v4, signed char v5) {
    int fail = 0;
    { int r = (~(v4 || ((long)(((signed char)1))))); if (r != ((int)65534UL)) fail |= 1; }
    { int r = (((v4 > v1) + v1) && ((unsigned char)0)); if (r != ((int)0)) fail |= 2; }
    { unsigned long r = (((unsigned long)1852759655UL) - ((signed char)0)); if (r != ((unsigned long)1852759655UL)) fail |= 4; }
    { int r = ((((unsigned char)(v0)) > (((signed char)0) | v3)) % ((signed char)((v1 >= v4)))); if (r != ((int)0)) fail |= 8; }
    { unsigned char r = v0; if (r != ((unsigned char)128)) fail |= 16; }
    { int r = ((-v5) + v3); if (r != ((int)24120)) fail |= 32; }
    { int r = (v5 & ((((unsigned int)9029u) <= v3) != (((unsigned int)7u) != ((long)256L)))); if (r != ((int)0)) fail |= 64; }
    { int r = v3; if (r != ((int)24125)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned char)128), ((signed char)1), ((signed char)0), ((int)24125), ((unsigned long)0UL), ((signed char)5)); }
