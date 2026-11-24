// expect: 14084
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned int g1 = ((unsigned int)2ULL);
unsigned long g2 = ((unsigned long)2147483648ULL);
long g3 = ((long)1ULL);
volatile signed char z4 [[c6502::zeropage]] = ((signed char)0ULL);
volatile unsigned int z5 [[c6502::zeropage]] = ((unsigned int)255ULL);
long z6 [[c6502::zeropage]] = ((long)65536ULL);
unsigned int ga7[4] = {((unsigned int)65535ULL), ((unsigned int)2ULL), ((unsigned int)1ULL), ((unsigned int)56941ULL)};
struct S_gs8 { unsigned char pad; unsigned long long m0; long m1; };
struct S_gs8 gs8 = {0, ((unsigned long long)32768ULL), ((long)2ULL)};
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(z4);
}
void fin1(void) {
    mix(z5);
    mix(z6);
    mix(ga7[((int)0ULL)]);
    mix(ga7[((int)1ULL)]);
}
void fin2(void) {
    mix(ga7[((int)2ULL)]);
    mix(ga7[((int)3ULL)]);
    mix(gs8.m0);
    mix(gs8.m1);
}
unsigned int f9(void) {
    static volatile unsigned int s12 = ((unsigned int)2ULL);
    unsigned char l10 = ((unsigned char)5ULL);
    volatile signed char l11 = ((signed char)255ULL);
    mix((l10 - l10));
    mix((((long)255ULL) & s12));
    gs8.m1 = ((unsigned long)((gs8.m0 + gs8.m1)));
    return ((unsigned char)255ULL);
}
int main(void) {
    unsigned int l13 = ((unsigned int)127ULL);
    z4 = ((g2 |= 3) - g3);
    mix((gs8.m1 += 3));
    z6 = g2;
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
