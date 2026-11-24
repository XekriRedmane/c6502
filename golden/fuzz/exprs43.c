int check(signed char v0, _BitInt(3) v1, unsigned char v2, signed char v3, unsigned long v4, unsigned _BitInt(23) v5) {
    int fail = 0;
    { unsigned long r = (((int)((((_BitInt(42))0x2ULL) - v0))) * ((v0 == ((signed char)0)) % (v4 & v3))); if (r != ((unsigned long)0UL)) fail |= 1; }
    { int r = ((int)((v4 > (((unsigned long)255UL) * v3)))); if (r != ((int)0)) fail |= 2; }
    { int r = (((v4 / v0) || (v5 / v2)) <= ((((unsigned _BitInt(23))0x12345ULL) | v5) < (v4 >= v4))); if (r != ((int)0)) fail |= 4; }
    { int r = (~((signed char)7)); if (r != ((int)65528UL)) fail |= 8; }
    { _BitInt(3) r = v1; if (r != ((_BitInt(3))0x0ULL)) fail |= 16; }
    { _BitInt(42) r = (((_BitInt(42))(((_BitInt(42))(v4)))) / ((v3 / v0) >= (~v1))); if (r != ((_BitInt(42))0x1ULL)) fail |= 32; }
    { unsigned char r = v2; if (r != ((unsigned char)255)) fail |= 64; }
    { int r = ((~v0) == ((v2 + v1) != (v2 >> v3))); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(((signed char)11), ((_BitInt(3))0x0ULL), ((unsigned char)255), ((signed char)1), ((unsigned long)1UL), ((unsigned _BitInt(23))0x326C79ULL)); }
