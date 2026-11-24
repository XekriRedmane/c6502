long g0 = 182;
unsigned int g1 = 183;
int f3(unsigned int p0, int * pp) {
    int l0 = g1;
    *pp += (int)(((g0) != (l0)));
    return g0;
}
bool f2(int * pp) {
    signed char l0 = g0;
    unsigned int l1 = ((g1) || (3));
    *pp += (int)(((l1) != (2)));
    int box3 = (int)(l0);
    l0 ^= f3(l1, &box3);
    l0 += box3;
    g0 += l0;
    return ((((3) != ((*pp)))) == (((32767) << ((g1) & 7))));
}
bool f1(register int p0) {
    signed char l0 = ((int)(((int)(g0))));
    unsigned char l1 = ((((100) & (g0))) + (((p0) >= (l0))));
    unsigned long l2 = ((((g0) - (g1))) && (p0));
    long l3 = ((unsigned long)(0));
    int box2 = (int)(l0);
    if (f2(&box2)) l0 += 3; else l0 ^= 5;
    l0 += box2;
    int box3 = (int)(l0);
    l2 ^= f3(((l2) % ((32767) | 1)), &box3);
    l2 += box3;
    g1 += l2;
    return ((signed char)(((l0) || (l2))));
}
unsigned long f0(void) {
    int l0 = g1;
    int l1 = g1;
    unsigned char l2 = ((((g1) == (l0))) & (((l1) != (g1))));
    long l3 = l1;
    register unsigned char ry = (unsigned char)(l0);
    int box2 = (int)(l0);
    if (f2(&box2)) l1 += 3; else l1 ^= 5;
    l1 += box2;
    return ((((l1) - (g0))) > (0));
}
int main(void) {
    long r = (long)f0();
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
