// expect: 7717
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned char g1 = ((unsigned char)255ULL);
unsigned char g2 = ((unsigned char)255ULL);
int z3 [[c6502::zeropage]] = ((int)17561ULL);
unsigned long long ga4[4] = {((unsigned long long)2ULL), ((unsigned long long)0ULL), ((unsigned long long)1ULL), ((unsigned long long)255ULL)};
struct S_gs5 { unsigned char pad; unsigned int m0; unsigned int m1; };
struct S_gs5 gs5 = {0, ((unsigned int)65535ULL), ((unsigned int)128ULL)};
unsigned char *gp6 = &g1;
unsigned char *gp7 = &g2;
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
signed char f8(unsigned char *pp9) {
    static volatile unsigned char s11 = ((unsigned char)1ULL);
    unsigned long l10 = ((unsigned long)32767ULL);
    unsigned char w12;
    unsigned char w13;
    if (((unsigned char)0ULL)) {
        mix((((int)32767ULL) << 0));
        w12 = 0;
        while (w12++ < 1) {
            g2 = (g2 && g2);
            (gs5.m0--);
            return ((*gp6)--);
            mix(((l10 + ((unsigned char)0ULL)) < (ga4[(w12 & ((int)3ULL))] - w12)));
        }
    } else {
        mix(((*pp9) >>= 0));
    }
    if ((gs5.m0++)) {
        return ((++g1) + (*gp7));
        w13 = 0;
        while (w13++ < 4) {
            for (unsigned char i14 = 0; i14 < 4; i14++) {
            }
        }
    }
    return (*gp7);
}
unsigned int f15(int p16, unsigned int p17, unsigned char p18) {
    static long s21 = ((long)2147483648ULL);
    signed char l19 = ((signed char)127ULL);
    unsigned char l20 = ((unsigned char)127ULL);
    unsigned char w22;
    w22 = 0;
    do {
        ((*gp6)++);
        (gs5.m1 -= 90);
        mix(f8(&l20));
    } while (++w22 < 1);
    l20 = (ga4[((int)3ULL)]++);
    if ((s21 - (l20 ^= 129))) {
        return (((signed char)255ULL) >> 5);
        return ((p16 ^= 255) & ((long)2147483647ULL));
        mix(((((int)2ULL) || s21) + (((unsigned long)2147483648ULL) & p17)));
    } else {
        (p17 *= 90);
        (l20 <<= 2);
    }
    return (((unsigned int)128ULL) & ga4[((int)1ULL)]);
}
unsigned char f23(int p24, unsigned int p25) {
    unsigned char l26 = ((unsigned char)0ULL);
    g1 = (((signed char)14ULL) & ga4[((int)0ULL)]);
    switch ((((int)((++g2))) & ((int)7ULL))) {
    case 0:
        break;
    case 4:
        break;
    case 5:
        mix(((*gp7)--));
        (*gp6) = ((ga4[((int)0ULL)] -= 255) ^ l26);
    case 6:
        (*gp7) = (l26 ^= 3);
        break;
    default:
        p24 = (((unsigned char)128ULL) | (z3 |= 255));
        return (((unsigned char)(p24)) < (p24 % ((int)3ULL)));
    }
    ga4[((int)2ULL)] = (--(*gp6));
    l26 = f15((p24 > gs5.m0), z3, (z3 && g1));
    (++p24);
    return gs5.m0;
}
int main(void) {
    unsigned int l27 = ((unsigned int)32767ULL);
    unsigned char l28 = ((unsigned char)255ULL);
    unsigned char w29;
    unsigned char w30;
    gs5.m0 = (gs5.m1 << 0);
    (ga4[((int)0ULL)] -= 255);
    w29 = 0;
    while (w29++ < 2) {
        w30 = 0;
        while (w30++ < 4) {
            switch ((((int)(((*gp6) &= 90))) & ((int)7ULL))) {
            case 2:
                break;
                break;
                break;
            case 3:
                (l27 *= 5);
                break;
                break;
            case 5:
                (*gp7) = (((unsigned long)2147483648ULL) & w29);
                break;
            case 6:
                ;
            default:
                l27 = (((unsigned char)255ULL) ^ z3);
            }
        }
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
