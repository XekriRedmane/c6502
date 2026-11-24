// expect: 897
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
int g1 = ((int)32767ULL);
signed char g2 = ((signed char)2ULL);
unsigned long g3 = ((unsigned long)2147483647ULL);
volatile signed char g4 = ((signed char)255ULL);
unsigned long z5 [[c6502::zeropage]] = ((unsigned long)12ULL);
unsigned _BitInt(26) ga6[4] = {((unsigned _BitInt(26))2ULL), ((unsigned _BitInt(26))0ULL), ((unsigned _BitInt(26))32767ULL), ((unsigned _BitInt(26))0ULL)};
struct S_gs7 { unsigned char pad; unsigned long m0; unsigned char m1; };
struct S_gs7 gs7 = {0, ((unsigned long)1484073689ULL), ((unsigned char)0ULL)};
unsigned long *gp8 = &z5;
unsigned long *gp9 = &g3;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(g4);
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
_BitInt(16) f10(void) {
    int l11 = ((int)65535ULL);
    unsigned char l12 = ((unsigned char)2ULL);
    unsigned long l13 = ((unsigned long)256ULL);
    if ((((unsigned long)(l13)) ^ ((*gp9)--))) {
        ((*gp9)--);
        gs7.m0 = z5;
        g4 = ((*gp9) |= 5);
    } else {
        (gs7.m1 *= 1);
        (--gs7.m0);
    }
    if (((ga6[((int)1ULL)] >>= 2) | ((unsigned int)1ULL))) {
        l11 = (((unsigned int)0ULL) % ((int)6ULL));
        mix((z5 + (++g3)));
        mix((l11 / ((int)5ULL)));
    }
    mix(((*gp8) <<= 5));
    g1 = ((*gp8) ^= 90);
    return l11;
}
_BitInt(16) f14(_BitInt(16) p15) {
    static volatile unsigned long s18 = ((unsigned long)672201737ULL);
    unsigned char l16 = ((unsigned char)255ULL);
    unsigned int l17 = ((unsigned int)32767ULL);
    gs7.m1 = ((g2 ? l16 : ((unsigned char)2ULL)) / ((int)2ULL));
    switch ((((int)(((unsigned char)3ULL))) & ((int)7ULL))) {
    case 0:
        switch ((((int)(z5)) & ((int)7ULL))) {
        case 3:
            (*gp9) = (gs7.m0 -= 3);
            break;
        case 4:
            for (unsigned char i19 = 0; i19 < 3; i19++) {
                break;
                mix((g4 & p15));
                l17 = g3;
                return (ga6[(i19 & ((int)3ULL))] / ((int)2ULL));
            }
            f10();
            break;
        case 5:
            return gs7.m1;
            break;
        case 6:
            ;
        default:
            l17 = ((g2 ^ z5) << 0);
        }
        break;
    case 2:
        break;
    case 5:
        ;
    default:
        ;
    }
    return ((signed char)(g3));
}
int main(void) {
    _BitInt(16) l20 = ((_BitInt(16))3ULL);
    signed char l21 = ((signed char)127ULL);
    volatile signed char l22 = ((signed char)0ULL);
    mix(f14((!g4)));
    f10();
    switch ((((int)((gs7.m0 *= 255))) & ((int)7ULL))) {
    case 1:
        break;
    case 3:
        ;
    case 4:
        if (((((unsigned long)65536ULL) * l21) ^ (g3 & gs7.m0))) {
            g4 = (ga6[((int)0ULL)] & ga6[((int)0ULL)]);
            if (g3) {
                l20 = f10();
            } else {
                (++l20);
                gs7.m0 = l22;
            }
            if (((--g1) + ((unsigned char)127ULL))) {
                l22 = f10();
            }
        } else {
            g1 = g1;
        }
    case 7:
        break;
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
