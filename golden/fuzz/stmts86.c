// expect: 2947
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
signed char g1 = ((signed char)128ULL);
unsigned char g2 = ((unsigned char)0ULL);
long g3 = ((long)2147483648ULL);
volatile unsigned char z4 [[c6502::zeropage]] = ((unsigned char)0ULL);
unsigned long z5 [[c6502::zeropage]] = ((unsigned long)3ULL);
unsigned char ga6[4] = {((unsigned char)3ULL), ((unsigned char)255ULL), ((unsigned char)255ULL), ((unsigned char)122ULL)};
struct S_gs7 { unsigned char pad; signed char m0; unsigned _BitInt(17) m1; };
struct S_gs7 gs7 = {0, ((signed char)0ULL), ((unsigned _BitInt(17))255ULL)};
unsigned char *gp8 = &g2;
long *gp9 = &g3;
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
_BitInt(58) f10(unsigned char p11, _BitInt(58) p12, signed char *pp13) {
    _BitInt(58) l14 = ((_BitInt(58))127ULL);
    volatile unsigned char l15 = ((unsigned char)255ULL);
    volatile int l16 = ((int)9ULL);
    unsigned char w17;
    ((*gp8) -= 1);
    if ((((int)0ULL) / ((int)5ULL))) {
        w17 = 0;
        while (w17++ < 1) {
            g2 = ((g2 | (*gp9)) / ((int)3ULL));
            mix((++l15));
        }
        mix((l14 % ((int)4ULL)));
    }
    (l16 += 3);
    return ((unsigned _BitInt(17))(gs7.m0));
}
unsigned char f18(int p19, signed char p20, unsigned int *pp21) {
    static unsigned int s24 = ((unsigned int)0ULL);
    volatile unsigned int l22 = ((unsigned int)255ULL);
    signed char l23 = ((signed char)255ULL);
    unsigned char w26;
    for (unsigned char i25 = 0; i25 < 1; i25++) {
        ((*gp8) <<= 5);
        w26 = 0;
        do {
            mix(((((unsigned char)1ULL) >> 2) / ((int)7ULL)));
            l23 = (z4 & (g1 <<= 7));
            switch ((((int)((l23 ? gs7.m1 : i25))) & ((int)7ULL))) {
            case 2:
                break;
                break;
            case 7:
                ga6[((int)0ULL)] = (s24 / ((int)1ULL));
                gs7.m1 = (i25 == (s24 + w26));
                break;
            }
        } while (++w26 < 3);
        mix(z4);
    }
    mix((z4++));
    f10((l22 << 7), (*gp8), &l23);
    return (ga6[((int)1ULL)] / ((int)5ULL));
}
int f27(unsigned long p28, int p29, _BitInt(58) p30, signed char *pp31) {
    static unsigned char s35 = ((unsigned char)0ULL);
    long l32 = ((long)255ULL);
    unsigned int l33 = ((unsigned int)65535ULL);
    signed char l34 = ((signed char)127ULL);
    unsigned char w36;
    (p28--);
    mix((~(*gp9)));
    if ((gs7.m1 > (g1 - l32))) {
        gs7.m1 = (g1 <<= 2);
        mix((z5 && (l32 % ((int)6ULL))));
        w36 = 0;
        while (w36++ < 4) {
            p30 = (g2 - ((unsigned char)255ULL));
            if (w36) {
                return ((p30 &= 5) | ((*gp8) > ((int)65535ULL)));
                f10(l32, p28, &l34);
                mix(((int)(l32)));
            } else {
                g3 = ((ga6[((int)2ULL)] ^= 3) - w36);
            }
        }
    }
    return (!l34);
}
int main(void) {
    unsigned int l37 = ((unsigned int)127ULL);
    volatile long l38 = ((long)3ULL);
    signed char l39 = ((signed char)255ULL);
    ga6[((int)0ULL)] = (g1 || ((unsigned _BitInt(17))0ULL));
    gs7.m0 = (g1 ^ (ga6[((int)0ULL)] &= 129));
    f10(((unsigned int)((*gp8))), ((unsigned long)65535ULL), &l39);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
