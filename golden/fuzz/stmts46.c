// expect: 21007
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned long g1 = ((unsigned long)12ULL);
unsigned char g2 = ((unsigned char)128ULL);
volatile int g3 = ((int)256ULL);
int z4 [[c6502::zeropage]] = ((int)256ULL);
unsigned char ga5[4] = {((unsigned char)0ULL), ((unsigned char)255ULL), ((unsigned char)128ULL), ((unsigned char)0ULL)};
struct S_gs6 { unsigned char pad; unsigned char m0; signed char m1; };
struct S_gs6 gs6 = {0, ((unsigned char)127ULL), ((signed char)1ULL)};
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
unsigned int f7(long p8, unsigned int p9, long p10, unsigned char *pp11) {
    static unsigned char s15 = ((unsigned char)176ULL);
    long l12 = ((long)32768ULL);
    signed char l13 = ((signed char)1ULL);
    signed char l14 = ((signed char)1ULL);
    unsigned char w17;
    unsigned char w18;
    (z4++);
    ((*pp11) |= 3);
    for (unsigned char i16 = 0; i16 < 4; i16++) {
        w17 = 0;
        do {
            s15 = (gs6.m1++);
            mix((z4 ? (ga5[((int)1ULL)] | ((unsigned int)0ULL)) : (((unsigned long long)0ULL) & gs6.m0)));
            break;
        } while (++w17 < 1);
        w18 = 0;
        do {
            gs6.m0 = (l13 >>= 0);
        } while (++w18 < 3);
    }
    l13 = ((*pp11)--);
    l14 = g2;
    return ((signed char)0ULL);
}
int main(void) {
    signed char l19 = ((signed char)1ULL);
    unsigned char l20 = ((unsigned char)1ULL);
    unsigned char w21;
    unsigned char w22;
    z4 = g3;
    if ((g1 + (l20 -= 90))) {
        w21 = 0;
        do {
            (l20 >>= 1);
        } while (++w21 < 4);
        if (((signed char)((l19 << 1)))) {
            w22 = 0;
            do {
                break;
                f7((g3 % ((int)5ULL)), (l20 % ((int)5ULL)), (w22 % ((int)2ULL)), &l20);
                g2 = (g1 |= 255);
            } while (++w22 < 1);
            g2 = (z4 | (l19--));
        } else {
            for (unsigned char i23 = 0; i23 < 3; i23++) {
                ga5[(i23 & ((int)3ULL))] = (gs6.m0 <= ((unsigned char)0ULL));
            }
        }
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
