// Compound assignment where lval is a subscript expression with pointer type
int main(void) {
    // array of 3 pointers to arrays of 4 ints
    static int (*array_of_pointers[3])[4] = {0, 0, 0};
    // Values renumbered to fit in c6502's 1-byte signed int (-128..127);
    // upstream uses 100/200/300 ranges which would wrap.
    int array1[4] = {10, 11, 12, 13};
    int nested_array[2][4] = {
        {20, 21, 22, 23},
        {30, 31, 32, 33}
    };
    array_of_pointers[0] = &array1;
    array_of_pointers[1] = &nested_array[0];
    array_of_pointers[2] = &nested_array[1];

    array_of_pointers[0] += 1; // points one past the end of array1
    if (array_of_pointers[0][-1][3] != 13) {
        return 1; // fail
    }

    // swap these so they point to last and first elements of nested_array, respectively
    array_of_pointers[1] += 1;
    array_of_pointers[2] -= 1;
    if (array_of_pointers[1][0][3] != 33) {
        return 2; // fail
    }
    if (array_of_pointers[2][0][3] != 23) {
        return 3; // fail
    }

    return 0;
}