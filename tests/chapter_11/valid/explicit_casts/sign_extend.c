long long sign_extend(int i, long long expected) {
    long long extended = (long long) i;
    return (extended == expected);
}


int main(void) {
    /* Converting a positive or negative int to a long long preserves its value */
    if (!sign_extend(10, 10ll)) {
        return 1;
    }

    if (!sign_extend(-10, -10ll)) {
        return 2;
    }

    /* sign-extend a constant to make sure we've implemented rewrite rule for sign-extend correctly */
    long long l = (long long) 100;
    if (l != 100ll) {
        return 3;
    }
    return 0;
}