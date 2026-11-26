"""Byte-indexed accesses to static arrays of structs, and link-time
address arithmetic.

`c99_to_tac` scales an index into a small array object at byte width
(`_translate_subscript_address`), `static_indexed_access` turns `o[i].m`
on a static `o` into `IndexedLoad(o, i, offset=m)` / `IndexedSymbolStore`
(`LDA o+m,X`), and `label_offset_fold` makes `&o + k` one immediate,
`#<(o+k)`. Each program runs in both pipelines against values computed
here.
"""
from __future__ import annotations

import shutil
import unittest

from compile import _run_source
from sim.harness import build_sim

_HAVE_PCPP = shutil.which("pcpp") is not None

_RECS = """
struct rec { unsigned char t[5]; unsigned int w; unsigned char a; };
struct rec r[4] = {
    {{1, 2, 3, 4, 5}, 1000, 7},
    {{6, 7, 8, 9, 10}, 2000, 8},
    {{11, 12, 13, 14, 15}, 3000, 9},
    {{16, 17, 18, 19, 20}, 4000, 10},
};
"""


def _codegen(src: str) -> str:
    return _run_source("codegen", src, optimize=True)


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestStaticIndexedAccess(unittest.TestCase):

    def _check(self, src: str, expected: int) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                got = build_sim(src, optimize=optimize).run().return_int()
                self.assertEqual(got, expected & 0xFFFF)

    def test_member_reads(self) -> None:
        src = _RECS + """
        unsigned char a(unsigned char i) { return r[i].a; }
        unsigned int w(unsigned char i) { return r[i].w; }
        unsigned char t(unsigned char i, unsigned char j) {
            return r[i].t[j];
        }
        int main(void) {
            return a(3) + a(0) + w(2) + t(1, 4) + t(3, 0);
        }
        """
        self._check(src, 10 + 7 + 3000 + 10 + 16)
        body = _codegen(src).split("\na:\n")[1].split("\nw:\n")[0]
        self.assertIn("r+7,X", body)
        self.assertNotIn("(", body.replace("#<", "").replace("#>", ""))

    def test_member_writes(self) -> None:
        src = _RECS + """
        void put(unsigned char i, unsigned char v, unsigned int x) {
            r[i].a = v;
            r[i].w = x;
        }
        int main(void) {
            put(2, 99, 12345);
            put(0, 1, 2);
            return r[2].a + r[2].w + r[0].a + r[0].w + r[1].a;
        }
        """
        self._check(src, 99 + 12345 + 1 + 2 + 8)

    def test_address_of_member(self) -> None:
        src = _RECS + """
        unsigned char deref(const unsigned char *p) { return *p; }
        int main(void) {
            unsigned char s = 0;
            for (unsigned char i = 0; i < 4; i++) s += deref(&r[i].a);
            return s + deref(&r[2].t[3]);
        }
        """
        self._check(src, 7 + 8 + 9 + 10 + 14)

    def test_constant_address_folds(self) -> None:
        src = _RECS + """
        unsigned char deref(const unsigned char *p) { return *p; }
        int main(void) { return deref(&r[2].a); }
        """
        self._check(src, 9)
        self.assertIn("#<(r+23)", _codegen(src))

    def test_index_types_and_sizes(self) -> None:
        # An int index into a small array, and an array too big for
        # byte offsets (the 16-bit path).
        src = """
        unsigned int small[10] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90};
        unsigned int big[200];
        unsigned int f(int i) { return small[i]; }
        unsigned int g(unsigned char i) { return big[i]; }
        int main(void) {
            big[150] = 7; big[3] = 5;
            return f(9) + f(0) + g(150) + g(3);
        }
        """
        self._check(src, 90 + 0 + 7 + 5)

    def test_constant_added_to_index(self) -> None:
        # `t[x + 1]` reads `t+1,X`, and `w[x + 1]` reads `w+2,X` and
        # `w+3,X`. `(uint8_t)(x + 1)` wraps at 255, so `u` keeps the
        # add.
        src = """
        unsigned char t[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        unsigned int w[4] = {100, 200, 300, 400};
        unsigned char s[4];
        unsigned char f(unsigned char x) { return t[x + 1] + t[2 + x]; }
        unsigned int g(unsigned char x) { return w[x + 1]; }
        void h(unsigned char x, unsigned char v) { s[x + 1] = v; }
        unsigned char u(unsigned char x) {
            return t[(unsigned char)(x + 1)];
        }
        int main(void) {
            h(2, 9);
            return f(0) + f(5) + g(2) + s[3] + u(255) + u(6);
        }
        """
        self._check(src, (2 + 3) + (7 + 8) + 400 + 9 + 1 + 8)
        asm = _codegen(src)
        f = asm.split("\nf:\n")[1].split("\ng:\n")[0]
        self.assertIn("t+1,X", f)
        self.assertIn("t+2,", f)
        g = asm.split("\ng:\n")[1].split("\nh:\n")[0]
        self.assertIn("w+2,X", g)
        self.assertIn("w+3,X", g)
        h = asm.split("\nh:\n")[1].split("\nu:\n")[0]
        self.assertIn("s+1,X", h)
        u = asm.split("\nu:\n")[1].split("\nmain:\n")[0]
        self.assertNotIn("t+1,", u)


if __name__ == "__main__":
    unittest.main()
