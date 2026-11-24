int g0 = 152;
unsigned char g1 = 61;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
unsigned long f3(long p0, register signed char p1) {
    int l0 = ((((g0) < (2))) < (7));
    signed char l1 = ((32767) + (((g0) / ((g1) | 1))));
    signed char l2 = ((((g0) || (32767))) != (2));
    signed char l3 = ((unsigned long)(((p0) != (l0))));
    register unsigned char ry = (unsigned char)(l0);
    return ((g1) ^ (l0));
}
unsigned int f2(unsigned char p0, unsigned int p1) {
    signed char l0 = p0;
    signed char l1 = ((int)(((g0) << ((l0) & 7))));
    l0 ^= f3(((g0) % ((l1) | 1)), 3);
    g1 += l0;
    l0 += rec(3, (int)(((l0) != (p0))));
    return ((((255) * (l0))) > (p1));
}
[[c6502::return_bool_in_carry]] bool f1(void) {
    unsigned char l0 = 1000;
    long l1 = ((g1) < (((l0) | (g1))));
    l0 ^= f2(((l1) / ((32767) | 1)), ((g1) > (g0)));
    g1 += l0;
    l0 ^= f3(g0, ((1) >> ((l1) & 7)));
    g0 += l0;
    return ((((100) < (l1))) * (g1));
}
[[c6502::return_bool_in_carry]] bool f0(void) {
    unsigned long l0 = g1;
    unsigned char l1 = (((-255)) && (g1));
    signed char l2 = g1;
    int l3 = ((((g0) <= (7))) < (((l0) < (l0))));
    return (!(!l2));
}
int main(void) {
    long r = (long)f0();
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
