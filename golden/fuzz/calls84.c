_BitInt(2) g0 = 1;
[[c6502::return_bool_in_carry]] bool f4(register unsigned char p0, signed char p1, unsigned int p2, unsigned int p3) {
    signed char l0 = ((g0) >> (((!p2)) & 7));
    int l1 = ((((0) + (1))) && (((p0) >= (p1))));
    _BitInt(23) l2 = ((((p2) >> ((l1) & 7))) <= (((p2) ^ (g0))));
    long l3 = ((l2) / ((g0) | 1));
    register unsigned char ry = (unsigned char)(l0);
    return l2;
}
signed char f3(signed char p0) {
    long l0 = p0;
    int l1 = ((((p0) / ((1000) | 1))) + (((g0) || (g0))));
    long l2 = (!l0);
    unsigned long l3 = ((((0) == (l0))) ^ (((l0) << ((l0) & 7))));
    l3 ^= f4((!100), ((1000) >> ((1000) & 7)), 0, l3);
    return l2;
}
[[c6502::return_bool_in_carry]] bool f2(void) {
    _BitInt(2) l0 = ((((g0) + (g0))) <= (100));
    unsigned long l1 = ((((long)(l0))) / ((g0) | 1));
    long l2 = (((!0)) || (((l1) == (l0))));
    if (f4(((_BitInt(23))(l2)), g0, ((7) / ((l2) | 1)), ((255) && (g0)))) l2 += 3; else l2 ^= 5;
    return ((((l2) != (g0))) % ((l2) | 1));
}
[[c6502::return_bool_in_carry]] bool f1(register _BitInt(2) p0, _BitInt(23) p1, unsigned long p2, unsigned int p3, int * pp) {
    _BitInt(23) l0 = ((int)(p2));
    signed char l1 = ((((p0) && (l0))) ^ (((p1) & (g0))));
    unsigned long l2 = ((((l1) < (l0))) <= (((p0) / ((l0) | 1))));
    *pp += (int)(((7) >= (p2)));
    l0 ^= f3(7);
    g0 += l0;
    if (f4(((1000) <= (l0)), ((l2) <= (p3)), l1, ((p1) + (p0)))) l0 += 3; else l0 ^= 5;
    return ((((g0) && (p3))) % ((((l1) && (l1))) | 1));
}
unsigned int f0(register _BitInt(2) p0, unsigned long p1, int p2) {
    unsigned char l0 = g0;
    _BitInt(23) l1 = ((((g0) ^ (g0))) >> ((((p0) ^ (p0))) & 7));
    l1 ^= f3(((p0) >> ((p0) & 7)));
    g0 += l1;
    return p2;
}
int main(void) {
    long r = (long)f0(107, 171, 99);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
