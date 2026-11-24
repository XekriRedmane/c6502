unsigned _BitInt(6) g0 = 6;
unsigned int g1 = 56;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
[[c6502::return_bool_in_carry]] bool f3(unsigned char p0, _BitInt(56) p1) {
    unsigned long l0 = ((((g1) >> ((g0) & 7))) <= (p0));
    _BitInt(56) l1 = ((((signed char)(g1))) / ((((p1) ^ (32767))) | 1));
    _BitInt(56) l2 = ((((3) != (p0))) | (((7) && (3))));
    int l3 = g0;
    l2 += rec(3, (int)(p1));
    return ((_BitInt(56))(1));
}
unsigned _BitInt(6) f2(void) {
    unsigned long l0 = g1;
    int l1 = g0;
    _BitInt(56) l2 = g0;
    int l3 = l1;
    for (int it = 0; it < 3; it++) l3 += (f3(3, ((l2) - (100)))) + it;
    g1 += l3;
    return ((((g0) && (l3))) != (((unsigned char)(l3))));
}
[[c6502::return_bool_in_carry]] bool f1(signed char p0, unsigned char p1) {
    int l0 = ((((1) / ((p0) | 1))) >> ((p1) & 7));
    if (f3(((32767) - (p1)), ((l0) << ((p1) & 7)))) l0 += 3; else l0 ^= 5;
    g0 += l0;
    l0 ^= f2();
    return ((((p1) == (p1))) >= (((1000) + (p0))));
}
[[c6502::return_bool_in_carry]] bool f0(int p0, signed char p1) {
    unsigned long l0 = ((((g1) && (g0))) > (((_BitInt(56))(p1))));
    long l1 = ((2) * (((g1) <= (2))));
    signed char l2 = l1;
    unsigned long l3 = ((((255) && (1))) < (((255) > (l1))));
    l2 ^= f1(((p1) ^ (32767)), ((p0) << ((100) & 7)));
    for (int it = 0; it < 3; it++) l1 += (f2()) + it;
    g1 += l1;
    return (~((l2) / ((p0) | 1)));
}
int main(void) {
    long r = (long)f0(156, 27);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
