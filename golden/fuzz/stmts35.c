// expect: 27894
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned int g1 = ((unsigned int)0ULL);
signed char g2 = ((signed char)255ULL);
unsigned long ga3[4] = {((unsigned long)2147483647ULL), ((unsigned long)32768ULL), ((unsigned long)3ULL), ((unsigned long)0ULL)};
struct S_gs4 { unsigned char pad; unsigned char m0; unsigned long m1; };
struct S_gs4 gs4 = {0, ((unsigned char)0ULL), ((unsigned long)32767ULL)};
unsigned int *gp5 = &g1;
signed char *gp6 = &g2;
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
unsigned long f7(_BitInt(44) p8, unsigned long p9, unsigned long *pp10) {
    unsigned char l11 = ((unsigned char)255ULL);
    _BitInt(33) l12 = ((_BitInt(33))128ULL);
    unsigned char w13;
    mix((ga3[((int)3ULL)]--));
    switch ((((int)(g1)) & ((int)7ULL))) {
    case 4:
        ;
    case 7:
        ;
    default:
        p8 = (((signed char)255ULL) + ((unsigned char)255ULL));
    }
    w13 = 0;
    do {
        g1 = (p8 &= 255);
        (gs4.m0 *= 3);
        mix((*gp5));
    } while (++w13 < 3);
    for (unsigned char i14 = 0; i14 < 3; i14++) {
        for (unsigned char i15 = 0; i15 < 4; i15++) {
            l12 = (g2--);
            mix(((l12 += 90) & (*pp10)));
        }
        if ((((signed char)0ULL) ^ (--gs4.m0))) {
        }
    }
    return (l11 ? l11 : (*pp10));
}
_BitInt(44) f16(_BitInt(44) p17, long p18, signed char p19, signed char *pp20) {
    static signed char s23 = ((signed char)0ULL);
    unsigned char l21 = ((unsigned char)255ULL);
    unsigned long l22 = ((unsigned long)3ULL);
    ((*pp20) >>= 1);
    mix(((p18--) ^ (((unsigned char)1ULL) == ((int)32768ULL))));
    l21 = f7(((unsigned long)32767ULL), (~g1), &l22);
    switch ((((int)((((*pp20) ^ ((int)0ULL)) + ((unsigned char)3ULL)))) & ((int)7ULL))) {
    case 7:
        return ((p17 <<= 4) + ga3[((int)0ULL)]);
    default:
        (--(*pp20));
    }
    return (l21 <= ga3[((int)2ULL)]);
}
unsigned char f24(long p25, unsigned int *pp26) {
    signed char l27 = ((signed char)128ULL);
    unsigned long l28 = ((unsigned long)32768ULL);
    f16((((unsigned int)127ULL) ? g2 : gs4.m0), (-l27), (gs4.m1 / ((int)5ULL)), &l27);
    mix((gs4.m0 |= 129));
    mix(f16((l28 & ga3[((int)2ULL)]), ((*pp26) - ((signed char)3ULL)), l27, &l27));
    return (((unsigned int)2ULL) * ((_BitInt(44))65535ULL));
}
int main(void) {
    signed char l29 = ((signed char)3ULL);
    int l30 = ((int)255ULL);
    unsigned long l31 = ((unsigned long)2147483647ULL);
    unsigned int l32 = ((unsigned int)3ULL);
    l32 = f7((gs4.m1 != l30), g1, &l31);
    ((*gp5) >>= 0);
    l30 = f16((g1 / ((int)5ULL)), l29, ((unsigned long)32767ULL), &l29);
    if ((*gp6)) {
        f24((l32 != gs4.m1), &l32);
        mix(f16(((long)0ULL), ((unsigned char)(l29)), (*gp6), &l29));
        mix((gs4.m0 % ((int)5ULL)));
    }
    for (unsigned char i33 = 0; i33 < 1; i33++) {
        (gs4.m0 >>= 6);
        break;
    }
    l30 = f7((!(*gp6)), (l30 == (*gp6)), &l31);
    fin0();
    fin1();
    return (int)(acc & 0x7FFF);
}
