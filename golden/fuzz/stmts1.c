// expect: 31300
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned int g1 = ((unsigned int)0ULL);
signed char g2 = ((signed char)0ULL);
signed char g3 = ((signed char)0ULL);
unsigned long long z4 [[c6502::zeropage]] = ((unsigned long long)256ULL);
long z5 [[c6502::zeropage]] = ((long)0ULL);
signed char z6 [[c6502::zeropage]] = ((signed char)0ULL);
unsigned int ga7[4] = {((unsigned int)13173ULL), ((unsigned int)255ULL), ((unsigned int)128ULL), ((unsigned int)3ULL)};
struct S_gs8 { unsigned char pad; unsigned char m0; unsigned char m1; };
struct S_gs8 gs8 = {0, ((unsigned char)0ULL), ((unsigned char)255ULL)};
long *gp9 = &z5;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(z4);
}
void fin1(void) {
    mix(z5);
    mix(z6);
    mix(ga7[((int)0ULL)]);
    mix(ga7[((int)1ULL)]);
}
void fin2(void) {
    mix(ga7[((int)2ULL)]);
    mix(ga7[((int)3ULL)]);
    mix(gs8.m0);
    mix(gs8.m1);
}
unsigned char f10(unsigned char *pp11) {
    static unsigned char s14 = ((unsigned char)4ULL);
    unsigned long long l12 = ((unsigned long long)5ULL);
    unsigned long l13 = ((unsigned long)32768ULL);
    unsigned char w16;
    if ((s14 | ((*pp11) += 1))) {
        return ((unsigned long)256ULL);
        switch ((((int)(((gs8.m1 -= 1) | g3))) & ((int)7ULL))) {
        case 0:
            break;
        case 2:
            for (unsigned char i15 = 0; i15 < 2; i15++) {
                break;
            }
            break;
        case 5:
            break;
        case 7:
            ;
        }
    } else {
        mix(z6);
        if (s14) {
            l12 = (((unsigned int)15ULL) ^ (++ga7[((int)0ULL)]));
            return (gs8.m1 >= z4);
        } else {
            (gs8.m1 *= 3);
        }
    }
    mix(((unsigned long)255ULL));
    w16 = 0;
    do {
    } while (++w16 < 3);
    return ((int)128ULL);
}
signed char f17(unsigned char p18, long p19, unsigned char p20, unsigned int *pp21) {
    static unsigned char s26 = ((unsigned char)2ULL);
    long l22 = ((long)127ULL);
    unsigned int l23 = ((unsigned int)127ULL);
    volatile unsigned int l24 = ((unsigned int)127ULL);
    unsigned char l25 = ((unsigned char)0ULL);
    unsigned char w27;
    unsigned char w28;
    l25 = f10(&l25);
    if ((l23 || ((signed char)0ULL))) {
        mix((z5 > l23));
        w27 = 0;
        while (w27++ < 4) {
            switch ((((int)((s26 % ((int)3ULL)))) & ((int)7ULL))) {
            case 0:
                break;
            default:
                l23 = (g3 + (gs8.m0 |= 255));
            }
        }
        (gs8.m1 >>= 7);
    } else {
        mix((++s26));
        mix(((g3--) - z4));
    }
    if (p20) {
        w28 = 0;
        do {
        } while (++w28 < 2);
    }
    return l24;
}
unsigned int f29(long p30, long p31, unsigned int *pp32) {
    static long s36 = ((long)3ULL);
    unsigned char l33 = ((unsigned char)255ULL);
    unsigned int l34 = ((unsigned int)32768ULL);
    unsigned int l35 = ((unsigned int)65535ULL);
    unsigned char w37;
    unsigned char w38;
    mix((gs8.m1 |= 129));
    g1 = ((*pp32) += 3);
    w37 = 0;
    do {
        (ga7[(w37 & ((int)3ULL))] &= 129);
        (l34 <<= 6);
        w38 = 0;
        do {
            l35 = f17((l33 != gs8.m1), w37, (w38 * p31), &l34);
        } while (++w38 < 4);
        if (l35) {
            f17((s36 < l34), ((long)(s36)), (((int)2ULL) & ((unsigned char)255ULL)), &l35);
            return ((unsigned long)3ULL);
            (++l33);
        }
    } while (++w37 < 1);
    (z5 |= 5);
    return l34;
}
int main(void) {
    volatile int l39 = ((int)9042ULL);
    unsigned char l40 = ((unsigned char)255ULL);
    unsigned int l41 = ((unsigned int)56488ULL);
    unsigned char w42;
    if (((unsigned long long)1ULL)) {
        mix((ga7[((int)1ULL)] += 1));
    } else {
        switch ((((int)((~((unsigned char)4ULL)))) & ((int)7ULL))) {
        case 4:
            g2 = (ga7[((int)2ULL)] >>= 5);
            w42 = 0;
            while (w42++ < 4) {
                break;
                (l39 *= 5);
                (l39 += 3);
                (gs8.m1 += 5);
            }
            break;
        case 6:
            (g1++);
            break;
        case 7:
            for (unsigned char i43 = 0; i43 < 3; i43++) {
                z4 = ((l39 += 129) | z5);
            }
        default:
            ;
        }
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
