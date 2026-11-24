// expect: 27152
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile int g1 = ((int)65535ULL);
unsigned char g2 = ((unsigned char)0ULL);
signed char z3 [[c6502::zeropage]] = ((signed char)1ULL);
unsigned int ga4[4] = {((unsigned int)255ULL), ((unsigned int)256ULL), ((unsigned int)128ULL), ((unsigned int)65535ULL)};
struct S_gs5 { unsigned char pad; unsigned char m0; int m1; };
struct S_gs5 gs5 = {0, ((unsigned char)128ULL), ((int)255ULL)};
unsigned char *gp6 = &g2;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(z3);
    mix(ga4[((int)0ULL)]);
}
void fin1(void) {
    mix(ga4[((int)1ULL)]);
    mix(ga4[((int)2ULL)]);
    mix(ga4[((int)3ULL)]);
    mix(gs5.m0);
}
void fin2(void) {
    mix(gs5.m1);
}
unsigned char f7(signed char p8, unsigned char *pp9) {
    unsigned char l10 = ((unsigned char)2ULL);
    unsigned int l11 = ((unsigned int)0ULL);
    unsigned char l12 = ((unsigned char)0ULL);
    mix(((unsigned char)1ULL));
    (--l10);
    ((*pp9) += 1);
    return (p8 ? p8 : (*gp6));
}
int main(void) {
    unsigned long l13 = ((unsigned long)128ULL);
    unsigned char l14 = ((unsigned char)255ULL);
    unsigned char w15;
    z3 = (ga4[((int)1ULL)]--);
    w15 = 0;
    do {
        (z3 <<= 0);
    } while (++w15 < 3);
    (*gp6) = (ga4[((int)3ULL)] <= gs5.m0);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
