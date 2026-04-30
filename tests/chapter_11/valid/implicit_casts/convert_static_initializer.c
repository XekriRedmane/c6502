/* Test that a long static initializer for an int variable
 * is truncated to an int,
 * and an int static initializer for a long variable preserves the same value
 */

#ifdef SUPPRESS_WARNINGS
#ifdef __clang__
#pragma clang diagnostic ignored "-Wconstant-conversion"
#else
#pragma GCC diagnostic ignored "-Woverflow"
#endif
#endif

int i = 256ll; // 2^8, truncated to 0 in 1-byte int
long long j = 123456;

int main(void) {
    if (i != 0) {
        return 1;
    }
    if (j != 123456ll) {
        return 2;
    }
    return 0;
}