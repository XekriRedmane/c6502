// expect: 3309
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned _BitInt(42) g1 = ((unsigned _BitInt(42))2147483648ULL);
long g2 = ((long)65535ULL);
long g3 = ((long)32768ULL);
unsigned char ga4[4] = {((unsigned char)0ULL), ((unsigned char)1ULL), ((unsigned char)255ULL), ((unsigned char)0ULL)};
struct S_gs5 { unsigned char pad; unsigned _BitInt(42) m0; _BitInt(26) m1; };
struct S_gs5 gs5 = {0, ((unsigned _BitInt(42))256ULL), ((_BitInt(26))1ULL)};
unsigned _BitInt(42) *gp6 = &g1;
long *gp7 = &g3;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(ga4[((int)0ULL)]);
}
void fin1(void) {
    mix(ga4[((int)1ULL)]);
    mix(ga4[((int)2ULL)]);
    mix(ga4[((int)3ULL)]);
    mix(gs5.m0);
}
void fin2(void) {
    mix(gs5.m1);
}
unsigned char f8(unsigned long *pp9) {
    static unsigned _BitInt(42) s12 = ((unsigned _BitInt(42))1465092931965ULL);
    unsigned char l10 = ((unsigned char)255ULL);
    unsigned char l11 = ((unsigned char)2ULL);
    unsigned char w13;
    w13 = 0;
    while (w13++ < 2) {
        mix(((unsigned int)(((unsigned _BitInt(42))2147483648ULL))));
    }
    s12 = ((l10 &= 1) ^ (*gp7));
    ga4[((int)2ULL)] = (g2 *= 3);
    return ((long)32767ULL);
}
long f14(void) {
    static signed char s18 = ((signed char)128ULL);
    unsigned char l15 = ((unsigned char)0ULL);
    int l16 = ((int)0ULL);
    unsigned long l17 = ((unsigned long)256ULL);
    s18 = (l17 | (ga4[((int)2ULL)] <<= 6));
    (gs5.m0 >>= 1);
    for (unsigned char i19 = 0; i19 < 4; i19++) {
        s18 = (++ga4[(i19 & ((int)3ULL))]);
        g2 = i19;
    }
    return ((int)65535ULL);
}
unsigned int f20(int p21, unsigned char *pp22) {
    unsigned char l23 = ((unsigned char)3ULL);
    unsigned long l24 = ((unsigned long)255ULL);
    gs5.m0 = ((l23 >>= 2) + ((unsigned long)65535ULL));
    (gs5.m0 ^= 90);
    switch ((((int)((gs5.m0 ^ (gs5.m1 ^= 3)))) & ((int)7ULL))) {
    case 6:
        gs5.m1 = (++p21);
        g2 = (l24 >>= 2);
    case 7:
        break;
    default:
        mix((p21 + gs5.m1));
        if ((--g3)) {
            g2 = ((((unsigned char)128ULL) >> 6) ? (g3 & g3) : (gs5.m1 << 5));
        }
    }
    mix(gs5.m0);
    l23 = f14();
    p21 = (((signed char)0ULL) + (g1 ^= 5));
    return (p21 % ((int)4ULL));
}
int main(void) {
    unsigned int l25 = ((unsigned int)3ULL);
    unsigned long l26 = ((unsigned long)128ULL);
    unsigned char l27 = ((unsigned char)0ULL);
    g1 = ((*gp7) != (((unsigned char)128ULL) & ((_BitInt(26))0ULL)));
    mix(gs5.m1);
    f14();
    l25 = f8(&l26);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
