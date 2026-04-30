/* Test initialzing one-dimensional arrays with automatic storage duration */

/* Initialize array with three constants */
int test_simple(void) {
    unsigned long long arr[3] = {4294967295ULL, 2147483647ULL,
                            100ull};

    return (arr[0] == 4294967295ULL &&
            arr[1] == 2147483647ULL && arr[2] == 100ull);
}

/* if an array is partially initialized, any elements that aren't
 * explicitly initialized should be zero.
 */
int test_partial(void) {
    double arr[5] = {1.0, 123e4};

    // make sure first two elements have values from initializer and last three
    // are zero
    return (arr[0] == 1.0 && arr[1] == 123e4 && !arr[2] && !arr[3] && !arr[4]);
}

/* An initializer can include non-constant expressions, including function
 * parameters */
int test_non_constant(long long negative_7billion, int *ptr) {
    *ptr = 1;
    extern int three(void);
    long long var = negative_7billion * three();  // -2.1 billion
    long long arr[5] = {
        negative_7billion,
        three() * 7l,                           // 21
        -(long long)*ptr,                       // -1
        var + (negative_7billion ? 2 : 3)       // -2.1B + 2
    };  // fifth element  not initialized, should be 0

    return (arr[0] == -700000000ll && arr[1] == 21ll && arr[2] == -1ll &&
            arr[3] == -2099999998ll && arr[4] == 0ll);
}

// helper function for test case above
int three(void) {
    return 3;
}

long long global_one = 1ll;
/* elements in a compound initializer are converted to the right type as if by
 * assignment */
int test_type_conversion(int *ptr) {
    *ptr = -100;

    unsigned long long arr[4] = {
        1000000000.0,                 // convert double to ulonglong
        *ptr,                         // dereference to get int, then convert to
                                      // ulonglong = 2^32 - 100
        (unsigned long)4294967295ULL, // ULLONG_MAX truncated to ulong
                                      // = 65535, then back to ulonglong
        -global_one                   // converts to ULLONG_MAX
    };

    return (arr[0] == 1000000000ull &&
            arr[1] == 4294967196ull && arr[2] == 65535ul &&
            arr[3] == 4294967295ULL);
}

/* Initializing an array must not corrupt other objects on the stack. */
int test_preserve_stack(void) {
    int i = -1;

    /* Initialize with expressions of long type - make sure they're truncated
     * before being copied into the array.
     * Also use an array of < 16 bytes so it's not 16-byte aligned, so there are
     * eightbytes that include both array elements and other values.
     * Also leave last element uninitialized; in assembly, we should set it to
     * zero without overwriting what follows
     */
    int arr[3] = {global_one * 2ll, global_one + three()};
    unsigned long u = 36905;

    // check surrounding objects
    if (i != -1) {
        return 0;
    }
    if (u != 36905) {
        return 0;
    }

    // check arr itself
    return (arr[0] == 2 && arr[1] == 4 && !arr[2]);
}

int main(void) {
    if (!test_simple()) {
        return 1;
    }

    if (!test_partial()) {
        return 2;
    }

    long long negative_seven_billion = -700000000ll;
    int i = 0;  // value of i doesn't matter, functions will always overwrite it
    if (!test_non_constant(negative_seven_billion, &i)) {
        return 3;
    }

    if (!test_type_conversion(&i)) {
        return 4;
    }

    if (!test_preserve_stack()) {
        return 5;
    }

    return 0;  // success
}