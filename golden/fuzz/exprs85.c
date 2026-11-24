int check(unsigned char v0, int v1, unsigned char v2, long v3, long v4, unsigned char v5) {
    int fail = 0;
    { int r = (((unsigned char)((v3 ^ ((unsigned int)7u)))) & ((!v1) >= (v3 / ((unsigned char)127)))); if (r != ((int)0)) fail |= 1; }
    { int r = ((-v5) > (-(!((int)2)))); if (r != ((int)0)) fail |= 2; }
    { long r = ((~((long)128L)) * v3); if (r != ((long)129L)) fail |= 4; }
    { long r = (v4 >> ((((unsigned int)256u) ^ ((long)65535L)) != v4)); if (r != ((long)0L)) fail |= 8; }
    { unsigned char r = v2; if (r != ((unsigned char)128)) fail |= 16; }
    { unsigned int r = ((v2 | v1) & (-(v1 - ((unsigned int)1u)))); if (r != ((unsigned int)130u)) fail |= 32; }
    { unsigned char r = ((unsigned char)1); if (r != ((unsigned char)1)) fail |= 64; }
    { int r = (v0 ^ v5); if (r != ((int)178)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned char)179), ((int)15), ((unsigned char)128), ((long)4294967295UL), ((long)1L), ((unsigned char)1)); }
