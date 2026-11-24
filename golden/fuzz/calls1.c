signed char g0 = 65;
signed char g1 = 126;
unsigned int g2 = 120;
long f3(void) {
    long l0 = g0;
    unsigned int l1 = ((((1) / ((g2) | 1))) | (((32767) && (255))));
    int l2 = 1000;
    long l3 = ((((l2) > (l1))) & (g0));
    return ((l2) + (((l2) ^ (l1))));
}
signed char f2(signed char p0, signed char p1, int * pp) {
    unsigned int l0 = ((((g2) <= (p0))) | (((32767) || (32767))));
    unsigned int l1 = ((((255) >= (g0))) ^ (((g2) + (g2))));
    *pp += (int)(((g2) << ((l1) & 7)));
    return 1;
}
[[c6502::return_bool_in_carry]] bool f1(register int p0, unsigned long p1, unsigned char p2) {
    unsigned int l0 = p1;
    register unsigned char ry = (unsigned char)(l0);
    l0 ^= f3();
    g0 += l0;
    int box2 = (int)(l0);
    l0 ^= f2(7, ((ry) >> ((p0) & 7)), &box2);
    l0 += box2;
    return g0;
}
signed char f0(signed char p0, unsigned int p1, unsigned int p2) {
    signed char l0 = ((unsigned int)(((g0) && (p1))));
    unsigned char l1 = ((unsigned char)(((g0) % ((p2) | 1))));
    for (int it = 0; it < 3; it++) l1 += (f3()) + it;
    g1 += l1;
    return ((p0) | (((1000) <= (p0))));
}
int main(void) {
    long r = (long)f0(177, 50, 105);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
