unsigned long v0 = ((unsigned long)0UL);
signed char v1 = ((signed char)0);
unsigned int v2 = ((unsigned int)1u);
unsigned long v3 = ((unsigned long)2147483648UL);
int v4 = ((int)32768UL);
long v5 = ((long)4294967295UL);
int check(void) {
    int fail = 0;
    { unsigned int r = ((v3 && ((unsigned long)(v4))) / v2); if (r != ((unsigned int)1u)) fail |= 1; }
    { long r = ((long)(((((signed char)69) > ((unsigned long)2433895564UL)) & (v3 * v3)))); if (r != ((long)0L)) fail |= 2; }
    { int r = v4; if (r != ((int)32768UL)) fail |= 4; }
    { unsigned long r = (v0 << ((v3 != v3) < (-v5))); if (r != ((unsigned long)0UL)) fail |= 8; }
    { unsigned int r = ((unsigned int)((v1 && ((signed char)(v3))))); if (r != ((unsigned int)0u)) fail |= 16; }
    { int r = ((((int)(((long)2503855223UL))) <= (v0 && v5)) && ((v3 == v5) > v5)); if (r != ((int)1)) fail |= 32; }
    { int r = (((unsigned long)(v2)) || (-((signed char)255))); if (r != ((int)1)) fail |= 64; }
    { unsigned long r = (((v1 >= v5) || (v3 && v5)) + ((v0 + v3) - (v4 && v2))); if (r != ((unsigned long)2147483648UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
