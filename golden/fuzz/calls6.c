int g0 = 9;
signed char g1 = 37;
[[c6502::return_bool_in_carry]] bool f2(unsigned long p0, unsigned long p1, register int p2, unsigned long p3) {
    int l0 = ((signed char)(p2));
    signed char l1 = ((((3) && (g1))) >> ((((unsigned int)(g0))) & 7));
    int l2 = ((unsigned int)(((g0) <= (p3))));
    unsigned char l3 = l2;
    register unsigned char ry = (unsigned char)(l0);
    return ((int)(((g0) <= (l2))));
}
[[c6502::return_bool_in_carry]] bool f1(long p0, long p1, unsigned long p2, int * pp) {
    unsigned long l0 = ((unsigned int)(((g0) == (p0))));
    unsigned int l1 = ((((p0) << ((l0) & 7))) > (((0) << ((p0) & 7))));
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(((l1) != (255)));
    if (f2(ry, ((ry) <= (p2)), ((ry) >> ((p2) & 7)), ((2) > (p1)))) l1 += 3; else l1 ^= 5;
    return (((~p0)) > (((p1) > (p1))));
}
unsigned long f0(int p0, signed char p1) {
    long l0 = ((unsigned long)(((g1) + (p0))));
    long l1 = 1;
    register unsigned char ry = (unsigned char)(l0);
    for (int it = 0; it < 3; it++) l1 += (f2(((1000) % ((p0) | 1)), ((1000) || (g1)), p0, ((g0) && (1)))) + it;
    return ((ry) - (((l1) - (g0))));
}
int main(void) {
    long r = (long)f0(107, 221);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
