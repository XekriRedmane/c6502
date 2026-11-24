// expect: 18747
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned long long g1 = ((unsigned long long)32768ULL);
unsigned _BitInt(8) g2 = ((unsigned _BitInt(8))0ULL);
unsigned char g3 = ((unsigned char)128ULL);
int z4 [[c6502::zeropage]] = ((int)3ULL);
signed char ga5[4] = {((signed char)0ULL), ((signed char)15ULL), ((signed char)0ULL), ((signed char)2ULL)};
struct S_gs6 { unsigned char pad; unsigned char m0; unsigned long long m1; };
struct S_gs6 gs6 = {0, ((unsigned char)255ULL), ((unsigned long long)2147483648ULL)};
int *gp7 = &z4;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
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
signed char f8(int p9, long p10) {
    unsigned long long l11 = ((unsigned long long)255ULL);
    unsigned char l12 = ((unsigned char)0ULL);
    switch ((((int)(p10)) & ((int)7ULL))) {
    case 1:
        mix(g3);
        (gs6.m1 *= 3);
        break;
    case 3:
        break;
    case 7:
        for (unsigned char i13 = 0; i13 < 2; i13++) {
            if (((l12 <<= 1) + g1)) {
                return (gs6.m0 -= 129);
                l11 = (z4 *= 255);
            }
            switch ((((int)((((unsigned char)255ULL) - ((int)(i13))))) & ((int)7ULL))) {
            case 0:
                break;
            default:
                (l12 >>= 7);
            }
        }
        g1 = (gs6.m0--);
    default:
        mix((g3 | (gs6.m1--)));
        switch ((((int)((((unsigned char)0ULL) / ((int)7ULL)))) & ((int)7ULL))) {
        case 3:
            break;
        case 4:
            break;
        case 6:
            break;
        }
    }
    return p10;
}
int f14(unsigned _BitInt(8) p15, long p16, signed char p17, unsigned char *pp18) {
    volatile signed char l19 = ((signed char)0ULL);
    g3 = (gs6.m0 |= 255);
    ((*pp18) |= 3);
    for (unsigned char i20 = 0; i20 < 3; i20++) {
        mix(((unsigned int)32767ULL));
    }
    g1 = (gs6.m1 += 3);
    mix(f8(p16, l19));
    return (((int)32768ULL) / ((int)1ULL));
}
int main(void) {
    unsigned _BitInt(30) l21 = ((unsigned _BitInt(30))1ULL);
    volatile int l22 = ((int)0ULL);
    unsigned int l23 = ((unsigned int)65535ULL);
    unsigned char l24 = ((unsigned char)80ULL);
    f8((((unsigned long)127ULL) * (*gp7)), ((unsigned char)127ULL));
    if ((((unsigned long)32767ULL) | (g2 &= 3))) {
        gs6.m1 = (ga5[((int)0ULL)] &= 129);
        mix((l22 >> 3));
        mix((ga5[((int)0ULL)]--));
    } else {
        z4 = (ga5[((int)0ULL)] |= 255);
    }
    mix(((*gp7) + (l23 <<= 2)));
    g2 = (((unsigned int)0ULL) | (g3 += 90));
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
