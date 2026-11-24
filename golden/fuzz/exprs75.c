unsigned int v0 = ((unsigned int)65535u);
signed char v1 = ((signed char)1);
unsigned long v2 = ((unsigned long)4294967295UL);
unsigned long v3 = ((unsigned long)0UL);
int v4 = ((int)0);
int v5 = ((int)32768UL);
int check(void) {
    int fail = 0;
    { unsigned long r = (((v4 | v3) & ((int)32768UL)) / ((unsigned int)((v2 * v0)))); if (r != ((unsigned long)0UL)) fail |= 1; }
    { int r = (((v1 + v5) == (((unsigned int)0u) || v4)) < v5); if (r != ((int)0)) fail |= 2; }
    { unsigned long r = v3; if (r != ((unsigned long)0UL)) fail |= 4; }
    { int r = (v4 && ((v0 < v3) <= (v2 + v4))); if (r != ((int)0)) fail |= 8; }
    { int r = ((!(v5 < ((signed char)0))) != (((signed char)2) >= v3)); if (r != ((int)1)) fail |= 16; }
    { int r = ((((unsigned char)255) > v4) < ((((int)65535UL) - ((long)256L)) ^ (v2 <= v0))); if (r != ((int)0)) fail |= 32; }
    { unsigned int r = v0; if (r != ((unsigned int)65535u)) fail |= 64; }
    { unsigned long r = v2; if (r != ((unsigned long)4294967295UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
