long v0 = ((long)1L);
unsigned int v1 = ((unsigned int)0u);
int v2 = ((int)8);
unsigned char v3 = ((unsigned char)128);
unsigned long v4 = ((unsigned long)737409258UL);
signed char v5 = ((signed char)1);
int check(void) {
    int fail = 0;
    { long r = ((long)2L); if (r != ((long)2L)) fail |= 1; }
    { long r = v0; if (r != ((long)1L)) fail |= 2; }
    { unsigned long r = (v0 + ((v4 <= ((unsigned char)2)) / (v0 * v4))); if (r != ((unsigned long)1UL)) fail |= 4; }
    { int r = v2; if (r != ((int)8)) fail |= 8; }
    { int r = (((v2 >= v5) + (v3 > ((signed char)69))) << ((v3 >= ((int)32768UL)) ^ (v0 > ((long)2147483648UL)))); if (r != ((int)2)) fail |= 16; }
    { int r = ((int)(v3)); if (r != ((int)128)) fail |= 32; }
    { unsigned int r = v1; if (r != ((unsigned int)0u)) fail |= 64; }
    { unsigned char r = v3; if (r != ((unsigned char)128)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
