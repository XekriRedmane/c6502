unsigned char g0 = 17;
unsigned char f2(unsigned int p0, signed char p1, unsigned int p2) {
    signed char l0 = 1;
    int l1 = ((((unsigned char)(p0))) != (p1));
    return ((int)(((g0) / ((p0) | 1))));
}
long f1(long p0, long p1) {
    unsigned long l0 = ((((p0) <= (p0))) >= (p1));
    int l1 = ((unsigned char)(((p0) >= (l0))));
    signed char l2 = ((255) || (((l0) / ((g0) | 1))));
    long l3 = ((((32767) / ((p0) | 1))) == (((l1) * (l2))));
    unsigned char (*fp)(unsigned int, signed char, unsigned int) = f2;
    l0 ^= fp(((l1) < (l3)), g0, ((p0) >> ((p0) & 7)));
    g0 += l0;
    return l3;
}
long f0(register unsigned int p0, unsigned int p1, long p2) {
    unsigned int l0 = p2;
    register unsigned char ry = (unsigned char)(l0);
    return ((((g0) && (p0))) == (((p1) | (ry))));
}
int main(void) {
    long r = (long)f0(178, 198, 167);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
