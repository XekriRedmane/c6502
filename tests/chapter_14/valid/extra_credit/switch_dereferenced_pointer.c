long long l = 65540ll;  // 2^16 + 4

long long *get_ptr(void) {
    return &l;
}
int main(void) {
    switch (*get_ptr()) {
        case 1:
            return 1;
        case 4: // l % 2^16
            return 2;
        case 65540ll:
            return 0; // success
        case 4294967280ULL:
            return 3;
        default:
            return 4;
    }
}