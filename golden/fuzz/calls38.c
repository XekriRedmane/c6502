unsigned long g0 = 26;
signed char g1 = 93;
signed char f5(signed char p0, unsigned long p1, int * pp) {
    unsigned long l0 = ((((p0) ^ (g1))) <= (((g0) ^ (0))));
    unsigned char l1 = ((p0) - (((p1) | (7))));
    *pp += (int)(((p0) | (l0)));
    return p1;
}
[[c6502::return_bool_in_carry]] bool f4(unsigned long p0, long p1, long p2, signed char p3, int * pp) {
    long l0 = ((p0) >= (((g1) << ((p3) & 7))));
    *pp += (int)(((p1) / ((g1) | 1)));
    int box5 = (int)(l0);
    signed char (*fp)(signed char, unsigned long, int *) = f5;
    l0 ^= fp(((p2) | (p3)), ((int)(p1)), &box5);
    l0 += box5;
    g1 += l0;
    return ((((p1) < (g0))) && ((!0)));
}
bool f3(long p0, register unsigned char p1, register signed char p2) {
    unsigned long l0 = ((((g1) > (p1))) << (((~7)) & 7));
    unsigned char l1 = ((((7) && (2))) && (p1));
    int l2 = 1;
    register unsigned char ry = (unsigned char)(l0);
    return 0;
}
[[c6502::return_bool_in_carry]] bool f2(unsigned char p0, long p1, signed char p2) {
    long l0 = ((((255) & (p1))) >> ((((255) & (p0))) & 7));
    if (f3(((32767) == (32767)), ((int)(l0)), g0)) l0 += 3; else l0 ^= 5;
    int box5 = (int)(l0);
    signed char (*fp)(signed char, unsigned long, int *) = f5;
    l0 ^= fp(((p2) - (g1)), p0, &box5);
    l0 += box5;
    return ((p1) + (((g0) * (l0))));
}
int f1(long p0, unsigned long p1) {
    int l0 = (((~g0)) / ((((g0) ^ (g0))) | 1));
    l0 ^= f2(p1, g0, p1);
    return ((((p0) >= (g0))) & (((3) == (g0))));
}
long f0(long p0) {
    unsigned int l0 = ((g0) & (((p0) + (g1))));
    if (f3(((l0) != (1)), ((p0) / ((g0) | 1)), ((32767) - (l0)))) l0 += 3; else l0 ^= 5;
    g1 += l0;
    return ((signed char)((~g1)));
}
int main(void) {
    long r = (long)f0(288);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
