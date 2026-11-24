int g0 = 170;
unsigned char f3(int p0) {
    unsigned char l0 = (((!g0)) * ((!g0)));
    return ((p0) % ((((unsigned long)(l0))) | 1));
}
long f2(void) {
    long l0 = ((((g0) + (g0))) % ((g0) | 1));
    unsigned char (*fp)(int) = f3;
    l0 ^= fp(l0);
    g0 += l0;
    return ((((g0) < (l0))) >> ((l0) & 7));
}
unsigned long f1(unsigned long p0, unsigned char p1, unsigned long p2) {
    signed char l0 = ((unsigned char)(((0) <= (p2))));
    unsigned int l1 = p1;
    return ((((p1) >= (p1))) || (((p1) != (1))));
}
unsigned int f0(void) {
    long l0 = ((g0) || (((g0) || (g0))));
    int l1 = ((((l0) >> ((g0) & 7))) % ((100) | 1));
    long l2 = l1;
    l0 ^= f1(((l0) > (l2)), ((g0) | (l1)), 7);
    l0 ^= f2();
    return l1;
}
int main(void) {
    long r = (long)f0();
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
