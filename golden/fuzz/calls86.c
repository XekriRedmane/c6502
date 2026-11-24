unsigned long g0 = 190;
signed char g1 = 196;
unsigned char g2 = 181;
signed char f2(unsigned _BitInt(17) p0) {
    _BitInt(58) l0 = 100;
    unsigned _BitInt(17) l1 = ((p0) >= (((g1) >= (g2))));
    long l2 = g2;
    return ((((int)(p0))) ^ (g2));
}
unsigned char f1(_BitInt(58) p0, signed char p1, unsigned int p2, int * pp) {
    signed char l0 = ((_BitInt(58))(((p2) <= (p2))));
    *pp += (int)(((g2) & (g1)));
    for (int it = 0; it < 3; it++) l0 += (f2(((unsigned char)(7)))) + it;
    g1 += l0;
    return ((l0) + (((p1) >= (p1))));
}
bool f0(void) {
    signed char l0 = ((((3) % ((7) | 1))) & (((g0) > (g2))));
    unsigned long l1 = 32767;
    signed char l2 = g2;
    for (int it = 0; it < 3; it++) l0 += (f2(((g1) * (g2)))) + it;
    return (!l1);
}
int main(void) {
    long r = (long)f0();
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
