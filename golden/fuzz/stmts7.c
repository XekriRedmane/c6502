// expect: 15696
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile signed char g1 = ((signed char)0ULL);
int g2 = ((int)127ULL);
long g3 = ((long)1ULL);
signed char ga4[4] = {((signed char)0ULL), ((signed char)2ULL), ((signed char)255ULL), ((signed char)1ULL)};
struct S_gs5 { unsigned char pad; unsigned char m0; unsigned char m1; };
struct S_gs5 gs5 = {0, ((unsigned char)127ULL), ((unsigned char)0ULL)};
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
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
unsigned char f6(signed char p7, long p8, signed char *pp9) {
    unsigned long l10 = ((unsigned long)0ULL);
    volatile int l11 = ((int)3ULL);
    volatile unsigned char l12 = ((unsigned char)0ULL);
    unsigned char w13;
    unsigned char w14;
    w13 = 0;
    while (w13++ < 1) {
        if (gs5.m1) {
            continue;
        }
        gs5.m0 = l11;
        w14 = 0;
        while (w14++ < 4) {
            switch ((((int)((l12 & (*pp9)))) & ((int)7ULL))) {
            case 3:
                break;
                break;
            }
            if (g2) {
                break;
            }
        }
        p7 = (gs5.m0 && w13);
    }
    g1 = ((ga4[((int)1ULL)] *= 129) | g1);
    gs5.m0 = (((signed char)127ULL) + l10);
    return ((unsigned char)(g1));
}
signed char f15(unsigned int p16, unsigned int *pp17) {
    volatile unsigned char l18 = ((unsigned char)3ULL);
    unsigned int l19 = ((unsigned int)3ULL);
    int l20 = ((int)32768ULL);
    signed char l21 = ((signed char)1ULL);
    g2 = l19;
    (--l18);
    mix((-p16));
    p16 = ((l20 >>= 2) - l21);
    l19 = g1;
    return p16;
}
int main(void) {
    unsigned int l22 = ((unsigned int)0ULL);
    volatile unsigned char l23 = ((unsigned char)1ULL);
    unsigned int l24 = ((unsigned int)128ULL);
    signed char l25 = ((signed char)255ULL);
    (++l24);
    mix((l24 & (l23 += 255)));
    (g3 ^= 1);
    l24 = ((g1 & ga4[((int)1ULL)]) >> 1);
    g2 = ((ga4[((int)1ULL)] || ((unsigned long)5ULL)) | (gs5.m0 -= 5));
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
