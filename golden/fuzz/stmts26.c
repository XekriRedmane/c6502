// expect: 19415
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile signed char g1 = ((signed char)255ULL);
unsigned int g2 = ((unsigned int)127ULL);
unsigned int z3 [[c6502::zeropage]] = ((unsigned int)65535ULL);
unsigned long ga4[4] = {((unsigned long)32768ULL), ((unsigned long)3ULL), ((unsigned long)255ULL), ((unsigned long)2147483648ULL)};
struct S_gs5 { unsigned char pad; int m0; unsigned char m1; };
struct S_gs5 gs5 = {0, ((int)0ULL), ((unsigned char)0ULL)};
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
unsigned char f6(signed char p7, unsigned int p8) {
    static unsigned char s12 = ((unsigned char)155ULL);
    int l9 = ((int)127ULL);
    int l10 = ((int)3ULL);
    long long l11 = ((long long)0ULL);
    ga4[((int)1ULL)] = ((g1 <= gs5.m1) ^ (gs5.m0 >>= 0));
    (l10 ^= 1);
    (l11 |= 129);
    mix((gs5.m1 >= ((unsigned int)2ULL)));
    p7 = ((l10 <<= 2) | (gs5.m1 ? s12 : g2));
    return (((int)256ULL) * ((unsigned int)3ULL));
}
int main(void) {
    int l13 = ((int)2ULL);
    unsigned char w14;
    gs5.m0 = ((--ga4[((int)0ULL)]) ^ g2);
    w14 = 0;
    do {
        mix((w14 > g2));
        (ga4[(w14 & ((int)3ULL))] &= 90);
        mix(f6((gs5.m1 & z3), ((unsigned char)(g1))));
    } while (++w14 < 1);
    switch ((((int)((l13 != ((unsigned char)0ULL)))) & ((int)7ULL))) {
    case 0:
        mix(f6((g1 / ((int)6ULL)), (z3 >> 5)));
        mix((z3 ? ((long)2528941371ULL) : l13));
    case 4:
        mix(ga4[((int)3ULL)]);
        mix(ga4[((int)2ULL)]);
    case 7:
        ;
    }
    if (((gs5.m1 += 5) - g1)) {
        for (unsigned char i15 = 0; i15 < 1; i15++) {
        }
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
