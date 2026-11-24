int check(unsigned int v0, unsigned char v1, unsigned int v2, unsigned long v3, signed char v4, int v5) {
    int fail = 0;
    { unsigned long r = ((unsigned long)(v1)); if (r != ((unsigned long)1UL)) fail |= 1; }
    { unsigned long r = (~((((unsigned long)255UL) % v0) * (v2 >= ((int)2)))); if (r != ((unsigned long)4294967295UL)) fail |= 2; }
    { long r = (((long)32767L) + (~(((int)7) | v2))); if (r != ((long)65527L)) fail |= 4; }
    { unsigned int r = ((unsigned int)256u); if (r != ((unsigned int)256u)) fail |= 8; }
    { signed char r = v4; if (r != ((signed char)255)) fail |= 16; }
    { unsigned int r = ((unsigned int)255u); if (r != ((unsigned int)255u)) fail |= 32; }
    { int r = v5; if (r != ((int)65535UL)) fail |= 64; }
    { int r = (((signed char)7) >> ((v5 & v0) >= ((int)(((unsigned long)2147483647UL))))); if (r != ((int)7)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned int)1u), ((unsigned char)1), ((unsigned int)32768u), ((unsigned long)14UL), ((signed char)255), ((int)65535UL)); }
