// expect: 24101
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned char g1 = ((unsigned char)2ULL);
int g2 = ((int)61063ULL);
signed char ga3[4] = {((signed char)31ULL), ((signed char)0ULL), ((signed char)255ULL), ((signed char)127ULL)};
struct S_gs4 { unsigned char pad; signed char m0; long m1; };
struct S_gs4 gs4 = {0, ((signed char)128ULL), ((long)2147483648ULL)};
int *gp5 = &g2;
unsigned char *gp6 = &g1;
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
unsigned int f7(unsigned long long p8, signed char p9, unsigned char p10) {
    static unsigned char s13 = ((unsigned char)127ULL);
    int l11 = ((int)0ULL);
    volatile int l12 = ((int)65018ULL);
    unsigned char w14;
    unsigned char w15;
    unsigned char w16;
    mix((gs4.m1--));
    mix((--(*gp5)));
    mix(l11);
    w14 = 0;
    while (w14++ < 3) {
        if ((*gp5)) {
            (p10 <<= 4);
        } else {
            break;
            w15 = 0;
            while (w15++ < 4) {
                break;
                (*gp6) = w15;
            }
        }
        w16 = 0;
        while (w16++ < 3) {
            (l11 &= 129);
        }
    }
    return (g2 / ((int)7ULL));
}
int main(void) {
    volatile unsigned int l17 = ((unsigned int)0ULL);
    unsigned int l18 = ((unsigned int)65535ULL);
    int l19 = ((int)127ULL);
    unsigned char w20;
    l18 = ((g1 -= 90) + ((unsigned long long)(l17)));
    w20 = 0;
    while (w20++ < 1) {
        (g2 -= 1);
        mix((g2 ? g2 : (((unsigned char)242ULL) + gs4.m1)));
        (++gs4.m1);
    }
    f7((~(*gp5)), (g1 + ga3[((int)2ULL)]), ((unsigned long)(((unsigned long)256ULL))));
    mix(((l17 -= 90) ^ g2));
    fin0();
    fin1();
    return (int)(acc & 0x7FFF);
}
