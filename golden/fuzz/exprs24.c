unsigned long v0 = ((unsigned long)1UL);
unsigned char v1 = ((unsigned char)15);
unsigned char v2 = ((unsigned char)0);
signed char v3 = ((signed char)12);
unsigned long v4 = ((unsigned long)1217071532UL);
unsigned int v5 = ((unsigned int)0u);
int check(void) {
    int fail = 0;
    { int r = ((int)(((v0 & v2) < (v5 | v0)))); if (r != ((int)1)) fail |= 1; }
    { unsigned int r = ((unsigned int)(v1)); if (r != ((unsigned int)15u)) fail |= 2; }
    { int r = (((v5 >> v1) > (((int)2) ^ ((unsigned int)0u))) == ((unsigned long)256UL)); if (r != ((int)0)) fail |= 4; }
    { unsigned int r = ((unsigned int)(v3)); if (r != ((unsigned int)12u)) fail |= 8; }
    { int r = ((v2 + ((signed char)0)) <= (v3 + (v3 < v2))); if (r != ((int)1)) fail |= 16; }
    { int r = ((!v5) - v1); if (r != ((int)65522UL)) fail |= 32; }
    { unsigned char r = v1; if (r != ((unsigned char)15)) fail |= 64; }
    { int r = (((long)((v1 * v4))) == ((v3 >= v3) % (((unsigned char)1) & v1))); if (r != ((int)0)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
