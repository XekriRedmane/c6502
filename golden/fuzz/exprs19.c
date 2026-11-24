signed char v0 = ((signed char)1);
unsigned char v1 = ((unsigned char)255);
unsigned long v2 = ((unsigned long)2147483648UL);
int v3 = ((int)38993UL);
long v4 = ((long)2147483648UL);
unsigned char v5 = ((unsigned char)128);
int check(void) {
    int fail = 0;
    { signed char r = v0; if (r != ((signed char)1)) fail |= 1; }
    { int r = (v3 / v5); if (r != ((int)65329UL)) fail |= 2; }
    { int r = (((v3 - v5) / v3) | ((v5 % v0) / v3)); if (r != ((int)1)) fail |= 4; }
    { unsigned _BitInt(31) r = (((unsigned _BitInt(31))((v3 - v2))) | (v0 * v1)); if (r != ((unsigned _BitInt(31))0x7FFF98FFULL)) fail |= 8; }
    { unsigned int r = (((v3 / ((unsigned long)74565UL)) < (v4 * v2)) - (((unsigned int)(v0)) * (v5 && ((unsigned char)0)))); if (r != ((unsigned int)0u)) fail |= 16; }
    { int r = ((int)((((signed char)7) == v4))); if (r != ((int)0)) fail |= 32; }
    { long r = v4; if (r != ((long)2147483648UL)) fail |= 64; }
    { unsigned long r = (((unsigned _BitInt(31))(v3)) - v2); if (r != ((unsigned long)4294940753UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
