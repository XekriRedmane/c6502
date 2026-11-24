signed char g0 = 66;
unsigned _BitInt(32) g1 = 103;
unsigned _BitInt(32) f5(void) {
    long l0 = ((((g1) < (g1))) * (((g0) != (g1))));
    register unsigned char ry = (unsigned char)(l0);
    return ((((32767) && (l0))) > (((255) != (ry))));
}
signed char f4(void) {
    unsigned long l0 = ((((g1) > (g0))) || (((1) - (g0))));
    for (int it = 0; it < 3; it++) l0 += (f5()) + it;
    return ((((0) & (g0))) >= (((g0) || (0))));
}
bool f3(unsigned _BitInt(32) p0, unsigned _BitInt(32) p1, long p2, signed char p3) {
    unsigned char l0 = p2;
    unsigned _BitInt(32) l1 = p0;
    unsigned char l2 = ((1000) & (((p3) & (0))));
    for (int it = 0; it < 3; it++) l1 += (f5()) + it;
    return ((int)(((7) << ((l0) & 7))));
}
unsigned long f2(unsigned char p0, unsigned long p1, unsigned _BitInt(32) p2) {
    unsigned _BitInt(32) l0 = ((p1) >> ((((p2) % ((g1) | 1))) & 7));
    return ((p2) <= (g0));
}
unsigned char f1(long p0, int p1, long p2, unsigned char p3) {
    signed char l0 = ((((p3) & (g1))) >> ((((0) < (1000))) & 7));
    unsigned _BitInt(32) l1 = 255;
    unsigned int l2 = ((((p0) < (l0))) && (p0));
    unsigned char l3 = p3;
    return 1;
}
bool f0(unsigned int p0, int p1) {
    signed char l0 = p0;
    l0 ^= f1(g0, p0, g0, ((0) == (1000)));
    l0 ^= f4();
    g0 += l0;
    return g0;
}
int main(void) {
    long r = (long)f0(47, 173);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
