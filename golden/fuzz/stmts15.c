// expect: 11714
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
int g1 = ((int)43598ULL);
volatile long g2 = ((long)127ULL);
volatile int g3 = ((int)0ULL);
unsigned char g4 = ((unsigned char)1ULL);
signed char z5 [[c6502::zeropage]] = ((signed char)128ULL);
unsigned char ga6[4] = {((unsigned char)255ULL), ((unsigned char)127ULL), ((unsigned char)1ULL), ((unsigned char)255ULL)};
struct S_gs7 { unsigned char pad; _BitInt(9) m0; _BitInt(7) m1; };
struct S_gs7 gs7 = {0, ((_BitInt(9))127ULL), ((_BitInt(7))127ULL)};
int *gp8 = &g1;
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
long f9(void) {
    long l10 = ((long)9ULL);
    unsigned char w11;
    unsigned char w12;
    w11 = 0;
    while (w11++ < 4) {
        mix(((g3 -= 5) - (w11 && gs7.m0)));
    }
    if (((int)57080ULL)) {
        (z5 ^= 1);
        return (g3 & (g1 *= 3));
    }
    z5 = (gs7.m1 - z5);
    g2 = ((l10 &= 1) - (-(*gp8)));
    (++gs7.m1);
    w12 = 0;
    do {
        if ((gs7.m0 - ((unsigned int)0ULL))) {
            mix((w12 <= g1));
            ga6[(w12 & ((int)3ULL))] = (g4 -= 1);
        }
    } while (++w12 < 1);
    return ga6[((int)2ULL)];
}
unsigned long f13(unsigned char p14) {
    int l15 = ((int)65535ULL);
    volatile unsigned char l16 = ((unsigned char)0ULL);
    (gs7.m1++);
    switch ((((int)((g4 - ((int)0ULL)))) & ((int)7ULL))) {
    case 0:
        l15 = ((int)((ga6[((int)3ULL)] * g2)));
        break;
        break;
    case 2:
        ;
    case 3:
        break;
        ((*gp8) ^= 1);
        break;
    }
    for (unsigned char i17 = 0; i17 < 1; i17++) {
        (gs7.m0--);
        mix((l16 >= ((unsigned char)255ULL)));
    }
    return p14;
}
int main(void) {
    volatile int l18 = ((int)0ULL);
    (z5++);
    if ((gs7.m1 &= 5)) {
        if ((gs7.m1 + gs7.m1)) {
            mix(((gs7.m0 |= 255) - (*gp8)));
        }
        mix(f13((gs7.m0 - g1)));
        mix((z5 |= 129));
    } else {
        mix(f13((((unsigned int)32768ULL) ^ gs7.m1)));
    }
    mix(g2);
    (z5++);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
