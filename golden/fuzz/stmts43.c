// expect: 23218
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
long g1 = ((long)3ULL);
long long g2 = ((long long)255ULL);
volatile unsigned char z3 [[c6502::zeropage]] = ((unsigned char)0ULL);
unsigned char ga4[4] = {((unsigned char)128ULL), ((unsigned char)128ULL), ((unsigned char)255ULL), ((unsigned char)3ULL)};
struct S_gs5 { unsigned char pad; signed char m0; int m1; };
struct S_gs5 gs5 = {0, ((signed char)255ULL), ((int)127ULL)};
long *gp6 = &g1;
long long *gp7 = &g2;
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
long f8(unsigned _BitInt(23) p9) {
    signed char l10 = ((signed char)255ULL);
    (ga4[((int)3ULL)] += 1);
    mix((ga4[((int)3ULL)] << 3));
    if (gs5.m0) {
        if ((l10 *= 255)) {
            (l10++);
        }
        (--z3);
    } else {
        l10 = (p9 += 255);
        for (unsigned char i11 = 0; i11 < 1; i11++) {
            (g1 += 1);
            mix((gs5.m1 | (ga4[(i11 & ((int)3ULL))] |= 129)));
            ((*gp6)--);
        }
    }
    return (p9 / ((int)3ULL));
}
int f12(signed char *pp13) {
    static unsigned char s17 = ((unsigned char)128ULL);
    int l14 = ((int)32767ULL);
    long l15 = ((long)65536ULL);
    unsigned long long l16 = ((unsigned long long)3ULL);
    l15 = f8((*pp13));
    if ((((unsigned long)(z3)) / ((int)5ULL))) {
        mix((s17 ^ (l16 += 5)));
    } else {
        (g1 >>= 2);
    }
    mix(((*gp7) -= 5));
    return (((int)1ULL) != l14);
}
unsigned long long f18(unsigned char p19, unsigned char *pp20) {
    _BitInt(3) l21 = ((_BitInt(3))3ULL);
    signed char l22 = ((signed char)255ULL);
    unsigned char w23;
    unsigned char w24;
    (l22 *= 255);
    if ((g2--)) {
        if (((*gp6) + p19)) {
            return (l21 + gs5.m1);
            ((*gp6) += 90);
            w23 = 0;
            do {
                f8((~((unsigned int)256ULL)));
            } while (++w23 < 3);
        }
        w24 = 0;
        while (w24++ < 2) {
            switch ((((int)((w24 ^ ((*gp7) *= 1)))) & ((int)7ULL))) {
            case 1:
                break;
                break;
            case 3:
                break;
                break;
            case 6:
                break;
            default:
                ;
            }
        }
    }
    return ga4[((int)3ULL)];
}
int main(void) {
    unsigned int l25 = ((unsigned int)0ULL);
    unsigned int l26 = ((unsigned int)128ULL);
    signed char l27 = ((signed char)127ULL);
    unsigned char l28 = ((unsigned char)255ULL);
    unsigned char w29;
    unsigned char w30;
    if ((g2 &= 5)) {
        f12(&l27);
        g2 = (l26 + (gs5.m0--));
    }
    w29 = 0;
    do {
        if ((((unsigned long long)32767ULL) << 1)) {
            (l27 &= 90);
        } else {
            break;
        }
        (ga4[((int)1ULL)] += 129);
        mix(f8((g2 - l26)));
        switch ((((int)((*gp7))) & ((int)7ULL))) {
        case 2:
            w30 = 0;
            while (w30++ < 3) {
                l25 = (gs5.m0 | (--l27));
            }
            break;
        case 4:
            break;
        case 7:
            break;
        default:
            ;
        }
    } while (++w29 < 4);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
