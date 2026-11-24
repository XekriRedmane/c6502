unsigned long g0 = 107;
unsigned long g1 = 72;
unsigned long f3(long p0, unsigned int p1, signed char p2) {
    unsigned long l0 = (!((p2) & (p1)));
    signed char l1 = ((l0) || (((g1) / ((1) | 1))));
    return ((32767) << ((((g0) <= (g0))) & 7));
}
[[c6502::return_bool_in_carry]] bool f2(unsigned long p0, int * pp) {
    signed char l0 = g0;
    *pp += (int)(g0);
    unsigned long (*fp)(long, unsigned int, signed char) = f3;
    l0 ^= fp(((1) > (g0)), ((g1) * (g0)), ((g0) ^ ((*pp))));
    g0 += l0;
    return ((((2) >= (l0))) >= (((0) - (7))));
}
bool f1(long p0, unsigned int p1, int * pp) {
    unsigned char l0 = (((-g1)) != (32767));
    signed char l1 = (((~g1)) <= (((l0) + (p1))));
    *pp += (int)(((l0) >= (p1)));
    unsigned long (*fp)(long, unsigned int, signed char) = f3;
    l0 ^= fp((*pp), (-p0), p1);
    int box2 = (int)(l0);
    l1 ^= f2(g0, &box2);
    l1 += box2;
    g0 += l1;
    return 3;
}
[[c6502::return_bool_in_carry]] bool f0(unsigned char p0, long p1, long p2, unsigned char p3) {
    long l0 = ((((p3) & (255))) % ((((100) <= (p1))) | 1));
    int box1 = (int)(l0);
    for (int it = 0; it < 3; it++) l0 += (f1(((g0) / ((1000) | 1)), ((long)(p2)), &box1)) + it;
    l0 += box1;
    int box2 = (int)(l0);
    if (f2(((p2) * (100)), &box2)) l0 += 3; else l0 ^= 5;
    l0 += box2;
    return ((((1000) == (p0))) - (((1) >= (p0))));
}
int main(void) {
    long r = (long)f0(126, 141, 207, 218);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
