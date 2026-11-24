int g0 = 175;
int g1 = 102;
unsigned _BitInt(19) g2 = 200;
unsigned char f2(long p0, unsigned _BitInt(21) p1, unsigned long p2) {
    unsigned long l0 = ((((g1) <= (0))) && (((g1) >> ((g2) & 7))));
    unsigned long l1 = ((((p2) | (g1))) >> ((((g1) && (g2))) & 7));
    unsigned int l2 = ((p0) & (32767));
    signed char l3 = ((g1) ^ (l0));
    register unsigned char ry = (unsigned char)(l0);
    return ((((l2) & (7))) >= (l3));
}
unsigned long f1(unsigned int p0, unsigned int p1, unsigned _BitInt(21) p2) {
    unsigned _BitInt(21) l0 = ((((p1) & (g1))) & (((unsigned char)(g0))));
    unsigned int l1 = ((((7) ^ (p0))) < ((-p1)));
    signed char l2 = 1;
    unsigned long l3 = (-((p1) < (p0)));
    l2 ^= f2(3, ((p2) + (p1)), ((p1) > (g2)));
    g2 += l2;
    return ((((7) && (3))) == (((p2) == (p2))));
}
long f0(register unsigned int p0) {
    signed char l0 = ((int)(((g0) % ((3) | 1))));
    unsigned char l1 = ((((unsigned _BitInt(21))(g0))) == (((p0) && (p0))));
    unsigned char l2 = ((l0) * (((32767) != (g1))));
    unsigned char l3 = ((g1) == (((1) == (g2))));
    register unsigned char ry = (unsigned char)(l0);
    return ((((l3) / ((l3) | 1))) || (((ry) ^ (l0))));
}
int main(void) {
    long r = (long)f0(293);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
