// expect: 3853
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
signed char g1 = ((signed char)0ULL);
signed char g2 = ((signed char)255ULL);
volatile unsigned char z3 [[c6502::zeropage]] = ((unsigned char)255ULL);
unsigned long z4 [[c6502::zeropage]] = ((unsigned long)32767ULL);
long z5 [[c6502::zeropage]] = ((long)3456152823ULL);
signed char ga6[4] = {((signed char)0ULL), ((signed char)0ULL), ((signed char)1ULL), ((signed char)0ULL)};
struct S_gs7 { unsigned char pad; unsigned long long m0; signed char m1; };
struct S_gs7 gs7 = {0, ((unsigned long long)65536ULL), ((signed char)1ULL)};
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
unsigned int f8(void) {
    static volatile int s12 = ((int)2ULL);
    unsigned char l9 = ((unsigned char)0ULL);
    signed char l10 = ((signed char)255ULL);
    unsigned int l11 = ((unsigned int)0ULL);
    unsigned char w13;
    unsigned char w14;
    gs7.m0 = (z5++);
    w13 = 0;
    do {
        w14 = 0;
        do {
            if ((z4 % ((int)3ULL))) {
                z3 = (w14 == (z4 + ga6[((int)2ULL)]));
                mix(((l9 && z3) << 0));
                break;
            }
        } while (++w14 < 3);
        break;
        ga6[((int)0ULL)] = (z4--);
        break;
    } while (++w13 < 3);
    l9 = (gs7.m0 >> 6);
    mix(z5);
    return z3;
}
unsigned char f15(unsigned long long *pp16) {
    static unsigned int s19 = ((unsigned int)32768ULL);
    volatile unsigned int l17 = ((unsigned int)256ULL);
    signed char l18 = ((signed char)2ULL);
    unsigned char w21;
    switch ((((int)((++gs7.m1))) & ((int)7ULL))) {
    case 1:
        for (unsigned char i20 = 0; i20 < 4; i20++) {
            if (((*pp16) += 90)) {
                break;
                gs7.m0 = (l17 ? z5 : (((long)127ULL) && l17));
                return z5;
            } else {
                s19 = ((gs7.m1--) | (i20 * i20));
            }
            switch ((((int)(((*pp16) ^= 5))) & ((int)7ULL))) {
            case 3:
                break;
            case 6:
                gs7.m1 = z3;
                break;
            }
        }
        switch ((((int)((l18 && g2))) & ((int)7ULL))) {
        case 2:
            (ga6[((int)3ULL)] >>= 5);
        case 6:
            break;
        case 7:
            w21 = 0;
            while (w21++ < 2) {
            }
            break;
        default:
            ;
        }
        break;
    case 3:
        break;
    }
    return (g1 ? g2 : g1);
}
int main(void) {
    signed char l22 = ((signed char)0ULL);
    unsigned long long l23 = ((unsigned long long)2147483648ULL);
    unsigned char w24;
    unsigned char w25;
    (gs7.m0 *= 5);
    w24 = 0;
    while (w24++ < 3) {
        if (((ga6[(w24 & ((int)3ULL))] -= 3) - (l23 - w24))) {
            f8();
            g1 = (w24 + (++ga6[(w24 & ((int)3ULL))]));
        } else {
            break;
            (--gs7.m1);
        }
        z4 = (w24 | w24);
        w25 = 0;
        do {
            switch ((((int)((l22 -= 5))) & ((int)7ULL))) {
            case 2:
                ;
            default:
                break;
                (z5++);
            }
        } while (++w25 < 2);
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
