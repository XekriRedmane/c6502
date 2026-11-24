int check(unsigned _BitInt(61) v0, unsigned long v1, _BitInt(33) v2, unsigned _BitInt(61) v3, unsigned long v4, unsigned char v5) {
    int fail = 0;
    { unsigned long r = ((unsigned long)0UL); if (r != ((unsigned long)0UL)) fail |= 1; }
    { int r = ((!(((unsigned long)32768UL) == v2)) <= ((unsigned char)(((unsigned char)(v3))))); if (r != ((int)0)) fail |= 2; }
    { int r = (-((unsigned char)((v4 * ((signed char)255))))); if (r != ((int)65281UL)) fail |= 4; }
    { int r = (!((signed char)0)); if (r != ((int)1)) fail |= 8; }
    { int r = (!((v3 & ((long)32767L)) > (v5 * v4))); if (r != ((int)1)) fail |= 16; }
    { int r = (((int)((v5 || ((unsigned long)2889005169UL)))) / ((int)((v3 + ((_BitInt(33))0x7FFFULL))))); if (r != ((int)0)) fail |= 32; }
    { int r = (((long)((((unsigned _BitInt(61))0x7FFFFFFFULL) | ((int)255)))) > ((v1 / v3) | (((signed char)0) ^ ((_BitInt(33))0x7FULL)))); if (r != ((int)1)) fail |= 64; }
    { unsigned char r = ((unsigned char)(v2)); if (r != ((unsigned char)221)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned _BitInt(61))0x0ULL), ((unsigned long)2147483648UL), ((_BitInt(33))0xF72FC1DDULL), ((unsigned _BitInt(61))0x1000000000000000ULL), ((unsigned long)1UL), ((unsigned char)1)); }
