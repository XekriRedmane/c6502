signed char g0 = 62;
long g1 = 170;
unsigned int g2 = 32;
signed char f5(unsigned long p0, unsigned int p1) {
    unsigned char l0 = ((g1) == (100));
    return g2;
}
unsigned long f4(unsigned char p0) {
    unsigned char l0 = ((((1000) | (g2))) & (((g0) | (1000))));
    int l1 = ((2) | (((unsigned int)(l0))));
    signed char (*fp)(unsigned long, unsigned int) = f5;
    l0 ^= fp(((g1) <= (p0)), ((long)(g0)));
    g2 += l0;
    return ((((32767) << ((g2) & 7))) >> ((((g0) > (p0))) & 7));
}
unsigned long f3(void) {
    signed char l0 = ((((int)(g0))) >= (((g2) < (g0))));
    return ((l0) || (((g1) < (l0))));
}
bool f2(int * pp) {
    _BitInt(16) l0 = g1;
    int l1 = ((((g0) / ((g2) | 1))) > (255));
    signed char l2 = g0;
    int l3 = l1;
    *pp += (int)(l0);
    signed char (*fp)(unsigned long, unsigned int) = f5;
    for (int it = 0; it < 3; it++) l3 += (fp(((signed char)(l1)), g2)) + it;
    return ((_BitInt(16))((((*pp)) >> ((g1) & 7))));
}
int f1(void) {
    unsigned int l0 = ((((g0) * (g0))) != ((!g2)));
    long l1 = ((((l0) * (g2))) < (((g2) | (255))));
    unsigned long l2 = (((-255)) % ((((unsigned int)(g2))) | 1));
    unsigned long l3 = (~((l0) | (l2)));
    signed char (*fp)(unsigned long, unsigned int) = f5;
    l1 ^= fp(((g2) & (1000)), 1000);
    g2 += l1;
    int box2 = (int)(l0);
    l3 ^= f2(&box2);
    l3 += box2;
    return ((((int)(0))) << ((g1) & 7));
}
[[c6502::return_bool_in_carry]] bool f0(void) {
    unsigned _BitInt(26) l0 = ((unsigned char)(((g1) / ((g1) | 1))));
    for (int it = 0; it < 3; it++) l0 += (f1()) + it;
    g1 += l0;
    for (int it = 0; it < 3; it++) l0 += (f4(((g1) || (g1)))) + it;
    return ((((g1) & (g2))) | (((g2) ^ (l0))));
}
int main(void) {
    long r = (long)f0();
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
