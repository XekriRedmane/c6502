// expect: 31498
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
signed char g1 = ((signed char)255ULL);
unsigned char g2 = ((unsigned char)128ULL);
unsigned char g3 = ((unsigned char)0ULL);
unsigned int g4 = ((unsigned int)127ULL);
long ga5[4] = {((long)2147483647ULL), ((long)65535ULL), ((long)2147483648ULL), ((long)255ULL)};
struct S_gs6 { unsigned char pad; long m0; unsigned char m1; };
struct S_gs6 gs6 = {0, ((long)128ULL), ((unsigned char)0ULL)};
unsigned char *gp7 = &g3;
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
int f8(unsigned int *pp9) {
    long l10 = ((long)0ULL);
    long l11 = ((long)65536ULL);
    mix(((g2 -= 3) ^ (ga5[((int)2ULL)] > ((int)128ULL))));
    g4 = (g3 | (ga5[((int)1ULL)] *= 1));
    (--gs6.m0);
    mix((gs6.m1 == gs6.m1));
    return (((unsigned int)1ULL) + ((int)127ULL));
}
int f12(signed char p13, signed char p14, long long p15) {
    static signed char s19 = ((signed char)127ULL);
    long l16 = ((long)2147483648ULL);
    unsigned int l17 = ((unsigned int)3ULL);
    unsigned int l18 = ((unsigned int)32767ULL);
    (l17 ^= 255);
    g4 = ((l16 &= 90) ^ l18);
    (ga5[((int)1ULL)] ^= 1);
    return (g1 - g4);
}
long f20(unsigned int *pp21) {
    volatile long l22 = ((long)7ULL);
    unsigned int l23 = ((unsigned int)0ULL);
    g4 = g2;
    g3 = ((unsigned int)(gs6.m0));
    (g2 |= 5);
    return (~((unsigned char)127ULL));
}
int main(void) {
    unsigned char l24 = ((unsigned char)255ULL);
    unsigned int l25 = ((unsigned int)65535ULL);
    unsigned int l26 = ((unsigned int)0ULL);
    unsigned char w27;
    g3 = (gs6.m0++);
    w27 = 0;
    while (w27++ < 2) {
        mix(f12((l26 >> 7), ga5[(w27 & ((int)3ULL))], (g1 % ((int)7ULL))));
    }
    (g4 -= 1);
    ((*gp7) |= 255);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
