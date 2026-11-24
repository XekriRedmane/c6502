int check(signed char v0, long v1, unsigned long v2, long v3, signed char v4, unsigned long v5) {
    int fail = 0;
    { long r = ((long)74565L); if (r != ((long)74565L)) fail |= 1; }
    { unsigned char r = ((unsigned char)((v0 << (v3 == v4)))); if (r != ((unsigned char)1)) fail |= 2; }
    { signed char r = v0; if (r != ((signed char)1)) fail |= 4; }
    { int r = (((((long)32768L) * ((unsigned long)128UL)) || (v1 & v5)) < v5); if (r != ((int)1)) fail |= 8; }
    { unsigned long r = v5; if (r != ((unsigned long)3543743247UL)) fail |= 16; }
    { int r = (((((unsigned char)7) * v0) <= (v3 != v2)) < (~(v0 / v1))); if (r != ((int)0)) fail |= 32; }
    { long r = (((v3 - v4) <= ((int)59798UL)) + ((long)((v0 >= ((int)65535UL))))); if (r != ((long)2L)) fail |= 64; }
    { unsigned long r = ((unsigned long)((((signed char)(((unsigned char)0))) >= (((signed char)7) + ((signed char)128))))); if (r != ((unsigned long)1UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((signed char)1), ((long)14L), ((unsigned long)1UL), ((long)4068190969UL), ((signed char)128), ((unsigned long)3543743247UL)); }
