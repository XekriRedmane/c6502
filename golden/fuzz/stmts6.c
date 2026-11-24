// expect: 32579
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned char g1 = ((unsigned char)255ULL);
unsigned int g2 = ((unsigned int)0ULL);
unsigned int z3 [[c6502::zeropage]] = ((unsigned int)65535ULL);
unsigned int z4 [[c6502::zeropage]] = ((unsigned int)0ULL);
signed char ga5[4] = {((signed char)255ULL), ((signed char)11ULL), ((signed char)0ULL), ((signed char)3ULL)};
struct S_gs6 { unsigned char pad; unsigned long m0; long m1; };
struct S_gs6 gs6 = {0, ((unsigned long)255ULL), ((long)32767ULL)};
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(z3);
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
signed char f7(unsigned int p8, int p9, unsigned char p10) {
    signed char l11 = ((signed char)255ULL);
    unsigned char w12;
    unsigned char w13;
    switch ((((int)(((g2--) & ((int)65535ULL)))) & ((int)7ULL))) {
    case 1:
        break;
    case 6:
        (++gs6.m0);
        w12 = 0;
        while (w12++ < 3) {
            ga5[(w12 & ((int)3ULL))] = ((unsigned char)(gs6.m0));
            switch ((((int)((l11 + (w12 ^ z3)))) & ((int)7ULL))) {
            case 7:
                break;
            }
            break;
            continue;
        }
        break;
    default:
        p9 = (p10 / ((int)7ULL));
    }
    p10 = g2;
    (l11++);
    switch ((((int)((ga5[((int)2ULL)]--))) & ((int)7ULL))) {
    case 4:
        break;
    default:
        w13 = 0;
        while (w13++ < 2) {
        }
    }
    return (g1 & z3);
}
unsigned int f14(signed char p15, unsigned int *pp16) {
    static int s18 = ((int)2ULL);
    unsigned long long l17 = ((unsigned long long)65536ULL);
    l17 = (((int)255ULL) + (p15 *= 3));
    mix((gs6.m0 >>= 7));
    p15 = ((g1 - gs6.m0) & (gs6.m1 - s18));
    if (((unsigned long long)(((unsigned int)3ULL)))) {
        g2 = (gs6.m1 / ((int)7ULL));
        mix(f7((gs6.m1 & ((unsigned int)6337ULL)), (z3 < g1), (gs6.m0 - g2)));
    }
    (g2 ^= 3);
    mix(((((unsigned char)127ULL) % ((int)7ULL)) / ((int)4ULL)));
    return ((unsigned int)(p15));
}
int main(void) {
    unsigned int l19 = ((unsigned int)13ULL);
    volatile unsigned long l20 = ((unsigned long)2147483647ULL);
    signed char l21 = ((signed char)0ULL);
    unsigned char w22;
    (l19 |= 90);
    switch ((((int)((l20 &= 129))) & ((int)7ULL))) {
    case 0:
        switch ((((int)((z3 | (z4--)))) & ((int)7ULL))) {
        case 3:
            switch ((((int)(((int)127ULL))) & ((int)7ULL))) {
            case 0:
                break;
                break;
            case 1:
                (ga5[((int)2ULL)] &= 90);
                break;
            case 2:
                ;
            case 4:
                (gs6.m0 <<= 1);
                gs6.m1 = ((--l20) & g2);
            default:
                break;
                break;
            }
        case 4:
            w22 = 0;
            while (w22++ < 4) {
                break;
            }
            break;
        case 6:
            break;
        default:
            ;
        }
        break;
    case 4:
        ;
    case 6:
        break;
    case 7:
        break;
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
