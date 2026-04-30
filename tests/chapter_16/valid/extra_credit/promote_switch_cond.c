#ifdef SUPPRESS_WARNINGS
#ifdef __clang__
#pragma clang diagnostic ignored "-Wswitch"
#else
#pragma GCC diagnostic ignored "-Woverflow"
#endif
#endif

// Make sure we promote the controlling condition in a switch statement from
// character type to int

int main(void) {
    char c = 100;
    switch (c) {
        case 0:
            return 1;
        case 100:
            return 0;
        // distinct from case 100; in upstream this would be 356 to verify
        // that case constants stay at int width rather than truncating to
        // char width, but for c6502 int is the same width as char (1B)
        // so we just pick another in-range value.
        case 50:
            return 2;
        default:
            return 3;
    }
}