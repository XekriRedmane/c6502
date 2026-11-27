"""Tests for designated initializers (C99 §6.7.8, C23 §6.7.11).

Coverage:

  * The parser gives a `Designation` for `[k] = v`, `.m = v`, and a
    chain of them, folds an array designator's index (an enumeration
    constant, a `constexpr` object, or an expression of them), and
    rejects a negative index or one that isn't a constant.
  * cpp/src/passes/designators.h rewrites a list with designations into the
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

The TAC interpreter's results are checked in
cpp/tests/test_designated_init.cpp.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim
from tests.cpp_build import needs_c6502cpp


_TYPES = """\
struct p { int x; int y; };
struct s { char n; struct p pt[2]; char name[4]; };
union u { char c; long l; int i; };
struct w { int tag; union u v; };
"""


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


@needs_c6502cpp
class TestPrograms(unittest.TestCase):

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
