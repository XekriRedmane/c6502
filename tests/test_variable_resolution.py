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
    return c99_ast.Function(
        name="main",
        body=c99_ast.Block(block_item=list(body_items)),
    )


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
        with self.assertRaises(VariableResolutionError):
            resolve_program(prog)

    def test_if_undeclared_in_then_branch_raises(self):
        prog = parse("int main(void) { if (1) return a; }")
        with self.assertRaises(VariableResolutionError):
            resolve_program(prog)

    def test_if_undeclared_in_else_branch_raises(self):
        prog = parse("int main(void) { if (1) return 0; else return a; }")
        with self.assertRaises(VariableResolutionError):
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
        with self.assertRaises(VariableResolutionError):
            resolve_program(prog)

    def test_undeclared_in_true_clause_raises(self):
        prog = parse("int main(void) { return 1 ? a : 2; }")
        with self.assertRaises(VariableResolutionError):
            resolve_program(prog)

    def test_undeclared_in_false_clause_raises(self):
        prog = parse("int main(void) { return 1 ? 2 : a; }")
        with self.assertRaises(VariableResolutionError):
            resolve_program(prog)

    def test_conditional_as_lvalue_is_rejected(self):
        # `1 ? 2 : a = 5` parses (via the loosened assignment LHS) as
        # `Assignment(Conditional(...), 5)`. The lvalue check rejects
        # a non-Var LHS.
        prog = parse("int main(void) { int a; 1 ? 2 : a = 5; return 0; }")
        with self.assertRaises(VariableResolutionError) as ctx:
            resolve_program(prog)
        self.assertIn("invalid lvalue", str(ctx.exception))


class TestLabeledAndGotoPassthrough(unittest.TestCase):
    """Labels live in their own namespace — variable resolution
    shouldn't touch the label string, but it must descend into a
    LabeledStmt's body so any Var references inside still get
    resolved."""

    def test_goto_passes_through_unchanged(self):
        prog = parse("int main(void) { goto foo; }")
        resolved = resolve_program(prog)
        items = resolved.function_definition.body.block_item
        self.assertEqual(items[0].statement, c99_ast.Goto(label="foo"))

    def test_labeled_statement_label_unchanged_body_resolved(self):
        # `foo: return a;` — the Return inside the labeled stmt has a
        # Var reference that must be resolved to the unique name.
        prog = parse("int main(void) { int a; foo: return a; }")
        resolved = resolve_program(prog)
        items = resolved.function_definition.body.block_item
        self.assertEqual(
            items[1].statement,
            c99_ast.LabeledStmt(
                label="foo",
                statement=c99_ast.Return(exp=c99_ast.Var(name="@0.a")),
            ),
        )

    def test_undeclared_var_inside_labeled_stmt_raises(self):
        prog = parse("int main(void) { foo: return a; }")
        with self.assertRaises(VariableResolutionError):
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
        # Outer decl: @0.a (the outer block's first decl).
        outer_decl = resolved.body.block_item[0].declaration
        self.assertEqual(outer_decl.name, "@0.a")
        # Inner Compound -> Block -> first item -> declaration. The
        # unique name is fresh (@1.a), distinct from @0.a.
        inner_block = resolved.body.block_item[1].statement.block
        inner_decl = inner_block.block_item[0].declaration
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
        self.assertEqual(inner_decl, c99_ast.Declaration(
            name="@1.a", init=c99_ast.Var(name="@1.a"),
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
        with self.assertRaises(VariableResolutionError) as ctx:
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
        with self.assertRaises(VariableResolutionError):
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
        outer = resolved.body.block_item[0].declaration
        first_inner = (
            resolved.body.block_item[1].statement.block.block_item[0].declaration
        )
        second_inner = (
            resolved.body.block_item[2].statement.block.block_item[0].declaration
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
        with self.assertRaises(VariableResolutionError) as ctx:
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
        # Loop labels are minted by loop_labeling — variable_resolution
        # leaves break/continue alone.
        prog = parse(
            "int main(void) { while (1) { break; continue; } return 0; }"
        )
        resolved = resolve_program(prog)
        # while -> body is a Compound -> block -> block_item.
        compound_body = (
            resolved.function_definition.body.block_item[0]
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
        while_stmt = resolved.function_definition.body.block_item[1].statement
        self.assertEqual(while_stmt.condition, c99_ast.Var(name="@0.a"))
        # Body is `a = a + 1` — both Vars resolve to @0.a.
        body = while_stmt.body
        self.assertIsInstance(body, c99_ast.Expression)
        self.assertEqual(body.exp.lval, c99_ast.Var(name="@0.a"))
        self.assertEqual(body.exp.rval.left, c99_ast.Var(name="@0.a"))

    def test_while_undeclared_in_condition_raises(self):
        prog = parse("int main(void) { while (a) ; return 0; }")
        with self.assertRaises(VariableResolutionError):
            resolve_program(prog)

    def test_do_while_body_resolves_outer_var(self):
        prog = parse("int main(void) { int a; do a = 1; while (a); return 0; }")
        resolved = resolve_program(prog)
        do_stmt = resolved.function_definition.body.block_item[1].statement
        self.assertEqual(do_stmt.condition, c99_ast.Var(name="@0.a"))
        self.assertEqual(do_stmt.body.exp.lval, c99_ast.Var(name="@0.a"))

    def test_do_while_undeclared_in_body_raises(self):
        prog = parse("int main(void) { do a = 1; while (1); return 0; }")
        with self.assertRaises(VariableResolutionError):
            resolve_program(prog)

    def test_for_init_decl_binds_in_header_and_body(self):
        # `for (int i = 0; i < 10; i++) i;` — every `i` resolves to the
        # same unique name introduced by the for-init declaration.
        prog = parse(
            "int main(void) { for (int i = 0; i < 10; i++) i; return 0; }"
        )
        resolved = resolve_program(prog)
        for_stmt = resolved.function_definition.body.block_item[0].statement
        decl = for_stmt.init.declaration
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
        items = resolved.function_definition.body.block_item
        outer_decl = items[0].declaration
        self.assertEqual(outer_decl.name, "@0.a")
        for_stmt = items[1].statement
        # for-header's `a` is a fresh unique name.
        self.assertEqual(for_stmt.init.declaration.name, "@1.a")
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
        for_stmt = resolved.function_definition.body.block_item[1].statement
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
        for_stmt = resolved.function_definition.body.block_item[1].statement
        self.assertEqual(for_stmt.init, c99_ast.InitExp(exp=None))
        self.assertEqual(for_stmt.body.exp, c99_ast.Var(name="@0.i"))

    def test_for_undeclared_in_condition_raises(self):
        # `for (;a<10;) ;` with no `a` in scope.
        prog = parse("int main(void) { for (; a < 10;) ; return 0; }")
        with self.assertRaises(VariableResolutionError):
            resolve_program(prog)

    def test_for_undeclared_in_post_raises(self):
        prog = parse("int main(void) { for (;;a++) ; return 0; }")
        with self.assertRaises(VariableResolutionError):
            resolve_program(prog)

    def test_for_init_decl_visible_to_compound_body(self):
        # The compound body sees the for-header's binding, even though
        # it opens its own scope (the inner scope's clone carries the
        # outer entry forward).
        prog = parse(
            "int main(void) { for (int i = 0; ; ) { i; break; } }"
        )
        resolved = resolve_program(prog)
        for_stmt = resolved.function_definition.body.block_item[0].statement
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
        for_stmt = resolved.function_definition.body.block_item[0].statement
        self.assertEqual(for_stmt.init.declaration.name, "@0.i")
        body_decl = for_stmt.body.block.block_item[0].declaration
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

    def test_inner_block_shadows_outer_from_source(self):
        # Source-level test of the per-block scope rule. Inside the
        # inner block, `a` rebinds to a fresh unique name; after the
        # block exits, the outer `a` is intact and `return a` reads
        # the outer binding.
        prog = parse(
            "int main(void) { int a = 1; { int a = 2; } return a; }"
        )
        resolved = resolve_program(prog)
        body = resolved.function_definition.body.block_item
        # Outer decl: @0.a = 1.
        self.assertEqual(body[0].declaration.name, "@0.a")
        # Inner Compound's decl: @1.a = 2 (distinct unique name).
        inner_decl = (
            body[1].statement.block.block_item[0].declaration
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

    def test_postfix_rejects_non_var_operand(self):
        # `1++` parses to `Postfix(Increment, Constant(1))`. Postfix
        # is mutating, so its operand has to name a storage location.
        prog = parse("int main(void) { 1++; return 0; }")
        with self.assertRaises(VariableResolutionError) as ctx:
            resolve_program(prog)
        self.assertIn("invalid lvalue in postfix", str(ctx.exception))

    def test_postfix_resolves_operand_to_unique_name(self):
        # `a++;` desugars to `Postfix(Increment, Var(a))` — the
        # operand should be rewritten to its unique resolved name.
        prog = parse("int main(void) { int a; a++; return a; }")
        resolved = resolve_program(prog)
        expected = c99_ast.Program(
            function_definition=_function(
                _decl("@0.a"),
                _expr(c99_ast.Postfix(
                    op=c99_ast.Increment(),
                    operand=c99_ast.Var(name="@0.a"),
                )),
                _ret(c99_ast.Var(name="@0.a")),
            ),
        )
        self.assertEqual(resolved, expected)

    def test_prefix_rejects_non_var_operand_via_assignment_check(self):
        # `++1` desugars to `Assignment(Constant(1), Binary(Add,
        # Constant(1), Constant(1)))`. The Assignment lvalue check
        # catches it; the error message reflects the assignment
        # branch, not a separate "prefix" branch.
        prog = parse("int main(void) { ++1; return 0; }")
        with self.assertRaises(VariableResolutionError) as ctx:
            resolve_program(prog)
        self.assertIn("invalid lvalue in assignment", str(ctx.exception))

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
