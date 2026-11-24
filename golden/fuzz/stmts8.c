// expect: 6379
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned char g1 = ((unsigned char)255ULL);
signed char g2 = ((signed char)0ULL);
volatile signed char g3 = ((signed char)128ULL);
signed char g4 = ((signed char)2ULL);
int z5 [[c6502::zeropage]] = ((int)65535ULL);
long long ga6[4] = {((long long)1ULL), ((long long)65535ULL), ((long long)127ULL), ((long long)256ULL)};
struct S_gs7 { unsigned char pad; unsigned int m0; int m1; };
struct S_gs7 gs7 = {0, ((unsigned int)0ULL), ((int)12ULL)};
unsigned char *gp8 = &g1;
signed char *gp9 = &g2;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(g4);
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
int f10(long p11, unsigned int p12) {
    int l13 = ((int)1ULL);
    unsigned char w14;
    unsigned char w15;
    mix(gs7.m0);
    w14 = 0;
    while (w14++ < 2) {
        mix((w14 <= g4));
        (g1 &= 3);
        mix(((unsigned int)65535ULL));
    }
    if ((p12 >>= 6)) {
        w15 = 0;
        while (w15++ < 4) {
            for (unsigned char i16 = 0; i16 < 3; i16++) {
                g3 = ((((unsigned char)255ULL) / ((int)3ULL)) <= (!l13));
                break;
                break;
                g4 = ((g3 &= 129) + ((unsigned char)(p12)));
            }
        }
    }
    return (((int)11ULL) / ((int)6ULL));
}
unsigned char f17(unsigned char *pp18) {
    unsigned long l19 = ((unsigned long)32767ULL);
    unsigned char l20 = ((unsigned char)0ULL);
    unsigned char w21;
    (--z5);
    if ((l19 + ((*pp18)++))) {
        mix(((g1++) - (*pp18)));
        switch ((((int)(((z5 -= 255) ^ g3))) & ((int)7ULL))) {
        case 1:
            ;
        default:
            (gs7.m0 -= 5);
        }
    }
    gs7.m0 = (((unsigned char)255ULL) ^ ((unsigned char)255ULL));
    if ((g2 &= 5)) {
        return g1;
        w21 = 0;
        do {
            (ga6[(w21 & ((int)3ULL))] |= 1);
            g4 = (((unsigned long)2ULL) & gs7.m0);
            return (gs7.m0 - (*gp9));
        } while (++w21 < 4);
    }
    return (gs7.m0 == ga6[((int)2ULL)]);
}
signed char f22(unsigned char p23, long p24, signed char *pp25) {
    static unsigned long s29 = ((unsigned long)32767ULL);
    unsigned int l26 = ((unsigned int)32767ULL);
    unsigned int l27 = ((unsigned int)0ULL);
    unsigned char l28 = ((unsigned char)217ULL);
    mix((((*gp8)++) & l27));
    if ((l28 *= 5)) {
        return (((*gp8) ^= 3) ^ p23);
        ((*gp9) -= 90);
        (*gp9) = (((int)255ULL) / ((int)7ULL));
    }
    gs7.m0 = (p24 & ((unsigned char)0ULL));
    return (l27 + g2);
}
int main(void) {
    unsigned char l30 = ((unsigned char)255ULL);
    signed char l31 = ((signed char)15ULL);
    f22(gs7.m0, ((int)3ULL), &l31);
    if ((((signed char)255ULL) ? l30 : g3)) {
        switch ((((int)(((g2++) | g4))) & ((int)7ULL))) {
        case 3:
            if ((((*gp8) != ((long)65536ULL)) + (g4 <<= 5))) {
                mix(f17(&l30));
            } else {
                break;
                (g1 &= 129);
            }
        case 4:
            g4 = ((*gp9)++);
            break;
        default:
            (z5 ^= 3);
            for (unsigned char i32 = 0; i32 < 4; i32++) {
                break;
            }
        }
    }
    f22((gs7.m1 + gs7.m1), ((int)255ULL), &l31);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
