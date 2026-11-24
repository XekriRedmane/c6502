// expect: 25723
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned long g1 = ((unsigned long)3ULL);
long g2 = ((long)65535ULL);
unsigned int g3 = ((unsigned int)255ULL);
volatile unsigned int g4 = ((unsigned int)41175ULL);
signed char z5 [[c6502::zeropage]] = ((signed char)3ULL);
volatile unsigned char z6 [[c6502::zeropage]] = ((unsigned char)5ULL);
signed char z7 [[c6502::zeropage]] = ((signed char)255ULL);
unsigned char ga8[4] = {((unsigned char)0ULL), ((unsigned char)128ULL), ((unsigned char)255ULL), ((unsigned char)0ULL)};
struct S_gs9 { unsigned char pad; long m0; unsigned char m1; };
struct S_gs9 gs9 = {0, ((long)8ULL), ((unsigned char)255ULL)};
signed char *gp10 = &z5;
long *gp11 = &g2;
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
signed char f12(long p13, unsigned long p14) {
    static int s17 = ((int)256ULL);
    long l15 = ((long)2147483647ULL);
    unsigned int l16 = ((unsigned int)10ULL);
    switch ((((int)((*gp11))) & ((int)7ULL))) {
    case 2:
        ;
    case 7:
        gs9.m1 = (++s17);
        switch ((((int)(z6)) & ((int)7ULL))) {
        case 1:
            return (g1 <<= 6);
        case 4:
            (*gp11) = (--g3);
            mix((z7 | (*gp10)));
        case 7:
            if (z5) {
                mix((((long)2147483647ULL) ^ (g3 <<= 5)));
                break;
            } else {
                break;
                mix(((*gp10) |= 129));
            }
            z5 = (l16 *= 5);
            break;
        }
    default:
        ;
    }
    return (p14 + ga8[((int)1ULL)]);
}
unsigned char f18(signed char p19) {
    static unsigned char s23 = ((unsigned char)255ULL);
    unsigned long l20 = ((unsigned long)1ULL);
    signed char l21 = ((signed char)8ULL);
    volatile unsigned char l22 = ((unsigned char)255ULL);
    unsigned char w24;
    mix(f12((((signed char)255ULL) ^ g4), (((signed char)0ULL) >= g4)));
    f12((g2 * ga8[((int)1ULL)]), ((unsigned char)((*gp11))));
    if ((gs9.m0 <<= 1)) {
        w24 = 0;
        while (w24++ < 4) {
            switch ((((int)(((*gp10) += 255))) & ((int)7ULL))) {
            case 2:
                z7 = (p19 | (--l22));
                break;
            default:
                gs9.m1 = ((unsigned long)(w24));
            }
            if ((g4 <<= 4)) {
                g4 = (z5 - (l21--));
                gs9.m0 = (ga8[(w24 & ((int)3ULL))] + w24);
                break;
            }
            f12(((unsigned char)(((unsigned char)0ULL))), (l22 % ((int)5ULL)));
        }
    }
    return (*gp10);
}
int main(void) {
    unsigned int l25 = ((unsigned int)0ULL);
    mix(f18(((*gp11) * z6)));
    for (unsigned char i26 = 0; i26 < 3; i26++) {
        if ((g3 | (--l25))) {
            l25 = ((z6 |= 90) + i26);
            z5 = (g1 ^= 255);
            if ((g1 += 255)) {
                break;
                (g2 &= 3);
                gs9.m1 = (((unsigned int)(ga8[(i26 & ((int)3ULL))])) + (gs9.m0 ^= 255));
            }
        } else {
            if (z7) {
                break;
                mix(f12((i26 / ((int)1ULL)), (z6 != z6)));
            }
        }
    }
    fin0();
    fin1();
    fin2();
    fin3();
    return (int)(acc & 0x7FFF);
}
