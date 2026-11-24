int check(unsigned _BitInt(32) v0, signed char v1, unsigned _BitInt(32) v2, unsigned _BitInt(32) v3, unsigned _BitInt(36) v4, long v5) {
    int fail = 0;
    { signed char r = ((signed char)69); if (r != ((signed char)69)) fail |= 1; }
    { unsigned _BitInt(32) r = (((v5 | v0) && (((int)32767) < v0)) | ((v2 || ((unsigned _BitInt(36))0x7FFFFFFFULL)) & (v3 >> v1))); if (r != ((unsigned _BitInt(32))0x1ULL)) fail |= 2; }
    { unsigned _BitInt(32) r = ((unsigned _BitInt(32))((v1 | (((unsigned _BitInt(36))0x80000000ULL) ^ ((long)2147483647L))))); if (r != ((unsigned _BitInt(32))0xFFFFFFFFULL)) fail |= 4; }
    { unsigned _BitInt(36) r = (((v4 < v4) >= (((unsigned char)7) < v1)) * ((unsigned _BitInt(36))(v0))); if (r != ((unsigned _BitInt(36))0xFFFFFFFFULL)) fail |= 8; }
    { unsigned _BitInt(36) r = v4; if (r != ((unsigned _BitInt(36))0x423A7711AULL)) fail |= 16; }
    { signed char r = ((signed char)(((unsigned char)0))); if (r != ((signed char)0)) fail |= 32; }
    { unsigned char r = ((unsigned char)7); if (r != ((unsigned char)7)) fail |= 64; }
    { unsigned _BitInt(32) r = ((unsigned _BitInt(32))(((v0 >= v2) / v0))); if (r != ((unsigned _BitInt(32))0x0ULL)) fail |= 128; }
    return fail;
}
int main(void) { return check(((unsigned _BitInt(32))0xFFFFFFFFULL), ((signed char)1), ((unsigned _BitInt(32))0x80000000ULL), ((unsigned _BitInt(32))0x9ULL), ((unsigned _BitInt(36))0x423A7711AULL), ((long)1L)); }
