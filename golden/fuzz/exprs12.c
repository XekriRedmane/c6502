int check(unsigned int v0, unsigned long v1, signed char v2, int v3, unsigned long v4, long v5) {
    int fail = 0;
    { unsigned long r = v1; if (r != ((unsigned long)4294967295UL)) fail |= 1; }
    { int r = ((-(v0 && v0)) >= ((unsigned int)65535u)); if (r != ((int)1)) fail |= 2; }
    { unsigned int r = ((v2 ^ (((unsigned char)255) + ((unsigned int)32767u))) & ((unsigned char)(v4))); if (r != ((unsigned int)1u)) fail |= 4; }
    { int r = v3; if (r != ((int)65535UL)) fail |= 8; }
    { int r = (((unsigned char)((v3 ^ v3))) ^ (((unsigned char)0) & v2)); if (r != ((int)0)) fail |= 16; }
    { int r = (v2 == (v5 << (v4 > v3))); if (r != ((int)0)) fail |= 32; }
    { int r = ((int)(v4)); if (r != ((int)9)) fail |= 64; }
    { int r = (((((int)0) * v4) + (v2 & ((unsigned long)255UL))) || (~v0)); if (r != ((int)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned int)1u), ((unsigned long)4294967295UL), ((signed char)255), ((int)65535UL), ((unsigned long)9UL), ((long)1L)); }
