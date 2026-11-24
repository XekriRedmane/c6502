// expect: 21355
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
int g1 = ((int)32767ULL);
long long g2 = ((long long)0ULL);
unsigned int g3 = ((unsigned int)32767ULL);
int z4 [[c6502::zeropage]] = ((int)65535ULL);
long ga5[4] = {((long)0ULL), ((long)2933972785ULL), ((long)256ULL), ((long)127ULL)};
struct S_gs6 { unsigned char pad; signed char m0; int m1; };
struct S_gs6 gs6 = {0, ((signed char)0ULL), ((int)255ULL)};
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
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
unsigned char f7(unsigned char *pp8) {
    static int s11 = ((int)3ULL);
    volatile int l9 = ((int)3ULL);
    unsigned long l10 = ((unsigned long)65535ULL);
    unsigned char w12;
    if (g2) {
        w12 = 0;
        while (w12++ < 1) {
            if ((ga5[((int)0ULL)] &= 3)) {
                break;
            } else {
                break;
                gs6.m0 = ((g2 -= 1) - w12);
            }
            g1 = ((gs6.m1 <<= 4) & (s11 + g2));
            return (s11 >>= 0);
            break;
        }
        switch ((((int)((s11 % ((int)6ULL)))) & ((int)7ULL))) {
        case 3:
            ga5[((int)2ULL)] = (g3 >>= 2);
            (l9++);
            break;
        case 5:
            ;
        default:
            ;
        }
    }
    return (gs6.m1 ^ g3);
}
int main(void) {
    unsigned char l13 = ((unsigned char)2ULL);
    unsigned int l14 = ((unsigned int)21290ULL);
    long long l15 = ((long long)65535ULL);
    unsigned char w16;
    l15 = f7(&l13);
    mix((gs6.m1 + gs6.m0));
    mix(f7(&l13));
    f7(&l13);
    w16 = 0;
    do {
        mix(f7(&l13));
        l14 = ((z4 % ((int)6ULL)) & (--g1));
    } while (++w16 < 3);
    f7(&l13);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
