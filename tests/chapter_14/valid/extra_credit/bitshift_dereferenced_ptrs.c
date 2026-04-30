// Test out bitshift operations on dereferenced pointers
// Same calculations as in tests/chapter_12/valid/extra_credit/bitwise_unsigned_shift.c
// but through pointers

unsigned long ui = 65535; // 2^16 - 1

unsigned long *get_ui_ptr(void){
    return &ui;
}

int shiftcount = 5;

int main(void) {

    // use dereferenced pointer as left operand
    if ((*get_ui_ptr() << 2ll) != 65532ul) {
        return 1;
    }

    if ((*get_ui_ptr() >> 2) != 16383) {
        return 2;
    }

    // also use dereferenced pointer as right operand
    int *shiftcount_ptr = &shiftcount;
    if ((1000ul >> *shiftcount_ptr) != 31) {
        return 3;
    }
    if ((1000ul << *shiftcount_ptr) != 32000) {
        return 4;
    }

    return 0;  // success
}