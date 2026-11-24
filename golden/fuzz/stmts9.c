// expect: 30972
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned char g1 = ((unsigned char)127ULL);
unsigned long g2 = ((unsigned long)128ULL);
int g3 = ((int)2ULL);
unsigned char ga4[4] = {((unsigned char)255ULL), ((unsigned char)255ULL), ((unsigned char)141ULL), ((unsigned char)0ULL)};
struct S_gs5 { unsigned char pad; long m0; long m1; };
struct S_gs5 gs5 = {0, ((long)65536ULL), ((long)602337308ULL)};
int *gp6 = &g3;
unsigned char *gp7 = &g1;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
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
int f8(int *pp9) {
    volatile unsigned char l10 = ((unsigned char)255ULL);
    volatile signed char l11 = ((signed char)1ULL);
    unsigned char l12 = ((unsigned char)255ULL);
    (ga4[((int)1ULL)] &= 255);
    if ((l11 >>= 4)) {
        switch ((((int)(gs5.m1)) & ((int)7ULL))) {
        case 0:
            break;
        case 2:
            (*gp6) = l10;
        case 4:
            mix((~gs5.m1));
            ((*gp7)++);
            break;
        default:
            return ((*pp9) & ga4[((int)3ULL)]);
        }
        return g2;
    } else {
        if ((l11 >>= 0)) {
            mix((g3 ^= 255));
            ((*gp7) &= 5);
        }
        switch ((((int)(((++g2) - l12))) & ((int)7ULL))) {
        case 3:
            ;
        case 5:
            break;
        default:
            ;
        }
    }
    return ((unsigned long)(l12));
}
int f13(unsigned int p14, int p15, signed char p16, unsigned int *pp17) {
    unsigned char l18 = ((unsigned char)0ULL);
    volatile unsigned int l19 = ((unsigned int)127ULL);
    int l20 = ((int)65535ULL);
    mix(((gs5.m0++) & gs5.m1));
    f8(&l20);
    (l19 -= 129);
    return gs5.m0;
}
signed char f21(signed char *pp22) {
    unsigned int l23 = ((unsigned int)128ULL);
    unsigned int l24 = ((unsigned int)128ULL);
    int l25 = ((int)65535ULL);
    for (unsigned char i26 = 0; i26 < 1; i26++) {
        switch ((((int)((i26 << 7))) & ((int)7ULL))) {
        case 4:
            mix(((*gp7) << 4));
            break;
        case 7:
            l24 = f8(&l25);
            break;
        default:
            if (((i26 / ((int)2ULL)) ^ (*gp6))) {
                gs5.m0 = ((*gp7)++);
                break;
            }
        }
        break;
    }
    if (gs5.m0) {
        switch ((((int)((((unsigned char)1ULL) ? (*gp6) : g1))) & ((int)7ULL))) {
        case 4:
            ;
        case 5:
            (gs5.m0 += 129);
        case 6:
            return ((l24 >>= 3) - g3);
            break;
        case 7:
            ;
        default:
            ;
        }
    }
    return ((long)(((unsigned long long)255ULL)));
}
int main(void) {
    signed char l27 = ((signed char)127ULL);
    int l28 = ((int)0ULL);
    unsigned int l29 = ((unsigned int)65535ULL);
    unsigned char w30;
    (l28 >>= 7);
    l29 = (((*gp7) ? ((long long)65536ULL) : g1) / ((int)6ULL));
    l29 = f21(&l27);
    (*gp7) = (l29 | g2);
    switch ((((int)(((g3 | l27) <= l27))) & ((int)7ULL))) {
    case 0:
        if (((gs5.m1--) ^ (((unsigned int)256ULL) - ga4[((int)1ULL)]))) {
            w30 = 0;
            while (w30++ < 4) {
                g2 = (++(*gp6));
                (ga4[(w30 & ((int)3ULL))] += 129);
                break;
            }
            mix(f13(((*gp6) * g1), ga4[((int)3ULL)], (((long)127ULL) & gs5.m0), &l29));
            switch ((((int)((((unsigned char)255ULL) + (ga4[((int)1ULL)] <<= 3)))) & ((int)7ULL))) {
            case 0:
                ;
            default:
                ;
            }
        }
        break;
    case 1:
        ;
    case 4:
        break;
    case 6:
        break;
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
