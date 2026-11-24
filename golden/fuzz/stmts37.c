// expect: 2389
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
int g1 = ((int)0ULL);
unsigned char g2 = ((unsigned char)255ULL);
signed char g3 = ((signed char)0ULL);
unsigned int z4 [[c6502::zeropage]] = ((unsigned int)1ULL);
signed char z5 [[c6502::zeropage]] = ((signed char)255ULL);
volatile signed char z6 [[c6502::zeropage]] = ((signed char)3ULL);
unsigned char ga7[4] = {((unsigned char)0ULL), ((unsigned char)0ULL), ((unsigned char)0ULL), ((unsigned char)0ULL)};
struct S_gs8 { unsigned char pad; unsigned long m0; signed char m1; };
struct S_gs8 gs8 = {0, ((unsigned long)128ULL), ((signed char)127ULL)};
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(z4);
}
void fin1(void) {
    mix(z5);
    mix(z6);
    mix(ga7[((int)0ULL)]);
    mix(ga7[((int)1ULL)]);
}
void fin2(void) {
    mix(ga7[((int)2ULL)]);
    mix(ga7[((int)3ULL)]);
    mix(gs8.m0);
    mix(gs8.m1);
}
unsigned int f9(unsigned int *pp10) {
    long l11 = ((long)128ULL);
    unsigned char l12 = ((unsigned char)1ULL);
    g1 = (g3 & (z4 *= 90));
    g2 = ((gs8.m0++) & ga7[((int)0ULL)]);
    if ((((long)256ULL) - (l11 ^= 3))) {
        switch ((((int)(((signed char)128ULL))) & ((int)7ULL))) {
        case 0:
            return ((--g1) ^ gs8.m1);
        }
        z6 = (z4 -= 1);
        mix((gs8.m0 | (((unsigned char)4ULL) | g1)));
    }
    mix((~(-z6)));
    return ((int)(g1));
}
unsigned int f13(unsigned char *pp14) {
    unsigned int l15 = ((unsigned int)0ULL);
    unsigned char w16;
    if (((z6 >>= 6) - z5)) {
        w16 = 0;
        while (w16++ < 4) {
            mix(f9(&l15));
            mix(f9(&l15));
        }
    }
    mix(((*pp14) ^= 255));
    gs8.m1 = (z5 ^= 90);
    switch ((((int)((ga7[((int)0ULL)] ^ (z5 <<= 0)))) & ((int)7ULL))) {
    case 0:
        if ((((signed char)0ULL) % ((int)6ULL))) {
            for (unsigned char i17 = 0; i17 < 2; i17++) {
                break;
                break;
                gs8.m1 = z5;
            }
        }
    case 7:
        break;
    default:
        ;
    }
    return ((unsigned long)(gs8.m0));
}
unsigned char f18(unsigned char p19) {
    static int s23 = ((int)0ULL);
    volatile int l20 = ((int)0ULL);
    unsigned int l21 = ((unsigned int)65535ULL);
    unsigned char l22 = ((unsigned char)0ULL);
    unsigned char w24;
    switch ((((int)((s23 * l21))) & ((int)7ULL))) {
    case 0:
        break;
    case 1:
        ;
    case 7:
        break;
    default:
        w24 = 0;
        while (w24++ < 4) {
            (p19++);
            (g2 -= 90);
        }
        gs8.m0 = ((gs8.m1 >>= 4) + ((signed char)255ULL));
    }
    mix(g1);
    gs8.m0 = ((unsigned int)1ULL);
    mix(((g3 + l21) >= (((unsigned int)45189ULL) / ((int)1ULL))));
    gs8.m1 = (s23 >>= 5);
    mix((l21 ^= 255));
    return p19;
}
int main(void) {
    unsigned int l25 = ((unsigned int)65535ULL);
    unsigned char l26 = ((unsigned char)2ULL);
    for (unsigned char i27 = 0; i27 < 4; i27++) {
        mix(f18((z5 & i27)));
    }
    g3 = ((signed char)((ga7[((int)1ULL)] / ((int)2ULL))));
    for (unsigned char i28 = 0; i28 < 2; i28++) {
        gs8.m0 = (gs8.m1 & z4);
        switch ((((int)(g3)) & ((int)7ULL))) {
        case 1:
            mix((gs8.m0 + (ga7[((int)1ULL)] += 129)));
            break;
        case 2:
            break;
        case 3:
            gs8.m1 = (gs8.m0 <<= 3);
            f13(&l26);
            break;
        case 5:
            ;
        default:
            switch ((((int)((z6 - (g3 & ((unsigned long)0ULL))))) & ((int)7ULL))) {
            case 1:
                g2 = (gs8.m0 >>= 5);
                break;
            case 3:
                (z6 >>= 2);
            case 4:
                break;
            case 6:
                ;
            }
        }
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
