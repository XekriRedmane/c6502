int check(unsigned int v0, unsigned long v1, unsigned int v2, signed char v3, unsigned long v4, int v5) {
    int fail = 0;
    { int r = (-((v0 >= v2) / v3)); if (r != ((int)0)) fail |= 1; }
    { int r = (((v3 / v5) != (((unsigned int)27978u) % v4)) != (v4 >= v4)); if (r != ((int)1)) fail |= 2; }
    { int r = (((v0 & v0) > ((signed char)255)) >> v1); if (r != ((int)0)) fail |= 4; }
    { unsigned long r = ((unsigned long)(((v4 < v5) <= (v3 < v0)))); if (r != ((unsigned long)1UL)) fail |= 8; }
    { unsigned int r = (((unsigned int)(v5)) >> ((((int)7) ^ v1) + (((unsigned int)7u) <= v2))); if (r != ((unsigned int)128u)) fail |= 16; }
    { unsigned int r = ((unsigned int)(v1)); if (r != ((unsigned int)0u)) fail |= 32; }
    { unsigned int r = (v0 + ((int)7)); if (r != ((unsigned int)32775u)) fail |= 64; }
    { int r = ((int)65535UL); if (r != ((int)65535UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned int)32768u), ((unsigned long)0UL), ((unsigned int)32768u), ((signed char)5), ((unsigned long)1UL), ((int)32768UL)); }
