// expect: 24942
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned int g1 = ((unsigned int)0ULL);
long g2 = ((long)32767ULL);
unsigned int z3 [[c6502::zeropage]] = ((unsigned int)3ULL);
unsigned char z4 [[c6502::zeropage]] = ((unsigned char)32ULL);
signed char z5 [[c6502::zeropage]] = ((signed char)1ULL);
unsigned int ga6[4] = {((unsigned int)1ULL), ((unsigned int)20015ULL), ((unsigned int)2ULL), ((unsigned int)32767ULL)};
struct S_gs7 { unsigned char pad; unsigned int m0; unsigned int m1; };
struct S_gs7 gs7 = {0, ((unsigned int)128ULL), ((unsigned int)55881ULL)};
long *gp8 = &g2;
unsigned int *gp9 = &g1;
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
long f10(void) {
    static signed char s13 = ((signed char)0ULL);
    int l11 = ((int)15ULL);
    unsigned char l12 = ((unsigned char)0ULL);
    switch ((((int)(z3)) & ((int)7ULL))) {
    case 1:
        (g2 *= 1);
        z5 = (gs7.m1 >>= 3);
    case 4:
        break;
    case 5:
        if ((l12 += 90)) {
            if ((((unsigned long long)65536ULL) & (l12 + l12))) {
                return (((unsigned long)(gs7.m0)) | (gs7.m0 && gs7.m0));
            } else {
                mix(((unsigned long)2147483647ULL));
                g1 = (z3 >> 6);
            }
        }
        switch ((((int)((((*gp8)--) & l12))) & ((int)7ULL))) {
        case 5:
            return (z5 & (gs7.m0 &= 3));
        case 6:
            ;
        default:
            (z4--);
            if ((g2 | (gs7.m0 |= 90))) {
            }
        }
        break;
    case 7:
        break;
    }
    return ((unsigned char)255ULL);
}
int f14(signed char p15, unsigned int p16, int p17, unsigned int *pp18) {
    static int s22 = ((int)256ULL);
    volatile unsigned char l19 = ((unsigned char)127ULL);
    int l20 = ((int)57558ULL);
    volatile unsigned char l21 = ((unsigned char)2ULL);
    mix((gs7.m1 &= 1));
    l20 = f10();
    g1 = (z3 -= 5);
    p17 = (((*gp9) >>= 1) | ((unsigned char)243ULL));
    return l20;
}
unsigned int f23(unsigned int p24, signed char p25, signed char p26, long *pp27) {
    static unsigned int s30 = ((unsigned int)2ULL);
    unsigned long l28 = ((unsigned long)1506634610ULL);
    unsigned int l29 = ((unsigned int)127ULL);
    l29 = p24;
    p25 = (gs7.m0 | (p26 |= 129));
    p25 = (((unsigned int)128ULL) ? l28 : p25);
    (*pp27) = (g1 + (ga6[((int)2ULL)] &= 129));
    if ((z3 <<= 6)) {
        (ga6[((int)1ULL)] -= 5);
    } else {
        p26 = (p24 + z3);
        l28 = f10();
    }
    return ga6[((int)2ULL)];
}
int main(void) {
    long l31 = ((long)256ULL);
    unsigned int l32 = ((unsigned int)65535ULL);
    (z4 |= 255);
    (gs7.m1 += 1);
    g1 = ((gs7.m1 <<= 4) - z5);
    if ((l32 <<= 5)) {
        l31 = (l31 | (gs7.m0 -= 3));
        if ((gs7.m1 &= 1)) {
            switch ((((int)((gs7.m0 + g1))) & ((int)7ULL))) {
            case 7:
                mix(((g1 & z5) & (((unsigned int)127ULL) & ((unsigned char)0ULL))));
            }
            if (((unsigned int)((*gp8)))) {
                mix(f10());
                f23((l31 - g1), (l32 - ga6[((int)3ULL)]), (~(*gp9)), &l31);
                f14((((unsigned int)0ULL) > gs7.m1), (((long)11ULL) >> 2), (ga6[((int)1ULL)] - ((unsigned char)3ULL)), &l32);
            }
        }
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
