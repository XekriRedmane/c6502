// expect: 12127
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned char g1 = ((unsigned char)1ULL);
int g2 = ((int)13ULL);
unsigned int z3 [[c6502::zeropage]] = ((unsigned int)32768ULL);
volatile int z4 [[c6502::zeropage]] = ((int)37167ULL);
unsigned int ga5[4] = {((unsigned int)65535ULL), ((unsigned int)256ULL), ((unsigned int)128ULL), ((unsigned int)65535ULL)};
struct S_gs6 { unsigned char pad; unsigned char m0; signed char m1; };
struct S_gs6 gs6 = {0, ((unsigned char)2ULL), ((signed char)255ULL)};
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
int f7(unsigned char p8, unsigned int p9, long long *pp10) {
    static unsigned int s14 = ((unsigned int)2ULL);
    volatile signed char l11 = ((signed char)255ULL);
    unsigned char l12 = ((unsigned char)255ULL);
    unsigned char l13 = ((unsigned char)145ULL);
    (p8 |= 129);
    mix(((*pp10)++));
    (++gs6.m1);
    if ((g2 + (z4 -= 3))) {
        mix(gs6.m0);
        s14 = (gs6.m1 += 1);
    } else {
        mix((((unsigned long)32767ULL) + (l11 | (*pp10))));
        (gs6.m0 -= 90);
    }
    mix((-((long long)256ULL)));
    return ((int)(((signed char)255ULL)));
}
int main(void) {
    unsigned char l15 = ((unsigned char)0ULL);
    long l16 = ((long)2ULL);
    long long l17 = ((long long)32767ULL);
    unsigned char w18;
    switch ((((int)(((l15 <<= 7) | gs6.m0))) & ((int)7ULL))) {
    case 1:
        break;
    case 3:
        w18 = 0;
        while (w18++ < 4) {
            l15 = (--gs6.m0);
            mix(((ga5[(w18 & ((int)3ULL))] &= 3) + ((signed char)128ULL)));
            (g2 -= 3);
            for (unsigned char i19 = 0; i19 < 1; i19++) {
                (ga5[((int)1ULL)] |= 90);
            }
        }
        mix((-l16));
    case 5:
        if ((g2 *= 90)) {
            switch ((((int)(((gs6.m1 & l17) == (((unsigned char)255ULL) ? ga5[((int)2ULL)] : l16)))) & ((int)7ULL))) {
            case 3:
                break;
            case 4:
                z4 = ((l17 - ga5[((int)3ULL)]) & ((long)2ULL));
                break;
            case 5:
                mix(z3);
                break;
            case 7:
                break;
            default:
                ;
            }
        }
    case 7:
        break;
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
