long g0 = 45;
int g1 = 174;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
int f3(int * pp) {
    signed char l0 = (-((g0) < (g0)));
    unsigned _BitInt(26) l1 = ((((3) * (l0))) && (((1) > (l0))));
    _BitInt(60) l2 = l1;
    *pp += (int)(((l2) && (g1)));
    return (*pp);
}
bool f2(_BitInt(60) p0, unsigned char p1) {
    unsigned int l0 = p1;
    unsigned long l1 = ((p1) != (((p1) < (l0))));
    long l2 = ((((1000) >> ((g1) & 7))) << (((-l1)) & 7));
    int box3 = (int)(l0);
    l1 ^= f3(&box3);
    l1 += box3;
    g0 += l1;
    l0 += rec(3, (int)(g0));
    return ((((p0) == (p1))) - ((-l1)));
}
bool f1(void) {
    unsigned long l0 = ((signed char)(255));
    unsigned _BitInt(26) l1 = g0;
    unsigned char l2 = ((((g0) && (l0))) == (((3) + (l1))));
    unsigned char l3 = ((((1) | (l0))) << ((((7) + (l0))) & 7));
    if (f2(((g0) + (l0)), ((unsigned char)(7)))) l0 += 3; else l0 ^= 5;
    g0 += l0;
    int box3 = (int)(l0);
    l3 ^= f3(&box3);
    l3 += box3;
    l3 += rec(3, (int)(((g0) >= (g1))));
    return ((((l1) - (g0))) | (((l0) == (7))));
}
[[c6502::return_bool_in_carry]] bool f0(_BitInt(60) p0, unsigned char p1, unsigned int p2) {
    unsigned long l0 = ((((p2) > (p1))) == (((p1) != (p2))));
    unsigned long l1 = ((unsigned _BitInt(26))(((p2) % ((p0) | 1))));
    signed char l2 = ((((unsigned _BitInt(26))(g1))) ^ (((2) * (g1))));
    unsigned char l3 = ((((l1) > (255))) == (((p2) <= (255))));
    l0 += rec(3, (int)(((2) | (p1))));
    return l3;
}
int main(void) {
    long r = (long)f0(299, 87, 19);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
