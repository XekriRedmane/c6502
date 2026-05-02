// Bitwise operations with structure members
//
// c6502 adaptation:
//   * `unsigned long l` widened to `unsigned long long l` so
//     2147483650 (= 2^31 + 2) fits — c6502's `unsigned long` is
//     2 bytes (max 65535) and would truncate to 2.
//   * `unsigned int u` widened to `unsigned long long u` so
//     100000 fits — c6502's `unsigned int` is 1 byte (max 255)
//     and would truncate to 160.
//   * Check 5's shift `i.b << o.bar` casts `i.b` to `long long`:
//     the promoted-left result type per §6.5.7.3 is then 4-byte
//     instead of 1-byte, so 97 << 12 = 397312 doesn't overflow
//     the shift width or wrap.

struct inner {
    char b;
    unsigned long long u;
};

struct outer {
    unsigned long long l;
    struct inner *in_ptr;
    int bar;
    struct inner in;
};

int main(void) {
    struct inner i = {'a', 100000u};
    struct outer o = {2147483650ul, &i, 100, {-80, 4294967295U}};

    if ((i.b | o.l) != 2147483747ul) {
        return 1;  // fail
    }

    if ((o.bar ^ i.u) != 100036u) {
        return 2;  // fail
    }

    if ((o.in_ptr->b & o.in.b) != 32) {
        return 3;  // fail
    }

    if ((o.l >> 26) != 32ul) {
        return 4;  // fail
    }

    o.bar = 12;
    if (((long long)i.b << o.bar) != 397312) {
        return 5;
    }

    return 0;
}
