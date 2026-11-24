// expect: 20125
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned int g1 = ((unsigned int)0ULL);
unsigned int g2 = ((unsigned int)32768ULL);
int g3 = ((int)11ULL);
unsigned int z4 [[c6502::zeropage]] = ((unsigned int)9ULL);
volatile int z5 [[c6502::zeropage]] = ((int)32768ULL);
unsigned int ga6[4] = {((unsigned int)7ULL), ((unsigned int)127ULL), ((unsigned int)1ULL), ((unsigned int)32767ULL)};
struct S_gs7 { unsigned char pad; int m0; _BitInt(18) m1; };
struct S_gs7 gs7 = {0, ((int)55144ULL), ((_BitInt(18))255ULL)};
int *gp8 = &g3;
unsigned int *gp9 = &z4;
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
unsigned char f10(signed char p11, unsigned char p12, unsigned int p13, int *pp14) {
    static int s17 = ((int)65535ULL);
    volatile int l15 = ((int)32768ULL);
    volatile _BitInt(25) l16 = ((_BitInt(25))32768ULL);
    unsigned char w19;
    switch ((((int)(((l15 >>= 7) ^ s17))) & ((int)7ULL))) {
    case 0:
        switch ((((int)((gs7.m1 ^ z5))) & ((int)7ULL))) {
        case 2:
            (ga6[((int)1ULL)] *= 3);
            break;
        case 6:
            switch ((((int)(((g2++) - ((_BitInt(18))2ULL)))) & ((int)7ULL))) {
            case 0:
                ;
            case 2:
                break;
            case 4:
                break;
                break;
            case 5:
                g3 = (z4 ^= 3);
            }
            p11 = l15;
            break;
        default:
            for (unsigned char i18 = 0; i18 < 2; i18++) {
                break;
                (z5 |= 3);
            }
            w19 = 0;
            while (w19++ < 2) {
                l15 = (w19 & (g3--));
            }
        }
        break;
    case 2:
        break;
    case 4:
        ;
    case 5:
        break;
    }
    return (((unsigned long)65536ULL) <= g2);
}
_BitInt(18) f20(unsigned int p21, unsigned int p22, _BitInt(25) *pp23) {
    static unsigned long s26 = ((unsigned long)2ULL);
    long long l24 = ((long long)255ULL);
    int l25 = ((int)3ULL);
    (z5++);
    mix(((((unsigned int)32768ULL) + gs7.m0) > (l25 << 3)));
    (*gp9) = ((++(*gp8)) ^ ((unsigned long long)127ULL));
    if ((gs7.m0 >>= 2)) {
        z4 = (((signed char)127ULL) | ((*pp23) *= 90));
        for (unsigned char i27 = 0; i27 < 1; i27++) {
            mix((s26 + (--g1)));
        }
    }
    if (ga6[((int)1ULL)]) {
        return ((((_BitInt(18))0ULL) * l25) - (s26 << 6));
        g2 = ((*pp23) + (p22 += 255));
    } else {
        gs7.m1 = (((*gp9) >> 1) & (p21 >>= 4));
    }
    return (z5 && (*gp9));
}
int main(void) {
    unsigned char l28 = ((unsigned char)0ULL);
    unsigned char l29 = ((unsigned char)255ULL);
    volatile unsigned int l30 = ((unsigned int)3ULL);
    int l31 = ((int)32768ULL);
    _BitInt(25) l32 = ((_BitInt(25))256ULL);
    unsigned char w33;
    unsigned char w34;
    w33 = 0;
    while (w33++ < 2) {
        continue;
        g2 = (l32 | ((_BitInt(18))65536ULL));
        w34 = 0;
        while (w34++ < 4) {
            (l28--);
        }
        if ((l29 >>= 1)) {
            mix((g2 ^= 1));
        } else {
            (l30--);
        }
    }
    switch ((((int)((l30 *= 255))) & ((int)7ULL))) {
    case 3:
        break;
    default:
        l30 = f20((g1 - l29), (l29 | g2), &l32);
        l31 = ((((signed char)0ULL) || ((signed char)127ULL)) & (l28 *= 1));
    }
    switch ((((int)((((*gp8) ? z4 : l32) || (((signed char)255ULL) + z5)))) & ((int)7ULL))) {
    case 5:
        break;
    case 6:
        ;
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
