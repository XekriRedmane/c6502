// expect: 6497
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
unsigned int g1 = ((unsigned int)256ULL);
signed char g2 = ((signed char)255ULL);
long long z3 [[c6502::zeropage]] = ((long long)32767ULL);
unsigned char z4 [[c6502::zeropage]] = ((unsigned char)255ULL);
unsigned int z5 [[c6502::zeropage]] = ((unsigned int)1ULL);
unsigned char ga6[4] = {((unsigned char)0ULL), ((unsigned char)3ULL), ((unsigned char)0ULL), ((unsigned char)255ULL)};
struct S_gs7 { unsigned char pad; signed char m0; signed char m1; };
struct S_gs7 gs7 = {0, ((signed char)255ULL), ((signed char)7ULL)};
unsigned char *gp8 = &z4;
signed char *gp9 = &g2;
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
int f10(unsigned long long p11, unsigned char p12, long p13, unsigned long *pp14) {
    volatile signed char l15 = ((signed char)1ULL);
    signed char l16 = ((signed char)60ULL);
    mix(((unsigned char)((*pp14))));
    (*gp9) = (((long)1ULL) + ga6[((int)0ULL)]);
    l15 = ((int)0ULL);
    return ((*gp8) ^ gs7.m0);
}
unsigned int f17(unsigned int p18, unsigned char p19, int p20) {
    static signed char s23 = ((signed char)127ULL);
    signed char l21 = ((signed char)90ULL);
    unsigned long l22 = ((unsigned long)8ULL);
    f10((((unsigned int)32767ULL) % ((int)2ULL)), (z5 - z5), (g2 & z4), &l22);
    mix(f10((g1 % ((int)2ULL)), z3, (z5 & g1), &l22));
    mix(f10((((long)32767ULL) * gs7.m0), (z4 >= ((int)32767ULL)), (z5 >= ga6[((int)1ULL)]), &l22));
    (gs7.m1 += 3);
    return (l22 << 2);
}
int main(void) {
    unsigned char l24 = ((unsigned char)2ULL);
    unsigned long l25 = ((unsigned long)1ULL);
    unsigned char w26;
    unsigned char w27;
    l24 = f17((((unsigned int)32768ULL) - l24), ((unsigned long long)32768ULL), (((unsigned int)2ULL) && ga6[((int)2ULL)]));
    if (l24) {
        w26 = 0;
        while (w26++ < 2) {
            continue;
            w27 = 0;
            do {
                g1 = ((--l24) - ((unsigned int)128ULL));
                mix((g1 != w26));
                (*gp9) = (ga6[(w26 & ((int)3ULL))] - w26);
            } while (++w27 < 1);
            (gs7.m1 |= 1);
        }
    } else {
        (l25 *= 3);
    }
    (--gs7.m0);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
