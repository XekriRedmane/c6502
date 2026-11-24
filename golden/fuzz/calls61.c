unsigned int g0 = 82;
int rec(int d, int x) {
    unsigned char a = (unsigned char)(x * 7);
    long b = (long)x * 3;
    if (d <= 0) return x & 255;
    [[c6502::save_hardware_stack]] int r = rec(d - 1, (x * 3 + d) & 1023);
    return (r + a + (int)(b & 15)) & 1023;
}
[[c6502::return_bool_in_carry]] bool f5(long p0, _BitInt(16) p1, register _BitInt(16) p2, unsigned int p3, int * pp) {
    _BitInt(16) l0 = ((((0) | (100))) / ((((32767) * (p3))) | 1));
    *pp += (int)(((l0) | (l0)));
    l0 += rec(3, (int)(((p3) ^ ((*pp)))));
    return p3;
}
bool f4(register _BitInt(2) p0, unsigned long p1) {
    long l0 = p0;
    int l1 = (~((_BitInt(16))(p1)));
    int box5 = (int)(l0);
    if (f5(((32767) % ((p0) | 1)), ((l0) - (p1)), l0, ((32767) >> ((l0) & 7)), &box5)) l0 += 3; else l0 ^= 5;
    l0 += box5;
    return p1;
}
int f3(_BitInt(16) p0) {
    int l0 = ((255) << (((~2)) & 7));
    _BitInt(2) l1 = g0;
    unsigned char l2 = ((l1) == (p0));
    for (int it = 0; it < 3; it++) l2 += (f4(p0, l2)) + it;
    l0 += rec(3, (int)(((l2) | (l0))));
    return ((((long)(g0))) & (((l2) < (g0))));
}
bool f2(unsigned long p0, int * pp) {
    signed char l0 = ((255) & (((p0) * (g0))));
    *pp += (int)(((255) + (7)));
    l0 += rec(3, (int)(((p0) + (100))));
    return ((p0) ^ (((p0) & (0))));
}
bool f1(unsigned long p0) {
    int l0 = ((((unsigned int)(p0))) + (((signed char)(p0))));
    unsigned int l1 = ((p0) % ((g0) | 1));
    l1 += rec(3, (int)(((l0) * (g0))));
    return ((((l0) / ((2) | 1))) ^ (l0));
}
[[c6502::return_bool_in_carry]] bool f0(unsigned long p0, unsigned long p1, register unsigned char p2) {
    _BitInt(2) l0 = ((signed char)(((p2) > (100))));
    unsigned long l1 = p1;
    long l2 = ((((p1) == (g0))) << ((((p0) - (p0))) & 7));
    if (f4(((g0) << ((255) & 7)), ((p1) & (l0)))) l1 += 3; else l1 ^= 5;
    g0 += l1;
    int box5 = (int)(l0);
    l0 ^= f5(((p1) * (255)), (!1000), ((p0) >= (p0)), ((p0) | (p0)), &box5);
    l0 += box5;
    g0 += l0;
    l1 += rec(3, (int)(255));
    return p1;
}
int main(void) {
    long r = (long)f0(157, 128, 217);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
