int main(void) {
    // take the bitwise complement of the smallest int we can construct right now
    // (minimum representable int is actually -128, but we can't
    // construct it b/c the constant 128 is out of bounds for c6502's
    // 1-byte int)
    return ~-127;
}