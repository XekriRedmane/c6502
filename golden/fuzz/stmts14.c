// expect: 28554
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
_BitInt(5) g1 = ((_BitInt(5))31ULL);
unsigned long g2 = ((unsigned long)1ULL);
int z3 [[c6502::zeropage]] = ((int)256ULL);
long ga4[4] = {((long)256ULL), ((long)1ULL), ((long)2147483647ULL), ((long)2147483647ULL)};
struct S_gs5 { unsigned char pad; int m0; unsigned char m1; };
struct S_gs5 gs5 = {0, ((int)127ULL), ((unsigned char)232ULL)};
_BitInt(5) *gp6 = &g1;
int *gp7 = &z3;
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
int f8(long long p9, signed char p10, _BitInt(5) p11) {
    static int s14 = ((int)256ULL);
    volatile unsigned int l12 = ((unsigned int)0ULL);
    signed char l13 = ((signed char)3ULL);
    unsigned char w15;
    unsigned char w17;
    w15 = 0;
    while (w15++ < 1) {
        switch ((((int)(((unsigned int)(((unsigned char)(ga4[((int)0ULL)])))))) & ((int)7ULL))) {
        case 2:
            break;
        case 7:
            for (unsigned char i16 = 0; i16 < 4; i16++) {
                break;
                l12 = (++g2);
            }
            break;
        }
        switch ((((int)((w15 & w15))) & ((int)7ULL))) {
        case 0:
            break;
        case 1:
            mix(z3);
            (l13 >>= 0);
        case 4:
            mix(((gs5.m0 * z3) & (z3 & (*gp6))));
            mix(((((unsigned _BitInt(32))0ULL) << 0) ^ (s14 <<= 0)));
        }
        w17 = 0;
        while (w17++ < 3) {
            (l12 -= 5);
        }
    }
    return g2;
}
int main(void) {
    volatile unsigned char l18 = ((unsigned char)255ULL);
    unsigned char w19;
    (gs5.m1 <<= 6);
    if ((gs5.m0 |= 129)) {
        ga4[((int)1ULL)] = (gs5.m0 &= 255);
        switch ((((int)((gs5.m0 ^= 129))) & ((int)7ULL))) {
        case 0:
            g1 = z3;
            w19 = 0;
            while (w19++ < 3) {
                gs5.m1 = ((long)32768ULL);
                mix(((g2 += 1) | ((unsigned char)2ULL)));
                break;
                break;
            }
            break;
        case 1:
            switch ((((int)((l18 == (*gp6)))) & ((int)7ULL))) {
            case 6:
                break;
            default:
                ;
            }
        case 3:
            ;
        case 5:
            ;
        default:
            ;
        }
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
