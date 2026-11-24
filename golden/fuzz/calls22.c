signed char g0 = 156;
unsigned char f3(signed char p0, unsigned long p1, unsigned long p2, long p3, int * pp) {
    unsigned _BitInt(30) l0 = p0;
    unsigned _BitInt(30) l1 = ((((p1) <= (p2))) >= (((g0) && (l0))));
    *pp += (int)(100);
    return p0;
}
signed char f2(signed char p0, long p1, long p2, unsigned _BitInt(30) p3, int * pp) {
    unsigned int l0 = ((unsigned _BitInt(8))(((p2) | (p2))));
    signed char l1 = l0;
    *pp += (int)(((p1) % ((l1) | 1)));
    int box3 = (int)(l0);
    unsigned char (*fp)(signed char, unsigned long, unsigned long, long, int *) = f3;
    l0 ^= fp(((1000) >> ((p2) & 7)), ((p1) / ((l0) | 1)), ((0) * (l0)), ((l1) - (p2)), &box3);
    l0 += box3;
    g0 += l0;
    return ((((unsigned _BitInt(8))(p1))) | (((p2) - (l1))));
}
signed char f1(int p0, unsigned _BitInt(30) p1) {
    unsigned _BitInt(8) l0 = ((int)(((p0) & (p0))));
    unsigned long l1 = g0;
    int box3 = (int)(l0);
    unsigned char (*fp)(signed char, unsigned long, unsigned long, long, int *) = f3;
    l0 ^= fp(p0, p1, ((3) % ((l0) | 1)), ((0) / ((l1) | 1)), &box3);
    l0 += box3;
    return ((7) > (p1));
}
unsigned char f0(unsigned char p0, unsigned int p1) {
    unsigned _BitInt(8) l0 = ((p0) ^ (((p0) < (p1))));
    signed char l1 = ((p0) * (((p0) / ((1) | 1))));
    int l2 = g0;
    unsigned _BitInt(30) l3 = ((((l2) && (p1))) >> ((((p1) < (p0))) & 7));
    l1 ^= f1(((l0) & (2)), ((l3) || (p1)));
    int box3 = (int)(l0);
    unsigned char (*fp)(signed char, unsigned long, unsigned long, long, int *) = f3;
    l1 ^= fp(((unsigned int)(32767)), 7, ((p1) || (1)), ((p0) <= (p1)), &box3);
    l1 += box3;
    return ((l3) % ((((l2) / ((p0) | 1))) | 1));
}
int main(void) {
    long r = (long)f0(232, 40);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
