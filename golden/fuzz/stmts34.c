// expect: 18092
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile signed char g1 = ((signed char)255ULL);
unsigned int g2 = ((unsigned int)4ULL);
unsigned char g3 = ((unsigned char)0ULL);
unsigned char z4 [[c6502::zeropage]] = ((unsigned char)255ULL);
unsigned int ga5[4] = {((unsigned int)32768ULL), ((unsigned int)0ULL), ((unsigned int)0ULL), ((unsigned int)0ULL)};
struct S_gs6 { unsigned char pad; unsigned int m0; int m1; };
struct S_gs6 gs6 = {0, ((unsigned int)0ULL), ((int)1ULL)};
unsigned int *gp7 = &g2;
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
unsigned int f8(_BitInt(12) *pp9) {
    static unsigned long s11 = ((unsigned long)127ULL);
    volatile unsigned int l10 = ((unsigned int)2ULL);
    unsigned char w12;
    mix((--ga5[((int)0ULL)]));
    if ((gs6.m0 &= 3)) {
        (gs6.m0 <<= 0);
    } else {
        (gs6.m1 *= 129);
        (g2 &= 255);
    }
    mix((gs6.m0 ^= 255));
    if ((g2 << 3)) {
        return ((l10++) - ((unsigned int)128ULL));
        w12 = 0;
        while (w12++ < 2) {
            if ((gs6.m0 += 5)) {
                return ((ga5[((int)3ULL)]--) - (*pp9));
                break;
            }
        }
    }
    return ((int)65535ULL);
}
int main(void) {
    long l13 = ((long)0ULL);
    _BitInt(12) l14 = ((_BitInt(12))128ULL);
    unsigned char w15;
    w15 = 0;
    do {
        (*gp7) = (g3 += 255);
    } while (++w15 < 2);
    (l13 <<= 1);
    l13 = gs6.m1;
    f8(&l14);
    ga5[((int)2ULL)] = (gs6.m1 << 5);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
