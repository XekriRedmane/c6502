// expect: 16621
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile unsigned int g1 = ((unsigned int)0ULL);
unsigned long long g2 = ((unsigned long long)10ULL);
unsigned char g3 = ((unsigned char)255ULL);
unsigned int z4 [[c6502::zeropage]] = ((unsigned int)128ULL);
unsigned char ga5[4] = {((unsigned char)255ULL), ((unsigned char)255ULL), ((unsigned char)128ULL), ((unsigned char)0ULL)};
struct S_gs6 { unsigned char pad; unsigned long long m0; unsigned char m1; };
struct S_gs6 gs6 = {0, ((unsigned long long)2ULL), ((unsigned char)255ULL)};
unsigned char *gp7 = &g3;
unsigned long long *gp8 = &g2;
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
int f9(unsigned int p10) {
    static int s12 = ((int)62664ULL);
    unsigned long l11 = ((unsigned long)32768ULL);
    (l11 *= 5);
    if (((unsigned int)65535ULL)) {
        mix((!ga5[((int)1ULL)]));
        if ((((int)2ULL) + (-(*gp7)))) {
            return (z4 <<= 1);
            switch ((((int)((((unsigned char)2ULL) + g2))) & ((int)7ULL))) {
            case 2:
                break;
            case 5:
                ;
            default:
                g2 = (++g3);
            }
            (s12 &= 1);
        }
    } else {
        if ((((unsigned int)0ULL) & (*gp8))) {
            (++g3);
            l11 = (gs6.m1 || (*gp7));
        } else {
            mix((g3 ^ s12));
        }
    }
    return (z4 ^ ((signed char)1ULL));
}
int main(void) {
    unsigned char l13 = ((unsigned char)128ULL);
    long l14 = ((long)256ULL);
    unsigned int l15 = ((unsigned int)256ULL);
    unsigned char w16;
    g2 = (ga5[((int)0ULL)] % ((int)4ULL));
    switch ((((int)((g2 & ((*gp7) <<= 4)))) & ((int)7ULL))) {
    case 0:
        break;
    case 4:
        break;
        break;
    default:
        switch ((((int)(((*gp7) != ga5[((int)1ULL)]))) & ((int)7ULL))) {
        case 5:
            gs6.m1 = (((unsigned char)(gs6.m1)) * g3);
            g1 = (gs6.m0 |= 129);
            break;
        }
        switch ((((int)((l15 <= ((unsigned int)34650ULL)))) & ((int)7ULL))) {
        case 2:
            break;
        case 6:
            l15 = (~gs6.m0);
            break;
        case 7:
            ;
        }
    }
    (gs6.m0 >>= 7);
    w16 = 0;
    while (w16++ < 4) {
        g2 = (l13 >>= 1);
    }
    mix((++gs6.m0));
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
