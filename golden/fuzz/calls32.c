unsigned char g0 = 77;
unsigned int f2(long p0, signed char p1) {
    int l0 = ((g0) >= (p0));
    signed char l1 = ((l0) * (7));
    return ((p1) == (((p0) & (p0))));
}
[[c6502::return_bool_in_carry]] bool f1(register signed char p0, long p1) {
    signed char l0 = g0;
    return ((g0) <= (((3) ^ (p1))));
}
signed char f0(int * pp) {
    long l0 = ((((g0) ^ (g0))) >= (((g0) < (g0))));
    unsigned char l1 = ((long)((!g0)));
    unsigned char l2 = l0;
    unsigned int l3 = ((l0) && (((unsigned long)(l0))));
    *pp += (int)(((l1) ^ (l0)));
    unsigned int (*fp)(long, signed char) = f2;
    l1 ^= fp(((signed char)(g0)), ((g0) / ((g0) | 1)));
    if (f1(((long)(l0)), (-0))) l1 += 3; else l1 ^= 5;
    return (((((*pp)) || (g0))) <= (((g0) ^ ((*pp)))));
}
int main(void) {
    int mbox = 5;
    long r = (long)f0(&mbox);
    r ^= (long)g0;
    return (int)(r ^ (r >> 16));
}
