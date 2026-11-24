int check(int v0, int v1, unsigned int v2, signed char v3, unsigned char v4, unsigned long v5) {
    int fail = 0;
    { unsigned long r = (v5 - v4); if (r != ((unsigned long)3406462201UL)) fail |= 1; }
    { int r = ((int)(((unsigned long)((v2 % v1))))); if (r != ((int)58016UL)) fail |= 2; }
    { int r = ((~(~v3)) - v0); if (r != ((int)4)) fail |= 4; }
    { signed char r = ((signed char)0); if (r != ((signed char)0)) fail |= 8; }
    { unsigned int r = ((unsigned int)1u); if (r != ((unsigned int)1u)) fail |= 16; }
    { unsigned long r = ((unsigned long)(((int)((v5 & v0))))); if (r != ((unsigned long)9UL)) fail |= 32; }
    { int r = ((((unsigned char)(v5)) * ((unsigned int)128u)) && (v5 | (v0 != v3))); if (r != ((int)1)) fail |= 64; }
    { int r = (((unsigned int)(v4)) || ((long)((((unsigned char)7) <= ((unsigned long)32767UL))))); if (r != ((int)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(((int)9), ((int)65535UL), ((unsigned int)58016u), ((signed char)13), ((unsigned char)0), ((unsigned long)3406462201UL)); }
