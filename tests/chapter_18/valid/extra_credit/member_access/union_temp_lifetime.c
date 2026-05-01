// We can implicitly get the address of a union with temporary lifetime
// (and subscript it)

struct has_char_array {
    char arr[8];
};

union has_array {
    long l;
    struct has_char_array s;
};

int get_flag(void) {
    static int flag = 0;
    flag = !flag;
    return flag;
}

int main(void) {
    union has_array union1 = {1234567890l};
    union has_array union2 = {987654321l};

    // first access member in union1
    if ((get_flag() ? union1 : union2).s.arr[0] != -22) {
        return 1; // fail
    }

    // then access member in union2
    if ((get_flag() ? union1 : union2).s.arr[0] != -46) {
        return 2; // fail
    }

    return 0; // success
}