signed char g0 = 150;
long g1 = 58;
unsigned char f2(int p0, long p1, int p2, unsigned long p3) {
    unsigned char l0 = ((((p3) == (g0))) <= (((p2) == (100))));
    unsigned char l1 = ((((g0) < (g1))) > (p0));
    long l2 = p2;
    return ((((p0) >> ((p2) & 7))) ^ (((32767) | (p0))));
}
int f1(void) {
    signed char l0 = g0;
    unsigned char l1 = ((((g1) <= (g0))) < (((g0) <= (g0))));
    int l2 = ((((g1) >> ((l1) & 7))) < (g0));
    unsigned char (*fp)(int, long, int, unsigned long) = f2;
    for (int it = 0; it < 3; it++) l2 += (fp(((255) && (g0)), ((unsigned long)(l0)), ((1) | (l2)), 2)) + it;
    return ((l2) >= (g0));
}
long f0(int * pp) {
    unsigned char l0 = g1;
    int l1 = (~((g1) * (g1)));
    *pp += (int)(((l1) && (l1)));
    unsigned char (*fp)(int, long, int, unsigned long) = f2;
    for (int it = 0; it < 3; it++) l0 += (fp(((g0) % ((255) | 1)), l1, 1, (*pp))) + it;
    return (*pp);
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(&mbox);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
