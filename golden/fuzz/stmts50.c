// expect: 25350
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
signed char g1 = ((signed char)0ULL);
volatile long long g2 = ((long long)2147483648ULL);
int g3 = ((int)2ULL);
volatile unsigned long z4 [[c6502::zeropage]] = ((unsigned long)12ULL);
unsigned int z5 [[c6502::zeropage]] = ((unsigned int)32768ULL);
signed char ga6[4] = {((signed char)3ULL), ((signed char)255ULL), ((signed char)3ULL), ((signed char)118ULL)};
struct S_gs7 { unsigned char pad; int m0; long m1; };
struct S_gs7 gs7 = {0, ((int)13252ULL), ((long)2147483647ULL)};
int *gp8 = &g3;
unsigned int *gp9 = &z5;
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
unsigned int f10(long p11, int *pp12) {
    long l13 = ((long)1789786164ULL);
    unsigned int l14 = ((unsigned int)127ULL);
    signed char l15 = ((signed char)255ULL);
    switch ((((int)((gs7.m1 *= 90))) & ((int)7ULL))) {
    case 4:
        if (((g3 ^= 129) - (gs7.m0 % ((int)7ULL)))) {
            mix((l14 ^ (*gp8)));
            (*pp12) = (--l15);
            break;
        } else {
            if (l15) {
                (l15 |= 255);
                (g3 -= 5);
            } else {
                break;
            }
        }
        if ((g1 *= 3)) {
            mix((l14 &= 255));
            if ((gs7.m1 ? (g1 * ga6[((int)1ULL)]) : ((*gp9) & l13))) {
            }
        }
        break;
    case 6:
        break;
    }
    return (gs7.m1 % ((int)4ULL));
}
unsigned char f16(void) {
    unsigned int l17 = ((unsigned int)32767ULL);
    int l18 = ((int)0ULL);
    unsigned char w19;
    w19 = 0;
    while (w19++ < 1) {
        (g3 += 1);
        switch ((((int)((w19 > ((long long)(w19))))) & ((int)7ULL))) {
        case 3:
            break;
        case 4:
            mix(((++ga6[(w19 & ((int)3ULL))]) | (w19 * (*gp9))));
            break;
        case 5:
            break;
            break;
        }
        if (((ga6[(w19 & ((int)3ULL))] | w19) - (l17 & z5))) {
            return (((unsigned char)255ULL) & (--(*gp8)));
            (ga6[((int)0ULL)] ^= 1);
        }
    }
    switch ((((int)((l17 <= ((long)2147483647ULL)))) & ((int)7ULL))) {
    case 3:
        (*gp9) = (gs7.m0 >>= 7);
        break;
    case 5:
        mix((g2 == g3));
    case 7:
        mix(((--g1) | ((unsigned long)0ULL)));
    default:
        ;
    }
    return (z5 & (*gp9));
}
unsigned char f20(void) {
    static unsigned char s23 = ((unsigned char)0ULL);
    unsigned char l21 = ((unsigned char)255ULL);
    int l22 = ((int)0ULL);
    l22 = f16();
    switch ((((int)((g1 > ((unsigned long long)127ULL)))) & ((int)7ULL))) {
    case 2:
        ((*gp8) *= 255);
        return (((long)32768ULL) + (g3 &= 5));
    case 5:
        for (unsigned char i24 = 0; i24 < 3; i24++) {
            gs7.m1 = (i24 << 5);
            z4 = (l22 -= 129);
            z4 = i24;
            mix((z4 -= 5));
        }
    default:
        z4 = (gs7.m1 >= ((int)32767ULL));
    }
    mix(((l22 >> 1) + (ga6[((int)2ULL)] >>= 2)));
    g2 = (--gs7.m0);
    return ((unsigned int)(z4));
}
int main(void) {
    unsigned long long l25 = ((unsigned long long)32768ULL);
    int l26 = ((int)65535ULL);
    for (unsigned char i27 = 0; i27 < 4; i27++) {
        g3 = ((unsigned int)0ULL);
    }
    for (unsigned char i28 = 0; i28 < 1; i28++) {
        (++g2);
        if (((g1 *= 129) | (i28 || l25))) {
            switch ((((int)((((unsigned char)128ULL) & (g2 -= 3)))) & ((int)7ULL))) {
            case 6:
                mix(f16());
            default:
                break;
                break;
            }
        }
        (gs7.m0 -= 1);
    }
    if (((*gp8)--)) {
        l26 = f10((l25 & l25), &l26);
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
