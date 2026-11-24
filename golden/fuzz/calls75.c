unsigned int g0 = 116;
signed char g1 = 186;
long g2 = 172;
unsigned char f5(unsigned int p0, long p1, int * pp) {
    int l0 = p1;
    unsigned int l1 = ((((1000) & (g2))) < (((g1) || (g0))));
    unsigned char l2 = ((((g0) && (g2))) || (((g1) && (p0))));
    unsigned int l3 = ((((2) <= (0))) && (((g1) % ((g1) | 1))));
    *pp += (int)(l2);
    return ((((l0) > (1))) != (g2));
}
long f4(unsigned long p0, unsigned int p1, unsigned long p2, unsigned char p3, int * pp) {
    unsigned char l0 = (!((0) && (100)));
    unsigned int l1 = 100;
    unsigned char l2 = ((((p3) - (g0))) < (p3));
    unsigned char l3 = ((g1) | (((l1) > (p0))));
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(((255) != (g0)));
    int box5 = (int)(l0);
    for (int it = 0; it < 3; it++) l3 += (f5(((ry) > (p2)), ((7) && (l3)), &box5)) + it;
    l3 += box5;
    return ((l1) % ((p2) | 1));
}
unsigned int f3(void) {
    unsigned int l0 = ((g1) >= (g2));
    int box5 = (int)(l0);
    l0 ^= f5(((long)(g1)), ((g0) || (g0)), &box5);
    l0 += box5;
    return ((((g1) & (g2))) / ((g1) | 1));
}
[[c6502::return_bool_in_carry]] bool f2(long p0, unsigned long p1) {
    unsigned long l0 = ((((p0) <= (g0))) > (7));
    long l1 = ((unsigned int)(l0));
    int box4 = (int)(l0);
    l0 ^= f4(2, ((255) && (l0)), p1, ((int)(g0)), &box4);
    l0 += box4;
    g1 += l0;
    return ((unsigned char)(((p1) & (100))));
}
[[c6502::return_bool_in_carry]] bool f1(int p0, register unsigned int p1) {
    int l0 = 32767;
    unsigned char l1 = ((((g1) + (g1))) % ((((p0) | (g1))) | 1));
    int l2 = 32767;
    register unsigned char ry = (unsigned char)(l0);
    return g1;
}
unsigned long f0(signed char p0, unsigned long p1) {
    long l0 = ((((g2) + (1))) | (((p1) << ((p1) & 7))));
    unsigned long l1 = ((((p0) % ((g0) | 1))) <= (l0));
    signed char l2 = g1;
    long l3 = 1;
    register unsigned char ry = (unsigned char)(l0);
    return ((((l2) != (g1))) % ((g2) | 1));
}
int main(void) {
    long r = (long)f0(34, 145);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
