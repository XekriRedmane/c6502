signed char g0 = 72;
long f4(unsigned long p0, unsigned char p1, long p2) {
    int l0 = ((((p2) && (p1))) & (((p0) | (7))));
    unsigned char l1 = p2;
    unsigned int l2 = ((((l0) * (7))) > (p1));
    unsigned char l3 = ((g0) - (((p0) || (l1))));
    return p1;
}
bool f3(register unsigned int p0, int * pp) {
    int l0 = ((((g0) & (p0))) == ((!g0)));
    register unsigned char ry = (unsigned char)(l0);
    *pp += (int)(p0);
    return ((((l0) >= (ry))) & (((2) > ((*pp)))));
}
long f2(unsigned int p0, register unsigned int p1, int p2) {
    signed char l0 = ((((g0) + (p0))) >= (((p1) ^ (g0))));
    unsigned long l1 = p1;
    unsigned char l2 = ((((l1) >= (32767))) && (((0) & (l0))));
    int box3 = (int)(l0);
    if (f3(((l2) | (l1)), &box3)) l0 += 3; else l0 ^= 5;
    l0 += box3;
    return ((signed char)(p1));
}
unsigned long f1(register int p0, long p1, int p2) {
    int l0 = ((((g0) <= (p1))) < (p0));
    signed char l1 = p1;
    unsigned long l2 = (((!p1)) <= (((p2) / ((l1) | 1))));
    return l0;
}
unsigned long f0(register unsigned char p0) {
    int l0 = (((-p0)) != (g0));
    int l1 = (~l0);
    int l2 = ((((signed char)(g0))) ^ (((l1) - (l0))));
    signed char l3 = ((((l0) / ((7) | 1))) != (l1));
    register unsigned char ry = (unsigned char)(l0);
    for (int it = 0; it < 3; it++) l1 += (f1(((g0) / ((l2) | 1)), ((long)(255)), ((p0) + (g0)))) + it;
    l2 ^= f2(((unsigned char)(l1)), ((p0) | (p0)), ((ry) == (100)));
    return ((((3) * (l0))) || (((l0) / ((l2) | 1))));
}
int main(void) {
    long r = (long)f0(117);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
