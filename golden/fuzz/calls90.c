unsigned char g0 = 123;
long g1 = 162;
unsigned long g2 = 115;
int f3(void) {
    unsigned char l0 = g2;
    int l1 = ((((g0) << ((g2) & 7))) || (((g2) != (l0))));
    signed char l2 = ((((g1) | (7))) % ((((g0) || (l1))) | 1));
    unsigned int l3 = ((((g2) > (g0))) + (l2));
    return (((!l3)) + (((l1) <= (g2))));
}
_BitInt(25) f2(_BitInt(18) p0, register unsigned char p1, int * pp) {
    unsigned char l0 = ((g0) & (((g1) <= (p0))));
    _BitInt(18) l1 = ((((l0) >= (7))) / ((((g1) % ((g2) | 1))) | 1));
    _BitInt(18) l2 = ((((l1) <= (100))) ^ (((7) - (g2))));
    int l3 = p1;
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)((~l0));
    return (((-p0)) >= (((100) - (l3))));
}
[[c6502::return_bool_in_carry]] bool f1(void) {
    unsigned int l0 = ((((g1) % ((g2) | 1))) != (((_BitInt(25))(g0))));
    _BitInt(25) l1 = ((((g0) << ((g1) & 7))) > ((~l0)));
    unsigned int l2 = ((((3) << ((g0) & 7))) >= (1));
    return ((7) > (l0));
}
[[c6502::return_bool_in_carry]] bool f0(_BitInt(18) p0, long p1) {
    int l0 = ((((p0) <= (0))) * (((g1) + (g2))));
    register unsigned char ry = (unsigned char)(l0);
    int box2 = (int)(l0);
    l0 ^= f2(((ry) << ((p1) & 7)), g0, &box2);
    l0 += box2;
    g1 += l0;
    for (int it = 0; it < 3; it++) l0 += (f3()) + it;
    g2 += l0;
    return ((unsigned char)(((2) ^ (l0))));
}
int main(void) {
    long r = (long)f0(210, 29);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
