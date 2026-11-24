// expect: 2063
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
signed char g1 = ((signed char)0ULL);
signed char g2 = ((signed char)15ULL);
unsigned long z3 [[c6502::zeropage]] = ((unsigned long)32767ULL);
int z4 [[c6502::zeropage]] = ((int)50882ULL);
unsigned long z5 [[c6502::zeropage]] = ((unsigned long)1ULL);
int ga6[4] = {((int)0ULL), ((int)128ULL), ((int)255ULL), ((int)2ULL)};
struct S_gs7 { unsigned char pad; unsigned _BitInt(56) m0; unsigned long m1; };
struct S_gs7 gs7 = {0, ((unsigned _BitInt(56))255ULL), ((unsigned long)1ULL)};
signed char *gp8 = &g1;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(z3);
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
unsigned long f9(unsigned int p10, unsigned char p11, unsigned long *pp12) {
    static int s15 = ((int)3ULL);
    unsigned char l13 = ((unsigned char)255ULL);
    volatile unsigned int l14 = ((unsigned int)128ULL);
    if ((g2 % ((int)6ULL))) {
        for (unsigned char i16 = 0; i16 < 4; i16++) {
            mix(((ga6[((int)2ULL)] == gs7.m0) - (p11 &= 3)));
            gs7.m1 = (s15 += 255);
            return (i16 >> 0);
            gs7.m1 = ((((unsigned int)256ULL) << 7) - (ga6[((int)2ULL)] + gs7.m0));
        }
        switch ((((int)((gs7.m0 / ((int)3ULL)))) & ((int)7ULL))) {
        case 2:
            z3 = ((-gs7.m1) & (p11 -= 90));
            mix((l13 >>= 1));
            break;
        default:
            (z3 ^= 90);
        }
        g2 = (z4--);
    } else {
        mix(((*pp12) - ((long)32768ULL)));
    }
    return (ga6[((int)1ULL)] & gs7.m1);
}
unsigned char f17(void) {
    unsigned char l18 = ((unsigned char)2ULL);
    unsigned long l19 = ((unsigned long)65535ULL);
    unsigned char w20;
    w20 = 0;
    do {
        for (unsigned char i21 = 0; i21 < 2; i21++) {
            mix(f9((z5 ? ((signed char)127ULL) : (*gp8)), (ga6[(w20 & ((int)3ULL))] / ((int)7ULL)), &l19));
            ga6[(i21 & ((int)3ULL))] = (z5 <<= 5);
            gs7.m0 = (l18 - z5);
            (gs7.m0 -= 5);
        }
    } while (++w20 < 1);
    l19 = f9((z3 >> 2), (~g2), &l19);
    switch ((((int)((gs7.m1 ^= 3))) & ((int)7ULL))) {
    case 2:
        (++l18);
        break;
    default:
        (++l19);
    }
    return (gs7.m1 ? g1 : ga6[((int)3ULL)]);
}
int f22(unsigned char p23, _BitInt(18) p24, unsigned _BitInt(56) p25, unsigned int *pp26) {
    volatile long long l27 = ((long long)128ULL);
    unsigned _BitInt(56) l28 = ((unsigned _BitInt(56))0ULL);
    int l29 = ((int)255ULL);
    unsigned long l30 = ((unsigned long)32768ULL);
    mix(f9((((unsigned char)255ULL) % ((int)7ULL)), (gs7.m0 >> 7), &l30));
    f9(((unsigned char)0ULL), ((signed char)((*pp26))), &l30);
    mix(f17());
    f9(((unsigned int)256ULL), z4, &l30);
    if ((gs7.m0++)) {
        switch ((((int)(((((unsigned char)0ULL) >> 3) / ((int)3ULL)))) & ((int)7ULL))) {
        case 0:
            for (unsigned char i31 = 0; i31 < 2; i31++) {
                (--p25);
            }
            break;
        case 4:
            if ((((*pp26) >>= 4) & z4)) {
                break;
                (--g2);
                (*pp26) = ((*gp8) >> 4);
            }
        case 6:
            break;
        default:
            ;
        }
    }
    return ((unsigned char)255ULL);
}
int main(void) {
    unsigned char l32 = ((unsigned char)255ULL);
    unsigned long l33 = ((unsigned long)0ULL);
    unsigned long long l34 = ((unsigned long long)128ULL);
    unsigned int l35 = ((unsigned int)127ULL);
    gs7.m1 = (((unsigned int)64632ULL) >> 6);
    mix((ga6[((int)1ULL)] += 255));
    mix(((unsigned long)(((int)13ULL))));
    mix((l35 << 5));
    mix((gs7.m0 <<= 7));
    ((*gp8) += 129);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
