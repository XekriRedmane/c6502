unsigned char v0 = ((unsigned char)145);
int v1 = ((int)65535UL);
unsigned int v2 = ((unsigned int)6u);
signed char v3 = ((signed char)255);
unsigned int v4 = ((unsigned int)39809u);
unsigned long v5 = ((unsigned long)0UL);
int check(void) {
    int fail = 0;
    { int r = (v0 | v3); if (r != ((int)65535UL)) fail |= 1; }
    { int r = ((v5 && (!v5)) > ((v4 <= ((unsigned int)32767u)) & ((unsigned int)(v3)))); if (r != ((int)0)) fail |= 2; }
    { int r = (((((long)255L) + v5) && (((long)32768L) & v1)) || v1); if (r != ((int)1)) fail |= 4; }
    { long r = (((((unsigned long)32768UL) && ((signed char)255)) | (v3 || ((unsigned int)128u))) * ((v4 < v4) & ((long)(v1)))); if (r != ((long)0L)) fail |= 8; }
    { int r = (-((v0 >= v2) + (v5 >= v1))); if (r != ((int)65535UL)) fail |= 16; }
    { unsigned long r = v5; if (r != ((unsigned long)0UL)) fail |= 32; }
    { unsigned int r = v2; if (r != ((unsigned int)6u)) fail |= 64; }
    { unsigned int r = v2; if (r != ((unsigned int)6u)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
