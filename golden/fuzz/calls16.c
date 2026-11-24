unsigned int g0 = 72;
unsigned int g1 = 58;
[[c6502::return_bool_in_carry]] bool f4(unsigned char p0, int p1) {
    long l0 = ((((p1) == (p0))) * (((1000) == (p0))));
    return p1;
}
[[c6502::return_bool_in_carry]] bool f3(unsigned long p0, unsigned long p1, signed char p2, unsigned int p3) {
    long l0 = g0;
    return ((((3) * (g1))) <= (((p0) - (1))));
}
unsigned long f2(register int p0, signed char p1, unsigned char p2, long p3) {
    unsigned char l0 = g0;
    unsigned char l1 = (~p0);
    unsigned int l2 = ((p3) >= (((255) / ((p3) | 1))));
    l0 ^= f3(p2, ((255) <= (p3)), p2, ((p0) / ((g0) | 1)));
    if (f4(((p3) % ((l2) | 1)), ((unsigned long)(p1)))) l2 += 3; else l2 ^= 5;
    g1 += l2;
    return p0;
}
int f1(int p0, unsigned long p1, int * pp) {
    signed char l0 = ((((p0) < (p1))) || (((p0) % ((p0) | 1))));
    unsigned long l1 = ((((g0) && (l0))) | (((p1) / ((p1) | 1))));
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(((1) / ((p1) | 1)));
    l0 ^= f4(((ry) == (p1)), g0);
    l1 ^= f2(((p1) >= (ry)), g1, ((l1) | (p1)), ((g1) - (l1)));
    return ((p0) | (((p0) & (p0))));
}
unsigned long f0(unsigned char p0, unsigned long p1, int * pp) {
    long l0 = ((((unsigned char)(p0))) >= (((g0) >> ((g1) & 7))));
    long l1 = p1;
    long l2 = l1;
    *pp += (int)(((l2) <= (7)));
    l0 ^= f3(((1000) < (p1)), ((1000) << ((g1) & 7)), ((g1) % ((255) | 1)), ((255) << ((p1) & 7)));
    g1 += l0;
    return ((p0) / ((((p1) | (7))) | 1));
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(61, 254, &mbox);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
