long v0 = ((long)4294967295UL);
int v1 = ((int)1);
unsigned char v2 = ((unsigned char)1);
_BitInt(34) v3 = ((_BitInt(34))0x133C57CDFULL);
long v4 = ((long)1767790497L);
unsigned _BitInt(7) v5 = ((unsigned _BitInt(7))0x7FULL);
int check(void) {
    int fail = 0;
    { int r = (v2 * ((v2 | ((int)0)) * (((signed char)255) >= ((_BitInt(34))0x8000ULL)))); if (r != ((int)0)) fail |= 1; }
    { _BitInt(34) r = ((_BitInt(34))((!v3))); if (r != ((_BitInt(34))0x0ULL)) fail |= 2; }
    { unsigned int r = ((unsigned int)255u); if (r != ((unsigned int)255u)) fail |= 4; }
    { long r = v0; if (r != ((long)4294967295UL)) fail |= 8; }
    { _BitInt(34) r = ((~((_BitInt(34))(v0))) - (((unsigned char)(v0)) / (v4 ^ ((unsigned int)0u)))); if (r != ((_BitInt(34))0x0ULL)) fail |= 16; }
    { int r = (-((v4 % v5) <= ((int)7))); if (r != ((int)0)) fail |= 32; }
    { int r = (v2 || ((v3 != v1) != (((long)65535L) && v4))); if (r != ((int)1)) fail |= 64; }
    { int r = (((v3 - v0) - v0) == ((v0 != v2) | ((unsigned char)(v4)))); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
