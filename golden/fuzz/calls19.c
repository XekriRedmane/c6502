unsigned char g0 = 130;
unsigned int g1 = 100;
unsigned long g2 = 135;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
signed char f2(unsigned int p0, unsigned char p1, unsigned int p2, unsigned char p3) {
    unsigned _BitInt(56) l0 = ((((p3) / ((g0) | 1))) + (((g1) == (g0))));
    unsigned _BitInt(56) l1 = ((((g2) + (2))) == (g2));
    unsigned char l2 = ((((g0) == (l0))) == (((p3) / ((g2) | 1))));
    unsigned char l3 = ((((p1) >= (p2))) | (p1));
    return ((((int)(l3))) >> ((l3) & 7));
}
unsigned long f1(unsigned char p0, unsigned long p1) {
    unsigned long l0 = ((((unsigned char)(p1))) % ((g2) | 1));
    l0 += rec(3, (int)(((l0) && (g0))));
    return g1;
}
bool f0(void) {
    signed char l0 = ((1) < (((g1) < (g2))));
    unsigned long l1 = 100;
    unsigned char l2 = ((((1) == (g2))) - (((g2) <= (g1))));
    int l3 = l1;
    for (int it = 0; it < 3; it++) l0 += (f1(g0, ((l2) ^ (g2)))) + it;
    g1 += l0;
    signed char (*fp)(unsigned int, unsigned char, unsigned int, unsigned char) = f2;
    l0 ^= fp(2, l1, ((g0) % ((g2) | 1)), ((l0) == (0)));
    l3 += rec(3, (int)(l0));
    return ((l1) / ((g1) | 1));
}
int main(void) {
    long r = (long)f0();
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
