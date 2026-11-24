// expect: 24550
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile unsigned int g1 = ((unsigned int)2ULL);
int g2 = ((int)0ULL);
unsigned char g3 = ((unsigned char)255ULL);
unsigned char g4 = ((unsigned char)2ULL);
signed char ga5[4] = {((signed char)11ULL), ((signed char)3ULL), ((signed char)136ULL), ((signed char)128ULL)};
struct S_gs6 { unsigned char pad; long m0; signed char m1; };
struct S_gs6 gs6 = {0, ((long)32767ULL), ((signed char)0ULL)};
unsigned char *gp7 = &g4;
int *gp8 = &g2;
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
unsigned long f9(unsigned long p10, signed char p11) {
    unsigned int l12 = ((unsigned int)0ULL);
    volatile unsigned char l13 = ((unsigned char)2ULL);
    unsigned char w15;
    unsigned char w16;
    ((*gp7) >>= 4);
    (l12 <<= 1);
    for (unsigned char i14 = 0; i14 < 3; i14++) {
        if ((i14 & (g2 ^= 3))) {
            break;
        } else {
            mix((((unsigned int)0ULL) & (g2++)));
        }
        g1 = ga5[(i14 & ((int)3ULL))];
    }
    w15 = 0;
    while (w15++ < 1) {
        w16 = 0;
        while (w16++ < 1) {
            switch ((((int)((gs6.m0 ^= 1))) & ((int)7ULL))) {
            case 0:
                ;
            case 2:
                return (p11 & gs6.m0);
                return (l13 / ((int)4ULL));
                break;
            case 5:
                ;
            }
        }
    }
    return (g3 >> 6);
}
int f17(signed char p18, unsigned long long p19, unsigned char *pp20) {
    static volatile unsigned char s24 = ((unsigned char)0ULL);
    unsigned int l21 = ((unsigned int)32768ULL);
    signed char l22 = ((signed char)1ULL);
    unsigned long l23 = ((unsigned long)127ULL);
    unsigned char w25;
    f9((p19 && g3), ((unsigned int)65535ULL));
    w25 = 0;
    do {
        for (unsigned char i26 = 0; i26 < 4; i26++) {
            l21 = f9((((unsigned int)7931ULL) ? ((unsigned char)255ULL) : ((unsigned int)127ULL)), (((unsigned char)0ULL) - i26));
            gs6.m0 = (g3 - (~ga5[(w25 & ((int)3ULL))]));
            break;
        }
        (s24++);
    } while (++w25 < 3);
    ga5[((int)1ULL)] = (l22 > ((unsigned char)0ULL));
    p19 = (s24 += 129);
    g1 = ((*pp20) + gs6.m1);
    ((*gp8) >>= 6);
    return (l21 % ((int)1ULL));
}
unsigned int f27(unsigned int p28, signed char *pp29) {
    static long s32 = ((long)0ULL);
    unsigned char l30 = ((unsigned char)0ULL);
    int l31 = ((int)0ULL);
    switch ((((int)((-(*pp29)))) & ((int)7ULL))) {
    case 4:
        break;
    }
    l31 = f17(g1, ((signed char)(p28)), &l30);
    f17(gs6.m1, (g3 | g2), &l30);
    g3 = ((unsigned int)1ULL);
    (--(*pp29));
    f17(((unsigned long long)65535ULL), (!gs6.m0), &l30);
    return (*gp7);
}
int main(void) {
    long l33 = ((long)2147483648ULL);
    unsigned char l34 = ((unsigned char)212ULL);
    signed char l35 = ((signed char)255ULL);
    (g2 ^= 5);
    mix(((*gp8) | g3));
    g2 = ((l34--) | ((unsigned int)1ULL));
    if (((((int)2ULL) || (*gp8)) | (gs6.m0 -= 5))) {
        ga5[((int)0ULL)] = (ga5[((int)1ULL)] ^ (gs6.m1 += 129));
        mix((~(((unsigned char)0ULL) % ((int)2ULL))));
    }
    mix(g1);
    if ((((unsigned long)(l35)) - (((unsigned long)2147483648ULL) * g4))) {
        mix(f17((l35 >= g2), ((*gp8) >> 6), &l34));
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
