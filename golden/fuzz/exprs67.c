int check(signed char v0, unsigned int v1, unsigned int v2, unsigned char v3, unsigned char v4, unsigned int v5) {
    int fail = 0;
    { unsigned char r = v3; if (r != ((unsigned char)0)) fail |= 1; }
    { int r = (((((int)0) ^ ((unsigned int)9029u)) == (v0 + ((unsigned char)255))) << ((signed char)((v5 <= v0)))); if (r != ((int)0)) fail |= 2; }
    { int r = (v4 >= v5); if (r != ((int)1)) fail |= 4; }
    { int r = (((v2 ^ v5) ^ (!v2)) > (((long)(v4)) && (((unsigned char)2) == v0))); if (r != ((int)1)) fail |= 8; }
    { unsigned int r = v2; if (r != ((unsigned int)32768u)) fail |= 16; }
    { int r = (((unsigned char)((v2 / v1))) + ((v2 / ((int)32768UL)) <= (((unsigned long)1322597944UL) ^ v4))); if (r != ((int)1)) fail |= 32; }
    { unsigned long r = ((unsigned long)0UL); if (r != ((unsigned long)0UL)) fail |= 64; }
    { int r = (v0 < ((unsigned long)((v5 == ((signed char)1))))); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(((signed char)255), ((unsigned int)1u), ((unsigned int)32768u), ((unsigned char)0), ((unsigned char)0), ((unsigned int)0u)); }
