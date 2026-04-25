import unittest

import c99_ast
from parser import parse
from passes.identifier_resolution import (
    resolve_program as resolve_identifiers,
)
from passes.type_checking import (
    FunType,
    Int,
    Symbol,
    SymbolTable,
    TypeCheckError,
    TypeChecker,
    check_program,
)


def _check(src: str):
    """Parse, run identifier resolution, then type-check. Returns
    `(prog, symbols)` from `check_program`. Most tests need the
    symbols half so they assert on the table; the prog half is
    rarely used (type checking doesn't modify the AST)."""
    return check_program(resolve_identifiers(parse(src)))


class TestSymbolTableContents(unittest.TestCase):
    """The symbol table is the primary product of the pass — the AST
    isn't modified, so every observable result of a successful run
    lives there."""

    def test_minimal_program_records_main_as_defined(self):
        _, symbols = _check("int main(void) { return 0; }")
        self.assertIn("main", symbols)
        self.assertEqual(
            symbols["main"].type, FunType(params=(), ret=Int()),
        )
        self.assertTrue(symbols["main"].defined)

    def test_variable_recorded_as_int(self):
        _, symbols = _check("int main(void) { int x = 3; return x; }")
        # identifier_resolution renamed `x` to `@0.x`; type-check
        # records the same key.
        self.assertEqual(symbols["@0.x"].type, Int())
        # `defined` is irrelevant for variables today; defaults to
        # False.
        self.assertFalse(symbols["@0.x"].defined)

    def test_function_decl_recorded_with_arity(self):
        _, symbols = _check(
            "int main(void) { int foo(int a, int b); return 0; }"
        )
        self.assertEqual(
            symbols["foo"].type,
            FunType(params=(Int(), Int()), ret=Int()),
        )
        self.assertFalse(symbols["foo"].defined)

    def test_function_definition_is_marked_defined(self):
        _, symbols = _check(
            "int foo(void) { return 1; } int main(void) { return 0; }"
        )
        self.assertTrue(symbols["foo"].defined)
        self.assertTrue(symbols["main"].defined)

    def test_parameters_recorded_in_table(self):
        _, symbols = _check("int main(int x, int y) { return x + y; }")
        self.assertEqual(symbols["@0.x"].type, Int())
        self.assertEqual(symbols["@1.y"].type, Int())

    def test_function_decl_params_not_added_to_table(self):
        # Block-scope function-declaration param names get unique
        # renames during identifier_resolution, but they have no
        # body to be referenced from — type-check doesn't waste
        # symbol-table entries on them.
        _, symbols = _check(
            "int main(void) { int foo(int a); return 0; }"
        )
        self.assertNotIn("@0.a", symbols)


class TestVariableUsedAsFunction(unittest.TestCase):
    """`int x; x();` — calling a name that's a variable. Caught by
    the FunctionCall branch of the type checker, not by identifier
    resolution: the loosened cross-namespace lookup deliberately
    routes it here for the better diagnostic."""

    def test_local_variable_called_as_function(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check("int main(void) { int x; x(); return 0; }")
        self.assertIn("called as a function", str(ctx.exception))

    def test_param_called_as_function(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check("int main(int x) { x(); return 0; }")
        self.assertIn("called as a function", str(ctx.exception))


class TestFunctionUsedAsVariable(unittest.TestCase):
    """`int foo(void); int x = foo;` — referring to a function as a
    value. C allows this with function-pointer semantics, but
    c6502 doesn't model function pointers yet, so it's a type
    error."""

    def test_function_in_return(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int foo(void); return foo; }"
            )
        self.assertIn("used as a variable", str(ctx.exception))

    def test_function_in_initializer(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int foo(void); int x = foo; "
                "return x; }"
            )
        self.assertIn("used as a variable", str(ctx.exception))

    def test_function_in_arithmetic(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int foo(void); return foo + 1; }"
            )
        self.assertIn("used as a variable", str(ctx.exception))


class TestCallArity(unittest.TestCase):
    """Function-call argument counts must match the declared param
    count. Today every type is `Int`, so element-by-element
    arg/param-type comparison is trivial; what matters is the
    arity check."""

    def test_too_few_args_raises(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int foo(int a, int b); "
                "return foo(1); }"
            )
        self.assertIn("expected 2", str(ctx.exception))

    def test_too_many_args_raises(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int foo(int a); "
                "return foo(1, 2); }"
            )
        self.assertIn("expected 1", str(ctx.exception))

    def test_correct_arity_passes(self):
        # Should not raise.
        _check(
            "int main(void) { int foo(int a, int b); "
            "return foo(1, 2); }"
        )

    def test_no_args_passes(self):
        _check(
            "int main(void) { int foo(void); return foo(); }"
        )


class TestRedeclaration(unittest.TestCase):
    """Multiple declarations of the same function name must agree
    on signature; otherwise `add_function` raises 'incompatible
    redeclaration'."""

    def test_matching_redeclaration_passes(self):
        # Two declarations with the same signature → fine; second
        # decl just re-confirms the first.
        _check(
            "int main(void) { "
            "int foo(int a); int foo(int a); return 0; }"
        )

    def test_different_arity_raises(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { "
                "int foo(int a); int foo(int a, int b); "
                "return 0; }"
            )
        self.assertIn("incompatible", str(ctx.exception))

    def test_def_then_matching_decl_passes(self):
        # Definition first, declaration second — common in C. The
        # decl re-confirms the signature; the `defined` flag stays
        # True.
        _, symbols = _check(
            "int foo(void) { return 1; } "
            "int main(void) { int foo(void); return foo(); }"
        )
        self.assertTrue(symbols["foo"].defined)

    def test_decl_then_matching_def_passes(self):
        # Declaration first, definition later — also common.
        # `defined` flips False → True when the definition is seen.
        _, symbols = _check(
            "int main(void) { int foo(void); return 0; } "
            "int foo(void) { return 1; }"
        )
        self.assertTrue(symbols["foo"].defined)


class TestRedefinition(unittest.TestCase):
    """`int foo(void) { ... } int foo(void) { ... }` — two
    definitions of the same function. C linkers reject this; we
    catch it during type checking via the `defined` flag."""

    def test_two_definitions_raises(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int foo(void) { return 1; } "
                "int foo(void) { return 2; } "
                "int main(void) { return 0; }"
            )
        self.assertIn("redefinition", str(ctx.exception))

    def test_single_definition_passes(self):
        _, symbols = _check(
            "int foo(void) { return 1; } "
            "int main(void) { return foo(); }"
        )
        self.assertTrue(symbols["foo"].defined)


class TestRecursion(unittest.TestCase):
    """A function definition is registered in the symbol table
    *before* its body is checked, so the body can call itself."""

    def test_self_recursive_call_passes(self):
        # Should not raise.
        _, symbols = _check(
            "int main(void) { return main(); }"
        )
        self.assertTrue(symbols["main"].defined)

    def test_mutual_recursion_via_decl_passes(self):
        # `even` and `odd` calling each other. Forward-declared
        # before use because file-scope forward decls don't exist
        # yet, so the decls live inside main's body.
        _check(
            "int main(void) { "
            "int even(int n); int odd(int n); "
            "return even(0); }"
        )


class TestVarInitializer(unittest.TestCase):
    """`int x = <exp>;` — the initializer is type-checked; today
    every Int-yielding expression is a valid initializer for an
    Int variable. The 'function as initializer' case is covered
    in TestFunctionUsedAsVariable above."""

    def test_constant_init_passes(self):
        _check("int main(void) { int x = 5; return x; }")

    def test_compound_init_passes(self):
        _check(
            "int main(void) { int x = 1 + 2 * 3; return x; }"
        )

    def test_call_as_init_passes(self):
        _check(
            "int main(void) { int foo(void); int x = foo(); "
            "return x; }"
        )


class TestStatementsAndExpressions(unittest.TestCase):
    """Spot-check that each statement / expression form gets its
    sub-expressions type-checked. Today every Int-yielding sub-
    expression passes, so these mostly verify that no node is
    silently skipped."""

    def test_if_passes(self):
        _check(
            "int main(void) { int x = 0; "
            "if (x) return 1; else return 2; }"
        )

    def test_while_passes(self):
        _check(
            "int main(void) { int x = 0; while (x) x = x - 1; "
            "return x; }"
        )

    def test_for_passes(self):
        _check(
            "int main(void) { for (int i = 0; i < 10; i++) ; "
            "return 0; }"
        )

    def test_compound_passes(self):
        _check(
            "int main(void) { int x = 1; { int y = x + 1; } "
            "return x; }"
        )

    def test_call_arg_is_type_checked(self):
        # The args list is recursively type-checked. Here a call
        # whose arg is itself a (correctly-arity) call passes.
        _check(
            "int main(void) { "
            "int f(int a); int g(int a); "
            "return f(g(1)); }"
        )

    def test_call_arg_with_undeclared_inside_raises(self):
        # The arg expression `bar()` (which is undeclared as
        # function and undeclared as variable) raises in identifier
        # resolution before the type checker even sees it. We just
        # confirm the program is rejected — the rejection is from
        # an earlier pass, but still surfaces via `_check`.
        from passes.identifier_resolution import (
            IdentifierResolutionError,
        )
        with self.assertRaises(
            (TypeCheckError, IdentifierResolutionError),
        ):
            _check(
                "int main(void) { int f(int a); return f(bar()); }"
            )


class TestSymbolTableAPI(unittest.TestCase):
    """Direct exercises of `SymbolTable` and the synthetic-AST
    paths in `TypeChecker`. These don't go through the parser, so
    they isolate the pass's own logic from earlier-pass behavior."""

    def test_get_returns_none_for_missing(self):
        t = SymbolTable()
        self.assertIsNone(t.get("nope"))

    def test_contains(self):
        t = SymbolTable()
        t.add_variable("x", Int())
        self.assertIn("x", t)
        self.assertNotIn("y", t)

    def test_add_variable_twice_raises(self):
        # NONE-linkage names should be unique by the time they
        # reach the type checker; double-adding indicates an
        # internal-consistency bug.
        t = SymbolTable()
        t.add_variable("x", Int())
        with self.assertRaises(TypeCheckError):
            t.add_variable("x", Int())

    def test_add_function_twice_with_matching_sig(self):
        # First add: declaration. Second add: the matching
        # definition. Net: one entry, defined=True.
        t = SymbolTable()
        ftype = FunType(params=(Int(),), ret=Int())
        t.add_function("foo", ftype, defined=False)
        t.add_function("foo", ftype, defined=True)
        self.assertEqual(t["foo"], Symbol(type=ftype, defined=True))

    def test_add_function_redefinition_raises(self):
        t = SymbolTable()
        ftype = FunType(params=(), ret=Int())
        t.add_function("foo", ftype, defined=True)
        with self.assertRaises(TypeCheckError):
            t.add_function("foo", ftype, defined=True)

    def test_add_function_after_variable_raises(self):
        # Identifier kind switched mid-program: the variable entry
        # blocks a later function declaration of the same name.
        t = SymbolTable()
        t.add_variable("foo", Int())
        with self.assertRaises(TypeCheckError):
            t.add_function(
                "foo", FunType(params=(), ret=Int()),
                defined=False,
            )

    def test_add_function_signature_mismatch_raises(self):
        t = SymbolTable()
        t.add_function(
            "foo", FunType(params=(Int(),), ret=Int()),
            defined=False,
        )
        with self.assertRaises(TypeCheckError) as ctx:
            t.add_function(
                "foo", FunType(params=(Int(), Int()), ret=Int()),
                defined=False,
            )
        self.assertIn("incompatible", str(ctx.exception))


class TestProgramReturnedUnchanged(unittest.TestCase):
    """The pass doesn't modify the AST. The returned program should
    be the same Python object as the input, so callers can still
    chain it through later passes."""

    def test_returned_prog_is_input_prog(self):
        from passes.identifier_resolution import (
            resolve_program as resolve_identifiers,
        )
        prog_in = resolve_identifiers(
            parse("int main(void) { return 0; }"),
        )
        prog_out, _symbols = check_program(prog_in)
        self.assertIs(prog_out, prog_in)


class TestTypeEquality(unittest.TestCase):
    """The Type subclasses are frozen dataclasses; equality and
    hashability are the value-comparison semantics we depend on."""

    def test_int_equals_itself(self):
        self.assertEqual(Int(), Int())
        self.assertEqual(hash(Int()), hash(Int()))

    def test_funtype_equals_structurally(self):
        a = FunType(params=(Int(), Int()), ret=Int())
        b = FunType(params=(Int(), Int()), ret=Int())
        self.assertEqual(a, b)
        self.assertEqual(hash(a), hash(b))

    def test_funtype_arity_distinguishes(self):
        a = FunType(params=(Int(),), ret=Int())
        b = FunType(params=(Int(), Int()), ret=Int())
        self.assertNotEqual(a, b)


class TestIntegrationWithLaterPipeline(unittest.TestCase):
    """End-to-end smoke tests: a program that the type checker
    accepts must still flow through `c99_to_tac` (modulo the
    FunctionCall TODO). Programs the type checker rejects shouldn't
    silently get further down the pipeline."""

    def test_well_typed_program_passes_through_to_tac(self):
        from compile import _run_stage
        from preprocessor import preprocess
        out = _run_stage(
            "tac",
            preprocess("int main(void) { return 42; }", []),
        )
        self.assertIn("Ret(", out)

    def test_ill_typed_program_rejected_at_tac_stage(self):
        # `int x; x();` — caught by type-check, never reaches TAC.
        from compile import _run_stage
        from preprocessor import preprocess
        with self.assertRaises(TypeCheckError):
            _run_stage(
                "tac",
                preprocess(
                    "int main(void) { int x; x(); return 0; }", [],
                ),
            )


class TestErrors(unittest.TestCase):
    def test_unknown_program_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_program,), {})
        with self.assertRaises(TypeError):
            TypeChecker().check_program(stub())

    def test_unknown_statement_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_statement,), {})
        with self.assertRaises(TypeError):
            TypeChecker().check_statement(stub())

    def test_unknown_block_item_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_block_item,), {})
        with self.assertRaises(TypeError):
            TypeChecker().check_block_item(stub())

    def test_unknown_exp_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_exp,), {})
        with self.assertRaises(TypeError):
            TypeChecker().check_exp(stub())


if __name__ == "__main__":
    unittest.main()
