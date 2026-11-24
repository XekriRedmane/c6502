// expect: 1131
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
int g1 = ((int)0ULL);
volatile long g2 = ((long)65536ULL);
int g3 = ((int)65535ULL);
unsigned long z4 [[c6502::zeropage]] = ((unsigned long)32768ULL);
long ga5[4] = {((long)65535ULL), ((long)3ULL), ((long)0ULL), ((long)2147483647ULL)};
struct S_gs6 { unsigned char pad; signed char m0; int m1; };
struct S_gs6 gs6 = {0, ((signed char)127ULL), ((int)27086ULL)};
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(z4);
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
int f7(unsigned _BitInt(59) p8, signed char *pp9) {
    unsigned char l10 = ((unsigned char)1ULL);
    unsigned char w12;
    for (unsigned char i11 = 0; i11 < 1; i11++) {
        (g2--);
        mix((--gs6.m0));
    }
    w12 = 0;
    do {
        (gs6.m1 <<= 5);
        switch ((((int)((g3 >>= 5))) & ((int)7ULL))) {
        case 1:
            if ((++gs6.m1)) {
                break;
                break;
            } else {
                (ga5[((int)1ULL)] -= 5);
                break;
            }
            break;
        case 3:
            ;
        case 4:
            mix(((!ga5[(w12 & ((int)3ULL))]) < (w12 != ga5[(w12 & ((int)3ULL))])));
            break;
        case 5:
            ;
        default:
            ;
        }
    } while (++w12 < 1);
    return gs6.m0;
}
long f13(void) {
    static unsigned int s17 = ((unsigned int)127ULL);
    long l14 = ((long)12ULL);
    unsigned long long l15 = ((unsigned long long)2147483647ULL);
    signed char l16 = ((signed char)255ULL);
    if (((--gs6.m0) ^ g3)) {
        if ((((unsigned int)3ULL) & (gs6.m0 ? l14 : ((int)0ULL)))) {
            if ((((unsigned char)255ULL) % ((int)6ULL))) {
                return (((unsigned char)0ULL) & ((unsigned char)3ULL));
            }
            switch ((((int)((g2 |= 1))) & ((int)7ULL))) {
            case 2:
                break;
            case 5:
                mix((((unsigned _BitInt(25))1ULL) <= ga5[((int)1ULL)]));
            }
        } else {
            switch ((((int)((l14 + l15))) & ((int)7ULL))) {
            case 6:
                break;
            default:
                g3 = (gs6.m0 ^ g3);
            }
            return (g2 - ((signed char)2ULL));
        }
        (z4 |= 5);
    }
    return l16;
}
int main(void) {
    volatile unsigned long l18 = ((unsigned long)65535ULL);
    signed char l19 = ((signed char)255ULL);
    unsigned char w20;
    unsigned char w21;
    mix((g1 ^ (g1 != ((long)65536ULL))));
    w20 = 0;
    while (w20++ < 4) {
        w21 = 0;
        while (w21++ < 2) {
            switch ((((int)((l18 ^= 255))) & ((int)7ULL))) {
            case 2:
                ;
            case 4:
                (--gs6.m1);
                mix((l19 += 3));
                break;
            default:
                (l18 -= 1);
                break;
            }
            (--gs6.m1);
            gs6.m1 = (((unsigned long)256ULL) - (((long)3ULL) & ((unsigned int)0ULL)));
        }
        g3 = ((unsigned long)((g2 - gs6.m1)));
    }
    if (((gs6.m0 >>= 5) - g3)) {
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
