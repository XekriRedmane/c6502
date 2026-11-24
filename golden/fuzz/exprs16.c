int v0 = ((int)32768UL);
unsigned int v1 = ((unsigned int)14853u);
unsigned int v2 = ((unsigned int)0u);
int v3 = ((int)10);
signed char v4 = ((signed char)128);
unsigned long v5 = ((unsigned long)0UL);
int check(void) {
    int fail = 0;
    { unsigned char r = ((unsigned char)(v0)); if (r != ((unsigned char)0)) fail |= 1; }
    { int r = (v3 * (~(v3 / v3))); if (r != ((int)65516UL)) fail |= 2; }
    { int r = ((int)(v3)); if (r != ((int)10)) fail |= 4; }
    { unsigned long r = v5; if (r != ((unsigned long)0UL)) fail |= 8; }
    { unsigned int r = (v1 << (v3 >= (((unsigned int)127u) | v0))); if (r != ((unsigned int)14853u)) fail |= 16; }
    { int r = (v3 | ((-v1) >= (v1 & v3))); if (r != ((int)11)) fail |= 32; }
    { int r = (((((int)256) != v5) != v1) < (((unsigned int)(v0)) && v0)); if (r != ((int)0)) fail |= 64; }
    { unsigned long r = ((v2 & v5) >> v2); if (r != ((unsigned long)0UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
