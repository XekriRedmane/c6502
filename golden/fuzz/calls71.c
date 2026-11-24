signed char g0 = 67;
int g1 = 193;
unsigned int g2 = 67;
int rec(int d, int x) {
    unsigned char a = (unsigned char)(x * 7);
    long b = (long)x * 3;
    if (d <= 0) return x & 255;
    [[c6502::save_hardware_stack]] int r = rec(d - 1, (x * 3 + d) & 1023);
    return (r + a + (int)(b & 15)) & 1023;
}
int f4(unsigned long p0) {
    unsigned int l0 = ((((p0) + (g1))) < (g2));
    l0 += rec(3, (int)(((p0) ^ (g2))));
    return g0;
}
unsigned _BitInt(35) f3(unsigned char p0, signed char p1, unsigned int p2) {
    long l0 = ((((1000) | (g0))) >= (((g2) - (0))));
    unsigned _BitInt(35) l1 = g1;
    int l2 = ((((g1) >> ((g1) & 7))) >= (((p0) < (255))));
    long l3 = ((((g1) >= (l1))) * (((p2) - (p2))));
    return (((~0)) % ((p2) | 1));
}
bool f2(unsigned int p0, register int p1, unsigned long p2) {
    unsigned _BitInt(59) l0 = ((unsigned long)(((g2) || (7))));
    return ((((32767) * (l0))) >> ((l0) & 7));
}
int f1(void) {
    long l0 = ((g0) | (((g1) == (32767))));
    l0 += rec(3, (int)((!l0)));
    return ((((l0) << ((7) & 7))) | (((signed char)(g0))));
}
[[c6502::return_bool_in_carry]] bool f0(signed char p0, unsigned long p1, unsigned _BitInt(59) p2, register int p3) {
    unsigned char l0 = (!p1);
    unsigned int l1 = ((((p0) - (g2))) != (((g0) == (p3))));
    long l2 = p3;
    int l3 = g0;
    register unsigned char ry = (unsigned char)(l0);
    l2 += rec(3, (int)(((l3) * (l0))));
    return ((g1) - (((l3) >> ((7) & 7))));
}
int main(void) {
    long r = (long)f0(130, 3, 140, 141);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
