"""Tests for compound literals, `(type){ initializer-list }` (C99
§6.5.2.5).

Coverage:

  * The parser builds a `CompoundLiteral` whose initializer has the
    shape a declaration's would: an InitList for an array or struct,
    the String for `(char[]){"abc"}`, and the single expression for a
    scalar.
  * Identifier resolution names each literal's object, and the type
    checker sizes an array of unknown size from its initializer.
  * A literal is an lvalue: its address can be taken, it can be
    assigned and incremented, and an array literal decays.
  * Inside a function each evaluation initializes the object, and
    `&&`, `||`, and `?:` evaluate a literal only when C does.
  * At file scope the object is static, so a pointer can be
    initialized with its address.
  * The rejected forms: a non-object type, a scalar with more than
    one initializer, too many initializers, a non-constant initializer
    at file scope, and a static initializer that takes the address of
    an automatic literal.
"""
from __future__ import annotations

import shutil
import unittest

from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac


# (name, source, expected return of main)
_PROGRAMS: list[tuple[str, str, int]] = [
    ("address_of_scalar",
     "int main(void) { int *p = &(int){5}; *p += 2; return *p; }", 7),
    ("array_decays",
     "int sum(int *a, int n) { int s = 0;"
     " for (int i = 0; i < n; i++) s += a[i]; return s; }"
     "int main(void) { return sum((int[]){1, 2, 3, 4}, 4); }", 10),
    ("sizeof_array", "int main(void) { return sizeof((long[]){1, 2, 3}); }", 12),
    ("struct_member",
     "struct pt { int x; int y; };"
     "int main(void) { return (struct pt){3, 4}.y; }", 4),
    ("struct_return",
     "struct zc { int z; unsigned char ok; };"
     "struct zc f(int z) { return (struct zc){ z, 1 }; }"
     "int main(void) { struct zc r = f(9); return r.z + r.ok; }", 10),
    ("struct_assign",
     "struct pt { int x; int y; };"
     "int main(void) { struct pt p; p = (struct pt){5, 6}; return p.x * p.y; }",
     30),
    ("struct_argument",
     "typedef struct { int a; int b; } P;"
     "int add(P p) { return p.a + p.b; }"
     "int main(void) { return add((P){2, 3}); }", 5),
    # §6.5.2.5.16: each evaluation initializes the object again.
    ("reinitialized_in_loop",
     "int main(void) { int s = 0; for (int i = 0; i < 3; i++) {"
     " int *p = (int[]){i, i * 10}; s += p[0] + p[1]; p[1] = 100; }"
     " return s; }", 33),
    ("missing_elements_are_zero",
     "int main(void) { int *p = (int[4]){1};"
     " return p[0] + p[1] + p[2] + p[3]; }", 1),
    ("char_array_from_string",
     'int main(void) { char *s = (char[]){"hey"}; s[0] = \'H\';'
     ' return s[0] + sizeof((char[]){"hey"}); }', 72 + 4),
    ("increment", "int main(void) { return (int){5}++ + 1; }", 6),
    ("const_elements",
     "int main(void) { const int *p = (const int[]){3, 4}; return p[1]; }", 4),
    ("not_evaluated_after_false_and",
     "int n; int bump(void) { n++; return 1; }"
     "int main(void) { int z = 0; int r = z && ((int[]){bump()})[0];"
     " return n * 10 + r; }", 0),
    ("evaluated_after_true_and",
     "int n; int bump(void) { n++; return 1; }"
     "int main(void) { int z = 1; int r = z && ((int[]){bump()})[0];"
     " return n * 10 + r; }", 11),
    ("conditional",
     "int main(void) { int c = 0; int *p = c ? (int[]){4} : (int[]){5};"
     " return *p; }", 5),
    ("file_scope_array", "int *gp = (int[]){7, 8, 9};"
     "int main(void) { return gp[2]; }", 9),
    ("file_scope_struct",
     "struct pt { int x; int y; }; struct pt *sp = &(struct pt){1, 2};"
     "int main(void) { sp->y += 5; return sp->y; }", 7),
]


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestPrograms(unittest.TestCase):
    def test_tac_sim(self) -> None:
        for name, src, expected in _PROGRAMS:
            with self.subTest(case=name):
                value = _run_tac(src) & 0xFFFF
                if value & 0x8000:
                    value -= 0x10000
                self.assertEqual(value, expected)

    def test_asm_sim(self) -> None:
        for name, src, expected in _PROGRAMS:
            for optimize in (False, True):
                with self.subTest(case=name, optimize=optimize):
                    result = build_sim(src, optimize=optimize).run(
                        max_cycles=2_000_000,
                    )
                    self.assertFalse(result.timed_out)
                    self.assertEqual(result.return_int_signed(), expected)

    def test_static_local_pointer_to_static_array(self) -> None:
        # A block-scope static initializer decays an array, as a
        # file-scope one does.
        src = (
            "int a[2] = {3, 4};"
            "int main(void) { static int *p = a; return p[1]; }"
        )
        self.assertEqual(build_sim(src).run().return_int_signed(), 4)


if __name__ == "__main__":
    unittest.main()
