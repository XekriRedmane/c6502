long g0 = 35;
unsigned long g1 = 16;
int g2 = 106;
int rec(int d, int x) {
    unsigned char a = (unsigned char)(x * 7);
    long b = (long)x * 3;
    if (d <= 0) return x & 255;
    [[c6502::save_hardware_stack]] int r = rec(d - 1, (x * 3 + d) & 1023);
    return (r + a + (int)(b & 15)) & 1023;
}
signed char f2(unsigned long p0, unsigned char p1, unsigned char p2, unsigned _BitInt(33) p3) {
    unsigned int l0 = (!g0);
    int l1 = ((((l0) | (g0))) >= (((7) > (3))));
    return 7;
}
int f1(signed char p0, unsigned int p1, int p2) {
    unsigned long l0 = ((long)(g1));
    unsigned char l1 = ((((1000) % ((p0) | 1))) == (((g0) - (32767))));
    signed char (*fp)(unsigned long, unsigned char, unsigned char, unsigned _BitInt(33)) = f2;
    for (int it = 0; it < 3; it++) l1 += (fp(l0, ((g1) >= (1)), 1, ((p1) + (g1)))) + it;
    g2 += l1;
    l0 += rec(3, (int)(g2));
    return ((((l0) && (g2))) | (((int)(l1))));
}
[[c6502::return_bool_in_carry]] bool f0(void) {
    int l0 = ((((g2) % ((g1) | 1))) <= (((g0) - (g1))));
    unsigned long l1 = ((((3) / ((g1) | 1))) | (((g1) || (g1))));
    l0 ^= f1(32767, ((g0) << ((g1) & 7)), ((g2) | (g1)));
    return ((((g2) <= (l1))) || (g1));
}
int main(void) {
    long r = (long)f0();
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
