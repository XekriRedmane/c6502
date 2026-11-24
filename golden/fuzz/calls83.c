unsigned char g0 = 32;
unsigned _BitInt(7) g1 = 8;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
unsigned long f5(_BitInt(57) p0, unsigned long p1, _BitInt(57) p2, int * pp) {
    unsigned _BitInt(7) l0 = ((unsigned long)(7));
    *pp += (int)(((p2) || (p1)));
    l0 += rec(3, (int)(((g0) / ((l0) | 1))));
    return ((p1) * (l0));
}
_BitInt(57) f4(unsigned _BitInt(7) p0, int p1) {
    signed char l0 = (-((g0) * (p1)));
    unsigned char l1 = ((((g0) / ((g1) | 1))) >= (((unsigned _BitInt(7))(l0))));
    unsigned char l2 = ((((p1) == (p1))) == (((100) == (g0))));
    unsigned long l3 = ((unsigned long)(l2));
    int box5 = (int)(l0);
    l1 ^= f5(((l0) > (g0)), (!l0), 100, &box5);
    l1 += box5;
    g0 += l1;
    l1 += rec(3, (int)(((32767) | (g0))));
    return g0;
}
signed char f3(void) {
    long l0 = ((((3) == (0))) + (1));
    int l1 = 0;
    unsigned _BitInt(7) l2 = l1;
    unsigned _BitInt(7) l3 = ((((g1) >= (l2))) == (l1));
    int box5 = (int)(l0);
    l0 ^= f5(((l3) - (g0)), l2, g1, &box5);
    l0 += box5;
    return l3;
}
unsigned int f2(unsigned int p0, _BitInt(57) p1, int p2, int * pp) {
    int l0 = g1;
    unsigned char l1 = ((((p1) / ((1) | 1))) * (((32767) || (g0))));
    unsigned long l2 = ((g1) == (((p0) < (p2))));
    int l3 = g1;
    *pp += (int)(l2);
    int box5 = (int)(l0);
    l1 ^= f5((*pp), ((g0) ^ (p1)), g1, &box5);
    l1 += box5;
    g0 += l1;
    l3 ^= f3();
    l1 += rec(3, (int)(((long)(l1))));
    return l2;
}
bool f1(int p0, unsigned long p1, long p2) {
    unsigned char l0 = ((((g1) <= (p0))) || (((g1) >> ((g1) & 7))));
    signed char l1 = ((((p2) >= (p2))) != (((g0) || (g1))));
    long l2 = ((l0) <= (((g1) / ((p0) | 1))));
    int box5 = (int)(l0);
    l0 ^= f5((~l2), l2, (-1000), &box5);
    l0 += box5;
    g0 += l0;
    int box2 = (int)(l0);
    for (int it = 0; it < 3; it++) l0 += (f2(((p1) << ((3) & 7)), ((l0) <= (l0)), ((3) < (l2)), &box2)) + it;
    l0 += box2;
    g1 += l0;
    l1 += rec(3, (int)(((p2) || (l2))));
    return ((((g0) * (l1))) > (((p0) >= (p0))));
}
unsigned int f0(long p0, int * pp) {
    unsigned _BitInt(7) l0 = ((((g1) >= (p0))) / ((((unsigned long)(g1))) | 1));
    int l1 = ((((g1) || (g0))) && (((l0) + (1000))));
    _BitInt(57) l2 = ((l0) == (((p0) / ((l1) | 1))));
    _BitInt(57) l3 = 7;
    *pp += (int)(l2);
    return ((unsigned int)(g0));
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(257, &mbox);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
