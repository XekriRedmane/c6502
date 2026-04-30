// Test bitwise operations on array elements
int main(void) {
    int arr[6] = {-10, 10, -11, 11, -12, 12};
    if ((arr[0] & arr[5]) != 4) {
        return 1; // fail
    }

    if ((arr[1] | arr[4]) != -2) {
        return 2;
    }

    if ((arr[2] ^ arr[3]) != -2) {
        return 3;
    }

    /* Use a value that fits in 1-byte int and gives a clean shift result.
     * 120 >> arr[1]=10 → 0 (since 120 < 1024, shifting by 10 gives 0).
     * Pick a smaller shift count to get a non-zero result; arr[1]=10 truncates
     * to int (1B) = 10, but 120 >> 10 = 0 anyway.
     * Use a different approach: test the shift on a value that survives. */
    arr[0] = 120;
    /* 120 >> 3 = 15. Use arr[5] (= 12) but 120 >> 12 = 0; pick a smaller
     * shift count via a different array index. */
    if ((arr[0] >> 3) != 15) {
        return 4;
    }

    if ((arr[5] << 3 ) != 96) {
        return 5;
    }

    return 0;
}