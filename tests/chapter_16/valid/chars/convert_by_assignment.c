/* Test implicit conversions to and from character types
 * as if by assignment.
 * This test includes integer promotions, but isn't
 * explicitly focused on them.
 * */

#ifdef SUPPRESS_WARNINGS
#pragma GCC diagnostic ignored "-Wunused-parameter"
#ifdef __clang__
#pragma clang diagnostic ignored "-Wliteral-conversion"
#pragma clang diagnostic ignored "-Wconstant-conversion"
#else
#pragma GCC diagnostic ignored "-Woverflow"
#endif
#endif

// helper functions
int check_int(int converted, int expected) {
    return (converted == expected);
}

int check_uint(unsigned long converted, unsigned long expected) {
    return (converted == expected);
}

int check_long(long long converted, long long expected) {
    return (converted == expected);
}

int check_ulong(unsigned long long converted, unsigned long long expected) {
    return (converted == expected);
}

int check_double(double converted, double expected) {
    return (converted == expected);
}

int check_char(char converted, char expected) {
    return (converted == expected);
}

int check_uchar(unsigned char converted, unsigned char expected) {
    return (converted == expected);
}

int check_char_on_stack(signed char expected, int dummy1, int dummy2,
                        int dummy3, int dummy4, int dummy5, int dummy6,
                        signed char converted) {
    return converted == expected;
}

// implicitly convert a return value from a character type to another type
int return_extended_uchar(unsigned char c) {
    return c;
}

unsigned long long return_extended_schar(signed char sc) {
    return sc;
}

// implicitly truncate a return value from long long to unsigned char
unsigned char return_truncated_long(long long l) {
    return l;
}

/* Split into smaller helper functions so each function's frame
 * fits in c6502's 256-byte FP-relative addressing window. */

int test_args_from_schar(void) {
    signed char sc = -10;

    if (!check_long(sc, -10ll)) return 1;
    if (!check_uint(sc, 65526ul)) return 2;
    if (!check_double(sc, -10.0)) return 3;

    unsigned char uc = 246;
    if (!check_uchar(sc, uc)) return 4;
    return 0;
}

int test_args_to_char(void) {
    char c = -10;
    if (!check_char(-10, c)) return 5;
    if (!check_char(65526ul, c)) return 6;
    if (!check_char(-10.0, c)) return 7;

    if (!check_char_on_stack(c, 0, 0, 0, 0, 0, 0, -10.0)) return 8;
    return 0;
}

int test_args_from_uchar(void) {
    unsigned char uc = 246;
    if (!check_int(uc, 246)) return 9;
    if (!check_ulong(uc, 246ull)) return 10;

    char expected_char = -10;
    if (!check_char(uc, expected_char)) return 11;

    if (!check_uchar(4294967286ull, uc)) return 12;
    return 0;
}

int test_returns(void) {
    unsigned char uc = 246;
    signed char sc = -10;

    if (return_extended_uchar(uc) != 246) return 13;
    if (return_extended_schar(sc) != 4294967286ull) return 14;
    if (return_truncated_long(327670ll) != uc) return 15;
    return 0;
}

int test_assign_signed_char(void) {
    char array[3] = {0, 0, 0};

    array[1] = 128;
    if (array[0] || array[2] || array[1] != -128) return 16;

    array[1] = 130ull;
    if (array[0] || array[2] || array[1] != -126) return 17;

    array[1] = -2.6;
    if (array[0] || array[2] || array[1] != -2) return 18;
    return 0;
}

int test_assign_unsigned_char(void) {
    unsigned char uchar_array[3] = {0, 0, 0};

    uchar_array[1] = 65536ll;
    if (uchar_array[0] || uchar_array[2] || uchar_array[1] != 0) return 19;

    uchar_array[1] = 250ul;
    if (uchar_array[0] || uchar_array[2] || uchar_array[1] != 250) return 20;
    return 0;
}

int test_assign_to_other(void) {
    unsigned long ui = 65535UL;
    static unsigned char uc_static;
    ui = uc_static;
    if (ui) return 21;

    signed long long l = -1;
    static signed s_static = 0;
    l = s_static;
    if (l) return 22;
    return 0;
}

int main(void) {
    int r;
    if ((r = test_args_from_schar())) return r;
    if ((r = test_args_to_char())) return r;
    if ((r = test_args_from_uchar())) return r;
    if ((r = test_returns())) return r;
    if ((r = test_assign_signed_char())) return r;
    if ((r = test_assign_unsigned_char())) return r;
    if ((r = test_assign_to_other())) return r;
    return 0;
}