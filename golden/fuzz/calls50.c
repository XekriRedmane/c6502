int g0 = 163;
unsigned char g1 = 177;
bool f5(unsigned int p0, unsigned long p1) {
    unsigned int l0 = ((p1) != (g1));
    unsigned long l1 = ((p0) * (((100) - (l0))));
    return ((((g1) / ((g1) | 1))) >> ((((g1) + (l0))) & 7));
}
signed char f4(register int p0, unsigned long p1, unsigned long p2, unsigned int p3, int * pp) {
    long l0 = ((((p0) & (p0))) >> ((0) & 7));
    unsigned long l1 = ((p1) >= (((g0) <= (p3))));
    int l2 = ((g1) <= (((l1) < (p1))));
    *pp += (int)(p1);
    l1 ^= f5(p1, ((g0) ^ ((*pp))));
    return ((((unsigned char)(p2))) + (l0));
}
int f3(long p0, unsigned int p1, unsigned long p2, signed char p3) {
    unsigned char l0 = ((((32767) & (p0))) | (g0));
    register unsigned char ry = (unsigned char)(l0);
    l0 ^= f5(ry, ((1) > (p0)));
    int box4 = (int)(l0);
    l0 ^= f4(ry, p3, 0, g1, &box4);
    l0 += box4;
    g1 += l0;
    return ((((ry) / ((p1) | 1))) <= (((p0) && (g0))));
}
unsigned char f2(void) {
    long l0 = g0;
    unsigned int l1 = ((((g1) * (1))) >= (((100) >= (l0))));
    unsigned int l2 = ((l0) > (((l0) && (g1))));
    signed char l3 = ((32767) == (((l2) >> ((1000) & 7))));
    int box4 = (int)(l0);
    for (int it = 0; it < 3; it++) l1 += (f4(((l1) == (255)), ((l0) >= (l2)), ((g0) == (l0)), l1, &box4)) + it;
    l1 += box4;
    g1 += l1;
    return ((int)(((g0) - (l0))));
}
unsigned char f1(register signed char p0, int p1) {
    signed char l0 = (((~2)) >> (((-2)) & 7));
    signed char l1 = ((unsigned long)(((g1) & (p0))));
    unsigned char l2 = ((((l1) != (g1))) ^ (((l1) > (g1))));
    unsigned long l3 = ((((p1) || (p1))) && (((7) <= (l1))));
    l2 ^= f3(p1, (-p0), p1, ((l0) || (p0)));
    l2 ^= f2();
    return ((unsigned int)(((p0) >= (l0))));
}
signed char f0(int p0, register unsigned char p1, unsigned long p2, long p3, int * pp) {
    signed char l0 = ((((g0) / ((p3) | 1))) > (((p3) - (1000))));
    unsigned char l1 = (!((100) == (p3)));
    unsigned int l2 = (~((p1) | (l1)));
    long l3 = ((((1) & (l0))) || (l0));
    *pp += (int)(((p2) == (g1)));
    return ((unsigned char)(((g0) < (p2))));
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(294, 228, 71, 157, &mbox);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
