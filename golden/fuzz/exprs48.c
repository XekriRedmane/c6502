long v0 = ((long)1L);
unsigned long v1 = ((unsigned long)1UL);
unsigned char v2 = ((unsigned char)255);
unsigned char v3 = ((unsigned char)0);
signed char v4 = ((signed char)10);
unsigned char v5 = ((unsigned char)1);
int check(void) {
    int fail = 0;
    { long r = (-v0); if (r != ((long)4294967295UL)) fail |= 1; }
    { int r = (~v2); if (r != ((int)65280UL)) fail |= 2; }
    { long r = ((long)(v0)); if (r != ((long)1L)) fail |= 4; }
    { int r = ((((long)0L) % (v3 ^ v0)) != ((v0 < ((unsigned int)27513u)) <= v5)); if (r != ((int)1)) fail |= 8; }
    { unsigned long r = ((unsigned long)128UL); if (r != ((unsigned long)128UL)) fail |= 16; }
    { int r = ((int)(((v3 | ((signed char)2)) ^ ((unsigned char)(((unsigned long)74565UL)))))); if (r != ((int)71)) fail |= 32; }
    { unsigned int r = ((unsigned int)127u); if (r != ((unsigned int)127u)) fail |= 64; }
    { long r = (((((unsigned int)0u) | ((unsigned long)1UL)) >= (((signed char)7) % v1)) % ((long)(((unsigned long)(((signed char)127)))))); if (r != ((long)1L)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
