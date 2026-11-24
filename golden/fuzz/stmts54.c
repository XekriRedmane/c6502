// expect: 25519
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned char g1 = ((unsigned char)255ULL);
volatile unsigned long g2 = ((unsigned long)128ULL);
int g3 = ((int)32767ULL);
unsigned long long g4 = ((unsigned long long)32768ULL);
volatile signed char z5 [[c6502::zeropage]] = ((signed char)53ULL);
unsigned int z6 [[c6502::zeropage]] = ((unsigned int)32767ULL);
unsigned long long ga7[4] = {((unsigned long long)32768ULL), ((unsigned long long)2147483647ULL), ((unsigned long long)127ULL), ((unsigned long long)65535ULL)};
struct S_gs8 { unsigned char pad; unsigned long long m0; unsigned char m1; };
struct S_gs8 gs8 = {0, ((unsigned long long)0ULL), ((unsigned char)255ULL)};
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(g4);
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
signed char f9(unsigned int *pp10) {
    unsigned char l11 = ((unsigned char)10ULL);
    int l12 = ((int)0ULL);
    unsigned int l13 = ((unsigned int)0ULL);
    (z5 >>= 5);
    (gs8.m0 ^= 255);
    l12 = ((ga7[((int)2ULL)] *= 255) ^ ((signed char)255ULL));
    (l11 >>= 0);
    return (z5 / ((int)4ULL));
}
signed char f14(unsigned int p15, long p16, signed char *pp17) {
    long l18 = ((long)256ULL);
    unsigned int l19 = ((unsigned int)0ULL);
    unsigned char w20;
    gs8.m1 = (((int)(l18)) ^ (z5 ^= 1));
    w20 = 0;
    while (w20++ < 4) {
        g3 = (l18 & (p16 &= 255));
        if ((ga7[((int)2ULL)] &= 90)) {
            (*pp17) = (p15 ? w20 : ((unsigned char)0ULL));
            continue;
        } else {
            continue;
        }
        return ((--p15) | ga7[(w20 & ((int)3ULL))]);
    }
    if ((g1 + ((int)127ULL))) {
        mix(((*pp17) - p15));
        (gs8.m1 &= 1);
    }
    (g4 &= 1);
    return ((int)127ULL);
}
int main(void) {
    volatile long long l21 = ((long long)3ULL);
    unsigned long l22 = ((unsigned long)128ULL);
    unsigned char l23 = ((unsigned char)5ULL);
    unsigned int l24 = ((unsigned int)256ULL);
    signed char l25 = ((signed char)255ULL);
    (g3--);
    (g2 |= 90);
    l23 = (--gs8.m0);
    if ((l25 + z5)) {
        (ga7[((int)2ULL)] -= 5);
        if ((ga7[((int)1ULL)] << 3)) {
            gs8.m0 = (g2 - g4);
            mix((g2 ^= 90));
            switch ((((int)(z5)) & ((int)7ULL))) {
            case 1:
                break;
            }
        }
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
