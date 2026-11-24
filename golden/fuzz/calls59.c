unsigned _BitInt(38) g0 = 5;
int rec(int d, int x) { if (d <= 0) return x & 255; return rec(d - 1, (x * 3 + d) & 1023) + 1; }
long f3(int * pp) {
    int l0 = ((unsigned long)(g0));
    _BitInt(5) l1 = ((l0) || (((l0) != (g0))));
    unsigned int l2 = l1;
    *pp += (int)(((l1) > (g0)));
    return ((((g0) ^ (g0))) < (((long)(l0))));
}
int f2(unsigned char p0, unsigned char p1, unsigned long p2, signed char p3) {
    unsigned long l0 = (~p0);
    int l1 = (((~l0)) - (((p0) ^ (0))));
    unsigned long l2 = ((((unsigned int)(l1))) % ((p2) | 1));
    int box3 = (int)(l0);
    long (*fp)(int *) = f3;
    l1 ^= fp(&box3);
    l1 += box3;
    g0 += l1;
    return ((((g0) & (p3))) == (((p2) + (l2))));
}
long f1(unsigned long p0) {
    int l0 = g0;
    int l1 = ((((p0) && (2))) != (((p0) + (g0))));
    long l2 = p0;
    unsigned int l3 = (~((unsigned int)(255)));
    int box3 = (int)(l0);
    long (*fp)(int *) = f3;
    l3 ^= fp(&box3);
    l3 += box3;
    g0 += l3;
    return ((l1) | (p0));
}
signed char f0(signed char p0, unsigned _BitInt(38) p1, _BitInt(5) p2, unsigned _BitInt(38) p3) {
    unsigned int l0 = ((((p0) * (p1))) && (((g0) || (32767))));
    unsigned _BitInt(38) l1 = p0;
    _BitInt(5) l2 = ((((l1) && (32767))) + (((p1) * (l0))));
    l0 ^= f2(p2, ((0) >= (g0)), ((p0) > (2)), l2);
    g0 += l0;
    l1 ^= f1(255);
    return ((255) + (p0));
}
int main(void) {
    long r = (long)f0(84, 267, 128, 89);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
