// expect: 7647
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned char g1 = ((unsigned char)255ULL);
int g2 = ((int)3ULL);
signed char g3 = ((signed char)0ULL);
volatile _BitInt(2) z4 [[c6502::zeropage]] = ((_BitInt(2))3ULL);
long z5 [[c6502::zeropage]] = ((long)32768ULL);
_BitInt(16) ga6[4] = {((_BitInt(16))256ULL), ((_BitInt(16))128ULL), ((_BitInt(16))256ULL), ((_BitInt(16))32767ULL)};
struct S_gs7 { unsigned char pad; signed char m0; _BitInt(16) m1; };
struct S_gs7 gs7 = {0, ((signed char)0ULL), ((_BitInt(16))255ULL)};
unsigned char *gp8 = &g1;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(z4);
}
void fin1(void) {
    mix(z5);
    mix(ga6[((int)0ULL)]);
    mix(ga6[((int)1ULL)]);
    mix(ga6[((int)2ULL)]);
}
void fin2(void) {
    mix(ga6[((int)3ULL)]);
    mix(gs7.m0);
    mix(gs7.m1);
}
_BitInt(16) f9(signed char *pp10) {
    volatile _BitInt(2) l11 = ((_BitInt(2))3ULL);
    int l12 = ((int)3ULL);
    long l13 = ((long)32768ULL);
    mix(((*gp8) += 3));
    g3 = ((unsigned char)2ULL);
    switch ((((int)(((l13 >>= 4) - (g3 ? g1 : z5)))) & ((int)7ULL))) {
    case 2:
        mix((((*gp8)--) ^ ((unsigned int)32767ULL)));
        (g2 |= 255);
    }
    (g3 ^= 90);
    return (g3 || l12);
}
int main(void) {
    volatile unsigned char l14 = ((unsigned char)0ULL);
    unsigned int l15 = ((unsigned int)32768ULL);
    signed char l16 = ((signed char)255ULL);
    if ((++gs7.m0)) {
        gs7.m1 = ((z4 - ga6[((int)1ULL)]) >= (gs7.m1 && g2));
    }
    l16 = f9(&l16);
    z4 = (((int)(l16)) << 3);
    g2 = (((signed char)255ULL) ^ ((*gp8) += 1));
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
