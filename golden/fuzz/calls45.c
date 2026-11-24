unsigned int g0 = 65;
signed char g1 = 77;
int rec(int d, int x) {
    unsigned char a = (unsigned char)(x * 7);
    long b = (long)x * 3;
    if (d <= 0) return x & 255;
    [[c6502::save_hardware_stack]] int r = rec(d - 1, (x * 3 + d) & 1023);
    return (r + a + (int)(b & 15)) & 1023;
}
unsigned long f4(signed char p0, long p1, unsigned int p2, int * pp) {
    long l0 = ((((p0) >= (g1))) % ((g0) | 1));
    long l1 = ((int)(((p0) && (g1))));
    int l2 = ((((signed char)(1))) << ((((p2) < (p1))) & 7));
    *pp += (int)(((g0) + (p2)));
    return ((((p1) % ((1) | 1))) <= (((l2) << ((255) & 7))));
}
unsigned long f3(unsigned char p0) {
    unsigned int l0 = p0;
    signed char l1 = ((((l0) <= (3))) ^ (((l0) - (l0))));
    long l2 = p0;
    signed char l3 = ((((32767) - (255))) ^ (((l2) <= (0))));
    int box4 = (int)(l0);
    unsigned long (*fp)(signed char, long, unsigned int, int *) = f4;
    l0 ^= fp(((int)(l2)), l0, ((unsigned long)(g0)), &box4);
    l0 += box4;
    return ((l2) * (g0));
}
long f2(int * pp) {
    unsigned char l0 = ((((g0) <= (g1))) - (((g0) || (g0))));
    *pp += (int)(g1);
    return ((((g0) % ((100) | 1))) == (((g1) && ((*pp)))));
}
signed char f1(unsigned long p0, unsigned long p1, int * pp) {
    signed char l0 = ((((p1) % ((g1) | 1))) - (((g0) > (p0))));
    *pp += (int)(g1);
    int box2 = (int)(l0);
    l0 ^= f2(&box2);
    l0 += box2;
    g1 += l0;
    int box4 = (int)(l0);
    unsigned long (*fp)(signed char, long, unsigned int, int *) = f4;
    l0 ^= fp(((p1) >> ((p0) & 7)), ((l0) & (l0)), ((p1) <= (1)), &box4);
    l0 += box4;
    l0 += rec(3, (int)(g1));
    return ((((l0) <= ((*pp)))) << ((p0) & 7));
}
signed char f0(void) {
    int l0 = g0;
    int box2 = (int)(l0);
    l0 ^= f2(&box2);
    l0 += box2;
    g0 += l0;
    l0 += rec(3, (int)(g1));
    return g1;
}
int main(void) {
    long r = (long)f0();
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
