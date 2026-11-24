// expect: 14116
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
signed char g1 = ((signed char)0ULL);
unsigned long long g2 = ((unsigned long long)16575779555702632366ULL);
int g3 = ((int)32768ULL);
volatile long z4 [[c6502::zeropage]] = ((long)4ULL);
int z5 [[c6502::zeropage]] = ((int)65535ULL);
unsigned long z6 [[c6502::zeropage]] = ((unsigned long)9ULL);
int ga7[4] = {((int)0ULL), ((int)0ULL), ((int)2ULL), ((int)65535ULL)};
struct S_gs8 { unsigned char pad; signed char m0; unsigned long m1; };
struct S_gs8 gs8 = {0, ((signed char)255ULL), ((unsigned long)2147483648ULL)};
signed char *gp9 = &g1;
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
unsigned char f10(void) {
    long l11 = ((long)32767ULL);
    (gs8.m1 >>= 7);
    switch ((((int)(((*gp9) >>= 7))) & ((int)7ULL))) {
    case 1:
        gs8.m0 = ((z5 *= 1) | g3);
        break;
    case 3:
        return (g1 + (z6 |= 3));
    }
    mix((z5 &= 255));
    return (z4 + l11);
}
unsigned int f12(long long p13) {
    static unsigned int s16 = ((unsigned int)127ULL);
    int l14 = ((int)1ULL);
    unsigned char l15 = ((unsigned char)128ULL);
    mix((((unsigned char)255ULL) / ((int)5ULL)));
    (z6 ^= 5);
    mix(((z6 - l15) + (z4 - g2)));
    ga7[((int)3ULL)] = (--(*gp9));
    z4 = (l15 |= 1);
    if (((unsigned int)0ULL)) {
        switch ((((int)((((int)32768ULL) ^ (gs8.m0--)))) & ((int)7ULL))) {
        case 1:
            break;
            break;
        case 7:
            break;
        }
    }
    return p13;
}
int main(void) {
    unsigned long l17 = ((unsigned long)128ULL);
    unsigned char w18;
    (ga7[((int)1ULL)] ^= 3);
    switch ((((int)((g2 - gs8.m0))) & ((int)7ULL))) {
    case 1:
        break;
    case 3:
        if ((ga7[((int)0ULL)] ^ (z6 |= 1))) {
            (ga7[((int)1ULL)] -= 129);
            gs8.m1 = (z4 + g1);
        }
        break;
    }
    w18 = 0;
    while (w18++ < 3) {
        mix(((unsigned long)3ULL));
        f10();
    }
    (gs8.m1 += 3);
    l17 = f10();
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
