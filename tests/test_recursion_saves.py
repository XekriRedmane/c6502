"""Tests for `[[c6502::save_hardware_stack]]`: a recursive
function whose recursive calls are marked gets a private zero-page
pool, and saves what's live across each marked call on the hardware
stack (`cpp/src/passes/recursion_saves.h`).

The tests that called the Python functions directly are doctest cases of
the C++ port now, each named for the test it came from
(docs/cpp_port.md, step 8, group G6), and those that compile a source
and run no simulator are in cpp/tests/test_recursion_saves.cpp."""
from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac

# The attribute, before a statement or declaration. In the programs
# below, `SAVE ` stands where it goes.
_SAVE = "[[c6502::save_hardware_stack]] "


_DESCEND = """
unsigned char level;
unsigned char deepest;
unsigned char remaining;

void descend(void) {
    unsigned char here = level;
    if (here > deepest) deepest = here;
    if (remaining != 0) {
        remaining = remaining - 1;
        level = here + 1;
        SAVE descend();
        level = here;
    }
}

int main(void) {
    remaining = 5;
    descend();
    return deepest * 10 + level;
}
"""


_PROGRAMS = {
    "globals": _DESCEND,
    "locals and parameters": """
unsigned char depth;
unsigned char out[16];
unsigned char n;
void walk(unsigned char k) {
    unsigned char a = k * 3;
    unsigned char b = depth;
    unsigned int w = k * 100;
    out[n++] = a;
    if (k > 0) {
        depth = depth + 1;
        SAVE walk(k - 1);
        depth = b;
    }
    out[n++] = a + (unsigned char)w + b;
}
int fact(int x) {
    int y = x;
    if (x <= 1) return 1;
    SAVE y = y * fact(x - 1);
    return y;
}
int main(void) {
    walk(4);
    int s = 0;
    for (unsigned char i = 0; i < n; i++) s = s * 3 + out[i];
    return (s ^ fact(6)) & 0x7FFF;
}
""",
    "return and declaration": """
unsigned char weights[8] = {3, 1, 4, 1, 5, 9, 2, 6};
unsigned int sum_from(unsigned char i) {
    if (i >= 8) return 0;
    unsigned char w = weights[i];
    SAVE return w + sum_from(i + 1);
}
int even(unsigned char n);
int odd(unsigned char n) {
    if (n == 0) return 0;
    unsigned char k = n * 2;
    SAVE int r = even(n - 1);
    return r + k;
}
int even(unsigned char n) {
    if (n == 0) return 1;
    unsigned char k = n + 7;
    SAVE int r = odd(n - 1);
    return r * 3 + k;
}
int main(void) { return (int)sum_from(0) * 100 + even(6); }
""",
    "in a loop": """
unsigned int count_paths(unsigned char n) {
    unsigned int total = 0;
    if (n < 2) return 1;
    for (unsigned char k = 1; k <= 2; k++) {
        SAVE total += count_paths(n - k);
    }
    return total;
}
int main(void) { return count_paths(9); }
""",
    "address taken": """
void bump(unsigned char *p) { *p += 1; }
unsigned char walk2(unsigned char n) {
    unsigned char local = n;
    unsigned char keep = n * 3;
    bump(&local);
    if (n == 0) return local;
    SAVE unsigned char r = walk2(n - 1);
    return r + local + keep;
}
int main(void) { return walk2(5); }
""",
    "long": """
long acc(unsigned char n) {
    long big = (long)n * 70000;
    if (n == 0) return 1;
    SAVE long r = acc(n - 1);
    return r + big;
}
int main(void) { return (int)(acc(5) & 0x7FFF); }
""",
    "carry flag": """
#include <stdbool.h>
[[c6502::return_bool_in_carry]]
bool any_bit(unsigned char n, unsigned char mask) {
    unsigned char h = n & mask;
    if (n == 0) return 0;
    SAVE bool r = any_bit(n >> 1, mask);
    return r || h == 3;
}
int main(void) {
    int s = 0;
    for (unsigned char i = 0; i < 40; i++) s = s * 2 + any_bit(i, 3);
    return s & 0x7FFF;
}
""",
    "zp_abi caller": """
int rec(int d) {
    int k = d * 3;
    if (d <= 0) return 0;
    SAVE int r = rec(d - 1);
    return r + 1 + (k & 1);
}
int f0(unsigned char d [[c6502::in_a]], int p0) {
    int l = rec(3);
    return p0 + l + d;
}
int main(void) { return f0(2, 4); }
""",
    "unmarked call on the cycle": """
int pong(int n);
int ping(int n) {
    int a = n * 2;
    if (n <= 0) return 0;
    SAVE int r = pong(n - 1);
    return r + a;
}
int pong(int n) {
    int b = n + 5;
    if (n <= 0) return 1;
    int r = ping(n - 1);
    return r * 2 + b;
}
int main(void) { return ping(5); }
""",
}


class TestPrograms(unittest.TestCase):

    def test_programs(self) -> None:
        for name, src in _PROGRAMS.items():
            src = src.replace("SAVE ", _SAVE)
            expected = _run_tac(src) & 0xFFFF
            for optimize in (False, True):
                with self.subTest(name, optimize=optimize):
                    result = build_sim(src, optimize=optimize).run(
                        max_cycles=5_000_000,
                    )
                    self.assertFalse(result.timed_out)
                    self.assertEqual(
                        result.return_int_signed() & 0xFFFF, expected,
                    )


if __name__ == "__main__":
    unittest.main()
