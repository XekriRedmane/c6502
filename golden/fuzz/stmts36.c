// expect: 8690
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned char g1 = ((unsigned char)255ULL);
signed char g2 = ((signed char)0ULL);
int g3 = ((int)2ULL);
int z4 [[c6502::zeropage]] = ((int)127ULL);
volatile long z5 [[c6502::zeropage]] = ((long)128ULL);
unsigned int ga6[4] = {((unsigned int)65535ULL), ((unsigned int)255ULL), ((unsigned int)2ULL), ((unsigned int)128ULL)};
struct S_gs7 { unsigned char pad; unsigned char m0; unsigned int m1; };
struct S_gs7 gs7 = {0, ((unsigned char)0ULL), ((unsigned int)11ULL)};
unsigned char *gp8 = &g1;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
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
signed char f9(signed char p10, unsigned long p11) {
    static int s15 = ((int)65535ULL);
    signed char l12 = ((signed char)127ULL);
    volatile unsigned long long l13 = ((unsigned long long)65536ULL);
    unsigned int l14 = ((unsigned int)255ULL);
    unsigned char w16;
    l13 = (++l12);
    w16 = 0;
    do {
        switch ((((int)(l12)) & ((int)7ULL))) {
        case 1:
            (*gp8) = (w16 ? l14 : w16);
            break;
        case 2:
            mix((((signed char)127ULL) + gs7.m1));
            break;
        case 3:
            break;
        default:
            (l14 -= 5);
        }
        continue;
        break;
        gs7.m0 = (p10 -= 90);
    } while (++w16 < 2);
    mix((++g1));
    switch ((((int)(((g3--) | g1))) & ((int)7ULL))) {
    case 0:
        mix((z5 ^= 1));
        break;
    case 3:
        break;
    case 7:
        break;
    }
    return gs7.m0;
}
unsigned int f17(unsigned char p18, unsigned char p19, long *pp20) {
    unsigned char l21 = ((unsigned char)255ULL);
    int l22 = ((int)256ULL);
    unsigned char w23;
    unsigned char w24;
    mix(((l22 >>= 1) - ((unsigned char)1ULL)));
    if (((--g2) & ((unsigned int)32768ULL))) {
        w23 = 0;
        while (w23++ < 4) {
            l21 = ((unsigned long long)(z5));
            if ((((*gp8) &= 90) ^ w23)) {
                return ((z4++) ^ (g1 + (*gp8)));
                p18 = (l21 >>= 6);
                break;
            }
            w24 = 0;
            while (w24++ < 2) {
                break;
                break;
                mix(f9((w24 + ((int)1ULL)), w23));
            }
        }
    }
    return (p19 - ga6[((int)3ULL)]);
}
int main(void) {
    signed char l25 = ((signed char)5ULL);
    volatile int l26 = ((int)1ULL);
    long l27 = ((long)255ULL);
    unsigned char w28;
    switch ((((int)((l27 - (z5 += 5)))) & ((int)7ULL))) {
    case 0:
        (z5 <<= 4);
        break;
        break;
    case 2:
        break;
    }
    w28 = 0;
    do {
        l27 = (ga6[(w28 & ((int)3ULL))] += 255);
        mix((l27 *= 5));
        mix(((w28 / ((int)2ULL)) ? (((unsigned int)65535ULL) - z5) : (l26 * (*gp8))));
        for (unsigned char i29 = 0; i29 < 4; i29++) {
            mix((((*gp8) != ((int)255ULL)) / ((int)4ULL)));
            g3 = (w28 + (z5 -= 3));
            switch ((((int)((w28 & (gs7.m1--)))) & ((int)7ULL))) {
            case 0:
                ;
            case 3:
                z4 = ((l27 / ((int)3ULL)) - l25);
            case 6:
                break;
            default:
                ;
            }
        }
    } while (++w28 < 3);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
