/* Test long long expressions in &&, ||, ! and controlling expressions */

int not(long long l) {
    return !l;
}

int if_cond(long long l) {
    if (l) {
        return 1;
    }
    return 0;
}

int and(long long l1, int l2) {
    return l1 && l2;
}

int or(int l1, long long l2) {
    return l1 || l2;
}

int main(void) {
    // this would be equal to zero if we only considered lower 16 bits
    long long l = 1073741824ll; // 2^30; low 16 bits == 0
    long long zero = 0ll;
    if (not(l)) {
        return 1;
    }
    if (!not(zero)) {
        return 2;
    }
    if(!if_cond(l)) {
        return 3;
    }
    if(if_cond(zero)) {
        return 4;
    }

    if (and(zero, 1)) {
        return 5;
    }

    if (!or(1, l)) {
        return 6;
    }

    return 0;

}