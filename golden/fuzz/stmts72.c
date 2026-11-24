// expect: 27609
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile long long g1 = ((long long)3ULL);
unsigned char g2 = ((unsigned char)255ULL);
signed char g3 = ((signed char)255ULL);
unsigned int z4 [[c6502::zeropage]] = ((unsigned int)1ULL);
int ga5[4] = {((int)255ULL), ((int)32767ULL), ((int)0ULL), ((int)128ULL)};
struct S_gs6 { unsigned char pad; _BitInt(57) m0; int m1; };
struct S_gs6 gs6 = {0, ((_BitInt(57))32768ULL), ((int)0ULL)};
unsigned char *gp7 = &g2;
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
unsigned int f8(long p9, unsigned char *pp10) {
    static signed char s12 = ((signed char)1ULL);
    unsigned int l11 = ((unsigned int)0ULL);
    unsigned char w13;
    unsigned char w14;
    switch ((((int)((z4 >>= 7))) & ((int)7ULL))) {
    case 0:
        w13 = 0;
        do {
            mix(g3);
            w14 = 0;
            do {
                break;
            } while (++w14 < 1);
        } while (++w13 < 2);
        break;
    case 1:
        ;
    case 5:
        gs6.m1 = (s12 ^ (--l11));
    default:
        if (g3) {
            break;
            switch ((((int)((++(*pp10)))) & ((int)7ULL))) {
            case 0:
                ;
            case 4:
                ;
            default:
                l11 = (s12 & ((unsigned long)32767ULL));
            }
        }
    }
    if ((z4 - ((int)3ULL))) {
        gs6.m1 = ((l11 | l11) < (g1 & gs6.m1));
    }
    return ((*pp10) << 4);
}
long long f15(unsigned int p16, unsigned char *pp17) {
    volatile unsigned long l18 = ((unsigned long)2147483647ULL);
    _BitInt(57) l19 = ((_BitInt(57))68642922949994493ULL);
    unsigned char l20 = ((unsigned char)0ULL);
    unsigned char w22;
    if ((g1++)) {
        mix(f8(g3, &l20));
        z4 = ((signed char)3ULL);
        (l18 >>= 6);
    }
    for (unsigned char i21 = 0; i21 < 3; i21++) {
        continue;
    }
    w22 = 0;
    do {
        (*pp17) = (g3 |= 255);
    } while (++w22 < 3);
    return gs6.m0;
}
unsigned int f23(int p24, long long p25) {
    static unsigned char s29 = ((unsigned char)0ULL);
    volatile signed char l26 = ((signed char)1ULL);
    _BitInt(57) l27 = ((_BitInt(57))32768ULL);
    unsigned char l28 = ((unsigned char)255ULL);
    unsigned char w30;
    (p25 >>= 1);
    switch ((((int)(((gs6.m1 && l26) + g2))) & ((int)7ULL))) {
    case 0:
        return (l28 ^ (p24++));
    case 1:
        mix((z4 <= p24));
    case 6:
        break;
        s29 = ((l28 ^= 255) | p25);
        break;
    default:
        w30 = 0;
        while (w30++ < 1) {
            return ((*gp7) |= 5);
            (++s29);
        }
    }
    mix(((--l27) | (z4 ? ((unsigned int)32767ULL) : z4)));
    return p24;
}
int main(void) {
    unsigned long l31 = ((unsigned long)128ULL);
    long l32 = ((long)32768ULL);
    volatile signed char l33 = ((signed char)171ULL);
    unsigned char l34 = ((unsigned char)15ULL);
    unsigned char w35;
    g1 = (g3 ^= 3);
    mix((l31 & ((unsigned long long)127ULL)));
    if ((l33 != (g1 + ((unsigned char)128ULL)))) {
        switch ((((int)(((l33 + ((unsigned long)2032661458ULL)) + (z4 - gs6.m0)))) & ((int)7ULL))) {
        case 1:
            switch ((((int)(((gs6.m0++) | ((_BitInt(61))2147483648ULL)))) & ((int)7ULL))) {
            case 1:
                break;
            case 6:
                mix((g2 ^ (--gs6.m0)));
            default:
                (ga5[((int)3ULL)] -= 90);
            }
            break;
        case 3:
            break;
            l32 = (g3 * ((unsigned char)2ULL));
            break;
        default:
            mix(((*gp7) << 2));
        }
        w35 = 0;
        do {
            g2 = ((l32--) + z4);
        } while (++w35 < 4);
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
