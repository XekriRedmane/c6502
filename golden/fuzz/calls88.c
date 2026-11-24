unsigned long g0 = 47;
[[c6502::return_bool_in_carry]] bool f5(_BitInt(45) p0, register signed char p1, _BitInt(45) p2, _BitInt(46) p3) {
    unsigned int l0 = ((p2) < (((p1) && (p3))));
    _BitInt(46) l1 = ((((100) ^ (p2))) % ((((p2) >> ((p0) & 7))) | 1));
    signed char l2 = ((((p0) % ((p3) | 1))) != (((int)(32767))));
    return ((((1000) - (p0))) && ((~p2)));
}
unsigned int f4(unsigned int p0, unsigned int p1, int p2) {
    unsigned int l0 = ((((255) <= (g0))) | (((100) >> ((p1) & 7))));
    unsigned long l1 = ((((l0) + (3))) == ((!p0)));
    _BitInt(46) l2 = ((((l1) >> ((p1) & 7))) && (l1));
    signed char l3 = p1;
    for (int it = 0; it < 3; it++) l0 += (f5(((l0) <= (3)), ((g0) <= (p1)), p2, ((l2) >= (l3)))) + it;
    g0 += l0;
    return ((((1) | (l3))) + (((g0) <= (p2))));
}
unsigned long f3(int * pp) {
    unsigned int l0 = (!g0);
    long l1 = ((((l0) >> ((l0) & 7))) > (((l0) == (l0))));
    int l2 = ((l1) & (((g0) > (l0))));
    _BitInt(45) l3 = l1;
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(((l2) > (l1)));
    for (int it = 0; it < 3; it++) l3 += (f5(g0, l3, ((l3) ^ (ry)), l2)) + it;
    g0 += l3;
    l1 ^= f4(l2, (!3), (*pp));
    g0 += l1;
    return ((((255) < (l3))) >= (((ry) && (g0))));
}
unsigned long f2(register int p0, long p1, int p2, int * pp) {
    unsigned long l0 = p2;
    _BitInt(46) l1 = ((p2) & (((l0) << ((g0) & 7))));
    long l2 = ((((g0) ^ (p1))) == (((100) ^ (p0))));
    signed char l3 = ((((p1) / ((p0) | 1))) % ((p2) | 1));
    *pp += (int)(p0);
    return ((((p0) || (l0))) && ((*pp)));
}
unsigned char f1(register int p0) {
    long l0 = ((((g0) || (p0))) || (((p0) < (g0))));
    int box3 = (int)(l0);
    l0 ^= f3(&box3);
    l0 += box3;
    g0 += l0;
    return ((g0) << ((((32767) || (g0))) & 7));
}
signed char f0(_BitInt(45) p0, int p1, int * pp) {
    unsigned int l0 = ((g0) && ((-p0)));
    *pp += (int)(p1);
    int box3 = (int)(l0);
    l0 ^= f3(&box3);
    l0 += box3;
    g0 += l0;
    return ((((1) == ((*pp)))) < (((p1) != (p0))));
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(140, 174, &mbox);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
