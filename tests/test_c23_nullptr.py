"""Tests for C23's `nullptr` and `nullptr_t` (C23 §6.2.5, §6.3.2.4,
§6.4.4.6, §6.5.10, §6.5.16, §6.5.17.2, §7.21.2) and for <stddef.h>.

Coverage:

  * `nullptr` is a keyword and a constant of type `nullptr_t`, which
    `typeof_unqual(nullptr)` names, and a name spelled `nullptr` is an
    error.
  * Each conversion the standard allows and each one it rejects, by
    assignment, initialization, argument, return, and cast, with the
    messages.
  * `==` and `!=` with a `nullptr_t` operand, the relational operators
    and arithmetic that reject one, and `?:`.
  * `sizeof(nullptr)` and `sizeof(nullptr_t)` are 2, and `nullptr`
    works in static initializers but isn't an integer constant.
  * `nullptr` reaches the TAC as the constant 0.
  * <stddef.h> includes without a diagnostic and gives `NULL`,
    `nullptr_t`, `size_t`, `ptrdiff_t`, and an `offsetof` that is a
    constant.
  * A program using all of these runs to the same result on the TAC
    interpreter and in the simulator, in both pipelines.

c6502 has no `_Generic`, so the type of `nullptr` is checked through
the type checker's `data_type` instead.

The tests of compile.py's messages, of <stddef.h>, and of the program
on the TAC interpreter are doctest cases in
cpp/tests/test_c23_nullptr.cpp.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


_PROGRAM = """\
#include <stddef.h>

struct node { int value; struct node *next; };

static struct node c = { 3, nullptr };
static struct node b = { 2, &c };
static struct node a = { 1, &b };
static int *gp = nullptr;
static nullptr_t gn;
static bool gb = nullptr;
int g;

int length(struct node *n) {
    int k = 0;
    for (; n != nullptr; n = n->next)
        k++;
    return k;
}

int *pick(int which) { return which ? &g : nullptr; }

nullptr_t none(void) { return nullptr; }

int is_null(void *p) { return p == nullptr; }

int main(void) {
    int *p = nullptr;
    nullptr_t n = 0;
    int r = 0;
    if (nullptr == 0 && 0 == nullptr) r += 1;
    if (nullptr == NULL && NULL == 0 && nullptr == (void *)0) r += 2;
    if (p == nullptr && !(nullptr != p)) r += 4;
    if (!nullptr && !(bool)nullptr && !n) r += 8;
    if (is_null(nullptr) && is_null(NULL) && !is_null(&g)) r += 16;
    if (pick(0) == nullptr && pick(1) == &g) r += 32;
    if (gp == nullptr && !gb && gn == nullptr && n == nullptr) r += 64;
    if (none() == nullptr && !none()) r += 128;
    if (sizeof(nullptr) == 2 && sizeof(nullptr_t) == 2) r += 256;
    p = r & 1 ? &g : nullptr;
    if (p == &g) r += 512;
    r += 1000 * length(&a) + 10000 * (int)offsetof(struct node, next);
    return r;
}
"""

# 1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 + 256 + 512, three nodes, and
# `next` 2 bytes into a node.
_EXPECTED = 1023 + 3000 + 20000


@needs_c6502cpp
class TestPrograms(unittest.TestCase):
    def test_asm_sim(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(_PROGRAM, optimize=optimize).run(
                    max_cycles=2_000_000,
                )
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int_signed(), _EXPECTED)


if __name__ == "__main__":
    unittest.main()
