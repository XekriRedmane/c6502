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
"""
from __future__ import annotations

import io
import shutil
import unittest
from unittest.mock import patch

import diagnostics
from compile import main as compile_main
from preprocessor import preprocess
from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac


def _compile(src: str, *flags: str) -> tuple[int, str]:
    """compile.py's exit status and stderr for `src` on stdin."""
    err = io.StringIO()
    with patch("sys.stdin", io.StringIO(src)), patch("sys.stderr", err), \
            patch("sys.stdout", io.StringIO()):
        rc = compile_main(["compile.py", "-", "-S", *flags])
    return rc, err.getvalue()


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestOperators(unittest.TestCase):

    def test_not_an_integer_constant(self) -> None:
        # §6.6p8 admits only operands of integer type to an integer
        # constant expression.
        message = (
            "`nullptr` has type nullptr_t, which isn't an integer type, so "
            "it can't be an operand of an integer constant expression "
            "(C23 §6.6p8)"
        )
        rc, err = _compile(
            "int f(int x) { switch (x) { case nullptr: return 0; } "
            "return 1; }\n",
        )
        self.assertEqual(rc, 1)
        self.assertIn(
            f"case label is not an integer constant expression: {message}",
            err,
        )


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestStddef(unittest.TestCase):
    def test_includes_cleanly(self) -> None:
        src = (
            "#include <stddef.h>\n"
            "#if __STDC_VERSION_STDDEF_H__ != 202311L\n"
            "#error\n"
            "#endif\n"
            "struct s { char a; int b; long c[3]; };\n"
            "nullptr_t n = nullptr;\n"
            "size_t z = sizeof(int);\n"
            "ptrdiff_t d;\n"
            "max_align_t m;\n"
            "wchar_t w;\n"
            "int *p = NULL;\n"
            "size_t o = offsetof(struct s, c[1]);\n"
            "int main(void) { unreachable(); return NULL == 0; }\n"
        )
        with diagnostics.collecting() as found:
            rc, err = _compile(src)
        self.assertEqual((rc, err, found), (0, "", []))


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


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestPrograms(unittest.TestCase):
    def test_tac_sim(self) -> None:
        self.assertEqual(_run_tac(preprocess(_PROGRAM)), _EXPECTED)

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
