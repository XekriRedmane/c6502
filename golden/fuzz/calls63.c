long g0 = 64;
unsigned _BitInt(55) g1 = 168;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
[[c6502::return_bool_in_carry]] bool f5(unsigned long p0, unsigned char p1, register signed char p2, int p3) {
    int l0 = p3;
    int l1 = (~((p3) <= (l0)));
    long l2 = 0;
    register unsigned char ry = (unsigned char)(l0);
    return ((l0) > (((unsigned _BitInt(55))(ry))));
}
unsigned long f4(unsigned int p0, register unsigned char p1, signed char p2, int * pp) {
    long l0 = 32767;
    unsigned char l1 = ((((g1) - (g0))) <= (((g0) == (l0))));
    unsigned char l2 = g1;
    unsigned long l3 = 255;
    *pp += (int)(((g0) << ((0) & 7)));
    if (f5(((p0) <= (p1)), (-l0), 0, l1)) l1 += 3; else l1 ^= 5;
    return (((((*pp)) == ((*pp)))) && (p0));
}
bool f3(unsigned _BitInt(51) p0) {
    long l0 = ((unsigned _BitInt(55))(((p0) != (p0))));
    unsigned _BitInt(55) l1 = ((((g0) * (g1))) >= ((!g0)));
    unsigned _BitInt(51) l2 = 100;
    l2 += rec(3, (int)((!l0)));
    return (!((l1) / ((g1) | 1)));
}
signed char f2(void) {
    unsigned long l0 = ((((g0) && (g1))) >= (g1));
    unsigned _BitInt(51) l1 = ((unsigned _BitInt(51))(((g1) || (g0))));
    unsigned char l2 = g0;
    register unsigned char ry = (unsigned char)(l0);
    l0 ^= f5(((unsigned _BitInt(51))(l0)), ((l0) / ((ry) | 1)), ((g0) >= (g1)), ((0) >= (ry)));
    int box4 = (int)(l0);
    for (int it = 0; it < 3; it++) l2 += (f4(l1, (~2), ((l1) && (g0)), &box4)) + it;
    l2 += box4;
    return l1;
}
bool f1(signed char p0, unsigned int p1, int p2, unsigned long p3, int * pp) {
    unsigned _BitInt(51) l0 = p1;
    signed char l1 = ((g1) >= (((2) / ((p1) | 1))));
    *pp += (int)(((p2) < (7)));
    if (f5(((3) << ((g0) & 7)), ((g0) == (p2)), ((p3) || (p3)), (*pp))) l0 += 3; else l0 ^= 5;
    l1 += rec(3, (int)(((l0) / ((g1) | 1))));
    return ((p3) | (p2));
}
unsigned char f0(long p0, int * pp) {
    unsigned int l0 = ((((g0) <= (p0))) >= (((g0) & (g0))));
    signed char l1 = g1;
    long l2 = ((((p0) - (g1))) || (((l1) ^ (p0))));
    *pp += (int)(((g0) != (g1)));
    return ((((g0) <= (g0))) << ((((g1) < (0))) & 7));
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(170, &mbox);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
