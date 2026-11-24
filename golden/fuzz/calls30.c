signed char g0 = 159;
unsigned long g1 = 53;
int g2 = 12;
int rec(int d, int x) {
    unsigned char a = (unsigned char)(x * 7);
    long b = (long)x * 3;
    if (d <= 0) return x & 255;
    [[c6502::save_hardware_stack]] int r = rec(d - 1, (x * 3 + d) & 1023);
    return (r + a + (int)(b & 15)) & 1023;
}
signed char f4(unsigned int p0, int p1, unsigned char p2, register int p3) {
    unsigned int l0 = ((((2) & (g2))) && (((p1) >> ((p3) & 7))));
    return ((((1000) & (p2))) - (((p3) || (g1))));
}
[[c6502::return_bool_in_carry]] bool f3(signed char p0, unsigned long p1, int p2, unsigned long p3) {
    unsigned long l0 = ((p0) > (g0));
    int l1 = ((32767) | (0));
    unsigned int l2 = ((p1) != (((p0) + (100))));
    unsigned int l3 = ((((p1) <= (l0))) / ((p1) | 1));
    l1 ^= f4(g1, l2, 100, ((p2) % ((p3) | 1)));
    return p2;
}
signed char f2(register signed char p0, register unsigned char p1, long p2) {
    int l0 = 7;
    signed char l1 = (((~p0)) | (((p0) == (g1))));
    l0 ^= f4(g1, g2, l1, ((p1) * (p0)));
    g0 += l0;
    l1 += rec(3, (int)(((l0) / ((p0) | 1))));
    return ((255) >= (l0));
}
unsigned char f1(long p0, unsigned int p1, unsigned long p2, int p3) {
    int l0 = ((unsigned int)(((p2) < (g1))));
    unsigned long l1 = p2;
    long l2 = ((p1) ^ (l0));
    return ((((p1) ^ (p0))) >= (l1));
}
signed char f0(register signed char p0, long p1, register unsigned char p2, int * pp) {
    unsigned char l0 = ((((2) > (p1))) > (((p1) | (2))));
    long l1 = ((((32767) ^ (p0))) / ((((g0) + (p2))) | 1));
    unsigned int l2 = l1;
    *pp += (int)((-g1));
    l1 += rec(3, (int)(((p2) >= (g1))));
    return ((((signed char)(p0))) >= (((g2) & (g2))));
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(41, 81, 221, &mbox);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
