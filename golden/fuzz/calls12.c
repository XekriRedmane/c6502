unsigned long g0 = 135;
unsigned long g1 = 89;
int rec(int d, int x) {
    unsigned char a = (unsigned char)(x * 7);
    long b = (long)x * 3;
    if (d <= 0) return x & 255;
    [[c6502::save_hardware_stack]] int r = rec(d - 1, (x * 3 + d) & 1023);
    return (r + a + (int)(b & 15)) & 1023;
}
int f5(signed char p0, signed char p1, signed char p2) {
    long l0 = p1;
    unsigned long l1 = 255;
    unsigned long l2 = ((((l1) <= (p0))) | (((p1) <= (p0))));
    long l3 = p2;
    l0 += rec(3, (int)(((long)(l0))));
    return ((unsigned char)(((l2) ^ (l2))));
}
unsigned char f4(void) {
    long l0 = ((((g1) & (100))) ^ (g0));
    long l1 = l0;
    long l2 = 7;
    unsigned long l3 = l2;
    return ((((l0) << ((g0) & 7))) && (((l1) >> ((g1) & 7))));
}
bool f3(signed char p0) {
    signed char l0 = ((255) - (g0));
    long l1 = ((((l0) | (l0))) + (((l0) % ((p0) | 1))));
    signed char l2 = ((((p0) >> ((g0) & 7))) != (l1));
    int (*fp)(signed char, signed char, signed char) = f5;
    for (int it = 0; it < 3; it++) l1 += (fp((!l2), ((g1) % ((g1) | 1)), ((g0) >= (l2)))) + it;
    g1 += l1;
    for (int it = 0; it < 3; it++) l1 += (f4()) + it;
    l1 += rec(3, (int)(((g0) > (g1))));
    return l0;
}
long f2(unsigned int p0) {
    unsigned char l0 = (~((g0) ^ (g1)));
    unsigned int l1 = ((signed char)(((l0) - (l0))));
    long l2 = ((((unsigned long)(g1))) << (((-l1)) & 7));
    if (f3(((l0) <= (7)))) l2 += 3; else l2 ^= 5;
    g0 += l2;
    l1 ^= f4();
    l2 += rec(3, (int)(((l2) <= (l0))));
    return ((((g1) && (g1))) % ((((l2) >> ((100) & 7))) | 1));
}
long f1(long p0, int * pp) {
    unsigned char l0 = ((((unsigned char)(3))) & (3));
    int l1 = ((unsigned int)(((g0) % ((p0) | 1))));
    long l2 = ((((32767) + (g1))) <= (((1) || (255))));
    *pp += (int)(((signed char)(100)));
    return (*pp);
}
int f0(int p0, unsigned long p1, unsigned int p2) {
    signed char l0 = ((unsigned long)(((0) != (g1))));
    l0 ^= f3(g0);
    g1 += l0;
    l0 += rec(3, (int)(((100) != (p0))));
    return ((g0) < (((p2) != (g0))));
}
int main(void) {
    long r = (long)f0(99, 274, 185);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
