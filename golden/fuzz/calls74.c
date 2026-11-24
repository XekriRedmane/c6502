signed char g0 = 79;
unsigned char g1 = 55;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
signed char f2(unsigned char p0) {
    unsigned long l0 = ((((p0) <= (p0))) == (((g1) >= (p0))));
    unsigned int l1 = (~1000);
    l0 += rec(3, (int)(((p0) >> ((l1) & 7))));
    return ((3) * (((l0) < (3))));
}
bool f1(void) {
    long l0 = (~((g1) >> ((g0) & 7)));
    for (int it = 0; it < 3; it++) l0 += (f2(0)) + it;
    g0 += l0;
    return ((((g1) >= (l0))) ^ (g0));
}
[[c6502::return_bool_in_carry]] bool f0(int p0, unsigned int p1, unsigned char p2, int p3) {
    unsigned long l0 = (~p1);
    signed char l1 = ((signed char)(((p1) ^ (g0))));
    unsigned int l2 = 2;
    signed char l3 = l0;
    register unsigned char ry = (unsigned char)(l0);
    return ((((l2) | (g1))) != (l2));
}
int main(void) {
    long r = (long)f0(276, 105, 122, 227);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
