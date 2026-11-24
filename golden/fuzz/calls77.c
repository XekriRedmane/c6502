unsigned char g0 = 61;
unsigned char g1 = 29;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
unsigned char f4(int * pp) {
    int l0 = (((~0)) <= (((g0) % ((g1) | 1))));
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(((unsigned int)(l0)));
    return 1000;
}
long f3(long p0, register unsigned int p1, int * pp) {
    unsigned long l0 = (!((unsigned int)(g0)));
    signed char l1 = p0;
    unsigned long l2 = g1;
    signed char l3 = ((3) & (p0));
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(3);
    int box4 = (int)(l0);
    l3 ^= f4(&box4);
    l3 += box4;
    return ((((1000) || (l0))) + (g1));
}
unsigned char f2(unsigned int p0, unsigned char p1, int p2, int p3, int * pp) {
    signed char l0 = p0;
    signed char l1 = (-p3);
    signed char l2 = ((((l0) << ((l0) & 7))) && (1));
    int l3 = ((l1) <= (((l0) << ((l0) & 7))));
    *pp += (int)(((7) / ((7) | 1)));
    int box4 = (int)(l0);
    for (int it = 0; it < 3; it++) l1 += (f4(&box4)) + it;
    l1 += box4;
    int box3 = (int)(l0);
    l1 ^= f3(((32767) - (l3)), ((1) << ((p2) & 7)), &box3);
    l1 += box3;
    g1 += l1;
    l0 += rec(3, (int)((((*pp)) - (p0))));
    return ((((p1) - (p1))) << (((!7)) & 7));
}
unsigned long f1(signed char p0, int p1, signed char p2, unsigned int p3, int * pp) {
    long l0 = ((long)(255));
    unsigned long l1 = p1;
    *pp += (int)(((l1) >= (l1)));
    l1 += rec(3, (int)(((l0) >> ((p3) & 7))));
    return ((((g1) << ((p3) & 7))) >> ((((p2) == (32767))) & 7));
}
bool f0(unsigned char p0) {
    unsigned int l0 = 1000;
    long l1 = ((g1) || (((l0) == (p0))));
    int l2 = ((((255) + (g1))) <= (((l1) * (0))));
    int box1 = (int)(l0);
    l0 ^= f1(((l1) > (1)), (-g0), g1, ((unsigned char)(7)), &box1);
    l0 += box1;
    int box4 = (int)(l0);
    l1 ^= f4(&box4);
    l1 += box4;
    l1 += rec(3, (int)(l1));
    return 1000;
}
int main(void) {
    long r = (long)f0(54);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
