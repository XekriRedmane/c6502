signed char g0 = 57;
unsigned long g1 = 122;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
_BitInt(41) f2(long p0, int p1, int p2, int p3) {
    unsigned int l0 = ((((g1) <= (p0))) - (g0));
    _BitInt(41) l1 = ((unsigned long)(p2));
    l1 += rec(3, (int)(p2));
    return g0;
}
int f1(unsigned long p0, _BitInt(62) p1, _BitInt(62) p2, unsigned char p3) {
    signed char l0 = p1;
    signed char l1 = ((g1) - (g1));
    l0 ^= f2(((l1) * (p2)), ((p2) ^ (l0)), (-p0), ((unsigned char)(l1)));
    return g1;
}
_BitInt(62) f0(void) {
    _BitInt(41) l0 = ((g1) >= (((g0) && (1000))));
    unsigned char l1 = ((((signed char)(g0))) | (((g0) == (l0))));
    signed char l2 = 100;
    l0 ^= f2(((g1) || (g0)), ((l0) != (l2)), ((1000) + (2)), l2);
    for (int it = 0; it < 3; it++) l0 += (f1(l1, 100, ((l2) | (3)), (~l0))) + it;
    g1 += l0;
    return 2;
}
int main(void) {
    long r = (long)f0();
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
