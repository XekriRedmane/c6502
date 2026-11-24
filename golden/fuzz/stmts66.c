// expect: 10788
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned long g1 = ((unsigned long)2147483648ULL);
int g2 = ((int)65535ULL);
unsigned int z3 [[c6502::zeropage]] = ((unsigned int)65535ULL);
volatile long long z4 [[c6502::zeropage]] = ((long long)3ULL);
signed char z5 [[c6502::zeropage]] = ((signed char)1ULL);
long ga6[4] = {((long)0ULL), ((long)2147483648ULL), ((long)2525957311ULL), ((long)65535ULL)};
struct S_gs7 { unsigned char pad; unsigned long m0; long m1; };
struct S_gs7 gs7 = {0, ((unsigned long)32767ULL), ((long)2ULL)};
int *gp8 = &g2;
unsigned int *gp9 = &z3;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(z3);
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
int f10(int p11) {
    volatile unsigned _BitInt(27) l12 = ((unsigned _BitInt(27))3ULL);
    volatile unsigned _BitInt(44) l13 = ((unsigned _BitInt(44))2ULL);
    long l14 = ((long)2009148653ULL);
    (l13++);
    (*gp9) = ((*gp8) & p11);
    if ((z3 == l14)) {
        mix((++g1));
        (--(*gp9));
    } else {
        switch ((((int)((ga6[((int)3ULL)] ^= 90))) & ((int)7ULL))) {
        case 1:
            (l14++);
            mix(((p11--) & gs7.m1));
            break;
        case 2:
            switch ((((int)((z4 + (gs7.m0 >>= 7)))) & ((int)7ULL))) {
            case 0:
                break;
            case 3:
                (g1 *= 3);
            case 6:
                break;
            }
            break;
        case 3:
            return (p11 & (l13 -= 255));
            mix((((*gp9) * ((signed char)2ULL)) ? (gs7.m1 - ((unsigned char)255ULL)) : (z4 > z3)));
            break;
        case 4:
            ;
        }
    }
    return l14;
}
int main(void) {
    unsigned char l15 = ((unsigned char)0ULL);
    signed char l16 = ((signed char)0ULL);
    unsigned char w17;
    f10((l16 <= z5));
    w17 = 0;
    do {
        z5 = ((~ga6[(w17 & ((int)3ULL))]) != g1);
        (z4++);
    } while (++w17 < 4);
    mix(((g2--) + ((unsigned char)(((unsigned int)128ULL)))));
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
