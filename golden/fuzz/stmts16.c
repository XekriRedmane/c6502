// expect: 26352
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile signed char g1 = ((signed char)1ULL);
unsigned int g2 = ((unsigned int)0ULL);
unsigned char z3 [[c6502::zeropage]] = ((unsigned char)1ULL);
int z4 [[c6502::zeropage]] = ((int)65535ULL);
unsigned char ga5[4] = {((unsigned char)2ULL), ((unsigned char)255ULL), ((unsigned char)0ULL), ((unsigned char)255ULL)};
struct S_gs6 { unsigned char pad; unsigned char m0; signed char m1; };
struct S_gs6 gs6 = {0, ((unsigned char)0ULL), ((signed char)0ULL)};
unsigned int *gp7 = &g2;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(z3);
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
int f8(unsigned int p9, unsigned char p10) {
    unsigned long l11 = ((unsigned long)65536ULL);
    unsigned char w13;
    z4 = ((z3++) | (ga5[((int)2ULL)] + ((unsigned char)255ULL)));
    if ((z4 * ((unsigned char)128ULL))) {
        for (unsigned char i12 = 0; i12 < 2; i12++) {
            (gs6.m0 |= 129);
        }
    }
    mix((g2 * ((signed char)127ULL)));
    w13 = 0;
    while (w13++ < 4) {
        z3 = ((gs6.m0 <<= 5) & ((long long)2147483647ULL));
        mix((w13 + w13));
        ((*gp7)--);
        continue;
    }
    (l11 |= 129);
    return (~p9);
}
int main(void) {
    unsigned char l14 = ((unsigned char)255ULL);
    if ((!(gs6.m1 ? gs6.m0 : l14))) {
        (g1 &= 255);
    }
    switch ((((int)((((*gp7)--) | gs6.m1))) & ((int)7ULL))) {
    case 3:
        break;
        break;
    }
    if (((*gp7) - (--z3))) {
        mix((((*gp7)++) + (!z3)));
    } else {
        (z3 |= 3);
    }
    f8(l14, (ga5[((int)0ULL)] ^ ga5[((int)3ULL)]));
    f8((l14 <= ga5[((int)0ULL)]), (!gs6.m0));
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
