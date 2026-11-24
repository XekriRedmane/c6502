_BitInt(44) g0 = 94;
bool f4(void) {
    _BitInt(33) l0 = ((g0) | (((g0) >> ((g0) & 7))));
    _BitInt(33) l1 = g0;
    int l2 = ((l1) | (((l1) && (255))));
    unsigned char l3 = ((g0) <= (((unsigned char)(l0))));
    return (-((int)(l0)));
}
_BitInt(33) f3(void) {
    unsigned long l0 = ((g0) ^ (g0));
    unsigned long l1 = ((l0) < (g0));
    return ((((l0) != (l1))) - (l1));
}
[[c6502::return_bool_in_carry]] bool f2(unsigned int p0, long p1, _BitInt(44) p2) {
    _BitInt(44) l0 = ((p0) & (32767));
    l0 ^= f4();
    g0 += l0;
    return (((!p1)) == (((p2) * (g0))));
}
[[c6502::return_bool_in_carry]] bool f1(_BitInt(33) p0, _BitInt(44) p1, register signed char p2, unsigned int p3, int * pp) {
    unsigned long l0 = ((p1) >= (p3));
    *pp += (int)(p1);
    if (f4()) l0 += 3; else l0 ^= 5;
    return (((((*pp)) <= (1000))) || (((p0) && (p2))));
}
int f0(register int p0, unsigned long p1, _BitInt(44) p2) {
    _BitInt(33) l0 = ((p2) & (((_BitInt(44))(g0))));
    unsigned long l1 = 1000;
    long l2 = (-((p1) == (p1)));
    return (!((p0) | (g0)));
}
int main(void) {
    long r = (long)f0(18, 168, 105);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
