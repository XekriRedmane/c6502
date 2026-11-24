unsigned int g0 = 42;
unsigned _BitInt(42) g1 = 147;
int f5(signed char p0) {
    unsigned int l0 = g0;
    unsigned _BitInt(42) l1 = ((((g1) - (p0))) * (((1) >= (2))));
    unsigned _BitInt(42) l2 = ((unsigned char)(((l0) == (l0))));
    long l3 = ((l2) | (((g1) * (l2))));
    return ((g0) % ((l1) | 1));
}
_BitInt(26) f4(signed char p0, int * pp) {
    unsigned long l0 = g1;
    unsigned char l1 = ((((l0) > (7))) % ((g0) | 1));
    int l2 = ((((unsigned int)(g1))) ^ (((7) <= (g1))));
    unsigned char l3 = ((((l2) && (1))) != (((l0) != (g0))));
    *pp += (int)(((l3) + (32767)));
    int (*fp)(signed char) = f5;
    l3 ^= fp((((*pp)) & (l2)));
    g1 += l3;
    return l3;
}
signed char f3(long p0, unsigned int p1, unsigned char p2) {
    unsigned int l0 = ((p1) * (((g1) != (p1))));
    int (*fp)(signed char) = f5;
    l0 ^= fp(((p1) == (p0)));
    g1 += l0;
    int box4 = (int)(l0);
    for (int it = 0; it < 3; it++) l0 += (f4(((100) > (l0)), &box4)) + it;
    l0 += box4;
    g1 += l0;
    return p2;
}
bool f2(void) {
    unsigned char l0 = g1;
    unsigned long l1 = ((unsigned char)(l0));
    int box4 = (int)(l0);
    l0 ^= f4(((7) > (l0)), &box4);
    l0 += box4;
    return ((((l0) ^ (g1))) < (((unsigned int)(g0))));
}
bool f1(unsigned long p0, int * pp) {
    long l0 = 7;
    long l1 = 7;
    int l2 = ((((unsigned long)(g0))) | (((1000) - (g0))));
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(((unsigned char)(l2)));
    return ((((g0) * ((*pp)))) << ((((1) * (0))) & 7));
}
bool f0(signed char p0, unsigned int p1, signed char p2) {
    unsigned _BitInt(42) l0 = ((p0) | (((g1) != (p2))));
    unsigned long l1 = ((32767) & (p1));
    long l2 = ((((p0) * (1))) % ((((l1) & (l1))) | 1));
    long l3 = p0;
    int (*fp)(signed char) = f5;
    for (int it = 0; it < 3; it++) l1 += (fp(((unsigned long)(p1)))) + it;
    return ((p2) >> ((((l3) | (l2))) & 7));
}
int main(void) {
    long r = (long)f0(42, 178, 260);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
