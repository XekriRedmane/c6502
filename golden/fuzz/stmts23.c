// expect: 9056
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned char g1 = ((unsigned char)4ULL);
signed char g2 = ((signed char)0ULL);
unsigned int g3 = ((unsigned int)13ULL);
unsigned int g4 = ((unsigned int)255ULL);
unsigned char z5 [[c6502::zeropage]] = ((unsigned char)3ULL);
long z6 [[c6502::zeropage]] = ((long)255ULL);
unsigned long long ga7[4] = {((unsigned long long)256ULL), ((unsigned long long)127ULL), ((unsigned long long)3ULL), ((unsigned long long)256ULL)};
struct S_gs8 { unsigned char pad; signed char m0; unsigned int m1; };
struct S_gs8 gs8 = {0, ((signed char)2ULL), ((unsigned int)0ULL)};
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(g4);
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
long f9(unsigned int p10, unsigned long long p11, signed char *pp12) {
    signed char l13 = ((signed char)0ULL);
    unsigned int l14 = ((unsigned int)2ULL);
    unsigned char l15 = ((unsigned char)255ULL);
    (z5 &= 1);
    if (((unsigned int)(((unsigned int)32767ULL)))) {
        mix((ga7[((int)0ULL)] ^= 3));
        mix((l14++));
        return (((unsigned char)5ULL) & ((int)0ULL));
    }
    gs8.m1 = (z6 ^= 255);
    return p11;
}
signed char f16(unsigned char p17, unsigned char p18, unsigned int *pp19) {
    static unsigned int s22 = ((unsigned int)127ULL);
    unsigned int l20 = ((unsigned int)32768ULL);
    signed char l21 = ((signed char)0ULL);
    unsigned char w23;
    p18 = ((signed char)(((long long)3ULL)));
    ga7[((int)1ULL)] = (((unsigned char)0ULL) - g1);
    l20 = f9((((unsigned long)128ULL) + z5), ((signed char)(((unsigned int)65535ULL))), &l21);
    z6 = (l21 + ((unsigned int)32767ULL));
    ga7[((int)0ULL)] = (((signed char)(g1)) ^ gs8.m0);
    switch ((((int)((l21 += 129))) & ((int)7ULL))) {
    case 3:
        break;
    case 5:
        ;
    case 6:
        ;
    default:
        if ((((signed char)(g1)) | (*pp19))) {
            w23 = 0;
            do {
                f9((((signed char)127ULL) + g2), g1, &l21);
                (g3++);
                break;
            } while (++w23 < 1);
        }
    }
    return (((unsigned long long)255ULL) & p17);
}
int main(void) {
    int l24 = ((int)0ULL);
    signed char l25 = ((signed char)14ULL);
    unsigned int l26 = ((unsigned int)128ULL);
    (z6 >>= 5);
    (l24 ^= 255);
    (g3 += 3);
    if ((g4 | (g1 += 3))) {
        if ((ga7[((int)1ULL)] * ga7[((int)2ULL)])) {
            (l24 -= 3);
            g3 = (z5 <<= 7);
        }
        ga7[((int)3ULL)] = (gs8.m0 >= g3);
        if (((l26 <<= 2) + g3)) {
            l26 = f9((gs8.m1 ? g2 : gs8.m0), (l26 && ((unsigned char)127ULL)), &l25);
            mix((--ga7[((int)2ULL)]));
        }
    }
    mix(f9((((unsigned long long)65535ULL) & l24), (g4 >= l24), &l25));
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
