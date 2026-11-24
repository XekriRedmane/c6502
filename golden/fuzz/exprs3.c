unsigned int v0 = ((unsigned int)8u);
unsigned long v1 = ((unsigned long)4294967295UL);
unsigned char v2 = ((unsigned char)255);
long v3 = ((long)3L);
_BitInt(29) v4 = ((_BitInt(29))0x1ULL);
_BitInt(29) v5 = ((_BitInt(29))0xAULL);
int check(void) {
    int fail = 0;
    { _BitInt(29) r = v4; if (r != ((_BitInt(29))0x1ULL)) fail |= 1; }
    { _BitInt(33) r = ((_BitInt(33))((!((_BitInt(33))(((long)32768L)))))); if (r != ((_BitInt(33))0x0ULL)) fail |= 2; }
    { _BitInt(29) r = v5; if (r != ((_BitInt(29))0xAULL)) fail |= 4; }
    { _BitInt(39) r = ((_BitInt(39))((v3 < ((_BitInt(33))(v0))))); if (r != ((_BitInt(39))0x1ULL)) fail |= 8; }
    { int r = ((((unsigned int)(v0)) > (v5 < v2)) >= ((v5 && ((_BitInt(39))0x7FFFFFFFULL)) + (((long)127L) < v4))); if (r != ((int)1)) fail |= 16; }
    { int r = ((v2 - (v5 + v3)) > ((unsigned long)((v4 & v2)))); if (r != ((int)1)) fail |= 32; }
    { long r = v3; if (r != ((long)3L)) fail |= 64; }
    { unsigned long r = ((unsigned long)(((((int)128) == ((unsigned int)32768u)) << (v1 & v4)))); if (r != ((unsigned long)0UL)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
