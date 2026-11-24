unsigned char g0 = 118;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
unsigned char f3(void) {
    unsigned char l0 = ((g0) > (g0));
    unsigned int l1 = ((((g0) || (l0))) + (((l0) / ((g0) | 1))));
    unsigned long l2 = (-((l0) >= (g0)));
    unsigned long l3 = l2;
    l2 += rec(3, (int)(((100) * (255))));
    return ((((l3) || (l0))) > (((unsigned int)(l2))));
}
unsigned int f2(signed char p0, int p1, long p2, unsigned long p3, int * pp) {
    signed char l0 = p0;
    int l1 = ((((7) && (p3))) >= (((p2) && (p1))));
    *pp += (int)(((p3) ^ (0)));
    for (int it = 0; it < 3; it++) l1 += (f3()) + it;
    g0 += l1;
    l0 += rec(3, (int)(((2) / ((l0) | 1))));
    return ((((p3) << ((p0) & 7))) << ((l1) & 7));
}
unsigned char f1(int * pp) {
    unsigned long l0 = ((long)(((g0) % ((0) | 1))));
    unsigned long l1 = ((((g0) > (g0))) - (((0) && (l0))));
    *pp += (int)(((l1) / ((255) | 1)));
    int box2 = (int)(l0);
    for (int it = 0; it < 3; it++) l0 += (f2(((l1) | (3)), g0, ((l0) != (g0)), ((l0) - (l0)), &box2)) + it;
    l0 += box2;
    return (*pp);
}
unsigned char f0(unsigned char p0, unsigned char p1, signed char p2, int p3) {
    unsigned long l0 = ((p3) != (((signed char)(p0))));
    l0 ^= f3();
    g0 += l0;
    int box1 = (int)(l0);
    l0 ^= f1(&box1);
    l0 += box1;
    g0 += l0;
    l0 += rec(3, (int)(p3));
    return p2;
}
int main(void) {
    long r = (long)f0(254, 150, 43, 264);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
