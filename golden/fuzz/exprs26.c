unsigned long v0 = ((unsigned long)0UL);
unsigned char v1 = ((unsigned char)1);
signed char v2 = ((signed char)255);
signed char v3 = ((signed char)1);
long v4 = ((long)4294967295UL);
unsigned char v5 = ((unsigned char)255);
int check(void) {
    int fail = 0;
    { int r = (((v1 > v3) >> v0) && ((v5 >= v2) >= (v2 <= v4))); if (r != ((int)0)) fail |= 1; }
    { unsigned char r = v1; if (r != ((unsigned char)1)) fail |= 2; }
    { unsigned int r = (v2 % ((v3 * ((unsigned int)256u)) * v1)); if (r != ((unsigned int)255u)) fail |= 4; }
    { int r = (((v3 <= v3) - (v0 != v4)) / (v3 & ((unsigned char)127))); if (r != ((int)0)) fail |= 8; }
    { signed char r = ((signed char)255); if (r != ((signed char)255)) fail |= 16; }
    { int r = (v3 && v0); if (r != ((int)0)) fail |= 32; }
    { int r = (v0 || ((signed char)0)); if (r != ((int)0)) fail |= 64; }
    { long r = v4; if (r != ((long)4294967295UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
