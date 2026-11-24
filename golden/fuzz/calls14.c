unsigned int g0 = 69;
long g1 = 74;
unsigned char g2 = 168;
[[c6502::return_bool_in_carry]] bool f2(long p0, int * pp) {
    unsigned char l0 = 255;
    *pp += (int)(((p0) & (g1)));
    return ((((l0) && (g1))) << ((p0) & 7));
}
unsigned long f1(long p0, unsigned long p1) {
    unsigned char l0 = p0;
    unsigned char l1 = ((g0) == (((p0) ^ (g1))));
    unsigned _BitInt(32) l2 = l0;
    register unsigned char ry = (unsigned char)(l0);
    int box2 = (int)(l0);
    if (f2(((l1) != (l0)), &box2)) l1 += 3; else l1 ^= 5;
    l1 += box2;
    g2 += l1;
    return l2;
}
_BitInt(5) f0(unsigned char p0, long p1, register unsigned int p2) {
    signed char l0 = p0;
    return 100;
}
int main(void) {
    long r = (long)f0(128, 58, 17);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
