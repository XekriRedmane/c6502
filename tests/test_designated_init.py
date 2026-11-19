"""Tests for designated initializers (C99 §6.7.8, C23 §6.7.11).

Coverage:

  * The parser gives a `Designation` for `[k] = v`, `.m = v`, and a
    chain of them, folds an array designator's index (an enumeration
    constant, a `constexpr` object, or an expression of them), and
    rejects a negative index or one that isn't a constant.
  * passes/designators.py rewrites a list with designations into the
    positional form, with `{}` for each subobject nothing initializes:
    a positional item after a designation, a later initializer that
    overrides an earlier one (whole, or the part a chain names), an
    array of unknown size, and a union's member.
  * Each error has its message: an index past the end, a member the
    struct or union lacks, `[ ]` on a non-array, `.` on a non-struct,
    a designator in a scalar's initializer, a positional item that
    would need brace elision, a positional item after a union's
    member, and an overridden initializer with side effects.
  * A program checks every byte of designated arrays, structs, nested
    chains, overrides, unions, an unsized array, a `constexpr` index,
    and compound literals, as file-scope objects, block-scope statics,
    and automatic objects, in both pipelines and on the TAC
    interpreter.
"""
from __future__ import annotations

import shutil
import unittest

import c99_ast
from parser import ParserError, parse
from passes.identifier_resolution import resolve_program
from passes.string_lifting import lift_program
from passes.type_checking import (
    TypeCheckError, UnionValue, check_program,
)
from preprocessor import preprocess
from sim.harness import build_sim
from tests.test_sim_chapters import _run_program as _run_tac


def _parse(src: str) -> c99_ast.Type_program:
    return parse(preprocess(src))


def _check(src: str, lift: bool = True):
    prog = resolve_program(_parse(src))
    if lift:
        prog = lift_program(prog)
    return check_program(prog)


def _var(prog, name: str) -> c99_ast.Type_var_decl:
    for d in prog.declaration:
        if isinstance(d, c99_ast.VarDecl) and d.var_decl.name == name:
            return d.var_decl
    raise KeyError(name)


def _initial(src: str, name: str, lift: bool = True):
    """The value tree the type checker records for the static object
    `name`."""
    _, symbols, _ = _check(src, lift)
    return symbols[name].attrs.initial_value.value


def _shape(init):
    """`init` with each constant replaced by its value, each list by a
    Python list, and each union's list by `(member, items)`, so a
    positional form can be compared at a glance."""
    if isinstance(init, c99_ast.InitList):
        items = [_shape(it) for it in init.items]
        if init.union_member is not None:
            return (init.union_member, items)
        return items
    while isinstance(init, c99_ast.Cast):
        init = init.exp
    if isinstance(init, c99_ast.Constant):
        return init.const.value
    return init


def _normalized(src: str, name: str):
    """The positional form string_lifting gives `name`'s initializer."""
    prog = lift_program(resolve_program(_parse(src)))
    return _shape(_var(prog, name).init)


_TYPES = """\
struct p { int x; int y; };
struct s { char n; struct p pt[2]; char name[4]; };
union u { char c; long l; int i; };
struct w { int tag; union u v; };
"""


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestParse(unittest.TestCase):
    def test_designations(self) -> None:
        prog = _parse(
            _TYPES + "struct s v = {.pt[1].y = 7, [2] = 1, 3};"
        )
        items = _var(prog, "v").init.items
        self.assertEqual(items[0], c99_ast.Designation(
            designators=[
                c99_ast.MemberDesignator(member="pt"),
                c99_ast.IndexDesignator(index=1),
                c99_ast.MemberDesignator(member="y"),
            ],
            init=c99_ast.Constant(const=c99_ast.ConstInt(value=7)),
        ))
        self.assertEqual(
            items[1].designators, [c99_ast.IndexDesignator(index=2)],
        )
        self.assertIsInstance(items[2], c99_ast.Constant)

    def test_index_is_folded(self) -> None:
        prog = _parse(
            "enum { E = 1 };\n"
            "constexpr int K = 2;\n"
            "int a[8] = {[E] = 1, [K * 2 + 1] = 2, [sizeof(long)] = 3};"
        )
        indexes = [
            it.designators[0].index for it in _var(prog, "a").init.items
        ]
        self.assertEqual(indexes, [1, 5, 4])

    def test_compound_literal(self) -> None:
        prog = _parse("int *p = (int[]){[3] = 1};\n"
                      "int q = (int){.x = 1};")
        init = _var(prog, "p").init.init
        self.assertIsInstance(init, c99_ast.InitList)
        self.assertIsInstance(init.items[0], c99_ast.Designation)
        # A scalar's one designated item stays in its list, so the
        # type checker sees the designator and rejects it.
        init = _var(prog, "q").init.init
        self.assertIsInstance(init, c99_ast.InitList)

    def test_index_errors(self) -> None:
        for src, message in (
            ("int a[3] = {[-1] = 1};", "must not be negative"),
            ("int n; int a[3] = {[n] = 1};",
             "must be an integer constant expression"),
            ("int a[3] = {[1.5] = 1};",
             "must be an integer constant expression"),
        ):
            with self.subTest(src=src):
                with self.assertRaises(ParserError) as cm:
                    _parse(src)
                self.assertIn(message, str(cm.exception))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestNormalize(unittest.TestCase):
    def test_positional_continues_after_designation(self) -> None:
        self.assertEqual(
            _normalized("int a[6] = {1, [3] = 4, 5, [1] = 2};", "a"),
            [1, 2, [], 4, 5],
        )
        self.assertEqual(
            _normalized(_TYPES + "struct p q = {.y = 1, .x = 2};", "q"),
            [2, 1],
        )

    def test_chain(self) -> None:
        self.assertEqual(
            _normalized(_TYPES + "struct s v = {.pt[1].y = 7, .n = 3};",
                        "v"),
            [3, [[], [[], 7]]],
        )

    def test_list_without_designators_is_unchanged(self) -> None:
        src = "int a[2][2] = {{1}, {2, 3}};"
        prog = resolve_program(_parse(src))
        before = _var(prog, "a").init
        after = _var(lift_program(prog), "a").init
        self.assertEqual(before, after)

    def test_overrides(self) -> None:
        # A later designation into a subaggregate keeps the rest of
        # the earlier list; a later braced list replaces all of it.
        self.assertEqual(
            _normalized(
                _TYPES + "struct s v = {.pt = {{1, 2}, {3, 4}}, "
                ".pt[0].y = 9, .n = 1, .n = 2};", "v",
            ),
            [2, [[1, 9], [3, 4]]],
        )
        self.assertEqual(
            _normalized(
                _TYPES + "struct s v = {.pt[1].y = 6, .pt[1] = {5}};", "v",
            ),
            [[], [[], [5]]],
        )
        self.assertEqual(
            _normalized("int a[3] = {[0] = 1, 2, [0] = 3};", "a"),
            [3, 2],
        )

    def test_unknown_size(self) -> None:
        self.assertEqual(
            _normalized("int a[] = {[4] = 7, [1] = 3};", "a"),
            [[], 3, [], [], 7],
        )
        # The 8 after `[1] = 3` goes to index 2.
        _, symbols, _ = _check("int a[] = {[4] = 7, [1] = 3, 8};")
        self.assertEqual(symbols["a"].type.size, 5)
        self.assertEqual(
            symbols["a"].attrs.initial_value.value, (0, 3, 8, 0, 7),
        )
        _, symbols, _ = _check("int a[] = {[9] = 1, [2] = 3};")
        self.assertEqual(symbols["a"].type.size, 10)

    def test_union(self) -> None:
        self.assertEqual(
            _normalized(_TYPES + "union u v = {.i = 5};", "v"),
            ("i", [5]),
        )
        # A later member replaces an earlier one.
        self.assertEqual(
            _normalized(_TYPES + "union u v = {.c = 1, .l = 2};", "v"),
            ("l", [2]),
        )
        self.assertEqual(
            _normalized(_TYPES + "union u v = {1, .i = 2};", "v"),
            ("i", [2]),
        )
        self.assertEqual(
            _normalized(
                _TYPES + "struct w v = {.v.l = 4, .tag = 1};", "v",
            ),
            [1, ("l", [4])],
        )

    def test_union_value_trees(self) -> None:
        self.assertEqual(
            _initial(_TYPES + "union u v = {.i = 5};", "v"),
            UnionValue(member=2, value=5),
        )
        # The first member keeps the plain tuple.
        self.assertEqual(
            _initial(_TYPES + "union u v = {.c = 5};", "v"), (5,),
        )
        self.assertEqual(
            _initial(_TYPES + "struct w v = {.v.l = 4, .tag = 1};", "v"),
            (1, UnionValue(member=1, value=4)),
        )

    def test_type_checker_normalizes_too(self) -> None:
        # Without string_lifting, the type checker rewrites the list.
        self.assertEqual(
            _initial("int a[4] = {[2] = 5, 6};", "a", lift=False),
            (0, 0, 5, 6),
        )

    def test_constexpr(self) -> None:
        # An index from a constexpr object, and a constexpr struct
        # and union with designators read as named constants.
        src = _TYPES + (
            "constexpr int K = 2;\n"
            "int a[4] = {[K] = 5, [K + 1] = 6};\n"
            "constexpr struct p c = {.y = 4};\n"
            "int b[c.y];\n"
            "constexpr union u cu = {.i = 3};\n"
            "int d[cu.i];\n"
        )
        _, symbols, _ = _check(src)
        self.assertEqual(symbols["a"].attrs.initial_value.value, (0, 0, 5, 6))
        self.assertEqual(symbols["b"].type.size, 4)
        self.assertEqual(symbols["d"].type.size, 3)
        # The parser reports an array size that isn't a constant as
        # NotImplementedError, as for a variable-length array.
        with self.assertRaises(NotImplementedError) as cm:
            _check(_TYPES + "constexpr union u cu = {.i = 3};\n"
                   "int d[cu.c];\n")
        self.assertIn("isn't the member its initializer sets",
                      str(cm.exception))

    def test_side_effects_kept_when_not_overridden(self) -> None:
        # A call that nothing overrides stays in the list.
        src = "int f(void);\nint g(void) { int a[3] = {[2] = f()}; " \
              "return a[2]; }"
        _check(src)


def _rejects(test: unittest.TestCase, src: str, message: str) -> None:
    with test.assertRaises(TypeCheckError) as cm:
        _check(src)
    test.assertIn(message, str(cm.exception))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestErrors(unittest.TestCase):
    def test_index_past_the_end(self) -> None:
        _rejects(self, "int a[3] = {[3] = 1};",
                 "the designator `[3]` is past the end of `a`, an array "
                 "of 3 elements")
        _rejects(self, _TYPES + "struct s v = {.pt[2].x = 1};",
                 "past the end of `v.pt`")

    def test_positional_past_the_end(self) -> None:
        _rejects(self, "int a[3] = {[2] = 1, 2};",
                 "too many initializers for array `a`")
        _rejects(self, _TYPES + "struct p q = {.y = 1, 2};",
                 "too many initializers for struct `q`")

    def test_unknown_member(self) -> None:
        _rejects(self, _TYPES + "struct p q = {.z = 1};",
                 "the designator `.z` names no member of `q`, which has "
                 "type `struct p`")
        _rejects(self, _TYPES + "union u v = {.x = 1};",
                 "names no member of `v`")

    def test_index_on_non_array(self) -> None:
        _rejects(self, _TYPES + "struct p q = {[0] = 1};",
                 "the designator `[0]` names an element of an array, and "
                 "`q` has type `struct p`")
        _rejects(self, _TYPES + "struct s v = {.n[0] = 1};",
                 "and `v.n` has type `char`")

    def test_member_on_non_struct(self) -> None:
        _rejects(self, "int a[3] = {.x = 1};",
                 "the designator `.x` names a member of a struct or "
                 "union, and `a` has type `int[3]`")
        _rejects(self, _TYPES + "struct p q = {.x.y = 1};",
                 "and `q.x` has type `int`")

    def test_designator_in_scalar(self) -> None:
        _rejects(self, "int x = {.x = 1};",
                 "`x` is a scalar of type `int`, so its initializer "
                 "can't have the designator `.x`")
        _rejects(self, "int x = {[0] = 1};",
                 "can't have the designator `[0]`")
        _rejects(self, "int a[2] = {[0] = {[0] = 1}};",
                 "`a[0]` is a scalar of type `int`")
        _rejects(self, "int f(void) { return (int){.x = 1}; }",
                 "`(int){...}` is a scalar of type `int`")

    def test_brace_elision(self) -> None:
        _rejects(self, _TYPES + "struct s v = {.pt[0].x = 1, 2};",
                 "would initialize the subobject after `v.pt[0].x` "
                 "inside `v.pt`, which takes brace elision, and c6502 "
                 "doesn't elide braces. Put the initializers of `v.pt` "
                 "in braces of their own, as in `.pt = {...}`")
        _rejects(self, "int a[2][2] = {[0][1] = 1, 2};",
                 "inside `a[0]`, which takes brace elision")

    def test_positional_after_union_member(self) -> None:
        _rejects(self, _TYPES + "union u v = {.c = 1, 2};",
                 "too many initializers for union `v`")

    def test_overridden_side_effects(self) -> None:
        for init in ("f()", "n++", "n = 1", "--n", "n += 2"):
            with self.subTest(init=init):
                _rejects(
                    self,
                    "int f(void);\nint g(void) { int n = 0; "
                    f"int a[3] = {{[0] = {init}, [0] = 2}}; return a[0]; }}",
                    "a later designator overrides the initializer of "
                    "`a[0]`, which has side effects",
                )
        _rejects(
            self,
            _TYPES + "int f(void);\nint g(void) { struct s v = "
            "{.pt[0] = {f(), 1}, .pt[0].x = 2}; return v.n; }",
            "overrides the initializer of `v.pt[0].x`",
        )
        _rejects(
            self,
            _TYPES + "int f(void);\nint g(void) { union u v = "
            "{.c = f(), .l = 2}; return v.c; }",
            "overrides the initializer of `v.c`",
        )
        # `sizeof` doesn't evaluate its operand.
        _check("int f(void);\nint g(void) { int a[3] = "
               "{[0] = sizeof(f()), [0] = 2}; return a[0]; }")

    def test_partial_override_of_expression(self) -> None:
        _rejects(
            self,
            _TYPES + "struct p p0;\nstruct q { struct p a; };\n"
            "int g(void) { struct q v = {.a = p0, .a.y = 2}; return 0; }",
            "a designator overrides part of `v.a`, which an expression "
            "initializes",
        )

    def test_designation_outside_list(self) -> None:
        # The grammar only puts a designation in a list, so the type
        # checker's own case is reached only by a hand-built AST.
        prog = resolve_program(_parse("int g(void) { return 1; }"))
        body = prog.declaration[0].function_decl.body
        body.block_item[0].statement.exp = c99_ast.Designation(
            designators=[c99_ast.IndexDesignator(index=0)],
            init=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
        )
        with self.assertRaises(TypeCheckError) as cm:
            check_program(prog)
        self.assertIn("goes only in front of an item", str(cm.exception))


# Bytes of each object, low byte first. `struct s` is n (1), pt (2 x
# 4), name (4); `union u` is 4 bytes; `struct w` is tag (2), v (4).
_PROGRAM = _TYPES + """\
enum { E = 1 };
constexpr int K = 2;

static int failed;
static int checks;
static int calls;

/* Notes the number of the first check whose bytes differ from
   `want`. */
static void expect(const void *object, unsigned n,
                   const unsigned char *want) {
    const unsigned char *p = object;
    ++checks;
    for (unsigned i = 0; i < n; i++) {
        if (p[i] != want[i]) {
            if (!failed)
                failed = checks;
            return;
        }
    }
}

static int next(void) { return ++calls; }

static const unsigned char a_bytes[12] = {
    1, 0, 2, 0, 0, 0, 4, 0, 5, 0, 0, 0 };
static const unsigned char un_bytes[10] = {
    0, 0, 3, 0, 0, 0, 0, 0, 7, 0 };
static const unsigned char v_bytes[13] = {
    3, 0, 0, 0, 0, 0, 0, 8, 7, 'a', 'b', 0, 0 };
static const unsigned char o_bytes[13] = {
    2, 1, 0, 9, 0, 3, 0, 4, 0, 0, 'q', 0, 'z' };
static const unsigned char r_bytes[13] = {
    0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0 };
static const unsigned char ui_bytes[4] = { 0x34, 0x12, 0, 0 };
static const unsigned char ul_bytes[4] = { 4, 3, 2, 1 };
static const unsigned char uc_bytes[4] = { 0x55, 0, 0, 0 };
static const unsigned char ww_bytes[6] = { 2, 1, 0x44, 0x33, 0x22, 0x11 };
static const unsigned char wa_bytes[12] = {
    9, 0, 0, 0, 0, 0, 0, 0, 6, 5, 0, 0 };
static const unsigned char ck_bytes[8] = { 0, 0, 3, 0, 5, 0, 6, 0 };
static const unsigned char cp_bytes[4] = { 0, 0, 3, 0 };
static const unsigned char ca_bytes[6] = { 0, 0, 0, 0, 9, 0 };
static const unsigned char cu_bytes[4] = { 0x0D, 0x0C, 0x0B, 0x0A };
static const unsigned char m2_bytes[12] = {
    0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0 };

/* The objects, declared with storage class SC, and their compound
   literals with storage class CL. They're in two halves, so that the
   automatic ones fit in two frames. */
#define OBJECTS_A(SC, CL) \\
    SC int a[6] = {1, [3] = 4, 5, [1] = 2}; \\
    SC int un[] = {[4] = 7, [1] = 3}; \\
    SC struct s v = {.pt[1].y = 0x0708, .n = 3, .name = "ab"}; \\
    SC struct s o = {.pt = {{1, 2}, {3, 4}}, .pt[0].y = 9, .n = 1, \\
                     .n = 2, .name = {[1] = 'q'}, .name[3] = 'z'}; \\
    SC struct s r = {.pt[1].y = 6, .pt[1] = {5}};

#define OBJECTS_B(SC, CL) \\
    SC union u ui = {.i = 0x1234}; \\
    SC union u ul = {.c = 1, .l = 0x01020304}; \\
    SC union u uc = {.c = 0x55}; \\
    SC struct w ww = {.v.l = 0x11223344, .tag = 0x0102}; \\
    SC struct w wa[2] = {[1].v.i = 0x0506, [0].tag = 9}; \\
    SC int ck[4] = {[E] = 3, [K] = 5, [K + 1] = 6}; \\
    SC struct p *cp = &(CL struct p){.y = 3}; \\
    SC int *ca = (CL int[]){[2] = 9}; \\
    SC union u *cu = &(CL union u){.l = 0x0A0B0C0D}; \\
    SC int m2[2][3] = {[0][2] = 1, [1] = {2}};

#define CHECKS_A \\
    expect(a, sizeof a, a_bytes); \\
    expect(un, sizeof un, un_bytes); \\
    if (sizeof un != 10 && !failed) failed = 100; \\
    expect(&v, sizeof v, v_bytes); \\
    expect(&o, sizeof o, o_bytes); \\
    expect(&r, sizeof r, r_bytes);

#define CHECKS_B \\
    expect(&ui, sizeof ui, ui_bytes); \\
    expect(&ul, sizeof ul, ul_bytes); \\
    expect(&uc, sizeof uc, uc_bytes); \\
    expect(&ww, sizeof ww, ww_bytes); \\
    expect(wa, sizeof wa, wa_bytes); \\
    expect(ck, sizeof ck, ck_bytes); \\
    expect(cp, sizeof *cp, cp_bytes); \\
    expect(ca, 3 * sizeof(int), ca_bytes); \\
    expect(cu, sizeof *cu, cu_bytes); \\
    expect(m2, sizeof m2, m2_bytes);

OBJECTS_A(, )
OBJECTS_B(, )

static void file_scope(void) {
    CHECKS_A
    CHECKS_B
}

static void block_statics(void) {
    OBJECTS_A(static, static)
    OBJECTS_B(static, static)
    CHECKS_A
    CHECKS_B
}

/* Leaves 0xA5 in the bytes the next call's locals will take. */
static void dirty(void) {
    volatile unsigned char junk[200];
    for (int i = 0; i < 200; i++)
        junk[i] = 0xA5;
}

static void automatics_a(void) {
    OBJECTS_A(, )
    CHECKS_A
}

static void automatics_b(void) {
    OBJECTS_B(, )
    CHECKS_B
}

/* An initializer that nothing overrides is evaluated once. */
static int evaluated_once(void) {
    int t[3] = {[2] = next(), [0] = 7};
    return calls == 1 && t[2] == 1 && t[0] == 7 && t[1] == 0;
}

int main(void) {
    file_scope();
    block_statics();
    dirty();
    automatics_a();
    dirty();
    automatics_b();
    if (failed)
        return failed;
    if (!evaluated_once())
        return 99;
    return 0;
}
"""


@unittest.skipUnless(shutil.which("pcpp"), "pcpp CLI not available")
class TestPrograms(unittest.TestCase):
    def test_tac_sim(self) -> None:
        self.assertEqual(_run_tac(_PROGRAM), 0)

    def test_asm_sim(self) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(_PROGRAM, optimize=optimize).run(
                    max_cycles=4_000_000,
                )
                self.assertFalse(result.timed_out)
                self.assertEqual(result.return_int_signed(), 0)

    def test_dirty_reaches_the_objects(self) -> None:
        # The automatic union `uc` sets only its first byte, so its
        # other three bytes hold what `dirty` left unless the
        # initializer zeroes them. A union without a designator
        # leaves them, so its check fails. `expect` numbers its
        # calls, and the check of the automatic `uc` is the 3rd in
        # `automatics_b`, after the 15 of each of `file_scope` and
        # `block_statics` and the 5 of `automatics_a`.
        src = _PROGRAM.replace(
            "SC union u uc = {.c = 0x55};", "SC union u uc = {0x55};",
        )
        self.assertNotEqual(src, _PROGRAM)
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(src, optimize=optimize).run(
                    max_cycles=4_000_000,
                )
                self.assertEqual(result.return_int_signed(), 15 + 15 + 5 + 3)


if __name__ == "__main__":
    unittest.main()
