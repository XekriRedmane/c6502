unsigned long g0 = 164;
unsigned int f3(void) {
    int l0 = 7;
    unsigned char l1 = ((((l0) ^ (g0))) != (1));
    return ((100) ^ ((-l0)));
}
signed char f2(void) {
    long l0 = ((((g0) % ((g0) | 1))) - (((g0) >> ((g0) & 7))));
    signed char l1 = g0;
    unsigned int l2 = 7;
    unsigned long l3 = ((((l1) % ((2) | 1))) >= (l1));
    register unsigned char ry = (unsigned char)(l0);
    return ((ry) >= (((l3) > (0))));
}
bool f1(long p0, register unsigned int p1, int p2) {
    signed char l0 = ((g0) % ((((p2) <= (p1))) | 1));
    long l1 = (-p0);
    unsigned long l2 = l1;
    unsigned char l3 = ((((l0) >> ((2) & 7))) + (g0));
    return g0;
}
[[c6502::return_bool_in_carry]] bool f0(unsigned int p0, int p1, register int p2) {
    unsigned char l0 = p2;
    unsigned int l1 = (!((32767) <= (p0)));
    if (f1(((p2) < (p0)), ((p1) ^ (100)), ((long)(32767)))) l1 += 3; else l1 ^= 5;
    for (int it = 0; it < 3; it++) l1 += (f2()) + it;
    g0 += l1;
    return ((((unsigned int)(p1))) >= (((p2) <= (g0))));
}
int main(void) {
    long r = (long)f0(56, 143, 5);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
