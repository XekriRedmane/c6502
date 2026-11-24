int g0 = 139;
int g1 = 57;
int g2 = 163;
unsigned char f2(unsigned long p0, int p1, register int p2) {
    long l0 = p2;
    unsigned char l1 = ((g0) && (((g0) <= (l0))));
    int l2 = g0;
    return ((g2) && (((1) != (p2))));
}
[[c6502::return_bool_in_carry]] bool f1(register unsigned int p0, unsigned int p1, signed char p2, unsigned _BitInt(19) p3, int * pp) {
    unsigned _BitInt(19) l0 = p3;
    long l1 = ((g0) > (((p0) > (l0))));
    unsigned char l2 = p1;
    int l3 = l1;
    *pp += (int)((!p2));
    return ((((255) * (g2))) * (((l2) && (100))));
}
bool f0(unsigned _BitInt(19) p0, int * pp) {
    unsigned char l0 = ((((1) % ((p0) | 1))) + (((p0) != (g0))));
    unsigned _BitInt(15) l1 = g2;
    signed char l2 = 1000;
    *pp += (int)(((l2) > (l2)));
    int box1 = (int)(l0);
    l1 ^= f1(((l1) / ((1) | 1)), ((long)(g1)), ((32767) > (3)), ((l0) ^ (32767)), &box1);
    l1 += box1;
    g2 += l1;
    l2 ^= f2((-l1), ((l1) < (g1)), (-1000));
    g2 += l2;
    return (~((l0) / ((7) | 1)));
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(42, &mbox);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
