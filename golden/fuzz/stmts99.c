// expect: 7181
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
int g1 = ((int)255ULL);
unsigned int g2 = ((unsigned int)255ULL);
volatile int g3 = ((int)127ULL);
volatile unsigned int g4 = ((unsigned int)49128ULL);
long z5 [[c6502::zeropage]] = ((long)1ULL);
unsigned char z6 [[c6502::zeropage]] = ((unsigned char)128ULL);
int z7 [[c6502::zeropage]] = ((int)0ULL);
unsigned long ga8[4] = {((unsigned long)32768ULL), ((unsigned long)256ULL), ((unsigned long)0ULL), ((unsigned long)32768ULL)};
struct S_gs9 { unsigned char pad; unsigned char m0; unsigned int m1; };
struct S_gs9 gs9 = {0, ((unsigned char)1ULL), ((unsigned int)26901ULL)};
unsigned char *gp10 = &z6;
long *gp11 = &z5;
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
int f12(unsigned int p13, int p14, unsigned int p15, long *pp16) {
    static unsigned char s19 = ((unsigned char)2ULL);
    long l17 = ((long)65536ULL);
    long l18 = ((long)0ULL);
    unsigned char w20;
    switch ((((int)((p15 += 1))) & ((int)7ULL))) {
    case 2:
        mix((l17 + g1));
        break;
    case 4:
        mix(((unsigned int)128ULL));
    case 6:
        break;
    default:
        mix((((unsigned long)7ULL) <= ((long)128ULL)));
        mix((g1 + (gs9.m1 += 1)));
    }
    if ((z6 += 90)) {
        switch ((((int)(((++(*gp11)) & ((signed char)255ULL)))) & ((int)7ULL))) {
        case 0:
            break;
            (ga8[((int)1ULL)] |= 5);
        case 2:
            mix(((*pp16) + ((int)65535ULL)));
            w20 = 0;
            do {
                (*gp10) = (g4 | (z5 ^= 129));
            } while (++w20 < 3);
            break;
        case 4:
            break;
        case 6:
            break;
        default:
            ;
        }
    }
    return ((unsigned char)255ULL);
}
long long f21(unsigned int p22) {
    static long long s27 = ((long long)127ULL);
    volatile unsigned char l23 = ((unsigned char)0ULL);
    unsigned int l24 = ((unsigned int)128ULL);
    long long l25 = ((long long)32767ULL);
    long l26 = ((long)4ULL);
    unsigned char w28;
    s27 = ((*gp11) & (z6 *= 90));
    mix((((signed char)(l26)) + (l23 >> 0)));
    w28 = 0;
    do {
        (g1 &= 3);
    } while (++w28 < 2);
    switch ((((int)((--ga8[((int)3ULL)]))) & ((int)7ULL))) {
    case 1:
        (z5 -= 255);
    case 5:
        break;
    case 7:
        if ((l24--)) {
            s27 = (p22 &= 255);
        } else {
            ((*gp11) &= 3);
        }
    default:
        s27 = ((unsigned char)((((long)256ULL) % ((int)3ULL))));
        break;
    }
    return ((*gp11) + z5);
}
int f29(int p30, int p31, signed char p32, int *pp33) {
    static unsigned char s36 = ((unsigned char)255ULL);
    int l34 = ((int)256ULL);
    long l35 = ((long)0ULL);
    unsigned char w37;
    unsigned char w38;
    mix(((signed char)((*gp11))));
    z6 = ((long)(l35));
    g3 = (s36 & (--(*gp10)));
    w37 = 0;
    do {
        w38 = 0;
        while (w38++ < 4) {
            switch ((((int)((p31 | gs9.m1))) & ((int)7ULL))) {
            case 1:
                break;
                return (s36 | ((*gp11) ^ ((long)3ULL)));
                break;
            case 6:
                break;
                break;
            }
            switch ((((int)((-(((unsigned int)128ULL) % ((int)4ULL))))) & ((int)7ULL))) {
            case 3:
                (z5 *= 5);
                break;
            case 4:
                break;
            default:
                ;
            }
        }
    } while (++w37 < 2);
    return ((unsigned char)(((int)65535ULL)));
}
int main(void) {
    signed char l39 = ((signed char)181ULL);
    long l40 = ((long)32767ULL);
    int l41 = ((int)1ULL);
    unsigned char w42;
    switch ((((int)((g4 &= 255))) & ((int)7ULL))) {
    case 3:
        switch ((((int)(((l41 <<= 7) ^ ((unsigned char)0ULL)))) & ((int)7ULL))) {
        case 3:
            gs9.m0 = (((int)256ULL) || ((signed char)0ULL));
            mix((-((unsigned int)256ULL)));
            break;
        case 4:
            l41 = (g3 - z7);
            mix((z5 += 3));
            break;
        case 6:
            ;
        case 7:
            break;
        default:
            (*gp10) = (gs9.m1 *= 129);
        }
        w42 = 0;
        while (w42++ < 4) {
            switch ((((int)((z7 &= 1))) & ((int)7ULL))) {
            case 0:
                break;
            }
            if ((w42 ^ (z7 ^= 129))) {
                mix((l41 | (--l39)));
            }
            (--gs9.m0);
        }
        break;
    case 4:
        break;
    case 6:
        ;
    default:
        ;
    }
    fin0();
    fin1();
    fin2();
    fin3();
    return (int)(acc & 0x7FFF);
}
