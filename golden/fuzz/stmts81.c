// expect: 5327
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
long g1 = ((long)2147483648ULL);
signed char g2 = ((signed char)255ULL);
int g3 = ((int)2ULL);
long long ga4[4] = {((long long)32767ULL), ((long long)2672242605973956915ULL), ((long long)2ULL), ((long long)3810093443866040379ULL)};
struct S_gs5 { unsigned char pad; unsigned int m0; unsigned char m1; };
struct S_gs5 gs5 = {0, ((unsigned int)3ULL), ((unsigned char)255ULL)};
int *gp6 = &g3;
signed char *gp7 = &g2;
void fin0(void) {
    mix(g1);
    mix(g2);
    mix(g3);
    mix(ga4[((int)0ULL)]);
}
void fin1(void) {
    mix(ga4[((int)1ULL)]);
    mix(ga4[((int)2ULL)]);
    mix(ga4[((int)3ULL)]);
    mix(gs5.m0);
}
void fin2(void) {
    mix(gs5.m1);
}
unsigned char f8(unsigned long long p9, long p10, unsigned int p11, int *pp12) {
    static volatile unsigned int s15 = ((unsigned int)1ULL);
    unsigned char l13 = ((unsigned char)127ULL);
    unsigned long long l14 = ((unsigned long long)3ULL);
    (*gp6) = ((signed char)(((long)127ULL)));
    mix((g3 >> 2));
    if ((((unsigned char)1ULL) % ((int)3ULL))) {
        gs5.m1 = (((*pp12) -= 1) - ((long long)(((unsigned int)0ULL))));
        switch ((((int)((l14 & ga4[((int)3ULL)]))) & ((int)7ULL))) {
        case 2:
            switch ((((int)(((g3++) ^ ((unsigned char)0ULL)))) & ((int)7ULL))) {
            case 0:
                (s15 &= 1);
                break;
            case 5:
                break;
            default:
                break;
                break;
            }
            (gs5.m1--);
        default:
            break;
            return (l14 | p9);
        }
    }
    return (g2 - (*gp7));
}
int main(void) {
    volatile long l16 = ((long)255ULL);
    int l17 = ((int)0ULL);
    for (unsigned char i18 = 0; i18 < 2; i18++) {
        gs5.m0 = (g3 | (gs5.m0 / ((int)7ULL)));
        gs5.m1 = (((unsigned int)32768ULL) + gs5.m0);
        if ((!g1)) {
            (g3++);
            g1 = (((unsigned char)(ga4[(i18 & ((int)3ULL))])) - (gs5.m1 * gs5.m1));
            continue;
        } else {
            g2 = ga4[(i18 & ((int)3ULL))];
        }
    }
    (l17 ^= 3);
    l16 = (l17 += 1);
    (*gp7) = (*gp7);
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
