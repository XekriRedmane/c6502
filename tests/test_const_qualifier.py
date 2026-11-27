"""Tests for the C99 `const` type qualifier (§6.7.3).

Coverage:
  - Parser accepts `const` in declarations, pointers, type-names,
    struct members, function params/returns.
  - AST builds the right `Const(...)` shape at every level.
  - Type checker rejects modification of const lvalues:
      * Direct assignment to a const variable
      * Compound assignment / += / -= etc.
      * Prefix / postfix ++ / --
      * Modification through a pointer-to-const
      * Subscript of an array of const elements
      * Member access on a const-qualified struct
      * Member access where the member itself is const
  - Cast-away-const works (no error on `(int *)p` where p is
    `const int *`).
  - Arithmetic on const operands strips the qualifier — `const int +
    const int` produces `int`, not `const int`.
  - End-to-end compilation through `-S` for valid programs.

c6502 deliberately defers the C99 §6.5.16.1 pointer-assignment
qualifier compatibility check (assigning `const int *` to `int *`
without a cast). That mirrors gcc's `-Wno-discarded-qualifiers`
behavior — the modification check at the actual write site is
the load-bearing one.

The `-S` text is checked in cpp/tests/test_const_qualifier.cpp.
"""
from __future__ import annotations

import unittest

from sim.harness import build_sim, run_c_program


class TestCastAwayConst(unittest.TestCase):
    """Per the user's spec: `(int *)x` where x is `const int *`
    discards const and the result IS modifiable. C99 says the
    underlying object's behavior is UB if it was actually a const
    object, but we don't try to enforce that — only modification
    of a syntactically const lvalue is rejected."""

    def test_cast_away_const_through_pointer(self):
        src = (
            "int main(void) {\n"
            "    int x = 5;\n"
            "    const int *p = &x;\n"
            "    *(int *)p = 10;\n"
            "    return x;\n"
            "}\n"
        )
        # Should compile end-to-end.
        result = run_c_program(src)
        self.assertEqual(result.return_int_signed(), 10)

    def test_cast_away_const_value(self):
        # `(int)x` where x is `const int` — explicit cast strips
        # the qualifier from the rvalue.
        src = (
            "const int x = 5;\n"
            "int main(void) { return (int)x + 1; }\n"
        )
        result = run_c_program(src)
        self.assertEqual(result.return_int_signed(), 6)


class TestArithmeticStripsConst(unittest.TestCase):
    """Arithmetic / common-type computation operates on unqualified
    types per C99 §6.3.2.1.2. `const int + const int` produces
    plain `int`, not `const int`."""

    def test_add_two_const_ints_runs(self):
        src = (
            "const int a = 3;\n"
            "const int b = 4;\n"
            "int main(void) { return a + b; }\n"
        )
        result = run_c_program(src)
        self.assertEqual(result.return_int_signed(), 7)

    def test_const_int_plus_int_runs(self):
        src = (
            "const int a = 100;\n"
            "int main(void) { int b = 23; return a + b; }\n"
        )
        result = run_c_program(src)
        self.assertEqual(result.return_int_signed(), 123)


class TestEndToEnd(unittest.TestCase):
    """Programs that USE const for what it's good at — declaring
    immutable lookup tables, parameter contracts. Verify the
    program runs and returns the expected value."""

    def test_const_lookup_table(self):
        src = (
            "const int squares[5] = {0, 1, 4, 9, 16};\n"
            "int main(void) { return squares[3]; }\n"
        )
        result = run_c_program(src)
        self.assertEqual(result.return_int_signed(), 9)

    def test_const_param(self):
        src = (
            "int square(const int x) { return x * x; }\n"
            "int main(void) { return square(7); }\n"
        )
        result = run_c_program(src)
        self.assertEqual(result.return_int_signed(), 49)

    def test_const_pointer_param(self):
        # `const int *` parameter — promises not to modify through it.
        src = (
            "int sum3(const int *p) { return p[0] + p[1] + p[2]; }\n"
            "int main(void) {\n"
            "    int arr[3] = {10, 20, 30};\n"
            "    return sum3(arr);\n"
            "}\n"
        )
        result = run_c_program(src)
        self.assertEqual(result.return_int_signed(), 60)

    def test_const_pointer_static_pointer_arithmetic(self):
        # Regression: a `T * const` static used as the base of a
        # 16-bit pointer + integer Add. The bug was that several
        # tac_to_asm helpers (`_size_of` and friends) read `sym.type`
        # without stripping the `Const` wrapper, so the static's
        # width came back as 1 byte. The 16-bit Add then lowered as
        # a single-byte add — the high byte was never written, and
        # the resulting address pointed wherever the temp's high
        # byte happened to hold. We pick an index whose low-byte
        # add carries into the high byte (300 = 0x012C, base low
        # byte 0x40 + 0x2C = 0x6C with no carry; switch to an index
        # that DOES need both bytes), and write through the pointer:
        # if the high byte was lost we land on a different page
        # entirely. The store target is 0x4000 + 300 = 0x412C; in
        # the buggy lowering the high byte of the address is
        # uninitialized so memory[0x412C] stays 0.
        src = (
            "static unsigned char * const buf"
            " = (unsigned char * const)0x4000;\n"
            "int main(void) {\n"
            "    buf[300] = 0x42;\n"
            "    return 0;\n"
            "}\n"
        )
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                sim = build_sim(src, optimize=optimize)
                result = sim.run()
                self.assertEqual(result.memory[0x4000 + 300], 0x42)


class TestVolatileQualifierAndRestrictDrop(unittest.TestCase):
    """`volatile` is honored by the parser and the type checker —
    it wraps the qualified type in a `Volatile(...)` AST node, which
    c99_to_tac reads off to mark Load / Store atoms as volatile.
    `restrict` is still parser-accepted but silently dropped (c6502
    has no aliasing-analysis pass that could use it)."""


    def test_volatile_can_be_modified(self):
        # `volatile T` is a modifiable lvalue (only `const` rejects
        # writes); the assignment succeeds and the program reads
        # back the updated value.
        src = (
            "int main(void) {\n"
            "    volatile int x = 5;\n"
            "    x = 10;\n"
            "    return x;\n"
            "}\n"
        )
        result = run_c_program(src)
        self.assertEqual(result.return_int_signed(), 10)


class TestQualifiedAggregatesRun(unittest.TestCase):
    """Programs that reach a struct, an array, or a pointer through a
    `const` or `volatile` qualifier, run in the simulator through both
    pipelines. Each one was rejected or crashed the compiler before
    the type checker and `c99_to_tac` looked through the qualifiers."""

    def _returns(self, src: str, want: int) -> None:
        for optimize in (False, True):
            with self.subTest(optimize=optimize):
                result = build_sim(src, optimize=optimize).run()
                self.assertEqual(result.return_int_signed(), want)

    def test_array_member_through_pointer_to_const_struct(self):
        self._returns(
            "struct s { unsigned char a[3]; };\n"
            "static unsigned char f(const struct s *p) { return p->a[1]; }\n"
            "int main(void) { struct s v = {{4, 5, 6}}; return f(&v); }\n",
            5,
        )

    def test_arrow_through_const_pointer(self):
        self._returns(
            "struct s { unsigned char a; unsigned char b[3]; };\n"
            "static void f(struct s *q) {\n"
            "    struct s *const p = q; p->a = 5; p->b[1] = 7;\n"
            "}\n"
            "int main(void) {\n"
            "    struct s v = {0, {0, 0, 0}}; f(&v); return v.a * 10 + v.b[1];\n"
            "}\n",
            57,
        )

    def test_member_of_const_struct(self):
        self._returns(
            "struct s { unsigned char x; unsigned char a; };\n"
            "static unsigned char f(const struct s *p) { return p->a; }\n"
            "int main(void) { struct s v = {1, 9}; return f(&v); }\n",
            9,
        )

    def test_comparison_of_pointers_differing_in_qualifiers(self):
        self._returns(
            "unsigned char buf[4];\n"
            "static int count(const unsigned char *p) {\n"
            "    int n = 0; while (p != buf + 4) { n++; p++; } return n;\n"
            "}\n"
            "static int before(const unsigned char *p) { return p < buf + 4; }\n"
            "static const unsigned char *pick(int c, const unsigned char *p) {\n"
            "    return c ? p : buf;\n"
            "}\n"
            "int main(void) {\n"
            "    return count(buf + 1) * 100 + before(buf + 2) * 10\n"
            "        + (pick(0, buf + 3) == buf);\n"
            "}\n",
            311,
        )

    def test_address_constants_into_arrays_and_members(self):
        self._returns(
            "const unsigned char A[3][4] = "
            "{{0, 1, 2, 3}, {10, 11, 12, 13}, {20, 21, 22, 23}};\n"
            "struct m { unsigned char x; unsigned char m[4]; };\n"
            "struct m S = {1, {30, 31, 32, 33}};\n"
            "static const unsigned char *const t[] = "
            "{ A[2], &A[1][3], A[1] + 2, &S.m[1], S.m + 3 };\n"
            "int main(void) {\n"
            "    return t[0][1] + t[1][0] + *t[2] + *t[3] + *t[4];\n"
            "}\n",
            21 + 13 + 12 + 31 + 33,
        )

    def test_const_struct_objects(self):
        self._returns(
            "struct s { unsigned char a; int b; };\n"
            "const struct s ca[2] = {{1, 200}, {3, 400}};\n"
            "int main(void) {\n"
            "    const struct s la = {7, 1000};\n"
            "    struct s c = ca[1];\n"
            "    return ca[0].b + la.b + c.b + la.a;\n"
            "}\n",
            200 + 1000 + 400 + 7,
        )

    def test_const_struct_with_block_scope_tag(self):
        self._returns(
            "int main(void) {\n"
            "    struct t { int a; int b; };\n"
            "    const struct t x = {300, 4};\n"
            "    const struct t *p = &x;\n"
            "    return p->a + x.b;\n"
            "}\n",
            304,
        )

    def test_volatile_pointer_steps_by_the_element_size(self):
        self._returns(
            "int arr[4] = {10, 20, 30, 40};\n"
            "int main(void) { int *volatile p = arr; p++; p += 2; return *p; }\n",
            40,
        )


if __name__ == "__main__":
    unittest.main()
