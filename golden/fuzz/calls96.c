unsigned _BitInt(34) g0 = 52;
unsigned char g1 = 87;
unsigned int f4(register unsigned char p0, int p1) {
    unsigned _BitInt(34) l0 = ((((g1) ^ (100))) ^ (2));
    int l1 = p1;
    int l2 = ((int)(((p1) > (0))));
    return p1;
}
bool f3(unsigned long p0, register signed char p1, int p2, register signed char p3, int * pp) {
    unsigned long l0 = 1;
    unsigned long l1 = 3;
    *pp += (int)(2);
    l1 ^= f4(((0) < (l1)), ((unsigned char)(l0)));
    return ((unsigned int)(2));
}
bool f2(_BitInt(9) p0, _BitInt(9) p1, register _BitInt(9) p2, unsigned long p3, int * pp) {
    unsigned long l0 = p2;
    *pp += (int)(((g0) || (p2)));
    l0 ^= f4((-p1), (((*pp)) % ((255) | 1)));
    return ((((long)(g0))) >= (((p3) << ((p3) & 7))));
}
[[c6502::return_bool_in_carry]] bool f1(unsigned long p0, unsigned long p1) {
    int l0 = (((~p0)) > (3));
    unsigned _BitInt(34) l1 = p1;
    unsigned _BitInt(34) l2 = l0;
    int box2 = (int)(l0);
    l2 ^= f2(p1, ((100) && (255)), l1, ((1000) < (l0)), &box2);
    l2 += box2;
    return ((g0) < (((1) && (2))));
}
signed char f0(unsigned _BitInt(34) p0) {
    int l0 = ((((p0) >> ((g0) & 7))) <= (g1));
    l0 ^= f4(((g0) | (p0)), p0);
    g0 += l0;
    int box2 = (int)(l0);
    l0 ^= f2(g0, ((g1) - (p0)), ((7) || (p0)), ((g0) & (g1)), &box2);
    l0 += box2;
    g0 += l0;
    return ((((l0) != (g0))) || (((p0) / ((l0) | 1))));
}
int main(void) {
    long r = (long)f0(191);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
