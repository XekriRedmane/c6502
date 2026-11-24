int g0 = 131;
unsigned char g1 = 113;
[[c6502::return_bool_in_carry]] bool f5(signed char p0) {
    unsigned long l0 = ((p0) >> ((((0) << ((p0) & 7))) & 7));
    unsigned int l1 = (!(!l0));
    unsigned int l2 = ((((255) != (l0))) + (((7) || (1000))));
    signed char l3 = l1;
    return ((((l3) >> ((3) & 7))) && ((!l3)));
}
bool f4(register unsigned int p0) {
    long l0 = ((((p0) << ((g0) & 7))) + (g0));
    register unsigned char ry = (unsigned char)(l0);
    l0 ^= f5(l0);
    g0 += l0;
    return ((((p0) & (p0))) && (((l0) > (1000))));
}
int f3(long p0, unsigned long p1) {
    unsigned long l0 = ((((p0) > (g1))) & ((~g1)));
    signed char l1 = p1;
    unsigned long l2 = ((0) - (((7) * (p1))));
    unsigned char l3 = ((unsigned char)(((100) + (p1))));
    if (f5(((g1) >= (l2)))) l3 += 3; else l3 ^= 5;
    if (f4(p1)) l0 += 3; else l0 ^= 5;
    return ((l0) | (((p1) >= (p1))));
}
bool f2(unsigned long p0, unsigned char p1, unsigned long p2, signed char p3, int * pp) {
    int l0 = ((((p2) || (p0))) << ((((p1) % ((p1) | 1))) & 7));
    *pp += (int)(p3);
    return p3;
}
bool f1(long p0, unsigned long p1, signed char p2) {
    unsigned char l0 = ((((unsigned int)(1000))) >> ((((p0) + (p2))) & 7));
    unsigned char l1 = ((((p0) % ((g0) | 1))) / ((7) | 1));
    unsigned long l2 = ((g1) > ((-g1)));
    long l3 = ((((p0) != (l2))) * (255));
    return g1;
}
[[c6502::return_bool_in_carry]] bool f0(unsigned char p0, register signed char p1, long p2) {
    signed char l0 = ((((g0) / ((g0) | 1))) >= (((p1) <= (p1))));
    long l1 = (!((g0) && (p2)));
    unsigned char l2 = ((g1) | ((-p2)));
    unsigned char l3 = g1;
    return ((((l1) >> ((l2) & 7))) == (1));
}
int main(void) {
    long r = (long)f0(4, 258, 100);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
