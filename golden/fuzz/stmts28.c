// expect: 26591
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned long g1 = ((unsigned long)3ULL);
unsigned char g2 = ((unsigned char)128ULL);
unsigned char g3 = ((unsigned char)0ULL);
int g4 = ((int)1ULL);
unsigned int ga5[4] = {((unsigned int)256ULL), ((unsigned int)49444ULL), ((unsigned int)3ULL), ((unsigned int)2ULL)};
struct S_gs6 { unsigned char pad; unsigned _BitInt(19) m0; long m1; };
struct S_gs6 gs6 = {0, ((unsigned _BitInt(19))32767ULL), ((long)127ULL)};
unsigned long *gp7 = &g1;
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
signed char f8(unsigned long long p9, int *pp10) {
    unsigned char l11 = ((unsigned char)6ULL);
    signed char l12 = ((signed char)255ULL);
    unsigned int l13 = ((unsigned int)255ULL);
    unsigned char w14;
    gs6.m1 = (((*pp10) % ((int)2ULL)) >> 6);
    mix(((*pp10) &= 3));
    if ((*pp10)) {
        switch ((((int)((((signed char)1ULL) | ((long)256ULL)))) & ((int)7ULL))) {
        case 1:
            gs6.m1 = ((g1 ^= 3) + p9);
            break;
        case 5:
            w14 = 0;
            while (w14++ < 2) {
                break;
                mix((g2++));
                mix((((*pp10) += 3) ^ w14));
                l11 = (ga5[((int)0ULL)] *= 3);
            }
        default:
            (++(*gp7));
        }
        return (ga5[((int)2ULL)] += 1);
    }
    return p9;
}
int f15(unsigned char p16, unsigned _BitInt(15) p17, long *pp18) {
    int l19 = ((int)3ULL);
    unsigned _BitInt(15) l20 = ((unsigned _BitInt(15))256ULL);
    long l21 = ((long)2147483648ULL);
    gs6.m1 = (p16 <<= 4);
    gs6.m1 = (g4 ^ g3);
    l21 = f8((-((unsigned char)3ULL)), &l19);
    return g3;
}
unsigned long f22(unsigned char *pp23) {
    volatile unsigned char l24 = ((unsigned char)0ULL);
    int l25 = ((int)0ULL);
    long l26 = ((long)373061612ULL);
    unsigned char w28;
    gs6.m0 = (g1 + (gs6.m1--));
    f15(gs6.m1, gs6.m0, &l26);
    mix(((*gp7) < ((int)0ULL)));
    gs6.m1 = (((int)12ULL) & ((unsigned char)255ULL));
    l25 = f8(((*gp7) || l25), &l25);
    for (unsigned char i27 = 0; i27 < 3; i27++) {
        w28 = 0;
        while (w28++ < 2) {
            (l24 <<= 6);
            if ((((*gp7) + ((unsigned char)3ULL)) & (gs6.m1 *= 129))) {
                ga5[(i27 & ((int)3ULL))] = l24;
                gs6.m0 = (gs6.m1--);
            } else {
                g2 = ((g1 += 1) ^ ((long long)127ULL));
            }
        }
    }
    return (l26 & gs6.m1);
}
int main(void) {
    signed char l29 = ((signed char)0ULL);
    unsigned int l30 = ((unsigned int)14ULL);
    int l31 = ((int)32767ULL);
    long l32 = ((long)32767ULL);
    unsigned char l33 = ((unsigned char)1ULL);
    (--gs6.m0);
    (--g4);
    if ((gs6.m1 & ((signed char)0ULL))) {
        mix((l33 |= 1));
        f15((l33 + (*gp7)), (!g2), &l32);
        l33 = ((l31 -= 255) & (l29 << 4));
    }
    (g3 -= 90);
    (g2--);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
