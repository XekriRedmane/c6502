import unittest

import c99_ast
from passes.type_checking import FunType, Int, LocalAttr, Long, Symbol, SymbolTable, TypeCheckError, TypeChecker


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


if __name__ == "__main__":
    unittest.main()
