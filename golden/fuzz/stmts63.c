// expect: 19076
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile unsigned int g1 = ((unsigned int)32768ULL);
signed char g2 = ((signed char)1ULL);
signed char z3 [[c6502::zeropage]] = ((signed char)255ULL);
unsigned _BitInt(55) ga4[4] = {((unsigned _BitInt(55))127ULL), ((unsigned _BitInt(55))2ULL), ((unsigned _BitInt(55))256ULL), ((unsigned _BitInt(55))3ULL)};
struct S_gs5 { unsigned char pad; long m0; unsigned int m1; };
struct S_gs5 gs5 = {0, ((long)1ULL), ((unsigned int)65535ULL)};
signed char *gp6 = &g2;
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
unsigned char f7(int p8, int *pp9) {
    static unsigned int s12 = ((unsigned int)32767ULL);
    unsigned int l10 = ((unsigned int)32767ULL);
    long l11 = ((long)65536ULL);
    unsigned char w13;
    w13 = 0;
    do {
        p8 = (w13 ? w13 : w13);
        if ((g1 *= 3)) {
            (p8 &= 3);
            mix(((*pp9) |= 90));
        } else {
            continue;
            mix((g1--));
        }
    } while (++w13 < 3);
    switch ((((int)((++s12))) & ((int)7ULL))) {
    case 1:
        g1 = (g2 | ((unsigned long long)0ULL));
    }
    (++p8);
    return (l10 ? ga4[((int)0ULL)] : s12);
}
signed char f14(unsigned int p15, unsigned char p16, unsigned int p17) {
    int l18 = ((int)0ULL);
    signed char l19 = ((signed char)0ULL);
    long l20 = ((long)65536ULL);
    unsigned char w21;
    l20 = f7(((unsigned int)((*gp6))), &l18);
    mix((ga4[((int)0ULL)] * g2));
    w21 = 0;
    while (w21++ < 1) {
        (l20 += 1);
        l19 = f7((l20 * w21), &l18);
        break;
    }
    return (p17 & l18);
}
unsigned _BitInt(51) f22(signed char p23, unsigned long p24, unsigned char p25) {
    static unsigned int s28 = ((unsigned int)0ULL);
    volatile unsigned _BitInt(55) l26 = ((unsigned _BitInt(55))5400875933050667ULL);
    int l27 = ((int)2ULL);
    mix(f7(p24, &l27));
    mix((p23 <<= 7));
    if ((s28 - g2)) {
        for (unsigned char i29 = 0; i29 < 1; i29++) {
            continue;
            (p24 <<= 5);
        }
        (p25 ^= 90);
        if ((g2 > s28)) {
            if (s28) {
                return (l26 -= 90);
            }
        } else {
            switch ((((int)((*gp6))) & ((int)7ULL))) {
            case 0:
                (l27--);
            case 4:
                ;
            case 6:
                break;
            default:
                ;
            }
        }
    }
    return (((unsigned char)1ULL) == ga4[((int)0ULL)]);
}
int main(void) {
    int l30 = ((int)1ULL);
    int l31 = ((int)2ULL);
    mix(((*gp6) == gs5.m0));
    gs5.m0 = z3;
    mix(f22((g2 - gs5.m1), g1, (gs5.m1 == z3)));
    for (unsigned char i32 = 0; i32 < 4; i32++) {
        (--gs5.m1);
        if (((ga4[(i32 & ((int)3ULL))] <<= 4) - ((unsigned long)65535ULL))) {
            f7((g1 > ((unsigned char)0ULL)), &l31);
            if (((z3 -= 255) ^ (!l30))) {
                f14(l30, (~i32), ((*gp6) & g2));
                break;
                break;
            }
        }
        continue;
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
