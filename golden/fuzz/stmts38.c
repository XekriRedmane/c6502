// expect: 2429
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned int g1 = ((unsigned int)255ULL);
unsigned char g2 = ((unsigned char)128ULL);
unsigned char g3 = ((unsigned char)1ULL);
unsigned long long g4 = ((unsigned long long)65535ULL);
int z5 [[c6502::zeropage]] = ((int)0ULL);
unsigned long z6 [[c6502::zeropage]] = ((unsigned long)2147483648ULL);
int z7 [[c6502::zeropage]] = ((int)10ULL);
int ga8[4] = {((int)2ULL), ((int)32768ULL), ((int)1ULL), ((int)2ULL)};
struct S_gs9 { unsigned char pad; long long m0; unsigned int m1; };
struct S_gs9 gs9 = {0, ((long long)2147483648ULL), ((unsigned int)0ULL)};
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(g4);
}
void fin1(void) {
    mix(z5);
    mix(z6);
    mix(z7);
    mix(ga8[((int)0ULL)]);
}
void fin2(void) {
    mix(ga8[((int)1ULL)]);
    mix(ga8[((int)2ULL)]);
    mix(ga8[((int)3ULL)]);
    mix(gs9.m0);
}
void fin3(void) {
    mix(gs9.m1);
}
signed char f10(void) {
    static unsigned int s13 = ((unsigned int)0ULL);
    volatile unsigned int l11 = ((unsigned int)65535ULL);
    unsigned int l12 = ((unsigned int)11ULL);
    unsigned char w14;
    unsigned char w15;
    (z6 >>= 5);
    ga8[((int)2ULL)] = ((g4 <<= 5) ^ ga8[((int)1ULL)]);
    z6 = ((s13 != ((unsigned int)0ULL)) & g3);
    w14 = 0;
    while (w14++ < 4) {
        z6 = ((g3 <<= 2) | g1);
        z6 = ((--ga8[((int)1ULL)]) ^ g2);
        g1 = (g3 &= 3);
        mix((w14 >> 4));
    }
    w15 = 0;
    while (w15++ < 2) {
        (++l12);
    }
    (s13 |= 5);
    return ((long long)(z6));
}
unsigned int f16(unsigned int *pp17) {
    static signed char s19 = ((signed char)127ULL);
    unsigned int l18 = ((unsigned int)2ULL);
    f10();
    mix((z5 >= g4));
    (z6++);
    l18 = f10();
    return s19;
}
unsigned char f20(signed char p21, unsigned int p22, unsigned int p23) {
    unsigned int l24 = ((unsigned int)3ULL);
    z5 = (ga8[((int)0ULL)] | (--z6));
    mix(f16(&l24));
    p23 = ((unsigned int)(ga8[((int)3ULL)]));
    mix(l24);
    mix(f16(&l24));
    if ((g3 % ((int)4ULL))) {
        return (p23 -= 5);
    } else {
        l24 = f10();
        mix(((z5 % ((int)1ULL)) & (++p22)));
    }
    return (((unsigned int)65535ULL) || ((unsigned int)127ULL));
}
int main(void) {
    volatile signed char l25 = ((signed char)0ULL);
    int l26 = ((int)255ULL);
    unsigned int l27 = ((unsigned int)65535ULL);
    unsigned char w28;
    mix((((int)32767ULL) & (gs9.m1--)));
    switch ((((int)((~(g4 - ((unsigned long)2ULL))))) & ((int)7ULL))) {
    case 0:
        break;
    case 4:
        mix((gs9.m1 - (z5 <<= 6)));
    case 6:
        break;
    case 7:
        g1 = (ga8[((int)0ULL)] += 129);
        w28 = 0;
        while (w28++ < 3) {
            l26 = (!z6);
            if (((g1 ^= 129) + (g2 % ((int)4ULL)))) {
                mix(w28);
                mix(((z7 -= 1) | gs9.m1));
            } else {
                l26 = (((unsigned long long)256ULL) + (l27 -= 3));
            }
            (z5 <<= 3);
            (z5--);
        }
        break;
    }
    fin0();
    fin1();
    fin2();
    fin3();
    return (int)(acc & 0x7FFF);
}
