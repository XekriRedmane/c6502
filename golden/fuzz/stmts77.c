// expect: 19109
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile unsigned char g1 = ((unsigned char)0ULL);
unsigned int g2 = ((unsigned int)255ULL);
volatile unsigned char g3 = ((unsigned char)255ULL);
volatile unsigned char z4 [[c6502::zeropage]] = ((unsigned char)9ULL);
unsigned int z5 [[c6502::zeropage]] = ((unsigned int)127ULL);
signed char z6 [[c6502::zeropage]] = ((signed char)127ULL);
unsigned char ga7[4] = {((unsigned char)0ULL), ((unsigned char)76ULL), ((unsigned char)0ULL), ((unsigned char)0ULL)};
struct S_gs8 { unsigned char pad; unsigned char m0; unsigned char m1; };
struct S_gs8 gs8 = {0, ((unsigned char)128ULL), ((unsigned char)0ULL)};
signed char *gp9 = &z6;
unsigned int *gp10 = &z5;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(z4);
}
void fin1(void) {
    mix(z5);
    mix(z6);
    mix(ga7[((int)0ULL)]);
    mix(ga7[((int)1ULL)]);
}
void fin2(void) {
    mix(ga7[((int)2ULL)]);
    mix(ga7[((int)3ULL)]);
    mix(gs8.m0);
    mix(gs8.m1);
}
int f11(signed char *pp12) {
    signed char l13 = ((signed char)5ULL);
    volatile unsigned long long l14 = ((unsigned long long)16424790632631474804ULL);
    unsigned char w15;
    w15 = 0;
    do {
        continue;
        switch ((((int)(((gs8.m0--) + (*gp10)))) & ((int)7ULL))) {
        case 0:
            (--g2);
            gs8.m1 = (g2 -= 3);
            break;
        case 2:
            ;
        case 3:
            break;
        default:
            (z4 += 255);
            switch ((((int)((g2 ^= 255))) & ((int)7ULL))) {
            case 0:
                (z4 += 129);
                (*gp10) = (g3 ^ (z6--));
                break;
            case 1:
                ga7[(w15 & ((int)3ULL))] = w15;
                mix((g2 << 6));
            case 4:
                (ga7[(w15 & ((int)3ULL))] <<= 4);
                break;
            }
        }
    } while (++w15 < 2);
    return (g3 % ((int)4ULL));
}
signed char f16(signed char p17, unsigned char p18, int p19) {
    unsigned long l20 = ((unsigned long)2147483647ULL);
    signed char l21 = ((signed char)255ULL);
    unsigned char w22;
    g2 = ((int)((*gp9)));
    mix((p18 & p19));
    w22 = 0;
    while (w22++ < 1) {
        l20 = ((*gp9) >>= 0);
        g3 = ((gs8.m0 & l20) | w22);
        l21 = (((long long)32767ULL) * ((unsigned char)2ULL));
    }
    if ((z4 ^= 90)) {
        ((*gp10) *= 1);
        return (~(*gp10));
    }
    if ((((*gp9) -= 5) ^ ga7[((int)3ULL)])) {
        return ((l21 * gs8.m0) ^ ((*gp10)--));
    }
    l21 = z4;
    return (l20 < p19);
}
unsigned char f23(int *pp24) {
    long l25 = ((long)256ULL);
    signed char l26 = ((signed char)255ULL);
    unsigned char w29;
    switch ((((int)((g3++))) & ((int)7ULL))) {
    case 5:
        (z4--);
        for (unsigned char i27 = 0; i27 < 1; i27++) {
            gs8.m1 = ((z5 > (*gp9)) % ((int)7ULL));
            return (g2--);
            mix(((unsigned long)1997571591ULL));
            (g3++);
        }
        break;
    default:
        (*pp24) = ((gs8.m1 |= 5) - ((unsigned long)2147483648ULL));
    }
    (ga7[((int)0ULL)] ^= 1);
    f11(&l26);
    for (unsigned char i28 = 0; i28 < 4; i28++) {
        w29 = 0;
        do {
        } while (++w29 < 1);
    }
    return (((unsigned int)16225ULL) % ((int)1ULL));
}
int main(void) {
    unsigned char l30 = ((unsigned char)0ULL);
    signed char l31 = ((signed char)255ULL);
    int l32 = ((int)65535ULL);
    unsigned char w33;
    g2 = (z6 && gs8.m1);
    l32 = (ga7[((int)3ULL)] | ((unsigned char)255ULL));
    w33 = 0;
    while (w33++ < 3) {
        if ((g2 >>= 1)) {
            f16((l32 / ((int)2ULL)), ((*gp10) == ((unsigned int)255ULL)), (((unsigned int)1ULL) - l32));
            continue;
            gs8.m0 = (z5 - ((unsigned int)32767ULL));
        } else {
            f23(&l32);
            switch ((((int)(((unsigned char)(((int)(gs8.m0)))))) & ((int)7ULL))) {
            case 1:
                break;
            case 5:
                break;
            default:
                mix(((g1 ^= 3) | (g3 << 4)));
                mix((l32 >> 1));
            }
        }
        (g2 |= 90);
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
