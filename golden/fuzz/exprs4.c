int check(unsigned char v0, unsigned int v1, signed char v2, int v3, unsigned char v4, int v5) {
    int fail = 0;
    { int r = (v2 ^ (!(((int)0) <= v5))); if (r != ((int)0)) fail |= 1; }
    { unsigned char r = v0; if (r != ((unsigned char)0)) fail |= 2; }
    { int r = ((v5 == (v3 >= v2)) * v0); if (r != ((int)0)) fail |= 4; }
    { signed char r = v2; if (r != ((signed char)1)) fail |= 8; }
    { int r = ((-((int)(((unsigned long)256UL)))) > ((((signed char)0) ^ ((int)32768UL)) - ((int)0))); if (r != ((int)1)) fail |= 16; }
    { int r = ((((unsigned long)(v1)) >> (v0 == v1)) < v1); if (r != ((int)0)) fail |= 32; }
    { unsigned int r = (((v3 / ((unsigned int)7u)) % v1) | ((v4 | v3) >= (v0 ^ v3))); if (r != ((unsigned int)7495u)) fail |= 64; }
    { long r = ((long)((v2 / v2))); if (r != ((long)1L)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned char)0), ((unsigned int)31383u), ((signed char)1), ((int)52458UL), ((unsigned char)128), ((int)51101UL)); }
