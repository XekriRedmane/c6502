signed char g0 = 58;
signed char g1 = 99;
unsigned long g2 = 16;
int f4(_BitInt(12) p0, _BitInt(12) p1, unsigned int p2, unsigned int p3) {
    unsigned _BitInt(18) l0 = ((int)(((p3) << ((p3) & 7))));
    unsigned int l1 = g0;
    unsigned char l2 = ((((32767) == (100))) >= (g1));
    return ((((g0) << ((l2) & 7))) == (((p1) || (p1))));
}
unsigned int f3(unsigned _BitInt(18) p0) {
    long l0 = ((((g1) % ((g2) | 1))) % ((g0) | 1));
    long l1 = ((((32767) >= (0))) && (((g0) <= (g2))));
    return ((((7) || (g1))) >> (((!l1)) & 7));
}
signed char f2(int p0, long p1, long p2) {
    signed char l0 = ((((g1) / ((100) | 1))) < (((p2) - (p2))));
    long l1 = ((((_BitInt(12))(g1))) == (((l0) & (0))));
    _BitInt(12) l2 = g1;
    return 32767;
}
int f1(unsigned char p0, signed char p1, unsigned long p2, long p3) {
    unsigned int l0 = (-((p1) ^ (g0)));
    signed char l1 = g1;
    long l2 = (((~1000)) - (((g1) * (p3))));
    return l0;
}
unsigned char f0(int p0, register unsigned char p1, long p2, unsigned long p3) {
    unsigned _BitInt(18) l0 = 2;
    unsigned char l1 = (~((p2) / ((255) | 1)));
    for (int it = 0; it < 3; it++) l1 += (f2(((p1) / ((p3) | 1)), ((1) + (g0)), p0)) + it;
    return p2;
}
int main(void) {
    long r = (long)f0(273, 104, 74, 264);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
