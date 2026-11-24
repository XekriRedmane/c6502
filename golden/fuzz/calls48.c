long g0 = 182;
signed char f4(register unsigned int p0) {
    unsigned long l0 = p0;
    unsigned char l1 = ((((g0) != (l0))) >= (((g0) < (l0))));
    signed char l2 = ((((l1) % ((l0) | 1))) % ((((g0) ^ (2))) | 1));
    register unsigned char ry = (unsigned char)(l0);
    return ((((l2) >= (255))) + (((l1) / ((p0) | 1))));
}
long f3(register unsigned char p0, unsigned int p1, register unsigned char p2, signed char p3, int * pp) {
    int l0 = ((((1000) * (p3))) - (((g0) > (p1))));
    unsigned int l1 = ((p2) / ((((l0) < (p3))) | 1));
    unsigned long l2 = p1;
    *pp += (int)((-l0));
    for (int it = 0; it < 3; it++) l2 += (f4(((0) <= (p1)))) + it;
    g0 += l2;
    return (!((unsigned int)((*pp))));
}
unsigned long f2(unsigned char p0) {
    unsigned int l0 = ((((g0) / ((g0) | 1))) < (p0));
    unsigned int l1 = l0;
    unsigned char l2 = g0;
    long l3 = ((((long)(g0))) <= (((p0) ^ (g0))));
    register unsigned char ry = (unsigned char)(l0);
    l0 ^= f4(((ry) >> ((p0) & 7)));
    return l0;
}
unsigned char f1(unsigned long p0, int * pp) {
    long l0 = ((((g0) | (g0))) & ((-p0)));
    *pp += (int)(((p0) != (g0)));
    return ((((p0) >> ((1) & 7))) < (((l0) << ((2) & 7))));
}
long f0(unsigned long p0) {
    long l0 = ((p0) >= (100));
    long l1 = 7;
    int box3 = (int)(l0);
    l0 ^= f3(l0, ((long)(g0)), ((l0) > (l0)), l1, &box3);
    l0 += box3;
    g0 += l0;
    return ((l1) + (l1));
}
int main(void) {
    long r = (long)f0(189);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
