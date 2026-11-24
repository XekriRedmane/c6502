long g0 = 196;
_BitInt(48) g1 = 181;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
int f2(signed char p0, int p1, _BitInt(48) p2) {
    unsigned char l0 = ((g1) && (((p0) + (p0))));
    unsigned char l1 = ((((g0) - (l0))) >> ((((l0) | (p0))) & 7));
    return p0;
}
unsigned int f1(void) {
    signed char l0 = ((((g1) ^ (g1))) % ((((g1) >= (g0))) | 1));
    int l1 = ((((g0) != (l0))) << ((g0) & 7));
    int (*fp)(signed char, int, _BitInt(48)) = f2;
    l0 ^= fp(((g0) ^ (255)), l0, g0);
    l0 += rec(3, (int)(l0));
    return 1;
}
unsigned char f0(long p0, unsigned int p1, int * pp) {
    unsigned char l0 = 32767;
    long l1 = ((((32767) ^ (g1))) >> ((((g1) <= (100))) & 7));
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(((100) || (p1)));
    return ((((l0) - (g1))) / (((!l1)) | 1));
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(130, 26, &mbox);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
