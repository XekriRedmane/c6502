// expect: 29120
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned char g1 = ((unsigned char)7ULL);
unsigned long long g2 = ((unsigned long long)127ULL);
long long z3 [[c6502::zeropage]] = ((long long)32767ULL);
_BitInt(58) ga4[4] = {((_BitInt(58))32767ULL), ((_BitInt(58))2147483647ULL), ((_BitInt(58))32768ULL), ((_BitInt(58))32768ULL)};
struct S_gs5 { unsigned char pad; unsigned int m0; long m1; };
struct S_gs5 gs5 = {0, ((unsigned int)2ULL), ((long)2ULL)};
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(z3);
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
unsigned char f6(_BitInt(40) *pp7) {
    unsigned long l8 = ((unsigned long)65536ULL);
    unsigned char w10;
    mix(((g2 += 3) & (*pp7)));
    mix(((unsigned int)(gs5.m1)));
    for (unsigned char i9 = 0; i9 < 3; i9++) {
        ((*pp7) |= 90);
        switch ((((int)((ga4[((int)2ULL)] && i9))) & ((int)7ULL))) {
        case 7:
            ;
        }
        w10 = 0;
        while (w10++ < 4) {
            mix((i9 & z3));
            return (g2 >>= 6);
            mix((w10 ? g1 : ((signed char)127ULL)));
            (gs5.m1 ^= 255);
        }
        if ((((_BitInt(40))32768ULL) % ((int)6ULL))) {
            mix((++gs5.m1));
        }
    }
    return (gs5.m1 ^ (*pp7));
}
_BitInt(40) f11(unsigned int p12, int p13, signed char p14) {
    static unsigned int s19 = ((unsigned int)1ULL);
    signed char l15 = ((signed char)1ULL);
    volatile int l16 = ((int)65535ULL);
    volatile signed char l17 = ((signed char)4ULL);
    _BitInt(40) l18 = ((_BitInt(40))3ULL);
    switch ((((int)((l16 += 5))) & ((int)7ULL))) {
    case 2:
        break;
    default:
        mix(((l17 <<= 5) | p12));
        (l15 >>= 6);
    }
    l15 = f6(&l18);
    ga4[((int)1ULL)] = (z3 ? ((unsigned char)0ULL) : z3);
    return (((_BitInt(40))65536ULL) ^ gs5.m0);
}
int main(void) {
    int l20 = ((int)53241ULL);
    volatile unsigned int l21 = ((unsigned int)128ULL);
    unsigned char l22 = ((unsigned char)0ULL);
    _BitInt(40) l23 = ((_BitInt(40))65536ULL);
    mix((z3 & (l23++)));
    f11(((unsigned char)0ULL), ga4[((int)3ULL)], l21);
    l22 = f11((!((unsigned char)2ULL)), (gs5.m0 * ((unsigned int)256ULL)), (ga4[((int)1ULL)] + ((signed char)128ULL)));
    l21 = f11((l21 - z3), ((unsigned char)(((unsigned char)255ULL))), ((unsigned int)0ULL));
    z3 = (((unsigned int)65535ULL) >= l20);
    (z3 *= 5);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
