unsigned long g0 = 191;
unsigned long g1 = 131;
signed char g2 = 161;
[[c6502::return_bool_in_carry]] bool f2(int p0, unsigned long p1) {
    long l0 = ((long)(((g2) > (g0))));
    unsigned char l1 = ((unsigned long)(7));
    register unsigned char ry = (unsigned char)(l0);
    return ((g1) + (((p0) || (p1))));
}
unsigned int f1(register unsigned char p0, register signed char p1, signed char p2) {
    unsigned long l0 = p0;
    unsigned char l1 = g1;
    unsigned long l2 = l1;
    signed char l3 = ((p2) ^ (((0) - (p1))));
    if (f2(((g1) == (g0)), ((l2) - (l3)))) l1 += 3; else l1 ^= 5;
    return ((((7) ^ (l1))) > (((g0) / ((100) | 1))));
}
signed char f0(long p0, int p1, register unsigned int p2) {
    long l0 = ((((g2) % ((g0) | 1))) & (((g0) != (p0))));
    unsigned char l1 = (((-p2)) << ((1) & 7));
    unsigned int l2 = p1;
    int l3 = ((1000) <= (((unsigned char)(g1))));
    register unsigned char ry = (unsigned char)(l0);
    for (int it = 0; it < 3; it++) l2 += (f1(((unsigned char)(7)), ((l2) * (g0)), l1)) + it;
    g0 += l2;
    for (int it = 0; it < 3; it++) l1 += (f2(((l1) % ((p2) | 1)), ((1000) + (p1)))) + it;
    return ((3) <= (((l0) != (l3))));
}
int main(void) {
    long r = (long)f0(261, 300, 261);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
