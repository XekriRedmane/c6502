long v0 = ((long)2213109988UL);
int v1 = ((int)32768UL);
unsigned char v2 = ((unsigned char)13);
int v3 = ((int)65535UL);
long v4 = ((long)4294967295UL);
unsigned char v5 = ((unsigned char)1);
int check(void) {
    int fail = 0;
    { int r = (((v0 % v3) || (~v0)) | ((v4 >= ((unsigned long)128UL)) != (v2 / v5))); if (r != ((int)1)) fail |= 1; }
    { int r = ((int)2); if (r != ((int)2)) fail |= 2; }
    { long r = ((long)256L); if (r != ((long)256L)) fail |= 4; }
    { long r = v4; if (r != ((long)4294967295UL)) fail |= 8; }
    { long r = ((long)(((v4 | v2) != v4))); if (r != ((long)0L)) fail |= 16; }
    { long r = v0; if (r != ((long)2213109988UL)) fail |= 32; }
    { unsigned long r = (((v0 != v2) + (v0 / ((unsigned long)255UL))) / (((signed char)(v5)) | (v1 > v0))); if (r != ((unsigned long)8678863UL)) fail |= 64; }
    { unsigned char r = v2; if (r != ((unsigned char)13)) fail |= 128; }
    return fail;
}
int main(void) { return check(); }
