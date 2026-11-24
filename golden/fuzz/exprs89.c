unsigned char v0 = ((unsigned char)0);
long v1 = ((long)603815478L);
int v2 = ((int)32768UL);
unsigned int v3 = ((unsigned int)0u);
unsigned long v4 = ((unsigned long)9UL);
unsigned _BitInt(28) v5 = ((unsigned _BitInt(28))0x0ULL);
int check(void) {
    int fail = 0;
    { unsigned char r = v0; if (r != ((unsigned char)0)) fail |= 1; }
    { int r = (v0 > ((!v1) + v2)); if (r != ((int)1)) fail |= 2; }
    { unsigned _BitInt(33) r = ((unsigned _BitInt(33))0xFFULL); if (r != ((unsigned _BitInt(33))0xFFULL)) fail |= 4; }
    { unsigned long r = (((signed char)((~v2))) - ((unsigned long)(v3))); if (r != ((unsigned long)4294967295UL)) fail |= 8; }
    { int r = (((v4 % ((unsigned _BitInt(33))0x2ULL)) - (v4 % ((long)7L))) <= v2); if (r != ((int)0)) fail |= 16; }
    { int r = (v4 <= (((unsigned _BitInt(33))0x80000000ULL) - v3)); if (r != ((int)1)) fail |= 32; }
    { int r = (((((unsigned long)65535UL) != ((unsigned _BitInt(33))0x7FULL)) / (v1 > v4)) == (v4 > (v1 || ((int)42036UL)))); if (r != ((int)1)) fail |= 64; }
    { signed char r = ((signed char)(((signed char)(v1)))); if (r != ((signed char)54)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
