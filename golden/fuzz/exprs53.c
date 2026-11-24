int check(long v0, unsigned int v1, unsigned long v2, unsigned int v3, unsigned char v4, unsigned long v5) {
    int fail = 0;
    { unsigned int r = v1; if (r != ((unsigned int)65535u)) fail |= 1; }
    { int r = (v5 > ((v1 || v5) ^ (v1 != ((unsigned int)0u)))); if (r != ((int)0)) fail |= 2; }
    { unsigned int r = (((int)(((long)(((unsigned int)255u))))) ^ ((unsigned int)((v4 | v2)))); if (r != ((unsigned int)127u)) fail |= 4; }
    { int r = (v2 <= v0); if (r != ((int)1)) fail |= 8; }
    { long r = (((((unsigned char)255) >= ((long)256L)) & ((long)2L)) & ((((signed char)62) || v5) & (v2 < v3))); if (r != ((long)0L)) fail |= 16; }
    { unsigned long r = (((unsigned long)((((unsigned long)32767UL) <= v1))) - ((v0 + v2) - ((long)(v4)))); if (r != ((unsigned long)115UL)) fail |= 32; }
    { int r = (((v4 / v4) && (((long)0L) | v3)) - ((v4 != v1) >= v5)); if (r != ((int)0)) fail |= 64; }
    { int r = ((!(v5 % ((unsigned int)255u))) || v1); if (r != ((int)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(((long)14L), ((unsigned int)65535u), ((unsigned long)0UL), ((unsigned int)47712u), ((unsigned char)128), ((unsigned long)0UL)); }
