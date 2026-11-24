// expect: 10009
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
signed char g1 = ((signed char)0ULL);
signed char g2 = ((signed char)172ULL);
unsigned char g3 = ((unsigned char)255ULL);
long g4 = ((long)65535ULL);
int z5 [[c6502::zeropage]] = ((int)0ULL);
_BitInt(29) z6 [[c6502::zeropage]] = ((_BitInt(29))536870911ULL);
int z7 [[c6502::zeropage]] = ((int)65535ULL);
long ga8[4] = {((long)127ULL), ((long)2ULL), ((long)2147483648ULL), ((long)2ULL)};
struct S_gs9 { unsigned char pad; unsigned int m0; _BitInt(33) m1; };
struct S_gs9 gs9 = {0, ((unsigned int)10ULL), ((_BitInt(33))12ULL)};
_BitInt(29) *gp10 = &z6;
signed char *gp11 = &g2;
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
unsigned int f12(_BitInt(29) p13, _BitInt(29) p14, unsigned char *pp15) {
    static unsigned char s18 = ((unsigned char)0ULL);
    volatile unsigned int l16 = ((unsigned int)65535ULL);
    volatile unsigned int l17 = ((unsigned int)127ULL);
    if ((((unsigned char)0ULL) - (p14 += 1))) {
        (*gp10) = (p13 | (++ga8[((int)1ULL)]));
    }
    mix((((signed char)(ga8[((int)3ULL)])) + (p13 <<= 1)));
    p14 = (z5 ^= 90);
    z5 = ((*pp15) | g1);
    g3 = (z7 & ((*pp15)++));
    p13 = ((g3 -= 90) ^ ((unsigned char)255ULL));
    return (gs9.m1 != l17);
}
unsigned int f19(long p20, unsigned int p21, int p22, unsigned long *pp23) {
    static unsigned int s27 = ((unsigned int)3ULL);
    _BitInt(33) l24 = ((_BitInt(33))65535ULL);
    volatile int l25 = ((int)32767ULL);
    unsigned char l26 = ((unsigned char)0ULL);
    unsigned char w28;
    l26 = g1;
    w28 = 0;
    while (w28++ < 1) {
        if ((((signed char)127ULL) - (gs9.m1++))) {
            switch ((((int)(((gs9.m0 != ((_BitInt(29))2ULL)) <= ((int)(((unsigned char)1ULL)))))) & ((int)7ULL))) {
            case 2:
                break;
            case 3:
                break;
            case 4:
                (*gp10) = (p20 <<= 6);
                (g1 *= 90);
            default:
                break;
            }
        }
    }
    mix((p22 <<= 0));
    l24 = f12((g4 >> 3), ((unsigned char)2ULL), &l26);
    return (((_BitInt(33))2ULL) == (*pp23));
}
_BitInt(33) f29(signed char p30, signed char p31) {
    unsigned int l32 = ((unsigned int)32767ULL);
    long l33 = ((long)128ULL);
    unsigned char l34 = ((unsigned char)3ULL);
    unsigned long l35 = ((unsigned long)127ULL);
    mix((z6 - (g3 *= 90)));
    mix((g1 <<= 6));
    g2 = (g3 & gs9.m0);
    return (l33 >= z6);
}
int main(void) {
    signed char l36 = ((signed char)237ULL);
    _BitInt(33) l37 = ((_BitInt(33))1ULL);
    _BitInt(33) l38 = ((_BitInt(33))65535ULL);
    unsigned char l39 = ((unsigned char)128ULL);
    unsigned long l40 = ((unsigned long)3ULL);
    mix((((*gp11) & z6) & g2));
    if ((g4 >>= 0)) {
        mix(f12((!l39), (l39 & (*gp10)), &l39));
        (g2--);
        (g2 &= 1);
    } else {
        switch ((((int)((gs9.m0 <<= 4))) & ((int)7ULL))) {
        case 4:
            break;
        }
    }
    g3 = (((_BitInt(33))3ULL) + ((signed char)255ULL));
    if (((g3 -= 1) - g1)) {
        for (unsigned char i41 = 0; i41 < 3; i41++) {
            switch ((((int)((z6--))) & ((int)7ULL))) {
            case 2:
                l38 = f12((i41 || g3), (l39 ? g4 : z6), &l39);
                break;
            case 6:
                ;
            case 7:
                g1 = (i41 || (i41 || gs9.m0));
            default:
                ;
            }
        }
    }
    fin0();
    fin1();
    fin2();
    fin3();
    return (int)(acc & 0x7FFF);
}
