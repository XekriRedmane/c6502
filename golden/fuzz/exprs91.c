unsigned char v0 = ((unsigned char)0);
int v1 = ((int)65535UL);
unsigned _BitInt(19) v2 = ((unsigned _BitInt(19))0x40000ULL);
unsigned int v3 = ((unsigned int)32768u);
unsigned _BitInt(19) v4 = ((unsigned _BitInt(19))0xFULL);
unsigned _BitInt(21) v5 = ((unsigned _BitInt(21))0x1826F2ULL);
int check(void) {
    int fail = 0;
    { unsigned _BitInt(19) r = (((v2 | v4) >> (v4 && v5)) % v4); if (r != ((unsigned _BitInt(19))0x9ULL)) fail |= 1; }
    { int r = (((unsigned _BitInt(45))0x9128C20411ULL) <= ((v2 ^ v5) | v4)); if (r != ((int)0)) fail |= 2; }
    { int r = (((!v4) < (v0 == v2)) && (v2 | (-((unsigned _BitInt(21))0x12345ULL)))); if (r != ((int)0)) fail |= 4; }
    { int r = ((v0 | ((unsigned char)0)) - ((!((unsigned char)128)) > (v4 << v0))); if (r != ((int)0)) fail |= 8; }
    { unsigned int r = (((unsigned int)48456u) | (!((signed char)101))); if (r != ((unsigned int)48456u)) fail |= 16; }
    { int r = ((int)(((v0 | v4) && (~((unsigned int)65535u))))); if (r != ((int)0)) fail |= 32; }
    { unsigned _BitInt(19) r = ((unsigned _BitInt(19))0x6150DULL); if (r != ((unsigned _BitInt(19))0x6150DULL)) fail |= 64; }
    { int r = (((int)((v1 | v1))) != ((unsigned char)2)); if (r != ((int)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
