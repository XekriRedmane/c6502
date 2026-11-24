// expect: 2421
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned char g1 = ((unsigned char)0ULL);
unsigned long g2 = ((unsigned long)255ULL);
signed char z3 [[c6502::zeropage]] = ((signed char)2ULL);
unsigned int ga4[4] = {((unsigned int)65535ULL), ((unsigned int)128ULL), ((unsigned int)0ULL), ((unsigned int)0ULL)};
struct S_gs5 { unsigned char pad; int m0; long m1; };
struct S_gs5 gs5 = {0, ((int)0ULL), ((long)3ULL)};
signed char *gp6 = &z3;
unsigned long *gp7 = &g2;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(z3);
    mix(ga4[((int)0ULL)]);
}
void fin1(void) {
    mix(ga4[((int)1ULL)]);
    mix(ga4[((int)2ULL)]);
    mix(ga4[((int)3ULL)]);
    mix(gs5.m0);
}
void fin2(void) {
    mix(gs5.m1);
}
unsigned int f8(signed char p9, unsigned char p10, long *pp11) {
    static volatile unsigned char s15 = ((unsigned char)2ULL);
    int l12 = ((int)35094ULL);
    long l13 = ((long)32768ULL);
    unsigned long l14 = ((unsigned long)128ULL);
    mix((g1 >>= 7));
    switch ((((int)((((signed char)0ULL) + (*gp7)))) & ((int)7ULL))) {
    case 4:
        break;
        ((*gp6) |= 255);
    case 5:
        (l12 &= 1);
        gs5.m1 = (++(*gp7));
        break;
    }
    if (((l14 += 1) - ((signed char)0ULL))) {
        s15 = ((g2 <<= 2) + ((int)45589ULL));
        switch ((((int)(((signed char)128ULL))) & ((int)7ULL))) {
        case 2:
            switch ((((int)((s15 += 5))) & ((int)7ULL))) {
            case 2:
                break;
            default:
                break;
            }
        case 3:
            break;
        case 5:
            break;
        }
    }
    return ((*gp6) << 7);
}
int main(void) {
    volatile unsigned char l16 = ((unsigned char)3ULL);
    long l17 = ((long)2147483648ULL);
    unsigned char w18;
    if (ga4[((int)2ULL)]) {
        if (((unsigned long)(gs5.m0))) {
            mix(f8((((int)0ULL) ? (*gp7) : (*gp7)), ((long)(z3)), &l17));
            w18 = 0;
            while (w18++ < 1) {
                l16 = f8((((unsigned long)128ULL) ? l17 : ((unsigned int)51572ULL)), (l17 != l16), &l17);
                break;
                break;
                break;
            }
        }
        z3 = (--ga4[((int)2ULL)]);
    } else {
        mix(f8(gs5.m1, (gs5.m0 ^ ((unsigned int)59776ULL)), &l17));
    }
    switch ((((int)(((gs5.m0 |= 5) | z3))) & ((int)7ULL))) {
    case 2:
        if (((*gp7) <<= 6)) {
        }
        break;
    case 6:
        break;
    default:
        ;
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
