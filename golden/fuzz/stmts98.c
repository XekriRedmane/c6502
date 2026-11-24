// expect: 17988
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile unsigned _BitInt(56) g1 = ((unsigned _BitInt(56))2ULL);
int g2 = ((int)35085ULL);
long g3 = ((long)0ULL);
unsigned int z4 [[c6502::zeropage]] = ((unsigned int)2ULL);
unsigned _BitInt(56) z5 [[c6502::zeropage]] = ((unsigned _BitInt(56))3ULL);
unsigned _BitInt(56) ga6[4] = {((unsigned _BitInt(56))0ULL), ((unsigned _BitInt(56))256ULL), ((unsigned _BitInt(56))32768ULL), ((unsigned _BitInt(56))2147483647ULL)};
struct S_gs7 { unsigned char pad; unsigned int m0; long m1; };
struct S_gs7 gs7 = {0, ((unsigned int)3ULL), ((long)32768ULL)};
long *gp8 = &g3;
int *gp9 = &g2;
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
signed char f10(unsigned char p11, unsigned int *pp12) {
    static _BitInt(44) s15 = ((_BitInt(44))0ULL);
    int l13 = ((int)10ULL);
    unsigned char l14 = ((unsigned char)128ULL);
    z5 = ((g3 ^= 5) & g2);
    if ((z5 >= ((signed char)0ULL))) {
        (*pp12) = (g2 |= 5);
    } else {
        switch ((((int)(((long)(z5)))) & ((int)7ULL))) {
        case 1:
            ;
        case 2:
            g1 = g1;
            break;
        case 3:
            (g2 &= 3);
            if ((((int)255ULL) | p11)) {
                break;
            }
            break;
        case 4:
            l14 = ((*gp8) + ((unsigned int)128ULL));
            if ((g2 - (*gp9))) {
                mix(g1);
                g3 = (l13 + ((unsigned long)65535ULL));
            }
            break;
        }
    }
    return (g1 - ((unsigned _BitInt(56))32768ULL));
}
unsigned char f16(unsigned long p17, _BitInt(44) *pp18) {
    static unsigned char s22 = ((unsigned char)0ULL);
    unsigned int l19 = ((unsigned int)256ULL);
    unsigned char l20 = ((unsigned char)255ULL);
    volatile unsigned char l21 = ((unsigned char)0ULL);
    unsigned char w23;
    switch ((((int)((gs7.m1 &= 5))) & ((int)7ULL))) {
    case 2:
        (z4 -= 129);
        break;
    case 4:
        (*gp9) = g1;
        break;
    case 7:
        w23 = 0;
        do {
            l21 = f10((z5 || g2), &l19);
        } while (++w23 < 3);
        mix((gs7.m0 += 129));
        break;
    default:
        (++p17);
    }
    l19 = f10(((*gp8) + z4), &l19);
    g2 = (((unsigned int)2ULL) ? z5 : ((int)9791ULL));
    if ((z5 |= 255)) {
        return ((++gs7.m0) - gs7.m1);
        mix((((*gp8) *= 1) & l19));
    }
    return ((*gp9) & ((unsigned int)3ULL));
}
int main(void) {
    unsigned int l24 = ((unsigned int)3ULL);
    _BitInt(44) l25 = ((_BitInt(44))32767ULL);
    unsigned char w27;
    for (unsigned char i26 = 0; i26 < 1; i26++) {
        if (((l25 + ((int)12ULL)) | (--gs7.m0))) {
            mix(f16(((unsigned _BitInt(56))0ULL), &l25));
        }
    }
    z4 = ((z4 + ((unsigned char)255ULL)) + (g3++));
    w27 = 0;
    do {
        (l25 |= 5);
        mix((g2 * (*gp8)));
    } while (++w27 < 4);
    ((*gp8) += 3);
    l25 = f10(((unsigned char)(gs7.m0)), &l24);
    z5 = ((g2 >>= 6) ^ l24);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
