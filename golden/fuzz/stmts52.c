// expect: 30983
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile signed char g1 = ((signed char)0ULL);
volatile unsigned int g2 = ((unsigned int)1ULL);
signed char g3 = ((signed char)255ULL);
_BitInt(44) z4 [[c6502::zeropage]] = ((_BitInt(44))65535ULL);
unsigned char ga5[4] = {((unsigned char)0ULL), ((unsigned char)0ULL), ((unsigned char)255ULL), ((unsigned char)255ULL)};
struct S_gs6 { unsigned char pad; int m0; unsigned int m1; };
struct S_gs6 gs6 = {0, ((int)65535ULL), ((unsigned int)0ULL)};
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
unsigned int f7(_BitInt(33) p8) {
    static int s10 = ((int)65535ULL);
    long long l9 = ((long long)3ULL);
    (p8 |= 5);
    (ga5[((int)3ULL)] >>= 6);
    s10 = ((z4 *= 3) - (ga5[((int)0ULL)] & g2));
    return ((signed char)(p8));
}
unsigned int f11(int p12, _BitInt(44) p13) {
    static long s15 = ((long)256ULL);
    unsigned char l14 = ((unsigned char)1ULL);
    f7((((signed char)255ULL) - z4));
    mix((--ga5[((int)1ULL)]));
    (--p12);
    if (((l14 & ((long long)0ULL)) && (g2 & p13))) {
        for (unsigned char i16 = 0; i16 < 2; i16++) {
            z4 = (gs6.m1 += 255);
        }
        mix(((-z4) + (g3 >>= 1)));
    }
    for (unsigned char i17 = 0; i17 < 3; i17++) {
        z4 = (g1 += 3);
        (ga5[(i17 & ((int)3ULL))] ^= 3);
        mix((((signed char)2ULL) | i17));
    }
    l14 = f7((((unsigned long)2900107355ULL) - p13));
    return (!s15);
}
int main(void) {
    unsigned char l18 = ((unsigned char)0ULL);
    unsigned int l19 = ((unsigned int)32767ULL);
    unsigned char w21;
    l18 = ((l19 &= 3) | gs6.m1);
    l18 = f7(((unsigned int)0ULL));
    for (unsigned char i20 = 0; i20 < 3; i20++) {
        (ga5[(i20 & ((int)3ULL))] *= 1);
        w21 = 0;
        while (w21++ < 1) {
            if (((unsigned char)223ULL)) {
                break;
            } else {
                mix(l18);
            }
            l18 = ((gs6.m1 |= 3) & (((signed char)255ULL) * gs6.m0));
            z4 = (ga5[(w21 & ((int)3ULL))] &= 255);
            l19 = g1;
        }
        gs6.m0 = ((l18 ^= 1) ^ z4);
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
