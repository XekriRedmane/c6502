import unittest

import c99_ast
from parser import parse
from passes.label_resolution import (
    LabelResolutionError,
    LabelResolver,
    resolve_function,
    resolve_program,
)


def _function(*body_items, name="main") -> c99_ast.Type_function_definition:
    return c99_ast.Function(name=name, body=list(body_items))


def _ret(exp) -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.Return(exp=exp))


def _expr(exp) -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.Expression(exp=exp))


def _null() -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.Null())


def _goto(label) -> c99_ast.Type_block_item:
    return c99_ast.S(statement=c99_ast.Goto(label=label))


def _labeled(label, inner) -> c99_ast.Type_block_item:
    return c99_ast.S(
        statement=c99_ast.LabeledStmt(label=label, statement=inner),
    )


class TestLabelRewriting(unittest.TestCase):
    def test_label_is_rewritten_with_function_prefix(self):
        # `foo: return 0;` -> label becomes `.main@foo`.
        fn = _function(_labeled("foo", c99_ast.Return(
            exp=c99_ast.Constant(value=0),
        )))
        resolved = resolve_function(fn)
        self.assertEqual(
            resolved,
            _function(_labeled(".main@foo", c99_ast.Return(
                exp=c99_ast.Constant(value=0),
            ))),
        )

    def test_goto_is_rewritten_to_match_label(self):
        # `goto foo; foo: ;` — both rewrites use the same `.main@foo`.
        fn = _function(
            _goto("foo"),
            _labeled("foo", c99_ast.Null()),
        )
        resolved = resolve_function(fn)
        self.assertEqual(
            resolved,
            _function(
                _goto(".main@foo"),
                _labeled(".main@foo", c99_ast.Null()),
            ),
        )

    def test_label_can_be_used_before_declaration(self):
        # Labels are visible across the entire function body — a goto
        # to a label that appears later still resolves.
        fn = _function(
            _goto("end"),
            _labeled("end", c99_ast.Return(exp=c99_ast.Constant(value=0))),
        )
        # Should not raise.
        resolve_function(fn)

    def test_function_name_used_in_prefix(self):
        # The prefix follows the function name, not a hard-coded
        # "main". Different functions get different prefixes.
        fn = _function(
            _labeled("foo", c99_ast.Null()),
            name="other",
        )
        resolved = resolve_function(fn)
        self.assertEqual(
            resolved.body[0].statement.label,
            ".other@foo",
        )


class TestDuplicateLabels(unittest.TestCase):
    def test_two_labels_with_same_name_raises(self):
        fn = _function(
            _labeled("foo", c99_ast.Null()),
            _labeled("foo", c99_ast.Null()),
        )
        with self.assertRaises(LabelResolutionError) as ctx:
            resolve_function(fn)
        self.assertIn("'foo'", str(ctx.exception))

    def test_duplicate_inside_if_branch_raises(self):
        # A label in the if-then and another with the same name in
        # the if-else (or elsewhere) is still a duplicate within the
        # function.
        fn = _function(
            c99_ast.S(statement=c99_ast.IfStmt(
                condition=c99_ast.Constant(value=1),
                then_clause=c99_ast.LabeledStmt(
                    label="x", statement=c99_ast.Null(),
                ),
                else_clause=c99_ast.LabeledStmt(
                    label="x", statement=c99_ast.Null(),
                ),
            )),
        )
        with self.assertRaises(LabelResolutionError):
            resolve_function(fn)

    def test_nested_labels_with_same_name_raises(self):
        # `a: a: ;` — the outer and inner label share a name.
        fn = _function(_labeled(
            "a", c99_ast.LabeledStmt(label="a", statement=c99_ast.Null()),
        ))
        with self.assertRaises(LabelResolutionError):
            resolve_function(fn)


class TestUndefinedGoto(unittest.TestCase):
    def test_goto_to_nonexistent_label_raises(self):
        fn = _function(_goto("foo"))
        with self.assertRaises(LabelResolutionError) as ctx:
            resolve_function(fn)
        self.assertIn("'foo'", str(ctx.exception))

    def test_goto_inside_if_branch_to_nonexistent_label_raises(self):
        fn = _function(
            c99_ast.S(statement=c99_ast.IfStmt(
                condition=c99_ast.Constant(value=1),
                then_clause=c99_ast.Goto(label="missing"),
                else_clause=None,
            )),
        )
        with self.assertRaises(LabelResolutionError):
            resolve_function(fn)


class TestLabelsInIfStatements(unittest.TestCase):
    def test_label_inside_if_then_is_visible(self):
        # `if (1) foo: ; goto foo;` — the label inside the if-then is
        # visible from outside it (function-wide visibility).
        fn = _function(
            c99_ast.S(statement=c99_ast.IfStmt(
                condition=c99_ast.Constant(value=1),
                then_clause=c99_ast.LabeledStmt(
                    label="foo", statement=c99_ast.Null(),
                ),
                else_clause=None,
            )),
            _goto("foo"),
        )
        resolved = resolve_function(fn)
        if_stmt = resolved.body[0].statement
        self.assertEqual(
            if_stmt.then_clause,
            c99_ast.LabeledStmt(label=".main@foo", statement=c99_ast.Null()),
        )
        self.assertEqual(
            resolved.body[1].statement,
            c99_ast.Goto(label=".main@foo"),
        )

    def test_goto_inside_if_branch_resolves_to_outer_label(self):
        # `foo: ; if (1) goto foo;` — goto in the if-branch still sees
        # the function-wide label.
        fn = _function(
            _labeled("foo", c99_ast.Null()),
            c99_ast.S(statement=c99_ast.IfStmt(
                condition=c99_ast.Constant(value=1),
                then_clause=c99_ast.Goto(label="foo"),
                else_clause=None,
            )),
        )
        resolved = resolve_function(fn)
        if_stmt = resolved.body[1].statement
        self.assertEqual(if_stmt.then_clause, c99_ast.Goto(label=".main@foo"))


class TestPassthrough(unittest.TestCase):
    """Statements without labels or gotos should pass through
    unchanged (modulo the new AST allocations the pass makes for
    consistency with variable_resolution)."""

    def test_function_without_labels_or_gotos(self):
        fn = _function(
            _ret(c99_ast.Constant(value=42)),
        )
        self.assertEqual(resolve_function(fn), fn)

    def test_nested_if_without_labels(self):
        fn = _function(
            c99_ast.S(statement=c99_ast.IfStmt(
                condition=c99_ast.Constant(value=1),
                then_clause=c99_ast.Return(exp=c99_ast.Constant(value=2)),
                else_clause=c99_ast.Return(exp=c99_ast.Constant(value=3)),
            )),
        )
        self.assertEqual(resolve_function(fn), fn)

    def test_declarations_pass_through(self):
        # Declarations don't introduce labels and shouldn't be
        # touched by this pass.
        decl = c99_ast.D(declaration=c99_ast.Declaration(
            name="x", init=c99_ast.Constant(value=5),
        ))
        fn = _function(decl, _ret(c99_ast.Var(name="x")))
        resolved = resolve_function(fn)
        # Block items are rebuilt but content is identical.
        self.assertEqual(resolved.body[0], decl)


class TestResolveProgram(unittest.TestCase):
    def test_wraps_function_in_program(self):
        fn = _function(
            _labeled("foo", c99_ast.Null()),
            _goto("foo"),
        )
        prog = c99_ast.Program(function_definition=fn)
        resolved = resolve_program(prog)
        self.assertEqual(
            resolved,
            c99_ast.Program(function_definition=_function(
                _labeled(".main@foo", c99_ast.Null()),
                _goto(".main@foo"),
            )),
        )


class TestIntegrationWithParser(unittest.TestCase):
    """End-to-end from source through parse + label_resolution, so
    the tests stay tied to the AST shapes the parser actually emits."""

    def test_basic_program(self):
        prog = parse("int main(void) { foo: goto foo; return 0; }")
        resolved = resolve_program(prog)
        body = resolved.function_definition.body
        self.assertEqual(
            body[0].statement,
            c99_ast.LabeledStmt(
                label=".main@foo",
                statement=c99_ast.Goto(label=".main@foo"),
            ),
        )

    def test_duplicate_label_from_source(self):
        prog = parse("int main(void) { foo: ; foo: ; return 0; }")
        with self.assertRaises(LabelResolutionError):
            resolve_program(prog)

    def test_undefined_goto_target_from_source(self):
        prog = parse("int main(void) { goto nowhere; return 0; }")
        with self.assertRaises(LabelResolutionError):
            resolve_program(prog)


class TestErrors(unittest.TestCase):
    def test_unknown_block_item_raises_type_error(self):
        stub = type("Stub", (c99_ast.Type_block_item,), {})
        with self.assertRaises(TypeError):
            LabelResolver()._rewrite_block_item(stub(), {})


if __name__ == "__main__":
    unittest.main()
