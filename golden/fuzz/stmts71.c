// expect: 18728
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
int g1 = ((int)28666ULL);
volatile long g2 = ((long)128ULL);
volatile long g3 = ((long)2ULL);
int z4 [[c6502::zeropage]] = ((int)0ULL);
int z5 [[c6502::zeropage]] = ((int)10ULL);
unsigned _BitInt(59) ga6[4] = {((unsigned _BitInt(59))256ULL), ((unsigned _BitInt(59))2ULL), ((unsigned _BitInt(59))1ULL), ((unsigned _BitInt(59))32768ULL)};
struct S_gs7 { unsigned char pad; unsigned int m0; signed char m1; };
struct S_gs7 gs7 = {0, ((unsigned int)32767ULL), ((signed char)2ULL)};
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
unsigned int f8(unsigned int p9, unsigned int p10) {
    static unsigned char s13 = ((unsigned char)255ULL);
    signed char l11 = ((signed char)1ULL);
    signed char l12 = ((signed char)0ULL);
    ga6[((int)3ULL)] = ((gs7.m1 << 1) == p9);
    mix((((unsigned _BitInt(35))30255055069ULL) > l12));
    (g1--);
    return (gs7.m0 < gs7.m1);
}
signed char f14(int p15, unsigned char p16, int p17) {
    static signed char s20 = ((signed char)255ULL);
    unsigned int l18 = ((unsigned int)3ULL);
    volatile int l19 = ((int)127ULL);
    (l18 <<= 2);
    for (unsigned char i21 = 0; i21 < 3; i21++) {
        switch ((((int)((!g2))) & ((int)7ULL))) {
        case 5:
            ;
        case 6:
            (g1 -= 3);
            break;
        }
        (ga6[((int)1ULL)] <<= 4);
    }
    ga6[((int)1ULL)] = (ga6[((int)3ULL)] + gs7.m0);
    f8(((unsigned char)(z5)), (((unsigned char)3ULL) < ((unsigned int)128ULL)));
    f8(((unsigned char)255ULL), ((unsigned char)(g2)));
    s20 = (s20 + (g3 += 255));
    return (((unsigned _BitInt(35))65535ULL) ? p16 : z5);
}
int main(void) {
    volatile unsigned char l22 = ((unsigned char)255ULL);
    unsigned int l23 = ((unsigned int)127ULL);
    int l24 = ((int)32767ULL);
    unsigned char w25;
    if ((g2 && l24)) {
        w25 = 0;
        do {
            z4 = (((unsigned int)65535ULL) ^ (++ga6[(w25 & ((int)3ULL))]));
            if (((g1 | ((unsigned char)1ULL)) || gs7.m0)) {
                (++ga6[(w25 & ((int)3ULL))]);
                mix(((gs7.m0--) ^ ((unsigned int)255ULL)));
                mix(((unsigned char)(w25)));
            }
        } while (++w25 < 2);
    }
    g3 = (l22 ^= 3);
    mix(g2);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
