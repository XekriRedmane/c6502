signed char g0 = 90;
_BitInt(44) g1 = 15;
unsigned _BitInt(56) g2 = 72;
[[c6502::return_bool_in_carry]] bool f4(register int p0, unsigned int p1) {
    int l0 = ((((g2) || (p0))) << ((p1) & 7));
    register unsigned char ry = (unsigned char)(l0);
    return ((unsigned long)(((g0) << ((g1) & 7))));
}
unsigned _BitInt(56) f3(register int p0) {
    unsigned long l0 = ((unsigned char)(((255) >= (3))));
    return ((((g0) == (1000))) >= (g2));
}
[[c6502::return_bool_in_carry]] bool f2(long p0, unsigned long p1, signed char p2) {
    _BitInt(44) l0 = 0;
    int l1 = ((g0) && (((g1) != (p2))));
    unsigned long l2 = ((((g0) < (p0))) & (((signed char)(p1))));
    unsigned long l3 = ((p0) == (g0));
    l3 ^= f3(((p0) % ((p2) | 1)));
    return ((l0) < (((l1) % ((p0) | 1))));
}
unsigned long f1(void) {
    unsigned long l0 = 0;
    _BitInt(44) l1 = ((((l0) / ((g2) | 1))) % (((!g0)) | 1));
    unsigned long l2 = l1;
    unsigned int l3 = ((l0) >= (l1));
    return ((g0) >= (g0));
}
unsigned char f0(register unsigned int p0, _BitInt(44) p1, signed char p2) {
    long l0 = g0;
    unsigned int l1 = g1;
    unsigned long l2 = ((((p0) - (g2))) * (l1));
    int l3 = ((((l2) < (g0))) <= (((unsigned long)(l0))));
    l3 ^= f2(((3) - (l1)), ((g0) % ((p0) | 1)), ((3) | (l3)));
    l0 ^= f1();
    g1 += l0;
    return ((((3) << ((p2) & 7))) == (((g1) && (3))));
}
int main(void) {
    long r = (long)f0(264, 32, 280);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
