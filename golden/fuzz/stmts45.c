// expect: 3775
unsigned int acc;
void mix(unsigned long v) {
    acc = (unsigned int)((acc << 1) | (acc >> 15)) ^ (unsigned int)v ^ (unsigned int)(v >> 16);
}
volatile long g1 = ((long)127ULL);
unsigned long long g2 = ((unsigned long long)65535ULL);
signed char g3 = ((signed char)255ULL);
unsigned int ga4[4] = {((unsigned int)0ULL), ((unsigned int)0ULL), ((unsigned int)65535ULL), ((unsigned int)65535ULL)};
struct S_gs5 { unsigned char pad; unsigned int m0; unsigned int m1; };
struct S_gs5 gs5 = {0, ((unsigned int)256ULL), ((unsigned int)0ULL)};
signed char *gp6 = &g3;
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
signed char f7(unsigned char *pp8) {
    static long s10 = ((long)2147483648ULL);
    int l9 = ((int)0ULL);
    (g2 *= 255);
    mix((g3 &= 3));
    (g3 >>= 6);
    ((*pp8) <<= 3);
    switch ((((int)(((*gp6) && s10))) & ((int)7ULL))) {
    case 4:
        break;
    default:
        s10 = ((long long)2147483647ULL);
        (*gp6) = ((*pp8) % ((int)5ULL));
    }
    return (g3 * s10);
}
int main(void) {
    long l11 = ((long)2ULL);
    unsigned char l12 = ((unsigned char)7ULL);
    mix(f7(&l12));
    if (((g2 == ((signed char)0ULL)) & ((unsigned char)(gs5.m0)))) {
        f7(&l12);
        if (((ga4[((int)1ULL)] <<= 7) | ((unsigned int)47240ULL))) {
            (g2 *= 129);
            l11 = f7(&l12);
            g1 = (++g2);
        } else {
            switch ((((int)((((*gp6) -= 5) | (gs5.m1 << 3)))) & ((int)7ULL))) {
            case 1:
                break;
                break;
            case 7:
                g3 = g1;
                break;
            }
            (l11++);
        }
        mix(f7(&l12));
    }
    fin0();
    fin1();
    fin2();
    return (int)(acc & 0x7FFF);
}
