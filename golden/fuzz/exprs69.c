unsigned long v0 = ((unsigned long)1UL);
signed char v1 = ((signed char)128);
long v2 = ((long)4294967295UL);
long v3 = ((long)1L);
int v4 = ((int)65535UL);
unsigned char v5 = ((unsigned char)255);
int check(void) {
    int fail = 0;
    { signed char r = v1; if (r != ((signed char)128)) fail |= 1; }
    { unsigned char r = v5; if (r != ((unsigned char)255)) fail |= 2; }
    { int r = ((v4 + (((long)1598071274L) | v0)) != ((int)9862)); if (r != ((int)1)) fail |= 4; }
    { unsigned char r = ((unsigned char)(((v5 << v3) != v5))); if (r != ((unsigned char)1)) fail |= 8; }
    { long r = (-(v3 - (v4 <= v4))); if (r != ((long)0L)) fail |= 16; }
    { unsigned long r = v0; if (r != ((unsigned long)1UL)) fail |= 32; }
    { int r = ((int)(((v4 + v2) || v4))); if (r != ((int)1)) fail |= 64; }
    { int r = (((v5 | ((int)65535UL)) | (v3 < v1)) > (-(v3 % v2))); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
