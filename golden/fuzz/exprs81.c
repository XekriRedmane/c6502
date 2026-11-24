int check(long v0, long v1, signed char v2, unsigned long v3, signed char v4, unsigned long v5) {
    int fail = 0;
    { int r = (((long)2L) > ((((unsigned char)0) == v5) >= ((unsigned long)3141981803UL))); if (r != ((int)1)) fail |= 1; }
    { unsigned long r = (((~((unsigned long)2147483648UL)) / (-((unsigned int)7u))) + ((unsigned int)0u)); if (r != ((unsigned long)32771UL)) fail |= 2; }
    { int r = (!((unsigned int)0u)); if (r != ((int)1)) fail |= 4; }
    { int r = (((v5 / ((unsigned int)7u)) | ((unsigned long)74565UL)) && ((v4 | ((long)128L)) - (((unsigned int)127u) % v4))); if (r != ((int)1)) fail |= 8; }
    { unsigned char r = ((unsigned char)127); if (r != ((unsigned char)127)) fail |= 16; }
    { long r = ((long)2L); if (r != ((long)2L)) fail |= 32; }
    { int r = (!((unsigned int)128u)); if (r != ((int)0)) fail |= 64; }
    { int r = (((v3 >= ((int)1)) | v2) - ((!((unsigned long)74565UL)) != (v3 & v5))); if (r != ((int)65535UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((long)2147483648UL), ((long)4294967295UL), ((signed char)255), ((unsigned long)1UL), ((signed char)128), ((unsigned long)2147483648UL)); }
