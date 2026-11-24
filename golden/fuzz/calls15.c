signed char g0 = 40;
unsigned int f3(_BitInt(7) p0, unsigned int p1) {
    _BitInt(9) l0 = ((((0) & (g0))) / ((((0) % ((p1) | 1))) | 1));
    unsigned char l1 = ((unsigned int)(((p1) * (g0))));
    long l2 = ((((l0) != (g0))) <= (l0));
    return l2;
}
[[c6502::return_bool_in_carry]] bool f2(unsigned long p0) {
    long l0 = p0;
    unsigned int (*fp)(_BitInt(7), unsigned int) = f3;
    for (int it = 0; it < 3; it++) l0 += (fp((~g0), p0)) + it;
    return ((((l0) > (255))) == (7));
}
unsigned long f1(unsigned long p0, long p1, _BitInt(7) p2) {
    signed char l0 = ((((_BitInt(7))(p2))) < (((unsigned long)(g0))));
    unsigned int l1 = ((((p2) && (2))) % (((~255)) | 1));
    unsigned long l2 = l1;
    l0 ^= f2(((p0) + (l2)));
    g0 += l0;
    unsigned int (*fp)(_BitInt(7), unsigned int) = f3;
    for (int it = 0; it < 3; it++) l2 += (fp(((l0) > (p2)), ((l1) == (l0)))) + it;
    return ((((g0) % ((p2) | 1))) * (((g0) / ((3) | 1))));
}
signed char f0(unsigned long p0, int * pp) {
    unsigned int l0 = ((_BitInt(7))(((p0) > (p0))));
    unsigned int l1 = ((((g0) == (1000))) == (((g0) + (3))));
    unsigned long l2 = ((p0) == (((l1) <= (7))));
    *pp += (int)(((g0) == (l0)));
    return ((((g0) && (l2))) == ((~l0)));
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(130, &mbox);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
