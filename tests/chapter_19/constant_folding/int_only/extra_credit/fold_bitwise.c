/* Test constant-folding the bitwise &, |, ^, >>, and << expressions */

long long target_and(void) {
    // 0x0f0f_0f0f & 0x00ff_00ff
    return 252645135LL & 16711935LL;
}

long long target_or(void) {
    // 0x0f0f_0f0f | 0x00ff_00ff
    return 252645135LL | 16711935LL;
}

long long target_xor(void){
    // 0x0f0f_0f0f ^ 0x00ff_00ff
    return 252645135LL ^ 16711935LL;
}

long long target_shift_left(void) {
    return 291LL << 18LL;
}

long long target_shift_right(void) {
    return 252645135LL >> 9LL;
}

long long main(void) {
    if (target_and() != 983055LL) {
        return 1LL;
    }

    if (target_or() != 268374015LL) {
        return 2LL;
    }

    if (target_xor() != 267390960LL) {
        return 3LL;
    }

    if (target_shift_left() != 76283904LL) {
        return 4LL;
    }

    if (target_shift_right() != 493447LL){
        return 5LL;
    }

     return 0LL;
}