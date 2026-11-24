_BitInt(37) g0 = 144;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
unsigned long f4(int p0, unsigned int p1, int * pp) {
    unsigned int l0 = ((p1) >= (((unsigned long)(p0))));
    int l1 = ((32767) / ((((l0) / ((p0) | 1))) | 1));
    long l2 = ((p0) | (g0));
    int l3 = ((((l2) | (p0))) / ((((255) * (l0))) | 1));
    *pp += (int)(((p1) > (l0)));
    l1 += rec(3, (int)((((*pp)) << ((p1) & 7))));
    return ((unsigned _BitInt(7))(((l2) && (l2))));
}
unsigned int f3(void) {
    _BitInt(37) l0 = ((signed char)(g0));
    unsigned long l1 = l0;
    unsigned char l2 = l0;
    unsigned long l3 = g0;
    l1 += rec(3, (int)(((l3) + (l0))));
    return ((((g0) != (g0))) | (((3) < (2))));
}
bool f2(unsigned _BitInt(7) p0, int p1, signed char p2, long p3) {
    unsigned int l0 = p0;
    unsigned _BitInt(7) l1 = 3;
    int l2 = ((p0) < (p0));
    l0 += rec(3, (int)(((unsigned int)(2))));
    return ((((l1) & (l1))) & (((p3) == (l1))));
}
bool f1(signed char p0, unsigned _BitInt(7) p1, unsigned long p2) {
    unsigned char l0 = ((((p1) << ((p0) & 7))) < (((p2) >> ((100) & 7))));
    l0 ^= f3();
    g0 += l0;
    if (f2(((p0) >= (255)), ((l0) << ((100) & 7)), ((l0) ^ (p2)), ((p0) && (p2)))) l0 += 3; else l0 ^= 5;
    g0 += l0;
    return ((((long)(l0))) / ((1000) | 1));
}
unsigned char f0(unsigned _BitInt(7) p0, unsigned int p1, _BitInt(37) p2, int * pp) {
    unsigned _BitInt(7) l0 = ((((p2) % ((p2) | 1))) && (((p1) - (p2))));
    unsigned _BitInt(7) l1 = ((((l0) >= (p2))) && (((int)(p1))));
    *pp += (int)(p2);
    l0 += rec(3, (int)(((p0) != (p1))));
    return p2;
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(201, 248, 40, &mbox);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
