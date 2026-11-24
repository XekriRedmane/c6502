// expect: 26572
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile unsigned int g1 = ((unsigned int)128ULL);
unsigned char g2 = ((unsigned char)53ULL);
volatile unsigned int z3 [[c6502::zeropage]] = ((unsigned int)32767ULL);
unsigned char z4 [[c6502::zeropage]] = ((unsigned char)255ULL);
signed char z5 [[c6502::zeropage]] = ((signed char)1ULL);
unsigned int ga6[4] = {((unsigned int)65535ULL), ((unsigned int)65535ULL), ((unsigned int)32767ULL), ((unsigned int)2ULL)};
struct S_gs7 { unsigned char pad; unsigned char m0; unsigned int m1; };
struct S_gs7 gs7 = {0, ((unsigned char)1ULL), ((unsigned int)0ULL)};
unsigned char *gp8 = &z4;
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
int f9(unsigned char *pp10) {
    int l11 = ((int)32767ULL);
    long long l12 = ((long long)3ULL);
    volatile signed char l13 = ((signed char)127ULL);
    mix(((-l12) + (gs7.m0 -= 1)));
    (l11 *= 255);
    switch ((((int)((--g2))) & ((int)7ULL))) {
    case 0:
        break;
    case 3:
        mix((gs7.m1 ^ (g2 &= 255)));
        break;
    default:
        switch ((((int)(((int)(l12)))) & ((int)7ULL))) {
        case 0:
            if (((z3++) - (*pp10))) {
                (z5--);
            }
            break;
        case 3:
            switch ((((int)((gs7.m1 <<= 7))) & ((int)7ULL))) {
            case 2:
                (*gp8) = (l12 *= 5);
                (*gp8) = ((signed char)0ULL);
                break;
            default:
                mix(((--z4) & (((unsigned char)0ULL) - ((int)65535ULL))));
            }
        case 6:
            (*pp10) = (l11 >> 1);
        default:
            ;
        }
    }
    return ((*gp8) + z5);
}
int f14(unsigned char p15, int p16, int p17, unsigned int *pp18) {
    static unsigned int s22 = ((unsigned int)0ULL);
    volatile int l19 = ((int)0ULL);
    signed char l20 = ((signed char)128ULL);
    unsigned char l21 = ((unsigned char)0ULL);
    l20 = (++l21);
    z3 = (p16 *= 1);
    (--z5);
    gs7.m0 = (++g2);
    z5 = ((g2 <<= 5) ^ (p17 & ga6[((int)0ULL)]));
    (p16 >>= 7);
    return s22;
}
signed char f23(int p24, unsigned char p25, unsigned char *pp26) {
    static int s30 = ((int)65535ULL);
    long long l27 = ((long long)1ULL);
    unsigned char l28 = ((unsigned char)0ULL);
    unsigned int l29 = ((unsigned int)32767ULL);
    (z5 <<= 6);
    f14((((unsigned int)0ULL) ^ z3), ((int)(s30)), (l29 ^ ((signed char)255ULL)), &l29);
    if (((l29 | l29) + (p25 -= 129))) {
        if ((++z4)) {
            mix((~(gs7.m1 >> 4)));
        }
    }
    s30 = (gs7.m1 | (z5 |= 3));
    mix((++ga6[((int)0ULL)]));
    if (((signed char)0ULL)) {
        p25 = (g1 & z5);
        mix((p25 ^= 255));
        return (z5 | (l27 -= 90));
    } else {
        if ((gs7.m1 & (p25 >>= 7))) {
        }
    }
    return (((signed char)255ULL) | g1);
}
int main(void) {
    volatile signed char l31 = ((signed char)3ULL);
    signed char l32 = ((signed char)255ULL);
    signed char l33 = ((signed char)0ULL);
    unsigned char l34 = ((unsigned char)0ULL);
    unsigned int l35 = ((unsigned int)0ULL);
    (gs7.m1 -= 1);
    (l32 &= 3);
    if ((z3 ^= 1)) {
        (--z3);
        mix((++z3));
    } else {
        ((*gp8) |= 90);
        f9(&l34);
    }
    z4 = (l35 <<= 5);
    switch ((((int)((l32 ^ l31))) & ((int)7ULL))) {
    case 4:
        g2 = ((gs7.m0++) | z3);
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
