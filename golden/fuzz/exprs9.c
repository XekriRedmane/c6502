unsigned int v0 = ((unsigned int)32768u);
unsigned char v1 = ((unsigned char)0);
signed char v2 = ((signed char)255);
long v3 = ((long)1L);
long v4 = ((long)0L);
unsigned int v5 = ((unsigned int)65535u);
int check(void) {
    int fail = 0;
    { unsigned int r = (v1 + ((v5 ^ ((int)65535UL)) | (v2 - ((unsigned int)128u)))); if (r != ((unsigned int)65407u)) fail |= 1; }
    { signed char r = ((signed char)((v0 < ((unsigned char)(((unsigned char)0)))))); if (r != ((signed char)0)) fail |= 2; }
    { int r = (!((v0 * v5) << (-((unsigned long)0UL)))); if (r != ((int)0)) fail |= 4; }
    { int r = ((((unsigned char)(((unsigned int)62319u))) > v5) >= ((((signed char)221) | v3) + (v0 < v4))); if (r != ((int)1)) fail |= 8; }
    { signed char r = v2; if (r != ((signed char)255)) fail |= 16; }
    { long r = ((long)(((unsigned char)(((unsigned long)(((unsigned int)128u))))))); if (r != ((long)128L)) fail |= 32; }
    { int r = ((int)(v3)); if (r != ((int)1)) fail |= 64; }
    { int r = ((!(~v4)) || v1); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
