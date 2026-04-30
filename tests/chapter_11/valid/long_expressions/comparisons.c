/* Test comparisons between long longs, making sure to exercise all rewrite rules for cmp */

long long l;
long long l2;

/* Comparisons where both operands are constants */
int compare_constants(void) {
    /* Note that if we considered only the lower 16 bits of
     * each number (or cast them to longs), 255 would be larger,
     * because 131073ll == 2^17 + 1 (low 16 bits == 1).
     * This exercises the rewrite rule for cmp with two constant operands
     */
    return 131073ll > 255ll;
}

int compare_constants_2(void) {
    /* Same as above with operands swapped: cmp with two constants. */
    return 255ll < 131073ll;
}

int l_geq_2_30(void) {
    /* This exercises the rewrite rule for cmp where src is a large constant
     * and dst is a variable.
     * 1073741824ll == 2^30
     */
    return (l >= 1073741824ll);
}

int ulong_max_leq_l(void) {
    /* The first operand to cmp is a variable and second is a
     * constant (ULONG_MAX as a long long). */
    return (65535ll <= l);
}

int l_eq_l2(void) {
    /* Exercise rewrite rule for cmp where both operands are in memory */
    return (l == l2);
}

int main(void) {

    if (!compare_constants()) {
        return 1;
    }

    if (!compare_constants_2()) {
        return 2;
    }

    l = -2147483647ll; // LONG_LONG_MIN + 1
    if (l_geq_2_30()) {
        return 3;
    }
    if (ulong_max_leq_l()) {
        return 4;
    }
    l = 1073741824ll; // 2^30
    if (!l_geq_2_30()) {
        return 5;
    }
    if (!ulong_max_leq_l()) {
        return 6;
    }
    l2 = l;
    if (!l_eq_l2()) {
        return 7;
    }
    return 0;
}
