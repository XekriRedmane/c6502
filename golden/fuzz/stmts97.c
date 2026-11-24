// expect: 23223
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
int g1 = ((int)0ULL);
signed char g2 = ((signed char)197ULL);
unsigned int g3 = ((unsigned int)127ULL);
long g4 = ((long)128ULL);
unsigned char z5 [[c6502::zeropage]] = ((unsigned char)0ULL);
int ga6[4] = {((int)32768ULL), ((int)65535ULL), ((int)3ULL), ((int)2ULL)};
struct S_gs7 { unsigned char pad; unsigned int m0; unsigned char m1; };
struct S_gs7 gs7 = {0, ((unsigned int)32768ULL), ((unsigned char)8ULL)};
unsigned int *gp8 = &g3;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(g4);
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
unsigned int f9(void) {
    static unsigned int s11 = ((unsigned int)0ULL);
    unsigned int l10 = ((unsigned int)2ULL);
    for (unsigned char i12 = 0; i12 < 1; i12++) {
        mix((g1 + (((unsigned long)255ULL) / ((int)4ULL))));
        for (unsigned char i13 = 0; i13 < 2; i13++) {
            if (((i12 & i12) % ((int)5ULL))) {
                (s11 -= 90);
            }
        }
        continue;
    }
    mix((*gp8));
    if ((++(*gp8))) {
        return ((s11 -= 3) & gs7.m0);
        (*gp8) = z5;
    }
    return ((*gp8) ^ z5);
}
signed char f14(unsigned char p15) {
    static unsigned int s19 = ((unsigned int)2ULL);
    unsigned int l16 = ((unsigned int)65535ULL);
    long l17 = ((long)256ULL);
    unsigned int l18 = ((unsigned int)2ULL);
    mix((gs7.m1 != l17));
    l17 = ((*gp8) | g2);
    if (((int)(((signed char)127ULL)))) {
        (gs7.m0 &= 129);
    }
    return ((signed char)255ULL);
}
int f20(void) {
    unsigned long l21 = ((unsigned long)256ULL);
    volatile long l22 = ((long)2147483648ULL);
    unsigned long l23 = ((unsigned long)3957147912ULL);
    z5 = ((g3 |= 129) ^ g1);
    l22 = (gs7.m1 & g4);
    switch ((((int)(g4)) & ((int)7ULL))) {
    case 4:
        mix(f14(g4));
        mix(((--g2) | (((unsigned char)1ULL) << 3)));
        break;
    case 5:
        gs7.m1 = (gs7.m0 <<= 2);
        if (((((int)0ULL) & g4) | (gs7.m1--))) {
            (*gp8) = (l21 ^ (z5++));
            g2 = (ga6[((int)0ULL)] % ((int)1ULL));
        }
        break;
    default:
        break;
        f14((((unsigned int)14755ULL) || ((unsigned char)0ULL)));
    }
    for (unsigned char i24 = 0; i24 < 4; i24++) {
    }
    return ((long long)2ULL);
}
int main(void) {
    int l25 = ((int)127ULL);
    signed char l26 = ((signed char)255ULL);
    f14((gs7.m1 & z5));
    mix(f9());
    mix(f20());
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
