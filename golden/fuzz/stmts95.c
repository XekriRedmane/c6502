// expect: 24470
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
_BitInt(9) g1 = ((_BitInt(9))128ULL);
volatile int g2 = ((int)256ULL);
volatile _BitInt(9) z3 [[c6502::zeropage]] = ((_BitInt(9))1ULL);
unsigned int z4 [[c6502::zeropage]] = ((unsigned int)127ULL);
_BitInt(9) ga5[4] = {((_BitInt(9))256ULL), ((_BitInt(9))128ULL), ((_BitInt(9))1ULL), ((_BitInt(9))255ULL)};
struct S_gs6 { unsigned char pad; int m0; unsigned int m1; };
struct S_gs6 gs6 = {0, ((int)128ULL), ((unsigned int)3ULL)};
_BitInt(9) *gp7 = &g1;
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
_BitInt(9) f8(signed char p9) {
    static long s11 = ((long)1ULL);
    unsigned int l10 = ((unsigned int)256ULL);
    ga5[((int)3ULL)] = gs6.m0;
    mix(((z4 |= 3) + g1));
    (ga5[((int)0ULL)] -= 3);
    mix((p9 & (*gp7)));
    mix(((*gp7) |= 3));
    z3 = (gs6.m1 - (s11++));
    return (((signed char)0ULL) ? gs6.m1 : ((signed char)10ULL));
}
unsigned char f12(unsigned char *pp13) {
    static unsigned long long s15 = ((unsigned long long)12ULL);
    unsigned char l14 = ((unsigned char)2ULL);
    (l14++);
    for (unsigned char i16 = 0; i16 < 2; i16++) {
        g1 = ((unsigned char)255ULL);
        switch ((((int)((gs6.m0 ^= 3))) & ((int)7ULL))) {
        case 1:
            (g2 &= 5);
            break;
        case 2:
            g1 = (i16 ^ (--gs6.m1));
            continue;
            break;
        case 3:
            s15 = (++z3);
            break;
        case 4:
            break;
        default:
            z4 = (!ga5[((int)3ULL)]);
        }
        switch ((((int)(i16)) & ((int)7ULL))) {
        case 3:
            if ((ga5[((int)3ULL)] & ga5[((int)0ULL)])) {
                break;
            }
            break;
        default:
            ;
        }
    }
    return ((*pp13) - s15);
}
int main(void) {
    unsigned char l17 = ((unsigned char)3ULL);
    unsigned long long l18 = ((unsigned long long)255ULL);
    int l19 = ((int)0ULL);
    (l17++);
    if ((z3 <<= 4)) {
        (--z4);
        mix((l17 &= 129));
    }
    mix(f12(&l17));
    g2 = (gs6.m1 >>= 3);
    l19 = (g2 - z3);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
