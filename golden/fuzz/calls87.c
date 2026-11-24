unsigned int g0 = 138;
unsigned char g1 = 71;
unsigned long g2 = 165;
_BitInt(11) f3(_BitInt(11) p0, unsigned int p1) {
    unsigned long l0 = ((((0) / ((g2) | 1))) + (((g1) << ((g1) & 7))));
    unsigned long l1 = ((g1) - (((3) / ((p1) | 1))));
    long l2 = p1;
    return 7;
}
unsigned char f2(signed char p0, long p1) {
    _BitInt(11) l0 = ((((p1) + (2))) >> ((((p0) > (p1))) & 7));
    unsigned int l1 = ((p0) & (100));
    _BitInt(11) (*fp)(_BitInt(11), unsigned int) = f3;
    for (int it = 0; it < 3; it++) l0 += (fp(((g0) & (g1)), ((3) << ((g1) & 7)))) + it;
    g1 += l0;
    return g1;
}
unsigned char f1(unsigned char p0, unsigned int p1, long p2) {
    unsigned char l0 = ((((p1) / ((3) | 1))) >= (((g0) + (p1))));
    return p1;
}
unsigned char f0(signed char p0, long p1) {
    _BitInt(11) l0 = ((0) >= (((p1) * (p1))));
    unsigned char l1 = ((p0) ^ (((g0) != (p1))));
    _BitInt(11) l2 = l0;
    _BitInt(11) l3 = ((((p1) <= (p1))) != (p0));
    _BitInt(11) (*fp)(_BitInt(11), unsigned int) = f3;
    l0 ^= fp(((g2) / ((g2) | 1)), ((l0) >= (p1)));
    g2 += l0;
    for (int it = 0; it < 3; it++) l3 += (f2(((p1) != (l3)), l1)) + it;
    return 1000;
}
int main(void) {
    long r = (long)f0(27, 280);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
