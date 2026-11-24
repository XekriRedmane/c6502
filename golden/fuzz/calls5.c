int g0 = 176;
unsigned long g1 = 166;
long g2 = 7;
unsigned int f4(register unsigned char p0, int * pp) {
    unsigned int l0 = g0;
    unsigned char l1 = l0;
    *pp += (int)(((g1) < (1000)));
    return g0;
}
unsigned char f3(register unsigned char p0, signed char p1, register unsigned char p2) {
    unsigned int l0 = p2;
    unsigned int l1 = ((signed char)(p1));
    unsigned char l2 = ((((l1) >> ((g2) & 7))) | (1));
    int box4 = (int)(l0);
    l0 ^= f4(((l2) >> ((g2) & 7)), &box4);
    l0 += box4;
    return ((((l2) ^ (0))) < (((l0) != (p0))));
}
signed char f2(unsigned char p0, register signed char p1, unsigned long p2, unsigned char p3) {
    unsigned long l0 = ((p0) != (((g0) == (32767))));
    unsigned long l1 = ((p2) & (32767));
    int box4 = (int)(l0);
    for (int it = 0; it < 3; it++) l1 += (f4(((g0) && (l1)), &box4)) + it;
    l1 += box4;
    l1 ^= f3(((g0) != (g0)), 7, ((p0) < (g0)));
    return ((((g2) == (p2))) != (((l1) > (3))));
}
signed char f1(register unsigned int p0, unsigned char p1) {
    unsigned long l0 = ((((g1) << ((g2) & 7))) & (((long)(g1))));
    register unsigned char ry = (unsigned char)(l0);
    int box4 = (int)(l0);
    for (int it = 0; it < 3; it++) l0 += (f4(((g1) - (2)), &box4)) + it;
    l0 += box4;
    return ((((p0) > (p1))) >= (g0));
}
unsigned char f0(void) {
    unsigned long l0 = ((g0) >> ((g2) & 7));
    return ((((g1) != (l0))) / ((((g2) <= (0))) | 1));
}
int main(void) {
    long r = (long)f0();
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
