/* Test unsigned expressions in &&, ||, ! and controlling expressions
 * Almost identical to chapter 11 logical.c, but with unsigned ints
 */

int not(unsigned long long ul) {
    return !ul;
}

int if_cond(unsigned long u) {
    if (u) {
        return 1;
    }
    return 0;
}

int and(unsigned long long ul, int i) {
    return ul && i;
}

int or(int i, unsigned long u) {
    return i || u;
}

int main(void) {
    // this would be equal to zero if we only considered lower 16 bits
    unsigned long long ul = 1073741824ull; // 2^30; low 16 bits == 0
    unsigned long u = 32768ul; // 2^15
    unsigned long long zero = 0ll;
    if (not(ul)) {
        return 1;
    }
    if (!not(zero)) {
        return 2;
    }
    if(!if_cond(u)) {
        return 3;
    }
    if(if_cond(zero)) {
        return 4;
    }

    if (and(zero, 1)) {
        return 5;
    }

    if (!or(1, u)) {
        return 6;
    }

    return 0;

}