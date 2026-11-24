// expect: 25075
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned char g1 = ((unsigned char)0ULL);
int g2 = ((int)65535ULL);
unsigned char ga3[4] = {((unsigned char)255ULL), ((unsigned char)0ULL), ((unsigned char)127ULL), ((unsigned char)128ULL)};
struct S_gs4 { unsigned char pad; long long m0; long m1; };
struct S_gs4 gs4 = {0, ((long long)127ULL), ((long)2147483648ULL)};
int *gp5 = &g2;
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
unsigned int f6(unsigned long p7, signed char p8, unsigned long p9, unsigned char *pp10) {
    static int s12 = ((int)3ULL);
    volatile unsigned char l11 = ((unsigned char)0ULL);
    mix(((unsigned char)((*pp10))));
    (--l11);
    switch ((((int)((l11--))) & ((int)7ULL))) {
    case 0:
        ;
    case 3:
        gs4.m0 = ((int)128ULL);
        break;
    default:
        for (unsigned char i13 = 0; i13 < 3; i13++) {
            (gs4.m1 <<= 2);
        }
    }
    gs4.m1 = (l11 *= 5);
    return gs4.m0;
}
unsigned char f14(void) {
    static signed char s18 = ((signed char)1ULL);
    unsigned char l15 = ((unsigned char)255ULL);
    int l16 = ((int)65535ULL);
    int l17 = ((int)1ULL);
    unsigned char w19;
    f6((ga3[((int)2ULL)] << 3), ((int)2ULL), ((long)128ULL), &l15);
    w19 = 0;
    while (w19++ < 1) {
        if ((l15 ^= 1)) {
            mix(((gs4.m1++) - (((unsigned long)1ULL) & w19)));
            g2 = ((*gp5) * ((unsigned char)0ULL));
        } else {
            for (unsigned char i20 = 0; i20 < 3; i20++) {
                break;
                l15 = f6((l15 - gs4.m0), (gs4.m0 + ga3[((int)1ULL)]), ((unsigned char)147ULL), &l15);
            }
            mix(l17);
        }
        mix((l15 * ga3[(w19 & ((int)3ULL))]));
        ga3[(w19 & ((int)3ULL))] = (g1 <<= 0);
        l16 = ((l15 |= 129) ^ (((signed char)2ULL) | g2));
    }
    return g1;
}
int f21(long p22) {
    static int s26 = ((int)13ULL);
    int l23 = ((int)128ULL);
    unsigned char l24 = ((unsigned char)1ULL);
    long l25 = ((long)32768ULL);
    unsigned char w27;
    w27 = 0;
    do {
        if ((g1 | (gs4.m1 += 3))) {
            l24 = f6((((signed char)7ULL) && w27), p22, ((unsigned char)0ULL), &l24);
        }
        g2 = (gs4.m1 ^ (--s26));
        switch ((((int)(p22)) & ((int)7ULL))) {
        case 0:
            (gs4.m1++);
            ((*gp5) ^= 129);
        case 2:
            break;
        case 6:
            ;
        case 7:
            if ((w27 ^ (gs4.m1 ^= 129))) {
                break;
                f6((((long)2ULL) >= l25), g2, l25, &l24);
            } else {
                ((*gp5) <<= 6);
                l25 = (((unsigned char)(p22)) | (ga3[(w27 & ((int)3ULL))] >>= 0));
            }
            break;
        default:
            ;
        }
    } while (++w27 < 1);
    return p22;
}
int main(void) {
    int l28 = ((int)3ULL);
    unsigned char l29 = ((unsigned char)255ULL);
    unsigned char w30;
    unsigned char w31;
    unsigned char w32;
    (l29 <<= 0);
    g1 = (++l28);
    w30 = 0;
    while (w30++ < 2) {
        w31 = 0;
        do {
            gs4.m1 = (((int)(((unsigned int)65535ULL))) ? (g2 & ((signed char)0ULL)) : (l29 << 3));
            g2 = (g1 + w31);
            mix(f6(gs4.m0, (g2 * g2), (l28 + w31), &l29));
        } while (++w31 < 3);
        mix((-g2));
        (l29 -= 255);
    }
    w32 = 0;
    do {
        gs4.m0 = ((l29 > gs4.m1) - g1);
        if (g1) {
        }
    } while (++w32 < 1);
    fin0();
    fin1();
    return (int)(acc & 0x7FFF);
}
