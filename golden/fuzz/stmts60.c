// expect: 24961
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned int g1 = ((unsigned int)255ULL);
signed char g2 = ((signed char)0ULL);
unsigned char g3 = ((unsigned char)255ULL);
unsigned char g4 = ((unsigned char)69ULL);
int z5 [[c6502::zeropage]] = ((int)127ULL);
unsigned int ga6[4] = {((unsigned int)128ULL), ((unsigned int)2ULL), ((unsigned int)3ULL), ((unsigned int)65535ULL)};
struct S_gs7 { unsigned char pad; unsigned int m0; unsigned char m1; };
struct S_gs7 gs7 = {0, ((unsigned int)0ULL), ((unsigned char)127ULL)};
unsigned char *gp8 = &g4;
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
signed char f9(unsigned char p10, unsigned char p11, signed char *pp12) {
    static unsigned char s14 = ((unsigned char)255ULL);
    volatile unsigned int l13 = ((unsigned int)65535ULL);
    mix((gs7.m1 |= 129));
    mix(((((int)0ULL) || gs7.m0) - (p10 - z5)));
    mix(gs7.m0);
    return gs7.m0;
}
signed char f15(long p16, int p17) {
    static unsigned int s21 = ((unsigned int)0ULL);
    unsigned long l18 = ((unsigned long)65535ULL);
    volatile unsigned char l19 = ((unsigned char)15ULL);
    signed char l20 = ((signed char)3ULL);
    unsigned char w22;
    (g3 <<= 7);
    if (((ga6[((int)2ULL)] -= 255) ^ ((unsigned char)0ULL))) {
        return (~p17);
        switch ((((int)((l18 >> 5))) & ((int)7ULL))) {
        case 1:
            mix((p16 == ((unsigned char)0ULL)));
            w22 = 0;
            while (w22++ < 1) {
                l19 = (p16 & l18);
                mix(z5);
                f9(((unsigned char)(g3)), (((unsigned int)127ULL) ? w22 : w22), &l20);
            }
            break;
        case 2:
            ((*gp8) >>= 0);
            if ((g3 &= 5)) {
                mix(((--p16) + (p17 + s21)));
            }
            break;
        case 6:
            break;
        }
    }
    return (ga6[((int)0ULL)] + l19);
}
int main(void) {
    unsigned char l23 = ((unsigned char)128ULL);
    signed char l24 = ((signed char)0ULL);
    unsigned char w25;
    if ((g1 >>= 6)) {
        mix((((signed char)127ULL) / ((int)4ULL)));
        w25 = 0;
        while (w25++ < 1) {
            mix((g4 & (g2 |= 90)));
        }
        ga6[((int)1ULL)] = ga6[((int)2ULL)];
    }
    mix(((g1 -= 3) - (l23 - (*gp8))));
    l23 = f9((gs7.m1 * z5), ((unsigned int)255ULL), &l24);
    f15((-ga6[((int)3ULL)]), (gs7.m0 & g4));
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
