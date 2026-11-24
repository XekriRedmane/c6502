unsigned char g0 = 92;
signed char f2(void) {
    int l0 = g0;
    int l1 = g0;
    return ((((l1) & (l0))) != (((l1) << ((l0) & 7))));
}
[[c6502::return_bool_in_carry]] bool f1(_BitInt(17) p0, unsigned long p1, unsigned _BitInt(7) p2) {
    unsigned _BitInt(7) l0 = ((((g0) & (p2))) | (((g0) >> ((g0) & 7))));
    unsigned _BitInt(7) l1 = ((((unsigned int)(0))) ^ ((~p0)));
    unsigned char l2 = (~0);
    for (int it = 0; it < 3; it++) l0 += (f2()) + it;
    g0 += l0;
    return ((long)(((unsigned long)(p0))));
}
long f0(register unsigned int p0, signed char p1) {
    int l0 = ((long)(((p0) / ((g0) | 1))));
    unsigned long l1 = ((((g0) != (p1))) == (((signed char)(p0))));
    for (int it = 0; it < 3; it++) l0 += (f1(((l0) < (p1)), l0, ((0) & (p1)))) + it;
    return ((((l0) ^ (p0))) + (((g0) && (g0))));
}
int main(void) {
    long r = (long)f0(77, 18);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
