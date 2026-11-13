"""Arrays sized by their initializer (C99 §6.7.8.22): `int t[] = {1, 2,
3};` has three elements, and `char s[] = "abc";` four, at file scope and
block scope, static or automatic. The size is the number of items in
the braces, or the string's length plus its null.

Covers the type each declaration gets, `sizeof`, an `extern` declaration
completed by a later definition, the errors that remain (no
initializer, an empty list, a scalar initializer), and runs in both
pipelines.
"""
from __future__ import annotations

import shutil
import unittest

from passes.type_checking import TypeCheckError
from sim.harness import build_sim

_HAVE_PCPP = shutil.which("pcpp") is not None


def _types(src: str) -> dict:
    import sim.harness as H
    from compile import _resolved
    _, symbols, _ = H.type_check_program(_resolved(H.preprocess(src)))
    return {name: sym.type for name, sym in symbols.items()}


def _run(src: str, optimize: bool) -> int:
    return build_sim(src, optimize=optimize).run().return_int()


@unittest.skipUnless(_HAVE_PCPP, "pcpp CLI not available")
class TestSizes(unittest.TestCase):

    def test_sizes(self) -> None:
        src = """
            int t[] = {1, 2, 3};
            char s[] = "abc";
            const unsigned char k[] = {9, 8};
            char b[] = {"xy"};
            int m[][2] = {{1, 2}, {3, 4}, {5, 6}};
            int main(void) {
                static int u[] = {4, 5, 6, 7};
                long v[] = {1, 2};
                return sizeof t + sizeof s + sizeof k + sizeof b
                    + sizeof m + sizeof u + sizeof v;
            }
        """
        types = _types(src)
        self.assertEqual(types["t"].size, 3)
        self.assertEqual(types["s"].size, 4)
        self.assertEqual(types["k"].size, 2)
        self.assertEqual(types["b"].size, 3)
        self.assertEqual(types["m"].size, 3)
        sizes = {n: t.size for n, t in types.items() if n.endswith(".u")
                 or n.endswith(".v")}
        self.assertEqual(sorted(sizes.values()), [2, 4])
        # 6 + 4 + 2 + 3 + 12 + 8 + 8
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(src, optimize), 43)

    def test_extern_completed_by_definition(self) -> None:
        src = """
            extern int t[];
            int first(void) { return t[0]; }
            int t[] = {7, 8, 9};
            int main(void) { return first() + t[2] + sizeof t; }
        """
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(src, optimize), 7 + 9 + 6)

    def test_contents(self) -> None:
        src = """
            unsigned char t[] = {10, 20, 30, 40};
            const char s[] = "hello";
            int main(void) {
                unsigned char local[] = {1, 2, 3};
                int sum = 0;
                for (unsigned char i = 0; i < sizeof t; i++) sum += t[i];
                for (unsigned char i = 0; i < sizeof local; i++)
                    sum += local[i];
                return sum + s[4] + s[5];
            }
        """
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                self.assertEqual(_run(src, optimize), 100 + 6 + ord("o"))

    def test_errors(self) -> None:
        for src in [
            "int t[];\nint main(void) { return 0; }\n",
            "int main(void) { int t[]; return 0; }\n",
            "int t[] = 5;\nint main(void) { return 0; }\n",
            "int main(void) { static int t[] = {}; return 0; }\n",
        ]:
            with self.subTest(src=src):
                with self.assertRaises(Exception):
                    _types(src)


if __name__ == "__main__":
    unittest.main()
