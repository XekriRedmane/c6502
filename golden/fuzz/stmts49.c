// expect: 1330
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile signed char g1 = ((signed char)145ULL);
volatile signed char g2 = ((signed char)128ULL);
signed char z3 [[c6502::zeropage]] = ((signed char)255ULL);
unsigned int z4 [[c6502::zeropage]] = ((unsigned int)255ULL);
unsigned int ga5[4] = {((unsigned int)32768ULL), ((unsigned int)256ULL), ((unsigned int)3ULL), ((unsigned int)127ULL)};
struct S_gs6 { unsigned char pad; unsigned long long m0; int m1; };
struct S_gs6 gs6 = {0, ((unsigned long long)3ULL), ((int)0ULL)};
unsigned int *gp7 = &z4;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(z3);
    mix(z4);
}
void fin1(void) {
    mix(ga5[((int)0ULL)]);
    mix(ga5[((int)1ULL)]);
    mix(ga5[((int)2ULL)]);
    mix(ga5[((int)3ULL)]);
}
void fin2(void) {
    mix(gs6.m0);
    mix(gs6.m1);
}
unsigned int f8(unsigned char p9, long p10) {
    static volatile unsigned int s13 = ((unsigned int)128ULL);
    volatile unsigned char l11 = ((unsigned char)255ULL);
    unsigned int l12 = ((unsigned int)127ULL);
    (*gp7) = (l12 & (gs6.m1--));
    mix((((unsigned int)(z3)) | (l12 -= 129)));
    mix((((unsigned int)0ULL) & ga5[((int)2ULL)]));
    return g2;
}
signed char f14(unsigned int p15, int p16, unsigned int p17) {
    static volatile signed char s19 = ((signed char)0ULL);
    unsigned long l18 = ((unsigned long)127ULL);
    unsigned char w20;
    ga5[((int)2ULL)] = (l18 | ga5[((int)1ULL)]);
    mix((g1 += 129));
    if ((p15 *= 3)) {
        g1 = ((z3 += 5) & gs6.m0);
        w20 = 0;
        while (w20++ < 3) {
            (p16++);
            l18 = (s19 | (++(*gp7)));
            gs6.m1 = (--g1);
            mix(((--p15) + gs6.m1));
        }
        (*gp7) = (gs6.m0 <<= 1);
    } else {
        mix((p16++));
        if (z4) {
        }
    }
    return (p16 <= g2);
}
unsigned char f21(unsigned long *pp22) {
    static unsigned char s25 = ((unsigned char)1ULL);
    volatile unsigned char l23 = ((unsigned char)0ULL);
    int l24 = ((int)65535ULL);
    unsigned char w26;
    s25 = (gs6.m1 / ((int)4ULL));
    w26 = 0;
    while (w26++ < 1) {
        l24 = f14((s25 && (*pp22)), l23, z4);
        mix(z3);
    }
    f8(((unsigned int)((*gp7))), (!g2));
    return (z4 / ((int)5ULL));
}
int main(void) {
    unsigned char l27 = ((unsigned char)15ULL);
    int l28 = ((int)2ULL);
    unsigned int l29 = ((unsigned int)11ULL);
    unsigned long l30 = ((unsigned long)4218720131ULL);
    mix((z4 & (l28++)));
    if ((((unsigned long)255ULL) * ((unsigned int)0ULL))) {
        switch ((((int)((gs6.m0 += 3))) & ((int)7ULL))) {
        case 2:
            mix(((l29 ^= 5) - ((*gp7) >> 5)));
            break;
        case 4:
            mix((z4 *= 129));
        case 7:
            if (((gs6.m1 *= 90) | g2)) {
                (z3 += 5);
                z3 = (((*gp7) &= 90) & g1);
            } else {
                (ga5[((int)1ULL)] ^= 5);
            }
        default:
            mix((l29 & l30));
            g1 = (g1 ^ (ga5[((int)3ULL)] *= 5));
        }
        (++z4);
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
