unsigned int g0 = 119;
unsigned int f2(signed char p0) {
    unsigned int l0 = (((!g0)) <= (3));
    return (!((g0) % ((p0) | 1)));
}
unsigned long f1(register int p0, int * pp) {
    long l0 = p0;
    unsigned int l1 = ((((l0) >= (l0))) * (g0));
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(((p0) && (2)));
    return ((((ry) && (l1))) && ((((*pp)) * (l0))));
}
unsigned int f0(long p0, int p1, unsigned char p2) {
    long l0 = p1;
    long l1 = l0;
    register unsigned char ry = (unsigned char)(l0);
    int box1 = (int)(l0);
    for (int it = 0; it < 3; it++) l0 += (f1(((l0) - (7)), &box1)) + it;
    l0 += box1;
    g0 += l0;
    return ((((l1) * (ry))) | (((p0) / ((p1) | 1))));
}
int main(void) {
    long r = (long)f0(245, 51, 48);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
