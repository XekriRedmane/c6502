unsigned int g0 = 166;
int rec(int d, int x) {
    unsigned char a = (unsigned char)(x * 7);
    long b = (long)x * 3;
    if (d <= 0) return x & 255;
    [[c6502::save_hardware_stack]] int r = rec(d - 1, (x * 3 + d) & 1023);
    return (r + a + (int)(b & 15)) & 1023;
}
bool f4(signed char p0, long p1, long p2, register unsigned int p3, int * pp) {
    long l0 = p3;
    *pp += (int)(((p1) ^ (l0)));
    l0 += rec(3, (int)(((1000) / ((p0) | 1))));
    return (~((l0) < (g0)));
}
long f3(signed char p0, long p1, signed char p2) {
    signed char l0 = ((((p2) >> ((g0) & 7))) - (((g0) != (p0))));
    unsigned char l1 = (-g0);
    unsigned char l2 = ((((7) <= (p2))) ^ ((!p1)));
    signed char l3 = ((g0) == (((p1) - (l0))));
    int box4 = (int)(l0);
    l2 ^= f4(((l2) / ((l2) | 1)), ((g0) < (p1)), (~p1), ((unsigned char)(l2)), &box4);
    l2 += box4;
    return (!((l0) < (p1)));
}
[[c6502::return_bool_in_carry]] bool f2(int * pp) {
    unsigned int l0 = (~32767);
    unsigned char l1 = ((((g0) && (l0))) ^ ((!l0)));
    long l2 = ((((3) > (l0))) == (((l0) > (l1))));
    *pp += (int)(((1) != (l0)));
    int box4 = (int)(l0);
    l1 ^= f4((*pp), ((l1) / ((g0) | 1)), ((l2) < (255)), 255, &box4);
    l1 += box4;
    l2 ^= f3(((l0) % ((l2) | 1)), ((l1) % (((*pp)) | 1)), g0);
    g0 += l2;
    return ((l0) && (((l0) == (l1))));
}
unsigned char f1(int * pp) {
    long l0 = g0;
    unsigned int l1 = (((!g0)) / ((l0) | 1));
    long l2 = g0;
    signed char l3 = ((((l0) && (l2))) | (((32767) || (7))));
    *pp += (int)(l2);
    int box4 = (int)(l0);
    if (f4(((l0) % ((g0) | 1)), ((l1) ^ (1000)), ((unsigned long)(2)), (~l3), &box4)) l2 += 3; else l2 ^= 5;
    l2 += box4;
    g0 += l2;
    int box2 = (int)(l0);
    if (f2(&box2)) l2 += 3; else l2 ^= 5;
    l2 += box2;
    g0 += l2;
    l0 += rec(3, (int)(((l2) >> ((l3) & 7))));
    return ((unsigned char)(((1) - (l1))));
}
[[c6502::return_bool_in_carry]] bool f0(long p0, register signed char p1) {
    unsigned int l0 = ((((g0) < (g0))) & (g0));
    unsigned int l1 = ((g0) << ((1) & 7));
    int box4 = (int)(l0);
    l0 ^= f4(((g0) && (p0)), ((7) >> ((p1) & 7)), ((l0) << ((p1) & 7)), ((signed char)(p0)), &box4);
    l0 += box4;
    g0 += l0;
    return 255;
}
int main(void) {
    long r = (long)f0(253, 279);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
