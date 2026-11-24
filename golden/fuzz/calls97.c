int g0 = 13;
long g1 = 5;
long f3(unsigned char p0) {
    long l0 = p0;
    unsigned char l1 = 2;
    long l2 = ((((p0) != (2))) + (l0));
    return ((long)(((0) - (p0))));
}
[[c6502::return_bool_in_carry]] bool f2(int p0) {
    long l0 = ((g1) < (((p0) & (3))));
    int l1 = p0;
    return p0;
}
[[c6502::return_bool_in_carry]] bool f1(unsigned long p0, signed char p1, register int p2, int * pp) {
    long l0 = g1;
    signed char l1 = g0;
    unsigned char l2 = (((-l1)) * (((p1) & (g0))));
    *pp += (int)(((3) + (0)));
    return ((((l2) - (p1))) % ((((p1) >= (p2))) | 1));
}
[[c6502::return_bool_in_carry]] bool f0(int p0, signed char p1, int * pp) {
    long l0 = p0;
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(p0);
    return ((ry) >> ((((l0) == ((*pp)))) & 7));
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(216, 196, &mbox);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
