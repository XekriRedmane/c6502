unsigned int g0 = 141;
unsigned int f4(signed char p0) {
    long l0 = ((p0) ^ (((p0) >> ((g0) & 7))));
    long l1 = ((((g0) && (l0))) % ((((p0) || (g0))) | 1));
    unsigned int l2 = p0;
    return l2;
}
int f3(unsigned int p0, unsigned char p1, unsigned char p2, long p3) {
    unsigned int l0 = ((((p3) + (100))) / ((((p1) % ((p1) | 1))) | 1));
    unsigned char l1 = (((-l0)) | (((p1) != (g0))));
    unsigned int l2 = ((l1) - (l0));
    long l3 = p1;
    unsigned int (*fp)(signed char) = f4;
    for (int it = 0; it < 3; it++) l2 += (fp(l3)) + it;
    return ((((1) || (l0))) > (l1));
}
signed char f2(unsigned int p0, unsigned long p1) {
    long l0 = 1;
    unsigned int l1 = ((32767) / ((((long)(l0))) | 1));
    unsigned char l2 = ((((l0) + (32767))) <= (((p0) != (l1))));
    signed char l3 = (~((l2) >> ((l1) & 7)));
    return ((((l3) || (g0))) - (((1) > (255))));
}
unsigned long f1(void) {
    long l0 = ((((g0) == (g0))) + (((0) + (g0))));
    unsigned int l1 = ((((l0) >= (g0))) == (((l0) <= (g0))));
    long l2 = ((((long)(g0))) == (g0));
    register unsigned char ry = (unsigned char)(l0);
    return ((signed char)(l2));
}
long f0(register unsigned int p0, long p1, signed char p2) {
    unsigned long l0 = (-((long)(p0)));
    unsigned long l1 = ((((g0) < (p1))) / ((((int)(1000))) | 1));
    int l2 = ((((long)(p0))) << ((l0) & 7));
    unsigned long l3 = (((!p2)) != (g0));
    register unsigned char ry = (unsigned char)(l0);
    l3 ^= f1();
    l1 ^= f2(((p1) ^ (p1)), ((p2) * (p1)));
    return ry;
}
int main(void) {
    long r = (long)f0(112, 177, 46);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
