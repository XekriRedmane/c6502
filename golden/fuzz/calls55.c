int g0 = 189;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
_BitInt(61) f2(signed char p0, long p1, signed char p2, unsigned int p3) {
    unsigned int l0 = p1;
    long l1 = ((int)(((g0) & (p3))));
    unsigned int l2 = (!l1);
    _BitInt(61) l3 = ((((p1) >> ((g0) & 7))) != ((~l2)));
    return l0;
}
unsigned _BitInt(21) f1(_BitInt(61) p0, _BitInt(61) p1, _BitInt(61) p2, unsigned long p3) {
    long l0 = p3;
    _BitInt(61) (*fp)(signed char, long, signed char, unsigned int) = f2;
    l0 ^= fp(((p1) > (l0)), ((l0) | (p2)), ((g0) != (l0)), 3);
    l0 += rec(3, (int)(((p0) && (g0))));
    return l0;
}
bool f0(unsigned char p0, unsigned long p1) {
    unsigned char l0 = ((((g0) != (g0))) >= (((p0) * (p1))));
    unsigned long l1 = ((((p1) * (p0))) || (((g0) - (g0))));
    unsigned char l2 = ((((g0) >= (p0))) || (((p1) >> ((l1) & 7))));
    long l3 = ((signed char)(((unsigned char)(32767))));
    _BitInt(61) (*fp)(signed char, long, signed char, unsigned int) = f2;
    l3 ^= fp(((p1) != (1)), ((p0) * (255)), (~p0), ((p0) / ((l3) | 1)));
    l3 ^= f1(l3, ((p1) != (g0)), ((g0) && (l1)), ((p0) < (l1)));
    l2 += rec(3, (int)(((l3) / ((p0) | 1))));
    return l0;
}
int main(void) {
    long r = (long)f0(267, 284);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
