long v0 = ((long)2147483648UL);
int v1 = ((int)28226);
unsigned long v2 = ((unsigned long)2147483648UL);
unsigned int v3 = ((unsigned int)32768u);
long v4 = ((long)87630400L);
unsigned long v5 = ((unsigned long)4294967295UL);
int check(void) {
    int fail = 0;
    { int r = (((v0 != v2) - (v2 > v5)) | ((v3 & v2) != ((unsigned long)74565UL))); if (r != ((int)1)) fail |= 1; }
    { unsigned char r = ((unsigned char)((((long)(((unsigned long)2603653819UL))) | (-v5)))); if (r != ((unsigned char)187)) fail |= 2; }
    { long r = v4; if (r != ((long)87630400L)) fail |= 4; }
    { int r = ((!((unsigned long)32767UL)) && ((v5 < ((unsigned char)69)) >> (v1 < v2))); if (r != ((int)0)) fail |= 8; }
    { int r = v1; if (r != ((int)28226)) fail |= 16; }
    { unsigned long r = ((unsigned long)(v5)); if (r != ((unsigned long)4294967295UL)) fail |= 32; }
    { long r = (~(v3 % v4)); if (r != ((long)4294934527UL)) fail |= 64; }
    { long r = v0; if (r != ((long)2147483648UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
