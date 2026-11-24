unsigned int v0 = ((unsigned int)65535u);
unsigned int v1 = ((unsigned int)1u);
unsigned char v2 = ((unsigned char)1);
unsigned int v3 = ((unsigned int)9u);
signed char v4 = ((signed char)4);
signed char v5 = ((signed char)0);
int check(void) {
    int fail = 0;
    { unsigned long r = (((((unsigned long)128UL) * ((long)0L)) * ((unsigned int)(v2))) * (v0 < (v0 << v1))); if (r != ((unsigned long)0UL)) fail |= 1; }
    { unsigned long r = ((unsigned long)((((signed char)0) | (!v3)))); if (r != ((unsigned long)0UL)) fail |= 2; }
    { unsigned char r = v2; if (r != ((unsigned char)1)) fail |= 4; }
    { long r = ((long)(v0)); if (r != ((long)65535L)) fail |= 8; }
    { int r = ((v4 >= ((unsigned int)1u)) << v4); if (r != ((int)16)) fail |= 16; }
    { int r = ((int)0); if (r != ((int)0)) fail |= 32; }
    { int r = (((!v5) ^ (((unsigned char)0) == v3)) && v2); if (r != ((int)1)) fail |= 64; }
    { int r = (-((v4 + ((long)150669728L)) != (v0 * v5))); if (r != ((int)65535UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
