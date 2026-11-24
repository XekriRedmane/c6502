unsigned long g0 = 39;
int rec(int d, int x) {
    unsigned char a = (unsigned char)(x * 7);
    long b = (long)x * 3;
    if (d <= 0) return x & 255;
    [[c6502::save_hardware_stack]] int r = rec(d - 1, (x * 3 + d) & 1023);
    return (r + a + (int)(b & 15)) & 1023;
}
unsigned long f4(int * pp) {
    _BitInt(33) l0 = g0;
    signed char l1 = ((((l0) == (g0))) & (((unsigned int)(3))));
    signed char l2 = ((int)(g0));
    _BitInt(44) l3 = ((_BitInt(33))(((l1) - (l2))));
    *pp += (int)(((l1) != (l1)));
    l0 += rec(3, (int)(((l1) <= ((*pp)))));
    return 1000;
}
_BitInt(44) f3(int * pp) {
    unsigned int l0 = ((((g0) / ((g0) | 1))) > (255));
    *pp += (int)(((1) && (g0)));
    return (*pp);
}
unsigned long f2(void) {
    unsigned int l0 = ((g0) > (((1) + (g0))));
    _BitInt(33) l1 = ((((0) == (g0))) - (((l0) - (l0))));
    _BitInt(33) l2 = ((g0) || (((l0) + (g0))));
    unsigned char l3 = g0;
    register unsigned char ry = (unsigned char)(l0);
    int box4 = (int)(l0);
    l2 ^= f4(&box4);
    l2 += box4;
    int box3 = (int)(l0);
    l2 ^= f3(&box3);
    l2 += box3;
    g0 += l2;
    return l1;
}
[[c6502::return_bool_in_carry]] bool f1(void) {
    signed char l0 = ((7) << ((((int)(g0))) & 7));
    int box4 = (int)(l0);
    for (int it = 0; it < 3; it++) l0 += (f4(&box4)) + it;
    l0 += box4;
    g0 += l0;
    return g0;
}
[[c6502::return_bool_in_carry]] bool f0(long p0, unsigned long p1, unsigned char p2, unsigned long p3, int * pp) {
    unsigned long l0 = ((((p1) + (p1))) >> ((((int)(1000))) & 7));
    unsigned long l1 = ((p2) > (g0));
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(((2) | (3)));
    int box4 = (int)(l0);
    for (int it = 0; it < 3; it++) l0 += (f4(&box4)) + it;
    l0 += box4;
    g0 += l0;
    return p2;
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(175, 103, 300, 5, &mbox);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
