int check(unsigned char v0, int v1, unsigned long v2, int v3, unsigned long v4, signed char v5) {
    int fail = 0;
    { unsigned long r = ((unsigned long)(v4)); if (r != ((unsigned long)6UL)) fail |= 1; }
    { int r = (((v1 & v3) >= (((unsigned long)256UL) < ((unsigned int)0u))) >> v1); if (r != ((int)0)) fail |= 2; }
    { unsigned char r = v0; if (r != ((unsigned char)0)) fail |= 4; }
    { int r = (((~((unsigned _BitInt(62))0x1ULL)) ^ (v2 > v5)) != (-(((int)1) + v5))); if (r != ((int)1)) fail |= 8; }
    { unsigned long r = v2; if (r != ((unsigned long)0UL)) fail |= 16; }
    { unsigned int r = ((unsigned int)128u); if (r != ((unsigned int)128u)) fail |= 32; }
    { int r = (((v1 < v0) & (v1 != ((signed char)0))) * ((v0 == ((unsigned _BitInt(62))0x80ULL)) > ((unsigned long)0UL))); if (r != ((int)0)) fail |= 64; }
    { unsigned char r = v0; if (r != ((unsigned char)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned char)0), ((int)1), ((unsigned long)0UL), ((int)0), ((unsigned long)6UL), ((signed char)12)); }
