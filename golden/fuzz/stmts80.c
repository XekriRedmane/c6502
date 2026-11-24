// expect: 27767
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned int g1 = ((unsigned int)1ULL);
int g2 = ((int)0ULL);
signed char g3 = ((signed char)0ULL);
unsigned long g4 = ((unsigned long)2ULL);
unsigned int ga5[4] = {((unsigned int)3ULL), ((unsigned int)21247ULL), ((unsigned int)127ULL), ((unsigned int)65535ULL)};
struct S_gs6 { unsigned char pad; long m0; unsigned char m1; };
struct S_gs6 gs6 = {0, ((long)10ULL), ((unsigned char)0ULL)};
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(g4);
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
unsigned char f7(void) {
    static int s11 = ((int)128ULL);
    long l8 = ((long)3ULL);
    volatile unsigned int l9 = ((unsigned int)1ULL);
    int l10 = ((int)0ULL);
    mix((l8--));
    mix((l10 & (g4 |= 255)));
    mix(((unsigned char)0ULL));
    return (gs6.m1 != ((unsigned char)0ULL));
}
signed char f12(unsigned char *pp13) {
    int l14 = ((int)32768ULL);
    volatile signed char l15 = ((signed char)0ULL);
    unsigned char w17;
    unsigned char w18;
    (gs6.m0 |= 90);
    for (unsigned char i16 = 0; i16 < 4; i16++) {
        w17 = 0;
        while (w17++ < 3) {
            switch ((((int)((((int)65535ULL) | l15))) & ((int)7ULL))) {
            case 5:
                (g1 += 1);
                break;
            default:
                mix((-g2));
            }
            (++g2);
            if (((gs6.m0 += 255) + ((long long)0ULL))) {
                break;
                mix((((int)255ULL) <= (*pp13)));
            }
        }
    }
    w18 = 0;
    do {
        l14 = f7();
    } while (++w18 < 1);
    return (g1 | ga5[((int)0ULL)]);
}
unsigned int f19(unsigned int p20, long p21) {
    static int s24 = ((int)256ULL);
    int l22 = ((int)0ULL);
    unsigned char l23 = ((unsigned char)10ULL);
    unsigned char w27;
    g3 = ((gs6.m0 *= 255) ^ p20);
    g1 = (l22 & g3);
    for (unsigned char i25 = 0; i25 < 4; i25++) {
        for (unsigned char i26 = 0; i26 < 3; i26++) {
            break;
        }
    }
    switch ((((int)((~s24))) & ((int)7ULL))) {
    case 6:
        break;
    default:
        w27 = 0;
        while (w27++ < 3) {
            gs6.m1 = ((l23 &= 90) + g1);
            continue;
            gs6.m0 = (w27 ^ ((unsigned int)2ULL));
            (g3 &= 129);
        }
    }
    return (!ga5[((int)0ULL)]);
}
int main(void) {
    unsigned int l28 = ((unsigned int)45274ULL);
    volatile unsigned long l29 = ((unsigned long)0ULL);
    signed char l30 = ((signed char)0ULL);
    unsigned char l31 = ((unsigned char)255ULL);
    mix(f19(l30, (g2 + ((signed char)255ULL))));
    mix((gs6.m1--));
    g3 = gs6.m0;
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
