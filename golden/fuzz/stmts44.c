// expect: 6457
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
long long g1 = ((long long)0ULL);
signed char g2 = ((signed char)255ULL);
unsigned char g3 = ((unsigned char)1ULL);
unsigned int g4 = ((unsigned int)3ULL);
volatile signed char z5 [[c6502::zeropage]] = ((signed char)255ULL);
long ga6[4] = {((long)65535ULL), ((long)7ULL), ((long)1490344570ULL), ((long)128ULL)};
struct S_gs7 { unsigned char pad; unsigned long m0; unsigned int m1; };
struct S_gs7 gs7 = {0, ((unsigned long)127ULL), ((unsigned int)2ULL)};
unsigned char *gp8 = &g3;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(g4);
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
int f9(unsigned int p10, unsigned int p11) {
    static unsigned char s14 = ((unsigned char)3ULL);
    volatile unsigned char l12 = ((unsigned char)0ULL);
    unsigned char l13 = ((unsigned char)129ULL);
    unsigned char w15;
    if (gs7.m1) {
        mix(((g2 |= 3) & l13));
    } else {
        mix((((unsigned int)127ULL) ^ (g4 &= 255)));
    }
    (ga6[((int)3ULL)] <<= 4);
    p10 = (g4 * p10);
    w15 = 0;
    while (w15++ < 4) {
        z5 = (p10 & w15);
        p10 = (p11 += 90);
        break;
    }
    return (g1 - gs7.m0);
}
int main(void) {
    unsigned char l16 = ((unsigned char)0ULL);
    if (((gs7.m1 % ((int)1ULL)) ^ ((int)2ULL))) {
        z5 = (gs7.m1 -= 129);
    } else {
        (g4--);
    }
    mix(f9((((unsigned char)0ULL) * g3), ga6[((int)3ULL)]));
    g1 = (-((int)127ULL));
    l16 = (g1 & (g4 += 1));
    if ((ga6[((int)2ULL)] == g2)) {
        l16 = gs7.m1;
        f9(((*gp8) + ((unsigned int)5ULL)), (((unsigned long)0ULL) && g1));
    } else {
        (--g4);
    }
    if ((ga6[((int)0ULL)]--)) {
        mix(((g2 ^= 129) + ((unsigned char)255ULL)));
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
