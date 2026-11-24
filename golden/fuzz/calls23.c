signed char g0 = 151;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
unsigned int f4(signed char p0, unsigned char p1) {
    unsigned long l0 = ((g0) >= (p0));
    signed char l1 = ((p0) / ((((l0) % ((g0) | 1))) | 1));
    long l2 = ((((l1) & (l0))) & (((signed char)(l0))));
    unsigned char l3 = ((((l0) + (g0))) == (((p0) | (l1))));
    l2 += rec(3, (int)(l1));
    return ((((p1) < (1000))) ^ (((l1) * (l0))));
}
unsigned long f3(unsigned int p0, signed char p1, long p2) {
    unsigned int l0 = ((((1) < (p1))) || (((p1) < (1))));
    unsigned long l1 = ((((p2) + (g0))) + ((!3)));
    unsigned char l2 = ((((l0) || (l0))) * (p2));
    unsigned int (*fp)(signed char, unsigned char) = f4;
    l2 ^= fp(g0, ((int)(g0)));
    g0 += l2;
    l1 += rec(3, (int)(((g0) || (1000))));
    return ((((2) == (p0))) ^ (((p0) / ((7) | 1))));
}
unsigned int f2(int * pp) {
    signed char l0 = ((unsigned char)(g0));
    unsigned long l1 = ((((g0) & (g0))) >> (((~l0)) & 7));
    long l2 = ((int)(g0));
    signed char l3 = l0;
    *pp += (int)(l1);
    for (int it = 0; it < 3; it++) l1 += (f3(((l2) ^ (l3)), (((*pp)) > (2)), ((0) | (l1)))) + it;
    g0 += l1;
    unsigned int (*fp)(signed char, unsigned char) = f4;
    l3 ^= fp(l2, ((l0) >> ((0) & 7)));
    g0 += l3;
    return ((((l3) | (l2))) - (((g0) << ((l2) & 7))));
}
unsigned int f1(int * pp) {
    unsigned int l0 = ((((g0) * (g0))) % ((((g0) != (255))) | 1));
    *pp += (int)(((g0) || (g0)));
    unsigned int (*fp)(signed char, unsigned char) = f4;
    l0 ^= fp(((l0) >= (l0)), ((l0) + ((*pp))));
    for (int it = 0; it < 3; it++) l0 += (f3((!g0), ((l0) * ((*pp))), ((l0) && (0)))) + it;
    return (((((*pp)) % (((*pp)) | 1))) % ((g0) | 1));
}
bool f0(unsigned long p0, int * pp) {
    signed char l0 = ((g0) != (((p0) + (g0))));
    signed char l1 = ((((signed char)(g0))) / ((l0) | 1));
    long l2 = ((g0) * (((g0) > (l1))));
    *pp += (int)(l0);
    int box2 = (int)(l0);
    l0 ^= f2(&box2);
    l0 += box2;
    g0 += l0;
    int box1 = (int)(l0);
    for (int it = 0; it < 3; it++) l1 += (f1(&box1)) + it;
    l1 += box1;
    return ((((g0) * (g0))) > (((l1) << ((2) & 7))));
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(105, &mbox);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
