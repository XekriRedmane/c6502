long g0 = 73;
unsigned int g1 = 18;
unsigned char g2 = 65;
signed char f5(unsigned char p0, int p1, unsigned _BitInt(29) p2, int p3) {
    int l0 = ((p1) / ((((p2) < (p3))) | 1));
    long l1 = p1;
    long l2 = ((g0) || (l1));
    return ((p3) - (p1));
}
[[c6502::return_bool_in_carry]] bool f4(int p0, unsigned char p1, unsigned _BitInt(33) p2, long p3) {
    unsigned _BitInt(33) l0 = ((100) * (((p1) <= (100))));
    unsigned long l1 = ((int)(p3));
    unsigned _BitInt(29) l2 = ((g0) || (g0));
    unsigned _BitInt(29) l3 = 7;
    signed char (*fp)(unsigned char, int, unsigned _BitInt(29), int) = f5;
    l3 ^= fp(((100) ^ (0)), l2, ((p0) * (g2)), ((p0) - (l0)));
    g2 += l3;
    return l1;
}
unsigned _BitInt(29) f3(int p0, unsigned long p1) {
    unsigned long l0 = ((((p1) <= (g0))) % ((((signed char)(g2))) | 1));
    signed char l1 = ((((p1) != (g2))) != (((g2) * (g0))));
    unsigned long l2 = ((((l0) || (g2))) <= (((g0) & (l0))));
    unsigned _BitInt(29) l3 = g0;
    register unsigned char ry = (unsigned char)(l0);
    return ((unsigned char)(7));
}
[[c6502::return_bool_in_carry]] bool f2(void) {
    int l0 = ((((g2) || (7))) == (g0));
    for (int it = 0; it < 3; it++) l0 += (f3(g2, (!g0))) + it;
    return ((((unsigned char)(g2))) & (2));
}
unsigned char f1(register unsigned char p0, unsigned _BitInt(33) p1, unsigned _BitInt(33) p2, register signed char p3) {
    unsigned long l0 = (~((p2) < (p1)));
    register unsigned char ry = (unsigned char)(l0);
    return ((g2) & (((255) >= (255))));
}
unsigned long f0(int p0, unsigned int p1, unsigned int p2) {
    unsigned char l0 = p1;
    if (f4((!g1), ((p2) * (l0)), ((32767) + (l0)), ((g0) >> ((0) & 7)))) l0 += 3; else l0 ^= 5;
    return ((((l0) + (p1))) + (((unsigned _BitInt(29))(g2))));
}
int main(void) {
    long r = (long)f0(216, 84, 257);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
