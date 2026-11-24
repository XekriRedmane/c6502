unsigned char g0 = 153;
unsigned char g1 = 58;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
int f5(signed char p0, unsigned int p1, unsigned char p2, int p3) {
    unsigned long l0 = p0;
    unsigned int l1 = ((p0) - (((l0) != (p2))));
    unsigned char l2 = (((-1)) >= (p2));
    unsigned char l3 = ((((g0) & (g1))) < (((p0) <= (l0))));
    l3 += rec(3, (int)(((p2) <= (100))));
    return ((((p1) | (100))) >= (((l0) == (p1))));
}
unsigned long f4(unsigned long p0, signed char p1, register unsigned int p2, int p3) {
    unsigned long l0 = ((((p2) & (g1))) == (g0));
    unsigned char l1 = ((((l0) & (g1))) > (((l0) >= (3))));
    unsigned long l2 = (~((long)(p2)));
    return 255;
}
unsigned int f3(long p0) {
    long l0 = ((((p0) >> ((p0) & 7))) >= (p0));
    int l1 = ((((p0) <= (255))) || (((g0) >= (255))));
    unsigned long l2 = g1;
    unsigned int l3 = ((((1000) != (l2))) != (((l0) < (p0))));
    for (int it = 0; it < 3; it++) l2 += (f4(p0, ((l1) & (g1)), 255, g0)) + it;
    g1 += l2;
    l0 ^= f5(l0, g1, 32767, l3);
    g0 += l0;
    l1 += rec(3, (int)(l3));
    return ((((l1) & (l1))) | (((unsigned int)(l2))));
}
unsigned char f2(unsigned int p0, unsigned long p1, int * pp) {
    unsigned char l0 = (!((3) ^ (p0)));
    unsigned int l1 = ((((p1) << ((g0) & 7))) % ((((unsigned char)(g0))) | 1));
    signed char l2 = ((g1) < (((int)(l1))));
    *pp += (int)(((255) || (3)));
    l0 ^= f4(l0, ((g1) - (l2)), g0, ((unsigned char)(p0)));
    g1 += l0;
    l1 ^= f5(2, ((p0) * ((*pp))), l2, 1);
    g1 += l1;
    l0 += rec(3, (int)((-g0)));
    return ((g0) <= (((p1) >= (g1))));
}
long f1(unsigned char p0, unsigned int p1, unsigned long p2) {
    unsigned char l0 = p2;
    l0 += rec(3, (int)(p0));
    return ((((p1) << ((l0) & 7))) > (p1));
}
bool f0(long p0, unsigned long p1, unsigned long p2) {
    unsigned char l0 = (~((p0) * (p1)));
    l0 ^= f3(255);
    l0 ^= f4(p1, ((0) >= (l0)), 100, ((g1) < (7)));
    g0 += l0;
    return p2;
}
int main(void) {
    long r = (long)f0(166, 52, 138);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
