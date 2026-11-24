int g0 = 94;
_BitInt(29) g1 = 160;
unsigned char g2 = 155;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
unsigned char f3(signed char p0) {
    unsigned char l0 = g1;
    _BitInt(33) l1 = ((((l0) ^ (l0))) || (((7) < (2))));
    unsigned char l2 = ((((p0) != (l1))) % ((255) | 1));
    l0 += rec(3, (int)(l2));
    return l2;
}
bool f2(void) {
    _BitInt(33) l0 = ((((g0) / ((g0) | 1))) << ((((long)(g1))) & 7));
    l0 ^= f3(g2);
    g1 += l0;
    return ((((g0) >= (g2))) || (((255) >> ((32767) & 7))));
}
bool f1(int p0) {
    signed char l0 = ((((g0) == (g1))) + (((g1) - (p0))));
    unsigned long l1 = l0;
    unsigned long l2 = g1;
    if (f2()) l2 += 3; else l2 ^= 5;
    g1 += l2;
    l1 ^= f3(2);
    return l1;
}
[[c6502::return_bool_in_carry]] bool f0(unsigned int p0, unsigned int p1, _BitInt(29) p2, _BitInt(29) p3) {
    unsigned char l0 = p2;
    unsigned int l1 = ((p3) & (((p1) < (p3))));
    long l2 = ((p2) >= (((1000) >> ((g2) & 7))));
    _BitInt(33) l3 = p3;
    l3 ^= f3(32767);
    return ((long)(((g0) * (p3))));
}
int main(void) {
    long r = (long)f0(61, 125, 23, 17);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
