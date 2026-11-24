// expect: 32193
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
signed char g1 = ((signed char)1ULL);
unsigned char g2 = ((unsigned char)1ULL);
long g3 = ((long)128ULL);
int z4 [[c6502::zeropage]] = ((int)2ULL);
signed char z5 [[c6502::zeropage]] = ((signed char)0ULL);
int z6 [[c6502::zeropage]] = ((int)32768ULL);
unsigned long ga7[4] = {((unsigned long)65536ULL), ((unsigned long)128ULL), ((unsigned long)7ULL), ((unsigned long)2341214899ULL)};
struct S_gs8 { unsigned char pad; unsigned int m0; signed char m1; };
struct S_gs8 gs8 = {0, ((unsigned int)1ULL), ((signed char)255ULL)};
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
unsigned int f9(unsigned char p10, int *pp11) {
    static volatile int s13 = ((int)45119ULL);
    int l12 = ((int)3ULL);
    mix(((l12 + ga7[((int)2ULL)]) / ((int)4ULL)));
    mix((g3 >> 3));
    mix((l12 & (++g3)));
    mix((((*pp11) |= 90) | gs8.m1));
    return (p10 >> 3);
}
unsigned char f14(unsigned long long p15, unsigned char p16, long long p17) {
    static int s21 = ((int)5ULL);
    volatile signed char l18 = ((signed char)0ULL);
    unsigned long long l19 = ((unsigned long long)57002257972832920ULL);
    int l20 = ((int)128ULL);
    (p17 ^= 1);
    gs8.m1 = (l18 | (g1 ^= 255));
    ga7[((int)3ULL)] = (s21 &= 3);
    (p15 &= 255);
    return (((unsigned int)0ULL) - l19);
}
int main(void) {
    unsigned long long l22 = ((unsigned long long)65535ULL);
    int l23 = ((int)2ULL);
    unsigned char w24;
    unsigned char w25;
    g2 = (l22 % ((int)3ULL));
    l22 = f14((((unsigned long)256ULL) % ((int)6ULL)), (ga7[((int)1ULL)] + ((_BitInt(62))3368382830356621356ULL)), (z5 % ((int)5ULL)));
    w24 = 0;
    while (w24++ < 3) {
        f14((g1 + g2), w24, ((int)(gs8.m1)));
        switch ((((int)(w24)) & ((int)7ULL))) {
        case 0:
            switch ((((int)(((z5 <<= 0) ^ w24))) & ((int)7ULL))) {
            case 6:
                ;
            }
            if (((int)65535ULL)) {
                g2 = (gs8.m1 % ((int)7ULL));
            }
            break;
        case 1:
            (--l22);
        case 6:
            continue;
            (gs8.m1 -= 5);
        default:
            w25 = 0;
            while (w25++ < 1) {
            }
        }
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
