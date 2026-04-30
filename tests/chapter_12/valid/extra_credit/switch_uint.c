#ifdef SUPPRESS_WARNINGS
#ifdef __clang__
#pragma clang diagnostic ignored "-Wswitch"
#else
#pragma GCC diagnostic ignored "-Woverflow"
#endif
#endif

int switch_on_uint(unsigned long ui) {
    switch (ui) {
        case 5ul:
            return 0;
        // this will be converted to an unsigned long, preserving its value
        case 65526ll:
            return 1;
        // 2^17 + 10, will be converted (mod 2^16) to 10
        case 131082ull:
            return 2;
        default:
            return 3;
    }
}

int main(void) {
    if (switch_on_uint(5) != 0)
        return 1;
    if (switch_on_uint(65526) != 1)
        return 2;
    if (switch_on_uint(10) != 2)
        return 3;
    return 0;
}