long g0 = 41;
unsigned char g1 = 59;
long g2 = 65;
signed char f3(unsigned long p0, unsigned char p1, int p2) {
    signed char l0 = ((((1) + (g0))) > (((100) & (g1))));
    unsigned int l1 = ((((signed char)(g1))) || (((g1) >> ((l0) & 7))));
    unsigned int l2 = ((int)(((long)(l0))));
    unsigned char l3 = ((((g2) == (l2))) >= (((7) || (p2))));
    return ((0) && ((!p1)));
}
signed char f2(void) {
    long l0 = ((((g2) <= (g2))) || (((g1) < (g2))));
    unsigned int l1 = ((((g1) >= (g1))) - (g2));
    long l2 = 0;
    signed char (*fp)(unsigned long, unsigned char, int) = f3;
    l2 ^= fp(l1, ((g1) * (g2)), ((g0) & (l0)));
    return ((((unsigned char)(l2))) >= (((g1) || (l1))));
}
unsigned int f1(unsigned int p0, unsigned long p1) {
    signed char l0 = ((1) % ((p1) | 1));
    unsigned long l1 = 3;
    unsigned int l2 = ((((p0) | (g2))) ^ (g2));
    signed char l3 = 1;
    return ((l0) % ((((l3) / ((7) | 1))) | 1));
}
[[c6502::return_bool_in_carry]] bool f0(long p0, unsigned long p1, int p2, long p3) {
    unsigned char l0 = p2;
    l0 ^= f1(g2, g2);
    g0 += l0;
    signed char (*fp)(unsigned long, unsigned char, int) = f3;
    for (int it = 0; it < 3; it++) l0 += (fp(g1, g2, ((g1) >= (p1)))) + it;
    g0 += l0;
    return ((int)(((l0) >= (p1))));
}
int main(void) {
    long r = (long)f0(256, 62, 13, 90);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
