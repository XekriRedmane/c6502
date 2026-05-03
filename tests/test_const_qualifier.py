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
  - End-to-end compilation through `--codegen` for valid programs.

c6502 deliberately defers the C99 §6.5.16.1 pointer-assignment
qualifier compatibility check (assigning `const int *` to `int *`
without a cast). That mirrors gcc's `-Wno-discarded-qualifiers`
behavior — the modification check at the actual write site is
the load-bearing one.
"""
from __future__ import annotations

import unittest

import c99_ast
from parser import parse
from passes.identifier_resolution import resolve_program as resolve_identifiers
from passes.type_checking import (
    Const, Int, Long, Pointer, TypeCheckError, check_program,
)
from sim.harness import run_c_program


def _check(src: str):
    """Parse, identifier-resolve, type-check. Raises on failure."""
    prog, symbols, _types = check_program(resolve_identifiers(parse(src)))
    return prog, symbols


def _expect_typecheck_error(test, src: str, fragment: str = ""):
    with test.assertRaises(TypeCheckError) as cm:
        _check(src)
    if fragment:
        test.assertIn(fragment, str(cm.exception))


class TestParsingConst(unittest.TestCase):
    """Parser accepts `const` in every C99 position c6502 cares
    about. The AST shape is verified against the Const wrapper
    placement convention."""

    def test_const_on_simple_var(self):
        prog = parse("const int x = 5;")
        decl = prog.declaration[0].var_decl
        self.assertIsInstance(decl.data_type, Const)
        self.assertIsInstance(decl.data_type.referenced_type, Int)

    def test_const_after_int(self):
        # `int const x` — order doesn't matter (C99 §6.7).
        prog = parse("int const x = 5;")
        decl = prog.declaration[0].var_decl
        self.assertIsInstance(decl.data_type, Const)
        self.assertIsInstance(decl.data_type.referenced_type, Int)

    def test_pointer_to_const(self):
        # `const int *p` — pointee is const, pointer is not.
        prog = parse("const int *p;")
        decl = prog.declaration[0].var_decl
        self.assertIsInstance(decl.data_type, Pointer)
        self.assertIsInstance(decl.data_type.referenced_type, Const)
        self.assertIsInstance(
            decl.data_type.referenced_type.referenced_type, Int,
        )

    def test_const_pointer(self):
        # `int * const p` — pointer is const, pointee is not.
        prog = parse("int x; int * const p = &x;")
        decl = prog.declaration[1].var_decl
        self.assertIsInstance(decl.data_type, Const)
        self.assertIsInstance(decl.data_type.referenced_type, Pointer)
        self.assertIsInstance(
            decl.data_type.referenced_type.referenced_type, Int,
        )

    def test_const_pointer_to_const(self):
        # `const int * const p` — both const.
        prog = parse("int x; const int * const p = &x;")
        decl = prog.declaration[1].var_decl
        self.assertIsInstance(decl.data_type, Const)
        ptr = decl.data_type.referenced_type
        self.assertIsInstance(ptr, Pointer)
        self.assertIsInstance(ptr.referenced_type, Const)
        self.assertIsInstance(ptr.referenced_type.referenced_type, Int)

    def test_const_in_struct_member(self):
        prog = parse("struct S { const int x; int y; };")
        struct_decl = prog.declaration[0].struct_decl
        members = {m.name: m.data_type for m in struct_decl.members}
        self.assertIsInstance(members["x"], Const)
        self.assertIsInstance(members["x"].referenced_type, Int)
        # `y` is unqualified.
        self.assertIsInstance(members["y"], Int)

    def test_const_in_cast(self):
        # `(const int)x` — qualified cast-target. Mostly meaningless
        # at runtime (rvalues don't carry qualifiers per §6.3.2.1.2),
        # but the syntax has to parse.
        src = (
            "int main(void) {\n"
            "    int x = 5;\n"
            "    return (const int)x;\n"
            "}\n"
        )
        # Just check it parses without error.
        parse(src)

    def test_const_in_pointer_cast(self):
        # `(const int *)p` — cast a pointer to pointer-to-const.
        src = (
            "int main(void) {\n"
            "    int x = 5;\n"
            "    int *p = &x;\n"
            "    const int *q = (const int *)p;\n"
            "    return *q;\n"
            "}\n"
        )
        # Should parse and type-check (adding const is always OK).
        _check(src)

    def test_const_function_param(self):
        src = "int f(const int x) { return x + 1; }"
        prog, _ = _check(src)
        # Function decl's data_type carries the param type.
        fn_decl = prog.declaration[0].function_decl
        param_types = fn_decl.data_type.params
        self.assertIsInstance(param_types[0], Const)

    def test_const_idempotent(self):
        # `const const int` — duplicate qualifiers are explicitly
        # allowed by C99 §6.7.3.4 with no extra effect. The AST
        # wraps in a single `Const(...)`.
        prog = parse("const const int x = 5;")
        decl = prog.declaration[0].var_decl
        self.assertIsInstance(decl.data_type, Const)
        # Inner type is Int, NOT Const(Int).
        self.assertIsInstance(decl.data_type.referenced_type, Int)


class TestModificationErrors(unittest.TestCase):
    """The whole point of const: errors on attempted modification
    of a const-qualified lvalue."""

    def test_assign_to_const_var(self):
        src = (
            "const int x = 5;\n"
            "int main(void) { x = 10; return x; }\n"
        )
        _expect_typecheck_error(
            self, src, "cannot assign to const-qualified",
        )

    def test_compound_assign_to_const(self):
        src = (
            "const int x = 5;\n"
            "int main(void) { x += 1; return x; }\n"
        )
        _expect_typecheck_error(
            self, src, "cannot modify const-qualified",
        )

    def test_postfix_increment_const(self):
        src = (
            "const int x = 5;\n"
            "int main(void) { x++; return x; }\n"
        )
        _expect_typecheck_error(
            self, src, "cannot use ++/-- on const-qualified",
        )

    def test_prefix_decrement_const(self):
        src = (
            "const int x = 5;\n"
            "int main(void) { --x; return x; }\n"
        )
        _expect_typecheck_error(
            self, src, "cannot use ++/-- on const-qualified",
        )

    def test_modify_through_pointer_to_const(self):
        # `*p = ...` where p is `const int *` — the pointee is
        # const-qualified, modification through *p is rejected.
        src = (
            "int main(void) {\n"
            "    int x = 5;\n"
            "    const int *p = &x;\n"
            "    *p = 10;\n"
            "    return *p;\n"
            "}\n"
        )
        _expect_typecheck_error(
            self, src, "cannot assign to const-qualified",
        )

    def test_modify_const_pointer_itself(self):
        # `p = ...` where p is `int * const` — the pointer itself
        # is const-qualified, reassigning p is rejected.
        src = (
            "int main(void) {\n"
            "    int x = 5;\n"
            "    int y = 10;\n"
            "    int * const p = &x;\n"
            "    p = &y;\n"
            "    return *p;\n"
            "}\n"
        )
        _expect_typecheck_error(
            self, src, "cannot assign to const-qualified",
        )

    def test_assign_through_const_pointer_is_OK(self):
        # `int * const p` — pointer is const, pointee is NOT.
        # `*p = 10;` writes through to non-const x — allowed.
        src = (
            "int main(void) {\n"
            "    int x = 5;\n"
            "    int * const p = &x;\n"
            "    *p = 10;\n"
            "    return x;\n"
            "}\n"
        )
        _check(src)  # no error expected

    def test_modify_array_of_const(self):
        # `const int arr[3]` → elements are const.
        src = (
            "int main(void) {\n"
            "    const int arr[3] = {1, 2, 3};\n"
            "    arr[0] = 99;\n"
            "    return arr[0];\n"
            "}\n"
        )
        _expect_typecheck_error(
            self, src, "cannot assign to const-qualified",
        )

    def test_modify_const_struct_member_directly(self):
        # `struct S { const int x; ... }; struct S s; s.x = 1;`
        # Member x is declared const → modification rejected.
        src = (
            "struct S { const int x; int y; };\n"
            "int main(void) {\n"
            "    struct S s; s.x = 5; return 0;\n"
            "}\n"
        )
        _expect_typecheck_error(
            self, src, "cannot assign to const-qualified",
        )

    def test_modify_member_of_const_struct(self):
        # `const struct S s; s.y = 1;` — even though y isn't declared
        # const, the container's const-qualification propagates per
        # C99 §6.5.2.3.3.
        src = (
            "struct S { int x; int y; };\n"
            "int main(void) {\n"
            "    const struct S s = {1, 2};\n"
            "    s.y = 99;\n"
            "    return 0;\n"
            "}\n"
        )
        _expect_typecheck_error(
            self, src, "cannot assign to const-qualified",
        )

    def test_modify_through_pointer_to_const_struct(self):
        # `const struct S *p; p->y = ...;` — Arrow propagates
        # const from the pointee.
        src = (
            "struct S { int x; int y; };\n"
            "int main(void) {\n"
            "    struct S s = {1, 2};\n"
            "    const struct S *p = &s;\n"
            "    p->y = 99;\n"
            "    return 0;\n"
            "}\n"
        )
        _expect_typecheck_error(
            self, src, "cannot assign to const-qualified",
        )


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


class TestVolatileAndRestrictParseButDrop(unittest.TestCase):
    """`volatile` and `restrict` are reserved keywords accepted by
    the parser but silently dropped — c6502 doesn't model their
    semantics. Programs using them parse and run as if the
    qualifier weren't there."""

    def test_volatile_int_parses(self):
        prog = parse("volatile int x = 5;")
        decl = prog.declaration[0].var_decl
        # Stripped — same as plain `int x = 5;`.
        self.assertIsInstance(decl.data_type, Int)

    def test_restrict_pointer_parses(self):
        prog = parse("int *restrict p;")
        decl = prog.declaration[0].var_decl
        # `restrict` only applies to pointers — same shape as
        # `int *p;`.
        self.assertIsInstance(decl.data_type, Pointer)

    def test_volatile_can_be_modified(self):
        # We don't enforce volatile, so writes succeed (which is
        # also the standard's behavior — `volatile T` is still a
        # modifiable lvalue).
        src = (
            "int main(void) {\n"
            "    volatile int x = 5;\n"
            "    x = 10;\n"
            "    return x;\n"
            "}\n"
        )
        result = run_c_program(src)
        self.assertEqual(result.return_int_signed(), 10)


if __name__ == "__main__":
    unittest.main()
