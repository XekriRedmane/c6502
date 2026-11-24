// expect: 4866
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile unsigned long g1 = ((unsigned long)65535ULL);
unsigned int g2 = ((unsigned int)32767ULL);
unsigned int g3 = ((unsigned int)65535ULL);
unsigned int z4 [[c6502::zeropage]] = ((unsigned int)0ULL);
_BitInt(49) ga5[4] = {((_BitInt(49))2147483647ULL), ((_BitInt(49))2ULL), ((_BitInt(49))2147483648ULL), ((_BitInt(49))2147483647ULL)};
struct S_gs6 { unsigned char pad; _BitInt(49) m0; unsigned long m1; };
struct S_gs6 gs6 = {0, ((_BitInt(49))65536ULL), ((unsigned long)2147483647ULL)};
unsigned int *gp7 = &z4;
unsigned int *gp8 = &g3;
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
unsigned long long f9(int p10, int p11, unsigned char p12, signed char *pp13) {
    static unsigned _BitInt(18) s17 = ((unsigned _BitInt(18))32767ULL);
    unsigned int l14 = ((unsigned int)0ULL);
    long l15 = ((long)128ULL);
    unsigned int l16 = ((unsigned int)3ULL);
    mix((l16 & (p12 &= 90)));
    p10 = (ga5[((int)0ULL)] ? l15 : ((_BitInt(49))127ULL));
    (l15 |= 3);
    mix((((*gp8) <<= 5) ^ ((*gp7) > g2)));
    if (((((int)255ULL) - p11) & (l14 ^= 90))) {
        if ((g1 + ((*gp7) |= 5))) {
            if (((((unsigned char)2ULL) / ((int)4ULL)) / ((int)7ULL))) {
                return (((unsigned char)0ULL) ? gs6.m1 : g1);
                gs6.m1 = gs6.m1;
                p10 = (((*gp8) <<= 4) | (*pp13));
            } else {
                mix(((*gp8) | l14));
                (s17 >>= 2);
            }
        }
    }
    return (gs6.m1 - l14);
}
unsigned int f18(unsigned char p19, signed char p20, int *pp21) {
    static signed char s26 = ((signed char)255ULL);
    unsigned int l22 = ((unsigned int)0ULL);
    long l23 = ((long)256ULL);
    unsigned char l24 = ((unsigned char)0ULL);
    signed char l25 = ((signed char)3ULL);
    l25 = f9((((long long)256ULL) / ((int)1ULL)), (*pp21), ga5[((int)3ULL)], &l25);
    mix((gs6.m1 / ((int)1ULL)));
    mix(((gs6.m0 ? p20 : l22) - l25));
    return g1;
}
unsigned int f27(int *pp28) {
    static signed char s34 = ((signed char)5ULL);
    long l29 = ((long)32767ULL);
    long l30 = ((long)32768ULL);
    unsigned char l31 = ((unsigned char)3ULL);
    signed char l32 = ((signed char)128ULL);
    int l33 = ((int)32767ULL);
    unsigned char w35;
    unsigned char w36;
    (l32 |= 1);
    w35 = 0;
    do {
        (*pp28) = ((l33 |= 5) ^ z4);
        mix((gs6.m1 &= 129));
        (g3 ^= 255);
        w36 = 0;
        do {
            (l29 += 3);
            (g2 += 5);
            return ((g1--) + l32);
        } while (++w36 < 3);
    } while (++w35 < 3);
    mix(((l32 *= 90) ^ ((signed char)0ULL)));
    mix(((--g3) ^ l33));
    mix(f9((l30 - l33), (*pp28), (z4 - l29), &l32));
    return l32;
}
int main(void) {
    unsigned long l37 = ((unsigned long)4070340952ULL);
    signed char l38 = ((signed char)255ULL);
    int l39 = ((int)65535ULL);
    unsigned char w40;
    mix((*gp7));
    if ((*gp7)) {
        w40 = 0;
        do {
            g1 = ((w40 + g3) == (l38 == l38));
            (g3 ^= 3);
            gs6.m1 = (!l38);
        } while (++w40 < 4);
        (g1--);
        (l37 *= 5);
    }
    l38 = ((*gp7) -= 1);
    mix((~l37));
    ga5[((int)3ULL)] = ((unsigned int)(((unsigned int)6ULL)));
    gs6.m0 = (gs6.m1 *= 129);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
