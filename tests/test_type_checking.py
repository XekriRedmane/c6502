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

    def test_long_init_with_int_literal_inserts_cast(self):
        # `long x = 5;` — the int literal is converted to Long via
        # an implicit Cast on the initializer, same shape as
        # assignment / arg / return conversion.
        prog, _ = _check(
            "int main(void) { long x = 5; return 0; }"
        )
        items = prog.declaration[0].function_decl.body.block_item
        vd = items[0].declaration.var_decl
        self.assertEqual(vd.data_type, Long())
        self.assertIsInstance(vd.init, c99_ast.Cast)
        self.assertEqual(vd.init.target_type, Long())
        self.assertEqual(vd.init.data_type, Long())
        self.assertIsInstance(vd.init.exp, c99_ast.Constant)

    def test_int_init_with_long_literal_inserts_cast(self):
        # `int x = 200;` — 200 doesn't fit in signed 1 byte so it's
        # a ConstLong; the initializer gets wrapped in Cast(Int).
        prog, _ = _check(
            "int main(void) { int x = 200; return 0; }"
        )
        items = prog.declaration[0].function_decl.body.block_item
        vd = items[0].declaration.var_decl
        self.assertEqual(vd.data_type, Int())
        self.assertIsInstance(vd.init, c99_ast.Cast)
        self.assertEqual(vd.init.target_type, Int())

    def test_init_no_cast_when_types_match(self):
        # `long x = (long)5;` — the user-written cast produces a
        # Long-typed initializer that already matches the declared
        # type; the type checker doesn't add a redundant wrapper.
        prog, _ = _check(
            "int main(void) { long x = (long)5; return 0; }"
        )
        items = prog.declaration[0].function_decl.body.block_item
        vd = items[0].declaration.var_decl
        # `vd.init` is the user's Cast(Long, ConstInt(5)) — not
        # a wrapper Cast(Long, Cast(Long, ...)).
        self.assertIsInstance(vd.init, c99_ast.Cast)
        self.assertEqual(vd.init.target_type, Long())
        self.assertIsInstance(vd.init.exp, c99_ast.Constant)

    def test_int_long_addition_promotes_to_long(self):
        # `int_a + long_b` performs the usual arithmetic
        # conversions: the int operand is wrapped in an implicit
        # `Cast(Long)`, and the binary's result type is Long. The
        # int-returning function then narrows the Long back to Int
        # via an implicit Cast on the Return statement.
        prog, _ = _check(
            "int main(void) { int a = 1; long b = (long)2; "
            "return a + b; }"
        )
        items = prog.declaration[0].function_decl.body.block_item
        ret_stmt = items[2].statement
        # Outer Cast(Int) from the Return-conversion rule.
        self.assertIsInstance(ret_stmt.exp, c99_ast.Cast)
        self.assertEqual(ret_stmt.exp.target_type, Int())
        # Inside that, the Binary is Long-typed (post-promotion).
        binary = ret_stmt.exp.exp
        self.assertIsInstance(binary, c99_ast.Binary)
        self.assertEqual(binary.data_type, Long())
        # The Int operand `a` got wrapped in Cast(Long).
        self.assertIsInstance(binary.left, c99_ast.Cast)
        self.assertEqual(binary.left.target_type, Long())
        # The Long operand `b` passes through unchanged.
        self.assertIsInstance(binary.right, c99_ast.Var)
        self.assertEqual(binary.right.data_type, Long())

    def test_int_long_addition_with_outer_cast(self):
        # Wrap the (long) result in an explicit (int) so the return
        # type matches.
        _check(
            "int main(void) { int a = 1; long b = (long)2; "
            "return (int)(a + b); }"
        )

    def test_binary_promotion_inserts_implicit_cast(self):
        # After type-checking, the int operand is wrapped in a
        # `Cast(target=Long(), exp=..., data_type=Long())` so the
        # binary's two operands both have type Long. The Binary
        # itself carries data_type=Long().
        prog, _symbols = _check(
            "long main(void) { int a = 1; long b = (long)2; "
            "return (long)0 + (a + b); }"
        )
        # Drill into `return (long)0 + (a + b);` — the outer Binary's
        # right operand is `(a + b)`, which should be a Binary whose
        # int operand `a` was wrapped in an implicit Cast.
        items = prog.declaration[0].function_decl.body.block_item
        ret = items[2].statement
        outer_binary = ret.exp
        self.assertIsInstance(outer_binary, c99_ast.Binary)
        self.assertEqual(outer_binary.data_type, Long())
        inner_binary = outer_binary.right
        self.assertIsInstance(inner_binary, c99_ast.Binary)
        self.assertEqual(inner_binary.data_type, Long())
        # `a` (Int) wrapped in implicit Cast to Long; `b` (Long)
        # passes through unchanged.
        self.assertIsInstance(inner_binary.left, c99_ast.Cast)
        self.assertEqual(inner_binary.left.target_type, Long())
        self.assertEqual(inner_binary.left.data_type, Long())
        self.assertIsInstance(inner_binary.left.exp, c99_ast.Var)
        self.assertEqual(inner_binary.left.exp.data_type, Int())
        self.assertIsInstance(inner_binary.right, c99_ast.Var)
        self.assertEqual(inner_binary.right.data_type, Long())

    def test_assignment_widens_rval_with_implicit_cast(self):
        # `long_x = int_y;` — the rval is wrapped in an implicit
        # Cast(Long) so the assignment's two sides have matching
        # types. The Assignment node itself reports data_type Long.
        prog, _ = _check(
            "int main(void) { int a = 1; long b = (long)0; "
            "b = a; return 0; }"
        )
        items = prog.declaration[0].function_decl.body.block_item
        # `b = a;` is the third item (after the two declarations).
        assign = items[2].statement.exp
        self.assertIsInstance(assign, c99_ast.Assignment)
        self.assertEqual(assign.data_type, Long())
        # rval was an Int Var; now wrapped in Cast(target=Long).
        self.assertIsInstance(assign.rval, c99_ast.Cast)
        self.assertEqual(assign.rval.target_type, Long())
        self.assertEqual(assign.rval.data_type, Long())
        self.assertIsInstance(assign.rval.exp, c99_ast.Var)
        self.assertEqual(assign.rval.exp.data_type, Int())

    def test_assignment_narrows_rval_with_implicit_cast(self):
        # `int_x = long_y;` — symmetric: rval gets wrapped in
        # Cast(Int). Assignment's data_type is Int.
        prog, _ = _check(
            "int main(void) { int a = 0; long b = (long)200; "
            "a = b; return a; }"
        )
        items = prog.declaration[0].function_decl.body.block_item
        assign = items[2].statement.exp
        self.assertIsInstance(assign, c99_ast.Assignment)
        self.assertEqual(assign.data_type, Int())
        self.assertIsInstance(assign.rval, c99_ast.Cast)
        self.assertEqual(assign.rval.target_type, Int())
        self.assertEqual(assign.rval.data_type, Int())

    def test_assignment_no_cast_when_types_match(self):
        # `int_x = int_y;` — rval already has the right type, so no
        # Cast is inserted; rval stays as the original Var node.
        prog, _ = _check(
            "int main(void) { int a = 0; int b = 1; "
            "a = b; return a; }"
        )
        items = prog.declaration[0].function_decl.body.block_item
        assign = items[2].statement.exp
        self.assertIsInstance(assign, c99_ast.Assignment)
        self.assertEqual(assign.data_type, Int())
        # No Cast wrapper.
        self.assertIsInstance(assign.rval, c99_ast.Var)
        self.assertEqual(assign.rval.data_type, Int())

    def test_compound_assignment_to_int_narrows_long_result(self):
        # `int_x += long_y;` desugars to `int_x = int_x + long_y;`.
        # The Binary promotes both operands to Long (result Long),
        # then the Assignment narrows the rval back to Int with an
        # implicit Cast. Net effect: same as
        # `int_x = (int)((long)int_x + long_y);`.
        prog, _ = _check(
            "int main(void) { int a = 0; long b = (long)2; "
            "a += b; return a; }"
        )
        items = prog.declaration[0].function_decl.body.block_item
        assign = items[2].statement.exp
        self.assertIsInstance(assign, c99_ast.Assignment)
        self.assertEqual(assign.data_type, Int())
        # The rval (a Binary that produced Long) is now wrapped in
        # an implicit Cast(Int).
        self.assertIsInstance(assign.rval, c99_ast.Cast)
        self.assertEqual(assign.rval.target_type, Int())
        # The Binary inside the Cast still has data_type Long.
        self.assertIsInstance(assign.rval.exp, c99_ast.Binary)
        self.assertEqual(assign.rval.exp.data_type, Long())

    def test_call_arg_widens_with_implicit_cast(self):
        # `foo(int_lit)` where foo's param is Long — the int literal
        # is converted to Long via an implicit Cast on the arg list.
        prog, _ = _check(
            "int foo(long x); "
            "int main(void) { return foo(1); }"
        )
        # Drill into main's return: `return foo(1);` → call's args[0]
        # is now a Cast(Long, ConstInt(1)).
        ret = (
            prog.declaration[1].function_decl.body.block_item[0]
            .statement.exp
        )
        self.assertIsInstance(ret, c99_ast.FunctionCall)
        self.assertEqual(len(ret.args), 1)
        arg0 = ret.args[0]
        self.assertIsInstance(arg0, c99_ast.Cast)
        self.assertEqual(arg0.target_type, Long())
        self.assertEqual(arg0.data_type, Long())
        self.assertIsInstance(arg0.exp, c99_ast.Constant)
        self.assertEqual(arg0.exp.data_type, Int())

    def test_call_arg_narrows_with_implicit_cast(self):
        # Symmetric: `foo(long_var)` where foo's param is Int — the
        # Long arg gets wrapped in Cast(Int).
        prog, _ = _check(
            "int foo(int x); "
            "int main(void) { long y = (long)1; return foo(y); }"
        )
        items = prog.declaration[1].function_decl.body.block_item
        ret = items[1].statement.exp
        arg0 = ret.args[0]
        self.assertIsInstance(arg0, c99_ast.Cast)
        self.assertEqual(arg0.target_type, Int())
        self.assertEqual(arg0.data_type, Int())

    def test_call_arg_no_cast_when_types_match(self):
        # When the arg's type already matches the param, no Cast
        # wrapper is inserted; the original arg node passes through.
        prog, _ = _check(
            "int foo(int x); "
            "int main(void) { return foo(5); }"
        )
        ret = (
            prog.declaration[1].function_decl.body.block_item[0]
            .statement.exp
        )
        self.assertIsInstance(ret.args[0], c99_ast.Constant)
        self.assertEqual(ret.args[0].data_type, Int())

    def test_call_arity_mismatch_still_raises(self):
        # Conversion only applies once arity matches; the wrong
        # count of args still raises.
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int foo(int a, int b); "
                "int main(void) { return foo(1); }"
            )
        self.assertIn("expected 2", str(ctx.exception))

    def test_return_widens_value_with_implicit_cast(self):
        # `long main() { return 5; }` — the int literal is converted
        # to Long via an implicit Cast on the Return statement's exp.
        prog, _ = _check("long main(void) { return 5; }")
        ret_stmt = (
            prog.declaration[0].function_decl.body.block_item[0]
            .statement
        )
        self.assertIsInstance(ret_stmt, c99_ast.Return)
        self.assertIsInstance(ret_stmt.exp, c99_ast.Cast)
        self.assertEqual(ret_stmt.exp.target_type, Long())
        self.assertEqual(ret_stmt.exp.data_type, Long())

    def test_return_narrows_value_with_implicit_cast(self):
        # Symmetric: returning a Long from an int-returning function
        # narrows via implicit Cast(Int).
        prog, _ = _check(
            "int main(void) { long x = (long)5; return x; }"
        )
        items = prog.declaration[0].function_decl.body.block_item
        ret_stmt = items[1].statement
        self.assertIsInstance(ret_stmt.exp, c99_ast.Cast)
        self.assertEqual(ret_stmt.exp.target_type, Int())

    def test_return_no_cast_when_types_match(self):
        # No Cast wrapper when the return value's type already
        # matches the function's declared return type.
        prog, _ = _check("int main(void) { return 5; }")
        ret_stmt = (
            prog.declaration[0].function_decl.body.block_item[0]
            .statement
        )
        self.assertNotIsInstance(ret_stmt.exp, c99_ast.Cast)
        self.assertEqual(ret_stmt.exp.data_type, Int())

    def test_static_long_init_with_int_literal_converts(self):
        # `static long x = 5;` — the int literal is converted to
        # Long by the initializer-conversion rule. The Initial's
        # value is the underlying integer (5); codegen narrows /
        # widens to the declared type's width when laying out the
        # StaticVariable.
        _, symbols = _check(
            "int main(void) { static long x = 5; return 0; }"
        )
        sym = symbols["@0.x"]
        self.assertEqual(sym.type, Long())
        self.assertEqual(
            sym.attrs,
            StaticAttr(initial_value=Initial(value=5), is_global=False),
        )

    def test_static_long_with_explicit_cast_initializer(self):
        # `static long x = (long)5;` — explicit cast, also fine.
        _, symbols = _check(
            "int main(void) { static long x = (long)5; return 0; }"
        )
        sym = symbols["@0.x"]
        self.assertEqual(sym.type, Long())
        self.assertEqual(
            sym.attrs,
            StaticAttr(initial_value=Initial(value=5), is_global=False),
        )

    def test_static_init_with_non_constant_raises(self):
        # `static int x = a;` — `a` isn't a constant; the
        # initializer-conversion rule still inserts a Cast wrapper
        # for the type, but `_const_init_value` rejects the Var
        # at the bottom of the Cast chain because it's not a
        # constant expression.
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int a = 1; static int x = a; "
                "return 0; }"
            )
        self.assertIn("constant expression", str(ctx.exception).lower())

    def test_file_scope_long_init_with_int_literal(self):
        # `long x = 5;` at file scope — same conversion as block-
        # scope static. The Initial captures the underlying value.
        _, symbols = _check(
            "long x = 5; int main(void) { return 0; }"
        )
        self.assertEqual(
            symbols["x"].attrs,
            StaticAttr(initial_value=Initial(value=5), is_global=True),
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

    def test_data_type_set_on_constant_var_unary_postfix(self):
        # Round-trip through type-checking and verify the per-node
        # data_type is populated on every expression node.
        prog, _symbols = _check(
            "int main(void) { int a = 5; a++; return -a; }"
        )
        items = prog.declaration[0].function_decl.body.block_item
        # `int a = 5;` — Constant gets Int.
        init = items[0].declaration.var_decl.init
        self.assertIsInstance(init, c99_ast.Constant)
        self.assertEqual(init.data_type, Int())
        # `a++;` — the Postfix and the inner Var both Int.
        post = items[1].statement.exp
        self.assertIsInstance(post, c99_ast.Postfix)
        self.assertEqual(post.data_type, Int())
        self.assertEqual(post.operand.data_type, Int())
        # `return -a;` — Unary's data_type is Int (preserves operand).
        ret_exp = items[2].statement.exp
        self.assertIsInstance(ret_exp, c99_ast.Unary)
        self.assertEqual(ret_exp.data_type, Int())
        self.assertEqual(ret_exp.exp.data_type, Int())

    def test_logical_not_long_yields_int(self):
        # `!(long)1` — the inner Cast has data_type Long, but the
        # surrounding Unary(LogicalNot) reports data_type Int.
        prog, _ = _check(
            "int main(void) { return !((long)1); }"
        )
        ret_exp = (
            prog.declaration[0].function_decl.body.block_item[0]
            .statement.exp
        )
        self.assertIsInstance(ret_exp, c99_ast.Unary)
        self.assertIsInstance(ret_exp.op, c99_ast.LogicalNot)
        self.assertEqual(ret_exp.data_type, Int())
        self.assertEqual(ret_exp.exp.data_type, Long())

    def test_comparison_with_promotion_yields_int(self):
        # `int_a == long_b` — operands promoted to common type Long,
        # but the Binary's data_type is Int.
        prog, _ = _check(
            "int main(void) { int a = 1; long b = (long)2; "
            "return a == b; }"
        )
        ret_exp = (
            prog.declaration[0].function_decl.body.block_item[2]
            .statement.exp
        )
        self.assertIsInstance(ret_exp, c99_ast.Binary)
        self.assertIsInstance(ret_exp.op, c99_ast.Equal)
        self.assertEqual(ret_exp.data_type, Int())
        # `a` got wrapped in a Cast to Long for the comparison.
        self.assertIsInstance(ret_exp.left, c99_ast.Cast)
        self.assertEqual(ret_exp.left.data_type, Long())
        # `b` passes through as Long.
        self.assertEqual(ret_exp.right.data_type, Long())

    def test_conditional_branches_promote_to_common_type(self):
        # `cond ? int_t : long_f` — true branch is Int, false branch
        # is Long; common type is Long, true branch wrapped in Cast.
        prog, _ = _check(
            "long main(void) { int a = 1; long b = (long)2; "
            "return 1 ? a : b; }"
        )
        ret_exp = (
            prog.declaration[0].function_decl.body.block_item[2]
            .statement.exp
        )
        self.assertIsInstance(ret_exp, c99_ast.Conditional)
        self.assertEqual(ret_exp.data_type, Long())
        # The Int branch (`a`) got wrapped in a Cast to Long.
        self.assertIsInstance(ret_exp.true_clause, c99_ast.Cast)
        self.assertEqual(ret_exp.true_clause.data_type, Long())
        self.assertEqual(ret_exp.false_clause.data_type, Long())

    def test_no_implicit_cast_when_operands_already_match(self):
        # When both operands have the same type, no Cast wrapping
        # happens — operand nodes remain the originals.
        prog, _ = _check(
            "int main(void) { int a = 1; int b = 2; return a + b; }"
        )
        ret_exp = (
            prog.declaration[0].function_decl.body.block_item[2]
            .statement.exp
        )
        self.assertIsInstance(ret_exp, c99_ast.Binary)
        self.assertEqual(ret_exp.data_type, Int())
        self.assertIsInstance(ret_exp.left, c99_ast.Var)
        self.assertIsInstance(ret_exp.right, c99_ast.Var)

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
        self.assertIn("object type", str(ctx.exception))


def _ret_binary(prog):
    """Helper: pull the Binary out of `return <exp>;` from a program
    whose last block item is a Return. Most pointer-arithmetic tests
    just want to inspect that Binary's data_type and operand types."""
    items = prog.declaration[0].function_decl.body.block_item
    return items[-1].statement.exp


class TestPointerArithmetic(unittest.TestCase):
    """C99 §6.5.6 additive operators on pointers. Four valid shapes:
    `ptr + int`, `int + ptr`, `ptr - int` (each yields a pointer of
    the same type), and `ptr - ptr` (yields Long, c6502's stand-in
    for ptrdiff_t). Everything else is a constraint violation."""

    def test_pointer_plus_int_yields_pointer(self):
        from c99_ast import Pointer
        prog, _ = _check(
            "long main(void) { int a = 0; int *p = &a; return (long)(p + 1); }"
        )
        bin_exp = _ret_binary(prog).exp  # unwrap the (long) Cast
        self.assertIsInstance(bin_exp, c99_ast.Binary)
        self.assertEqual(bin_exp.data_type, Pointer(referenced_type=Int()))

    def test_int_plus_pointer_yields_pointer(self):
        # Commutative: `int + ptr` is the same shape as `ptr + int`.
        from c99_ast import Pointer
        prog, _ = _check(
            "long main(void) { int a = 0; int *p = &a; return (long)(1 + p); }"
        )
        bin_exp = _ret_binary(prog).exp
        self.assertIsInstance(bin_exp, c99_ast.Binary)
        self.assertEqual(bin_exp.data_type, Pointer(referenced_type=Int()))

    def test_pointer_minus_int_yields_pointer(self):
        from c99_ast import Pointer
        prog, _ = _check(
            "long main(void) { int a = 0; int *p = &a; return (long)(p - 1); }"
        )
        bin_exp = _ret_binary(prog).exp
        self.assertIsInstance(bin_exp, c99_ast.Binary)
        self.assertEqual(bin_exp.data_type, Pointer(referenced_type=Int()))

    def test_pointer_minus_pointer_yields_long(self):
        # Both pointers are int*, result is the byte-difference / 1
        # (c6502's ptrdiff_t is Long).
        prog, _ = _check(
            "long main(void) { int a = 0; int *p = &a; int *q = &a; "
            "return p - q; }"
        )
        bin_exp = _ret_binary(prog)
        self.assertIsInstance(bin_exp, c99_ast.Binary)
        self.assertEqual(bin_exp.data_type, Long())

    def test_int_operand_widened_to_long(self):
        # The integer operand of pointer arithmetic gets widened to
        # Long via an implicit Cast — pointers are 2 bytes wide, so
        # the underlying byte-level add operates at one width.
        prog, _ = _check(
            "long main(void) { int a = 0; int *p = &a; return (long)(p + 1); }"
        )
        bin_exp = _ret_binary(prog).exp
        self.assertIsInstance(bin_exp, c99_ast.Binary)
        # Right operand was `1` (Int); now wrapped in Cast(Long).
        self.assertIsInstance(bin_exp.right, c99_ast.Cast)
        self.assertEqual(bin_exp.right.target_type, Long())
        # Left operand `p` is the pointer, unchanged.
        from c99_ast import Pointer
        self.assertEqual(bin_exp.left.data_type, Pointer(referenced_type=Int()))

    def test_long_operand_passes_through(self):
        # If the integer operand is already Long, no implicit Cast is
        # inserted (matches `_convert_to`'s same-type identity).
        prog, _ = _check(
            "long main(void) { int a = 0; int *p = &a; long n = (long)1; "
            "return (long)(p + n); }"
        )
        bin_exp = _ret_binary(prog).exp
        self.assertIsInstance(bin_exp, c99_ast.Binary)
        self.assertIsInstance(bin_exp.right, c99_ast.Var)
        self.assertEqual(bin_exp.right.data_type, Long())

    def test_pointer_to_long_arithmetic_yields_pointer_to_long(self):
        # The pointer's referenced_type is preserved across the
        # arithmetic — a `long *p + 1` is still a `long *`.
        from c99_ast import Pointer
        prog, _ = _check(
            "long main(void) { long a = (long)0; long *p = &a; "
            "return (long)(p + 1); }"
        )
        bin_exp = _ret_binary(prog).exp
        self.assertIsInstance(bin_exp, c99_ast.Binary)
        self.assertEqual(bin_exp.data_type, Pointer(referenced_type=Long()))

    def test_pointer_plus_pointer_rejected(self):
        # `ptr + ptr` is undefined per §6.5.6.2 — not a constraint
        # the standard allows, since adding two addresses is
        # meaningless.
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int a = 0; int *p = &a; int *q = &a; "
                "return (int)(p + q); }"
            )
        self.assertIn("'+'", str(ctx.exception))
        self.assertIn("two pointer", str(ctx.exception))

    def test_int_minus_pointer_rejected(self):
        # `int - ptr` is undefined per §6.5.6.2; only `ptr - int` and
        # `ptr - ptr` are legal.
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "long main(void) { int a = 0; int *p = &a; "
                "return (long)(1 - p); }"
            )
        self.assertIn("'-'", str(ctx.exception))

    def test_pointer_minus_distinct_pointer_rejected(self):
        # ptr - ptr requires matching pointer types per §6.5.6.3.
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "long main(void) { int a = 0; long b = (long)0; "
                "int *p = &a; long *q = &b; return p - q; }"
            )
        self.assertIn("distinct pointer", str(ctx.exception))

    def test_pointer_plus_float_rejected(self):
        # FP isn't an integer type, so `ptr + double` violates the
        # §6.5.6.2 constraint that the non-pointer operand have
        # integer type.
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "long main(void) { int a = 0; int *p = &a; "
                "return (long)(p + 1.0); }"
            )
        self.assertIn("floating-point", str(ctx.exception))

    def test_pointer_minus_double_rejected(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "long main(void) { int a = 0; int *p = &a; "
                "return (long)(p - 1.0); }"
            )
        self.assertIn("floating-point", str(ctx.exception))

    def test_pointer_plus_zero_accepted(self):
        # `p + 0` is the C identity for the pointer — accepted.
        from c99_ast import Pointer
        prog, _ = _check(
            "long main(void) { int a = 0; int *p = &a; "
            "return (long)(p + 0); }"
        )
        bin_exp = _ret_binary(prog).exp
        self.assertEqual(bin_exp.data_type, Pointer(referenced_type=Int()))

    def test_zero_plus_pointer_accepted(self):
        # `0 + p` is commutatively the same identity.
        from c99_ast import Pointer
        prog, _ = _check(
            "long main(void) { int a = 0; int *p = &a; "
            "return (long)(0 + p); }"
        )
        bin_exp = _ret_binary(prog).exp
        self.assertEqual(bin_exp.data_type, Pointer(referenced_type=Int()))

    def test_pointer_minus_zero_accepted(self):
        # `p - 0` is the C identity for the pointer — accepted.
        # (Only `0 - p` is rejected, by the existing `int - ptr` rule.)
        from c99_ast import Pointer
        prog, _ = _check(
            "long main(void) { int a = 0; int *p = &a; "
            "return (long)(p - 0); }"
        )
        bin_exp = _ret_binary(prog).exp
        self.assertEqual(bin_exp.data_type, Pointer(referenced_type=Int()))

    def test_zero_minus_pointer_rejected(self):
        # `0 - p` falls under the existing `int - ptr` constraint
        # violation per C99 §6.5.6.2.
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "long main(void) { int a = 0; int *p = &a; "
                "return (long)(0 - p); }"
            )
        self.assertIn("'-'", str(ctx.exception))


class TestPointerOrdering(unittest.TestCase):
    """C99 §6.5.8 relational operators on pointers. Both operands must
    be pointers to compatible object types; the result is Int. Unlike
    equality (§6.5.9.2) the relational ops don't accept null pointer
    constants."""

    def test_pointer_lt_pointer_yields_int(self):
        prog, _ = _check(
            "int main(void) { int a = 0; int *p = &a; int *q = &a; "
            "return p < q; }"
        )
        bin_exp = _ret_binary(prog)
        self.assertIsInstance(bin_exp, c99_ast.Binary)
        self.assertIsInstance(bin_exp.op, c99_ast.LessThan)
        self.assertEqual(bin_exp.data_type, Int())

    def test_pointer_gt_pointer_yields_int(self):
        prog, _ = _check(
            "int main(void) { int a = 0; int *p = &a; int *q = &a; "
            "return p > q; }"
        )
        bin_exp = _ret_binary(prog)
        self.assertIsInstance(bin_exp.op, c99_ast.GreaterThan)
        self.assertEqual(bin_exp.data_type, Int())

    def test_pointer_le_pointer_yields_int(self):
        prog, _ = _check(
            "int main(void) { int a = 0; int *p = &a; int *q = &a; "
            "return p <= q; }"
        )
        bin_exp = _ret_binary(prog)
        self.assertIsInstance(bin_exp.op, c99_ast.LessOrEqual)
        self.assertEqual(bin_exp.data_type, Int())

    def test_pointer_ge_pointer_yields_int(self):
        prog, _ = _check(
            "int main(void) { int a = 0; int *p = &a; int *q = &a; "
            "return p >= q; }"
        )
        bin_exp = _ret_binary(prog)
        self.assertIsInstance(bin_exp.op, c99_ast.GreaterOrEqual)
        self.assertEqual(bin_exp.data_type, Int())

    def test_pointer_to_long_ordering(self):
        # Same-type pointers to Long are also legal — the type rule
        # is about the pointer types matching, not the pointee being
        # int-typed.
        prog, _ = _check(
            "int main(void) { long a = (long)0; long *p = &a; "
            "long *q = &a; return p < q; }"
        )
        bin_exp = _ret_binary(prog)
        self.assertEqual(bin_exp.data_type, Int())

    def test_array_decay_in_ordering(self):
        # `a < b` where both are arrays: each decays to a pointer of
        # the same type, and the relational compares those.
        prog, _ = _check(
            "int main(void) { int a[10]; int b[10]; return a < b; }"
        )
        bin_exp = _ret_binary(prog)
        self.assertIsInstance(bin_exp, c99_ast.Binary)
        self.assertEqual(bin_exp.data_type, Int())
        # Both sides got wrapped in implicit AddressOf via decay.
        self.assertIsInstance(bin_exp.left, c99_ast.AddressOf)
        self.assertIsInstance(bin_exp.right, c99_ast.AddressOf)

    def test_distinct_pointer_types_rejected(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int a = 0; long b = (long)0; "
                "int *p = &a; long *q = &b; return p < q; }"
            )
        self.assertIn("distinct pointer", str(ctx.exception))

    def test_pointer_vs_int_rejected(self):
        # No null-pointer-constant exception for relational ops.
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int a = 0; int *p = &a; "
                "return p < 0; }"
            )
        self.assertIn("non-pointer", str(ctx.exception))

    def test_int_vs_pointer_rejected(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int a = 0; int *p = &a; "
                "return 0 < p; }"
            )
        self.assertIn("non-pointer", str(ctx.exception))


class TestArrays(unittest.TestCase):
    """Block-scope array declarations and the four contexts where
    array-to-pointer decay applies (C99 §6.3.2.1.3): subscript array
    operand, Binary operand, Conditional branch, Assignment rval.
    The decay is reified as an `AddressOf(exp)` wrapper stamped with
    `Pointer(elem)` — narrower than the strict C99 `Pointer(Array(
    elem, N))`, but matches the runtime address (the address of the
    array's first element)."""

    def test_array_decl_records_array_type(self):
        from c99_ast import Array
        _, symbols = _check(
            "int main(void) { int a[10]; return 0; }"
        )
        # Block-scope decl renamed to `@0.a` by identifier_resolution.
        sym = symbols["@0.a"]
        self.assertEqual(sym.type, Array(element_type=Int(), size=10))
        self.assertIsInstance(sym.attrs, LocalAttr)

    def test_subscript_yields_element_type(self):
        # `a[i]` where a is `int[10]` has type Int.
        prog, _ = _check(
            "int main(void) { int a[10]; return a[3]; }"
        )
        items = prog.declaration[0].function_decl.body.block_item
        ret_exp = items[1].statement.exp
        self.assertIsInstance(ret_exp, c99_ast.Subscript)
        self.assertEqual(ret_exp.data_type, Int())

    def test_subscript_array_operand_decays_to_pointer(self):
        # The Subscript's `array` field gets wrapped in an AddressOf
        # with type Pointer(elem) — that's the reified decay.
        from c99_ast import Pointer
        prog, _ = _check(
            "int main(void) { int a[10]; return a[3]; }"
        )
        ret_exp = (
            prog.declaration[0].function_decl.body.block_item[1]
            .statement.exp
        )
        self.assertIsInstance(ret_exp.array, c99_ast.AddressOf)
        self.assertEqual(
            ret_exp.array.data_type, Pointer(referenced_type=Int()),
        )

    def test_subscript_index_widened_to_long(self):
        # The index gets a Cast(Long) wrapper so the underlying
        # pointer arithmetic operates at one width.
        prog, _ = _check(
            "int main(void) { int a[10]; return a[3]; }"
        )
        ret_exp = (
            prog.declaration[0].function_decl.body.block_item[1]
            .statement.exp
        )
        self.assertIsInstance(ret_exp.index, c99_ast.Cast)
        self.assertEqual(ret_exp.index.target_type, Long())

    def test_pointer_init_from_array_decays(self):
        # `int *p = a;` — the rval `a` (array) decays to `&a` (pointer
        # to first element) before the pointer-init conversion fires.
        from c99_ast import Pointer
        prog, _ = _check(
            "int main(void) { int a[10]; int *p = a; return 0; }"
        )
        items = prog.declaration[0].function_decl.body.block_item
        # `int *p = a;` is the second item.
        p_decl = items[1].declaration.var_decl
        self.assertEqual(p_decl.data_type, Pointer(referenced_type=Int()))
        # The init was an array-typed Var; now wrapped in AddressOf.
        self.assertIsInstance(p_decl.init, c99_ast.AddressOf)
        self.assertEqual(
            p_decl.init.data_type, Pointer(referenced_type=Int()),
        )

    def test_pointer_arithmetic_on_array(self):
        # `a + 1` where a is `int[10]` — array decays to `int *`,
        # then pointer arithmetic kicks in. Result is `int *`.
        from c99_ast import Pointer
        prog, _ = _check(
            "long main(void) { int a[10]; return (long)(a + 1); }"
        )
        ret_exp = (
            prog.declaration[0].function_decl.body.block_item[1]
            .statement.exp
        )
        # Outer Cast(long); inner Binary is the `a + 1`.
        binary = ret_exp.exp
        self.assertIsInstance(binary, c99_ast.Binary)
        self.assertEqual(
            binary.data_type, Pointer(referenced_type=Int()),
        )

    def test_array_minus_array_yields_long(self):
        # `a - b` where both are `int[N]` — both decay to `int *`,
        # then ptr - ptr yields Long (ptrdiff_t).
        prog, _ = _check(
            "long main(void) { int a[10]; int b[10]; return a - b; }"
        )
        ret_exp = (
            prog.declaration[0].function_decl.body.block_item[2]
            .statement.exp
        )
        self.assertIsInstance(ret_exp, c99_ast.Binary)
        self.assertEqual(ret_exp.data_type, Long())

    def test_assigning_to_array_rejected(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int a[10]; int b[10]; a = b; return 0; }"
            )
        self.assertIn("array", str(ctx.exception))

    def test_scalar_initializer_for_array_rejected(self):
        # `int a[3] = 5;` (scalar init for an array) — must use a
        # brace-enclosed initializer list instead.
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int a[3] = 5; return 0; }"
            )
        self.assertIn("brace-enclosed initializer", str(ctx.exception))

    def test_extern_array_rejected(self):
        # `extern T a[N];` would need to defer the static-init to
        # whichever TU defines the array — c6502's symbol model
        # doesn't support that yet.
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { extern int a[10]; return 0; }"
            )
        self.assertIn("extern arrays", str(ctx.exception))

    def test_static_array_with_init_list(self):
        # Block-scope `static int a[3] = {1, 2, 3};` produces a
        # StaticAttr whose `value` is a 3-element tuple matching
        # the declared array size.
        _, symbols = _check(
            "int main(void) { static int a[3] = {1, 2, 3}; "
            "return a[0]; }"
        )
        sym = symbols["@0.a"]
        self.assertIsInstance(sym.attrs, StaticAttr)
        self.assertIsInstance(sym.attrs.initial_value, Initial)
        self.assertEqual(sym.attrs.initial_value.value, (1, 2, 3))

    def test_file_scope_array_with_init_list(self):
        _, symbols = _check(
            "int a[3] = {10, 20, 30}; int main(void) { return a[0]; }"
        )
        sym = symbols["a"]
        self.assertIsInstance(sym.attrs, StaticAttr)
        self.assertEqual(sym.attrs.initial_value.value, (10, 20, 30))

    def test_static_array_partial_init_zero_pads(self):
        # `static int a[5] = {1, 2};` zero-pads the trailing 3
        # entries per C99 §6.7.8.21.
        _, symbols = _check(
            "int main(void) { static int a[5] = {1, 2}; "
            "return a[0]; }"
        )
        sym = symbols["@0.a"]
        self.assertEqual(sym.attrs.initial_value.value, (1, 2, 0, 0, 0))

    def test_static_array_no_init_zeroes(self):
        # `static int a[3];` (no init) is zero-initialized per
        # C99 §6.7.8.10.
        _, symbols = _check(
            "int main(void) { static int a[3]; return a[0]; }"
        )
        sym = symbols["@0.a"]
        self.assertEqual(sym.attrs.initial_value.value, (0, 0, 0))

    def test_static_multi_dim_array_with_init_list(self):
        # `static int nested[3][2] = {{1,2},{3,4},{5,6}};` →
        # value tuple is ((1,2),(3,4),(5,6)).
        _, symbols = _check(
            "int main(void) { "
            "static int nested[3][2] = {{1,2},{3,4},{5,6}}; "
            "return nested[1][1]; }"
        )
        sym = symbols["@0.nested"]
        self.assertEqual(
            sym.attrs.initial_value.value,
            ((1, 2), (3, 4), (5, 6)),
        )

    def test_static_long_array_with_init_list(self):
        # The init values pass through `_convert_to`, so an int
        # literal initializing a long element gets the right
        # underlying value.
        _, symbols = _check(
            "int main(void) { static long a[3] = {1, 2, 3}; "
            "return 0; }"
        )
        sym = symbols["@0.a"]
        self.assertEqual(sym.attrs.initial_value.value, (1, 2, 3))

    def test_address_of_array_typed_as_pointer_to_array(self):
        # `&arr` for an array yields `Pointer(Array(elem, N))` per
        # C99 §6.5.3.2.3. `_to_tac_data_type` collapses Pointer to
        # Long and `_pointee_size` recurses into Array, so the rest
        # of the pipeline handles this fine.
        prog, _ = _check(
            "int main(void) { int a[10]; int (*p)[10] = &a; return 0; }"
        )
        # Find the AddressOf inside the var-decl initializer.
        body = prog.declaration[0].function_decl.body
        decl = body.block_item[1].declaration.var_decl
        addr = decl.init
        self.assertIsInstance(addr, c99_ast.AddressOf)
        self.assertEqual(
            addr.data_type,
            c99_ast.Pointer(referenced_type=c99_ast.Array(
                element_type=c99_ast.Int(), size=10,
            )),
        )

    def test_subscript_index_must_be_integer(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int a[10]; double i = 1.0; "
                "return a[i]; }"
            )
        self.assertIn("integer", str(ctx.exception))

    def test_array_param_adjusts_to_pointer_in_symbol_table(self):
        # C99 §6.7.5.3.7: `int foo(int a[3])` adjusts the parameter
        # type to `int *`. The function's FunType records the
        # adjusted type, AND the parameter `a`'s symbol-table entry
        # is a `Pointer(Int)` LocalAttr — so subscript on `a` inside
        # the body goes through the pointer-subscript path.
        from c99_ast import Pointer
        _, symbols = _check(
            "int foo(int a[3]) { return a[0]; } "
            "int main(void) { return 0; }"
        )
        self.assertEqual(
            symbols["foo"].type.params, [Pointer(referenced_type=Int())],
        )
        # Param is renamed to `@0.a` by identifier_resolution.
        self.assertEqual(
            symbols["@0.a"].type, Pointer(referenced_type=Int()),
        )

    def test_array_param_decl_compatible_with_pointer_param_def(self):
        # Two declarations of the same function should be compatible
        # after the §6.7.5.3.7 adjustment — `int foo(int a[3]);`
        # forward-declared then defined as `int foo(int *a)` should
        # type-check cleanly.
        _check(
            "int foo(int a[3]); "
            "int foo(int *a) { return a[0]; } "
            "int main(void) { return 0; }"
        )

    def test_passing_array_to_array_param_decays(self):
        # `foo(arr)` where foo takes `int a[3]` (adjusted to int*)
        # and arr is `int[3]` — the array decays at the call site,
        # matching the parameter's adjusted type.
        _check(
            "int foo(int a[3]) { return a[0]; } "
            "int main(void) { int arr[3]; return foo(arr); }"
        )

    def test_array_init_list_stamps_data_type_and_converts_items(self):
        # `int a[3] = {1, 2, 3};` — InitList gets data_type=
        # Array(Int, 3), and each item is type-checked (here Int
        # constants matching the element type — no Cast wrapping).
        from c99_ast import Array
        prog, _ = _check(
            "int main(void) { int a[3] = {1, 2, 3}; return 0; }"
        )
        items = prog.declaration[0].function_decl.body.block_item
        decl = items[0].declaration.var_decl
        self.assertIsInstance(decl.init, c99_ast.InitList)
        self.assertEqual(decl.init.data_type, Array(Int(), 3))
        for it in decl.init.items:
            self.assertEqual(it.data_type, Int())

    def test_array_init_list_widens_items_with_cast(self):
        # `long a[2] = {1, 2};` — items are Int constants but
        # element type is Long, so each item is wrapped in Cast(Long)
        # by `_convert_to`.
        prog, _ = _check(
            "int main(void) { long a[2] = {1, 2}; return 0; }"
        )
        decl = (
            prog.declaration[0].function_decl.body.block_item[0]
            .declaration.var_decl
        )
        for it in decl.init.items:
            self.assertIsInstance(it, c99_ast.Cast)
            self.assertEqual(it.target_type, Long())

    def test_too_many_initializers_rejected(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int a[2] = {1, 2, 3}; return 0; }"
            )
        self.assertIn("too many initializers", str(ctx.exception))

    def test_short_init_list_accepted(self):
        # Fewer items than the array size — legal; c99_to_tac pads
        # the rest with zero-of-element-type Stores.
        _check(
            "int main(void) { int a[5] = {1, 2}; return 0; }"
        )

    def test_init_list_for_scalar_rejected(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int x = {1, 2}; return 0; }"
            )
        self.assertIn("brace-enclosed", str(ctx.exception))

    def test_scalar_init_for_array_rejected(self):
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int a[3] = 5; return 0; }"
            )
        self.assertIn("brace-enclosed", str(ctx.exception))

    def test_nested_init_list_accepted_for_multi_dim(self):
        # `int a[2][3] = {{1,2,3},{4,5,6}};` — both inner InitLists
        # type-check against the inner Array type.
        from c99_ast import Array
        prog, _ = _check(
            "int main(void) { int a[2][3] = {{1,2,3},{4,5,6}}; return 0; }"
        )
        decl = (
            prog.declaration[0].function_decl.body.block_item[0]
            .declaration.var_decl
        )
        self.assertEqual(
            decl.init.data_type, Array(Array(Int(), 3), 2),
        )
        # Each top-level item is itself a typed InitList.
        for sub in decl.init.items:
            self.assertIsInstance(sub, c99_ast.InitList)
            self.assertEqual(sub.data_type, Array(Int(), 3))

    def test_flat_init_for_multi_dim_rejected(self):
        # `int a[2][3] = {1,2,3,4,5,6};` — C99 §6.7.8 allows this
        # via the "subaggregate" rule, but our type checker only
        # supports the fully-nested form. Flat forms with too many
        # outer items hit "too many initializers"; ones that fit the
        # outer count fail with "expected nested initializer".
        with self.assertRaises(TypeCheckError):
            _check(
                "int main(void) { int a[2][3] = {1,2,3,4,5,6}; return 0; }"
            )

    def test_flat_init_for_multi_dim_short_rejected(self):
        # `int a[3][3] = {1, 2, 3};` — fits the outer count (3) but
        # the items are scalars where InitLists are required.
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int a[3][3] = {1, 2, 3}; return 0; }"
            )
        self.assertIn("expected nested initializer", str(ctx.exception))

    def test_nested_init_extra_braces_rejected(self):
        # `int a[3] = {{1,2,3}};` — element type is Int (not an
        # array), so a nested `{1,2,3}` is the wrong shape.
        with self.assertRaises(TypeCheckError) as ctx:
            _check(
                "int main(void) { int a[3] = {{1,2,3}}; return 0; }"
            )
        self.assertIn("unexpected nested initializer", str(ctx.exception))

    def test_init_list_in_expression_position_rejected(self):
        # The grammar doesn't allow `{1, 2}` as a regular expression
        # so user source can't reach the type-checker's InitList
        # reject — but a synthesized AST can. Build the rejected
        # shape directly so the defensive case stays covered.
        from passes.identifier_resolution import resolve_program
        prog = c99_ast.Program(declaration=[c99_ast.FunctionDecl(
            function_decl=c99_ast.Type_function_decl(
                name="main",
                params=[],
                body=c99_ast.Block(block_item=[c99_ast.S(
                    statement=c99_ast.Return(exp=c99_ast.InitList(
                        items=[
                            c99_ast.Constant(
                                const=c99_ast.ConstInt(int=1),
                            ),
                            c99_ast.Constant(
                                const=c99_ast.ConstInt(int=2),
                            ),
                        ],
                    )),
                )]),
                data_type=c99_ast.FunType(params=[], ret=c99_ast.Int()),
                storage_class=None,
            ),
        )])
        with self.assertRaises(TypeCheckError) as ctx:
            check_program(resolve_program(prog))
        self.assertIn(
            "brace-enclosed initializer", str(ctx.exception),
        )

    def test_two_dim_subscript_yields_element_type(self):
        # `a[i][j]` for `int a[3][4]` — outer Subscript has data_type
        # Int (the leaf type); inner Subscript has data_type
        # Array(Int, 4) which decays via AddressOf for the outer
        # pointer-arithmetic path.
        from c99_ast import Array, Pointer
        prog, _ = _check(
            "int main(void) { int a[3][4]; return a[1][2]; }"
        )
        ret_exp = (
            prog.declaration[0].function_decl.body.block_item[1]
            .statement.exp
        )
        self.assertIsInstance(ret_exp, c99_ast.Subscript)
        self.assertEqual(ret_exp.data_type, Int())
        # The outer's array operand is AddressOf wrapping the inner
        # Subscript (the decay reification).
        self.assertIsInstance(ret_exp.array, c99_ast.AddressOf)
        self.assertEqual(
            ret_exp.array.data_type, Pointer(referenced_type=Int()),
        )
        # The inner Subscript stamps Array(Int, 4) — the type before
        # the outer's decay.
        inner = ret_exp.array.exp
        self.assertIsInstance(inner, c99_ast.Subscript)
        self.assertEqual(inner.data_type, Array(Int(), 4))

    def test_address_of_subscript_yields_pointer_to_element(self):
        # `&a[i]` ≡ `a + i` per C99 §6.5.3.2.3 — the result is a
        # pointer to the element type. Works for both pointer
        # operands (`&p[i]`) and array operands (`&arr[i]`, where
        # arr decays to a pointer first).
        from c99_ast import Pointer
        prog, _ = _check(
            "long main(void) { int x = 0; int *p = &x; "
            "int *q = &p[3]; return (long)q; }"
        )
        items = prog.declaration[0].function_decl.body.block_item
        # Third item is `int *q = &p[3];`.
        q_decl = items[2].declaration.var_decl
        self.assertEqual(
            q_decl.data_type, Pointer(referenced_type=Int()),
        )
        self.assertIsInstance(q_decl.init, c99_ast.AddressOf)
        self.assertEqual(
            q_decl.init.data_type, Pointer(referenced_type=Int()),
        )
        # The inner is the Subscript that `&` is taking the address
        # of; its data_type is the element type (Int).
        self.assertIsInstance(q_decl.init.exp, c99_ast.Subscript)
        self.assertEqual(q_decl.init.exp.data_type, Int())

    def test_address_of_array_subscript(self):
        # `&arr[i]` for `int arr[10]` — arr decays inside the
        # Subscript, then `&` of the (Int-typed) Subscript yields
        # `int *`.
        from c99_ast import Pointer
        prog, _ = _check(
            "long main(void) { int arr[10]; int *q = &arr[3]; "
            "return (long)q; }"
        )
        items = prog.declaration[0].function_decl.body.block_item
        q_decl = items[1].declaration.var_decl
        self.assertEqual(
            q_decl.data_type, Pointer(referenced_type=Int()),
        )
        self.assertIsInstance(q_decl.init, c99_ast.AddressOf)

    def test_pointer_subscript_works_too(self):
        # `p[i]` where p is `int *` — same shape as `a[i]` but the
        # array-decay step is a no-op. Result is the pointee type.
        prog, _ = _check(
            "int main(void) { int a[10]; int *p = a; return p[3]; }"
        )
        ret_exp = (
            prog.declaration[0].function_decl.body.block_item[2]
            .statement.exp
        )
        self.assertIsInstance(ret_exp, c99_ast.Subscript)
        self.assertEqual(ret_exp.data_type, Int())
        # The array operand here is the bare Var(p) — no AddressOf
        # wrapper since p was already pointer-typed.
        self.assertIsInstance(ret_exp.array, c99_ast.Var)


if __name__ == "__main__":
    unittest.main()
