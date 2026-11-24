int check(unsigned int v0, signed char v1, int v2, long v3, long v4, unsigned long v5) {
    int fail = 0;
    { int r = (((signed char)((v0 < v2))) > ((v3 ^ ((unsigned char)127)) < ((signed char)(((unsigned long)74565UL))))); if (r != ((int)1)) fail |= 1; }
    { unsigned int r = v0; if (r != ((unsigned int)0u)) fail |= 2; }
    { signed char r = v1; if (r != ((signed char)6)) fail |= 4; }
    { unsigned long r = v5; if (r != ((unsigned long)1UL)) fail |= 8; }
    { int r = ((((long)128L) && ((long)1L)) <= (((int)7) <= (v2 * ((unsigned char)0)))); if (r != ((int)0)) fail |= 16; }
    { signed char r = ((signed char)0); if (r != ((signed char)0)) fail |= 32; }
    { unsigned int r = v0; if (r != ((unsigned int)0u)) fail |= 64; }
    { int r = (v2 != (v1 != (((unsigned char)2) == v2))); if (r != ((int)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned int)0u), ((signed char)6), ((int)1906), ((long)1L), ((long)2147483648UL), ((unsigned long)1UL)); }
