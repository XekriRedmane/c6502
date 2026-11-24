long g0 = 155;
int g1 = 22;
unsigned int f2(long p0, long p1) {
    unsigned int l0 = ((((g0) >= (p1))) / ((((g0) <= (g1))) | 1));
    unsigned long l1 = ((g0) < (((g1) % ((l0) | 1))));
    unsigned long l2 = ((((3) + (p0))) - (((g1) - (p0))));
    unsigned long l3 = ((unsigned long)(((32767) & (p1))));
    return ((l3) - (g0));
}
signed char f1(unsigned long p0, unsigned char p1, register int p2, int * pp) {
    unsigned char l0 = 32767;
    unsigned char l1 = ((((long)(l0))) - (g0));
    *pp += (int)(((g0) - (g0)));
    return ((((p2) <= ((*pp)))) ^ ((((*pp)) ^ (1000))));
}
unsigned int f0(void) {
    unsigned char l0 = ((((1000) >> ((g1) & 7))) < (3));
    unsigned char l1 = l0;
    unsigned char l2 = (-0);
    int l3 = 255;
    unsigned int (*fp)(long, long) = f2;
    l0 ^= fp(l0, ((g1) & (l3)));
    g0 += l0;
    return ((l0) >= (((l3) < (l3))));
}
int main(void) {
    long r = (long)f0();
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
