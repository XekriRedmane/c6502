import unittest

from c99_ast import FunType, Int, Long


class TestTypeEquality(unittest.TestCase):
    """The data-type classes (Int, Long, FunType) live on the c99 AST
    now and are non-frozen `@dataclass`-generated, so they support
    structural equality but aren't hashable. The type checker compares
    types for equality, and the symbol table, which is keyed by name,
    never hashes them."""

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


if __name__ == "__main__":
    unittest.main()
