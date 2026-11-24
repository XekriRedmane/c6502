unsigned char g0 = 55;
unsigned char g1 = 49;
unsigned char g2 = 171;
int f5(void) {
    unsigned char l0 = ((g0) % ((((g0) * (g0))) | 1));
    int l1 = ((g2) ^ (((7) < (l0))));
    int l2 = 100;
    unsigned int l3 = ((255) >> ((((l0) >> ((3) & 7))) & 7));
    return g0;
}
int f4(void) {
    int l0 = ((int)(((g1) > (g1))));
    return 1000;
}
bool f3(long p0, unsigned char p1) {
    unsigned char l0 = ((((g1) / ((p0) | 1))) != (((unsigned long)(32767))));
    register unsigned char ry = (unsigned char)(l0);
    return ((p1) << ((((g2) >> ((l0) & 7))) & 7));
}
unsigned char f2(unsigned long p0, unsigned int p1, unsigned long p2) {
    unsigned int l0 = ((((100) % ((g1) | 1))) >> ((p2) & 7));
    unsigned char l1 = (~3);
    unsigned int l2 = l1;
    long l3 = (((-p2)) < (((g0) ^ (l1))));
    register unsigned char ry = (unsigned char)(l0);
    l1 ^= f3(g1, ((l3) > (g0)));
    l3 ^= f5();
    g1 += l3;
    return ((((p0) + (l2))) | (((l2) | (100))));
}
bool f1(int * pp) {
    unsigned char l0 = ((((g2) << ((g0) & 7))) <= (((2) % ((g2) | 1))));
    long l1 = 3;
    unsigned int l2 = ((((g1) > (3))) | (((g2) ^ (g1))));
    *pp += (int)(((l0) - (32767)));
    if (f3((((*pp)) ^ (l2)), (~0))) l2 += 3; else l2 ^= 5;
    g0 += l2;
    return ((((100) + (g1))) < (((l1) & (l2))));
}
unsigned char f0(unsigned long p0, signed char p1) {
    unsigned char l0 = (~((p1) << ((p1) & 7)));
    unsigned char l1 = ((((g0) % ((p0) | 1))) >= (((g2) & (l0))));
    long l2 = ((((p0) >> ((l1) & 7))) ^ (p0));
    unsigned int l3 = ((((p0) >= (g2))) <= (((l0) <= (l1))));
    l2 ^= f2(((1) % ((0) | 1)), l2, ((long)(g0)));
    g0 += l2;
    return ((l3) || (((32767) % ((l0) | 1))));
}
int main(void) {
    long r = (long)f0(121, 67);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
