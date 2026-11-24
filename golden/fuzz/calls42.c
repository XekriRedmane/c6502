long g0 = 62;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
signed char f2(unsigned _BitInt(43) p0) {
    signed char l0 = ((((g0) - (p0))) == (((3) << ((255) & 7))));
    unsigned long l1 = l0;
    long l2 = ((g0) < (g0));
    return ((l0) | (100));
}
signed char f1(unsigned int p0, unsigned _BitInt(43) p1, unsigned int p2, unsigned _BitInt(25) p3) {
    unsigned _BitInt(43) l0 = ((g0) && (((g0) % ((p1) | 1))));
    unsigned _BitInt(25) l1 = p0;
    int l2 = ((((long)(p3))) < (((l0) << ((g0) & 7))));
    unsigned char l3 = p0;
    l2 ^= f2(((l3) / ((l3) | 1)));
    l1 += rec(3, (int)(l3));
    return ((((p1) ^ (7))) * (1));
}
bool f0(unsigned _BitInt(43) p0, signed char p1, signed char p2, unsigned char p3, int * pp) {
    unsigned char l0 = ((g0) >= (g0));
    unsigned int l1 = ((((l0) && (p3))) + (((p0) ^ (p1))));
    unsigned char l2 = ((unsigned char)(p2));
    unsigned int l3 = p2;
    *pp += (int)(((p3) | (l1)));
    l0 += rec(3, (int)(l1));
    return ((p1) ^ (p1));
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(98, 97, 274, 229, &mbox);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
