long g0 = 181;
unsigned int g1 = 94;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
bool f4(signed char p0, unsigned long p1, int * pp) {
    unsigned int l0 = ((((p1) + (p0))) != (g0));
    unsigned long l1 = 100;
    unsigned int l2 = ((((g1) / ((p1) | 1))) << ((g1) & 7));
    *pp += (int)(((32767) | (1000)));
    l2 += rec(3, (int)(((l1) || (l2))));
    return l0;
}
unsigned char f3(int p0) {
    unsigned int l0 = ((((g1) >> ((g0) & 7))) & (((1) & (p0))));
    unsigned long l1 = (-((1000) / ((p0) | 1)));
    long l2 = ((g1) >= (((g0) <= (g1))));
    l2 += rec(3, (int)(((unsigned int)(l0))));
    return ((((unsigned char)(l0))) >= (((g1) < (g1))));
}
bool f2(int p0) {
    long l0 = ((((g0) >> ((g1) & 7))) << ((g0) & 7));
    unsigned char l1 = ((((g1) / ((g1) | 1))) / (((-g1)) | 1));
    l1 ^= f3(((p0) <= (g0)));
    int box4 = (int)(l0);
    if (f4(p0, ((l1) & (g0)), &box4)) l0 += 3; else l0 ^= 5;
    l0 += box4;
    return p0;
}
unsigned char f1(long p0, signed char p1, unsigned char p2, unsigned int p3) {
    signed char l0 = ((p2) >> ((((g1) + (p1))) & 7));
    int box4 = (int)(l0);
    for (int it = 0; it < 3; it++) l0 += (f4(((g1) > (p2)), 32767, &box4)) + it;
    l0 += box4;
    return ((((p2) != (p1))) << ((((g0) % ((2) | 1))) & 7));
}
signed char f0(unsigned int p0, unsigned char p1, long p2, signed char p3) {
    int l0 = ((p3) > (((p2) > (p3))));
    signed char l1 = g1;
    unsigned int l2 = ((g0) * ((!p2)));
    unsigned int l3 = g0;
    l2 ^= f3(((100) == (l1)));
    return (((!p1)) | (1000));
}
int main(void) {
    long r = (long)f0(18, 220, 123, 244);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
