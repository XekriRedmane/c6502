/* Make sure static initializers are set to the correct
 * implicitly-converted value at program startup
 */

#ifdef SUPPRESS_WARNINGS
#ifdef __clang__
#pragma clang diagnostic ignored "-Wconstant-conversion"
#else
#pragma GCC diagnostic ignored "-Woverflow"
#endif
#endif

// this is 2^25 + 2^15 + 12
// should be truncated to 2^15 + 12 (which is 32780)
unsigned long u = 33587212l;

/* This should be initialized to -32766,
 * or 32770 - 2^16
 */
long i = 32770u;

/* This should be initialized to -100,
 * or 4294967196 - 2^32
 */
long long l = 4294967196u; // note: this has type unsigned long long

// this can be converted to a long long with no change in value
long long l2 = 32770u;

// any unsigned long can be converted to an unsigned long long w/ no change in value
unsigned long long ul = 65534u;

/* any signed long long _literal_ can be converted to an unsigned long long w/ no change in value
 * (we don't support negation expressions in constant initializers) */
unsigned long long ul2 = 2147483638l;

// truncate ulong 2**31 + 2**15 + 150
// to int -2**15 + 150 (which is -32618)
long i2 = 2147516614ul;

// truncate ulong 2**31 + 2**15 + 150
// to uint 2**15 + 150 (which is 32918)
unsigned long ui2 = 2147516614ul;

int main(void)
{
    if (u != 32780ul)
        return 1;
    if (i != -32766)
        return 2;
    if (l != -100ll)
        return 3;
    if (l2 != 32770ll)
        return 4;
    if (ul != 65534ull)
        return 5;
    if (ul2 != 2147483638ull)
        return 6;
    if (i2 != -32618)
        return 7;
    if (ui2 != 32918ul)
        return 8;
    return 0;
}