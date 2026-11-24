int g0 = 166;
unsigned int g1 = 170;
int rec(int d, int x) {
    unsigned char a = (unsigned char)(x * 7);
    long b = (long)x * 3;
    if (d <= 0) return x & 255;
    [[c6502::save_hardware_stack]] int r = rec(d - 1, (x * 3 + d) & 1023);
    return (r + a + (int)(b & 15)) & 1023;
}
unsigned int f4(unsigned _BitInt(48) p0, unsigned long p1, unsigned _BitInt(48) p2) {
    int l0 = ((((p1) <= (g0))) ^ (p0));
    int l1 = ((((l0) | (100))) != (((l0) ^ (l0))));
    unsigned int l2 = ((int)(((unsigned long)(g1))));
    long l3 = p1;
    return (!((0) * (100)));
}
long f3(unsigned _BitInt(48) p0, register int p1, long p2) {
    unsigned char l0 = ((((32767) + (p0))) != ((!g1)));
    unsigned int l1 = ((p1) > (((p1) ^ (g1))));
    unsigned int l2 = ((((g0) - (l1))) != (((unsigned _BitInt(12))(p1))));
    l1 += rec(3, (int)(((p2) / ((p0) | 1))));
    return (~((l0) != (l0)));
}
bool f2(int p0, unsigned char p1) {
    signed char l0 = ((((signed char)(p1))) > (((p0) * (g0))));
    long l1 = ((((32767) | (g1))) > (((0) || (p1))));
    unsigned _BitInt(48) l2 = 7;
    long l3 = p0;
    l3 ^= f3(l0, ((255) - (l0)), l2);
    g0 += l3;
    unsigned int (*fp)(unsigned _BitInt(48), unsigned long, unsigned _BitInt(48)) = f4;
    l1 ^= fp(((long)(l0)), ((l1) >= (l3)), l3);
    l1 += rec(3, (int)(1000));
    return ((l0) - (32767));
}
unsigned int f1(signed char p0, unsigned _BitInt(48) p1, int * pp) {
    unsigned _BitInt(48) l0 = p1;
    long l1 = (-g0);
    *pp += (int)(((l0) == (l0)));
    unsigned int (*fp)(unsigned _BitInt(48), unsigned long, unsigned _BitInt(48)) = f4;
    l1 ^= fp(((l0) >= (l1)), p0, 1000);
    return ((((l1) - (p0))) > ((~p0)));
}
int f0(unsigned char p0) {
    unsigned _BitInt(12) l0 = ((32767) != (((p0) * (g0))));
    unsigned _BitInt(48) l1 = ((l0) || (((p0) % ((p0) | 1))));
    unsigned long l2 = ((((p0) / ((p0) | 1))) & (((g0) & (p0))));
    signed char l3 = g0;
    unsigned int (*fp)(unsigned _BitInt(48), unsigned long, unsigned _BitInt(48)) = f4;
    l2 ^= fp(((l3) % ((l1) | 1)), p0, ((g0) >= (l3)));
    g1 += l2;
    for (int it = 0; it < 3; it++) l2 += (f3(((g1) | (l2)), g0, l3)) + it;
    l2 += rec(3, (int)(l1));
    return l1;
}
int main(void) {
    long r = (long)f0(279);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
