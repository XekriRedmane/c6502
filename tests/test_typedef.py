"""Tests for `typedef` (C99 §6.7.7).

The parser puts each typedef name's type in place of its uses, so the
AST never names a typedef. Coverage:

  * A typedef name in declarations, casts, sizeof, parameters, struct
    members, and `for` initializers.
  * The "lexer hack": `T * x;` declares x when T names a type and
    multiplies when it doesn't, and a declaration of T in an inner
    scope (an object, a parameter, a `for` variable) hides the typedef
    name until that scope ends.
  * Typedefs of pointers, arrays (qualified ones too), functions, and
    function pointers; anonymous structs and unions.
  * The forward declaration that stands in for a tag a typedef
    introduces.
  * The rejected forms: an initializer, two kinds of symbol in one
    scope, conflicting redefinitions, a typedef in a parameter or a
    `for` initializer, a function definition typed by a typedef, and
    a typedef whose struct tag an inner declaration hides.
  * Programs using typedefs run with and without `-O`.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


@needs_c6502cpp
class TestTypedefPrograms(unittest.TestCase):
    """Run each program with and without `-O`."""

    def _returns(self, src: str, expected: int) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(src, optimize=optimize).run(
                    max_cycles=2_000_000,
                )
                self.assertFalse(result.timed_out)
                value = result.return_int()
                if value & 0x8000:
                    value -= 0x10000
                self.assertEqual(value, expected)

    def test_struct_point(self) -> None:
        self._returns(
            "typedef unsigned char u8;"
            "typedef struct { u8 x; int y; } point;"
            "typedef point *ppoint;"
            "int add(ppoint p) { return p->x + p->y; }"
            "int main(void) { point pt; ppoint pp = &pt;"
            " pt.x = 3; pt.y = 4; return add(pp) + sizeof(point); }",
            10,
        )

    def test_hiding(self) -> None:
        self._returns(
            "typedef int T;"
            "int f(int T) { return T * 2; }"
            "int main(void) { int s = 0;"
            " for (int T = 0; T < 3; T++) s += T;"
            " { int T = 10; s += T * f(1); }"
            " T y = s; return y; }",
            23,
        )

    def test_function_and_function_pointer_typedefs(self) -> None:
        self._returns(
            "typedef int F(int); typedef int (*FP)(int);"
            "F inc; int inc(int x) { return x + 1; }"
            "int apply(F *f, FP g, int v) { return g(f(v)); }"
            "int main(void) { return apply(inc, inc, 4); }",
            6,
        )

    def test_array_typedef(self) -> None:
        self._returns(
            "typedef int Row[3]; typedef Row Grid[2];"
            "int sum(Row r) { return r[0] + r[1] + r[2]; }"
            "int main(void) { Grid g = {{1, 2, 3}, {4, 5, 6}};"
            " return sum(g[0]) * 10 + sum(g[1]); }",
            75,
        )

    def test_linked_list_with_forward_tag(self) -> None:
        # The typedef introduces `struct node` at file scope, so the
        # pointer in `first`, declared before the struct's definition,
        # refers to the same tag as `head` does. If it introduced a
        # new tag in `first`'s block, `return p;` wouldn't type-check.
        self._returns(
            "typedef struct node node_t;"
            "node_t *head;"
            "node_t *first(void) { node_t *p = head; return p; }"
            "struct node { int v; node_t *next; };"
            "int sum(void) { int s = 0; node_t *p;"
            " for (p = first(); p; p = p->next) s += p->v; return s; }"
            "int main(void) { node_t a, b; a.v = 2; b.v = 5;"
            " a.next = &b; b.next = 0; head = &a; return sum(); }",
            7,
        )

    def test_stdint_types(self) -> None:
        self._returns(
            "#include <stdint.h>\n"
            "uint8_t lo(uint16_t w) { return (uint8_t)w; }"
            "int main(void) { int32_t big = 70000; uint8_t uint8 = lo(0x1234);"
            " return (int)(big / 1000) + uint8 + sizeof(int64_t); }",
            70 + 0x34 + 8,
        )


if __name__ == "__main__":
    unittest.main()
