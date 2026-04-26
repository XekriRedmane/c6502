import unittest

import c99_ast
from parser import parse
from passes.identifier_resolution import (
    IdentifierResolutionError,
    Linkage,
    Resolver,
    resolve_function,
    resolve_program,
)


def _function(*body_items) -> c99_ast.Type_function_definition:
    # Returns the legacy `Function(...)` AST shape — no longer
    # produced by the parser, but `resolve_function` still accepts
    # it for unit-testing convenience (see resolve_function's
    # docstring in passes.identifier_resolution).
    return c99_ast.Function(
        name="main",
        body=c99_ast.Block(block_item=list(body_items)),
    )


def _program(*functions) -> c99_ast.Type_program:
    """Wrap one or more legacy-shape `Function` nodes (as returned
    by `_function`) into a new-shape `Program(declaration=[...])`.
    Each Function becomes `FunctionDecl(function_decl=Type_function_decl(
    name, params, body, storage_class=None))`. Tests that compare
    against an entire resolved program use this so they don't have
    to spell out the wrapping by hand for each fixture."""
    decls: list[c99_ast.Type_declaration] = []
    for fn in functions:
        decls.append(c99_ast.FunctionDecl(
            function_decl=c99_ast.Type_function_decl(
                name=fn.name,
                params=list(fn.params),
                body=fn.body,
                storage_class=None,
            ),
        ))
    return c99_ast.Program(declaration=decls)


def _decl(name, init=None) -> c99_ast.Type_block_item:
    # Build a `D(VarDecl(Type_var_decl(...)))` block-item for tests.
    # The triple-wrap is the price of having `declaration` be a sum
    # type with `VarDecl(var_decl)` and `var_decl` itself a product —
    # the helper hides that so tests stay readable.
    return c99_ast.D(declaration=c99_ast.VarDecl(
        var_decl=c99_ast.Type_var_decl(name=name, init=init),
    ))


def _expr(exp) -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.Expression(exp=exp))


def _ret(exp) -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.Return(exp=exp))


def _null() -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.Null())


def _compound(*inner_items) -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.Compound(
        block=c99_ast.Block(block_item=list(inner_items)),
    ))


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
        with self.assertRaises(IdentifierResolutionError) as ctx:
            resolve_function(fn)
        self.assertIn("'a'", str(ctx.exception))

    def test_duplicate_check_is_against_original_name(self):
        # After `int a;` the map has `a -> @0.a`. A second `int a;`
        # should be rejected even though `@0.a` isn't in the map
        # under that key — we check the *original* name.
        fn = _function(_decl("a"), _decl("a"))
        with self.assertRaises(IdentifierResolutionError):
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
        with self.assertRaises(IdentifierResolutionError) as ctx:
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
        with self.assertRaises(IdentifierResolutionError) as ctx:
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
        with self.assertRaises(IdentifierResolutionError) as ctx:
            resolve_function(fn)
        self.assertIn("invalid lvalue", str(ctx.exception))

    def test_constant_on_left_is_rejected(self):
        fn = _function(_expr(c99_ast.Assignment(
            lval=c99_ast.Constant(value=5),
            rval=c99_ast.Constant(value=3),
        )))
        with self.assertRaises(IdentifierResolutionError):
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
        with self.assertRaises(IdentifierResolutionError):
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
        with self.assertRaises(IdentifierResolutionError):
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


class TestIfStatementResolution(unittest.TestCase):
    """If-statements don't open a new scope today (there are no
    nested blocks yet), so the same flat per-function scope is shared
    between the condition, the then-branch, and the optional else-
    branch."""

    def test_if_resolves_condition_and_then(self):
        fn = _function(
            _decl("a"),
            c99_ast.S(statement=c99_ast.IfStmt(
                condition=c99_ast.Var(name="a"),
                then_clause=c99_ast.Return(exp=c99_ast.Var(name="a")),
                else_clause=None,
            )),
        )
        self.assertEqual(
            resolve_function(fn),
            _function(
                _decl("@0.a"),
                c99_ast.S(statement=c99_ast.IfStmt(
                    condition=c99_ast.Var(name="@0.a"),
                    then_clause=c99_ast.Return(
                        exp=c99_ast.Var(name="@0.a"),
                    ),
                    else_clause=None,
                )),
            ),
        )

    def test_if_resolves_else_branch(self):
        fn = _function(
            _decl("a"),
            _decl("b"),
            c99_ast.S(statement=c99_ast.IfStmt(
                condition=c99_ast.Var(name="a"),
                then_clause=c99_ast.Return(exp=c99_ast.Var(name="a")),
                else_clause=c99_ast.Return(exp=c99_ast.Var(name="b")),
            )),
        )
        resolved = resolve_function(fn)
        if_stmt = resolved.body.block_item[2].statement
        self.assertEqual(if_stmt.else_clause,
                         c99_ast.Return(exp=c99_ast.Var(name="@1.b")))

    def test_if_undeclared_in_condition_raises(self):
        prog = parse("int main(void) { if (a) return 0; }")
        with self.assertRaises(IdentifierResolutionError):
            resolve_program(prog)

    def test_if_undeclared_in_then_branch_raises(self):
        prog = parse("int main(void) { if (1) return a; }")
        with self.assertRaises(IdentifierResolutionError):
            resolve_program(prog)

    def test_if_undeclared_in_else_branch_raises(self):
        prog = parse("int main(void) { if (1) return 0; else return a; }")
        with self.assertRaises(IdentifierResolutionError):
            resolve_program(prog)


class TestConditionalResolution(unittest.TestCase):
    """Ternary `cond ? t : f` — all three sub-expressions share the
    same flat scope (same story as if/else) and each gets resolved."""

    def test_all_three_subexpressions_resolve(self):
        fn = _function(
            _decl("a"),
            _decl("b"),
            _decl("c"),
            _ret(c99_ast.Conditional(
                condition=c99_ast.Var(name="a"),
                true_clause=c99_ast.Var(name="b"),
                false_clause=c99_ast.Var(name="c"),
            )),
        )
        resolved = resolve_function(fn)
        self.assertEqual(
            resolved.body.block_item[3],
            _ret(c99_ast.Conditional(
                condition=c99_ast.Var(name="@0.a"),
                true_clause=c99_ast.Var(name="@1.b"),
                false_clause=c99_ast.Var(name="@2.c"),
            )),
        )

    def test_undeclared_in_condition_raises(self):
        prog = parse("int main(void) { return a ? 1 : 2; }")
        with self.assertRaises(IdentifierResolutionError):
            resolve_program(prog)

    def test_undeclared_in_true_clause_raises(self):
        prog = parse("int main(void) { return 1 ? a : 2; }")
        with self.assertRaises(IdentifierResolutionError):
            resolve_program(prog)

    def test_undeclared_in_false_clause_raises(self):
        prog = parse("int main(void) { return 1 ? 2 : a; }")
        with self.assertRaises(IdentifierResolutionError):
            resolve_program(prog)

    def test_conditional_as_lvalue_is_rejected(self):
        # `1 ? 2 : a = 5` parses (via the loosened assignment LHS) as
        # `Assignment(Conditional(...), 5)`. The lvalue check rejects
        # a non-Var LHS.
        prog = parse("int main(void) { int a; 1 ? 2 : a = 5; return 0; }")
        with self.assertRaises(IdentifierResolutionError) as ctx:
            resolve_program(prog)
        self.assertIn("invalid lvalue", str(ctx.exception))


class TestLabeledAndGotoPassthrough(unittest.TestCase):
    """Labels live in their own namespace — identifier resolution
    shouldn't touch the label string, but it must descend into a
    LabeledStmt's body so any Var references inside still get
    resolved."""

    def test_goto_passes_through_unchanged(self):
        prog = parse("int main(void) { goto foo; }")
        resolved = resolve_program(prog)
        items = resolved.declaration[0].function_decl.body.block_item
        self.assertEqual(items[0].statement, c99_ast.Goto(label="foo"))

    def test_labeled_statement_label_unchanged_body_resolved(self):
        # `foo: return a;` — the Return inside the labeled stmt has a
        # Var reference that must be resolved to the unique name.
        prog = parse("int main(void) { int a; foo: return a; }")
        resolved = resolve_program(prog)
        items = resolved.declaration[0].function_decl.body.block_item
        self.assertEqual(
            items[1].statement,
            c99_ast.LabeledStmt(
                label="foo",
                statement=c99_ast.Return(exp=c99_ast.Var(name="@0.a")),
            ),
        )

    def test_undeclared_var_inside_labeled_stmt_raises(self):
        prog = parse("int main(void) { foo: return a; }")
        with self.assertRaises(IdentifierResolutionError):
            resolve_program(prog)


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


class TestNestedScopes(unittest.TestCase):
    """Nested-block scope semantics. Each `Compound` opens a new
    scope: a redeclaration of the same name in the *same* block is
    an error, but a declaration that shadows an outer block's name
    is legal — and the inner declaration mints a fresh unique name
    distinct from the outer one. When the inner block exits, the
    outer block's binding is intact (the inner scope was a clone,
    not an alias)."""

    def test_inner_block_can_shadow_outer_name(self):
        # int a = 1;       // @0.a, inner-scoped to outer block
        # {
        #   int a = 2;     // @1.a, inner-scoped to inner block
        # }
        # The inner `a` shadows the outer one; both get fresh unique
        # names. The outer `a` is untouched after the inner block.
        fn = _function(
            _decl("a", init=c99_ast.Constant(value=1)),
            _compound(
                _decl("a", init=c99_ast.Constant(value=2)),
            ),
        )
        resolved = resolve_function(fn)
        # Outer decl: @0.a (the outer block's first decl). The path
        # is D -> VarDecl -> Type_var_decl, since `declaration` is a
        # sum type with `VarDecl(var_decl)` and the actual name
        # field lives on the inner product.
        outer_decl = resolved.body.block_item[0].declaration.var_decl
        self.assertEqual(outer_decl.name, "@0.a")
        # Inner Compound -> Block -> first item -> declaration. The
        # unique name is fresh (@1.a), distinct from @0.a.
        inner_block = resolved.body.block_item[1].statement.block
        inner_decl = inner_block.block_item[0].declaration.var_decl
        self.assertEqual(inner_decl.name, "@1.a")

    def test_inner_block_sees_outer_name_when_not_shadowed(self):
        # int a = 1;
        # { return a; }    // resolves to outer @0.a
        fn = _function(
            _decl("a", init=c99_ast.Constant(value=1)),
            _compound(
                _ret(c99_ast.Var(name="a")),
            ),
        )
        resolved = resolve_function(fn)
        inner_block = resolved.body.block_item[1].statement.block
        ret_stmt = inner_block.block_item[0].statement
        self.assertEqual(ret_stmt, c99_ast.Return(
            exp=c99_ast.Var(name="@0.a"),
        ))

    def test_inner_decl_self_init_reads_inner_uninitialized_name(self):
        # int a = 5;
        # { int a = a; }   // inner `a` rebinds before resolving
        #                  // its initializer, so the RHS `a` is the
        #                  // *inner* @1.a (uninitialized — UB in C
        #                  // but syntactically what `int a = a;`
        #                  // means; matches the same rule the outer
        #                  // self-init test exercises).
        fn = _function(
            _decl("a", init=c99_ast.Constant(value=5)),
            _compound(
                _decl("a", init=c99_ast.Var(name="a")),
            ),
        )
        resolved = resolve_function(fn)
        inner_block = resolved.body.block_item[1].statement.block
        inner_decl = inner_block.block_item[0].declaration
        # inner_decl is a VarDecl(var_decl=Type_var_decl(...)).
        self.assertEqual(inner_decl, c99_ast.VarDecl(
            var_decl=c99_ast.Type_var_decl(
                name="@1.a", init=c99_ast.Var(name="@1.a"),
            ),
        ))

    def test_outer_name_intact_after_inner_block_exits(self):
        # int a = 1;
        # { int a = 2; }   // shadow
        # return a;        // resolves to outer @0.a, not the inner one
        fn = _function(
            _decl("a", init=c99_ast.Constant(value=1)),
            _compound(
                _decl("a", init=c99_ast.Constant(value=2)),
            ),
            _ret(c99_ast.Var(name="a")),
        )
        resolved = resolve_function(fn)
        ret_stmt = resolved.body.block_item[2].statement
        self.assertEqual(ret_stmt, c99_ast.Return(
            exp=c99_ast.Var(name="@0.a"),
        ))

    def test_redeclaration_in_same_inner_block_raises(self):
        # { int a; int a; }  — both decls in the SAME inner block,
        # so the second collides with the first's inner-scoped entry.
        fn = _function(
            _compound(
                _decl("a"),
                _decl("a"),
            ),
        )
        with self.assertRaises(IdentifierResolutionError) as ctx:
            resolve_function(fn)
        self.assertIn("'a'", str(ctx.exception))

    def test_redeclaration_in_outer_after_inner_decl_still_raises(self):
        # int a;
        # { int a; }   // shadow, fine
        # int a;       // collides with the OUTER `a`, raises
        fn = _function(
            _decl("a"),
            _compound(_decl("a")),
            _decl("a"),
        )
        with self.assertRaises(IdentifierResolutionError):
            resolve_function(fn)

    def test_two_inner_blocks_can_each_redeclare(self):
        # int a;
        # { int a; }   // ok — fresh inner block
        # { int a; }   // also ok — separate inner block, separate scope
        # The two inner `a`s get distinct unique names.
        fn = _function(
            _decl("a"),
            _compound(_decl("a")),
            _compound(_decl("a")),
        )
        resolved = resolve_function(fn)
        outer = resolved.body.block_item[0].declaration.var_decl
        first_inner = (
            resolved.body.block_item[1].statement.block
            .block_item[0].declaration.var_decl
        )
        second_inner = (
            resolved.body.block_item[2].statement.block
            .block_item[0].declaration.var_decl
        )
        # Three distinct unique names.
        self.assertEqual(
            {outer.name, first_inner.name, second_inner.name},
            {"@0.a", "@1.a", "@2.a"},
        )

    def test_doubly_nested_blocks_chain_outer_visibility(self):
        # int a;
        # { { return a; } }   // resolves to the outermost @0.a
        fn = _function(
            _decl("a"),
            _compound(
                _compound(
                    _ret(c99_ast.Var(name="a")),
                ),
            ),
        )
        resolved = resolve_function(fn)
        outer_compound = resolved.body.block_item[1].statement
        inner_compound = outer_compound.block.block_item[0].statement
        ret_stmt = inner_compound.block.block_item[0].statement
        self.assertEqual(ret_stmt, c99_ast.Return(
            exp=c99_ast.Var(name="@0.a"),
        ))

    def test_doubly_nested_redeclaration_uses_innermost(self):
        # int a = 1;
        # {
        #   int a = 2;         // shadow #1: @1.a
        #   { return a; }      // resolves to @1.a (innermost visible)
        # }
        fn = _function(
            _decl("a", init=c99_ast.Constant(value=1)),
            _compound(
                _decl("a", init=c99_ast.Constant(value=2)),
                _compound(
                    _ret(c99_ast.Var(name="a")),
                ),
            ),
        )
        resolved = resolve_function(fn)
        middle_compound = resolved.body.block_item[1].statement
        inner_compound = middle_compound.block.block_item[1].statement
        ret_stmt = inner_compound.block.block_item[0].statement
        self.assertEqual(ret_stmt, c99_ast.Return(
            exp=c99_ast.Var(name="@1.a"),
        ))

    def test_inner_block_undeclared_var_still_raises(self):
        # { return b; }   — `b` was never declared anywhere.
        fn = _function(
            _compound(
                _ret(c99_ast.Var(name="b")),
            ),
        )
        with self.assertRaises(IdentifierResolutionError) as ctx:
            resolve_function(fn)
        self.assertIn("'b'", str(ctx.exception))


class TestLoopResolution(unittest.TestCase):
    """C99 §6.8.5 iteration statements. While/do-while don't introduce a
    variable scope of their own (any header sees the enclosing block's
    scope; the body opens a scope only if it's a Compound). The for-
    statement's header IS its own block-scope (§6.8.5.3): a declaration
    in the for-init shadows any outer name for the entire loop, and the
    condition / post / body all resolve in that header scope."""

    def test_break_continue_pass_through(self):
        # Loop labels are minted by loop_labeling — identifier_resolution
        # leaves break/continue alone.
        prog = parse(
            "int main(void) { while (1) { break; continue; } return 0; }"
        )
        resolved = resolve_program(prog)
        # while -> body is a Compound -> block -> block_item.
        compound_body = (
            resolved.declaration[0].function_decl.body.block_item[0]
            .statement.body
        )
        body_items = compound_body.block.block_item
        self.assertEqual(body_items[0].statement, c99_ast.BreakStmt(label=""))
        self.assertEqual(
            body_items[1].statement, c99_ast.ContinueStmt(label=""),
        )

    def test_while_body_resolves_outer_var(self):
        prog = parse("int main(void) { int a; while (a) a = a + 1; return 0; }")
        resolved = resolve_program(prog)
        while_stmt = resolved.declaration[0].function_decl.body.block_item[1].statement
        self.assertEqual(while_stmt.condition, c99_ast.Var(name="@0.a"))
        # Body is `a = a + 1` — both Vars resolve to @0.a.
        body = while_stmt.body
        self.assertIsInstance(body, c99_ast.Expression)
        self.assertEqual(body.exp.lval, c99_ast.Var(name="@0.a"))
        self.assertEqual(body.exp.rval.left, c99_ast.Var(name="@0.a"))

    def test_while_undeclared_in_condition_raises(self):
        prog = parse("int main(void) { while (a) ; return 0; }")
        with self.assertRaises(IdentifierResolutionError):
            resolve_program(prog)

    def test_do_while_body_resolves_outer_var(self):
        prog = parse("int main(void) { int a; do a = 1; while (a); return 0; }")
        resolved = resolve_program(prog)
        do_stmt = resolved.declaration[0].function_decl.body.block_item[1].statement
        self.assertEqual(do_stmt.condition, c99_ast.Var(name="@0.a"))
        self.assertEqual(do_stmt.body.exp.lval, c99_ast.Var(name="@0.a"))

    def test_do_while_undeclared_in_body_raises(self):
        prog = parse("int main(void) { do a = 1; while (1); return 0; }")
        with self.assertRaises(IdentifierResolutionError):
            resolve_program(prog)

    def test_for_init_decl_binds_in_header_and_body(self):
        # `for (int i = 0; i < 10; i++) i;` — every `i` resolves to the
        # same unique name introduced by the for-init declaration.
        prog = parse(
            "int main(void) { for (int i = 0; i < 10; i++) i; return 0; }"
        )
        resolved = resolve_program(prog)
        for_stmt = resolved.declaration[0].function_decl.body.block_item[0].statement
        decl = for_stmt.init.var_decl
        self.assertEqual(decl.name, "@0.i")
        self.assertEqual(for_stmt.condition.left, c99_ast.Var(name="@0.i"))
        self.assertEqual(for_stmt.post_clause.operand, c99_ast.Var(name="@0.i"))
        self.assertEqual(for_stmt.body.exp, c99_ast.Var(name="@0.i"))

    def test_for_init_decl_shadows_outer(self):
        # `int a; for (int a = 1; a < 10; a++) a = a + 5; return a;`
        # The for-header `a` is a fresh binding (@1.a) that shadows
        # the outer @0.a inside the loop. After the loop, `return a`
        # reads the outer @0.a (the for-header scope is gone).
        prog = parse(
            "int main(void) { int a; "
            "for (int a = 1; a < 10; a = a + 1) a = a + 5; "
            "return a; }"
        )
        resolved = resolve_program(prog)
        items = resolved.declaration[0].function_decl.body.block_item
        outer_decl = items[0].declaration.var_decl
        self.assertEqual(outer_decl.name, "@0.a")
        for_stmt = items[1].statement
        # for-header's `a` is a fresh unique name.
        self.assertEqual(for_stmt.init.var_decl.name, "@1.a")
        # Condition / post / body all see @1.a.
        self.assertEqual(for_stmt.condition.left, c99_ast.Var(name="@1.a"))
        self.assertEqual(for_stmt.post_clause.lval, c99_ast.Var(name="@1.a"))
        self.assertEqual(for_stmt.body.exp.lval, c99_ast.Var(name="@1.a"))
        # After the loop, `return a` resolves to the outer @0.a.
        self.assertEqual(
            items[2].statement,
            c99_ast.Return(exp=c99_ast.Var(name="@0.a")),
        )

    def test_for_init_exp_uses_outer_scope(self):
        # `int i; for (i = 0; i < 5; i++) i;` — the init is an
        # expression, not a declaration, so it just references the
        # already-declared outer `i`.
        prog = parse(
            "int main(void) { int i; "
            "for (i = 0; i < 5; i++) i; return 0; }"
        )
        resolved = resolve_program(prog)
        for_stmt = resolved.declaration[0].function_decl.body.block_item[1].statement
        # Init is the assignment `i = 0`; both Vars resolve to @0.i.
        self.assertEqual(for_stmt.init.exp.lval, c99_ast.Var(name="@0.i"))
        self.assertEqual(for_stmt.body.exp, c99_ast.Var(name="@0.i"))

    def test_for_empty_header_body_resolves(self):
        # `for (;;) i;` — the empty header still opens a scope, but no
        # binding is added, so the body sees outer names as usual.
        prog = parse(
            "int main(void) { int i; for (;;) i; }"
        )
        resolved = resolve_program(prog)
        for_stmt = resolved.declaration[0].function_decl.body.block_item[1].statement
        self.assertEqual(for_stmt.init, c99_ast.InitExp(exp=None))
        self.assertEqual(for_stmt.body.exp, c99_ast.Var(name="@0.i"))

    def test_for_undeclared_in_condition_raises(self):
        # `for (;a<10;) ;` with no `a` in scope.
        prog = parse("int main(void) { for (; a < 10;) ; return 0; }")
        with self.assertRaises(IdentifierResolutionError):
            resolve_program(prog)

    def test_for_undeclared_in_post_raises(self):
        prog = parse("int main(void) { for (;;a++) ; return 0; }")
        with self.assertRaises(IdentifierResolutionError):
            resolve_program(prog)

    def test_for_init_decl_visible_to_compound_body(self):
        # The compound body sees the for-header's binding, even though
        # it opens its own scope (the inner scope's clone carries the
        # outer entry forward).
        prog = parse(
            "int main(void) { for (int i = 0; ; ) { i; break; } }"
        )
        resolved = resolve_program(prog)
        for_stmt = resolved.declaration[0].function_decl.body.block_item[0].statement
        body_block = for_stmt.body.block
        first_item = body_block.block_item[0].statement
        self.assertEqual(first_item.exp, c99_ast.Var(name="@0.i"))

    def test_for_compound_body_can_shadow_for_init(self):
        # `for (int i = 0;;) { int i = 5; ... }` — the compound body
        # opens its own scope and can shadow the for-init's `i`.
        prog = parse(
            "int main(void) { for (int i = 0;;) { int i = 5; break; } }"
        )
        resolved = resolve_program(prog)
        for_stmt = resolved.declaration[0].function_decl.body.block_item[0].statement
        self.assertEqual(for_stmt.init.var_decl.name, "@0.i")
        body_decl = for_stmt.body.block.block_item[0].declaration.var_decl
        self.assertEqual(body_decl.name, "@1.i")

    def test_for_init_duplicate_with_for_scope_decl_in_body_is_legal(self):
        # `for (int i = 0; ; ) { int i = 5; }` is legal because the
        # body's compound is a new (inner) scope. But `for (int i = 0;
        # ; ) int i = 5;` would be a parse error (declarations aren't
        # legal as a non-compound for-body in C99) — we don't have to
        # cover that here, the grammar handles it.
        prog = parse(
            "int main(void) { for (int i = 0;;) { int i = 1; break; } }"
        )
        # Should not raise.
        resolve_program(prog)


class TestResolveProgram(unittest.TestCase):
    def test_wraps_function_in_program(self):
        # Program.declaration is a list now; the helper builds
        # a single Function and we wrap it in a one-element list.
        fn = _function(_decl("x"), _ret(c99_ast.Var(name="x")))
        prog = _program(fn)
        self.assertEqual(
            resolve_program(prog),
            _program(_function(
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
        expected = _program(_function(
            _decl("@0.a", init=c99_ast.Constant(value=5)),
            _ret(c99_ast.Var(name="@0.a")),
        ))
        self.assertEqual(resolved, expected)

    def test_duplicate_decl_from_source(self):
        prog = parse("int main(void) { int a; int a; return 0; }")
        with self.assertRaises(IdentifierResolutionError):
            resolve_program(prog)

    def test_undeclared_use_from_source(self):
        prog = parse("int main(void) { return a; }")
        with self.assertRaises(IdentifierResolutionError):
            resolve_program(prog)

    def test_inner_block_shadows_outer_from_source(self):
        # Source-level test of the per-block scope rule. Inside the
        # inner block, `a` rebinds to a fresh unique name; after the
        # block exits, the outer `a` is intact and `return a` reads
        # the outer binding.
        prog = parse(
            "int main(void) { int a = 1; { int a = 2; } return a; }"
        )
        resolved = resolve_program(prog)
        body = resolved.declaration[0].function_decl.body.block_item
        # Outer decl: @0.a = 1.
        self.assertEqual(body[0].declaration.var_decl.name, "@0.a")
        # Inner Compound's decl: @1.a = 2 (distinct unique name).
        inner_decl = (
            body[1].statement.block.block_item[0].declaration.var_decl
        )
        self.assertEqual(inner_decl.name, "@1.a")
        # Return reads the outer @0.a, not the (now-discarded) @1.a.
        self.assertEqual(
            body[2].statement,
            c99_ast.Return(exp=c99_ast.Var(name="@0.a")),
        )

    def test_same_block_redeclaration_from_source(self):
        # `{ int a; int a; }` — both decls in the same inner block
        # collide.
        prog = parse(
            "int main(void) { { int a; int a; } return 0; }"
        )
        with self.assertRaises(IdentifierResolutionError):
            resolve_program(prog)

    def test_compound_assignment_rejects_non_var_lhs(self):
        # `1 += 2` desugars to `Assignment(Constant(1), Binary(Add,
        # Constant(1), Constant(2)))` at parse time. The lval-is-Var
        # check in resolution then rejects it just like plain `1 = 2`.
        prog = parse("int main(void) { 1 += 2; return 0; }")
        with self.assertRaises(IdentifierResolutionError) as ctx:
            resolve_program(prog)
        self.assertIn("invalid lvalue", str(ctx.exception))

    def test_postfix_rejects_non_var_operand(self):
        # `1++` parses to `Postfix(Increment, Constant(1))`. Postfix
        # is mutating, so its operand has to name a storage location.
        prog = parse("int main(void) { 1++; return 0; }")
        with self.assertRaises(IdentifierResolutionError) as ctx:
            resolve_program(prog)
        self.assertIn("invalid lvalue in postfix", str(ctx.exception))

    def test_postfix_resolves_operand_to_unique_name(self):
        # `a++;` desugars to `Postfix(Increment, Var(a))` — the
        # operand should be rewritten to its unique resolved name.
        prog = parse("int main(void) { int a; a++; return a; }")
        resolved = resolve_program(prog)
        expected = _program(_function(
            _decl("@0.a"),
            _expr(c99_ast.Postfix(
                op=c99_ast.Increment(),
                operand=c99_ast.Var(name="@0.a"),
            )),
            _ret(c99_ast.Var(name="@0.a")),
        ))
        self.assertEqual(resolved, expected)

    def test_prefix_rejects_non_var_operand_via_assignment_check(self):
        # `++1` desugars to `Assignment(Constant(1), Binary(Add,
        # Constant(1), Constant(1)))`. The Assignment lvalue check
        # catches it; the error message reflects the assignment
        # branch, not a separate "prefix" branch.
        prog = parse("int main(void) { ++1; return 0; }")
        with self.assertRaises(IdentifierResolutionError) as ctx:
            resolve_program(prog)
        self.assertIn("invalid lvalue in assignment", str(ctx.exception))

    def test_compound_assignment_resolves_var_on_both_sides(self):
        # `a += 1` desugars to `a = a + 1` — both occurrences of `a`
        # must resolve to the same unique name.
        prog = parse("int main(void) { int a; a += 1; return a; }")
        resolved = resolve_program(prog)
        expected = _program(_function(
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
        ))
        self.assertEqual(resolved, expected)


class TestFunctionLinkage(unittest.TestCase):
    """Function names have external linkage (C99 §6.2.2): they pass
    through resolution untouched, multiple declarations of the same
    name all refer to the same external symbol, and a `FunctionCall`
    must reference some declared name."""

    def test_function_decl_name_is_not_renamed(self):
        # `int foo(void); foo();` — the FunctionDecl registers `foo`,
        # the FunctionCall references it, neither name gets `@N.`-
        # prefixed.
        prog = parse(
            "int main(void) { int foo(void); foo(); return 0; }"
        )
        resolved = resolve_program(prog)
        items = resolved.declaration[0].function_decl.body.block_item
        decl = items[0].declaration
        self.assertIsInstance(decl, c99_ast.FunctionDecl)
        self.assertEqual(decl.function_decl.name, "foo")
        call = items[1].statement.exp
        self.assertEqual(call, c99_ast.FunctionCall(name="foo", args=[]))

    def test_duplicate_function_decl_is_legal(self):
        # Multiple declarations of the same function refer to the
        # same external symbol and are explicitly permitted (C99
        # §6.2.2 / §6.7). The pass shouldn't raise.
        prog = parse(
            "int main(void) { int foo(void); int foo(void); "
            "return foo(); }"
        )
        # Should not raise.
        resolve_program(prog)

    def test_call_to_undeclared_function_raises(self):
        # No declaration of `foo` anywhere — the call has no target
        # to bind to.
        prog = parse("int main(void) { return foo(); }")
        with self.assertRaises(IdentifierResolutionError) as ctx:
            resolve_program(prog)
        self.assertIn("'foo'", str(ctx.exception))

    def test_call_args_are_resolved(self):
        # The args list is recursed into like any expression — Var
        # references inside must resolve to their unique names.
        # `int f(int x);` renames its param to `@0.x` (param scope is
        # discarded after the decl, but the unique-counter has been
        # bumped), so the next NONE-linkage decl `int a;` lands at
        # `@1.a`.
        prog = parse(
            "int main(void) { int f(int x); int a; return f(a); }"
        )
        resolved = resolve_program(prog)
        items = resolved.declaration[0].function_decl.body.block_item
        ret = items[2].statement
        self.assertEqual(
            ret.exp,
            c99_ast.FunctionCall(
                name="f", args=[c99_ast.Var(name="@1.a")],
            ),
        )

    def test_top_level_function_is_visible_for_self_call(self):
        # `int main(void) { return main(); }` — the program's own
        # name is registered before its body is walked, so the
        # recursive call resolves.
        prog = parse("int main(void) { return main(); }")
        resolved = resolve_program(prog)
        ret = resolved.declaration[0].function_decl.body.block_item[0].statement
        self.assertEqual(
            ret.exp, c99_ast.FunctionCall(name="main", args=[]),
        )

    def test_call_to_top_level_function_from_another(self):
        prog = parse(
            "int foo(void) { return 1; } "
            "int main(void) { return foo(); }"
        )
        resolved = resolve_program(prog)
        ret = (
            resolved.declaration[1].function_decl.body.block_item[0].statement
        )
        self.assertEqual(
            ret.exp, c99_ast.FunctionCall(name="foo", args=[]),
        )

    def test_var_resolution_of_arg_inside_call(self):
        # `int f(int x); int a = 5; f(a + 1);` — the `a` arg expr
        # resolves to the local `@N.a`; the call name stays `f`.
        # `f`'s param `x` consumes `@0`, so the var `a` is `@1.a`.
        prog = parse(
            "int main(void) { int f(int x); int a = 5; f(a + 1); "
            "return 0; }"
        )
        resolved = resolve_program(prog)
        call = (
            resolved.declaration[0].function_decl.body.block_item[2]
            .statement.exp
        )
        self.assertEqual(call.name, "f")
        self.assertEqual(
            call.args,
            [c99_ast.Binary(
                op=c99_ast.Add(),
                left=c99_ast.Var(name="@1.a"),
                right=c99_ast.Constant(value=1),
            )],
        )


class TestParameterResolution(unittest.TestCase):
    """Function parameters get the same treatment as block-scope
    variable declarations: validated for uniqueness within the
    parameter list and renamed to fresh `@<N>.<orig>` strings. The
    *param scope* this builds is independent of the surrounding block
    scope (so `int a; int foo(int a);` is legal — the param `a`
    doesn't conflict with the outer variable `a`). For function
    *definitions*, the param scope IS the body's outermost scope per
    C99 §6.9.1.7, so a body decl that reuses a param name raises."""

    def test_function_decl_renames_params(self):
        prog = parse(
            "int main(void) { int foo(int x, int y); return 0; }"
        )
        resolved = resolve_program(prog)
        decl = (
            resolved.declaration[0].function_decl.body.block_item[0]
            .declaration.function_decl
        )
        self.assertEqual(decl.name, "foo")
        self.assertEqual(decl.params, ["@0.x", "@1.y"])

    def test_function_decl_duplicate_param_raises(self):
        # `int foo(int a, int a);` — two params named `a` is a
        # duplicate within the param list.
        prog = parse("int main(void) { int foo(int a, int a); return 0; }")
        with self.assertRaises(IdentifierResolutionError) as ctx:
            resolve_program(prog)
        self.assertIn("'a'", str(ctx.exception))

    def test_function_decl_param_does_not_conflict_with_outer_var(self):
        # `int a; int foo(int a);` — the param `a` lives in its own
        # param scope, separate from the block scope holding the
        # outer `int a;`. Both should resolve cleanly to fresh names.
        prog = parse(
            "int main(void) { int a; int foo(int a); return a; }"
        )
        resolved = resolve_program(prog)
        items = resolved.declaration[0].function_decl.body.block_item
        # Outer variable: NONE-linkage → renamed.
        outer_a = items[0].declaration.var_decl
        self.assertEqual(outer_a.name, "@0.a")
        # Param of foo: also NONE-linkage → renamed, but to a
        # different unique name.
        foo_decl = items[1].declaration.function_decl
        self.assertEqual(foo_decl.params, ["@1.a"])
        # `return a;` references the OUTER `a` (the param scope died
        # at the end of the foo declaration).
        ret = items[2].statement
        self.assertEqual(ret.exp, c99_ast.Var(name="@0.a"))

    def test_function_definition_renames_params(self):
        prog = parse("int main(int x, int y) { return x + y; }")
        resolved = resolve_program(prog)
        fn = resolved.declaration[0].function_decl
        self.assertEqual(fn.params, ["@0.x", "@1.y"])
        # Body sees the renamed params.
        ret = fn.body.block_item[0].statement
        self.assertEqual(
            ret.exp,
            c99_ast.Binary(
                op=c99_ast.Add(),
                left=c99_ast.Var(name="@0.x"),
                right=c99_ast.Var(name="@1.y"),
            ),
        )

    def test_function_definition_duplicate_param_raises(self):
        prog = parse("int main(int a, int a) { return a; }")
        with self.assertRaises(IdentifierResolutionError) as ctx:
            resolve_program(prog)
        self.assertIn("'a'", str(ctx.exception))

    def test_function_body_decl_collides_with_param(self):
        # C99 §6.9.1.7: parameters and the function's outermost
        # locals share one scope. `int foo(int a) { int a = 3; ...}`
        # is a duplicate-decl error, not a legal shadow.
        prog = parse("int foo(int a) { int a = 3; return a; }")
        with self.assertRaises(IdentifierResolutionError) as ctx:
            resolve_program(prog)
        self.assertIn("'a'", str(ctx.exception))

    def test_function_body_inner_block_can_shadow_param(self):
        # The collision is only with the body's *outermost* scope.
        # An inner compound opens a fresh scope and may legally
        # shadow the param.
        prog = parse(
            "int foo(int a) { { int a = 3; return a; } return a; }"
        )
        resolved = resolve_program(prog)
        fn = resolved.declaration[0].function_decl
        # Param: @0.a.
        self.assertEqual(fn.params, ["@0.a"])
        # Inner compound's `int a = 3;` rebinds to a fresh @1.a.
        inner_block = fn.body.block_item[0].statement.block
        inner_decl = inner_block.block_item[0].declaration.var_decl
        self.assertEqual(inner_decl.name, "@1.a")
        # The trailing `return a;` (in the body's outer scope) reads
        # the param.
        outer_ret = fn.body.block_item[1].statement
        self.assertEqual(outer_ret.exp, c99_ast.Var(name="@0.a"))

    def test_function_body_can_reference_param(self):
        prog = parse("int foo(int x) { return x + 1; }")
        resolved = resolve_program(prog)
        fn = resolved.declaration[0].function_decl
        ret = fn.body.block_item[0].statement
        self.assertEqual(
            ret.exp,
            c99_ast.Binary(
                op=c99_ast.Add(),
                left=c99_ast.Var(name="@0.x"),
                right=c99_ast.Constant(value=1),
            ),
        )

    def test_function_decl_param_scope_dies_after_decl(self):
        # `int foo(int x); int x;` — the FunctionDecl's param `x`
        # lives in its own scope, dying at the end of the decl. So
        # `int x;` afterward is the *first* declaration of `x` in
        # the surrounding block — no duplicate-decl error.
        prog = parse(
            "int main(void) { int foo(int x); int x; return x; }"
        )
        # Should not raise.
        resolved = resolve_program(prog)
        items = resolved.declaration[0].function_decl.body.block_item
        # The outer `x` gets `@1.x` (`@0.x` was minted for the param,
        # then the param scope was discarded).
        self.assertEqual(items[1].declaration.var_decl.name, "@1.x")
        ret = items[2].statement
        self.assertEqual(ret.exp, c99_ast.Var(name="@1.x"))


class TestLinkageTracking(unittest.TestCase):
    """The resolver tags every visible identifier with its C99 §6.2.2
    linkage kind. Today: function declarations and definitions are
    EXTERNAL; block-scope variable declarations are NONE. The pass
    uses linkage to decide whether to rename (NONE → fresh `@<N>.<orig>`,
    INTERNAL/EXTERNAL → keep source spelling), so reading the resolver's
    internal tables is the cleanest way to assert linkage is being
    recorded — until the type-checking pass lands and consumes it
    via a public surface."""

    def test_function_decl_recorded_as_external(self):
        # `int main(...) { ... }` is a file-scope function definition;
        # the block-scope `int foo(void);` declaration doesn't appear
        # in the file-scope table at all (it lives in the body's local
        # scope dict, which goes out of scope when the body is done).
        # So we expect only `main` here, with EXTERNAL linkage.
        prog = parse("int main(void) { int foo(void); return foo(); }")
        r = Resolver()
        r.resolve_program(prog)
        self.assertEqual(r._file_scope["main"], ("main", Linkage.EXTERNAL))
        self.assertNotIn("foo", r._file_scope)

    def test_top_level_function_definition_recorded_as_external(self):
        # `int foo(void) { ... } int main(void) { ... }` — both
        # registered (with EXTERNAL linkage) before any body is
        # walked, so each can call the other regardless of source
        # order.
        prog = parse(
            "int foo(void) { return 1; } "
            "int main(void) { return 0; }"
        )
        r = Resolver()
        r.resolve_program(prog)
        self.assertEqual(r._file_scope, {
            "foo": ("foo", Linkage.EXTERNAL),
            "main": ("main", Linkage.EXTERNAL),
        })

    def test_static_at_file_scope_is_internal(self):
        # `static int foo(void);` at file scope → INTERNAL linkage.
        prog = parse("static int foo(void) { return 0; }")
        r = Resolver()
        r.resolve_program(prog)
        self.assertEqual(r._file_scope["foo"], ("foo", Linkage.INTERNAL))

    def test_extern_inherits_prior_linkage(self):
        # File-scope `static int foo(void);` followed by `extern int
        # foo(void) { ... }` — the extern decl takes the prior visible
        # decl's linkage, so the definition is also INTERNAL.
        prog = parse(
            "static int foo(void); "
            "extern int foo(void) { return 0; }"
        )
        r = Resolver()
        r.resolve_program(prog)
        self.assertEqual(r._file_scope["foo"], ("foo", Linkage.INTERNAL))

    def test_file_scope_variable_recorded(self):
        # `int g;` at file scope → EXTERNAL linkage; `static int s;`
        # → INTERNAL.
        prog = parse("int g; static int s; int main(void) { return 0; }")
        r = Resolver()
        r.resolve_program(prog)
        self.assertEqual(r._file_scope["g"], ("g", Linkage.EXTERNAL))
        self.assertEqual(r._file_scope["s"], ("s", Linkage.INTERNAL))

    def test_changing_linkage_at_file_scope_is_an_error(self):
        # `static int x; int x;` is UB per C99 §6.2.2.7. We give a
        # clean diagnostic rather than silently accepting it.
        prog = parse("static int x; int x; int main(void) { return 0; }")
        with self.assertRaises(IdentifierResolutionError) as ctx:
            resolve_program(prog)
        self.assertIn("'x'", str(ctx.exception))

    def test_block_scope_extern_inherits_file_scope_linkage(self):
        # File scope `static int x = 5;` has INTERNAL linkage. Inside
        # `main`, `extern int x;` follows the prior-visible rule and
        # also picks up INTERNAL — and importantly *doesn't get
        # renamed*, so references to `x` inside main bind by source
        # spelling.
        prog = parse(
            "static int x = 5; "
            "int main(void) { extern int x; return x; }"
        )
        resolved = resolve_program(prog)
        items = resolved.declaration[1].function_decl.body.block_item
        block_extern = items[0].declaration.var_decl
        self.assertEqual(block_extern.name, "x")
        ret = items[1].statement
        self.assertEqual(ret.exp, c99_ast.Var(name="x"))

    def test_static_at_block_scope_is_none_linkage(self):
        # `static int x = 5;` at block scope changes storage duration
        # but not linkage — §6.2.2 still tags this as NONE-linkage,
        # so it gets renamed like any block-scope local.
        prog = parse(
            "int main(void) { static int x = 5; return x; }"
        )
        resolved = resolve_program(prog)
        items = resolved.declaration[0].function_decl.body.block_item
        decl = items[0].declaration.var_decl
        self.assertEqual(decl.name, "@0.x")
        ret = items[1].statement
        self.assertEqual(ret.exp, c99_ast.Var(name="@0.x"))

    def test_static_on_block_scope_function_decl_raises(self):
        # `static int foo(void);` is forbidden at block scope per
        # C99 §6.2.2: "A function declaration can contain the
        # storage-class specifier static only if it is at file scope."
        prog = parse(
            "int main(void) { static int foo(void); return 0; }"
        )
        with self.assertRaises(IdentifierResolutionError) as ctx:
            resolve_program(prog)
        self.assertIn("'foo'", str(ctx.exception))

    def test_block_scope_variable_recorded_as_none_linkage(self):
        # `int a;` at block scope — NONE linkage, gets renamed.
        # The scope dict goes out of scope when resolve_block returns,
        # so we exercise resolve_var_decl directly to inspect the
        # tagged entry.
        r = Resolver()
        scope: dict = {}
        r.resolve_var_decl(
            c99_ast.Type_var_decl(name="a", init=None),
            scope,
            Linkage.NONE,
        )
        self.assertEqual(scope["a"], ("@0.a", True, Linkage.NONE))

    def test_external_linkage_var_keeps_source_spelling(self):
        # Forcing a synthetic EXTERNAL var-decl through the resolver
        # exercises the future-extern path: the unique-counter is
        # NOT bumped, and the resolved name equals the source name.
        r = Resolver()
        scope: dict = {}
        result = r.resolve_var_decl(
            c99_ast.Type_var_decl(name="g", init=None),
            scope,
            Linkage.EXTERNAL,
        )
        self.assertEqual(result.name, "g")
        self.assertEqual(scope["g"], ("g", True, Linkage.EXTERNAL))
        # Counter untouched — the next NONE-linkage var still gets
        # `@0.<name>`, not `@1.<name>`.
        self.assertEqual(r.make_unique("x"), "@0.x")


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
