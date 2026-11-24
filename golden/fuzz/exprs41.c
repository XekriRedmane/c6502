unsigned _BitInt(42) v0 = ((unsigned _BitInt(42))0xFULL);
unsigned _BitInt(42) v1 = ((unsigned _BitInt(42))0x20000000000ULL);
unsigned _BitInt(30) v2 = ((unsigned _BitInt(30))0x0ULL);
signed char v3 = ((signed char)0);
signed char v4 = ((signed char)15);
int v5 = ((int)32768UL);
int check(void) {
    int fail = 0;
    { signed char r = v4; if (r != ((signed char)15)) fail |= 1; }
    { unsigned int r = ((unsigned int)(((v2 + ((int)65535UL)) < ((int)127)))); if (r != ((unsigned int)0u)) fail |= 2; }
    { int r = (((signed char)255) + v3); if (r != ((int)65535UL)) fail |= 4; }
    { int r = (((unsigned _BitInt(42))(v5)) >= ((v2 - v0) && (((unsigned char)255) * ((unsigned _BitInt(42))0x12345ULL)))); if (r != ((int)1)) fail |= 8; }
    { unsigned int r = ((unsigned int)(((((_BitInt(26))0xFFFFULL) ^ v1) * (v4 == v2)))); if (r != ((unsigned int)0u)) fail |= 16; }
    { unsigned _BitInt(42) r = (v1 % v4); if (r != ((unsigned _BitInt(42))0x2ULL)) fail |= 32; }
    { int r = (!((((unsigned int)65535u) << v2) && ((int)(v1)))); if (r != ((int)1)) fail |= 64; }
    { int r = (-(v4 ^ (v2 <= v2))); if (r != ((int)65522UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
