long g0 = 27;
int g1 = 49;
unsigned char g2 = 118;
long f3(void) {
    unsigned long l0 = ((g0) & (((g0) < (1000))));
    signed char l1 = g1;
    int l2 = ((((g1) != (g1))) & ((-g2)));
    unsigned long l3 = ((((g1) >> ((l1) & 7))) && (((l0) * (l2))));
    register unsigned char ry = (unsigned char)(l0);
    return ((((l3) ^ (l2))) >> ((ry) & 7));
}
long f2(long p0, register unsigned int p1, signed char p2, int * pp) {
    unsigned long l0 = ((((p1) < (g0))) | (p1));
    long l1 = ((signed char)(((l0) - (p0))));
    unsigned char l2 = ((((p2) << ((p1) & 7))) <= (g0));
    unsigned char l3 = 7;
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(((signed char)(g0)));
    return ((((p0) / ((g1) | 1))) & (((p1) != (g1))));
}
long f1(unsigned long p0) {
    signed char l0 = ((((g2) >= (g0))) < (((g0) <= (g2))));
    register unsigned char ry = (unsigned char)(l0);
    for (int it = 0; it < 3; it++) l0 += (f3()) + it;
    g1 += l0;
    int box2 = (int)(l0);
    l0 ^= f2(((g2) << ((g0) & 7)), ((32767) ^ (1)), g2, &box2);
    l0 += box2;
    g0 += l0;
    return ((p0) - (0));
}
[[c6502::return_bool_in_carry]] bool f0(register unsigned char p0, unsigned long p1, unsigned int p2, long p3) {
    int l0 = ((long)(g1));
    unsigned char l1 = (!((g1) <= (g0)));
    unsigned long l2 = l1;
    l0 ^= f1(((100) == (0)));
    return 100;
}
int main(void) {
    long r = (long)f0(113, 278, 23, 257);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
