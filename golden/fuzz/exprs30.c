long v0 = ((long)1L);
signed char v1 = ((signed char)10);
int v2 = ((int)65535UL);
unsigned long v3 = ((unsigned long)575831485UL);
unsigned int v4 = ((unsigned int)1u);
unsigned char v5 = ((unsigned char)1);
int check(void) {
    int fail = 0;
    { int r = (((((unsigned int)7u) >= ((unsigned long)2147483647UL)) >> (v0 % v2)) >> (v2 && v5)); if (r != ((int)0)) fail |= 1; }
    { unsigned long r = v3; if (r != ((unsigned long)575831485UL)) fail |= 2; }
    { int r = ((((unsigned int)(((signed char)121))) && (v5 & ((unsigned char)2))) && ((((unsigned long)32768UL) <= v3) >> (((unsigned int)127u) & v5))); if (r != ((int)0)) fail |= 4; }
    { long r = ((long)(((long)32768L))); if (r != ((long)32768L)) fail |= 8; }
    { unsigned long r = (((v1 > ((unsigned int)0u)) + (v3 < v5)) + v3); if (r != ((unsigned long)575831486UL)) fail |= 16; }
    { long r = v0; if (r != ((long)1L)) fail |= 32; }
    { unsigned long r = (((v3 != ((unsigned long)128UL)) | (((signed char)128) | v0)) & ((unsigned long)((!((int)1397))))); if (r != ((unsigned long)0UL)) fail |= 64; }
    { int r = (((v1 % v4) < v1) >= (-v2)); if (r != ((int)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
