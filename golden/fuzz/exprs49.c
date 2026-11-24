signed char v0 = ((signed char)88);
int v1 = ((int)1);
signed char v2 = ((signed char)0);
int v3 = ((int)0);
signed char v4 = ((signed char)1);
unsigned char v5 = ((unsigned char)1);
int check(void) {
    int fail = 0;
    { int r = (((v3 || v5) * ((signed char)51)) == (((unsigned int)(((unsigned long)0UL))) << (v3 & v3))); if (r != ((int)0)) fail |= 1; }
    { int r = (v2 << ((((long)2147483648UL) + v1) == v3)); if (r != ((int)0)) fail |= 2; }
    { int r = (((v0 != v0) ^ (!v2)) << ((v1 - v3) && ((unsigned char)(v3)))); if (r != ((int)1)) fail |= 4; }
    { int r = (-((v5 < v1) < v5)); if (r != ((int)65535UL)) fail |= 8; }
    { int r = (((((unsigned char)69) + ((signed char)255)) < (v3 & v1)) != v2); if (r != ((int)0)) fail |= 16; }
    { int r = ((int)32768UL); if (r != ((int)32768UL)) fail |= 32; }
    { long r = ((long)32768L); if (r != ((long)32768L)) fail |= 64; }
    { unsigned long r = ((unsigned long)0UL); if (r != ((unsigned long)0UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
