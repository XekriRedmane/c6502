int check(signed char v0, unsigned int v1, unsigned char v2, long v3, unsigned int v4, unsigned int v5) {
    int fail = 0;
    { int r = ((v2 * (~v0)) * ((unsigned char)255)); if (r != ((int)0)) fail |= 1; }
    { unsigned long r = (((((long)128L) || ((unsigned char)2)) || (v1 << v3)) | ((((unsigned long)2147483648UL) / v1) - v0)); if (r != ((unsigned long)240319UL)) fail |= 2; }
    { int r = (~((((signed char)0) | v0) && (v5 - ((unsigned long)128UL)))); if (r != ((int)65534UL)) fail |= 4; }
    { signed char r = ((signed char)(((unsigned int)(v1)))); if (r != ((signed char)232)) fail |= 8; }
    { int r = (((v4 % ((unsigned char)7)) && ((unsigned long)0UL)) > v3); if (r != ((int)0)) fail |= 16; }
    { unsigned int r = v1; if (r != ((unsigned int)8936u)) fail |= 32; }
    { int r = (((v2 == v1) | ((signed char)(((signed char)0)))) != v4); if (r != ((int)1)) fail |= 64; }
    { int r = (((v2 != ((unsigned long)0UL)) % (((unsigned char)2) >= ((signed char)128))) <= ((v1 <= ((unsigned long)2147483647UL)) && (v5 - ((unsigned char)255)))); if (r != ((int)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(((signed char)255), ((unsigned int)8936u), ((unsigned char)0), ((long)0L), ((unsigned int)6u), ((unsigned int)1u)); }
