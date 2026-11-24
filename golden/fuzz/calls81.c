long g0 = 136;
unsigned int g1 = 122;
unsigned long f5(int p0, int p1, int p2) {
    long l0 = ((((signed char)(p1))) > (((g1) ^ (p0))));
    unsigned int l1 = ((unsigned long)(((p1) != (g1))));
    return 0;
}
[[c6502::return_bool_in_carry]] bool f4(unsigned char p0, unsigned long p1, unsigned int p2, int * pp) {
    long l0 = ((255) && (((g1) - (g1))));
    signed char l1 = p2;
    signed char l2 = ((p1) ^ (((p2) | (l1))));
    unsigned char l3 = ((((g1) / ((l1) | 1))) << ((g0) & 7));
    *pp += (int)((!0));
    return ((((p2) | (p0))) >= (1000));
}
unsigned long f3(unsigned char p0, unsigned int p1) {
    signed char l0 = ((((g0) & (g0))) % ((p0) | 1));
    int l1 = ((((l0) > (7))) || (((signed char)(g0))));
    register unsigned char ry = (unsigned char)(l0);
    return (-((int)(g0)));
}
[[c6502::return_bool_in_carry]] bool f2(long p0, long p1) {
    unsigned long l0 = ((p1) - (((g1) <= (g0))));
    register unsigned char ry = (unsigned char)(l0);
    int box4 = (int)(l0);
    l0 ^= f4(l0, ((ry) << ((p0) & 7)), (!l0), &box4);
    l0 += box4;
    return 1;
}
bool f1(long p0, int * pp) {
    signed char l0 = ((((7) - (p0))) <= (((signed char)(g0))));
    signed char l1 = ((g0) + (((p0) || (l0))));
    int l2 = (((~255)) >> ((l1) & 7));
    *pp += (int)(((unsigned int)(g1)));
    unsigned long (*fp)(int, int, int) = f5;
    l0 ^= fp(p0, ((1000) | ((*pp))), ((l0) - (g0)));
    return ((((100) & (l1))) ^ (((p0) + (l1))));
}
bool f0(signed char p0, int p1, int p2, unsigned long p3) {
    unsigned long l0 = ((((unsigned long)(p1))) * (((1) | (g0))));
    long l1 = p1;
    unsigned char l2 = (((~p1)) - (l0));
    return ((((l0) % ((g0) | 1))) >= (((p0) - (l1))));
}
int main(void) {
    long r = (long)f0(293, 195, 161, 294);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
