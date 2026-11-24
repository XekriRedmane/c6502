// expect: 21623
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned char g1 = ((unsigned char)0ULL);
int g2 = ((int)1ULL);
unsigned char ga3[4] = {((unsigned char)3ULL), ((unsigned char)255ULL), ((unsigned char)127ULL), ((unsigned char)0ULL)};
struct S_gs4 { unsigned char pad; unsigned int m0; unsigned _BitInt(29) m1; };
struct S_gs4 gs4 = {0, ((unsigned int)127ULL), ((unsigned _BitInt(29))1ULL)};
int *gp5 = &g2;
unsigned char *gp6 = &g1;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(ga3[((int)0ULL)]);
    mix(ga3[((int)1ULL)]);
}
void fin1(void) {
    mix(ga3[((int)2ULL)]);
    mix(ga3[((int)3ULL)]);
    mix(gs4.m0);
    mix(gs4.m1);
}
unsigned int f7(unsigned char p8, unsigned int p9) {
    static unsigned int s13 = ((unsigned int)0ULL);
    signed char l10 = ((signed char)128ULL);
    unsigned int l11 = ((unsigned int)1ULL);
    volatile unsigned char l12 = ((unsigned char)2ULL);
    mix(((*gp6) & (p8--)));
    mix(((((unsigned char)0ULL) ^ ((unsigned _BitInt(29))65536ULL)) - (l12 >>= 2)));
    l12 = (g2 || s13);
    return (((signed char)127ULL) ^ ((unsigned char)0ULL));
}
unsigned _BitInt(33) f14(signed char p15) {
    volatile long l16 = ((long)65535ULL);
    g1 = ((l16 <<= 3) - ((unsigned long)32767ULL));
    if ((++g1)) {
        ((*gp6) <<= 6);
        f7((l16 + g2), gs4.m0);
    }
    (gs4.m1 >>= 0);
    return ((*gp5) == g1);
}
unsigned long f17(unsigned int p18, int p19) {
    static unsigned int s23 = ((unsigned int)2ULL);
    unsigned char l20 = ((unsigned char)1ULL);
    volatile int l21 = ((int)1ULL);
    unsigned int l22 = ((unsigned int)255ULL);
    l21 = f14(((*gp6) << 0));
    mix(((*gp6) - gs4.m1));
    (s23--);
    (++ga3[((int)0ULL)]);
    return l21;
}
int main(void) {
    volatile unsigned _BitInt(29) l24 = ((unsigned _BitInt(29))11ULL);
    unsigned char w25;
    unsigned char w26;
    f14((((unsigned _BitInt(33))65535ULL) - gs4.m1));
    ((*gp6) += 3);
    w25 = 0;
    do {
        g2 = w25;
        continue;
    } while (++w25 < 1);
    mix((g1 |= 3));
    f7((!g1), ((unsigned _BitInt(33))65536ULL));
    w26 = 0;
    while (w26++ < 2) {
        ga3[(w26 & ((int)3ULL))] = (w26 / ((int)1ULL));
        mix(((unsigned char)0ULL));
        (*gp5) = (((signed char)7ULL) - w26);
    }
    fin0();
    fin1();
    return (int)(acc & 0x7FFF);
}
