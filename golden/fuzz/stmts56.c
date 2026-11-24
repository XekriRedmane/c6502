// expect: 2402
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile int g1 = ((int)0ULL);
unsigned int g2 = ((unsigned int)3ULL);
signed char z3 [[c6502::zeropage]] = ((signed char)128ULL);
volatile unsigned int z4 [[c6502::zeropage]] = ((unsigned int)32768ULL);
unsigned long z5 [[c6502::zeropage]] = ((unsigned long)128ULL);
signed char ga6[4] = {((signed char)255ULL), ((signed char)255ULL), ((signed char)1ULL), ((signed char)0ULL)};
struct S_gs7 { unsigned char pad; int m0; unsigned int m1; };
struct S_gs7 gs7 = {0, ((int)64368ULL), ((unsigned int)12ULL)};
unsigned int *gp8 = &g2;
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
signed char f9(unsigned long p10, _BitInt(48) p11) {
    _BitInt(48) l12 = ((_BitInt(48))256ULL);
    unsigned long l13 = ((unsigned long)2147483648ULL);
    unsigned char w15;
    for (unsigned char i14 = 0; i14 < 2; i14++) {
        (p11 *= 5);
        (--z3);
        z5 = ((l13 / ((int)2ULL)) / ((int)2ULL));
    }
    (++z5);
    switch ((((int)(g1)) & ((int)7ULL))) {
    case 1:
        gs7.m0 = (z4 *= 129);
        mix((ga6[((int)2ULL)] ? g1 : ((unsigned char)127ULL)));
    case 2:
        switch ((((int)(z3)) & ((int)7ULL))) {
        case 0:
            g2 = (((unsigned long)3ULL) <= ((unsigned long)0ULL));
            w15 = 0;
            do {
                ((*gp8) |= 5);
            } while (++w15 < 3);
        case 5:
            break;
        case 6:
            ;
        }
        break;
    default:
        ;
    }
    return z4;
}
signed char f16(unsigned int p17, _BitInt(48) p18, unsigned long p19, int *pp20) {
    static unsigned long long s23 = ((unsigned long long)0ULL);
    volatile int l21 = ((int)256ULL);
    unsigned _BitInt(60) l22 = ((unsigned _BitInt(60))65536ULL);
    unsigned char w24;
    z4 = (g2 <<= 2);
    l22 = f9((ga6[((int)0ULL)] - p17), (((unsigned char)166ULL) % ((int)4ULL)));
    w24 = 0;
    while (w24++ < 2) {
        for (unsigned char i25 = 0; i25 < 1; i25++) {
            return (p18 ^ (z3 ^= 129));
            switch ((((int)((z5 << 2))) & ((int)7ULL))) {
            case 0:
                break;
            case 1:
                break;
            case 4:
                break;
            default:
                break;
            }
            switch ((((int)((++(*gp8)))) & ((int)7ULL))) {
            case 2:
                ga6[((int)0ULL)] = ((*gp8) -= 1);
                break;
            case 5:
                break;
            }
            mix(((w24 >= (*gp8)) / ((int)4ULL)));
        }
        ((*gp8) >>= 1);
    }
    return l22;
}
unsigned char f26(unsigned char p27, long *pp28) {
    volatile unsigned int l29 = ((unsigned int)0ULL);
    signed char l30 = ((signed char)0ULL);
    _BitInt(48) l31 = ((_BitInt(48))255ULL);
    int l32 = ((int)32768ULL);
    unsigned char w33;
    unsigned char w34;
    w33 = 0;
    while (w33++ < 4) {
        if ((p27++)) {
            gs7.m0 = (g2 != p27);
            return (w33 - z4);
        } else {
            w34 = 0;
            while (w34++ < 4) {
                continue;
            }
        }
        if ((gs7.m1--)) {
            mix(l30);
            (--z5);
            switch ((((int)(l30)) & ((int)7ULL))) {
            case 3:
                g2 = (l30 *= 5);
            default:
                mix(w33);
            }
        }
    }
    return gs7.m1;
}
int main(void) {
    volatile unsigned int l35 = ((unsigned int)0ULL);
    int l36 = ((int)12275ULL);
    long l37 = ((long)2ULL);
    switch ((((int)((ga6[((int)1ULL)] ^ (z4 |= 1)))) & ((int)7ULL))) {
    case 1:
        break;
    case 4:
        f26((z5 + ((unsigned int)32767ULL)), &l37);
        break;
    case 6:
        break;
    }
    z4 = z3;
    mix((((int)(g2)) & (gs7.m1 += 255)));
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
