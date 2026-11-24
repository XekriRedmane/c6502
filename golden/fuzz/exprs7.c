int check(int v0, unsigned long v1, signed char v2, long v3, unsigned int v4, signed char v5) {
    int fail = 0;
    { int r = (!v0); if (r != ((int)0)) fail |= 1; }
    { int r = ((int)(v1)); if (r != ((int)1)) fail |= 2; }
    { int r = ((((int)(v1)) | ((long)32768L)) >= ((((int)7) * ((unsigned long)7UL)) && (((unsigned long)127UL) / v0))); if (r != ((int)1)) fail |= 4; }
    { long r = v3; if (r != ((long)922121676L)) fail |= 8; }
    { int r = (v2 >> ((v0 * ((unsigned int)65535u)) == ((unsigned long)65535UL))); if (r != ((int)93)) fail |= 16; }
    { unsigned long r = (((v1 <= ((unsigned char)7)) >= (v3 % v2)) % ((((unsigned char)1) ^ v1) + ((unsigned char)127))); if (r != ((unsigned long)0UL)) fail |= 32; }
    { unsigned long r = (((v5 || ((unsigned int)0u)) | (v5 * v1)) + ((int)(((signed char)(v4))))); if (r != ((unsigned long)4294967182UL)) fail |= 64; }
    { long r = v3; if (r != ((long)922121676L)) fail |= 128; }
    return fail;
}
int main(void) { return check(((int)65535UL), ((unsigned long)1UL), ((signed char)93), ((long)922121676L), ((unsigned int)1u), ((signed char)141)); }
