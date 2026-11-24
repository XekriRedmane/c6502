long g0 = 39;
int g1 = 58;
unsigned long f4(long p0) {
    signed char l0 = (~((p0) && (p0)));
    unsigned int l1 = ((unsigned char)(((signed char)(p0))));
    unsigned int l2 = ((((g0) == (g1))) % ((((l0) && (g1))) | 1));
    register unsigned char ry = (unsigned char)(l0);
    return ((l1) | (((long)(0))));
}
unsigned long f3(unsigned char p0) {
    int l0 = ((p0) + (((unsigned char)(g1))));
    int l1 = ((((g0) && (g0))) && (((2) | (1000))));
    return ((p0) ^ (p0));
}
bool f2(void) {
    unsigned int l0 = g1;
    long l1 = ((g1) <= (((g1) - (l0))));
    for (int it = 0; it < 3; it++) l1 += (f4(((g0) & (g1)))) + it;
    g1 += l1;
    return g1;
}
signed char f1(register unsigned char p0) {
    int l0 = (~((g1) >= (g1)));
    unsigned char l1 = p0;
    long l2 = g0;
    unsigned char l3 = ((((l1) != (l2))) / ((((g0) + (p0))) | 1));
    for (int it = 0; it < 3; it++) l0 += (f3(((l1) <= (g0)))) + it;
    g0 += l0;
    l3 ^= f4(l0);
    return ((((32767) >= (l3))) - (((int)(l2))));
}
int f0(void) {
    signed char l0 = ((((g0) && (g0))) - (g1));
    signed char l1 = 100;
    unsigned long l2 = (!1);
    long l3 = ((((1) | (l1))) - (l0));
    if (f2()) l0 += 3; else l0 ^= 5;
    g1 += l0;
    return l2;
}
int main(void) {
    long r = (long)f0();
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
