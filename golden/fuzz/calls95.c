signed char g0 = 35;
signed char g1 = 153;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
unsigned long f3(int p0, unsigned _BitInt(17) p1, long p2, int p3, int * pp) {
    unsigned int l0 = ((p0) && (((unsigned int)(p3))));
    _BitInt(9) l1 = ((g1) | (((g0) == (1000))));
    unsigned long l2 = 32767;
    *pp += (int)(p1);
    l0 += rec(3, (int)(((3) + (l1))));
    return ((((3) > (p0))) - (((g0) != (l0))));
}
unsigned _BitInt(17) f2(int p0, int p1, int * pp) {
    _BitInt(9) l0 = p1;
    long l1 = p1;
    unsigned long l2 = (-p0);
    unsigned _BitInt(17) l3 = ((((l2) * (p1))) % ((l2) | 1));
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(((p1) || (l3)));
    return (((((*pp)) != (l1))) + (p1));
}
unsigned int f1(void) {
    unsigned long l0 = ((((g1) % ((7) | 1))) || (g0));
    unsigned _BitInt(17) l1 = ((l0) | (((l0) >> ((g0) & 7))));
    int box2 = (int)(l0);
    l1 ^= f2(((l1) | (g0)), l1, &box2);
    l1 += box2;
    return ((((3) ^ (g1))) && (g1));
}
unsigned int f0(void) {
    unsigned char l0 = g0;
    unsigned int l1 = g0;
    long l2 = ((((l0) - (l0))) | (((g0) | (l0))));
    long l3 = g1;
    int box3 = (int)(l0);
    unsigned long (*fp)(int, unsigned _BitInt(17), long, int, int *) = f3;
    l0 ^= fp(((long)(g0)), ((l2) >= (g0)), 3, l0, &box3);
    l0 += box3;
    int box2 = (int)(l0);
    l3 ^= f2(((l1) >> ((g1) & 7)), l3, &box2);
    l3 += box2;
    g0 += l3;
    l1 += rec(3, (int)(((7) || (l2))));
    return ((((3) / ((l2) | 1))) <= (l3));
}
int main(void) {
    long r = (long)f0();
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
