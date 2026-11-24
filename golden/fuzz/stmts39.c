// expect: 20232
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
_BitInt(56) g1 = ((_BitInt(56))32768ULL);
unsigned char g2 = ((unsigned char)10ULL);
unsigned long z3 [[c6502::zeropage]] = ((unsigned long)1ULL);
unsigned _BitInt(6) z4 [[c6502::zeropage]] = ((unsigned _BitInt(6))7ULL);
unsigned char ga5[4] = {((unsigned char)0ULL), ((unsigned char)0ULL), ((unsigned char)43ULL), ((unsigned char)255ULL)};
struct S_gs6 { unsigned char pad; int m0; unsigned int m1; };
struct S_gs6 gs6 = {0, ((int)2ULL), ((unsigned int)128ULL)};
unsigned char *gp7 = &g2;
unsigned long *gp8 = &z3;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(z3);
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
unsigned int f9(unsigned char p10, unsigned char p11, signed char p12, unsigned char *pp13) {
    unsigned _BitInt(6) l14 = ((unsigned _BitInt(6))1ULL);
    unsigned char w15;
    switch ((((int)((p12 | (gs6.m1 |= 3)))) & ((int)7ULL))) {
    case 2:
        if (((int)65535ULL)) {
            w15 = 0;
            do {
                gs6.m0 = (l14 == g2);
                return gs6.m1;
                gs6.m1 = ((unsigned int)((w15 ^ (*gp7))));
            } while (++w15 < 4);
            p12 = (g2--);
        } else {
            if ((*pp13)) {
                break;
            }
            if ((gs6.m1 ^= 255)) {
                p12 = ((gs6.m0++) | (*pp13));
            } else {
                mix(g1);
            }
        }
        break;
    case 3:
        ;
    case 4:
        break;
    case 5:
        ;
    }
    return (*gp8);
}
unsigned int f16(unsigned int p17) {
    unsigned _BitInt(6) l18 = ((unsigned _BitInt(6))3ULL);
    unsigned char l19 = ((unsigned char)3ULL);
    unsigned char w20;
    mix(((l18 << 2) | (z4 *= 90)));
    w20 = 0;
    while (w20++ < 2) {
        if (gs6.m1) {
            (*gp7) = (g1--);
            mix(f9((-((_BitInt(56))2ULL)), (((unsigned char)255ULL) / ((int)6ULL)), ((_BitInt(56))65536ULL), &l19));
            mix((l18++));
        } else {
            mix(f9((gs6.m1 + gs6.m0), (w20 + w20), ((*gp8) * w20), &l19));
        }
        (*gp7) = ((*gp8) >>= 6);
        l19 = (gs6.m0--);
        gs6.m0 = ((l19 += 3) + w20);
    }
    switch ((((int)(((z3++) & ((long)2ULL)))) & ((int)7ULL))) {
    case 5:
        switch ((((int)((-((unsigned char)127ULL)))) & ((int)7ULL))) {
        case 2:
            ;
        case 3:
            ;
        default:
            ;
        }
        break;
    default:
        ;
    }
    return ((long)2ULL);
}
int main(void) {
    volatile unsigned int l21 = ((unsigned int)0ULL);
    unsigned int l22 = ((unsigned int)127ULL);
    unsigned char l23 = ((unsigned char)255ULL);
    l23 = f9(((unsigned char)(gs6.m0)), (z3 || l23), (gs6.m1 ? gs6.m1 : z3), &l23);
    if ((z4 += 5)) {
        if (((++(*gp8)) & ((unsigned _BitInt(6))2ULL))) {
            gs6.m1 = ((l23 += 5) - ga5[((int)2ULL)]);
            mix(f16(((*gp8) & (*gp8))));
            if ((((signed char)128ULL) % ((int)1ULL))) {
                f16((l21 | gs6.m0));
            } else {
                l21 = f9((z4 + ((unsigned _BitInt(6))0ULL)), ((signed char)(l21)), (((signed char)0ULL) + ga5[((int)1ULL)]), &l23);
                (z4 *= 5);
            }
        }
        ((*gp8) &= 129);
    } else {
        gs6.m1 = (++gs6.m0);
        if ((z4 + g2)) {
        }
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
