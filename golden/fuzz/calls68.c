unsigned long g0 = 128;
signed char g1 = 155;
long g2 = 62;
unsigned long f5(signed char p0, long p1, unsigned long p2, unsigned char p3) {
    unsigned int l0 = p0;
    unsigned int l1 = (!100);
    int l2 = ((p3) + (((32767) >> ((g2) & 7))));
    return p1;
}
bool f4(unsigned int p0, register unsigned int p1, long p2, unsigned int p3) {
    long l0 = ((((g2) << ((g0) & 7))) >= (((0) == (g0))));
    long l1 = ((((g1) | (p2))) > (((3) != (g2))));
    unsigned int l2 = ((p0) / (((!g0)) | 1));
    register unsigned char ry = (unsigned char)(l0);
    return ((((g0) <= (7))) > (((signed char)(l0))));
}
long f3(unsigned char p0) {
    unsigned char l0 = ((((g1) == (g1))) > (((g1) != (g1))));
    register unsigned char ry = (unsigned char)(l0);
    l0 ^= f4(((p0) & (100)), g0, 3, ((p0) | (p0)));
    g1 += l0;
    return ((((g0) - (ry))) * (((7) != (p0))));
}
unsigned long f2(signed char p0, int p1, signed char p2, int * pp) {
    long l0 = (((~p1)) / ((((unsigned long)(p1))) | 1));
    unsigned int l1 = ((((l0) << ((p2) & 7))) == (((g0) < (p0))));
    unsigned char l2 = p1;
    *pp += (int)(((l2) | (l1)));
    unsigned long (*fp)(signed char, long, unsigned long, unsigned char) = f5;
    for (int it = 0; it < 3; it++) l2 += (fp(p2, (((*pp)) > (32767)), ((g1) >= (p1)), ((l0) & (p1)))) + it;
    g1 += l2;
    return p1;
}
long f1(int * pp) {
    unsigned char l0 = ((((0) == (32767))) * (0));
    unsigned int l1 = g1;
    unsigned int l2 = ((signed char)(((g1) << ((g0) & 7))));
    unsigned long l3 = 255;
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(l2);
    return ((3) & (ry));
}
unsigned long f0(register unsigned int p0, long p1) {
    int l0 = p0;
    l0 ^= f4(((p1) << ((g0) & 7)), ((p1) || (0)), ((1000) <= (p0)), ((p0) % ((3) | 1)));
    g0 += l0;
    return p1;
}
int main(void) {
    long r = (long)f0(91, 29);
    r ^= (long)g0 ^ (long)g1 ^ (long)g2;
    return (int)(r ^ (r >> 16));
}
