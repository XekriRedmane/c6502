int check(int v0, unsigned int v1, long v2, signed char v3, signed char v4, int v5) {
    int fail = 0;
    { long r = ((long)(((v3 > ((signed char)128)) > (((unsigned char)0) / ((signed char)128))))); if (r != ((long)1L)) fail |= 1; }
    { int r = (((v1 || v2) || v1) < (v4 != (v0 && v5))); if (r != ((int)0)) fail |= 2; }
    { unsigned int r = (v5 % v1); if (r != ((unsigned int)32767u)) fail |= 4; }
    { long r = ((long)32767L); if (r != ((long)32767L)) fail |= 8; }
    { int r = (v0 || (((unsigned int)65535u) % (v4 & v0))); if (r != ((int)1)) fail |= 16; }
    { int r = (v4 > v3); if (r != ((int)0)) fail |= 32; }
    { signed char r = v3; if (r != ((signed char)0)) fail |= 64; }
    { unsigned int r = ((unsigned int)((v0 == (v1 >> v3)))); if (r != ((unsigned int)0u)) fail |= 128; }
    return fail;
}
int main(void) { return check(((int)1), ((unsigned int)32768u), ((long)1L), ((signed char)0), ((signed char)245), ((int)65535UL)); }
