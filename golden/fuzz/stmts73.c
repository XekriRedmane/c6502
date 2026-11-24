// expect: 590
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile long g1 = ((long)32767ULL);
int g2 = ((int)0ULL);
volatile int g3 = ((int)32767ULL);
volatile unsigned int g4 = ((unsigned int)0ULL);
unsigned long z5 [[c6502::zeropage]] = ((unsigned long)0ULL);
unsigned long long z6 [[c6502::zeropage]] = ((unsigned long long)1ULL);
signed char ga7[4] = {((signed char)255ULL), ((signed char)128ULL), ((signed char)0ULL), ((signed char)0ULL)};
struct S_gs8 { unsigned char pad; long m0; unsigned long long m1; };
struct S_gs8 gs8 = {0, ((long)2147483648ULL), ((unsigned long long)255ULL)};
unsigned long *gp9 = &z5;
unsigned long long *gp10 = &z6;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(g4);
}
void fin1(void) {
    mix(z5);
    mix(z6);
    mix(ga7[((int)0ULL)]);
    mix(ga7[((int)1ULL)]);
}
void fin2(void) {
    mix(ga7[((int)2ULL)]);
    mix(ga7[((int)3ULL)]);
    mix(gs8.m0);
    mix(gs8.m1);
}
unsigned _BitInt(7) f11(_BitInt(37) p12, unsigned char p13, unsigned char p14, signed char *pp15) {
    volatile signed char l16 = ((signed char)255ULL);
    signed char l17 = ((signed char)194ULL);
    _BitInt(37) l18 = ((_BitInt(37))15ULL);
    (z6 *= 3);
    mix(gs8.m1);
    switch ((((int)(((unsigned _BitInt(7))(((_BitInt(37))79444036292ULL))))) & ((int)7ULL))) {
    case 1:
        ;
    case 2:
        break;
    case 5:
        g3 = (p13 | (l16--));
        break;
    default:
        mix(((gs8.m0 &= 1) & g4));
    }
    (g1++);
    if ((((unsigned int)1ULL) | (g1 >>= 1))) {
        mix((z6 - l18));
        ((*pp15) &= 90);
        mix((((unsigned long)256ULL) >> 4));
    } else {
        g2 = ((!gs8.m1) >= (~ga7[((int)2ULL)]));
    }
    return (g3 >> 4);
}
int main(void) {
    unsigned long long l19 = ((unsigned long long)65535ULL);
    volatile unsigned int l20 = ((unsigned int)29614ULL);
    unsigned int l21 = ((unsigned int)2ULL);
    signed char l22 = ((signed char)12ULL);
    unsigned char w23;
    unsigned char w24;
    mix((l22 & z5));
    switch ((((int)(((g4 <<= 6) ^ z5))) & ((int)7ULL))) {
    case 0:
        break;
    case 2:
        mix(f11((*gp10), (*gp9), ((long)(g4)), &l22));
        (g1++);
    case 4:
        ;
    case 6:
        f11(ga7[((int)1ULL)], l20, (l22 ^ gs8.m0), &l22);
        (z5 -= 90);
        break;
    default:
        w23 = 0;
        do {
            z5 = (g3 | (z5 << 2));
            (l22 &= 3);
            w24 = 0;
            while (w24++ < 1) {
                g1 = ((--g3) & (((int)0ULL) & gs8.m1));
                g2 = (((unsigned long)(g2)) ? l19 : z6);
            }
        } while (++w23 < 4);
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
