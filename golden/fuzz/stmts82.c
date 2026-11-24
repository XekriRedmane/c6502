// expect: 16177
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned int g1 = ((unsigned int)65535ULL);
volatile unsigned int g2 = ((unsigned int)128ULL);
unsigned int g3 = ((unsigned int)32768ULL);
unsigned long g4 = ((unsigned long)1ULL);
unsigned int ga5[4] = {((unsigned int)0ULL), ((unsigned int)65535ULL), ((unsigned int)32767ULL), ((unsigned int)2ULL)};
struct S_gs6 { unsigned char pad; signed char m0; unsigned long m1; };
struct S_gs6 gs6 = {0, ((signed char)0ULL), ((unsigned long)65535ULL)};
unsigned long *gp7 = &g4;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(g4);
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
int f8(unsigned char p9, long p10) {
    static unsigned _BitInt(26) s14 = ((unsigned _BitInt(26))6537564ULL);
    unsigned char l11 = ((unsigned char)0ULL);
    volatile unsigned int l12 = ((unsigned int)0ULL);
    unsigned char l13 = ((unsigned char)128ULL);
    mix(((p10 <<= 5) - l11));
    l13 = (gs6.m0 >>= 7);
    (g4 += 255);
    mix(((gs6.m1 *= 129) & ((unsigned _BitInt(26))(((signed char)255ULL)))));
    return (g3 | gs6.m0);
}
int main(void) {
    signed char l15 = ((signed char)255ULL);
    volatile unsigned int l16 = ((unsigned int)1ULL);
    (g2 &= 3);
    l15 = f8((g2 / ((int)1ULL)), (g1 - g4));
    ((*gp7) ^= 3);
    g3 = (--l15);
    (l16 |= 90);
    mix(gs6.m0);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
