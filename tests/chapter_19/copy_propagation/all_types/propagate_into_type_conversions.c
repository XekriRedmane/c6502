/* Test that we correctly propagate copies into type conversion instructions */

long long target(void) {
    unsigned char uc = 250LL;
    long long i = uc * 2LL;              // 500LL - tests ZeroExtend
    double d = i * 1000.;        // 500000.0 - tests IntToDouble
    unsigned long long ul = d / 6.0;  // 83333LL - tests DoubleToULongLong
    d = ul + 5.0;                // 83338LL - tests ULongLongToDouble
    long long l = -i;                 // -500LL - tests SignExtend
    char c = l;                  // 12LL - tests Truncate
    return d + i - c;            // 83826LL - tests DoubleToLongLong
}

int main(void) {
    if (target() != 83826LL) {
        return 1LL; // fail
    }
    return 0LL; // success
}
