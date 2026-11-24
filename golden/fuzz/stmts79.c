// expect: 26763
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
signed char g1 = ((signed char)1ULL);
int g2 = ((int)127ULL);
unsigned char ga3[4] = {((unsigned char)127ULL), ((unsigned char)0ULL), ((unsigned char)255ULL), ((unsigned char)10ULL)};
struct S_gs4 { unsigned char pad; long m0; unsigned long m1; };
struct S_gs4 gs4 = {0, ((long)32767ULL), ((unsigned long)4ULL)};
signed char *gp5 = &g1;
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
long f7(unsigned int p8, unsigned int p9, unsigned int p10) {
    static volatile unsigned char s13 = ((unsigned char)127ULL);
    signed char l11 = ((signed char)0ULL);
    unsigned int l12 = ((unsigned int)65535ULL);
    (*gp5) = (ga3[((int)3ULL)] - gs4.m1);
    s13 = (gs4.m1 <<= 4);
    if ((gs4.m0 - (*gp6))) {
        g1 = (l11 += 3);
        mix((((*gp5) *= 5) ^ p10));
        return (((long)0ULL) | (s13 += 255));
    }
    (p9 |= 255);
    return l11;
}
signed char f14(unsigned char p15) {
    static volatile unsigned long s18 = ((unsigned long)32767ULL);
    unsigned char l16 = ((unsigned char)2ULL);
    unsigned char l17 = ((unsigned char)0ULL);
    unsigned char w19;
    l17 = f7((((unsigned char)1ULL) - g1), l16, g1);
    (g1 >>= 1);
    if ((--g1)) {
        mix((++g1));
    }
    w19 = 0;
    do {
        mix(((g2 &= 5) + ga3[((int)2ULL)]));
    } while (++w19 < 2);
    return (((unsigned char)1ULL) * (*gp5));
}
int main(void) {
    unsigned char l20 = ((unsigned char)128ULL);
    signed char l21 = ((signed char)3ULL);
    l20 = (l20 >> 4);
    for (unsigned char i22 = 0; i22 < 2; i22++) {
        mix((((*gp5)++) + (*gp6)));
        switch ((((int)((g1 += 5))) & ((int)7ULL))) {
        case 2:
            break;
        default:
            continue;
            (*gp6) = (gs4.m1 ? (*gp6) : (l20 & i22));
        }
        switch ((((int)(i22)) & ((int)7ULL))) {
        case 1:
            ga3[((int)3ULL)] = (l20 &= 129);
            g2 = ((unsigned int)(((*gp6) / ((int)6ULL))));
        case 7:
            (++gs4.m1);
            if ((((int)8ULL) | (g1 >>= 4))) {
                break;
            }
            break;
        default:
            ;
        }
    }
    fin0();
    fin1();
    return (int)(acc & 0x7FFF);
}
