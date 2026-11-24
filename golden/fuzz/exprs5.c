int check(long v0, unsigned long v1, unsigned long v2, unsigned int v3, unsigned long v4, signed char v5) {
    int fail = 0;
    { unsigned int r = ((unsigned int)(((unsigned int)2u))); if (r != ((unsigned int)2u)) fail |= 1; }
    { unsigned char r = ((unsigned char)0); if (r != ((unsigned char)0)) fail |= 2; }
    { int r = (!((unsigned long)((v1 || v1)))); if (r != ((int)1)) fail |= 4; }
    { unsigned long r = ((unsigned long)845589051UL); if (r != ((unsigned long)845589051UL)) fail |= 8; }
    { int r = ((int)(((((long)32768L) * v2) - (v2 & v3)))); if (r != ((int)0)) fail |= 16; }
    { long r = ((long)((((int)32768UL) > (((long)7L) == v1)))); if (r != ((long)0L)) fail |= 32; }
    { long r = (-((long)255L)); if (r != ((long)4294967041UL)) fail |= 64; }
    { unsigned long r = v2; if (r != ((unsigned long)3961355680UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((long)2147483648UL), ((unsigned long)0UL), ((unsigned long)3961355680UL), ((unsigned int)15u), ((unsigned long)14UL), ((signed char)7)); }
