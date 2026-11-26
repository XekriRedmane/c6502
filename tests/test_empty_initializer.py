"""Tests for C23's empty initializer `{}` and for a scalar's braced
initializer `{e}` (C23 §6.7.11, C99 §6.7.8p11).

Coverage:

  * The grammar takes `= {}`, a nested `{}`, and `(T){}`, and the
    parser gives an InitList with no items.
  * The type checker gives every scalar kind its zero (integer,
    pointer, floating, `bool`, `_BitInt`, `nullptr_t`), takes `{}`
    for arrays, structs, unions, nested subobjects, and compound
    literals, and rejects `{}` for an array of unknown size.
  * A braced scalar, `{5}` or `{5,}`, gives its expression at file
    scope, at block scope, as a static, as a member or element, and in
    a compound literal. Two expressions, or a second pair of braces,
    are rejected.
  * A static object initialized by `{}` is all zero bytes and goes in
    `.bss`. An automatic aggregate's run of zero bytes is cleared by
    stores when it's short and by a loop when it's long.
  * A program checks every byte of each kind of object initialized
    by `{}` (or the braced scalar's value), in each storage class, in
    both pipelines and on the TAC interpreter. Before each function of
    automatic objects it fills the bytes those objects take with
    0xA5, and a control run without the initializers finds them.
"""
from __future__ import annotations

import io
import unittest
from unittest.mock import patch

import tac_ast
from compile import main as compile_main
from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac
from tests.cpp_build import needs_c6502cpp


def _compile(src: str, *flags: str) -> str:
    """compile.py's `-S` output for `src`."""
    out = io.StringIO()
    with patch("sys.stdin", io.StringIO(src)), patch("sys.stdout", out):
        rc = compile_main(["compile.py", "-", "-S", "-o", "-", *flags])
    assert rc == 0
    return out.getvalue()


def _tac_function(src: str, name: str) -> tac_ast.Function:
    """The function `name` of the TAC that `compile.py --tac` prints for
    `src`, read back into `tac_ast` objects. Its `pretty` text is the
    constructors' calls with their fields as keywords, so Python can
    evaluate it."""
    from compile import _run_source
    tac = eval(_run_source("tac", src), vars(tac_ast))
    for tl in tac.top_level:
        if isinstance(tl, tac_ast.Function) and tl.name == name:
            return tl
    raise KeyError(name)


_STRUCTS = """\
struct s { int a; char b[3]; long c; };
union u { char c; long l; };
"""


@needs_c6502cpp
class TestCode(unittest.TestCase):
    def test_static_goes_in_bss(self) -> None:
        asm = _compile(_STRUCTS + (
            "struct s v = {}; static union u un = {};"
            "int f(void) { static int a[10] = {}; return a[1] + v.a + un.c; }"
        ), "-O")
        for name, size in (("v", 9), ("un", 4)):
            with self.subTest(name=name):
                self.assertIn(f"\n{name}:\n   DS.B  {size}\n", asm)
                self.assertIn(f"item {name} section=.bss", asm)
        self.assertRegex(asm, r"\n@\d+\.a:\n   DS\.B  20\n")


    def test_large_automatic_struct_is_a_loop(self) -> None:
        src = (
            _STRUCTS + "struct big { struct s s[4]; union u u; };"
            "void use(struct big *p);"
            "void f(void) { struct big v = {}; use(&v); }"
        )
        fn = _tac_function(src, "f")
        labels = [i.name for i in fn.instructions
                  if isinstance(i, tac_ast.Label)]
        self.assertEqual(len(labels), 1)
        self.assertTrue(labels[0].startswith(".clear@"))
        stores = [i for i in fn.instructions if isinstance(i, tac_ast.Store)]
        self.assertEqual(len(stores), 1)
        # Under -O the object is in the function's pool and the loop
        # indexes it with Y: 40 bytes, counted down. The 0 is loaded
        # before the loop, and the BNE reads the flags DEY sets
        # (cpp/src/passes/loop_constant_a.h).
        asm = _compile(src, "-O")
        self.assertIn(
            "   LDY   #$28\n   LDA   #$00\n.clear@0:\n   DEY\n"
            "   STA   __local_f__v,Y\n   BNE   .clear@0\n",
            asm,
        )


# Bytes of `struct s`: a (2), b (3), c (4).
_PROGRAM = """\
struct s { int a; char b[3]; long c; };
union u { char c; long l; double d; };
struct n { struct s s; union u u; int k[2][3]; float f; };
typedef typeof_unqual(nullptr) nullptr_t;

static int failed;
static int checks;

/* Notes the number of the first check whose bytes differ from `want`
   (all zero when `want` is null). */
static void expect(const void *object, unsigned n,
                   const unsigned char *want) {
    const unsigned char *p = object;
    ++checks;
    for (unsigned i = 0; i < n; i++) {
        unsigned char w = want ? want[i] : 0;
        if (p[i] != w) {
            if (!failed)
                failed = checks;
            return;
        }
    }
}

#define ZERO(x) expect(&(x), sizeof(x), 0)

static const unsigned char s_braced[9] = { 3, 0, 0, 0, 0, 4, 0, 0, 0 };
static const unsigned char k_partial[12] = {
    0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0 };
static const unsigned char five[2] = { 5, 0 };
static const unsigned char minus7[4] = { 0xF9, 0xFF, 0xFF, 0xFF };

/* Leaves 0xA5 in the bytes the next call's locals will take. */
static void dirty(void) {
    volatile unsigned char junk[110];
    for (int i = 0; i < 110; i++)
        junk[i] = 0xA5;
}

struct s g_s = {};
union u g_u = {};
struct n g_n = {};
int g_i = {};
int *g_p = {};
double g_d = {};
bool g_b = {};
_BitInt(12) g_w = {};
nullptr_t g_np = {};
int g_a[5] = {};
struct s g_braced = { {3}, {}, {4} };
int g_k[2][3] = { {}, {1, 2} };
int g_five = {5};
long g_minus7 = {-7,};
struct s *g_ps = &(struct s){};
int *g_pa = (int[4]){};

static void file_scope(void) {
    ZERO(g_s); ZERO(g_u); ZERO(g_n); ZERO(g_i); ZERO(g_p); ZERO(g_d);
    ZERO(g_b); ZERO(g_w); ZERO(g_np); ZERO(g_a);
    expect(&g_braced, 9, s_braced);
    expect(g_k, 12, k_partial);
    expect(&g_five, 2, five);
    expect(&g_minus7, 4, minus7);
    expect(g_ps, sizeof(struct s), 0);
    expect(g_pa, 4 * sizeof(int), 0);
}

static void block_statics(void) {
    static struct s s = {};
    static union u u = {};
    static struct n n = {};
    static int i = {};
    static int *p = {};
    static float f = {};
    static bool b = {};
    static _BitInt(12) w = {};
    static char c[7] = {};
    static struct s braced = { {3}, {}, {4} };
    static int five_ = {5};
    ZERO(s); ZERO(u); ZERO(n); ZERO(i); ZERO(p); ZERO(f); ZERO(b);
    ZERO(w); ZERO(c);
    expect(&braced, 9, s_braced);
    expect(&five_, 2, five);
}

static void automatic_aggregates(void) {
    struct s s = {};
    union u u = {};
    struct n n = {};
    int a[5] = {};
    char text[12] = {};
    struct s braced = { {3}, {}, {4} };
    int k[2][3] = { {}, {1, 2} };
    ZERO(s); ZERO(u); ZERO(n); ZERO(a); ZERO(text);
    expect(&braced, 9, s_braced);
    expect(k, 12, k_partial);
}

static void automatic_scalars(void) {
    int i = {};
    int *p = {};
    float f = {};
    double d = {};
    bool b = {};
    _BitInt(12) w = {};
    nullptr_t np = {};
    char c = {};
    long long ll = {};
    int five_ = {5};
    long m7 = {-7,};
    struct s *ps = &(struct s){};
    int *pa = (int[4]){};
    int ci = (int){};
    ZERO(i); ZERO(p); ZERO(f); ZERO(d); ZERO(b); ZERO(w); ZERO(np);
    ZERO(c); ZERO(ll);
    expect(&five_, 2, five);
    expect(&m7, 4, minus7);
    expect(ps, sizeof(struct s), 0);
    expect(pa, 4 * sizeof(int), 0);
    ZERO(ci);
    for (int j = {}; j < 2; j++) {
        union u again = {};
        ZERO(again);
        again.l = -1;
    }
}

int main(void) {
    file_scope();
    dirty();
    block_statics();
    dirty();
    automatic_aggregates();
    dirty();
    automatic_scalars();
    return failed ? failed : 1000 + checks;
}
"""

# Every check passes: 16 at file scope, 11 for the block statics, 7
# for the automatic aggregates, and 14 plus 2 passes of the loop for
# the automatic scalars.
_EXPECTED = 1000 + 16 + 11 + 7 + 16


# An automatic object over 256 bytes, which the loop clears in two
# runs. The unoptimized pipeline can't address a frame that big.
_LARGE = """\
static void dirty(void) {
    volatile unsigned char junk[300];
    for (int i = 0; i < 300; i++)
        junk[i] = 0xA5;
}

static int check(void) {
    unsigned char big[300] = {};
    for (int i = 0; i < 300; i++)
        if (big[i])
            return i + 1;
    return 0;
}

int main(void) {
    dirty();
    return check();
}
"""


@needs_c6502cpp
class TestPrograms(unittest.TestCase):
    def test_tac_sim(self) -> None:
        self.assertEqual(_run_tac(_PROGRAM), _EXPECTED)

    def test_asm_sim(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(_PROGRAM, optimize=optimize).run(
                    max_cycles=4_000_000,
                )
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int_signed(), _EXPECTED)

    def test_dirty_reaches_the_objects(self) -> None:
        # Without `= {}`, an automatic object holds the bytes `dirty`
        # left, so the programs above catch a missing clear. A union
        # initialized by `{1}` has the same bytes past its first
        # member, which `{}` zeroes. Each change makes the check of
        # that object the first to fail.
        for old, new, check in (
            ("    struct s s = {};\n", "    struct s s;\n", 16 + 11 + 1),
            ("    union u u = {};\n", "    union u u = {1};\n",
             16 + 11 + 2),
            ("    int i = {};\n", "    int i;\n", 16 + 11 + 7 + 1),
        ):
            src = _PROGRAM.replace(old, new)
            self.assertNotEqual(src, _PROGRAM)
            for optimize in (False, True):
                with self.subTest(old=old, optimize=optimize):
                    result = build_sim(src, optimize=optimize).run(
                        max_cycles=4_000_000,
                    )
                    self.assertEqual(result.return_int_signed(), check)
        src = _LARGE.replace(" = {}", "")
        result = build_sim(src, optimize=True).run(max_cycles=2_000_000)
        self.assertEqual(result.return_int_signed(), 1)

    def test_large_object(self) -> None:
        result = build_sim(_LARGE, optimize=True).run(max_cycles=2_000_000)
        self.assertFalse(result.timed_out)
        self.assertEqual(result.return_int_signed(), 0)


if __name__ == "__main__":
    unittest.main()
