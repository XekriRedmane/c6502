// expect: 5012
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
int g1 = ((int)256ULL);
unsigned char g2 = ((unsigned char)255ULL);
unsigned int z3 [[c6502::zeropage]] = ((unsigned int)0ULL);
unsigned int ga4[4] = {((unsigned int)256ULL), ((unsigned int)128ULL), ((unsigned int)2ULL), ((unsigned int)127ULL)};
struct S_gs5 { unsigned char pad; unsigned long m0; unsigned long m1; };
struct S_gs5 gs5 = {0, ((unsigned long)32768ULL), ((unsigned long)32768ULL)};
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
int f6(unsigned int p7, int p8) {
    unsigned long long l9 = ((unsigned long long)128ULL);
    volatile unsigned int l10 = ((unsigned int)2ULL);
    unsigned char w12;
    unsigned char w13;
    switch ((((int)(((l9--) & (gs5.m0 >> 3)))) & ((int)7ULL))) {
    case 3:
        for (unsigned char i11 = 0; i11 < 1; i11++) {
            w12 = 0;
            do {
                (z3 |= 129);
            } while (++w12 < 4);
        }
    case 6:
        if ((gs5.m1 != ((int)0ULL))) {
            return gs5.m1;
            w13 = 0;
            do {
                break;
            } while (++w13 < 3);
            (z3 += 255);
        } else {
            mix((((unsigned int)0ULL) * ((int)0ULL)));
        }
        switch ((((int)(((signed char)2ULL))) & ((int)7ULL))) {
        case 1:
            mix((gs5.m0 -= 129));
        case 3:
            break;
        case 7:
            break;
        }
        break;
    case 7:
        break;
    default:
        ;
    }
    return (-((long)0ULL));
}
int f14(unsigned char p15, unsigned long p16, signed char p17) {
    static unsigned int s19 = ((unsigned int)256ULL);
    unsigned int l18 = ((unsigned int)23389ULL);
    unsigned char w20;
    (l18 -= 129);
    l18 = (p16 - g1);
    mix((++l18));
    switch ((((int)(p17)) & ((int)7ULL))) {
    case 3:
        ;
    case 4:
        switch ((((int)(g1)) & ((int)7ULL))) {
        case 3:
            break;
        case 7:
            w20 = 0;
            while (w20++ < 2) {
                mix(s19);
                break;
            }
        default:
            break;
        }
        break;
    case 7:
        (p16 -= 90);
        mix((ga4[((int)2ULL)] & z3));
        break;
    default:
        return (ga4[((int)3ULL)] ? p16 : ((signed char)0ULL));
    }
    return (g2 << 6);
}
int main(void) {
    volatile long l21 = ((long)9ULL);
    unsigned int l22 = ((unsigned int)0ULL);
    unsigned char l23 = ((unsigned char)255ULL);
    for (unsigned char i24 = 0; i24 < 3; i24++) {
        ga4[((int)2ULL)] = (l21 + g2);
        g1 = (ga4[(i24 & ((int)3ULL))] < gs5.m1);
        g2 = (((unsigned char)228ULL) + (gs5.m1 &= 90));
    }
    mix(l21);
    (g2 ^= 3);
    mix((gs5.m0 >= (((unsigned int)5ULL) + g1)));
    gs5.m1 = (g1 -= 129);
    mix((g1 + (z3 ? ((unsigned char)3ULL) : ((int)65535ULL))));
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
