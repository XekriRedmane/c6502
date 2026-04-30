int main(void) {
    /* Initialize and then update a mix of long and int variables,
     * to check that we allocate enough stack space for each of them,
     * and writing to one doesn't clobber another */

    long long a = 1000000000ll; // this number is outside the range of int and long
    int b = -1;
    long long c = -1000000000ll; // also outside the range of int and long
    int d = 10;

    /* Make sure every variable has the right value */
    if (a != 1000000000ll) {
        return 1;
    }
    if (b != -1){
        return 2;
    }
    if (c != -1000000000ll) {
        return 3;
    }
    if (d != 10) {
        return 4;
    }

    /* update every variable */
    a = -a;
    b = b - 1;
    c = c + 1000000002ll;
    d = d + 10;

    /* Make sure the updated values are correct */
    if (a != -1000000000ll) {
        return 5;
    }
    if (b != -2) {
        return 6;
    }
    if (c != 2) {
        return 7;
    }
    if (d != 20) {
        return 8;
    }

    return 0;
}