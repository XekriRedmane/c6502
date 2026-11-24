unsigned long g0 = 74;
int g1 = 196;
unsigned char f5(unsigned _BitInt(25) p0, register int p1, unsigned _BitInt(59) p2, unsigned long p3, int * pp) {
    long l0 = g0;
    *pp += (int)(((p2) / ((p2) | 1)));
    return ((g1) - ((*pp)));
}
[[c6502::return_bool_in_carry]] bool f4(void) {
    long l0 = g1;
    signed char l1 = ((l0) || (((2) - (g0))));
    int box5 = (int)(l0);
    l0 ^= f5(((g1) >> ((1) & 7)), ((g0) ^ (g1)), ((l0) <= (l1)), ((l1) | (l1)), &box5);
    l0 += box5;
    return ((((g0) & (100))) >= (((l0) >> ((g1) & 7))));
}
unsigned int f3(register unsigned int p0) {
    unsigned char l0 = ((((g0) % ((g1) | 1))) < (((g0) == (p0))));
    return p0;
}
unsigned _BitInt(59) f2(register signed char p0, int * pp) {
    long l0 = ((0) - (((0) >= (1))));
    signed char l1 = g1;
    unsigned long l2 = ((((p0) * (g1))) % ((((g1) < (p0))) | 1));
    unsigned int l3 = ((((l0) != (g0))) == (((2) >= (p0))));
    *pp += (int)(((g0) & (g0)));
    l2 ^= f4();
    int box5 = (int)(l0);
    l0 ^= f5(1, (~l0), ((g1) != (l0)), p0, &box5);
    l0 += box5;
    g0 += l0;
    return ((1) < (1000));
}
bool f1(unsigned _BitInt(59) p0, long p1, unsigned long p2) {
    int l0 = ((((p1) ^ (p2))) >= (p2));
    unsigned _BitInt(59) l1 = g0;
    if (f4()) l0 += 3; else l0 ^= 5;
    return 0;
}
long f0(void) {
    unsigned _BitInt(25) l0 = ((((g0) | (g0))) ^ (g0));
    unsigned _BitInt(59) l1 = g1;
    int l2 = ((((l1) <= (g1))) & (((3) | (l1))));
    return ((((2) != (l0))) - (g1));
}
int main(void) {
    long r = (long)f0();
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
