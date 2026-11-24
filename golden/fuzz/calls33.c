unsigned char g0 = 70;
unsigned int g1 = 166;
long g2 = 135;
int rec(int d, int x) {
    unsigned char a = (unsigned char)(x * 7);
    long b = (long)x * 3;
    if (d <= 0) return x & 255;
    [[c6502::save_hardware_stack]] int r = rec(d - 1, (x * 3 + d) & 1023);
    return (r + a + (int)(b & 15)) & 1023;
}
bool f3(register int p0, int p1, long p2) {
    int l0 = ((((p0) || (g2))) - (((signed char)(p2))));
    l0 += rec(3, (int)(g2));
    return ((((l0) + (p0))) || (((p0) + (p1))));
}
[[c6502::return_bool_in_carry]] bool f2(unsigned int p0, signed char p1, unsigned long p2, long p3) {
    unsigned int l0 = ((((7) && (p3))) < (((g2) <= (p0))));
    int l1 = 100;
    l1 += rec(3, (int)(((g1) < (255))));
    return ((((p0) << ((p1) & 7))) * (((unsigned char)(p3))));
}
unsigned long f1(void) {
    unsigned long l0 = ((((g0) % ((g0) | 1))) < (((g0) / ((g0) | 1))));
    l0 ^= f2(((0) + (g0)), ((g1) < (g0)), ((g0) << ((g1) & 7)), g1);
    l0 += rec(3, (int)(((g0) & (g2))));
    return ((((l0) << ((g2) & 7))) <= (g1));
}
long f0(int p0, unsigned long p1, long p2, register unsigned int p3) {
    unsigned char l0 = ((((p1) && (p1))) != (1));
    register unsigned char ry = (unsigned char)(l0);
    return 255;
}
int main(void) {
    long r = (long)f0(145, 144, 196, 159);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
