// expect: 25449
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
int g1 = ((int)65535ULL);
unsigned int g2 = ((unsigned int)32767ULL);
int z3 [[c6502::zeropage]] = ((int)127ULL);
long ga4[4] = {((long)2147483647ULL), ((long)2147483647ULL), ((long)127ULL), ((long)2ULL)};
struct S_gs5 { unsigned char pad; unsigned char m0; unsigned int m1; };
struct S_gs5 gs5 = {0, ((unsigned char)127ULL), ((unsigned int)0ULL)};
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(z3);
    mix(ga4[((int)0ULL)]);
}
void fin1(void) {
    mix(ga4[((int)1ULL)]);
    mix(ga4[((int)2ULL)]);
    mix(ga4[((int)3ULL)]);
    mix(gs5.m0);
}
void fin2(void) {
    mix(gs5.m1);
}
unsigned int f6(unsigned int p7, _BitInt(23) *pp8) {
    static int s12 = ((int)255ULL);
    unsigned int l9 = ((unsigned int)256ULL);
    signed char l10 = ((signed char)3ULL);
    signed char l11 = ((signed char)0ULL);
    (p7 >>= 0);
    gs5.m0 = (((int)(((_BitInt(23))128ULL))) & ga4[((int)1ULL)]);
    mix((((unsigned int)0ULL) & (p7 & ((int)128ULL))));
    (gs5.m0--);
    l9 = (((unsigned long)2147483647ULL) % ((int)3ULL));
    return ((signed char)4ULL);
}
signed char f13(unsigned char p14, long p15) {
    unsigned int l16 = ((unsigned int)0ULL);
    _BitInt(23) l17 = ((_BitInt(23))4386864ULL);
    unsigned char w18;
    unsigned char w19;
    if ((((unsigned int)11428ULL) || (l17 + p15))) {
        switch ((((int)((ga4[((int)1ULL)] >>= 1))) & ((int)7ULL))) {
        case 1:
            w18 = 0;
            do {
                mix((gs5.m1 *= 255));
                (ga4[((int)3ULL)]++);
                continue;
            } while (++w18 < 2);
            break;
        case 2:
            z3 = ((l16 - p14) >> 4);
            w19 = 0;
            do {
                break;
                (z3 ^= 129);
                continue;
                l17 = ((--l16) + (((int)255ULL) <= p15));
            } while (++w19 < 3);
        case 4:
            ;
        default:
            ;
        }
    }
    return (p14 - ((unsigned long)3ULL));
}
unsigned long f20(unsigned long long p21, unsigned int *pp22) {
    unsigned char l23 = ((unsigned char)0ULL);
    _BitInt(23) l24 = ((_BitInt(23))32768ULL);
    unsigned char w25;
    switch ((((int)((gs5.m0--))) & ((int)7ULL))) {
    case 1:
        break;
    case 2:
        w25 = 0;
        do {
            break;
            break;
        } while (++w25 < 4);
    default:
        l24 = ((l24 / ((int)7ULL)) & (gs5.m1++));
    }
    if ((~g2)) {
        p21 = (gs5.m0++);
    }
    f13((gs5.m0 & ((unsigned char)0ULL)), gs5.m1);
    mix((g2 & ga4[((int)2ULL)]));
    mix((gs5.m0 *= 90));
    return (z3 - g2);
}
int main(void) {
    _BitInt(23) l26 = ((_BitInt(23))2ULL);
    _BitInt(23) l27 = ((_BitInt(23))65536ULL);
    signed char l28 = ((signed char)0ULL);
    unsigned int l29 = ((unsigned int)7ULL);
    unsigned char w30;
    switch ((((int)((g2 << 3))) & ((int)7ULL))) {
    case 0:
        break;
    case 2:
        switch ((((int)((ga4[((int)3ULL)] == l28))) & ((int)7ULL))) {
        case 0:
            break;
        case 2:
            break;
        case 4:
            ;
        case 6:
            w30 = 0;
            do {
                break;
                break;
            } while (++w30 < 1);
            mix(f6(g1, &l26));
        default:
            if ((l26 >>= 5)) {
                break;
            } else {
                g1 = (l26 <= l27);
            }
            l26 = f13((gs5.m0 < l26), (((unsigned long)128ULL) + ((unsigned int)32767ULL)));
        }
        break;
    case 4:
        mix((g2 <<= 4));
        l28 = f13(((_BitInt(2))3ULL), (g1 ? l28 : l26));
    case 5:
        break;
    default:
        ;
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
