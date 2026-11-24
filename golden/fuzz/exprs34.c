int check(_BitInt(51) v0, signed char v1, unsigned char v2, unsigned long v3, int v4, signed char v5) {
    int fail = 0;
    { unsigned char r = v2; if (r != ((unsigned char)128)) fail |= 1; }
    { int r = ((-((unsigned long)2147483648UL)) >= ((v3 & v1) >> ((_BitInt(51))(v0)))); if (r != ((int)1)) fail |= 2; }
    { int r = (((v5 * v3) < v3) < v3); if (r != ((int)1)) fail |= 4; }
    { int r = (((long)32767L) || (!v0)); if (r != ((int)1)) fail |= 8; }
    { unsigned long r = (~((v5 % v3) % v1)); if (r != ((unsigned long)4294967295UL)) fail |= 16; }
    { int r = (v5 >> (v0 < (((long)32767L) < ((unsigned long)1UL)))); if (r != ((int)1)) fail |= 32; }
    { int r = (((long)((((signed char)255) == v0))) == ((unsigned char)((v3 < v1)))); if (r != ((int)0)) fail |= 64; }
    { int r = (((unsigned long)((v0 ^ v2))) > ((v2 && v2) ^ (v1 % v5))); if (r != ((int)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(((_BitInt(51))0x0ULL), ((signed char)128), ((unsigned char)128), ((unsigned long)1UL), ((int)32768UL), ((signed char)1)); }
