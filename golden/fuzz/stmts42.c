// expect: 12960
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile signed char g1 = ((signed char)0ULL);
volatile unsigned char g2 = ((unsigned char)0ULL);
int g3 = ((int)0ULL);
unsigned long g4 = ((unsigned long)2ULL);
int z5 [[c6502::zeropage]] = ((int)127ULL);
unsigned char z6 [[c6502::zeropage]] = ((unsigned char)3ULL);
unsigned int z7 [[c6502::zeropage]] = ((unsigned int)2ULL);
unsigned int ga8[4] = {((unsigned int)127ULL), ((unsigned int)0ULL), ((unsigned int)65535ULL), ((unsigned int)32767ULL)};
struct S_gs9 { unsigned char pad; long long m0; unsigned _BitInt(43) m1; };
struct S_gs9 gs9 = {0, ((long long)12ULL), ((unsigned _BitInt(43))3968850721993ULL)};
int *gp10 = &z5;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(g4);
}
void fin1(void) {
    mix(z5);
    mix(z6);
    mix(z7);
    mix(ga8[((int)0ULL)]);
}
void fin2(void) {
    mix(ga8[((int)1ULL)]);
    mix(ga8[((int)2ULL)]);
    mix(ga8[((int)3ULL)]);
    mix(gs9.m0);
}
void fin3(void) {
    mix(gs9.m1);
}
unsigned int f11(signed char p12, unsigned int p13, signed char p14, unsigned char *pp15) {
    static unsigned _BitInt(25) s18 = ((unsigned _BitInt(25))2ULL);
    signed char l16 = ((signed char)3ULL);
    signed char l17 = ((signed char)0ULL);
    unsigned char w19;
    unsigned char w20;
    g1 = ((p13 <<= 7) ^ ((unsigned _BitInt(25))2ULL));
    mix(((p14 |= 90) & ((signed char)1ULL)));
    w19 = 0;
    do {
        if ((l16 / ((int)5ULL))) {
            switch ((((int)((((unsigned int)1ULL) | p14))) & ((int)7ULL))) {
            case 0:
                break;
            case 4:
                break;
                break;
            default:
                l16 = (g3--);
                (p12++);
            }
            (*gp10) = (((long long)127ULL) + (++g4));
        }
    } while (++w19 < 2);
    z5 = (g2 <<= 2);
    w20 = 0;
    do {
        g1 = (p14 ^= 3);
    } while (++w20 < 3);
    return (((unsigned char)1ULL) | l16);
}
int main(void) {
    unsigned char l21 = ((unsigned char)255ULL);
    unsigned int l22 = ((unsigned int)32767ULL);
    long l23 = ((long)65536ULL);
    g4 = ((*gp10) | z7);
    switch ((((int)(l22)) & ((int)7ULL))) {
    case 0:
        ;
    case 1:
        (ga8[((int)0ULL)] |= 1);
    case 3:
        (--gs9.m1);
        l22 = (((*gp10)++) ^ gs9.m1);
        break;
    case 7:
        gs9.m0 = z7;
        f11((~g3), g1, l21, &l21);
    default:
        (++ga8[((int)0ULL)]);
        mix((l23 ^ g1));
    }
    switch ((((int)((((int)128ULL) - (l23 *= 5)))) & ((int)7ULL))) {
    case 6:
        break;
    default:
        if ((z7 &= 129)) {
            l21 = ((*gp10) >>= 4);
        }
    }
    fin0();
    fin1();
    fin2();
    fin3();
    return (int)(acc & 0x7FFF);
}
