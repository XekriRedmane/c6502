// expect: 25879
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned long long g1 = ((unsigned long long)256ULL);
int g2 = ((int)65535ULL);
int g3 = ((int)0ULL);
signed char z4 [[c6502::zeropage]] = ((signed char)255ULL);
unsigned int z5 [[c6502::zeropage]] = ((unsigned int)0ULL);
unsigned char ga6[4] = {((unsigned char)255ULL), ((unsigned char)87ULL), ((unsigned char)3ULL), ((unsigned char)2ULL)};
struct S_gs7 { unsigned char pad; unsigned char m0; int m1; };
struct S_gs7 gs7 = {0, ((unsigned char)0ULL), ((int)256ULL)};
signed char *gp8 = &z4;
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
unsigned int f9(unsigned int p10, unsigned char p11) {
    static int s14 = ((int)0ULL);
    int l12 = ((int)255ULL);
    unsigned int l13 = ((unsigned int)32768ULL);
    unsigned char w15;
    g1 = (z5 & gs7.m0);
    mix(((unsigned int)1ULL));
    mix(((unsigned char)(((unsigned int)0ULL))));
    mix((((signed char)255ULL) ^ (g1--)));
    if ((++g1)) {
        z4 = ((g1 >>= 6) + z4);
        w15 = 0;
        while (w15++ < 2) {
            (g3 >>= 5);
        }
    }
    return (s14 + ga6[((int)0ULL)]);
}
signed char f16(unsigned int *pp17) {
    static unsigned char s19 = ((unsigned char)128ULL);
    signed char l18 = ((signed char)128ULL);
    unsigned char w20;
    unsigned char w21;
    unsigned char w23;
    w20 = 0;
    while (w20++ < 1) {
        break;
        l18 = (gs7.m0 -= 90);
    }
    w21 = 0;
    do {
        for (unsigned char i22 = 0; i22 < 1; i22++) {
            return ((unsigned char)(ga6[((int)1ULL)]));
        }
        switch ((((int)(((unsigned int)(w21)))) & ((int)7ULL))) {
        case 2:
            mix((g1 <= ((signed char)128ULL)));
            break;
        case 4:
            break;
        default:
            w23 = 0;
            while (w23++ < 3) {
                (z5 -= 3);
            }
        }
        (gs7.m1 *= 1);
        if (((unsigned int)(g3))) {
        }
    } while (++w21 < 4);
    return (gs7.m1 + l18);
}
signed char f24(int p25, unsigned int p26, unsigned char p27, signed char *pp28) {
    volatile signed char l29 = ((signed char)2ULL);
    unsigned int l30 = ((unsigned int)32767ULL);
    mix((p26 ^ (z5 <<= 7)));
    ((*gp8)++);
    (*gp8) = ((*gp8) > gs7.m1);
    return (l29 | ((long)29213894ULL));
}
int main(void) {
    unsigned long l31 = ((unsigned long)1ULL);
    unsigned long l32 = ((unsigned long)0ULL);
    unsigned int l33 = ((unsigned int)1ULL);
    signed char l34 = ((signed char)0ULL);
    f24(l33, (g1 ? ((unsigned char)0ULL) : gs7.m0), (z5 ^ (*gp8)), &l34);
    (gs7.m0 -= 255);
    mix(((unsigned long long)65535ULL));
    gs7.m0 = (-(*gp8));
    (g1 *= 90);
    for (unsigned char i35 = 0; i35 < 4; i35++) {
        continue;
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
