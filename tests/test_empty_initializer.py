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
import shutil
import unittest
from unittest.mock import patch

import c99_ast
import tac_ast
from c99_to_tac import translate_program
from compile import main as compile_main
from parser import ParserError, parse
from passes.identifier_resolution import resolve_program
from passes.type_checking import TypeCheckError, check_program
from preprocessor import preprocess
from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac


def _check(src: str):
    return check_program(resolve_program(parse(preprocess(src))))


def _initial(src: str, name: str):
    """The initial value the type checker records for the static
    object `name` (a block-scope one is found by its spelling)."""
    _, symbols, _ = _check(src)
    for key, sym in symbols.items():
        if key == name or key.endswith("." + name):
            return sym.attrs.initial_value.value
    raise KeyError(name)


def _rejects(test: unittest.TestCase, src: str, message: str) -> None:
    with test.assertRaises(TypeCheckError) as cm:
        _check(src)
    test.assertIn(message, str(cm.exception))


def _compile(src: str, *flags: str) -> str:
    """compile.py's `-S` output for `src`."""
    out = io.StringIO()
    with patch("sys.stdin", io.StringIO(src)), patch("sys.stdout", out):
        rc = compile_main(["compile.py", "-", "-S", "-o", "-", *flags])
    assert rc == 0
    return out.getvalue()


def _tac_function(src: str, name: str) -> tac_ast.Function:
    prog, symbols, types = _check(src)
    tac = translate_program(prog, symbols, types)
    for tl in tac.top_level:
        if isinstance(tl, tac_ast.Function) and tl.name == name:
            return tl
    raise KeyError(name)


_STRUCTS = """\
struct s { int a; char b[3]; long c; };
union u { char c; long l; };
"""


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestParse(unittest.TestCase):
    def test_empty_initializer(self) -> None:
        vd = parse("int x = {};").declaration[0].var_decl
        self.assertEqual(vd.init, c99_ast.InitList(items=[]))

    def test_nested_empty_initializers(self) -> None:
        vd = parse("int a[3][2] = { {}, {1, 2}, };").declaration[0].var_decl
        self.assertEqual(len(vd.init.items), 2)
        self.assertEqual(vd.init.items[0], c99_ast.InitList(items=[]))

    def test_compound_literals(self) -> None:
        fn = parse(
            "struct s { int a; };"
            "int f(void) { return (int){} + (int[4]){}[0]"
            " + (struct s){}.a; }"
        ).declaration[-1].function_decl
        found = []

        def walk(e) -> None:
            if isinstance(e, c99_ast.CompoundLiteral):
                found.append(e.init)
            for v in vars(e).values():
                if isinstance(v, (c99_ast.Type_exp,)):
                    walk(v)
        walk(fn.body.block_item[0].statement.exp)
        self.assertEqual(found, [c99_ast.InitList(items=[])] * 3)

    def test_braced_scalar_compound_literal_keeps_its_expression(self) -> None:
        fn = parse("int f(void) { return (int){5,}; }").declaration[0] \
            .function_decl
        cl = fn.body.block_item[0].statement.exp
        self.assertEqual(
            cl.init, c99_ast.Constant(const=c99_ast.ConstInt(value=5)),
        )

    def test_comma_alone_is_a_syntax_error(self) -> None:
        for src in ("int x = {,};", "int f(void) { return (int){,}; }"):
            with self.subTest(src=src):
                with self.assertRaises(Exception) as cm:
                    parse(src)
                self.assertNotIsInstance(cm.exception, TypeCheckError)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestTypeCheck(unittest.TestCase):
    def test_scalars_are_zero(self) -> None:
        src = (
            "typedef __c6502_nullptr_t nullptr_t;"
            "int i = {}; unsigned long ul = {}; char c = {};"
            "signed char sc = {}; long long ll = {}; int *p = {};"
            "void (*fp)(void) = {}; float f = {}; double d = {};"
            "bool b = {}; _BitInt(12) w = {}; unsigned _BitInt(40) uw = {};"
            "nullptr_t n = {}; const volatile int cv = {};"
        )
        _, symbols, _ = _check(src)
        for name in ("i", "ul", "c", "sc", "ll", "p", "fp", "f", "d", "b",
                     "w", "uw", "n", "cv"):
            with self.subTest(name=name):
                self.assertEqual(
                    symbols[name].attrs.initial_value.value, 0,
                )

    def test_automatic_scalars_get_a_zero_of_their_type(self) -> None:
        prog, _, _ = _check(
            "int f(void) { float f = {}; double d = {}; int *p = {};"
            " char c = {}; return 0; }"
        )
        items = prog.declaration[0].function_decl.body.block_item
        inits = [it.declaration.var_decl.init for it in items[:4]]
        # A floating object gets a floating zero, so no conversion
        # from an integer is left for run time.
        self.assertEqual(
            inits[0], c99_ast.Constant(
                const=c99_ast.ConstFloat(bits=0), data_type=c99_ast.Float(),
            ),
        )
        self.assertEqual(
            inits[1], c99_ast.Constant(
                const=c99_ast.ConstDouble(bits=0),
                data_type=c99_ast.Double(),
            ),
        )
        for init, t in ((inits[2], c99_ast.Pointer(c99_ast.Int())),
                        (inits[3], c99_ast.Char())):
            self.assertIsInstance(init, c99_ast.Cast)
            self.assertEqual(init.target_type, t)
            self.assertEqual(init.exp.const, c99_ast.ConstInt(value=0))

    def test_aggregates(self) -> None:
        src = _STRUCTS + (
            "struct s v = {}; union u un = {}; int a[4] = {};"
            "struct s w = { {}, {1}, {} };"
            "int k[3][2] = { {}, {1, 2} };"
            "char str[3] = {};"
        )
        _, symbols, _ = _check(src)
        values = {n: symbols[n].attrs.initial_value.value
                  for n in ("v", "un", "a", "w", "k", "str")}
        self.assertEqual(values, {
            "v": (0, (0, 0, 0), 0),
            "un": (0,),
            "a": (0, 0, 0, 0),
            "w": (0, (1, 0, 0), 0),
            "k": ((0, 0), (1, 2), (0, 0)),
            "str": (0, 0, 0),
        })

    def test_block_scope_statics(self) -> None:
        src = _STRUCTS + (
            "int f(void) { static struct s v = {}; static int i = {};"
            " static int j = {3}; static union u un = {};"
            " static double d = {}; return 0; }"
        )
        self.assertEqual(_initial(src, "v"), (0, (0, 0, 0), 0))
        self.assertEqual(_initial(src, "i"), 0)
        self.assertEqual(_initial(src, "j"), 3)
        self.assertEqual(_initial(src, "un"), (0,))
        self.assertEqual(_initial(src, "d"), 0)

    def test_compound_literals(self) -> None:
        src = _STRUCTS + (
            "struct s *ps = &(struct s){}; int *pa = (int[4]){};"
            "int f(void) { struct s *q = &(struct s){};"
            " int *r = (int[3]){}; return (int){} + (long){7}; }"
        )
        _, symbols, _ = _check(src)
        literals = [
            sym.attrs.initial_value.value for name, sym in symbols.items()
            if name.endswith(".compound_literal")
            and hasattr(sym.attrs, "initial_value")
        ]
        self.assertCountEqual(literals, [(0, (0, 0, 0), 0), (0, 0, 0, 0)])

    def test_unknown_size_array_is_rejected(self) -> None:
        message = (
            "has unknown size, so it can't be initialized by the empty "
            "initializer `{}`, which has no elements to give it a size "
            "(C23 §6.7.11p4)"
        )
        for src, what in (
            ("int a[] = {};", "array 'a'"),
            ("int f(void) { int a[] = {}; return 0; }", "array 'a'"),
            ("int f(void) { static char a[] = {}; return 0; }", "array 'a'"),
            ("int f(void) { for (int a[] = {};;) return 0; }", "array 'a'"),
            ("int f(void) { return (int[]){}[0]; }",
             "the compound literal's array"),
            ("int *p = (int[]){};", "the compound literal's array"),
        ):
            with self.subTest(src=src):
                _rejects(self, src, f"{what} {message}")

    def test_braced_scalars(self) -> None:
        src = _STRUCTS + (
            "int x = {5}; long y = {-7,}; double d = {1.5};"
            "int *p = {0}; struct s v = { {3}, {}, {4} };"
            "int a[3] = { {1}, {}, 3 };"
        )
        _, symbols, _ = _check(src)
        self.assertEqual(symbols["x"].attrs.initial_value.value, 5)
        self.assertEqual(symbols["y"].attrs.initial_value.value, -7)
        self.assertEqual(
            symbols["d"].attrs.initial_value.value, 0x3FF8000000000000,
        )
        self.assertEqual(symbols["p"].attrs.initial_value.value, 0)
        self.assertEqual(
            symbols["v"].attrs.initial_value.value, (3, (0, 0, 0), 4),
        )
        self.assertEqual(symbols["a"].attrs.initial_value.value, (1, 0, 3))

    def test_braced_scalar_at_block_scope(self) -> None:
        prog, _, _ = _check(
            "int f(void) { int x = {5}; for (int i = {2,}; i;) break;"
            " return x; }"
        )
        items = prog.declaration[0].function_decl.body.block_item
        self.assertEqual(
            items[0].declaration.var_decl.init,
            c99_ast.Constant(
                const=c99_ast.ConstInt(value=5), data_type=c99_ast.Int(),
            ),
        )

    def test_braced_scalar_errors(self) -> None:
        too_many = "a scalar's braces hold one expression (C23 §6.7.11p12)"
        nested = (
            "a scalar's initializer is one expression, optionally in one "
            "pair of braces, or the empty initializer `{}` "
            "(C23 §6.7.11p12)"
        )
        for src, message in (
            ("int x = {1, 2};",
             f"too many initializers for scalar variable 'x': 2 given, "
             f"and {too_many}"),
            ("int f(void) { int x = {1, 2,}; return x; }",
             "too many initializers for scalar variable 'x'"),
            ("int f(void) { static int x = {1, 2}; return x; }",
             "too many initializers for scalar variable 'x'"),
            ("int f(void) { for (int i = {1, 2};;) return 0; }",
             "too many initializers for scalar variable 'i'"),
            ("int x = {{5}};",
             f"unexpected nested initializer for scalar variable 'x': "
             f"{nested}"),
            ("int x = {{}};", "unexpected nested initializer"),
            ("struct t { int a; }; struct t v = { {1, 2} };",
             "too many initializers for member 'a' of 'v'"),
            ("int a[2] = { {{1}} };",
             "unexpected nested initializer for element 0 of 'a'"),
            ("int f(void) { return (int){1, 2}; }",
             "too many initializers for scalar variable '(compound "
             "literal)'"),
            ("int f(void) { return (int){{1}}; }",
             "unexpected nested initializer for scalar variable "
             "'(compound literal)'"),
        ):
            with self.subTest(src=src):
                _rejects(self, src, message)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
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

    def test_small_automatic_struct_is_stores(self) -> None:
        # 7 bytes: a 4-byte store, a 2-byte store, and a 1-byte store.
        fn = _tac_function(
            "struct t { int a; char b; long c; };"
            "void use(struct t *p);"
            "void f(void) { struct t v = {}; use(&v); }", "f",
        )
        stores = [i for i in fn.instructions if isinstance(i, tac_ast.Store)]
        self.assertEqual(
            [type(s.src.const).__name__ for s in stores],
            ["ConstULong", "ConstUInt", "ConstUChar"],
        )
        self.assertFalse(any(isinstance(i, tac_ast.Label)
                             for i in fn.instructions))

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
        # (passes/loop_constant_a.py).
        asm = _compile(src, "-O")
        self.assertIn(
            "   LDY   #$28\n   LDA   #$00\n.clear@0:\n   DEY\n"
            "   STA   __local_f__v,Y\n   BNE   .clear@0\n",
            asm,
        )

    def test_zero_constants_join_the_run(self) -> None:
        # `{0}` clears the object the way `{}` does: the explicit 0
        # and the members after it are one run.
        src = (
            "struct t { int a[10]; };"
            "void use(struct t *p);"
            "void f(void) { struct t v = {{0}}; use(&v); }"
            "void g(void) { struct t v = {}; use(&v); }"
        )
        f = _tac_function(src, "f")
        g = _tac_function(src, "g")
        self.assertEqual(
            [type(i).__name__ for i in f.instructions],
            [type(i).__name__ for i in g.instructions],
        )

    def test_partial_initializer_with_long_tail(self) -> None:
        # The stores for the explicit items, then one loop for the
        # 36 bytes after them.
        fn = _tac_function(
            "void use(int *p);"
            "void f(int x) { int a[20] = {x, 2}; use(a); }", "f",
        )
        kinds = [type(i).__name__ for i in fn.instructions]
        self.assertEqual(kinds.count("Label"), 1)
        self.assertEqual(kinds.count("Store"), 3)


# Bytes of `struct s`: a (2), b (3), c (4).
_PROGRAM = """\
struct s { int a; char b[3]; long c; };
union u { char c; long l; double d; };
struct n { struct s s; union u u; int k[2][3]; float f; };
typedef __c6502_nullptr_t nullptr_t;

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


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestPrograms(unittest.TestCase):
    def test_tac_sim(self) -> None:
        self.assertEqual(_run_tac(preprocess(_PROGRAM)), _EXPECTED)

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
