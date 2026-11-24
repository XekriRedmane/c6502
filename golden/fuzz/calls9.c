int g0 = 68;
unsigned char g1 = 47;
unsigned long g2 = 1;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
unsigned int f5(void) {
    signed char l0 = ((((2) && (g0))) >= (g0));
    register unsigned char ry = (unsigned char)(l0);
    return ry;
}
[[c6502::return_bool_in_carry]] bool f4(void) {
    unsigned char l0 = ((g2) / (((-g2)) | 1));
    unsigned long l1 = ((g2) == (((g0) > (1))));
    register unsigned char ry = (unsigned char)(l0);
    return 0;
}
signed char f3(int * pp) {
    long l0 = ((g1) || (((g2) <= (g0))));
    unsigned int l1 = ((l0) > (((long)(g2))));
    unsigned char l2 = ((((g1) / ((g1) | 1))) >> ((((unsigned int)(g1))) & 7));
    long l3 = g2;
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)((!l1));
    for (int it = 0; it < 3; it++) l0 += (f5()) + it;
    g2 += l0;
    return ((((l1) > (ry))) % (((*pp)) | 1));
}
unsigned char f2(unsigned long p0, unsigned int p1, unsigned char p2, int * pp) {
    unsigned long l0 = (~((g0) || (g1)));
    signed char l1 = p2;
    *pp += (int)(((l0) || (p1)));
    l1 += rec(3, (int)(p2));
    return l1;
}
long f1(void) {
    unsigned int l0 = ((((unsigned char)(g1))) - (((g1) * (0))));
    l0 ^= f5();
    g0 += l0;
    l0 += rec(3, (int)(((g2) + (l0))));
    return ((unsigned int)(l0));
}
[[c6502::return_bool_in_carry]] bool f0(void) {
    unsigned int l0 = ((g0) - ((-1)));
    signed char l1 = g0;
    return ((((g1) | (l1))) + (l1));
}
int main(void) {
    long r = (long)f0();
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
