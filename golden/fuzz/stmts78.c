// expect: 17038
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned char g1 = ((unsigned char)110ULL);
volatile int g2 = ((int)2ULL);
signed char g3 = ((signed char)127ULL);
volatile unsigned long z4 [[c6502::zeropage]] = ((unsigned long)65536ULL);
unsigned int ga5[4] = {((unsigned int)2ULL), ((unsigned int)255ULL), ((unsigned int)0ULL), ((unsigned int)32767ULL)};
struct S_gs6 { unsigned char pad; int m0; unsigned int m1; };
struct S_gs6 gs6 = {0, ((int)128ULL), ((unsigned int)128ULL)};
signed char *gp7 = &g3;
unsigned char *gp8 = &g1;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(z4);
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
unsigned int f9(int p10) {
    unsigned int l11 = ((unsigned int)32767ULL);
    (gs6.m0 += 129);
    g3 = ((*gp8) | (gs6.m1 |= 129));
    switch ((((int)((p10 / ((int)6ULL)))) & ((int)7ULL))) {
    case 3:
        p10 = g1;
        mix((ga5[((int)0ULL)] ? ((unsigned long)1ULL) : ((unsigned int)3ULL)));
    case 4:
        break;
    case 5:
        for (unsigned char i12 = 0; i12 < 1; i12++) {
            mix((i12 ? (gs6.m0 % ((int)2ULL)) : ((*gp7) >> 1)));
            switch ((((int)((ga5[(i12 & ((int)3ULL))] - gs6.m0))) & ((int)7ULL))) {
            case 0:
                break;
            default:
                break;
            }
            if ((i12 % ((int)5ULL))) {
                g2 = (g3 & g1);
                break;
            }
        }
        break;
    default:
        ;
    }
    return (((unsigned long long)127ULL) - g2);
}
int main(void) {
    volatile unsigned int l13 = ((unsigned int)0ULL);
    volatile signed char l14 = ((signed char)0ULL);
    (g3++);
    f9((*gp8));
    for (unsigned char i15 = 0; i15 < 4; i15++) {
        (l14 -= 255);
        ((*gp8) >>= 2);
        break;
    }
    switch ((((int)(((*gp8) += 1))) & ((int)7ULL))) {
    case 0:
        l13 = f9((((signed char)127ULL) % ((int)5ULL)));
        gs6.m1 = ((g1 ^ ((unsigned long)2ULL)) * gs6.m1);
        break;
    case 3:
        ((*gp7)--);
        break;
    case 4:
        switch ((((int)(((*gp8) -= 255))) & ((int)7ULL))) {
        case 1:
            ga5[((int)3ULL)] = (l13 | (++(*gp8)));
        case 5:
            break;
        case 6:
            ;
        case 7:
            break;
        default:
            ;
        }
    default:
        ;
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
