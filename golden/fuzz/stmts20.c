// expect: 25989
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile int g1 = ((int)65535ULL);
unsigned char g2 = ((unsigned char)3ULL);
unsigned int g3 = ((unsigned int)60140ULL);
int z4 [[c6502::zeropage]] = ((int)0ULL);
unsigned int z5 [[c6502::zeropage]] = ((unsigned int)32768ULL);
volatile unsigned int z6 [[c6502::zeropage]] = ((unsigned int)128ULL);
unsigned _BitInt(6) ga7[4] = {((unsigned _BitInt(6))63ULL), ((unsigned _BitInt(6))33ULL), ((unsigned _BitInt(6))63ULL), ((unsigned _BitInt(6))2ULL)};
struct S_gs8 { unsigned char pad; int m0; unsigned char m1; };
struct S_gs8 gs8 = {0, ((int)11ULL), ((unsigned char)127ULL)};
unsigned int *gp9 = &g3;
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
unsigned char f10(unsigned long p11, signed char p12) {
    volatile unsigned int l13 = ((unsigned int)128ULL);
    long l14 = ((long)256ULL);
    unsigned char l15 = ((unsigned char)0ULL);
    unsigned char w16;
    g1 = (g3 ^= 1);
    w16 = 0;
    do {
        l14 = ((unsigned char)(w16));
    } while (++w16 < 3);
    gs8.m1 = (z6 << 6);
    l14 = (--p12);
    return ((int)255ULL);
}
unsigned _BitInt(64) f17(signed char *pp18) {
    static unsigned char s21 = ((unsigned char)3ULL);
    long l19 = ((long)13ULL);
    unsigned int l20 = ((unsigned int)32768ULL);
    mix((g1 & (g3 &= 1)));
    l20 = f10(l20, (((long)32767ULL) >> 1));
    mix(g3);
    return (ga7[((int)0ULL)] ? (*gp9) : ((unsigned long long)2147483647ULL));
}
unsigned _BitInt(64) f22(int *pp23) {
    static unsigned char s27 = ((unsigned char)255ULL);
    long l24 = ((long)65536ULL);
    long long l25 = ((long long)32767ULL);
    signed char l26 = ((signed char)255ULL);
    ((*pp23)--);
    mix(((z4 > s27) && (l26 + g3)));
    l25 = f17(&l26);
    f10((((unsigned long long)256ULL) * ((long long)0ULL)), z4);
    f17(&l26);
    return ((*pp23) < g3);
}
int main(void) {
    signed char l28 = ((signed char)127ULL);
    volatile unsigned int l29 = ((unsigned int)0ULL);
    unsigned char l30 = ((unsigned char)0ULL);
    int l31 = ((int)33605ULL);
    mix(((g1 *= 90) - l29));
    mix(f17(&l28));
    gs8.m1 = (--g1);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
