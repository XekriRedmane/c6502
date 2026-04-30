// Test compound assignment where LHS is a subscript expression

unsigned long unsigned_arr[4] = {65535UL, 65534UL, 65533UL, 65532UL};

int idx = 2;
long long_idx = 1;

int main(void) {
    long_idx = -long_idx; // -1
    // flat array
    unsigned_arr[1] += 2;  // should wrap around to 0
    if (unsigned_arr[1]) {
        return 1;  // fail
    }
    unsigned_arr[idx] -= 10.0;
    if (unsigned_arr[idx] != 65523UL) {
        return 2;  // fail
    }

    unsigned long *unsigned_ptr = unsigned_arr + 4;  // pointer one past end
    unsigned_ptr[long_idx] /= 10;  // pointer to last element, unsigned_arr[3]
    if (unsigned_arr[3] != 6553UL) {
        return 3;  // fail
    }

    // unsigned_arr[2]; 65523 * 65535 (wraps around mod 2^16)
    // = 65523 * 65535 = 4293459405; mod 65536 = 4293459405 - 65521*65536 = 13
    unsigned_ptr[long_idx *= 2] *= unsigned_arr[0];
    if (unsigned_arr[2] != 13) {
        return 4;  // fail
    }

    // unsigned_arr[2 + -2] --> unsigned_arr[0]
    if ((unsigned_arr[idx + long_idx] %= 10) != 5) {
        return 5;  // fail
    }

    // validate other three four elements; make sure updating one didn't
    // accidentally clobber its neighbors
    if (unsigned_arr[0] != 5ul) {
        return 6;  // fail
    }

    if (unsigned_arr[1]) {  // should still be 0
        return 7;           // fail
    }

    if (unsigned_arr[2] != 13) {
        return 8;  // fail
    }

    if (unsigned_arr[3] != 6553UL) {
        return 9;  // fail
    }

    return 0;
}