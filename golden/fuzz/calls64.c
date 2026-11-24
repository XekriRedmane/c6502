unsigned _BitInt(7) g0 = 137;
unsigned long f5(long p0, unsigned long p1, unsigned _BitInt(19) p2, int p3, int * pp) {
    unsigned int l0 = ((7) - (((p1) >> ((p1) & 7))));
    int l1 = g0;
    *pp += (int)(p2);
    return (((((*pp)) - (p0))) < (((p2) && (1))));
}
unsigned _BitInt(19) f4(register unsigned int p0, long p1, int p2) {
    unsigned _BitInt(7) l0 = ((((int)(p2))) < (((p1) * (g0))));
    long l1 = p2;
    return l1;
}
signed char f3(long p0, unsigned int p1, unsigned int p2, unsigned char p3) {
    unsigned char l0 = g0;
    return (-((p1) / ((100) | 1)));
}
signed char f2(int * pp) {
    unsigned long l0 = g0;
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(g0);
    return l0;
}
unsigned _BitInt(7) f1(unsigned int p0, signed char p1, unsigned char p2) {
    unsigned _BitInt(7) l0 = ((((p1) << ((p2) & 7))) < (((g0) + (p1))));
    unsigned long l1 = ((7) == (((7) << ((255) & 7))));
    unsigned long l2 = g0;
    register unsigned char ry = (unsigned char)(l0);
    return ((((int)(p0))) && (((p0) && (l0))));
}
[[c6502::return_bool_in_carry]] bool f0(register int p0, int * pp) {
    unsigned int l0 = ((((p0) % ((32767) | 1))) << ((((p0) ^ (g0))) & 7));
    int l1 = ((((g0) != (p0))) == (((g0) || (l0))));
    *pp += (int)(((l0) + (p0)));
    return l1;
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(265, &mbox);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
