unsigned int g0 = 115;
long g1 = 150;
unsigned char g2 = 47;
signed char f5(int * pp) {
    long l0 = ((((g1) / ((g2) | 1))) | (((g0) | (0))));
    signed char l1 = ((((int)(g2))) / ((l0) | 1));
    unsigned char l2 = g0;
    unsigned int l3 = ((((l2) <= (l0))) + (g1));
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(((g0) / ((l3) | 1)));
    return ry;
}
long f4(void) {
    signed char l0 = ((g0) > (g0));
    int l1 = (-((l0) >> ((g1) & 7)));
    long l2 = l1;
    register unsigned char ry = (unsigned char)(l0);
    int box5 = (int)(l0);
    l1 ^= f5(&box5);
    l1 += box5;
    return (!3);
}
unsigned int f3(unsigned long p0, unsigned long p1, long p2) {
    int l0 = ((((1) || (g0))) * (((p1) + (3))));
    int l1 = ((g1) && ((~g0)));
    long l2 = ((((g1) * (0))) % ((((p1) - (g2))) | 1));
    int l3 = ((((p0) <= (0))) / ((((unsigned long)(p1))) | 1));
    int box5 = (int)(l0);
    l3 ^= f5(&box5);
    l3 += box5;
    g2 += l3;
    return ((((l2) * (l3))) + (((l1) << ((l1) & 7))));
}
unsigned long f2(void) {
    int l0 = g2;
    unsigned char l1 = ((((l0) >= (g1))) % ((((g1) | (g2))) | 1));
    unsigned char l2 = ((((7) == (1))) == (((255) < (l0))));
    return ((((g2) + (l1))) / ((((g2) - (g1))) | 1));
}
[[c6502::return_bool_in_carry]] bool f1(void) {
    signed char l0 = ((((g0) & (7))) || (((g0) > (g1))));
    unsigned long l1 = (((-3)) || (((l0) <= (g0))));
    long l2 = ((((g1) | (g0))) >> ((g0) & 7));
    long l3 = ((g0) << ((((unsigned int)(l1))) & 7));
    int box5 = (int)(l0);
    for (int it = 0; it < 3; it++) l0 += (f5(&box5)) + it;
    l0 += box5;
    l3 ^= f2();
    g1 += l3;
    return ((((32767) != (1000))) & (((l0) || (g2))));
}
long f0(signed char p0) {
    int l0 = ((((g0) << ((32767) & 7))) - (((p0) || (p0))));
    long l1 = (((-g0)) % ((((255) != (g2))) | 1));
    int l2 = ((((g0) << ((p0) & 7))) - (p0));
    register unsigned char ry = (unsigned char)(l0);
    for (int it = 0; it < 3; it++) l0 += (f4()) + it;
    g2 += l0;
    return ((g0) & (((l2) && (l2))));
}
int main(void) {
    long r = (long)f0(275);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
