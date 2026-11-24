int v0 = ((int)58486UL);
signed char v1 = ((signed char)255);
unsigned int v2 = ((unsigned int)5u);
unsigned long v3 = ((unsigned long)4294967295UL);
signed char v4 = ((signed char)255);
signed char v5 = ((signed char)0);
int check(void) {
    int fail = 0;
    { unsigned char r = ((unsigned char)(((v2 | v4) != (((int)32768UL) * v2)))); if (r != ((unsigned char)1)) fail |= 1; }
    { unsigned long r = (((unsigned long)((((unsigned int)7u) / v0))) ^ ((-((signed char)255)) & ((signed char)(v3)))); if (r != ((unsigned long)1UL)) fail |= 2; }
    { int r = (((((int)65535UL) - v1) == v1) >= (v1 < (v0 == v0))); if (r != ((int)0)) fail |= 4; }
    { signed char r = v1; if (r != ((signed char)255)) fail |= 8; }
    { unsigned long r = (((unsigned long)((((unsigned int)127u) || v5))) & ((v0 + ((unsigned int)65535u)) < (v2 << v2))); if (r != ((unsigned long)0UL)) fail |= 16; }
    { int r = (((v0 % v2) == ((int)(v0))) ^ v0); if (r != ((int)58486UL)) fail |= 32; }
    { int r = ((((signed char)(v4)) >= (-v1)) <= (v3 > (v4 % ((int)29916)))); if (r != ((int)1)) fail |= 64; }
    { int r = (((v1 >= v3) != v3) | ((v5 <= v1) == (((long)32768L) & v4))); if (r != ((int)1)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
