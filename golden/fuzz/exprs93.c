int check(unsigned int v0, int v1, int v2, signed char v3, signed char v4, unsigned long v5) {
    int fail = 0;
    { int r = (!((unsigned char)7)); if (r != ((int)0)) fail |= 1; }
    { int r = (~(((signed char)0) * ((signed char)1))); if (r != ((int)65535UL)) fail |= 2; }
    { int r = (((unsigned int)(v0)) || ((int)((v4 && v1)))); if (r != ((int)1)) fail |= 4; }
    { int r = (v1 < ((unsigned int)((v2 & v5)))); if (r != ((int)0)) fail |= 8; }
    { unsigned long r = ((unsigned long)((~(v3 % ((unsigned int)7u))))); if (r != ((unsigned long)65534UL)) fail |= 16; }
    { int r = (v3 / ((-v3) - (((unsigned char)0) / v2))); if (r != ((int)65535UL)) fail |= 32; }
    { unsigned long r = (v1 % ((v3 > ((signed char)251)) * ((unsigned long)65535UL))); if (r != ((unsigned long)57877UL)) fail |= 64; }
    { long r = ((long)2147483648UL); if (r != ((long)2147483648UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned int)0u), ((int)57877UL), ((int)65535UL), ((signed char)1), ((signed char)158), ((unsigned long)2147483648UL)); }
