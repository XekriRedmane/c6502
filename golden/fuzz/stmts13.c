// expect: 16241
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
signed char g1 = ((signed char)0ULL);
long long g2 = ((long long)65535ULL);
unsigned int z3 [[c6502::zeropage]] = ((unsigned int)65535ULL);
unsigned _BitInt(12) ga4[4] = {((unsigned _BitInt(12))127ULL), ((unsigned _BitInt(12))4095ULL), ((unsigned _BitInt(12))0ULL), ((unsigned _BitInt(12))4095ULL)};
struct S_gs5 { unsigned char pad; unsigned int m0; unsigned int m1; };
struct S_gs5 gs5 = {0, ((unsigned int)10ULL), ((unsigned int)32768ULL)};
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
unsigned _BitInt(12) f6(unsigned _BitInt(48) p7, long p8, long long p9, long *pp10) {
    long long l11 = ((long long)32768ULL);
    if ((g1 *= 3)) {
        gs5.m1 = l11;
    } else {
        if (((unsigned char)0ULL)) {
            p7 = gs5.m1;
            if ((((unsigned char)0ULL) - (--l11))) {
                return (--p9);
            } else {
                return (gs5.m0 &= 255);
            }
            switch ((((int)(((z3 >>= 6) - ((signed char)3ULL)))) & ((int)7ULL))) {
            case 5:
                break;
                break;
            }
        }
    }
    mix(g1);
    g2 = ((p8 |= 255) ^ ((int)2ULL));
    if ((gs5.m1--)) {
    }
    return (p7 - ga4[((int)3ULL)]);
}
int main(void) {
    unsigned _BitInt(48) l12 = ((unsigned _BitInt(48))10ULL);
    unsigned long l13 = ((unsigned long)3ULL);
    int l14 = ((int)0ULL);
    long l15 = ((long)256ULL);
    switch ((((int)((g2 <= gs5.m0))) & ((int)7ULL))) {
    case 1:
        l14 = (g1 ? l15 : l14);
    }
    g2 = (((unsigned int)1ULL) < gs5.m1);
    gs5.m0 = ((signed char)((ga4[((int)0ULL)] < ((int)23444ULL))));
    for (unsigned char i16 = 0; i16 < 4; i16++) {
        l13 = ((ga4[((int)0ULL)] >>= 4) - gs5.m1);
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
