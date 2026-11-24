// expect: 9631
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned _BitInt(53) g1 = ((unsigned _BitInt(53))3ULL);
long g2 = ((long)0ULL);
unsigned char g3 = ((unsigned char)255ULL);
unsigned char g4 = ((unsigned char)3ULL);
long long z5 [[c6502::zeropage]] = ((long long)256ULL);
_BitInt(49) z6 [[c6502::zeropage]] = ((_BitInt(49))32767ULL);
volatile long long z7 [[c6502::zeropage]] = ((long long)3ULL);
long ga8[4] = {((long)2147483648ULL), ((long)127ULL), ((long)65536ULL), ((long)4033488792ULL)};
struct S_gs9 { unsigned char pad; unsigned _BitInt(53) m0; unsigned char m1; };
struct S_gs9 gs9 = {0, ((unsigned _BitInt(53))12ULL), ((unsigned char)255ULL)};
long long *gp10 = &z5;
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
int f12(unsigned int p13, unsigned int p14) {
    int l15 = ((int)255ULL);
    volatile int l16 = ((int)127ULL);
    volatile int l17 = ((int)1ULL);
    unsigned char w18;
    mix(((*gp10) | (ga8[((int)1ULL)] *= 90)));
    g3 = l15;
    w18 = 0;
    while (w18++ < 1) {
        break;
        for (unsigned char i19 = 0; i19 < 3; i19++) {
            switch ((((int)((((long long)65536ULL) != ((unsigned char)(g4))))) & ((int)7ULL))) {
            case 0:
                mix(((++(*gp10)) ^ (i19 % ((int)1ULL))));
                break;
                break;
            case 1:
                break;
                break;
            case 6:
                ;
            case 7:
                ;
            }
            if ((g4 <<= 7)) {
                l17 = (g4 << 2);
                g1 = (((_BitInt(49))3ULL) + (g4 &= 90));
            }
        }
    }
    return (l16 - (*gp11));
}
int main(void) {
    unsigned char l20 = ((unsigned char)0ULL);
    long l21 = ((long)65536ULL);
    signed char l22 = ((signed char)0ULL);
    unsigned char w23;
    unsigned char w24;
    mix((g4 != ga8[((int)0ULL)]));
    if ((ga8[((int)0ULL)] > ((unsigned _BitInt(53))65536ULL))) {
        w23 = 0;
        while (w23++ < 3) {
            break;
        }
        l21 = f12((!gs9.m0), (((unsigned int)0ULL) < l20));
    } else {
        mix((ga8[((int)2ULL)] &= 3));
    }
    l21 = ((l22 ^= 129) - (*gp11));
    if (((long long)(l21))) {
        w24 = 0;
        while (w24++ < 1) {
            if (((unsigned int)(w24))) {
                break;
                l20 = f12(((*gp11) || g1), (ga8[(w24 & ((int)3ULL))] != (*gp11)));
            }
        }
    }
    fin0();
    fin1();
    fin2();
    fin3();
    return (int)(acc & 0x7FFF);
}
