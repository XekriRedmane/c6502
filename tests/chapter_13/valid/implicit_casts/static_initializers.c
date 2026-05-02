/* Test initializing static doubles with integer constants and vice versa */

#ifdef SUPPRESS_WARNINGS
#ifdef __clang__
#pragma GCC diagnostic ignored "-Wimplicit-const-int-float-conversion"
#pragma GCC diagnostic ignored "-Wliteral-conversion"
#endif
#endif

// double variables

// can convert from int/uint without rounding
double d1 = 32767;
double d2 = 65535u;

/* All c6502 4-byte integer values fit exactly in a double's
 * 52-bit mantissa — no rounding needed.
 */
double d3 = 1000000000ll;
double d4 = 1000000000ll;
double d5 = 4000000000ull;
double d6 = 1000000000ull;
double d7 = 4000000000ull;

double uninitialized; // should be initialized to 0.0

// integer variables

static int i = 4.9; // truncated to 4

unsigned long u = 42949.6e3; // truncated to 42949600 mod 2^16 = 23520 (since 42949600 = 655 * 65536 + 23520)

// this token is first converted to a double w/ value 1000000000.0,
// then truncated down to long long 1000000000
long long l = 1000000000.;

unsigned long long ul = 4000000000.;

int main(void) {
    if (d1 != 32767.) {
        return 1;
    }

    if (d2 != 65535.) {
        return 2;
    }
    if (d3 != 1000000000.) {
        return 3;
    }

    if (d4 != d3) {
        return 4;
    }

    if (d5 != 4000000000.) {
        return 5;
    }

    if (d6 != d3) {
        return 6;
    }

    if (d7 != d5) {
        return 7;
    }

    if (uninitialized) {
        return 8;
    }

    if (i != 4) {
        return 9;
    }

    /* 42949.6e3 = 42949600. Truncated to unsigned long (2B) =
     * 42949600 mod 65536 = 23520 (since 42949600 = 655*65536 + 23520).
     */
    if (u != 23520ul) {
        return 10;
    }

    if (l != 1000000000ll) {
        return 11;
    }

    if (ul != 4000000000ull) {
        return 12;
    }

    return 0;
}