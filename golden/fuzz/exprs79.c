int check(unsigned char v0, unsigned char v1, unsigned long v2, int v3, unsigned long v4, unsigned char v5) {
    int fail = 0;
    { unsigned int r = ((((unsigned int)(((int)9029))) << (v5 <= v1)) + ((v4 > v4) < (~v5))); if (r != ((unsigned int)18058u)) fail |= 1; }
    { int r = (((long)32767L) || ((v3 + v0) ^ (v3 > v1))); if (r != ((int)1)) fail |= 2; }
    { unsigned long r = ((((unsigned char)(v4)) / (v4 | v5)) / ((v5 & v1) + v3)); if (r != ((unsigned long)0UL)) fail |= 4; }
    { signed char r = ((signed char)(((v3 && ((int)127)) && (v2 ^ ((unsigned long)2UL))))); if (r != ((signed char)1)) fail |= 8; }
    { unsigned int r = (((v3 * ((unsigned int)32767u)) >> (v3 || v3)) >> (-v1)); if (r != ((unsigned int)31794u)) fail |= 16; }
    { int r = (((unsigned long)((v2 | v0))) <= (v5 ^ (v2 >= v0))); if (r != ((int)0)) fail |= 32; }
    { unsigned char r = v1; if (r != ((unsigned char)0)) fail |= 64; }
    { int r = (((v0 != v5) & (v4 < ((unsigned long)7UL))) + ((((int)256) || ((int)17344)) < ((long)(v1)))); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned char)1), ((unsigned char)0), ((unsigned long)2123726655UL), ((int)1948), ((unsigned long)2147483648UL), ((unsigned char)0)); }
