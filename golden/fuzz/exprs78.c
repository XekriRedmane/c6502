unsigned char v0 = ((unsigned char)0);
unsigned long v1 = ((unsigned long)0UL);
unsigned int v2 = ((unsigned int)0u);
unsigned char v3 = ((unsigned char)255);
unsigned long v4 = ((unsigned long)0UL);
unsigned int v5 = ((unsigned int)1u);
int check(void) {
    int fail = 0;
    { unsigned long r = ((unsigned long)128UL); if (r != ((unsigned long)128UL)) fail |= 1; }
    { int r = ((((int)0) == v1) >= ((((unsigned char)255) <= v5) ^ (v4 > v3))); if (r != ((int)1)) fail |= 2; }
    { int r = (((unsigned char)0) >= ((v4 <= ((unsigned long)7UL)) == v0)); if (r != ((int)1)) fail |= 4; }
    { unsigned int r = ((unsigned int)(v4)); if (r != ((unsigned int)0u)) fail |= 8; }
    { int r = (~v3); if (r != ((int)65280UL)) fail |= 16; }
    { unsigned char r = v0; if (r != ((unsigned char)0)) fail |= 32; }
    { unsigned int r = v2; if (r != ((unsigned int)0u)) fail |= 64; }
    { unsigned int r = v2; if (r != ((unsigned int)0u)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
