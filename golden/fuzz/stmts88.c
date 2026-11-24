// expect: 14563
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
int g1 = ((int)127ULL);
_BitInt(46) g2 = ((_BitInt(46))43289964059397ULL);
unsigned int g3 = ((unsigned int)28744ULL);
unsigned char g4 = ((unsigned char)1ULL);
_BitInt(46) z5 [[c6502::zeropage]] = ((_BitInt(46))1ULL);
unsigned long z6 [[c6502::zeropage]] = ((unsigned long)2ULL);
unsigned char z7 [[c6502::zeropage]] = ((unsigned char)128ULL);
_BitInt(46) ga8[4] = {((_BitInt(46))1ULL), ((_BitInt(46))3ULL), ((_BitInt(46))2147483647ULL), ((_BitInt(46))28753168629528ULL)};
struct S_gs9 { unsigned char pad; long m0; unsigned int m1; };
struct S_gs9 gs9 = {0, ((long)256ULL), ((unsigned int)1ULL)};
_BitInt(46) *gp10 = &g2;
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
unsigned int f11(int p12, unsigned char p13) {
    static volatile unsigned char s15 = ((unsigned char)3ULL);
    long l14 = ((long)65536ULL);
    unsigned char w16;
    mix(p12);
    gs9.m1 = z6;
    w16 = 0;
    do {
        (s15 += 3);
        if ((++p13)) {
            (g1++);
            (*gp10) = g2;
        }
        p13 = (gs9.m1 -= 255);
        (--g4);
    } while (++w16 < 1);
    mix((++gs9.m0));
    return (g2 - gs9.m0);
}
int main(void) {
    volatile int l17 = ((int)127ULL);
    unsigned int l18 = ((unsigned int)0ULL);
    _BitInt(46) l19 = ((_BitInt(46))65536ULL);
    (l18++);
    for (unsigned char i20 = 0; i20 < 1; i20++) {
        mix(((signed char)(z6)));
        continue;
        for (unsigned char i21 = 0; i21 < 1; i21++) {
            mix(z7);
            (z7++);
        }
    }
    mix(((gs9.m1 ? g3 : g2) & (g4 ^ gs9.m0)));
    z5 = z5;
    g3 = (g1 &= 5);
    fin0();
    fin1();
    fin2();
    fin3();
    return (int)(acc & 0x7FFF);
}
