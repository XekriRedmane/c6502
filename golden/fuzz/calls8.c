unsigned int g0 = 32;
unsigned char g1 = 180;
int rec(int d, int x) {
    unsigned char a = (unsigned char)(x * 7);
    long b = (long)x * 3;
    if (d <= 0) return x & 255;
    [[c6502::save_hardware_stack]] int r = rec(d - 1, (x * 3 + d) & 1023);
    return (r + a + (int)(b & 15)) & 1023;
}
signed char f3(long p0, unsigned int p1) {
    unsigned int l0 = ((1) == (((long)(p1))));
    l0 += rec(3, (int)(g1));
    return p0;
}
unsigned int f2(void) {
    unsigned long l0 = ((((g1) && (g0))) - (((g0) << ((g0) & 7))));
    unsigned int l1 = l0;
    unsigned int l2 = g1;
    return ((unsigned char)((~l1)));
}
unsigned int f1(unsigned int p0, unsigned int p1, register unsigned int p2) {
    unsigned int l0 = ((p0) % ((((p1) != (32767))) | 1));
    long l1 = p1;
    int l2 = ((((0) | (g0))) & (l1));
    unsigned char l3 = ((g0) == (((p1) % ((l2) | 1))));
    l0 += rec(3, (int)(((l0) | (p0))));
    return g0;
}
[[c6502::return_bool_in_carry]] bool f0(unsigned int p0) {
    unsigned long l0 = ((((g1) | (g1))) >> ((((g1) >> ((p0) & 7))) & 7));
    signed char (*fp)(long, unsigned int) = f3;
    for (int it = 0; it < 3; it++) l0 += (fp(((255) * (g1)), ((g1) >> ((g1) & 7)))) + it;
    g1 += l0;
    for (int it = 0; it < 3; it++) l0 += (f1(l0, ((100) >= (100)), 32767)) + it;
    g1 += l0;
    return ((((7) << ((32767) & 7))) ^ (255));
}
int main(void) {
    long r = (long)f0(185);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
