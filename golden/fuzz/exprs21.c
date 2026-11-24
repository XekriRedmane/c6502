int check(unsigned char v0, unsigned long v1, unsigned int v2, unsigned char v3, signed char v4, unsigned int v5) {
    int fail = 0;
    { unsigned int r = (((v2 / v5) == (v5 + ((signed char)207))) & ((unsigned int)2u)); if (r != ((unsigned int)0u)) fail |= 1; }
    { int r = (((long)(v5)) && ((unsigned int)((v2 > ((long)127L))))); if (r != ((int)0)) fail |= 2; }
    { int r = (((signed char)(((unsigned int)256u))) > (v4 < ((int)(v5)))); if (r != ((int)0)) fail |= 4; }
    { int r = (v1 != ((((unsigned char)104) > ((unsigned long)0UL)) >= (v1 == v0))); if (r != ((int)1)) fail |= 8; }
    { unsigned int r = v2; if (r != ((unsigned int)1u)) fail |= 16; }
    { unsigned long r = ((unsigned long)(v2)); if (r != ((unsigned long)1UL)) fail |= 32; }
    { unsigned long r = (((int)((v4 >= v1))) / ((-v3) * v1)); if (r != ((unsigned long)0UL)) fail |= 64; }
    { int r = (!((v1 || ((unsigned char)69)) <= (v5 * ((signed char)0)))); if (r != ((int)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned char)255), ((unsigned long)7UL), ((unsigned int)1u), ((unsigned char)8), ((signed char)1), ((unsigned int)2u)); }
