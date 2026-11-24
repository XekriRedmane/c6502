// expect: 16985
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned long g1 = ((unsigned long)128ULL);
unsigned int g2 = ((unsigned int)2ULL);
unsigned _BitInt(19) g3 = ((unsigned _BitInt(19))127ULL);
long long z4 [[c6502::zeropage]] = ((long long)256ULL);
long long z5 [[c6502::zeropage]] = ((long long)255ULL);
unsigned char ga6[4] = {((unsigned char)1ULL), ((unsigned char)0ULL), ((unsigned char)0ULL), ((unsigned char)0ULL)};
struct S_gs7 { unsigned char pad; unsigned int m0; unsigned int m1; };
struct S_gs7 gs7 = {0, ((unsigned int)32768ULL), ((unsigned int)32767ULL)};
unsigned _BitInt(19) *gp8 = &g3;
unsigned int *gp9 = &g2;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(z4);
}
void fin1(void) {
    mix(z5);
    mix(ga6[((int)0ULL)]);
    mix(ga6[((int)1ULL)]);
    mix(ga6[((int)2ULL)]);
}
void fin2(void) {
    mix(ga6[((int)3ULL)]);
    mix(gs7.m0);
    mix(gs7.m1);
}
unsigned _BitInt(7) f10(int p11, int p12) {
    unsigned _BitInt(19) l13 = ((unsigned _BitInt(19))32768ULL);
    unsigned int l14 = ((unsigned int)65535ULL);
    volatile unsigned char l15 = ((unsigned char)1ULL);
    unsigned char w19;
    for (unsigned char i16 = 0; i16 < 4; i16++) {
        for (unsigned char i17 = 0; i17 < 3; i17++) {
            if (((g3 &= 1) & p12)) {
                return gs7.m1;
            }
        }
        mix((((signed char)(gs7.m0)) & (i16 & p12)));
    }
    switch ((((int)((gs7.m1 |= 3))) & ((int)7ULL))) {
    case 1:
        mix(((int)43944ULL));
        break;
    case 5:
        for (unsigned char i18 = 0; i18 < 1; i18++) {
            (l15--);
            continue;
            (*gp8) = (g1 >>= 3);
            w19 = 0;
            while (w19++ < 4) {
            }
        }
        break;
    case 7:
        break;
    }
    return ((unsigned char)255ULL);
}
int main(void) {
    signed char l20 = ((signed char)0ULL);
    long l21 = ((long)256ULL);
    unsigned int l22 = ((unsigned int)65535ULL);
    unsigned char w23;
    unsigned char w24;
    g1 = (l22 ? (l21 << 5) : (l22 || g2));
    w23 = 0;
    while (w23++ < 3) {
        gs7.m1 = ((l21 -= 90) | l20);
        switch ((((int)((((*gp8) << 7) + (l20 &= 255)))) & ((int)7ULL))) {
        case 2:
            ;
        }
        if (w23) {
            mix(((g2 *= 5) ^ g3));
        }
        if ((l21 ^ w23)) {
            if ((++l22)) {
                break;
            }
            w24 = 0;
            while (w24++ < 4) {
                f10((((long)128ULL) >> 1), l22);
                break;
            }
        }
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
