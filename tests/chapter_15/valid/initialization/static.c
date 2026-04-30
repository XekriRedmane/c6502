/* Test initializing one-dimensional arrays with static storage duration */

// fully initialized
double double_arr[3] = {1.0, 2.0, 3.0};

int check_double_arr(double *arr) {
    if (arr[0] != 1.0) {
        return 1;
    }

    if (arr[1] != 2.0) {
        return 2;
    }

    if (arr[2] != 3.0) {
        return 3;
    }

    return 0;
}

// partly initialized
unsigned long uint_arr[5] = {
    1ul,
    0ul,
    32781ul,
};

int check_uint_arr(unsigned long *arr) {
    if (arr[0] != 1ul) {
        return 4;
    }

    if (arr[1]) {
        return 5;
    }
    if (arr[2] != 32781ul) {
        return 6;
    }

    if (arr[3] || arr[4]) {
        return 7;
    }

    return 0;
}

// uninitialized; should be all zeros
// (use 100 instead of 1000 elements — 1000 longs = 2000 bytes,
//  which is fine for static storage but slow for codegen testing.)
long long_arr[100];

int check_long_arr(long *arr) {
    for (int i = 0; i < 100; i = i + 1) {
        if (arr[i]) {
            return 8;
        }
    }
    return 0;
}

// initialized w/ values of different types
unsigned long long ulong_arr[4] = {
    100.0, 11, 12345ll, 65535UL
};

int check_ulong_arr(unsigned long long *arr) {
    if (arr[0] != 100ull) {
        return 9;
    }

    if (arr[1] != 11ull) {
        return 10;
    }

    if (arr[2] != 12345ull) {
        return 11;
    }

    if (arr[3] != 65535Ull) {
        return 12;
    }
    return 0;
}

int test_global(void) {
    int check = check_double_arr(double_arr);
    if (check) {
        return check;
    }

    check = check_uint_arr(uint_arr);
    if (check) {
        return check;
    }
    check = check_long_arr(long_arr);
    if (check) {
        return check;
    }
    check = check_ulong_arr(ulong_arr);
    if (check) {
        return check;
    }
    return 0;
}

// equivalent static local arrays
int test_local(void) {

    // fully initialized
    double local_double_arr[3] = {1.0, 2.0, 3.0};
    // partly initialized
    static unsigned long local_uint_arr[5] = {
        1ul,
        0ul, // truncated to 0
        32781ul,
    };

    // uninitialized
    static long local_long_arr[100];

    // initialized w/ values of different types
    static unsigned long long local_ulong_arr[4] = {
        100.0, 11, 12345ll, 65535UL
    };

    // validate
    int check = check_double_arr(local_double_arr);
    if (check) {
        return 100 + check;
    }

    check = check_uint_arr(local_uint_arr);
    if (check) {
        return 100 + check;
    }
    check = check_long_arr(local_long_arr);
    if (check) {
        return 100 + check;
    }
    check = check_ulong_arr(local_ulong_arr);
    if (check) {
        return 100 + check;
    }
    return 0;
}

int main(void) {
    int check = test_global();
    if (check) {
        return check;
    }
    return test_local();
}
