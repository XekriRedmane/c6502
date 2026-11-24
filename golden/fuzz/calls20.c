unsigned char g0 = 83;
int g1 = 6;
bool f3(unsigned char p0, register unsigned int p1, unsigned int p2, int * pp) {
    signed char l0 = p1;
    *pp += (int)(g1);
    return (~((p1) || (7)));
}
unsigned long f2(register unsigned _BitInt(6) p0, unsigned char p1) {
    int l0 = (((~p1)) / ((p0) | 1));
    int box3 = (int)(l0);
    l0 ^= f3(100, ((p0) / ((2) | 1)), ((l0) < (p0)), &box3);
    l0 += box3;
    return ((((p0) % ((l0) | 1))) | (((p0) ^ (p1))));
}
[[c6502::return_bool_in_carry]] bool f1(unsigned int p0, register unsigned int p1, unsigned long p2) {
    unsigned _BitInt(6) l0 = ((((unsigned _BitInt(6))(p2))) >> ((((unsigned long)(3))) & 7));
    int l1 = ((long)(100));
    l1 ^= f2(((g1) && (p0)), g1);
    g1 += l1;
    int box3 = (int)(l0);
    if (f3(((p0) <= (p1)), ((unsigned _BitInt(64))(7)), g1, &box3)) l1 += 3; else l1 ^= 5;
    l1 += box3;
    return ((g0) ^ (100));
}
[[c6502::return_bool_in_carry]] bool f0(unsigned long p0) {
    long l0 = ((((p0) + (g0))) > (((g0) % ((p0) | 1))));
    unsigned long l1 = ((long)(((255) && (g1))));
    return ((l0) % ((((g1) || (100))) | 1));
}
int main(void) {
    long r = (long)f0(275);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
