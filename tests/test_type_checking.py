import unittest

import c99_ast
from parser import parse
from passes.identifier_resolution import (
    resolve_program as resolve_identifiers,
)
from passes.type_checking import (
    FunAttr,
    FunType,
    Initial,
    Int,
    LocalAttr,
    Long,
    NoInitializer,
    StaticAttr,
    Symbol,
    SymbolTable,
    Tentative,
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
    lives there. Each entry's `attrs` carries the runtime category
    (FunAttr / StaticAttr / LocalAttr) plus the metadata that
    category needs."""

    def test_minimal_program_records_main_as_defined(self):
        _, symbols = _check("int main(void) { return 0; }")
        self.assertIn("main", symbols)
        sym = symbols["main"]
        self.assertEqual(sym.type, FunType(params=[], ret=Int()))
        self.assertIsInstance(sym.attrs, FunAttr)
        self.assertTrue(sym.attrs.defined)
        # `main` has no specifier → external linkage → is_global=True.
        self.assertTrue(sym.attrs.is_global)

    def test_local_variable_is_local_attr(self):
        _, symbols = _check("int main(void) { int x = 3; return x; }")
        # identifier_resolution renamed `x` to `@0.x`; type-check
        # records the same key. A plain block-scope `int x;` is an
        # automatic-storage local, so the attrs are LocalAttr —
        # no init tracking, no is_global flag (LocalAttr has no
        # fields by design).
        self.assertEqual(symbols["@0.x"].type, Int())
        self.assertIsInstance(symbols["@0.x"].attrs, LocalAttr)

    def test_function_decl_recorded_with_arity(self):
        _, symbols = _check(
            "int main(void) { int foo(int a, int b); return 0; }"
        )
        sym = symbols["foo"]
        self.assertEqual(sym.type, FunType(params=[Int(), Int()], ret=Int()))
        self.assertIsInstance(sym.attrs, FunAttr)
        self.assertFalse(sym.attrs.defined)
        # No specifier on a block-scope function decl → as-if-extern
        # → external linkage → is_global=True.
        self.assertTrue(sym.attrs.is_global)

    def test_function_definition_is_marked_defined(self):
        _, symbols = _check(
            "int foo(void) { return 1; } int main(void) { return 0; }"
        )
        self.assertTrue(symbols["foo"].attrs.defined)
        self.assertTrue(symbols["main"].attrs.defined)

    def test_parameters_recorded_as_local_attr(self):
        _, symbols = _check("int main(int x, int y) { return x + y; }")
        self.assertEqual(symbols["@0.x"].type, Int())
        self.assertIsInstance(symbols["@0.x"].attrs, LocalAttr)
        self.assertEqual(symbols["@1.y"].type, Int())
        self.assertIsInstance(symbols["@1.y"].attrs, LocalAttr)

    def test_function_decl_params_not_added_to_table(self):
        # Block-scope function-declaration param names get unique
        # renames during identifier_resolution, but they have no
        # body to be referenced from — type-check doesn't waste
        # symbol-table entries on them.
        _, symbols = _check(
            "int main(void) { int foo(int a); return 0; }"
        )
        self.assertNotIn("@0.a", symbols)


class TestStaticStorageAttrs(unittest.TestCase):
    """File-scope objects, block-scope `static`, and block-scope
    `extern` all land as `StaticAttr` entries with one of three
    initial-value tags. The c99_to_tac pass enumerates these to emit
    `StaticVariable` TAC instructions."""

    def test_file_scope_default_object_is_tentative(self):
        # `int x;` at file scope → external linkage, tentative
        # initializer (resolved to 0 at end-of-TU per §6.9.2.2).
        _, symbols = _check("int x; int main(void) { return 0; }")
        sym = symbols["x"]
        self.assertEqual(sym.type, Int())
        self.assertEqual(
            sym.attrs, StaticAttr(initial_value=Tentative(), is_global=True),
        )

    def test_file_scope_object_with_initializer_is_initial(self):
        _, symbols = _check("int x = 5; int main(void) { return 0; }")
        self.assertEqual(
            symbols["x"].attrs,
            StaticAttr(initial_value=Initial(value=5), is_global=True),
        )

    def test_static_at_file_scope_is_internal_linkage(self):
        # `static int x = 5;` → internal linkage (is_global=False).
        _, symbols = _check(
            "static int x = 5; int main(void) { return 0; }"
        )
        self.assertEqual(
            symbols["x"].attrs,
            StaticAttr(initial_value=Initial(value=5), is_global=False),
        )

    def test_static_at_file_scope_no_init_is_tentative_internal(self):
        _, symbols = _check(
            "static int x; int main(void) { return 0; }"
        )
        self.assertEqual(
            symbols["x"].attrs,
            StaticAttr(initial_value=Tentative(), is_global=False),
        )

    def test_extern_at_file_scope_no_init_is_no_initializer(self):
        _, symbols = _check(
            "extern int x; int main(void) { return 0; }"
        )
        # No prior decl, so extern at file scope picks up EXTERNAL
        # linkage; without an initializer it's a NoInitializer
        # reference, not a tentative definition.
        self.assertEqual(
            symbols["x"].attrs,
            StaticAttr(initial_value=NoInitializer(), is_global=True),
        )

    def test_extern_after_static_inherits_internal_linkage(self):
        # `static int x; extern int x;` — the extern follows the
        # prior visible decl's linkage (INTERNAL), and the merged
        # entry remains a tentative definition.
        _, symbols = _check(
            "static int x; extern int x; int main(void) { return 0; }"
        )
        self.assertEqual(
            symbols["x"].attrs,
            StaticAttr(initial_value=Tentative(), is_global=False),
        )

    def test_extern_then_initializer_promotes_to_initial(self):
        # `extern int x; int x = 5;` — second decl provides the
        # definition; merged initial value is Initial(5).
        _, symbols = _check(
            "extern int x; int x = 5; int main(void) { return 0; }"
        )
        self.assertEqual(
            symbols["x"].attrs,
            StaticAttr(initial_value=Initial(value=5), is_global=True),
        )

    def test_two_initializers_at_file_scope_raise(self):
        # `int x = 1; int x = 2;` — two definitions of the same
        # object. C99 §6.9.2 constraint violation.
        with self.assertRaises(TypeCheckError) as ctx:
            _check("int x = 1; int x = 2; int main(void) { return 0; }")
        self.assertIn("redefinition", str(ctx.exception))

    def test_block_scope_static_no_init_zero_initializes(self):
        # `static int x;` at block scope → C99 §6.7.8.10: zero-
        # initialized. NONE linkage → is_global=False.
        _, symbols = _check(
            "int main(void) { static int x; return x; }"
        )
        # identifier_resolution gave the block-scope static a unique
        # `@<N>.<orig>` because static at block scope has NONE
        # linkage (storage duration changes, not linkage).
        self.assertEqual(
            symbols["@0.x"].attrs,
            StaticAttr(initial_value=Initial(value=0), is_global=False),
        )

    def test_block_scope_static_with_init(self):
        _, symbols = _check(
            "int main(void) { static int x = 7; return x; }"
        )
        self.assertEqual(
            symbols["@0.x"].attrs,
            StaticAttr(initial_value=Initial(value=7), is_global=False),
        )

    def test_block_scope_extern_inherits_file_scope(self):
        # File-scope `static int x = 5;` then block-scope `extern
        # int x;` — the block decl inherits INTERNAL linkage and
        # records NoInitializer (definition is the file-scope decl).
        _, symbols = _check(
            "static int x = 5; "
            "int main(void) { extern int x; return x; }"
        )
        self.assertEqual(
            symbols["x"].attrs,
            StaticAttr(initial_value=Initial(value=5), is_global=False),
        )

    def test_block_scope_extern_with_initializer_raises(self):
        # C99 §6.7.8.5: a block-scope extern declaration shall have
        # no initializer.
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { extern int x = 5; return x; }"
            )
        self.assertIn("initializer", str(ctx.exception))

    def test_static_storage_initializer_must_be_constant(self):
        # `static int x = some_call();` — initializer for a
        # static-storage object must be a constant expression.
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int foo(void); "
                "static int x = foo(); return x; }"
            )
        self.assertIn("constant expression", str(ctx.exception))


class TestVariableUsedAsFunction(unittest.TestCase):
    def test_local_variable_called_as_function(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check("int main(void) { int x; x(); return 0; }")
        self.assertIn("called as a function", str(ctx.exception))

    def test_param_called_as_function(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check("int main(int x) { x(); return 0; }")
        self.assertIn("called as a function", str(ctx.exception))


class TestFunctionUsedAsVariable(unittest.TestCase):
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
        _check(
            "int main(void) { int foo(int a, int b); "
            "return foo(1, 2); }"
        )

    def test_no_args_passes(self):
        _check(
            "int main(void) { int foo(void); return foo(); }"
        )


class TestRedeclaration(unittest.TestCase):
    def test_matching_redeclaration_passes(self):
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
        _, symbols = _check(
            "int foo(void) { return 1; } "
            "int main(void) { int foo(void); return foo(); }"
        )
        self.assertTrue(symbols["foo"].attrs.defined)

    def test_decl_then_matching_def_passes(self):
        _, symbols = _check(
            "int main(void) { int foo(void); return 0; } "
            "int foo(void) { return 1; }"
        )
        self.assertTrue(symbols["foo"].attrs.defined)


class TestRedefinition(unittest.TestCase):
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
        self.assertTrue(symbols["foo"].attrs.defined)


class TestRecursion(unittest.TestCase):
    def test_self_recursive_call_passes(self):
        _, symbols = _check(
            "int main(void) { return main(); }"
        )
        self.assertTrue(symbols["main"].attrs.defined)

    def test_mutual_recursion_via_decl_passes(self):
        _check(
            "int main(void) { "
            "int even(int n); int odd(int n); "
            "return even(0); }"
        )


class TestVarInitializer(unittest.TestCase):
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
        _check(
            "int main(void) { "
            "int f(int a); int g(int a); "
            "return f(g(1)); }"
        )

    def test_call_arg_with_undeclared_inside_raises(self):
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
    """Direct exercises of `SymbolTable`. The table is now a thin
    `dict[str, Symbol]` wrapper — the merging logic moved to
    `TypeChecker`, so the assertion focus shifts to direct
    set/get/contains plus the high-level merge behavior tested via
    full programs above."""

    def test_get_returns_none_for_missing(self):
        t = SymbolTable()
        self.assertIsNone(t.get("nope"))

    def test_setitem_and_contains(self):
        t = SymbolTable()
        t["x"] = Symbol(type=Int(), attrs=LocalAttr())
        self.assertIn("x", t)
        self.assertNotIn("y", t)
        self.assertEqual(t["x"], Symbol(type=Int(), attrs=LocalAttr()))


class TestProgramReturnedUnchanged(unittest.TestCase):
    def test_returned_prog_is_input_prog(self):
        prog_in = resolve_identifiers(
            parse("int main(void) { return 0; }"),
        )
        prog_out, _symbols = check_program(prog_in)
        self.assertIs(prog_out, prog_in)


class TestTypeEquality(unittest.TestCase):
    """The data-type classes (Int, Long, FunType) live on the c99 AST
    now and are non-frozen `@dataclass`-generated, so they support
    structural equality but aren't hashable. Equality is what the
    type checker actually relies on; hashability isn't load-bearing
    for the symbol table (keyed by string name)."""

    def test_int_equals_itself(self):
        self.assertEqual(Int(), Int())

    def test_int_does_not_equal_long(self):
        self.assertNotEqual(Int(), Long())

    def test_funtype_equals_structurally(self):
        a = FunType(params=[Int(), Int()], ret=Int())
        b = FunType(params=[Int(), Int()], ret=Int())
        self.assertEqual(a, b)

    def test_funtype_distinguishes_param_types(self):
        a = FunType(params=[Int(), Int()], ret=Int())
        b = FunType(params=[Int(), Long()], ret=Int())
        self.assertNotEqual(a, b)

    def test_funtype_arity_distinguishes(self):
        a = FunType(params=[Int()], ret=Int())
        b = FunType(params=[Int(), Int()], ret=Int())
        self.assertNotEqual(a, b)


class TestIntegrationWithLaterPipeline(unittest.TestCase):
    def test_well_typed_program_passes_through_to_tac(self):
        from compile import _run_stage
        from preprocessor import preprocess
        out = _run_stage(
            "tac",
            preprocess("int main(void) { return 42; }", []),
        )
        self.assertIn("Ret(", out)

    def test_ill_typed_program_rejected_at_tac_stage(self):
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
            TypeChecker()._check_statement(stub())

    def test_unknown_block_item_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_block_item,), {})
        with self.assertRaises(TypeError):
            TypeChecker()._check_block_item(stub())

    def test_unknown_exp_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_exp,), {})
        with self.assertRaises(TypeError):
            TypeChecker()._check_exp(stub())


class TestLongAndCasts(unittest.TestCase):
    """End-to-end type-checking with `long` declarations and explicit
    casts. The strict rule: no implicit Int↔Long conversion. A cast is
    the only way to convert."""

    def test_long_variable_is_recorded_as_long(self):
        _, symbols = _check("long x; int main(void) { return 0; }")
        self.assertEqual(symbols["x"].type, Long())

    def test_long_local_is_recorded_as_long(self):
        _, symbols = _check(
            "int main(void) { long x = (long)5; return 0; }"
        )
        self.assertEqual(symbols["@0.x"].type, Long())

    def test_function_return_type_long(self):
        _, symbols = _check(
            "long foo(void); int main(void) { return 0; }"
        )
        self.assertEqual(
            symbols["foo"].type,
            FunType(params=[], ret=Long()),
        )

    def test_function_param_types(self):
        _, symbols = _check(
            "int foo(int a, long b); int main(void) { return 0; }"
        )
        self.assertEqual(
            symbols["foo"].type,
            FunType(params=[Int(), Long()], ret=Int()),
        )

    def test_int_init_with_long_literal_raises(self):
        # `int x = 200;` — 200 is a Long literal (doesn't fit in
        # signed 1 byte), so the initializer's type doesn't match the
        # declared `int`.
        with self.assertRaises(TypeCheckError) as ctx:
            _check("int main(void) { int x = 200; return 0; }")
        self.assertIn("type", str(ctx.exception).lower())

    def test_long_init_with_int_literal_raises(self):
        # `long x = 5;` — 5 is an Int literal, doesn't match declared
        # Long. User must write `(long)5`.
        with self.assertRaises(TypeCheckError) as ctx:
            _check("int main(void) { long x = 5; return 0; }")
        self.assertIn("type", str(ctx.exception).lower())

    def test_long_init_with_cast(self):
        # `long x = (long)5;` — explicit cast, should type-check.
        _check("int main(void) { long x = (long)5; return 0; }")

    def test_int_long_addition_without_cast_raises(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int a = 1; long b = (long)2; "
                "return a + b; }"
            )
        self.assertIn("disagree", str(ctx.exception))

    def test_int_long_addition_with_cast(self):
        # Promote the int to long before adding; result is long; cast
        # back to int for the return.
        _check(
            "int main(void) { int a = 1; long b = (long)2; "
            "return (int)((long)a + b); }"
        )

    def test_assignment_with_mismatched_types_raises(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int a; long b = (long)1; "
                "a = b; return 0; }"
            )
        self.assertIn("type mismatch", str(ctx.exception))

    def test_call_arg_type_mismatch_raises(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int foo(long x); "
                "int main(void) { return foo(1); }"
            )
        self.assertIn("argument", str(ctx.exception).lower())

    def test_call_arg_with_explicit_cast(self):
        _check(
            "int foo(long x); "
            "int main(void) { return foo((long)1); }"
        )

    def test_return_type_mismatch_raises(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check("long main(void) { return 5; }")
        self.assertIn("return", str(ctx.exception).lower())

    def test_return_type_with_explicit_cast(self):
        _check("long main(void) { return (long)5; }")

    def test_static_long_with_int_literal_raises(self):
        # `static long x = 5;` — 5 is an Int literal; the cast factory
        # rejects it because the variant doesn't match the declared
        # type.
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { static long x = 5; return 0; }"
            )
        self.assertIn("constant expression", str(ctx.exception).lower())

    def test_static_long_with_cast_initializer(self):
        # `static long x = (long)5;` — cast bridges the type, should
        # type-check.
        _, symbols = _check(
            "int main(void) { static long x = (long)5; return 0; }"
        )
        sym = symbols["@0.x"]
        self.assertEqual(sym.type, Long())
        self.assertEqual(
            sym.attrs,
            StaticAttr(initial_value=Initial(value=5), is_global=False),
        )

    def test_logical_not_returns_int_for_long_operand(self):
        # !x on a Long operand yields Int per C99 §6.5.3.3.5. The
        # surrounding context here forces the `!` result to be
        # comparable to an Int (= 0 → still int) — the type-check
        # passing means !long_x returned int.
        _check(
            "int main(void) { long x = (long)1; return !x; }"
        )

    def test_comparison_on_longs_returns_int(self):
        # `a == b` with both operands Long: result is Int, used as a
        # return value of int main → must match.
        _check(
            "int main(void) { long a = (long)1; long b = (long)2; "
            "return a == b; }"
        )

    def test_cast_target_must_be_object_type(self):
        # Casting to a function type isn't representable in our
        # grammar (type_name only accepts INT/LONG specifiers), so we
        # exercise this via a synthetic AST.
        from passes.identifier_resolution import resolve_program
        prog = c99_ast.Program(declaration=[c99_ast.FunctionDecl(
            function_decl=c99_ast.Type_function_decl(
                name="main",
                params=[],
                body=c99_ast.Block(block_item=[c99_ast.S(
                    statement=c99_ast.Return(exp=c99_ast.Cast(
                        target_type=c99_ast.FunType(
                            params=[], ret=c99_ast.Int(),
                        ),
                        exp=c99_ast.Constant(
                            const=c99_ast.ConstInt(int=0),
                        ),
                    )),
                )]),
                data_type=c99_ast.FunType(params=[], ret=c99_ast.Int()),
                storage_class=None,
            ),
        )])
        with self.assertRaises(TypeCheckError) as ctx:
            check_program(resolve_program(prog))
        self.assertIn("Int or Long", str(ctx.exception))


if __name__ == "__main__":
    unittest.main()
