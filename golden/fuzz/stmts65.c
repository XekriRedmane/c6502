// expect: 25335
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned long g1 = ((unsigned long)65535ULL);
volatile int g2 = ((int)128ULL);
int g3 = ((int)255ULL);
unsigned int z4 [[c6502::zeropage]] = ((unsigned int)3ULL);
unsigned char ga5[4] = {((unsigned char)1ULL), ((unsigned char)255ULL), ((unsigned char)3ULL), ((unsigned char)0ULL)};
struct S_gs6 { unsigned char pad; signed char m0; int m1; };
struct S_gs6 gs6 = {0, ((signed char)127ULL), ((int)1ULL)};
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
int f7(signed char p8, long p9, long long p10, long *pp11) {
    unsigned int l12 = ((unsigned int)23802ULL);
    unsigned char w13;
    (p8 += 90);
    mix((p8--));
    g3 = ((gs6.m1 ^= 255) ^ ((unsigned int)1ULL));
    (g2++);
    w13 = 0;
    do {
        for (unsigned char i14 = 0; i14 < 3; i14++) {
            continue;
        }
        switch ((((int)(g2)) & ((int)7ULL))) {
        case 2:
            g1 = (g1 ^ (++ga5[(w13 & ((int)3ULL))]));
            return (ga5[(w13 & ((int)3ULL))] &= 1);
            break;
        case 4:
            z4 = (g3 ^ l12);
            switch ((((int)((--gs6.m1))) & ((int)7ULL))) {
            case 2:
                ;
            case 5:
                break;
            default:
                ;
            }
        }
    } while (++w13 < 4);
    return (((signed char)0ULL) - ga5[((int)1ULL)]);
}
unsigned int f15(unsigned int *pp16) {
    static volatile unsigned char s21 = ((unsigned char)3ULL);
    volatile int l17 = ((int)127ULL);
    int l18 = ((int)0ULL);
    unsigned char l19 = ((unsigned char)127ULL);
    long l20 = ((long)255ULL);
    unsigned char w22;
    w22 = 0;
    while (w22++ < 2) {
        continue;
        (--l19);
    }
    l17 = g3;
    z4 = (g1 - gs6.m1);
    f7((*pp16), l17, ((long long)32768ULL), &l20);
    for (unsigned char i23 = 0; i23 < 1; i23++) {
        if ((g1 << 2)) {
            switch ((((int)((((unsigned char)232ULL) - ((signed char)0ULL)))) & ((int)7ULL))) {
            case 1:
                break;
                break;
            case 3:
                (--g3);
                break;
            case 7:
                break;
            default:
                break;
            }
        }
    }
    return gs6.m0;
}
int main(void) {
    unsigned int l24 = ((unsigned int)255ULL);
    long l25 = ((long)127ULL);
    l25 = f7((gs6.m0 % ((int)2ULL)), (g1 - g3), ((unsigned int)0ULL), &l25);
    l25 = ((gs6.m0 |= 5) & ((unsigned int)15ULL));
    gs6.m1 = ((g1 & z4) ? gs6.m0 : g3);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
