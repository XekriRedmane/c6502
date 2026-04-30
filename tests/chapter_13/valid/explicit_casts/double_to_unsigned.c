/* Test conversions from double to unsigned integer types */

unsigned long double_to_uint(double d) {
    return (unsigned long) d;
}

unsigned long long double_to_ulong(double d) {
    return (unsigned long long) d;
}

int main(void) {

    // try a double in the range of signed long
    if (double_to_uint(10.9) != 10ul) {
        return 1;
    }

    // now try a double in the range of unsigned long but not of long
    if (double_to_uint(50000.5) != 50000ul) {
        return 2;
    }

    // convert a double within the range of signed long long
    if (double_to_ulong(1000000000.5) != 1000000000ull) {
        return 3;
    }

    // now convert a double larger than LONG_LONG_MAX
    if (double_to_ulong(3000000000.0) != 3000000000ull) {
        return 4;
    }

    return 0;

}