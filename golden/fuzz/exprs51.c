unsigned char v0 = ((unsigned char)1);
unsigned char v1 = ((unsigned char)13);
long v2 = ((long)4294967295UL);
long v3 = ((long)0L);
unsigned long v4 = ((unsigned long)0UL);
int v5 = ((int)32768UL);
int check(void) {
    int fail = 0;
    { unsigned int r = (((unsigned int)(v0)) & ((v5 == v0) / (-v0))); if (r != ((unsigned int)0u)) fail |= 1; }
    { int r = (((((int)256) > ((long)2L)) - (v3 || v3)) | ((v2 + ((unsigned int)65535u)) != (((long)32767L) != ((int)14739)))); if (r != ((int)1)) fail |= 2; }
    { int r = (((v2 % ((int)256)) & (((unsigned char)127) + v5)) <= ((((int)9029) == v3) <= ((unsigned long)(((int)65535UL))))); if (r != ((int)1)) fail |= 4; }
    { int r = ((int)0); if (r != ((int)0)) fail |= 8; }
    { unsigned long r = (v4 & ((long)((((signed char)0) && ((unsigned long)0UL))))); if (r != ((unsigned long)0UL)) fail |= 16; }
    { int r = ((!(v3 * ((unsigned int)255u))) ^ ((int)((((unsigned long)32768UL) == ((unsigned char)255))))); if (r != ((int)1)) fail |= 32; }
    { int r = (((unsigned int)(((unsigned char)255))) || v1); if (r != ((int)1)) fail |= 64; }
    { long r = v3; if (r != ((long)0L)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
