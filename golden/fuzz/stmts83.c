// expect: 8846
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned char g1 = ((unsigned char)2ULL);
unsigned int g2 = ((unsigned int)128ULL);
unsigned _BitInt(7) g3 = ((unsigned _BitInt(7))127ULL);
unsigned int g4 = ((unsigned int)127ULL);
signed char ga5[4] = {((signed char)0ULL), ((signed char)3ULL), ((signed char)0ULL), ((signed char)255ULL)};
struct S_gs6 { unsigned char pad; signed char m0; unsigned int m1; };
struct S_gs6 gs6 = {0, ((signed char)127ULL), ((unsigned int)32768ULL)};
unsigned _BitInt(7) *gp7 = &g3;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(g4);
}
void fin1(void) {
    mix(ga5[((int)0ULL)]);
    mix(ga5[((int)1ULL)]);
    mix(ga5[((int)2ULL)]);
    mix(ga5[((int)3ULL)]);
}
void fin2(void) {
    mix(gs6.m0);
    mix(gs6.m1);
}
int f8(unsigned long long p9, signed char p10, signed char *pp11) {
    static volatile unsigned int s13 = ((unsigned int)256ULL);
    int l12 = ((int)65535ULL);
    if ((((int)2ULL) ^ g4)) {
        ga5[((int)3ULL)] = (p10 == ((int)31622ULL));
    }
    mix((l12 >>= 1));
    switch ((((int)((g3++))) & ((int)7ULL))) {
    case 3:
        ;
    case 4:
        mix((l12 - ((*pp11) &= 255)));
        if ((-g1)) {
            return (g1 << 3);
            (p9 <<= 3);
        } else {
            (--(*gp7));
        }
        break;
    case 5:
        break;
    case 7:
        p9 = (g3 ^ ((*pp11) += 5));
        break;
    default:
        (*pp11) = (((unsigned _BitInt(7))127ULL) ^ (l12 <<= 5));
    }
    switch ((((int)((--g2))) & ((int)7ULL))) {
    case 3:
        break;
    case 4:
        ;
    case 5:
        break;
    case 6:
        break;
    default:
        ;
    }
    return (p9 | ((unsigned char)2ULL));
}
int main(void) {
    _BitInt(57) l14 = ((_BitInt(57))0ULL);
    volatile unsigned int l15 = ((unsigned int)128ULL);
    signed char l16 = ((signed char)0ULL);
    (g1--);
    switch ((((int)((((signed char)3ULL) * gs6.m1))) & ((int)7ULL))) {
    case 0:
        break;
    default:
        if ((-((unsigned int)32768ULL))) {
            break;
            f8((l16 ? l14 : (*gp7)), l16, &l16);
            (gs6.m0 ^= 5);
        } else {
            gs6.m0 = ((l16--) | g3);
            for (unsigned char i17 = 0; i17 < 1; i17++) {
                continue;
                ga5[((int)0ULL)] = ((l15 % ((int)2ULL)) - (g1 -= 5));
            }
        }
    }
    if (((++gs6.m0) ^ gs6.m1)) {
        (--l14);
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
