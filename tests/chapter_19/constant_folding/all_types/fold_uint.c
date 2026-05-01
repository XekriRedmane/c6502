/* Test constant folding of all operations on unsigned ints;
 * make sure they wrap around correctly
 * and that we evaluate them with unsigned division/comparison functions.
 */
unsigned long long target_add(void) {
    // result exceeds UINT_MAX and wraps around past 0
    return 4294967295ULL + 10uLL;
}

unsigned long long target_sub(void) {
    // result is less then 0 and wraps back round past UINT_MAX
    return 10uLL - 12uLL;
}

unsigned long long target_mult(void) {
    // wraps back around to 2147483648uLL
    return 2147483648uLL * 3uLL;
}

unsigned long long target_div(void) {
    // result would be different if we interpreted values as signed
    return 4294967286uLL / 10uLL;
}

unsigned long long target_rem(void) {
    // result would be different if we interpreted values as signed
    return 4294967286uLL % 10uLL;
}

unsigned long long target_complement(void) {
    return ~1uLL;
}

unsigned long long target_neg(void) {
    return -10uLL;
}

int target_not(void) {
    return !65536uLL;  // 2^16
}

int target_eq(void) {
    return 100uLL == 100uLL;
}

int target_neq(void) {
    // these have identical binary representations except for the most
    // significant bit
    return 2147483649uLL != 1uLL;
}

int target_gt(void) {
    // make sure we're using unsigned comparisons;
    // if we interpret these as signed integers,
    // we'll think 2147483649uLL is negative and return 0
    return 2147483649uLL > 1000uLL;
}

int target_ge(void) {
    return 4000000000uLL >= 3999999999uLL;
}

int target_lt(void) {
    // as with target_gt, make sure we don't interpret 2147483649uLL
    // as a negative signed integer
    return 2147483649uLL < 1000uLL;
}

int target_le(void) {
    return 4000000000uLL <= 3999999999uLL;
}

int main(void) {
    // binary arithmetic
    if (target_add() != 9uLL) {
        return 1;
    }
    if (target_sub() != 4294967294ULL) {
        return 2;
    }
    if (target_mult() != 2147483648uLL) {
        return 3;
    }
    if (target_div() != 429496728uLL) {
        return 4;
    }
    if (target_rem() != 6uLL) {
        return 5;
    }

    // unary operators
    if (target_complement() != 4294967294ULL) {
        return 6;
    }

    if (target_neg() + 10 != 0) {
        return 7;
    }

    if (target_not() != 0) {
        return 8;
    }

    // comparisons
    if (!target_eq()) {
        return 9;
    }
    if (!target_neq()) {
        return 10;
    }
    if (!target_gt()) {
        return 11;
    }
    if (!target_ge()) {
        return 12;
    }
    if (target_lt()) {
        return 13;
    }
    if (target_le()) {
        return 14;
    }

    return 0;
}
