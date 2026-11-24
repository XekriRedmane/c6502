_BitInt(14) g0 = 85;
int rec(int d, int x) {
    unsigned char a = (unsigned char)(x * 7);
    long b = (long)x * 3;
    if (d <= 0) return x & 255;
    [[c6502::save_hardware_stack]] int r = rec(d - 1, (x * 3 + d) & 1023);
    return (r + a + (int)(b & 15)) & 1023;
}
[[c6502::return_bool_in_carry]] bool f3(register unsigned int p0, _BitInt(14) p1, int * pp) {
    unsigned char l0 = g0;
    int l1 = ((((l0) / ((l0) | 1))) != (((g0) * (l0))));
    int l2 = ((g0) || (255));
    *pp += (int)(((l2) != (1)));
    l1 += rec(3, (int)(((g0) > (l2))));
    return (*pp);
}
bool f2(int p0, unsigned int p1, int * pp) {
    signed char l0 = ((((255) || (g0))) != (((g0) <= (p1))));
    _BitInt(14) l1 = ((((p1) != (7))) || (100));
    int l2 = ((((l1) % ((p1) | 1))) * ((~g0)));
    long l3 = ((((l2) + (l2))) - (l2));
    *pp += (int)(((int)(g0)));
    return ((l2) * (((1000) ^ (p1))));
}
long f1(long p0) {
    unsigned long l0 = 1;
    unsigned int l1 = ((((255) == (7))) * (((3) + (p0))));
    _BitInt(22) l2 = ((g0) + (l0));
    int l3 = l0;
    register unsigned char ry = (unsigned char)(l0);
    int box2 = (int)(l0);
    if (f2(((l0) ^ (l1)), ((l3) - (l2)), &box2)) l3 += 3; else l3 ^= 5;
    l3 += box2;
    g0 += l3;
    l0 += rec(3, (int)(l2));
    return ((3) & (l2));
}
_BitInt(14) f0(register _BitInt(14) p0) {
    _BitInt(14) l0 = g0;
    _BitInt(14) l1 = g0;
    l1 += rec(3, (int)(l0));
    return ((((3) % ((p0) | 1))) | (((unsigned long)(7))));
}
int main(void) {
    long r = (long)f0(244);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
