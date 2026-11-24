// expect: 32304
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned int g1 = ((unsigned int)65535ULL);
volatile unsigned char g2 = ((unsigned char)0ULL);
_BitInt(9) ga3[4] = {((_BitInt(9))128ULL), ((_BitInt(9))3ULL), ((_BitInt(9))511ULL), ((_BitInt(9))127ULL)};
struct S_gs4 { unsigned char pad; unsigned char m0; unsigned int m1; };
struct S_gs4 gs4 = {0, ((unsigned char)255ULL), ((unsigned int)65535ULL)};
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
unsigned _BitInt(34) f5(unsigned char *pp6) {
    static signed char s9 = ((signed char)128ULL);
    int l7 = ((int)2ULL);
    volatile signed char l8 = ((signed char)0ULL);
    mix((g1 - g1));
    gs4.m1 = ((unsigned int)(g1));
    gs4.m1 = ((*pp6) & (l7--));
    mix((*pp6));
    l7 = (((unsigned long)1028115956ULL) | (++gs4.m1));
    if (l8) {
        (l7 -= 5);
    }
    return ((unsigned char)(gs4.m1));
}
signed char f10(unsigned int p11, unsigned char p12, unsigned char p13) {
    static unsigned char s17 = ((unsigned char)255ULL);
    int l14 = ((int)0ULL);
    long l15 = ((long)32768ULL);
    unsigned char l16 = ((unsigned char)127ULL);
    mix(f5(&l16));
    if ((gs4.m0 - (l15--))) {
        (p12 &= 1);
        g1 = (gs4.m1 <<= 4);
        mix((((signed char)(l14)) % ((int)2ULL)));
    } else {
        mix(((g2 -= 3) - ((unsigned int)32768ULL)));
        l16 = f5(&l16);
    }
    f5(&l16);
    switch ((((int)((((unsigned int)65535ULL) || ((_BitInt(9))127ULL)))) & ((int)7ULL))) {
    case 1:
        return (p11 -= 255);
    case 6:
        break;
    }
    return (((unsigned char)1ULL) + g2);
}
signed char f18(signed char p19, unsigned int p20, unsigned int p21) {
    static unsigned char s24 = ((unsigned char)0ULL);
    volatile long l22 = ((long)0ULL);
    unsigned char l23 = ((unsigned char)1ULL);
    (s24 &= 5);
    mix(((ga3[((int)1ULL)] -= 5) + g2));
    mix(f10(((int)0ULL), (((signed char)255ULL) & s24), (((unsigned _BitInt(34))32768ULL) << 1)));
    for (unsigned char i25 = 0; i25 < 4; i25++) {
        mix(f10((~i25), (s24 & p21), (i25 != p20)));
        gs4.m1 = ((p20 += 255) | (l23 + i25));
    }
    f10((gs4.m1 != gs4.m0), (((signed char)0ULL) % ((int)6ULL)), (!p19));
    if (((int)(((unsigned char)(ga3[((int)2ULL)]))))) {
        return (p19 *= 129);
    }
    return (((unsigned char)3ULL) || ((long long)256ULL));
}
int main(void) {
    volatile signed char l26 = ((signed char)2ULL);
    unsigned char l27 = ((unsigned char)0ULL);
    mix(((l26 | l27) ^ (l26 ? ((signed char)0ULL) : ((unsigned char)46ULL))));
    mix(g2);
    if ((gs4.m1 -= 90)) {
        f5(&l27);
    } else {
        f5(&l27);
        mix(gs4.m0);
    }
    ga3[((int)3ULL)] = (l27 &= 255);
    fin0();
    fin1();
    return (int)(acc & 0x7FFF);
}
