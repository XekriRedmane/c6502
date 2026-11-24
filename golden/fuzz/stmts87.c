// expect: 13493
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned char g1 = ((unsigned char)127ULL);
int g2 = ((int)127ULL);
int ga3[4] = {((int)127ULL), ((int)65535ULL), ((int)2ULL), ((int)128ULL)};
struct S_gs4 { unsigned char pad; int m0; unsigned char m1; };
struct S_gs4 gs4 = {0, ((int)65535ULL), ((unsigned char)255ULL)};
unsigned char *gp5 = &g1;
int *gp6 = &g2;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(ga3[((int)0ULL)]);
    mix(ga3[((int)1ULL)]);
}
void fin1(void) {
    mix(ga3[((int)2ULL)]);
    mix(ga3[((int)3ULL)]);
    mix(gs4.m0);
    mix(gs4.m1);
}
unsigned int f7(void) {
    static signed char s10 = ((signed char)255ULL);
    unsigned int l8 = ((unsigned int)38702ULL);
    volatile signed char l9 = ((signed char)255ULL);
    (gs4.m0 |= 129);
    mix(((*gp5) *= 1));
    (*gp5) = (gs4.m1 |= 90);
    return (l9 & g2);
}
unsigned char f11(_BitInt(11) p12, unsigned int p13, int p14, unsigned char *pp15) {
    static volatile unsigned char s19 = ((unsigned char)255ULL);
    int l16 = ((int)2ULL);
    volatile unsigned long long l17 = ((unsigned long long)1ULL);
    volatile int l18 = ((int)1ULL);
    unsigned char w20;
    unsigned char w21;
    mix(((!p14) + (p14 << 1)));
    mix(((*pp15) < gs4.m0));
    w20 = 0;
    while (w20++ < 1) {
        (p14 <<= 7);
        w21 = 0;
        while (w21++ < 4) {
            mix(f7());
        }
        switch ((((int)(((p14 / ((int)2ULL)) - l16))) & ((int)7ULL))) {
        case 1:
            ;
        case 3:
            mix(l17);
            g1 = (w20 << 2);
        case 4:
            switch ((((int)(((l18 <<= 4) + l17))) & ((int)7ULL))) {
            case 0:
                mix(((*gp6) && w20));
            case 3:
                break;
            case 5:
                (ga3[((int)2ULL)] += 5);
                break;
            case 6:
                ;
            default:
                ;
            }
            break;
        }
    }
    return (((int)0ULL) / ((int)4ULL));
}
int main(void) {
    int l22 = ((int)0ULL);
    long long l23 = ((long long)65536ULL);
    unsigned int l24 = ((unsigned int)2ULL);
    unsigned char l25 = ((unsigned char)0ULL);
    (++g2);
    switch ((((int)(gs4.m0)) & ((int)7ULL))) {
    case 1:
        if ((g2 | (++(*gp5)))) {
            (*gp5) = (((*gp6) ^ gs4.m0) >> 2);
        }
        g2 = ((l22 ^= 90) & ga3[((int)0ULL)]);
    case 3:
        (++g2);
    case 7:
        ;
    default:
        l24 = f11(l25, (*gp5), ((signed char)0ULL), &l25);
    }
    mix(f7());
    (l23--);
    fin0();
    fin1();
    return (int)(acc & 0x7FFF);
}
