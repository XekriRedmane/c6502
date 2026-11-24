unsigned long g0 = 90;
signed char g1 = 23;
int g2 = 37;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
signed char f5(int * pp) {
    unsigned long l0 = ((((g0) + (g0))) >> ((((g0) && (g0))) & 7));
    unsigned long l1 = ((g2) < (g0));
    unsigned char l2 = ((((g2) >> ((l0) & 7))) && (l0));
    *pp += (int)((~g2));
    l0 += rec(3, (int)(((g0) * (l2))));
    return ((g1) | (((l0) & (255))));
}
int f4(unsigned char p0, register unsigned int p1) {
    unsigned long l0 = ((g1) != (((p1) * (g1))));
    unsigned int l1 = g2;
    signed char l2 = ((p1) >> ((((p1) ^ (g0))) & 7));
    unsigned long l3 = ((((100) | (p0))) <= (((long)(g2))));
    return g2;
}
unsigned int f3(long p0) {
    int l0 = ((((g1) > (g0))) || (g0));
    unsigned char l1 = ((g0) / ((2) | 1));
    long l2 = ((((l0) ^ (g0))) % ((((g2) + (g1))) | 1));
    l0 ^= f4(p0, ((p0) % ((1000) | 1)));
    g2 += l0;
    int box5 = (int)(l0);
    signed char (*fp)(int *) = f5;
    for (int it = 0; it < 3; it++) l2 += (fp(&box5)) + it;
    l2 += box5;
    g0 += l2;
    l2 += rec(3, (int)(3));
    return ((((7) - (l0))) > (((p0) - (g2))));
}
unsigned long f2(unsigned char p0) {
    signed char l0 = (!((100) <= (g1)));
    int box5 = (int)(l0);
    signed char (*fp)(int *) = f5;
    for (int it = 0; it < 3; it++) l0 += (fp(&box5)) + it;
    l0 += box5;
    g2 += l0;
    return ((((p0) ^ (p0))) >= ((~1000)));
}
[[c6502::return_bool_in_carry]] bool f1(signed char p0, unsigned long p1, signed char p2) {
    long l0 = ((((g2) > (100))) & (p2));
    unsigned int l1 = ((g1) < (p2));
    long l2 = g0;
    int l3 = g0;
    int box5 = (int)(l0);
    signed char (*fp)(int *) = f5;
    for (int it = 0; it < 3; it++) l2 += (fp(&box5)) + it;
    l2 += box5;
    l0 ^= f2(l3);
    l0 += rec(3, (int)(g1));
    return p2;
}
unsigned char f0(signed char p0, long p1, long p2, signed char p3) {
    int l0 = ((((g0) << ((2) & 7))) + (p0));
    signed char l1 = ((signed char)(((signed char)(p0))));
    l0 += rec(3, (int)(3));
    return g1;
}
int main(void) {
    long r = (long)f0(258, 285, 218, 13);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
