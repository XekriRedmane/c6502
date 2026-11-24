// expect: 8719
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
long g1 = ((long)32768ULL);
unsigned long long g2 = ((unsigned long long)32767ULL);
volatile long long g3 = ((long long)2147483647ULL);
unsigned char g4 = ((unsigned char)3ULL);
unsigned char z5 [[c6502::zeropage]] = ((unsigned char)0ULL);
long ga6[4] = {((long)65536ULL), ((long)65536ULL), ((long)2147483647ULL), ((long)413939763ULL)};
struct S_gs7 { unsigned char pad; unsigned char m0; signed char m1; };
struct S_gs7 gs7 = {0, ((unsigned char)255ULL), ((signed char)255ULL)};
long *gp8 = &g1;
unsigned char *gp9 = &z5;
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
unsigned int f10(unsigned int p11, long *pp12) {
    int l13 = ((int)0ULL);
    long l14 = ((long)0ULL);
    unsigned char l15 = ((unsigned char)0ULL);
    unsigned char w16;
    w16 = 0;
    while (w16++ < 1) {
        (gs7.m0 &= 1);
        mix((w16 >> 7));
        mix((((unsigned int)32768ULL) - ((*pp12) -= 90)));
    }
    gs7.m1 = (z5--);
    mix((ga6[((int)1ULL)] - g4));
    return (((unsigned int)9ULL) ^ p11);
}
int f17(void) {
    static unsigned long s21 = ((unsigned long)1ULL);
    volatile unsigned int l18 = ((unsigned int)65535ULL);
    unsigned char l19 = ((unsigned char)255ULL);
    long l20 = ((long)32767ULL);
    mix(((unsigned long)((((unsigned char)3ULL) + s21))));
    (++gs7.m0);
    (l18--);
    if ((((unsigned int)50257ULL) >> 1)) {
        return ((gs7.m0 |= 255) ^ ((*gp9) - z5));
        mix(((g3 ^= 90) - l19));
    }
    mix(((gs7.m0 |= 90) + ((unsigned int)(g4))));
    return (g2 - ((unsigned char)0ULL));
}
int main(void) {
    unsigned long l22 = ((unsigned long)2ULL);
    signed char l23 = ((signed char)1ULL);
    long l24 = ((long)1ULL);
    if ((g1 |= 3)) {
        (l23 &= 129);
        g4 = (((*gp9) - ga6[((int)0ULL)]) | (++g2));
    } else {
        (*gp8) = (g4 * ((int)255ULL));
        z5 = ((~((unsigned char)2ULL)) & l22);
    }
    if (g4) {
        mix((g3 |= 255));
        mix(f17());
    }
    l23 = f10(g2, &l24);
    switch ((((int)((--l24))) & ((int)7ULL))) {
    case 1:
        break;
    case 4:
        if (((g2 || ga6[((int)2ULL)]) / ((int)3ULL))) {
            mix((l24 + z5));
        }
        break;
    case 5:
        break;
    case 7:
        break;
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
