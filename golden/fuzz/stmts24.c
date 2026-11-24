// expect: 10280
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile long g1 = ((long)9ULL);
volatile unsigned long g2 = ((unsigned long)32768ULL);
signed char z3 [[c6502::zeropage]] = ((signed char)0ULL);
signed char ga4[4] = {((signed char)255ULL), ((signed char)171ULL), ((signed char)127ULL), ((signed char)255ULL)};
struct S_gs5 { unsigned char pad; unsigned int m0; unsigned char m1; };
struct S_gs5 gs5 = {0, ((unsigned int)1ULL), ((unsigned char)255ULL)};
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
unsigned int f6(unsigned char p7, unsigned int p8) {
    static signed char s12 = ((signed char)255ULL);
    unsigned char l9 = ((unsigned char)255ULL);
    long l10 = ((long)127ULL);
    unsigned long l11 = ((unsigned long)3162596869ULL);
    mix(((gs5.m0 &= 90) + g1));
    mix(((ga4[((int)0ULL)] | l11) / ((int)6ULL)));
    mix(((z3 >>= 5) ^ g1));
    mix(((--s12) - gs5.m1));
    gs5.m1 = ((s12++) + gs5.m1);
    l11 = (p7 - l9);
    return (((unsigned long)2147483648ULL) << 0);
}
int main(void) {
    signed char l13 = ((signed char)8ULL);
    volatile signed char l14 = ((signed char)1ULL);
    unsigned char w17;
    l13 = f6((((unsigned int)2ULL) - l13), g1);
    mix(f6((((long)255ULL) / ((int)5ULL)), l13));
    for (unsigned char i15 = 0; i15 < 4; i15++) {
        gs5.m0 = (ga4[((int)1ULL)] ^= 5);
        mix((ga4[(i15 & ((int)3ULL))] += 5));
    }
    l14 = (gs5.m1 <<= 0);
    switch ((((int)((l13 / ((int)4ULL)))) & ((int)7ULL))) {
    case 2:
        ga4[((int)0ULL)] = ((gs5.m1 ? ((int)7ULL) : l14) % ((int)4ULL));
        break;
    case 3:
        switch ((((int)(g1)) & ((int)7ULL))) {
        case 0:
            break;
        case 5:
            for (unsigned char i16 = 0; i16 < 3; i16++) {
                break;
            }
            w17 = 0;
            do {
            } while (++w17 < 2);
            break;
        }
    case 5:
        break;
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
