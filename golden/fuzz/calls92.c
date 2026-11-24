long g0 = 172;
unsigned char g1 = 145;
_BitInt(40) g2 = 40;
int f5(int p0) {
    unsigned char l0 = 1000;
    _BitInt(40) l1 = 2;
    unsigned long l2 = g2;
    return p0;
}
long f4(register int p0, int * pp) {
    _BitInt(40) l0 = ((((32767) + (p0))) ^ ((!g0)));
    unsigned long l1 = ((((255) || (255))) < (l0));
    int l2 = ((((l1) | (p0))) * (((l0) == (l1))));
    *pp += (int)(((l1) <= (g2)));
    l2 ^= f5(((l1) > (p0)));
    g1 += l2;
    return ((((l1) >= (3))) + ((((*pp)) / ((l0) | 1))));
}
int f3(unsigned char p0, _BitInt(58) p1, unsigned long p2) {
    unsigned char l0 = ((((p2) != (3))) * (((long)(g1))));
    long l1 = 0;
    signed char l2 = l1;
    return g0;
}
int f2(_BitInt(40) p0, register int p1, unsigned char p2) {
    signed char l0 = g1;
    signed char l1 = ((((g1) == (p0))) | (g0));
    unsigned int l2 = ((((100) || (l0))) ^ (((l1) && (p2))));
    int l3 = ((((g1) <= (p0))) + (((g0) || (100))));
    int box4 = (int)(l0);
    for (int it = 0; it < 3; it++) l3 += (f4(((l0) <= (l2)), &box4)) + it;
    l3 += box4;
    g1 += l3;
    return ((((l2) > (p2))) / ((((p2) || (p2))) | 1));
}
signed char f1(int p0) {
    unsigned int l0 = ((((g2) == (g0))) >= (((g2) / ((g1) | 1))));
    unsigned char l1 = ((g0) >= (((g1) ^ (g1))));
    long l2 = ((((7) != (p0))) != (((g1) ^ (1))));
    signed char l3 = g2;
    for (int it = 0; it < 3; it++) l1 += (f3(l1, (-g1), ((l3) && (g2)))) + it;
    l3 ^= f2((~g1), ((l3) > (2)), ((l1) ^ (g2)));
    g1 += l3;
    return (-l0);
}
bool f0(int * pp) {
    long l0 = ((g1) != (g0));
    int l1 = g1;
    _BitInt(40) l2 = ((g2) || (((7) < (l1))));
    unsigned long l3 = ((((l2) + (g0))) % ((((g0) == (l0))) | 1));
    *pp += (int)(l3);
    l0 ^= f5(((255) % ((l1) | 1)));
    g1 += l0;
    return l1;
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(&mbox);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
