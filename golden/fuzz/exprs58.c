int check(long v0, unsigned long v1, signed char v2, int v3, unsigned int v4, int v5) {
    int fail = 0;
    { long r = v0; if (r != ((long)0L)) fail |= 1; }
    { int r = (((((long)128L) * v2) > (!v1)) > ((v5 - v0) >= ((signed char)255))); if (r != ((int)0)) fail |= 2; }
    { int r = ((v4 <= (v3 | v0)) <= ((signed char)2)); if (r != ((int)1)) fail |= 4; }
    { int r = ((int)(((unsigned char)((((unsigned int)0u) - ((unsigned int)36081u)))))); if (r != ((int)15)) fail |= 8; }
    { int r = v3; if (r != ((int)32768UL)) fail |= 16; }
    { long r = (((long)((v4 >= v2))) >> (((long)(((int)0))) & (v3 <= ((unsigned long)7UL)))); if (r != ((long)1L)) fail |= 32; }
    { int r = ((v4 + v1) == (((int)(((unsigned char)255))) <= (v5 + ((signed char)2)))); if (r != ((int)0)) fail |= 64; }
    { int r = (!((unsigned char)((v0 && ((signed char)0))))); if (r != ((int)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(((long)0L), ((unsigned long)15UL), ((signed char)255), ((int)32768UL), ((unsigned int)65535u), ((int)57136UL)); }
