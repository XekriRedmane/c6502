// expect: 16870
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
signed char g1 = ((signed char)255ULL);
volatile long g2 = ((long)127ULL);
unsigned long g3 = ((unsigned long)127ULL);
int z4 [[c6502::zeropage]] = ((int)0ULL);
volatile unsigned char z5 [[c6502::zeropage]] = ((unsigned char)0ULL);
unsigned int ga6[4] = {((unsigned int)38883ULL), ((unsigned int)2ULL), ((unsigned int)256ULL), ((unsigned int)15ULL)};
struct S_gs7 { unsigned char pad; long m0; signed char m1; };
struct S_gs7 gs7 = {0, ((long)256ULL), ((signed char)0ULL)};
int *gp8 = &z4;
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
unsigned int f9(unsigned int p10, unsigned char p11, _BitInt(17) *pp12) {
    volatile unsigned char l13 = ((unsigned char)0ULL);
    unsigned char w14;
    if (((*pp12)--)) {
        w14 = 0;
        do {
            switch ((((int)(((*gp8)--))) & ((int)7ULL))) {
            case 0:
                break;
            case 2:
                mix((p11 <<= 3));
            case 7:
                mix(((*pp12) ^= 255));
                break;
            }
            if ((gs7.m0 |= 90)) {
                break;
                return ((unsigned int)255ULL);
            }
        } while (++w14 < 2);
    }
    g1 = (l13--);
    mix(((unsigned int)0ULL));
    return ((signed char)(l13));
}
int main(void) {
    signed char l15 = ((signed char)2ULL);
    signed char l16 = ((signed char)60ULL);
    unsigned char l17 = ((unsigned char)3ULL);
    _BitInt(17) l18 = ((_BitInt(17))0ULL);
    (g3 |= 255);
    z4 = (--l15);
    ga6[((int)2ULL)] = (z4 |= 1);
    g2 = (((unsigned char)(l18)) ^ (l15 << 5));
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
