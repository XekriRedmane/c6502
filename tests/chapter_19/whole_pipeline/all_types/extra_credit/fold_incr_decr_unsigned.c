/* Propagate ++/-- with unsigned integers (make sure they wrap around correctly) */

long long target(void) {
    unsigned long long u = 0;
    unsigned long long u2 = --u;
    unsigned long long u3 = u--;

    unsigned long long u4 = 4294967295U;
    unsigned long long u5 = u4++;
    unsigned long long u6 = ++u4;

    if (!(u == 4294967294U && u2 == 4294967295U && u3 == 4294967295U)) {
        return 1; // fail
    }

    if (!(u4 == 1 && u5 == 4294967295U && u6 == 1)) {
        return 2; // fail
    }

    return 0; // success
}

long long main(void) {
    return target();

}