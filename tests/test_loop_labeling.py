import unittest

import c99_ast
from parser import parse
from passes.loop_labeling import (
    LoopLabeler,
    LoopLabelingError,
    label_function,
    label_program,
)


def _function(*body_items, name="main") -> c99_ast.Type_function_definition:
    return c99_ast.Function(
        name=name,
        body=c99_ast.Block(block_item=list(body_items)),
    )


def _ret(exp) -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.Return(exp=exp))


def _expr(exp) -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.Expression(exp=exp))


def _null() -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.Null())


def _break() -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.BreakStmt(label=""))


def _continue() -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.ContinueStmt(label=""))


def _while(cond, body) -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.WhileStmt(
        condition=cond, body=body, label="",
    ))


def _do_while(body, cond) -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.DoWhileStmt(
        body=body, condition=cond, label="",
    ))


def _for(init, cond, post, body) -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.ForStmt(
        init=init, condition=cond, post_clause=post, body=body, label="",
    ))


def _compound(*items) -> c99_ast.Type_statement:
    return c99_ast.Compound(block=c99_ast.Block(block_item=list(items)))


class TestMakeLabel(unittest.TestCase):
    def test_format_is_dot_loop_underscore_counter(self):
        lbl = LoopLabeler()
        self.assertEqual(lbl.make_label(), ".loop@0")
        self.assertEqual(lbl.make_label(), ".loop@1")

    def test_counter_is_program_unique(self):
        # One LoopLabeler per program; the counter doesn't reset
        # between functions visited by the same instance.
        lbl = LoopLabeler()
        self.assertEqual(lbl.make_label(), ".loop@0")
        self.assertEqual(lbl.make_label(), ".loop@1")
        self.assertEqual(lbl.make_label(), ".loop@2")


class TestSimpleLoops(unittest.TestCase):
    def test_while_loop_gets_label_and_break_inside_picks_it_up(self):
        # while (1) break;
        fn = _function(_while(c99_ast.Constant(value=1), c99_ast.BreakStmt(label="")))
        labeled = label_function(fn)
        while_stmt = labeled.body.block_item[0].statement
        self.assertEqual(while_stmt.label, ".loop@0")
        self.assertEqual(while_stmt.body, c99_ast.BreakStmt(label=".loop@0"))

    def test_while_loop_continue_inside_body_picks_label_up(self):
        fn = _function(_while(
            c99_ast.Constant(value=1), c99_ast.ContinueStmt(label=""),
        ))
        labeled = label_function(fn)
        while_stmt = labeled.body.block_item[0].statement
        self.assertEqual(while_stmt.body, c99_ast.ContinueStmt(label=".loop@0"))

    def test_do_while_loop_gets_label(self):
        fn = _function(_do_while(
            c99_ast.BreakStmt(label=""), c99_ast.Constant(value=1),
        ))
        labeled = label_function(fn)
        do_stmt = labeled.body.block_item[0].statement
        self.assertEqual(do_stmt.label, ".loop@0")
        self.assertEqual(do_stmt.body, c99_ast.BreakStmt(label=".loop@0"))

    def test_for_loop_gets_label(self):
        # for (;;) break;
        fn = _function(_for(
            c99_ast.InitExp(exp=None), None, None,
            c99_ast.BreakStmt(label=""),
        ))
        labeled = label_function(fn)
        for_stmt = labeled.body.block_item[0].statement
        self.assertEqual(for_stmt.label, ".loop@0")
        self.assertEqual(for_stmt.body, c99_ast.BreakStmt(label=".loop@0"))

    def test_consecutive_loops_get_distinct_labels(self):
        # Two independent while loops each mint their own label.
        fn = _function(
            _while(c99_ast.Constant(value=1), c99_ast.BreakStmt(label="")),
            _while(c99_ast.Constant(value=1), c99_ast.BreakStmt(label="")),
        )
        labeled = label_function(fn)
        first = labeled.body.block_item[0].statement
        second = labeled.body.block_item[1].statement
        self.assertEqual(first.label, ".loop@0")
        self.assertEqual(second.label, ".loop@1")
        self.assertEqual(first.body, c99_ast.BreakStmt(label=".loop@0"))
        self.assertEqual(second.body, c99_ast.BreakStmt(label=".loop@1"))


class TestNestedLoops(unittest.TestCase):
    """Inside a nested loop, break/continue target the *innermost*
    enclosing loop. When the inner loop's body ends, control returns
    to the outer loop's label as the current target."""

    def test_break_in_inner_loop_targets_inner(self):
        # while (1) { while (1) break; }   — the break inside the inner
        # while targets the inner loop, not the outer one.
        fn = _function(_while(
            c99_ast.Constant(value=1),
            _compound(_while(
                c99_ast.Constant(value=1), c99_ast.BreakStmt(label=""),
            )),
        ))
        labeled = label_function(fn)
        outer = labeled.body.block_item[0].statement
        inner_compound = outer.body
        inner_while = inner_compound.block.block_item[0].statement
        self.assertEqual(outer.label, ".loop@0")
        self.assertEqual(inner_while.label, ".loop@1")
        # The break inside the inner while targets `.loop@1`.
        self.assertEqual(
            inner_while.body, c99_ast.BreakStmt(label=".loop@1"),
        )

    def test_break_after_inner_loop_targets_outer(self):
        # while (1) { while (1) ; break; }  — the second break sits in
        # the OUTER loop's body, after the inner loop, so it targets
        # the outer loop.
        fn = _function(_while(
            c99_ast.Constant(value=1),
            _compound(
                _while(c99_ast.Constant(value=1), c99_ast.Null()),
                _break(),
            ),
        ))
        labeled = label_function(fn)
        outer = labeled.body.block_item[0].statement
        outer_body_items = outer.body.block.block_item
        # Second item in outer body is the post-inner break.
        self.assertEqual(
            outer_body_items[1].statement,
            c99_ast.BreakStmt(label=".loop@0"),
        )

    def test_three_deep_nesting(self):
        # for { while { do { break; } while (1); break; } break; }
        # Each break targets the loop it lives in.
        fn = _function(_for(
            c99_ast.InitExp(exp=None), None, None,
            _compound(
                _while(
                    c99_ast.Constant(value=1),
                    _compound(
                        _do_while(
                            _compound(_break()),
                            c99_ast.Constant(value=1),
                        ),
                        _break(),
                    ),
                ),
                _break(),
            ),
        ))
        labeled = label_function(fn)
        for_stmt = labeled.body.block_item[0].statement
        for_body = for_stmt.body.block.block_item
        while_stmt = for_body[0].statement
        while_body = while_stmt.body.block.block_item
        do_stmt = while_body[0].statement
        # Labels: for=.loop@0, while=.loop@1, do=.loop@2.
        self.assertEqual(for_stmt.label, ".loop@0")
        self.assertEqual(while_stmt.label, ".loop@1")
        self.assertEqual(do_stmt.label, ".loop@2")
        # break inside do-while body -> .loop@2.
        do_break = do_stmt.body.block.block_item[0].statement
        self.assertEqual(do_break, c99_ast.BreakStmt(label=".loop@2"))
        # break in the while body, after the do-while -> .loop@1.
        self.assertEqual(
            while_body[1].statement, c99_ast.BreakStmt(label=".loop@1"),
        )
        # break in the for body, after the while -> .loop@0.
        self.assertEqual(
            for_body[1].statement, c99_ast.BreakStmt(label=".loop@0"),
        )

    def test_continue_in_nested_loop_targets_innermost(self):
        # while (1) for (;;) continue;   — the continue is inside the
        # for-loop body (not its header), so it targets the for-loop.
        fn = _function(_while(
            c99_ast.Constant(value=1),
            c99_ast.ForStmt(
                init=c99_ast.InitExp(exp=None),
                condition=None,
                post_clause=None,
                body=c99_ast.ContinueStmt(label=""),
                label="",
            ),
        ))
        labeled = label_function(fn)
        outer = labeled.body.block_item[0].statement
        for_stmt = outer.body
        self.assertEqual(outer.label, ".loop@0")
        self.assertEqual(for_stmt.label, ".loop@1")
        # continue inside for body -> .loop@1.
        self.assertEqual(
            for_stmt.body, c99_ast.ContinueStmt(label=".loop@1"),
        )


class TestBreakContinueOutsideLoop(unittest.TestCase):
    def test_break_at_top_level_raises(self):
        fn = _function(_break())
        with self.assertRaises(LoopLabelingError) as ctx:
            label_function(fn)
        self.assertIn("break", str(ctx.exception))

    def test_continue_at_top_level_raises(self):
        fn = _function(_continue())
        with self.assertRaises(LoopLabelingError) as ctx:
            label_function(fn)
        self.assertIn("continue", str(ctx.exception))

    def test_break_inside_if_outside_loop_raises(self):
        # `if (1) break;` at function scope — the if doesn't establish
        # a loop, so the break has no enclosing loop.
        fn = _function(c99_ast.S(statement=c99_ast.IfStmt(
            condition=c99_ast.Constant(value=1),
            then_clause=c99_ast.BreakStmt(label=""),
            else_clause=None,
        )))
        with self.assertRaises(LoopLabelingError):
            label_function(fn)

    def test_break_inside_compound_outside_loop_raises(self):
        # `{ break; }` at function scope — Compound doesn't establish
        # a loop either.
        fn = _function(c99_ast.S(statement=_compound(_break())))
        with self.assertRaises(LoopLabelingError):
            label_function(fn)

    def test_break_inside_labeled_stmt_outside_loop_raises(self):
        # `foo: break;` outside any loop — the LabeledStmt itself
        # doesn't introduce loop scope.
        fn = _function(c99_ast.S(statement=c99_ast.LabeledStmt(
            label="foo", statement=c99_ast.BreakStmt(label=""),
        )))
        with self.assertRaises(LoopLabelingError):
            label_function(fn)

    def test_break_after_inner_loop_ends_at_top_level_raises(self):
        # `while (1) ; break;`  — the break is OUTSIDE the while loop,
        # so it has no enclosing loop and must raise.
        fn = _function(
            _while(c99_ast.Constant(value=1), c99_ast.Null()),
            _break(),
        )
        with self.assertRaises(LoopLabelingError):
            label_function(fn)


class TestBreakInsideIfBranchInsideLoop(unittest.TestCase):
    """An `if` inside a loop body doesn't create a new loop scope; the
    break/continue inside an if-branch still targets the enclosing
    loop."""

    def test_break_in_if_then_inside_while(self):
        # while (1) if (1) break;
        fn = _function(_while(
            c99_ast.Constant(value=1),
            c99_ast.IfStmt(
                condition=c99_ast.Constant(value=1),
                then_clause=c99_ast.BreakStmt(label=""),
                else_clause=None,
            ),
        ))
        labeled = label_function(fn)
        while_stmt = labeled.body.block_item[0].statement
        self.assertEqual(while_stmt.body.then_clause,
                         c99_ast.BreakStmt(label=".loop@0"))

    def test_break_in_if_else_inside_while(self):
        # while (1) if (1) ; else continue;
        fn = _function(_while(
            c99_ast.Constant(value=1),
            c99_ast.IfStmt(
                condition=c99_ast.Constant(value=1),
                then_clause=c99_ast.Null(),
                else_clause=c99_ast.ContinueStmt(label=""),
            ),
        ))
        labeled = label_function(fn)
        while_stmt = labeled.body.block_item[0].statement
        self.assertEqual(while_stmt.body.else_clause,
                         c99_ast.ContinueStmt(label=".loop@0"))

    def test_break_inside_labeled_stmt_inside_loop(self):
        # while (1) foo: break;   — the labeled statement inside the
        # loop doesn't change loop scope.
        fn = _function(_while(
            c99_ast.Constant(value=1),
            c99_ast.LabeledStmt(
                label="foo", statement=c99_ast.BreakStmt(label=""),
            ),
        ))
        labeled = label_function(fn)
        while_stmt = labeled.body.block_item[0].statement
        labeled_inner = while_stmt.body
        self.assertEqual(labeled_inner.statement,
                         c99_ast.BreakStmt(label=".loop@0"))


class TestPassthrough(unittest.TestCase):
    """Statements that aren't loops and don't contain break/continue
    should land in the output untouched. (label_resolution-rewritten
    goto/labeled-stmt strings must pass through unchanged so the loop
    pass doesn't have to know about that namespace.)"""

    def test_function_without_loops(self):
        fn = _function(_ret(c99_ast.Constant(value=42)))
        self.assertEqual(label_function(fn), fn)

    def test_if_with_no_loops_or_breaks(self):
        fn = _function(c99_ast.S(statement=c99_ast.IfStmt(
            condition=c99_ast.Constant(value=1),
            then_clause=c99_ast.Return(exp=c99_ast.Constant(value=2)),
            else_clause=c99_ast.Return(exp=c99_ast.Constant(value=3)),
        )))
        self.assertEqual(label_function(fn), fn)

    def test_goto_and_labeled_stmt_pass_through(self):
        # The labels here have already been rewritten by
        # label_resolution; loop_labeling must leave them alone.
        fn = _function(
            c99_ast.S(statement=c99_ast.Goto(label=".main@end")),
            c99_ast.S(statement=c99_ast.LabeledStmt(
                label=".main@end",
                statement=c99_ast.Return(exp=c99_ast.Constant(value=0)),
            )),
        )
        self.assertEqual(label_function(fn), fn)


class TestLabelProgram(unittest.TestCase):
    def test_wraps_function_in_program(self):
        fn = _function(_while(c99_ast.Constant(value=1), c99_ast.BreakStmt(label="")))
        prog = c99_ast.Program(function_definition=fn)
        labeled = label_program(prog)
        labeled_while = labeled.function_definition.body.block_item[0].statement
        self.assertEqual(labeled_while.label, ".loop@0")
        self.assertEqual(labeled_while.body,
                         c99_ast.BreakStmt(label=".loop@0"))


class TestIntegrationWithParser(unittest.TestCase):
    """End-to-end from source through parse + label_program, so the
    tests stay pinned to the AST shapes the parser actually produces.
    These bypass variable_resolution / label_resolution because
    loop_labeling doesn't depend on either pass having run."""

    def test_while_with_break_from_source(self):
        prog = parse("int main(void) { while (1) break; return 0; }")
        labeled = label_program(prog)
        items = labeled.function_definition.body.block_item
        while_stmt = items[0].statement
        self.assertEqual(while_stmt.label, ".loop@0")
        self.assertEqual(while_stmt.body, c99_ast.BreakStmt(label=".loop@0"))

    def test_for_with_continue_from_source(self):
        prog = parse(
            "int main(void) { for (;;) continue; return 0; }"
        )
        labeled = label_program(prog)
        for_stmt = labeled.function_definition.body.block_item[0].statement
        self.assertEqual(for_stmt.label, ".loop@0")
        self.assertEqual(for_stmt.body, c99_ast.ContinueStmt(label=".loop@0"))

    def test_nested_loops_from_source(self):
        # Each break / continue targets its innermost enclosing loop.
        prog = parse(
            "int main(void) { "
            "while (1) { for (;;) { break; continue; } break; } "
            "return 0; }"
        )
        labeled = label_program(prog)
        outer = labeled.function_definition.body.block_item[0].statement
        outer_items = outer.body.block.block_item
        for_stmt = outer_items[0].statement
        for_items = for_stmt.body.block.block_item
        self.assertEqual(outer.label, ".loop@0")
        self.assertEqual(for_stmt.label, ".loop@1")
        self.assertEqual(for_items[0].statement,
                         c99_ast.BreakStmt(label=".loop@1"))
        self.assertEqual(for_items[1].statement,
                         c99_ast.ContinueStmt(label=".loop@1"))
        self.assertEqual(outer_items[1].statement,
                         c99_ast.BreakStmt(label=".loop@0"))

    def test_break_outside_loop_from_source_raises(self):
        prog = parse("int main(void) { break; return 0; }")
        with self.assertRaises(LoopLabelingError):
            label_program(prog)

    def test_continue_outside_loop_from_source_raises(self):
        prog = parse("int main(void) { continue; return 0; }")
        with self.assertRaises(LoopLabelingError):
            label_program(prog)

    def test_break_in_if_outside_loop_from_source_raises(self):
        prog = parse("int main(void) { if (1) break; return 0; }")
        with self.assertRaises(LoopLabelingError):
            label_program(prog)


class TestErrors(unittest.TestCase):
    def test_unknown_statement_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_statement,), {})
        with self.assertRaises(TypeError):
            LoopLabeler().label_statement(stub(), current_loop=None)

    def test_unknown_block_item_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_block_item,), {})
        with self.assertRaises(TypeError):
            LoopLabeler().label_block_item(stub(), current_loop=None)

    def test_unknown_block_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_block,), {})
        with self.assertRaises(TypeError):
            LoopLabeler().label_block(stub(), current_loop=None)


if __name__ == "__main__":
    unittest.main()
