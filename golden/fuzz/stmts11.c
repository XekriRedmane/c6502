// expect: 2734
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
int g1 = ((int)32768ULL);
signed char g2 = ((signed char)2ULL);
unsigned char ga3[4] = {((unsigned char)127ULL), ((unsigned char)0ULL), ((unsigned char)255ULL), ((unsigned char)128ULL)};
struct S_gs4 { unsigned char pad; unsigned int m0; unsigned char m1; };
struct S_gs4 gs4 = {0, ((unsigned int)32768ULL), ((unsigned char)255ULL)};
signed char *gp5 = &g2;
int *gp6 = &g1;
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
unsigned char f7(unsigned long p8, unsigned char p9) {
    static int s13 = ((int)0ULL);
    volatile long l10 = ((long)2ULL);
    volatile unsigned char l11 = ((unsigned char)0ULL);
    int l12 = ((int)12ULL);
    unsigned char w14;
    unsigned char w15;
    switch ((((int)((!((*gp6) - ((unsigned char)0ULL))))) & ((int)7ULL))) {
    case 7:
        w14 = 0;
        while (w14++ < 4) {
            (--p9);
            l12 = ((l11 ^= 129) ^ ga3[(w14 & ((int)3ULL))]);
        }
        mix(((l12 *= 255) | ((unsigned int)65535ULL)));
        break;
    default:
        return ((signed char)(p9));
        (p8 &= 3);
    }
    mix((p8 + ((*gp5) == ((unsigned char)0ULL))));
    s13 = (((unsigned char)(((unsigned int)127ULL))) + (((unsigned char)7ULL) != (*gp6)));
    if (((p9 -= 129) & g2)) {
        w15 = 0;
        do {
            g1 = (gs4.m0 & (*gp6));
        } while (++w15 < 1);
    }
    return l12;
}
int main(void) {
    unsigned int l16 = ((unsigned int)127ULL);
    volatile unsigned int l17 = ((unsigned int)65535ULL);
    unsigned char w18;
    (gs4.m0--);
    switch ((((int)(g2)) & ((int)7ULL))) {
    case 7:
        break;
    default:
        ga3[((int)3ULL)] = (l16 += 3);
        w18 = 0;
        while (w18++ < 1) {
            switch ((((int)((++g1))) & ((int)7ULL))) {
            case 1:
                ;
            case 3:
                continue;
            case 4:
                break;
                break;
            case 7:
                mix(((++(*gp5)) & ((*gp6) ? l17 : l17)));
                break;
                break;
            }
        }
    }
    switch ((((int)((ga3[((int)3ULL)] & gs4.m1))) & ((int)7ULL))) {
    case 3:
        l17 = f7(((unsigned char)255ULL), ((signed char)128ULL));
        (++l17);
        break;
    case 7:
        ;
    }
    fin0();
    fin1();
    return (int)(acc & 0x7FFF);
}
