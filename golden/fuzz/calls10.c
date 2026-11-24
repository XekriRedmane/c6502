_BitInt(49) g0 = 147;
signed char g1 = 52;
unsigned long f2(void) {
    long l0 = ((g1) == (g0));
    unsigned _BitInt(53) l1 = g0;
    unsigned int l2 = ((((l1) < (g0))) - ((-l1)));
    signed char l3 = ((((2) % ((l0) | 1))) == (g0));
    register unsigned char ry = (unsigned char)(l0);
    return ((((l0) & (l2))) & (((l3) & (ry))));
}
bool f1(int * pp) {
    unsigned long l0 = ((((unsigned char)(3))) <= (g0));
    _BitInt(49) l1 = ((((l0) != (g0))) != (((l0) <= (l0))));
    int l2 = ((unsigned char)(((1000) > (g0))));
    int l3 = l1;
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(((100) == (ry)));
    l3 ^= f2();
    return ((((signed char)((*pp)))) || (((l1) - (g1))));
}
long f0(signed char p0) {
    long l0 = g0;
    l0 ^= f2();
    g0 += l0;
    int box1 = (int)(l0);
    for (int it = 0; it < 3; it++) l0 += (f1(&box1)) + it;
    l0 += box1;
    g1 += l0;
    return ((((l0) << ((g0) & 7))) >= ((-p0)));
}
int main(void) {
    long r = (long)f0(209);
    r ^= (long)g0 ^ (long)g1;
    return (int)(r ^ (r >> 16));
}
