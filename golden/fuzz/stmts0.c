// expect: 4363
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
long g1 = ((long)32767ULL);
unsigned _BitInt(32) g2 = ((unsigned _BitInt(32))32768ULL);
unsigned _BitInt(32) g3 = ((unsigned _BitInt(32))255ULL);
unsigned char z4 [[c6502::zeropage]] = ((unsigned char)255ULL);
unsigned _BitInt(32) ga5[4] = {((unsigned _BitInt(32))127ULL), ((unsigned _BitInt(32))2ULL), ((unsigned _BitInt(32))65535ULL), ((unsigned _BitInt(32))3ULL)};
struct S_gs6 { unsigned char pad; unsigned char m0; unsigned char m1; };
struct S_gs6 gs6 = {0, ((unsigned char)255ULL), ((unsigned char)8ULL)};
unsigned _BitInt(32) *gp7 = &g3;
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
unsigned long f8(unsigned long *pp9) {
    static unsigned int s13 = ((unsigned int)127ULL);
    unsigned long l10 = ((unsigned long)0ULL);
    unsigned int l11 = ((unsigned int)65535ULL);
    unsigned char l12 = ((unsigned char)2ULL);
    unsigned char w14;
    if ((l11 += 129)) {
        mix((g2 - (l12 << 4)));
    } else {
        gs6.m0 = ((gs6.m1 |= 5) | ((long long)2147483648ULL));
    }
    (l11 -= 129);
    if ((++s13)) {
        w14 = 0;
        while (w14++ < 2) {
            g2 = (w14 | (l10 ^= 255));
            z4 = gs6.m1;
            return (gs6.m0 += 5);
            if (((int)128ULL)) {
                ((*gp7) <<= 2);
                gs6.m1 = (gs6.m0++);
            }
        }
    }
    return ((unsigned char)(l12));
}
int main(void) {
    unsigned char l15 = ((unsigned char)0ULL);
    unsigned int l16 = ((unsigned int)127ULL);
    unsigned long l17 = ((unsigned long)2147483647ULL);
    z4 = g1;
    (l15 *= 255);
    mix(((--ga5[((int)1ULL)]) + (((int)20451ULL) * ((long long)2ULL))));
    (l15--);
    ((*gp7) |= 129);
    if (((g2 += 255) + ga5[((int)0ULL)])) {
        z4 = (((unsigned char)1ULL) - ((*gp7) |= 255));
        g1 = ((*gp7) &= 255);
    } else {
        if ((gs6.m1 > l17)) {
            switch ((((int)(((--g2) + gs6.m1))) & ((int)7ULL))) {
            case 0:
                break;
            case 3:
                f8(&l17);
            case 4:
                break;
                break;
            }
        }
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
