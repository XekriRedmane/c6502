// expect: 18744
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile unsigned char g1 = ((unsigned char)2ULL);
long g2 = ((long)2147483648ULL);
unsigned int g3 = ((unsigned int)3ULL);
unsigned char z4 [[c6502::zeropage]] = ((unsigned char)127ULL);
volatile int z5 [[c6502::zeropage]] = ((int)11ULL);
volatile signed char z6 [[c6502::zeropage]] = ((signed char)0ULL);
int ga7[4] = {((int)49468ULL), ((int)32768ULL), ((int)65535ULL), ((int)11ULL)};
struct S_gs8 { unsigned char pad; unsigned int m0; unsigned char m1; };
struct S_gs8 gs8 = {0, ((unsigned int)2ULL), ((unsigned char)0ULL)};
long *gp9 = &g2;
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
unsigned int f10(unsigned int p11) {
    static unsigned char s13 = ((unsigned char)3ULL);
    unsigned char l12 = ((unsigned char)2ULL);
    unsigned char w15;
    mix(((unsigned char)0ULL));
    for (unsigned char i14 = 0; i14 < 3; i14++) {
        mix((((*gp9) % ((int)3ULL)) * (i14 ? gs8.m0 : z5)));
        switch ((((int)(((z5--) + i14))) & ((int)7ULL))) {
        case 2:
            if (((z6 >> 6) ^ (--ga7[(i14 & ((int)3ULL))]))) {
                continue;
                (s13 += 129);
                mix((((int)127ULL) - (l12 &= 1)));
            } else {
                break;
                break;
            }
            break;
        case 6:
            break;
        }
        w15 = 0;
        while (w15++ < 2) {
            (z4++);
        }
    }
    return (((unsigned long long)127ULL) + g2);
}
int main(void) {
    unsigned long l16 = ((unsigned long)2744336818ULL);
    unsigned char w17;
    switch ((((int)(((gs8.m0 + z6) || (ga7[((int)0ULL)] << 6)))) & ((int)7ULL))) {
    case 0:
        switch ((((int)(((long long)(ga7[((int)2ULL)])))) & ((int)7ULL))) {
        case 3:
            if ((g1 -= 90)) {
                break;
            } else {
                ga7[((int)2ULL)] = gs8.m1;
                break;
            }
        case 5:
            break;
        }
        mix((g1 << 6));
        break;
    case 2:
        break;
    case 3:
        if ((g3 & (l16 |= 255))) {
            w17 = 0;
            while (w17++ < 2) {
                (l16 |= 5);
            }
            g1 = g3;
            mix(gs8.m1);
        }
        break;
    default:
        ;
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
