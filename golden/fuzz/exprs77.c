int v0 = ((int)3);
unsigned char v1 = ((unsigned char)210);
int v2 = ((int)1);
long v3 = ((long)1L);
unsigned long v4 = ((unsigned long)2839491241UL);
int v5 = ((int)65535UL);
int check(void) {
    int fail = 0;
    { long r = ((long)2147483648UL); if (r != ((long)2147483648UL)) fail |= 1; }
    { int r = (v2 * (v1 < (v4 | v5))); if (r != ((int)1)) fail |= 2; }
    { int r = v0; if (r != ((int)3)) fail |= 4; }
    { unsigned long r = (((((int)32767) && v3) % v3) % (((unsigned long)(v0)) / (v4 != ((unsigned long)0UL)))); if (r != ((unsigned long)0UL)) fail |= 8; }
    { int r = (!(v0 <= ((unsigned long)2147483647UL))); if (r != ((int)0)) fail |= 16; }
    { int r = ((((long)(v2)) % (v4 + v0)) && v0); if (r != ((int)1)) fail |= 32; }
    { long r = v3; if (r != ((long)1L)) fail |= 64; }
    { unsigned int r = ((unsigned int)256u); if (r != ((unsigned int)256u)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
