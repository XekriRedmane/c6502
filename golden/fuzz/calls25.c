unsigned char g0 = 78;
int f5(int p0, unsigned char p1, unsigned long p2, int * pp) {
    unsigned int l0 = ((((p1) <= (g0))) % ((((p1) * (p2))) | 1));
    signed char l1 = p1;
    unsigned char l2 = ((((l0) == (l0))) > (((g0) <= (g0))));
    long l3 = (!((3) << ((p1) & 7)));
    *pp += (int)(0);
    return 0;
}
unsigned int f4(signed char p0, unsigned long p1, unsigned long p2, signed char p3) {
    unsigned char l0 = ((signed char)(((p1) << ((p1) & 7))));
    long l1 = g0;
    unsigned long l2 = ((p0) * ((-p2)));
    int box5 = (int)(l0);
    int (*fp)(int, unsigned char, unsigned long, int *) = f5;
    l0 ^= fp(((g0) >> ((l0) & 7)), p2, g0, &box5);
    l0 += box5;
    g0 += l0;
    return ((l0) > (((unsigned char)(g0))));
}
long f3(unsigned char p0, long p1) {
    signed char l0 = p0;
    long l1 = ((long)((~g0)));
    signed char l2 = p0;
    signed char l3 = ((l2) & (((1) != (p0))));
    int box5 = (int)(l0);
    int (*fp)(int, unsigned char, unsigned long, int *) = f5;
    l0 ^= fp(((2) || (g0)), 7, ((3) <= (l1)), &box5);
    l0 += box5;
    g0 += l0;
    return ((((l0) * (p1))) >= (((g0) & (p1))));
}
signed char f2(unsigned long p0, unsigned long p1, unsigned char p2, long p3) {
    unsigned int l0 = ((g0) != (((p0) > (3))));
    unsigned long l1 = p0;
    signed char l2 = (((~p0)) == (((0) != (p3))));
    register unsigned char ry = (unsigned char)(l0);
    return ((((p3) / ((p0) | 1))) / ((p1) | 1));
}
unsigned int f1(void) {
    signed char l0 = (((!g0)) && (((signed char)(g0))));
    int l1 = (((-l0)) || (l0));
    int l2 = ((((32767) & (g0))) < (l1));
    l2 ^= f3(((l1) << ((l1) & 7)), ((l2) > (l0)));
    g0 += l2;
    int box5 = (int)(l0);
    int (*fp)(int, unsigned char, unsigned long, int *) = f5;
    for (int it = 0; it < 3; it++) l2 += (fp(((l1) > (l2)), ((g0) - (g0)), ((l0) + (2)), &box5)) + it;
    l2 += box5;
    return ((unsigned char)(((100) - (l1))));
}
int f0(void) {
    unsigned int l0 = ((((g0) == (g0))) << ((((g0) & (g0))) & 7));
    l0 ^= f3(l0, ((g0) / ((g0) | 1)));
    l0 ^= f2(((unsigned int)(g0)), l0, ((3) || (g0)), g0);
    return ((((l0) + (l0))) / ((((g0) + (3))) | 1));
}
int main(void) {
    long r = (long)f0();
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
