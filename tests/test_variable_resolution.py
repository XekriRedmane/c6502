import unittest

import c99_ast
from parser import parse
from passes.variable_resolution import (
    Resolver,
    VariableResolutionError,
    resolve_function,
    resolve_program,
)


def _function(*body_items) -> c99_ast.Type_function_definition:
    return c99_ast.Function(name="main", body=list(body_items))


def _decl(name, init=None) -> c99_ast.Type_block_item:
    return c99_ast.D(
        declaration=c99_ast.Declaration(name=name, init=init),
    )


def _expr(exp) -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.Expression(exp=exp))


def _ret(exp) -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.Return(exp=exp))


def _null() -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.Null())


class TestMakeUnique(unittest.TestCase):
    def test_format_is_at_counter_dot_original(self):
        r = Resolver()
        self.assertEqual(r.make_unique("x"), "@0.x")
        self.assertEqual(r.make_unique("y"), "@1.y")

    def test_counter_is_global_across_calls(self):
        # Even the same original name gets a fresh number each time,
        # so names stay program-unique rather than scope-unique.
        r = Resolver()
        self.assertEqual(r.make_unique("a"), "@0.a")
        self.assertEqual(r.make_unique("a"), "@1.a")


class TestDeclarations(unittest.TestCase):
    def test_bare_declaration_renames(self):
        fn = _function(_decl("a"))
        resolved = resolve_function(fn)
        self.assertEqual(
            resolved,
            _function(_decl("@0.a")),
        )

    def test_initializer_is_resolved(self):
        fn = _function(
            _decl("a", init=c99_ast.Constant(value=3)),
            _decl("b", init=c99_ast.Var(name="a")),
        )
        resolved = resolve_function(fn)
        self.assertEqual(
            resolved,
            _function(
                _decl("@0.a", init=c99_ast.Constant(value=3)),
                _decl("@1.b", init=c99_ast.Var(name="@0.a")),
            ),
        )

    def test_duplicate_declaration_raises(self):
        fn = _function(_decl("a"), _decl("a"))
        with self.assertRaises(VariableResolutionError) as ctx:
            resolve_function(fn)
        self.assertIn("'a'", str(ctx.exception))

    def test_duplicate_check_is_against_original_name(self):
        # After `int a;` the map has `a -> @0.a`. A second `int a;`
        # should be rejected even though `@0.a` isn't in the map
        # under that key — we check the *original* name.
        fn = _function(_decl("a"), _decl("a"))
        with self.assertRaises(VariableResolutionError):
            resolve_function(fn)

    def test_self_initialization_resolves_to_new_binding(self):
        # `int a = a;` — the `a` on the RHS is the one being declared
        # (per C's declaration rules). This is UB in C, but resolution
        # just substitutes both sides to the same unique name.
        fn = _function(_decl("a", init=c99_ast.Var(name="a")))
        resolved = resolve_function(fn)
        self.assertEqual(
            resolved,
            _function(_decl("@0.a", init=c99_ast.Var(name="@0.a"))),
        )

    def test_initializer_using_undeclared_name_raises(self):
        fn = _function(_decl("a", init=c99_ast.Var(name="b")))
        with self.assertRaises(VariableResolutionError) as ctx:
            resolve_function(fn)
        self.assertIn("'b'", str(ctx.exception))


class TestExpressionResolution(unittest.TestCase):
    def test_var_reference_is_renamed(self):
        fn = _function(
            _decl("x"),
            _ret(c99_ast.Var(name="x")),
        )
        resolved = resolve_function(fn)
        self.assertEqual(
            resolved,
            _function(
                _decl("@0.x"),
                _ret(c99_ast.Var(name="@0.x")),
            ),
        )

    def test_undeclared_var_reference_raises(self):
        fn = _function(_ret(c99_ast.Var(name="x")))
        with self.assertRaises(VariableResolutionError) as ctx:
            resolve_function(fn)
        self.assertIn("'x'", str(ctx.exception))

    def test_constant_is_preserved(self):
        fn = _function(_ret(c99_ast.Constant(value=42)))
        self.assertEqual(
            resolve_function(fn),
            _function(_ret(c99_ast.Constant(value=42))),
        )

    def test_unary_recurses(self):
        fn = _function(
            _decl("x"),
            _ret(c99_ast.Unary(
                op=c99_ast.Negate(), exp=c99_ast.Var(name="x"),
            )),
        )
        self.assertEqual(
            resolve_function(fn),
            _function(
                _decl("@0.x"),
                _ret(c99_ast.Unary(
                    op=c99_ast.Negate(), exp=c99_ast.Var(name="@0.x"),
                )),
            ),
        )

    def test_binary_recurses_both_sides(self):
        fn = _function(
            _decl("a"),
            _decl("b"),
            _ret(c99_ast.Binary(
                op=c99_ast.Add(),
                left=c99_ast.Var(name="a"),
                right=c99_ast.Var(name="b"),
            )),
        )
        self.assertEqual(
            resolve_function(fn),
            _function(
                _decl("@0.a"),
                _decl("@1.b"),
                _ret(c99_ast.Binary(
                    op=c99_ast.Add(),
                    left=c99_ast.Var(name="@0.a"),
                    right=c99_ast.Var(name="@1.b"),
                )),
            ),
        )

    def test_assignment_resolves_both_sides(self):
        fn = _function(
            _decl("a"),
            _decl("b"),
            _expr(c99_ast.Assignment(
                lval=c99_ast.Var(name="a"),
                rval=c99_ast.Var(name="b"),
            )),
        )
        self.assertEqual(
            resolve_function(fn),
            _function(
                _decl("@0.a"),
                _decl("@1.b"),
                _expr(c99_ast.Assignment(
                    lval=c99_ast.Var(name="@0.a"),
                    rval=c99_ast.Var(name="@1.b"),
                )),
            ),
        )

    def test_chained_assignment_preserves_right_associativity(self):
        # `a = b = c` -> `@0.a = (@1.b = @2.c)` after resolution.
        fn = _function(
            _decl("a"),
            _decl("b"),
            _decl("c"),
            _expr(c99_ast.Assignment(
                lval=c99_ast.Var(name="a"),
                rval=c99_ast.Assignment(
                    lval=c99_ast.Var(name="b"),
                    rval=c99_ast.Var(name="c"),
                ),
            )),
        )
        self.assertEqual(
            resolve_function(fn),
            _function(
                _decl("@0.a"),
                _decl("@1.b"),
                _decl("@2.c"),
                _expr(c99_ast.Assignment(
                    lval=c99_ast.Var(name="@0.a"),
                    rval=c99_ast.Assignment(
                        lval=c99_ast.Var(name="@1.b"),
                        rval=c99_ast.Var(name="@2.c"),
                    ),
                )),
            ),
        )

    def test_binary_on_left_is_rejected(self):
        # The grammar accepts `1+2 = 3+4` so resolution can produce a
        # clear "invalid lvalue" diagnostic.
        fn = _function(_expr(c99_ast.Assignment(
            lval=c99_ast.Binary(
                op=c99_ast.Add(),
                left=c99_ast.Constant(value=1),
                right=c99_ast.Constant(value=2),
            ),
            rval=c99_ast.Constant(value=3),
        )))
        with self.assertRaises(VariableResolutionError) as ctx:
            resolve_function(fn)
        self.assertIn("invalid lvalue", str(ctx.exception))

    def test_constant_on_left_is_rejected(self):
        fn = _function(_expr(c99_ast.Assignment(
            lval=c99_ast.Constant(value=5),
            rval=c99_ast.Constant(value=3),
        )))
        with self.assertRaises(VariableResolutionError):
            resolve_function(fn)

    def test_unary_on_left_is_rejected(self):
        # `-a = 5` — unary expressions aren't lvalues either.
        fn = _function(
            _decl("a"),
            _expr(c99_ast.Assignment(
                lval=c99_ast.Unary(
                    op=c99_ast.Negate(), exp=c99_ast.Var(name="a"),
                ),
                rval=c99_ast.Constant(value=5),
            )),
        )
        with self.assertRaises(VariableResolutionError):
            resolve_function(fn)

    def test_assignment_on_left_is_rejected(self):
        # `(a = b) = c` — the inner assignment is an expression but
        # isn't an lvalue in c6502 (C itself treats it as a non-lvalue).
        # The *outer* assignment's lval is an Assignment node, so the
        # check fires on the outer one.
        fn = _function(
            _decl("a"),
            _decl("b"),
            _decl("c"),
            _expr(c99_ast.Assignment(
                lval=c99_ast.Assignment(
                    lval=c99_ast.Var(name="a"),
                    rval=c99_ast.Var(name="b"),
                ),
                rval=c99_ast.Var(name="c"),
            )),
        )
        with self.assertRaises(VariableResolutionError):
            resolve_function(fn)

    def test_chained_assignment_with_var_lvals_still_works(self):
        # Sanity check: `a = b = c` has a Var on every LHS so the new
        # check must not reject the right-associative chain.
        fn = _function(
            _decl("a"),
            _decl("b"),
            _decl("c"),
            _expr(c99_ast.Assignment(
                lval=c99_ast.Var(name="a"),
                rval=c99_ast.Assignment(
                    lval=c99_ast.Var(name="b"),
                    rval=c99_ast.Var(name="c"),
                ),
            )),
        )
        # Should not raise.
        resolve_function(fn)


class TestStatementPassthrough(unittest.TestCase):
    def test_null_statement_is_preserved(self):
        fn = _function(_null())
        self.assertEqual(resolve_function(fn), _function(_null()))

    def test_mixed_block_items_are_handled_in_order(self):
        fn = _function(
            _decl("a", init=c99_ast.Constant(value=1)),
            _null(),
            _expr(c99_ast.Assignment(
                lval=c99_ast.Var(name="a"),
                rval=c99_ast.Constant(value=2),
            )),
            _ret(c99_ast.Var(name="a")),
        )
        self.assertEqual(
            resolve_function(fn),
            _function(
                _decl("@0.a", init=c99_ast.Constant(value=1)),
                _null(),
                _expr(c99_ast.Assignment(
                    lval=c99_ast.Var(name="@0.a"),
                    rval=c99_ast.Constant(value=2),
                )),
                _ret(c99_ast.Var(name="@0.a")),
            ),
        )


class TestResolveProgram(unittest.TestCase):
    def test_wraps_function_in_program(self):
        fn = _function(_decl("x"), _ret(c99_ast.Var(name="x")))
        prog = c99_ast.Program(function_definition=fn)
        self.assertEqual(
            resolve_program(prog),
            c99_ast.Program(function_definition=_function(
                _decl("@0.x"),
                _ret(c99_ast.Var(name="@0.x")),
            )),
        )


class TestIntegrationWithParser(unittest.TestCase):
    """End-to-end from source text through parse + resolve, so the
    tests don't drift from the AST shapes the parser actually emits."""

    def test_simple_program(self):
        prog = parse("int main(void) { int a = 5; return a; }")
        resolved = resolve_program(prog)
        expected = c99_ast.Program(
            function_definition=_function(
                _decl("@0.a", init=c99_ast.Constant(value=5)),
                _ret(c99_ast.Var(name="@0.a")),
            ),
        )
        self.assertEqual(resolved, expected)

    def test_duplicate_decl_from_source(self):
        prog = parse("int main(void) { int a; int a; return 0; }")
        with self.assertRaises(VariableResolutionError):
            resolve_program(prog)

    def test_undeclared_use_from_source(self):
        prog = parse("int main(void) { return a; }")
        with self.assertRaises(VariableResolutionError):
            resolve_program(prog)

    def test_compound_assignment_rejects_non_var_lhs(self):
        # `1 += 2` desugars to `Assignment(Constant(1), Binary(Add,
        # Constant(1), Constant(2)))` at parse time. The lval-is-Var
        # check in resolution then rejects it just like plain `1 = 2`.
        prog = parse("int main(void) { 1 += 2; return 0; }")
        with self.assertRaises(VariableResolutionError) as ctx:
            resolve_program(prog)
        self.assertIn("invalid lvalue", str(ctx.exception))

    def test_compound_assignment_resolves_var_on_both_sides(self):
        # `a += 1` desugars to `a = a + 1` — both occurrences of `a`
        # must resolve to the same unique name.
        prog = parse("int main(void) { int a; a += 1; return a; }")
        resolved = resolve_program(prog)
        expected = c99_ast.Program(
            function_definition=_function(
                _decl("@0.a"),
                _expr(c99_ast.Assignment(
                    lval=c99_ast.Var(name="@0.a"),
                    rval=c99_ast.Binary(
                        op=c99_ast.Add(),
                        left=c99_ast.Var(name="@0.a"),
                        right=c99_ast.Constant(value=1),
                    ),
                )),
                _ret(c99_ast.Var(name="@0.a")),
            ),
        )
        self.assertEqual(resolved, expected)


class TestErrors(unittest.TestCase):
    def test_unknown_exp_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_exp,), {})
        with self.assertRaises(TypeError):
            Resolver().resolve_exp(stub(), {})

    def test_unknown_statement_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_statement,), {})
        with self.assertRaises(TypeError):
            Resolver().resolve_statement(stub(), {})

    def test_unknown_block_item_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_block_item,), {})
        with self.assertRaises(TypeError):
            Resolver().resolve_block_item(stub(), {})


if __name__ == "__main__":
    unittest.main()
