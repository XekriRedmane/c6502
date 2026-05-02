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
    // c6502 adaptation: pick literals whose low byte matches the
    // -22 / -46 the assertions check. Upstream uses 8-byte literals
    // 9876543210l (low byte 0xEA = -22) and 1234567890l (low byte
    // 0xD2 = -46). Both exceed c6502's 4-byte long long, so we
    // pick small values with the same low bytes: 234 = 0xEA, 210
    // = 0xD2.
    union has_array union1 = {234l};
    union has_array union2 = {210l};

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