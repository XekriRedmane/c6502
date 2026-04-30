/* Test that function arguments, including arguments put on the stack,
 * are converted to the corresponding parameter type */

int foo(long long a, int b, int c, int d, long long e, int f, long long g, int h) {
    if (a != -1ll)
        return 1;

    if (b != 2)
        return 2;

    if (c != 0)
        return 3;

    if (d != -5)
        return 4;

    if (e != -101ll)
        return 5;

    if (f != -123)
        return 6;

    if (g != -10ll)
        return 7;

    if (h != -46)
        return 8;

    return 0;
}

int main(void) {
    int a = -1;
    long long int b = 258;  // 2^8 + 2, becomes 2 when converted to a 1-byte int
    long long c = -256;     // -2^8, becomes 0 when converted to int
    long long d = 65787;  // 2^16 + 251, becomes -5 (251 mod 256 = 251 → signed 1B = -5)
    int e = -101;
    long long f = -123;
    int g = -10;
    long long h = 1234;  // mod 256 = 210 → signed 1B int = -46
    return foo(a, b, c, d, e, f, g, h);
}