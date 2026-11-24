long g0 = 76;
unsigned int g1 = 124;
int rec(int d, int x) {
    unsigned char a = (unsigned char)(x * 7);
    long b = (long)x * 3;
    if (d <= 0) return x & 255;
    [[c6502::save_hardware_stack]] int r = rec(d - 1, (x * 3 + d) & 1023);
    return (r + a + (int)(b & 15)) & 1023;
}
[[c6502::return_bool_in_carry]] bool f3(register int p0, unsigned int p1) {
    signed char l0 = ((((p0) < (p1))) >> ((((g0) == (g1))) & 7));
    unsigned char l1 = ((l0) < (((1000) >> ((l0) & 7))));
    register unsigned char ry = (unsigned char)(l0);
    return ((((g1) % ((g1) | 1))) || (((l1) % ((p0) | 1))));
}
int f2(int p0, long p1, unsigned char p2, int * pp) {
    long l0 = (-((g1) >> ((p2) & 7)));
    unsigned int l1 = ((unsigned long)(p1));
    unsigned char l2 = (~p2);
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(((ry) + (0)));
    l2 ^= f3(g1, ((l0) - (3)));
    return (*pp);
}
[[c6502::return_bool_in_carry]] bool f1(long p0, register signed char p1, long p2) {
    int l0 = ((signed char)(((g0) + (255))));
    unsigned char l1 = ((((p0) / ((7) | 1))) | (((32767) * (p1))));
    if (f3(((255) || (l0)), ((p2) || (g1)))) l1 += 3; else l1 ^= 5;
    g0 += l1;
    l1 += rec(3, (int)(g0));
    return ((100) << ((((p2) > (p2))) & 7));
}
long f0(unsigned char p0, unsigned long p1, unsigned char p2, int * pp) {
    int l0 = ((((p2) / ((p1) | 1))) & (((p0) || (p0))));
    signed char l1 = ((unsigned char)(((long)(p1))));
    unsigned int l2 = ((((p1) * (7))) - (((unsigned long)(g1))));
    unsigned char l3 = p1;
    *pp += (int)(((255) >> ((p0) & 7)));
    if (f3(g0, ((long)(l0)))) l2 += 3; else l2 ^= 5;
    int box2 = (int)(l0);
    l3 ^= f2(((g1) && (2)), p2, l2, &box2);
    l3 += box2;
    g1 += l3;
    l2 += rec(3, (int)(p1));
    return ((unsigned char)(p1));
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(244, 149, 173, &mbox);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
