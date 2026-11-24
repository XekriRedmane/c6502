// expect: 15083
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
signed char g1 = ((signed char)255ULL);
volatile int g2 = ((int)128ULL);
signed char z3 [[c6502::zeropage]] = ((signed char)255ULL);
volatile long long z4 [[c6502::zeropage]] = ((long long)32767ULL);
long ga5[4] = {((long)65536ULL), ((long)255ULL), ((long)65535ULL), ((long)13ULL)};
struct S_gs6 { unsigned char pad; unsigned int m0; signed char m1; };
struct S_gs6 gs6 = {0, ((unsigned int)256ULL), ((signed char)3ULL)};
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(z3);
    mix(z4);
}
void fin1(void) {
    mix(ga5[((int)0ULL)]);
    mix(ga5[((int)1ULL)]);
    mix(ga5[((int)2ULL)]);
    mix(ga5[((int)3ULL)]);
}
void fin2(void) {
    mix(gs6.m0);
    mix(gs6.m1);
}
int f7(unsigned char p8, int p9, long long p10, unsigned long long *pp11) {
    static unsigned int s13 = ((unsigned int)32768ULL);
    unsigned char l12 = ((unsigned char)206ULL);
    ga5[((int)0ULL)] = (z3 * gs6.m1);
    (z3 |= 3);
    if ((--gs6.m1)) {
        switch ((((int)(((g1 ? g1 : ((long)32768ULL)) + (s13 <<= 2)))) & ((int)7ULL))) {
        case 0:
            ;
        case 2:
            break;
        default:
            break;
        }
        mix(((!l12) * (s13 << 0)));
        mix((gs6.m0 &= 129));
    }
    mix(((g2 *= 5) ^ p10));
    return s13;
}
long f14(void) {
    static unsigned int s18 = ((unsigned int)127ULL);
    long l15 = ((long)65536ULL);
    int l16 = ((int)1ULL);
    unsigned long long l17 = ((unsigned long long)3ULL);
    unsigned char w19;
    unsigned char w20;
    unsigned char w21;
    l17 = ((g2 -= 5) ^ gs6.m0);
    w19 = 0;
    while (w19++ < 4) {
        z3 = ((unsigned int)3ULL);
        return (l17 || l15);
        w20 = 0;
        while (w20++ < 3) {
            return ((unsigned int)((((int)32767ULL) && w19)));
            switch ((((int)((g2 >> 4))) & ((int)7ULL))) {
            case 7:
                ;
            default:
                mix(gs6.m1);
            }
            g1 = ((l15 >>= 3) | gs6.m0);
        }
    }
    w21 = 0;
    while (w21++ < 3) {
        if ((++gs6.m0)) {
            l16 = (gs6.m0 << 4);
        }
    }
    return l15;
}
long f22(unsigned char p23, int p24) {
    static unsigned char s27 = ((unsigned char)0ULL);
    volatile unsigned int l25 = ((unsigned int)127ULL);
    unsigned long long l26 = ((unsigned long long)6ULL);
    (l25++);
    gs6.m1 = ((z4 % ((int)7ULL)) << 7);
    f7(gs6.m1, (!l26), z3, &l26);
    return ((unsigned int)0ULL);
}
int main(void) {
    signed char l28 = ((signed char)0ULL);
    unsigned int l29 = ((unsigned int)255ULL);
    unsigned long long l30 = ((unsigned long long)0ULL);
    mix(z4);
    if ((--l29)) {
        l28 = f14();
    }
    for (unsigned char i31 = 0; i31 < 4; i31++) {
        switch ((((int)(((z3 &= 129) | g2))) & ((int)7ULL))) {
        case 0:
            l29 = f14();
        case 3:
            ;
        case 4:
            f22((i31 < i31), z3);
        case 7:
            l29 = ((signed char)128ULL);
            break;
        }
    }
    f22((g2 % ((int)6ULL)), ((int)1ULL));
    switch ((((int)((((unsigned int)128ULL) | l30))) & ((int)7ULL))) {
    case 0:
        (--l28);
    case 5:
        l29 = (g2 - (ga5[((int)0ULL)] &= 1));
    default:
        ;
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
