// expect: 21986
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
int g1 = ((int)0ULL);
unsigned char g2 = ((unsigned char)255ULL);
int z3 [[c6502::zeropage]] = ((int)37521ULL);
unsigned char z4 [[c6502::zeropage]] = ((unsigned char)128ULL);
signed char z5 [[c6502::zeropage]] = ((signed char)19ULL);
signed char ga6[4] = {((signed char)255ULL), ((signed char)3ULL), ((signed char)255ULL), ((signed char)1ULL)};
struct S_gs7 { unsigned char pad; int m0; unsigned long m1; };
struct S_gs7 gs7 = {0, ((int)128ULL), ((unsigned long)3ULL)};
unsigned char *gp8 = &z4;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(z3);
    mix(z4);
}
void fin1(void) {
    mix(z5);
    mix(ga6[((int)0ULL)]);
    mix(ga6[((int)1ULL)]);
    mix(ga6[((int)2ULL)]);
}
void fin2(void) {
    mix(ga6[((int)3ULL)]);
    mix(gs7.m0);
    mix(gs7.m1);
}
long long f9(unsigned char p10, unsigned char p11) {
    static volatile unsigned char s13 = ((unsigned char)203ULL);
    signed char l12 = ((signed char)255ULL);
    mix(((((unsigned long)65535ULL) == ((unsigned long long)2ULL)) & (gs7.m1 - gs7.m0)));
    gs7.m0 = (s13 -= 3);
    (++p10);
    return (gs7.m1 & (*gp8));
}
unsigned char f14(unsigned int p15, unsigned int p16) {
    static volatile signed char s20 = ((signed char)128ULL);
    unsigned int l17 = ((unsigned int)255ULL);
    unsigned int l18 = ((unsigned int)65535ULL);
    long l19 = ((long)0ULL);
    unsigned char w21;
    switch ((((int)((--l17))) & ((int)7ULL))) {
    case 6:
        break;
    default:
        w21 = 0;
        do {
            switch ((((int)((z5 + ((*gp8) * ((unsigned char)127ULL))))) & ((int)7ULL))) {
            case 2:
                mix(z4);
                break;
            default:
                break;
            }
            p16 = (++z4);
            z5 = (g1 <= w21);
            break;
        } while (++w21 < 3);
        l19 = f9(gs7.m0, gs7.m1);
    }
    gs7.m1 = (p16 -= 129);
    switch ((((int)((((unsigned long long)128ULL) == (z4 <= l17)))) & ((int)7ULL))) {
    case 1:
        ;
    case 2:
        break;
    case 6:
        break;
    default:
        ;
    }
    return (((signed char)1ULL) || ((signed char)128ULL));
}
int main(void) {
    unsigned int l22 = ((unsigned int)256ULL);
    unsigned char w23;
    if ((++gs7.m1)) {
        if (((gs7.m1 &= 255) + (gs7.m0 - z5))) {
            f9((((unsigned char)255ULL) * g1), ((int)65535ULL));
            w23 = 0;
            while (w23++ < 3) {
                l22 = (((int)0ULL) - g2);
                mix(f9((z3 || z5), ((int)255ULL)));
            }
        } else {
            f9((((unsigned int)128ULL) % ((int)2ULL)), ((*gp8) >= ((long)128ULL)));
            l22 = f9((z4 && g2), (((signed char)0ULL) | (*gp8)));
        }
    } else {
        if ((gs7.m1 -= 129)) {
            if ((*gp8)) {
                mix(gs7.m1);
            } else {
                l22 = f14((gs7.m1 >= ((unsigned char)255ULL)), (l22 + gs7.m1));
            }
        }
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
