// expect: 6939
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
int g1 = ((int)0ULL);
unsigned long long g2 = ((unsigned long long)2147483647ULL);
int g3 = ((int)57618ULL);
unsigned long long z4 [[c6502::zeropage]] = ((unsigned long long)127ULL);
unsigned int z5 [[c6502::zeropage]] = ((unsigned int)32768ULL);
unsigned long ga6[4] = {((unsigned long)2147483648ULL), ((unsigned long)2147483647ULL), ((unsigned long)0ULL), ((unsigned long)255ULL)};
struct S_gs7 { unsigned char pad; long m0; int m1; };
struct S_gs7 gs7 = {0, ((long)127ULL), ((int)47024ULL)};
unsigned int *gp8 = &z5;
int *gp9 = &g1;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
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
signed char f10(long p11, int p12, int p13) {
    static int s17 = ((int)128ULL);
    volatile unsigned int l14 = ((unsigned int)128ULL);
    unsigned char l15 = ((unsigned char)0ULL);
    unsigned int l16 = ((unsigned int)256ULL);
    unsigned char w18;
    unsigned char w19;
    l16 = ((unsigned long)(((unsigned char)255ULL)));
    w18 = 0;
    do {
        w19 = 0;
        do {
            switch ((((int)(((w18 < ((signed char)2ULL)) - ((*gp8) + l16)))) & ((int)7ULL))) {
            case 0:
                break;
            case 2:
                mix(((--l16) - (g2 + p12)));
                (gs7.m1 |= 255);
            case 5:
                z4 = w19;
                return ((*gp9) + s17);
            default:
                return (p11 | ((*gp8) *= 90));
            }
            ga6[(w18 & ((int)3ULL))] = ((unsigned int)(w18));
            s17 = (((*gp8) % ((int)2ULL)) != (z4 < g3));
        } while (++w19 < 2);
        (l14 -= 1);
    } while (++w18 < 3);
    return (l14 >> 2);
}
unsigned char f20(int p21, signed char p22, unsigned int p23) {
    static long s25 = ((long)32768ULL);
    unsigned char l24 = ((unsigned char)255ULL);
    switch ((((int)((g1--))) & ((int)7ULL))) {
    case 0:
        ((*gp8) <<= 7);
        z4 = l24;
        break;
    case 4:
        ;
    case 5:
        (*gp8) = ((unsigned char)((p21 - gs7.m1)));
        break;
    case 7:
        g1 = ((unsigned int)(l24));
        l24 = f10((g3 + ga6[((int)2ULL)]), z5, (gs7.m1 & ga6[((int)3ULL)]));
        break;
    default:
        mix((s25 += 129));
        (--(*gp8));
    }
    ((*gp8) |= 5);
    g3 = ((s25 * ((unsigned char)0ULL)) - (g3 << 1));
    f10(((unsigned char)(g2)), (gs7.m0 << 7), (p22 >> 1));
    ((*gp8)++);
    return ga6[((int)3ULL)];
}
long long f26(unsigned long p27, int p28, unsigned int p29) {
    unsigned char l30 = ((unsigned char)0ULL);
    unsigned char l31 = ((unsigned char)0ULL);
    unsigned char w32;
    unsigned char w33;
    switch ((((int)(((p29 += 255) & g1))) & ((int)7ULL))) {
    case 3:
        ;
    case 4:
        g3 = (--g2);
    case 6:
        if ((p28 &= 90)) {
            w32 = 0;
            do {
                break;
            } while (++w32 < 1);
        } else {
            (p29 *= 90);
            l30 = f10((g2 | z4), (((int)1ULL) + ((unsigned int)0ULL)), (gs7.m0 - ((unsigned int)65535ULL)));
        }
        break;
    case 7:
        break;
    }
    w33 = 0;
    do {
        (g2 += 5);
        g2 = ((*gp9) -= 255);
        (z4++);
        mix(((*gp8) & g1));
    } while (++w33 < 3);
    return (gs7.m1 != g3);
}
int main(void) {
    unsigned int l34 = ((unsigned int)128ULL);
    unsigned int l35 = ((unsigned int)2ULL);
    unsigned char l36 = ((unsigned char)0ULL);
    unsigned char w37;
    w37 = 0;
    do {
        continue;
        if ((z4 *= 5)) {
            switch ((((int)((w37 << 1))) & ((int)7ULL))) {
            case 1:
                break;
            case 2:
                break;
                mix((w37 / ((int)5ULL)));
                break;
            }
            (*gp9) = gs7.m1;
        }
    } while (++w37 < 4);
    mix(f10((((unsigned int)1ULL) * g2), ((unsigned int)2ULL), (((unsigned char)0ULL) ^ (*gp9))));
    f10(((signed char)255ULL), ((unsigned int)(g1)), (((unsigned char)128ULL) ? ((unsigned char)0ULL) : z5));
    z5 = (!((unsigned long)2ULL));
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
