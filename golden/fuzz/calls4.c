signed char g0 = 184;
unsigned int g1 = 122;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
int f3(signed char p0, long p1) {
    int l0 = ((((g0) ^ (p1))) || (((p0) && (g0))));
    return ((p0) || (((g0) < (g1))));
}
signed char f2(int p0, unsigned char p1, int * pp) {
    int l0 = p1;
    int l1 = ((p1) * (g0));
    *pp += (int)(((g0) < (g0)));
    int (*fp)(signed char, long) = f3;
    for (int it = 0; it < 3; it++) l1 += (fp((((*pp)) * ((*pp))), g1)) + it;
    g0 += l1;
    return ((int)((*pp)));
}
int f1(signed char p0) {
    long l0 = 255;
    unsigned char l1 = g0;
    long l2 = (~g0);
    int (*fp)(signed char, long) = f3;
    for (int it = 0; it < 3; it++) l1 += (fp((!l0), ((l0) >= (g0)))) + it;
    return ((g0) / ((((g1) < (p0))) | 1));
}
unsigned int f0(int p0, signed char p1, unsigned char p2, long p3) {
    unsigned int l0 = ((((2) != (g1))) < (((32767) << ((g1) & 7))));
    return ((int)(((p0) % ((g0) | 1))));
}
int main(void) {
    long r = (long)f0(30, 85, 237, 291);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
