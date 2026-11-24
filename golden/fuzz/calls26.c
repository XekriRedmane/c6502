unsigned char g0 = 110;
long g1 = 139;
signed char g2 = 32;
long f3(unsigned int p0, int p1, unsigned char p2) {
    int l0 = ((((7) <= (g1))) - (((p0) || (g1))));
    signed char l1 = ((((g1) - (g0))) * (((p0) ^ (g2))));
    unsigned char l2 = ((((l0) <= (l1))) & (((p0) - (l1))));
    return l0;
}
signed char f2(register unsigned char p0, int * pp) {
    unsigned char l0 = p0;
    signed char l1 = ((long)(g2));
    signed char l2 = ((((l1) / ((p0) | 1))) <= (((g0) <= (32767))));
    long l3 = ((((l1) <= (g0))) < (g1));
    *pp += (int)(p0);
    l3 ^= f3(((p0) % ((g0) | 1)), (-32767), ((0) / (((*pp)) | 1)));
    g0 += l3;
    return l0;
}
unsigned long f1(unsigned long p0, register unsigned char p1, unsigned long p2) {
    unsigned long l0 = ((((p2) != (7))) & (((p0) >> ((g1) & 7))));
    long l1 = 7;
    int box2 = (int)(l0);
    l0 ^= f2(((g2) >> ((l1) & 7)), &box2);
    l0 += box2;
    l1 ^= f3(((1) > (p1)), g0, ((p1) * (l1)));
    g0 += l1;
    return ((((g2) - (1000))) != (((l1) == (l1))));
}
signed char f0(long p0, register unsigned char p1, unsigned long p2, unsigned int p3) {
    int l0 = g1;
    int box2 = (int)(l0);
    l0 ^= f2(((p3) & (p1)), &box2);
    l0 += box2;
    g1 += l0;
    l0 ^= f3(((p2) != (g0)), ((signed char)(32767)), (!g1));
    return ((((unsigned long)(g2))) & (100));
}
int main(void) {
    long r = (long)f0(178, 187, 123, 71);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
