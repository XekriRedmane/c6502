unsigned long g0 = 117;
unsigned int g1 = 34;
int rec(int d, int x) {
    unsigned char a = (unsigned char)(x * 7);
    long b = (long)x * 3;
    if (d <= 0) return x & 255;
    [[c6502::save_hardware_stack]] int r = rec(d - 1, (x * 3 + d) & 1023);
    return (r + a + (int)(b & 15)) & 1023;
}
int f2(unsigned int p0, signed char p1, signed char p2, int * pp) {
    unsigned long l0 = (-((p0) >> ((p0) & 7)));
    long l1 = ((g1) << ((((g0) >= (g0))) & 7));
    long l2 = ((((l0) & (p0))) - (((signed char)(l1))));
    unsigned long l3 = ((0) * (((p1) >> ((1000) & 7))));
    *pp += (int)(p2);
    return ((((l3) - ((*pp)))) ^ (l1));
}
unsigned char f1(unsigned long p0, register signed char p1, long p2) {
    unsigned int l0 = g1;
    unsigned long l1 = ((((long)(g1))) != (p0));
    return ((int)(g0));
}
unsigned long f0(register unsigned char p0, unsigned long p1, unsigned long p2, unsigned int p3) {
    int l0 = g0;
    int l1 = (((~p1)) == (((p0) % ((l0) | 1))));
    unsigned long l2 = ((((l0) >> ((p1) & 7))) - ((!l1)));
    for (int it = 0; it < 3; it++) l1 += (f1(p1, ((unsigned int)(l2)), ((g0) == (l0)))) + it;
    g0 += l1;
    l2 += rec(3, (int)(((p2) ^ (p1))));
    return ((((32767) & (p3))) < (((255) == (255))));
}
int main(void) {
    long r = (long)f0(60, 97, 66, 166);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
