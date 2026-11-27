"""Tests for `cpp/src/passes/optimization/direct_member_access.h`, which turns
an access through `&obj + k` into a direct access to byte `k` of
`obj`: struct members, and array elements at a constant index, of
statics and of automatic aggregates.

Each program runs on the unoptimized TAC and through both pipelines,
and all three must agree with the expected value. The shape tests in
cpp/tests/test_direct_member_access_source.cpp check that a member
store to a global is a plain `STA g+k`, and that a local struct whose
address no longer escapes gets zero-page slots instead of a pointer.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.test_sim_chapters import _run_program
from tests.cpp_build import needs_c6502cpp

_S = "struct s { unsigned char a; int b; unsigned char c; };\n"


def _tac_main(src: str) -> int:
    return _run_program(src)


@needs_c6502cpp
class TestDirectMemberAccessSim(unittest.TestCase):

    def _check(self, src: str, expected: int) -> None:
        self.assertEqual(_tac_main(src), expected)
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got, expected)

    def test_global_and_local_members(self):
        self._check(_S + (
            "struct s g;\n"
            "unsigned char h(unsigned char x) {\n"
            "    struct s r;\n"
            "    g.c = x;\n"
            "    g.b++;\n"
            "    g.b += 300;\n"
            "    r.a = x;\n"
            "    r.c = g.a + 1;\n"
            "    r.b = g.b;\n"
            "    return r.a + r.c + (r.b == 301);\n"
            "}\n"
            "int main(void) { g.a = 4; return h(3) * 10 + g.c; }\n"
        ), (3 + 5 + 1) * 10 + 3)

    def test_copies_in_and_out(self):
        # A struct copied into a local, read by member, written by
        # member, and copied out again.
        self._check(_S + (
            "struct s g, out;\n"
            "int main(void) {\n"
            "    struct s a, b;\n"
            "    g.a = 1; g.b = 1000; g.c = 3;\n"
            "    a = g;\n"
            "    b = a;\n"
            "    b.b = a.b + 1;\n"
            "    out = b;\n"
            "    return out.a + out.b + out.c + a.b;\n"
            "}\n"
        ), 1 + 1001 + 3 + 1000)

    def test_nested_struct_and_array_members(self):
        self._check(
            "struct in { unsigned char x; int y; };\n"
            "struct out { int k; struct in i; unsigned char t[3]; };\n"
            "struct out g;\n"
            "int main(void) {\n"
            "    struct out o;\n"
            "    unsigned char a[4];\n"
            "    o.k = 7; o.i.x = 8; o.i.y = 900; o.t[2] = 5; o.t[0] = 6;\n"
            "    a[0] = 1; a[3] = 2;\n"
            "    g.i.y = o.i.y + a[3];\n"
            "    g.t[1] = o.t[2] + a[0];\n"
            "    return o.k + o.i.x + g.i.y + g.t[1] + o.t[0];\n"
            "}\n",
            7 + 8 + 902 + 6 + 6,
        )

    def test_address_escapes_after_member_writes(self):
        # `r` is written by member, then passed by pointer, so its
        # bytes have to be in memory where `sum` reads them.
        self._check(_S + (
            "int sum(struct s *p) { return p->a + p->b + p->c; }\n"
            "int main(void) {\n"
            "    struct s r;\n"
            "    r.a = 2; r.b = 40; r.c = 5;\n"
            "    int t = sum(&r);\n"
            "    r.b = 100;\n"
            "    return t + sum(&r);\n"
            "}\n"
        ), 47 + 107)

    def test_union(self):
        self._check(
            "union u { unsigned int w; unsigned char b[2]; };\n"
            "int main(void) {\n"
            "    union u x;\n"
            "    x.w = 0x1234;\n"
            "    x.b[0] = 0x56;\n"
            "    return x.w;\n"
            "}\n",
            0x1256,
        )

    def test_member_in_a_loop(self):
        self._check(_S + (
            "int main(void) {\n"
            "    struct s r;\n"
            "    r.b = 0; r.c = 0;\n"
            "    for (unsigned char i = 0; i < 10; i++) {\n"
            "        r.b += i;\n"
            "        if (i & 1) r.c++;\n"
            "    }\n"
            "    return r.b * 10 + r.c;\n"
            "}\n"
        ), 45 * 10 + 5)


if __name__ == "__main__":
    unittest.main()
