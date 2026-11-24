int g0 = 88;
unsigned long g1 = 78;
int g2 = 179;
unsigned char f2(unsigned long p0, signed char p1, _BitInt(57) p2, long p3) {
    unsigned long l0 = ((((100) | (g2))) <= (p0));
    unsigned char l1 = (!p0);
    return ((((l0) << ((p1) & 7))) | (((g0) ^ (g1))));
}
[[c6502::return_bool_in_carry]] bool f1(signed char p0, _BitInt(61) p1, unsigned char p2) {
    int l0 = ((((signed char)(g1))) ^ (((g2) == (p2))));
    unsigned long l1 = ((((p1) < (p0))) & (((p1) != (0))));
    _BitInt(57) l2 = ((3) > (((g0) + (g2))));
    unsigned char (*fp)(unsigned long, signed char, _BitInt(57), long) = f2;
    for (int it = 0; it < 3; it++) l2 += (fp(((l0) < (p0)), ((g2) - (0)), ((g1) * (1)), ((0) < (p2)))) + it;
    return ((l1) >= (((l1) || (p1))));
}
bool f0(void) {
    _BitInt(57) l0 = 2;
    int l1 = (!g1);
    long l2 = ((((g1) * (g0))) * (((g0) | (l0))));
    return ((((g1) << ((l0) & 7))) <= (((32767) && (g1))));
}
int main(void) {
    long r = (long)f0();
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
