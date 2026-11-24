// expect: 206
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned int g1 = ((unsigned int)65535ULL);
unsigned _BitInt(33) g2 = ((unsigned _BitInt(33))65535ULL);
unsigned int g3 = ((unsigned int)65535ULL);
signed char z4 [[c6502::zeropage]] = ((signed char)1ULL);
unsigned int ga5[4] = {((unsigned int)128ULL), ((unsigned int)65535ULL), ((unsigned int)1ULL), ((unsigned int)127ULL)};
struct S_gs6 { unsigned char pad; unsigned long m0; unsigned long m1; };
struct S_gs6 gs6 = {0, ((unsigned long)32768ULL), ((unsigned long)11ULL)};
signed char *gp7 = &z4;
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
int f8(signed char p9) {
    unsigned _BitInt(33) l10 = ((unsigned _BitInt(33))8036165736ULL);
    unsigned _BitInt(28) l11 = ((unsigned _BitInt(28))1ULL);
    unsigned char w12;
    switch ((((int)((++g1))) & ((int)7ULL))) {
    case 2:
        w12 = 0;
        do {
            p9 = (++g3);
            (g2 ^= 255);
        } while (++w12 < 3);
    case 3:
        mix(((z4 *= 129) ^ ((unsigned char)2ULL)));
        break;
    case 6:
        ;
    default:
        mix((gs6.m1 ^= 90));
        (gs6.m0 &= 90);
    }
    mix((l11 ^ (z4 <<= 3)));
    (gs6.m0 <<= 5);
    mix((((unsigned char)255ULL) | (gs6.m0 <<= 1)));
    (p9 &= 129);
    (g1 += 129);
    return (l11 - g3);
}
signed char f13(void) {
    static long s16 = ((long)2ULL);
    unsigned int l14 = ((unsigned int)128ULL);
    int l15 = ((int)0ULL);
    f8((z4 & z4));
    if ((--g2)) {
        (*gp7) = ga5[((int)0ULL)];
        ((*gp7) -= 255);
    } else {
        mix(((((unsigned _BitInt(33))1ULL) + z4) | ((unsigned int)(gs6.m0))));
        switch ((((int)((g3 + (l15 -= 129)))) & ((int)7ULL))) {
        case 0:
            g3 = (ga5[((int)2ULL)] |= 90);
            (--gs6.m1);
        case 1:
            break;
        default:
            switch ((((int)((~((long)2147483647ULL)))) & ((int)7ULL))) {
            case 1:
                mix((l14 *= 1));
            case 3:
                break;
            case 6:
                mix(((l14 >>= 3) & (gs6.m1 + gs6.m1)));
                break;
            case 7:
                (g2++);
                break;
            default:
                ;
            }
        }
    }
    return (z4 + ((int)65535ULL));
}
int main(void) {
    int l17 = ((int)65535ULL);
    unsigned long l18 = ((unsigned long)6ULL);
    unsigned char l19 = ((unsigned char)0ULL);
    unsigned char w20;
    ((*gp7) <<= 6);
    gs6.m1 = gs6.m1;
    mix(l19);
    (++g3);
    w20 = 0;
    while (w20++ < 3) {
        for (unsigned char i21 = 0; i21 < 3; i21++) {
            break;
        }
    }
    ga5[((int)2ULL)] = ((g2 &= 1) ^ l18);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
