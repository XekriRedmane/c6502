// expect: 12050
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile unsigned char g1 = ((unsigned char)3ULL);
int g2 = ((int)65535ULL);
volatile int z3 [[c6502::zeropage]] = ((int)2ULL);
long long z4 [[c6502::zeropage]] = ((long long)0ULL);
unsigned int z5 [[c6502::zeropage]] = ((unsigned int)255ULL);
unsigned int ga6[4] = {((unsigned int)0ULL), ((unsigned int)255ULL), ((unsigned int)2ULL), ((unsigned int)1ULL)};
struct S_gs7 { unsigned char pad; unsigned char m0; unsigned long m1; };
struct S_gs7 gs7 = {0, ((unsigned char)255ULL), ((unsigned long)256ULL)};
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
unsigned long f8(signed char p9, unsigned char p10, unsigned char p11, unsigned long *pp12) {
    static int s15 = ((int)0ULL);
    int l13 = ((int)65535ULL);
    unsigned char l14 = ((unsigned char)3ULL);
    unsigned char w16;
    unsigned char w17;
    (ga6[((int)2ULL)] += 255);
    gs7.m0 = p10;
    switch ((((int)((g1 *= 255))) & ((int)7ULL))) {
    case 0:
        (p9--);
        break;
    case 1:
        break;
    case 5:
        w16 = 0;
        while (w16++ < 2) {
            w17 = 0;
            while (w17++ < 3) {
                mix((p11++));
                break;
                break;
                continue;
            }
            if ((p10 & (gs7.m0 &= 129))) {
                break;
            }
        }
    default:
        ;
    }
    return (z3 >> 0);
}
int main(void) {
    int l18 = ((int)127ULL);
    unsigned long l19 = ((unsigned long)128ULL);
    unsigned long l20 = ((unsigned long)65536ULL);
    z5 = (l20--);
    mix(((gs7.m0++) & (gs7.m1 - ((unsigned int)127ULL))));
    l19 = f8((l20 <= g2), (g1 >= gs7.m0), (gs7.m1 + gs7.m0), &l19);
    mix((ga6[((int)0ULL)] ^ l20));
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
