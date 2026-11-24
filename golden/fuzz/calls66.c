unsigned _BitInt(44) g0 = 63;
unsigned _BitInt(27) g1 = 75;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
bool f2(signed char p0, unsigned char p1, unsigned _BitInt(44) p2, register int p3) {
    unsigned int l0 = ((((1) << ((p3) & 7))) >= (p3));
    register unsigned char ry = (unsigned char)(l0);
    return ((((g1) & (p3))) + (g0));
}
bool f1(register int p0, unsigned _BitInt(44) p1) {
    unsigned _BitInt(27) l0 = ((((p0) / ((g0) | 1))) || (p1));
    int l1 = (!((g1) * (3)));
    signed char l2 = ((g0) >= (g0));
    return l2;
}
unsigned char f0(int p0, unsigned _BitInt(27) p1, int p2) {
    unsigned _BitInt(44) l0 = p1;
    unsigned int l1 = ((((p0) << ((p1) & 7))) < (((p1) < (p0))));
    if (f2(((1) && (g0)), ((1) >= (p1)), ((g1) <= (g1)), g0)) l0 += 3; else l0 ^= 5;
    if (f1(((p1) || (l1)), ((p2) * (p2)))) l1 += 3; else l1 ^= 5;
    l1 += rec(3, (int)(((255) == (32767))));
    return ((((p0) && (32767))) & (((0) % ((l0) | 1))));
}
int main(void) {
    long r = (long)f0(205, 162, 138);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
