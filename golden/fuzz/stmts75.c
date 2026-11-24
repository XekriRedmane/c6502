// expect: 31841
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
long g1 = ((long)0ULL);
long g2 = ((long)1ULL);
volatile unsigned int g3 = ((unsigned int)0ULL);
unsigned int g4 = ((unsigned int)32768ULL);
signed char z5 [[c6502::zeropage]] = ((signed char)0ULL);
volatile long long z6 [[c6502::zeropage]] = ((long long)128ULL);
long z7 [[c6502::zeropage]] = ((long)256ULL);
signed char ga8[4] = {((signed char)1ULL), ((signed char)0ULL), ((signed char)0ULL), ((signed char)0ULL)};
struct S_gs9 { unsigned char pad; long m0; signed char m1; };
struct S_gs9 gs9 = {0, ((long)32768ULL), ((signed char)0ULL)};
long *gp10 = &g1;
long *gp11 = &g2;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(g4);
}
void fin1(void) {
    mix(z5);
    mix(z6);
    mix(z7);
    mix(ga8[((int)0ULL)]);
}
void fin2(void) {
    mix(ga8[((int)1ULL)]);
    mix(ga8[((int)2ULL)]);
    mix(ga8[((int)3ULL)]);
    mix(gs9.m0);
}
void fin3(void) {
    mix(gs9.m1);
}
unsigned long f12(void) {
    static signed char s16 = ((signed char)14ULL);
    volatile int l13 = ((int)127ULL);
    unsigned long l14 = ((unsigned long)256ULL);
    volatile unsigned int l15 = ((unsigned int)256ULL);
    unsigned char w17;
    unsigned char w18;
    mix(((l15--) ^ ((unsigned char)255ULL)));
    mix((z6 >>= 2));
    w17 = 0;
    do {
        (z6 -= 3);
    } while (++w17 < 1);
    mix(((ga8[((int)3ULL)] + (*gp11)) ^ (++z5)));
    w18 = 0;
    do {
        mix((l15 && (*gp10)));
        mix((gs9.m1 >>= 3));
        mix((gs9.m1 - g1));
    } while (++w18 < 3);
    return (z5 < ((long long)256ULL));
}
int f19(unsigned char p20, int p21, unsigned int *pp22) {
    static int s26 = ((int)2ULL);
    unsigned int l23 = ((unsigned int)128ULL);
    unsigned int l24 = ((unsigned int)128ULL);
    volatile signed char l25 = ((signed char)255ULL);
    unsigned char w27;
    unsigned char w28;
    unsigned char w29;
    switch ((((int)((l23 | (*gp11)))) & ((int)7ULL))) {
    case 0:
        mix((-(((unsigned long long)3ULL) * ((unsigned long)256ULL))));
        w27 = 0;
        while (w27++ < 1) {
            w28 = 0;
            do {
                (z7 >>= 3);
                break;
                break;
            } while (++w28 < 3);
            w29 = 0;
            do {
                mix(f12());
                break;
                l25 = f12();
            } while (++w29 < 3);
            mix(l25);
        }
        break;
    case 2:
        break;
    case 5:
        break;
    case 7:
        ;
    default:
        ;
    }
    return (gs9.m0 >= g1);
}
int main(void) {
    signed char l30 = ((signed char)0ULL);
    unsigned long long l31 = ((unsigned long long)2147483648ULL);
    unsigned int l32 = ((unsigned int)127ULL);
    unsigned char w33;
    unsigned char w34;
    w33 = 0;
    do {
        mix((z6 ? g4 : (((signed char)0ULL) - w33)));
        if ((((long long)2147483647ULL) + (z6 <<= 6))) {
            w34 = 0;
            while (w34++ < 1) {
                break;
                mix((++z7));
                break;
            }
            (gs9.m1 *= 3);
            continue;
        }
        (l30--);
    } while (++w33 < 2);
    g1 = ((z7 += 5) & (l31 / ((int)3ULL)));
    if (l30) {
    }
    fin0();
    fin1();
    fin2();
    fin3();
    return (int)(acc & 0x7FFF);
}
