long g0 = 179;
signed char g1 = 45;
unsigned int g2 = 57;
int rec(int d, int x) {
    unsigned char a = (unsigned char)(x * 7);
    long b = (long)x * 3;
    if (d <= 0) return x & 255;
    [[c6502::save_hardware_stack]] int r = rec(d - 1, (x * 3 + d) & 1023);
    return (r + a + (int)(b & 15)) & 1023;
}
int f5(void) {
    unsigned char l0 = ((((g2) + (g0))) || (g2));
    long l1 = 3;
    return ((g0) >> ((((l0) + (g1))) & 7));
}
int f4(signed char p0, unsigned long p1) {
    int l0 = p1;
    int (*fp)() = f5;
    l0 ^= fp();
    return g0;
}
[[c6502::return_bool_in_carry]] bool f3(unsigned char p0, signed char p1, long p2, signed char p3) {
    long l0 = ((((p3) >> ((p3) & 7))) * (((p2) % ((p1) | 1))));
    int (*fp)() = f5;
    l0 ^= fp();
    g1 += l0;
    for (int it = 0; it < 3; it++) l0 += (f4(2, g1)) + it;
    g1 += l0;
    return g2;
}
unsigned int f2(int p0, signed char p1) {
    unsigned int l0 = g2;
    int l1 = ((((1) <= (255))) & (((l0) ^ (p1))));
    int (*fp)() = f5;
    l0 ^= fp();
    g1 += l0;
    for (int it = 0; it < 3; it++) l0 += (f4(g2, p0)) + it;
    g2 += l0;
    l0 += rec(3, (int)(((l1) * (l0))));
    return ((((0) / ((p1) | 1))) || (p1));
}
long f1(unsigned long p0, unsigned long p1) {
    unsigned char l0 = ((g0) >> ((p1) & 7));
    register unsigned char ry = (unsigned char)(l0);
    return ((((g1) != (1000))) == (g2));
}
[[c6502::return_bool_in_carry]] bool f0(int * pp) {
    unsigned int l0 = (~((g0) & (g2)));
    signed char l1 = ((((l0) || (g1))) != (((g2) == (g2))));
    signed char l2 = ((int)(l1));
    unsigned long l3 = g0;
    *pp += (int)(((l0) << ((l0) & 7)));
    return g0;
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(&mbox);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
