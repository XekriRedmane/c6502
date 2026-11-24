// expect: 26065
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
signed char g1 = ((signed char)1ULL);
unsigned int g2 = ((unsigned int)128ULL);
unsigned char g3 = ((unsigned char)0ULL);
unsigned char z4 [[c6502::zeropage]] = ((unsigned char)0ULL);
volatile int z5 [[c6502::zeropage]] = ((int)32767ULL);
_BitInt(14) z6 [[c6502::zeropage]] = ((_BitInt(14))13ULL);
unsigned int ga7[4] = {((unsigned int)0ULL), ((unsigned int)0ULL), ((unsigned int)12ULL), ((unsigned int)3ULL)};
struct S_gs8 { unsigned char pad; int m0; unsigned char m1; };
struct S_gs8 gs8 = {0, ((int)65535ULL), ((unsigned char)255ULL)};
unsigned char *gp9 = &g3;
_BitInt(14) *gp10 = &z6;
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
signed char f11(int p12, long p13) {
    static unsigned int s16 = ((unsigned int)0ULL);
    unsigned char l14 = ((unsigned char)1ULL);
    unsigned long l15 = ((unsigned long)1979631564ULL);
    unsigned char w18;
    if ((l15 | (g2 ^= 3))) {
        for (unsigned char i17 = 0; i17 < 3; i17++) {
            mix((gs8.m0 <= g2));
            if ((p13 &= 5)) {
                break;
                mix((p13 & ((*gp9) -= 5)));
            }
            mix((z5 - ((int)65535ULL)));
        }
        p12 = (z6 += 3);
    } else {
        w18 = 0;
        while (w18++ < 4) {
            continue;
        }
    }
    gs8.m0 = (((*gp10)--) + ((unsigned int)32767ULL));
    mix((z4 *= 1));
    return ((*gp9) % ((int)6ULL));
}
unsigned char f19(int p20, unsigned char p21, unsigned char p22, unsigned char *pp23) {
    volatile _BitInt(14) l24 = ((_BitInt(14))128ULL);
    unsigned char w25;
    (*gp9) = (z5 ^= 90);
    mix(((*gp9) += 255));
    (gs8.m1++);
    mix(((--gs8.m0) | p22));
    w25 = 0;
    do {
        if ((g2 % ((int)4ULL))) {
            mix(f11((z4 & p21), (w25 - w25)));
            (gs8.m0 >>= 7);
            (p20 >>= 5);
        }
        l24 = f11((((int)255ULL) & p21), ((signed char)(z4)));
        break;
        gs8.m1 = ((*gp10) - (--(*gp9)));
    } while (++w25 < 2);
    return (!ga7[((int)2ULL)]);
}
int f26(unsigned char p27, signed char p28, unsigned int p29, int *pp30) {
    unsigned char l31 = ((unsigned char)119ULL);
    volatile signed char l32 = ((signed char)255ULL);
    unsigned char w34;
    for (unsigned char i33 = 0; i33 < 4; i33++) {
        f19((-i33), (((unsigned char)127ULL) == p29), (((_BitInt(14))2ULL) + gs8.m1), &l31);
    }
    gs8.m1 = (gs8.m1 <= ((_BitInt(22))255ULL));
    if ((p27 + (z4++))) {
        mix((g2 - (p29 ^= 129)));
    }
    w34 = 0;
    while (w34++ < 4) {
        ga7[((int)1ULL)] = (g1 - (g2 ^= 5));
    }
    mix((p27 - gs8.m1));
    z4 = ((p29 &= 3) ^ ((int)3ULL));
    return ((_BitInt(22))1ULL);
}
int main(void) {
    _BitInt(22) l35 = ((_BitInt(22))127ULL);
    unsigned char l36 = ((unsigned char)0ULL);
    int l37 = ((int)1ULL);
    f19((z6 ? z5 : z6), (g3 + l35), ((signed char)(g3)), &l36);
    switch ((((int)((((unsigned int)256ULL) - (--gs8.m0)))) & ((int)7ULL))) {
    case 1:
        break;
    case 2:
        break;
    case 5:
        f11(l35, l37);
        break;
    case 6:
        (g3 <<= 6);
        (z4 <<= 7);
        break;
    default:
        g1 = (l37 + (((int)0ULL) - ((unsigned long)12ULL)));
        (z4 <<= 5);
    }
    switch ((((int)(((*gp9) |= 5))) & ((int)7ULL))) {
    case 4:
        for (unsigned char i38 = 0; i38 < 2; i38++) {
            mix(f19((g3 ^ z4), (l36 * l35), (!i38), &l36));
            mix((i38 != l35));
            (*gp10) = ((g3 <<= 5) & i38);
        }
        break;
    case 6:
        ;
    case 7:
        break;
    default:
        ;
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
