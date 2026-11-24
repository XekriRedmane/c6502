// expect: 14900
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned long g1 = ((unsigned long)256ULL);
signed char g2 = ((signed char)255ULL);
_BitInt(61) g3 = ((_BitInt(61))1998639239459844970ULL);
_BitInt(61) z4 [[c6502::zeropage]] = ((_BitInt(61))2ULL);
unsigned long ga5[4] = {((unsigned long)32767ULL), ((unsigned long)128ULL), ((unsigned long)11ULL), ((unsigned long)1ULL)};
struct S_gs6 { unsigned char pad; signed char m0; signed char m1; };
struct S_gs6 gs6 = {0, ((signed char)0ULL), ((signed char)0ULL)};
_BitInt(61) *gp7 = &g3;
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
_BitInt(61) f8(void) {
    static volatile long s11 = ((long)1ULL);
    signed char l9 = ((signed char)0ULL);
    volatile unsigned int l10 = ((unsigned int)65535ULL);
    unsigned char w12;
    unsigned char w13;
    z4 = l9;
    w12 = 0;
    while (w12++ < 4) {
        w13 = 0;
        do {
            return ((gs6.m0 >> 7) >= (gs6.m1 << 2));
            switch ((((int)(((_BitInt(61))32767ULL))) & ((int)7ULL))) {
            case 0:
                mix(l10);
                break;
            case 3:
                (s11 >>= 2);
                break;
            case 4:
                ;
            case 5:
                break;
            }
        } while (++w13 < 1);
        (*gp7) = ((g2 &= 3) - l9);
    }
    mix((ga5[((int)2ULL)] + (g2 ^= 90)));
    (*gp7) = z4;
    return (((signed char)0ULL) % ((int)1ULL));
}
unsigned int f14(int p15) {
    static unsigned char s17 = ((unsigned char)2ULL);
    int l16 = ((int)0ULL);
    unsigned char w18;
    (ga5[((int)0ULL)] >>= 7);
    w18 = 0;
    do {
        switch ((((int)((((long)6ULL) + (*gp7)))) & ((int)7ULL))) {
        case 2:
            switch ((((int)((ga5[(w18 & ((int)3ULL))] && w18))) & ((int)7ULL))) {
            case 4:
                (gs6.m1 *= 90);
                break;
                break;
            }
            break;
        case 3:
            break;
        case 7:
            ;
        default:
            for (unsigned char i19 = 0; i19 < 3; i19++) {
                mix((i19 <= s17));
                break;
                ga5[(i19 & ((int)3ULL))] = s17;
                break;
            }
            switch ((((int)(gs6.m1)) & ((int)7ULL))) {
            case 4:
                break;
            default:
                ;
            }
        }
    } while (++w18 < 2);
    return (z4 != l16);
}
int main(void) {
    int l20 = ((int)0ULL);
    int l21 = ((int)128ULL);
    (--z4);
    z4 = ((unsigned int)0ULL);
    (l21 <<= 6);
    l20 = f14((z4 - l20));
    gs6.m0 = ((g3++) - (l21 | g1));
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
