// expect: 3983
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
signed char g1 = ((signed char)3ULL);
unsigned char g2 = ((unsigned char)127ULL);
unsigned char ga3[4] = {((unsigned char)127ULL), ((unsigned char)0ULL), ((unsigned char)2ULL), ((unsigned char)128ULL)};
struct S_gs4 { unsigned char pad; signed char m0; unsigned char m1; };
struct S_gs4 gs4 = {0, ((signed char)255ULL), ((unsigned char)255ULL)};
unsigned char *gp5 = &g2;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(ga3[((int)0ULL)]);
    mix(ga3[((int)1ULL)]);
}
void fin1(void) {
    mix(ga3[((int)2ULL)]);
    mix(ga3[((int)3ULL)]);
    mix(gs4.m0);
    mix(gs4.m1);
}
unsigned int f6(unsigned long long p7, unsigned int *pp8) {
    static int s12 = ((int)127ULL);
    volatile unsigned int l9 = ((unsigned int)65535ULL);
    signed char l10 = ((signed char)1ULL);
    volatile unsigned int l11 = ((unsigned int)0ULL);
    unsigned char w13;
    w13 = 0;
    do {
        ga3[((int)3ULL)] = (*gp5);
        switch ((((int)(((*gp5) | l10))) & ((int)7ULL))) {
        case 1:
            for (unsigned char i14 = 0; i14 < 1; i14++) {
                g2 = (++p7);
                mix(((gs4.m1++) | (*pp8)));
            }
            mix((((*pp8)++) + w13));
            break;
        case 5:
            switch ((((int)(((l11 <<= 1) - l10))) & ((int)7ULL))) {
            case 0:
                break;
            case 2:
                break;
            case 4:
                continue;
                break;
            case 6:
                break;
            }
            mix(((-((unsigned int)39524ULL)) == (((int)127ULL) < gs4.m0)));
        }
        if ((l9 / ((int)2ULL))) {
            l11 = ((++l9) ^ s12);
        }
    } while (++w13 < 1);
    return gs4.m0;
}
unsigned _BitInt(21) f15(unsigned char p16, signed char p17, int p18, unsigned long long *pp19) {
    static signed char s23 = ((signed char)3ULL);
    volatile long long l20 = ((long long)65536ULL);
    unsigned char l21 = ((unsigned char)7ULL);
    unsigned int l22 = ((unsigned int)128ULL);
    mix((ga3[((int)2ULL)] | (s23 |= 1)));
    f6(((signed char)(gs4.m0)), &l22);
    f6((((long long)2ULL) & ((long)32768ULL)), &l22);
    mix(f6((((unsigned char)0ULL) * gs4.m0), &l22));
    (l21--);
    return (l22 >> 6);
}
unsigned _BitInt(19) f24(unsigned char p25, int p26) {
    volatile int l27 = ((int)0ULL);
    unsigned int l28 = ((unsigned int)32767ULL);
    unsigned long long l29 = ((unsigned long long)0ULL);
    unsigned char w30;
    g2 = (gs4.m1 |= 255);
    l27 = f6((gs4.m1 ^ ((unsigned int)0ULL)), &l28);
    f15(((unsigned _BitInt(21))151108ULL), (!g2), (ga3[((int)0ULL)] & l27), &l29);
    (gs4.m0 &= 255);
    w30 = 0;
    do {
        if ((w30 / ((int)5ULL))) {
            if ((ga3[((int)2ULL)] |= 3)) {
                (g2 |= 3);
            }
            mix(l29);
            continue;
        } else {
            switch ((((int)((l28 &= 255))) & ((int)7ULL))) {
            case 3:
                (++gs4.m1);
            case 7:
                ;
            }
        }
    } while (++w30 < 2);
    return (((unsigned int)32768ULL) % ((int)2ULL));
}
int main(void) {
    int l31 = ((int)2ULL);
    unsigned int l32 = ((unsigned int)255ULL);
    unsigned long long l33 = ((unsigned long long)32768ULL);
    g2 = (ga3[((int)1ULL)] & (l31 ^= 1));
    l33 = ((*gp5) ^ g1);
    mix(l31);
    fin0();
    fin1();
    return (int)(acc & 0x7FFF);
}
