import shutil
import subprocess
import unittest
from pathlib import Path

from lark.exceptions import UnexpectedInput

import c99_ast
from lexer import LexError
from parser import parse


_TESTS_DIR = Path(__file__).parent


def _preprocess(src: str) -> str:
    result = subprocess.run(
        ["pcpp", "-", "--line-directive"],
        input=src, capture_output=True, text=True, check=True,
    )
    return result.stdout


class TestParser(unittest.TestCase):
    def test_minimal_function(self):
        ast = parse("int main(void) { return 42; }")
        expected = c99_ast.Program(
            function_definition=c99_ast.Function(
                name="main",
                body=c99_ast.Block(block_item=[c99_ast.S(
                    statement=c99_ast.Return(
                        exp=c99_ast.Constant(value=42),
                    ),
                )]),
            ),
        )
        self.assertEqual(ast, expected)

    def test_whitespace_insensitive(self):
        for src in [
            "int main(void){return 42;}",
            "int  main  ( void )  {  return  42  ;  }",
            "int\nmain(void)\n{\n    return 42;\n}",
        ]:
            with self.subTest(src=src):
                self.assertEqual(_return_stmt(parse(src)).exp.value, 42)

    def test_various_return_values(self):
        for val in [0, 1, 42, 255, 1000, 0xDEADBEEF]:
            with self.subTest(val=val):
                ast = parse(f"int main(void) {{ return {val}; }}")
                self.assertEqual(_return_stmt(ast).exp.value, val)

    def test_function_name_captured(self):
        for name in ["main", "foo", "_start", "a1b2"]:
            with self.subTest(name=name):
                ast = parse(f"int {name}(void) {{ return 0; }}")
                self.assertEqual(ast.function_definition.name, name)

    def test_unary_negate(self):
        ast = parse("int main(void) { return -42; }")
        self.assertEqual(
            _return_stmt(ast),
            c99_ast.Return(exp=c99_ast.Unary(
                op=c99_ast.Negate(),
                exp=c99_ast.Constant(value=42),
            )),
        )

    def test_unary_complement(self):
        ast = parse("int main(void) { return ~10; }")
        self.assertEqual(
            _return_stmt(ast),
            c99_ast.Return(exp=c99_ast.Unary(
                op=c99_ast.Complement(),
                exp=c99_ast.Constant(value=10),
            )),
        )

    def test_parens_do_not_appear_in_ast(self):
        ast = parse("int main(void) { return (42); }")
        self.assertEqual(
            _return_stmt(ast),
            c99_ast.Return(exp=c99_ast.Constant(value=42)),
        )

    def test_nested_unary(self):
        ast = parse("int main(void) { return -(-42); }")
        self.assertEqual(
            _return_stmt(ast).exp,
            c99_ast.Unary(
                op=c99_ast.Negate(),
                exp=c99_ast.Unary(
                    op=c99_ast.Negate(),
                    exp=c99_ast.Constant(value=42),
                ),
            ),
        )

    def test_mixed_unary_with_parens(self):
        ast = parse("int main(void) { return ~(-5); }")
        self.assertEqual(
            _return_stmt(ast).exp,
            c99_ast.Unary(
                op=c99_ast.Complement(),
                exp=c99_ast.Unary(
                    op=c99_ast.Negate(),
                    exp=c99_ast.Constant(value=5),
                ),
            ),
        )

    def test_returned_ast_types(self):
        ast = parse("int main(void) { return 0; }")
        self.assertIsInstance(ast, c99_ast.Type_program)
        self.assertIsInstance(ast, c99_ast.Program)
        self.assertIsInstance(ast.function_definition, c99_ast.Type_function_definition)
        self.assertIsInstance(ast.function_definition, c99_ast.Function)
        # body is a Block wrapping a list of block_items.
        body = ast.function_definition.body
        self.assertIsInstance(body, c99_ast.Type_block)
        self.assertIsInstance(body, c99_ast.Block)
        self.assertEqual(len(body.block_item), 1)
        item = body.block_item[0]
        self.assertIsInstance(item, c99_ast.Type_block_item)
        self.assertIsInstance(item, c99_ast.S)
        self.assertIsInstance(item.statement, c99_ast.Type_statement)
        self.assertIsInstance(item.statement, c99_ast.Return)
        self.assertIsInstance(item.statement.exp, c99_ast.Type_exp)
        self.assertIsInstance(item.statement.exp, c99_ast.Constant)


def _return_stmt(ast: c99_ast.Type_program) -> c99_ast.Return:
    """Extract the single Return statement from
    `int main(void) { return <exp>; }`. Function.body is a Block
    around a list of block_items; here we expect exactly one
    S(statement=Return)."""
    items = ast.function_definition.body.block_item
    assert len(items) == 1, items
    item = items[0]
    assert isinstance(item, c99_ast.S), item
    stmt = item.statement
    assert isinstance(stmt, c99_ast.Return), stmt
    return stmt


def _exp_of(src: str) -> c99_ast.Type_exp:
    return _return_stmt(parse(f"int main(void) {{ return {src}; }}")).exp


class TestBinaryPrecedence(unittest.TestCase):
    def test_add_then_multiply_groups_multiply(self):
        # 1 + 2 * 3  ->  +(1, *(2, 3))
        # The * has two int children; the + has int left + Binary right.
        self.assertEqual(
            _exp_of("1 + 2 * 3"),
            c99_ast.Binary(
                op=c99_ast.Add(),
                left=c99_ast.Constant(value=1),
                right=c99_ast.Binary(
                    op=c99_ast.Multiply(),
                    left=c99_ast.Constant(value=2),
                    right=c99_ast.Constant(value=3),
                ),
            ),
        )

    def test_multiply_then_add_groups_multiply(self):
        # 1 * 2 + 3  ->  +(*(1, 2), 3)
        # The * has two int children; the + has Binary left + int right.
        self.assertEqual(
            _exp_of("1 * 2 + 3"),
            c99_ast.Binary(
                op=c99_ast.Add(),
                left=c99_ast.Binary(
                    op=c99_ast.Multiply(),
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                ),
                right=c99_ast.Constant(value=3),
            ),
        )

    def test_multiply_add_multiply(self):
        # 1 * 2 + 3 * 4  ->  +(*(1, 2), *(3, 4))
        # Both * nodes have two int children; the + has Binary on
        # both sides.
        self.assertEqual(
            _exp_of("1 * 2 + 3 * 4"),
            c99_ast.Binary(
                op=c99_ast.Add(),
                left=c99_ast.Binary(
                    op=c99_ast.Multiply(),
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                ),
                right=c99_ast.Binary(
                    op=c99_ast.Multiply(),
                    left=c99_ast.Constant(value=3),
                    right=c99_ast.Constant(value=4),
                ),
            ),
        )

    def test_left_associative_subtract(self):
        # 1 - 2 - 3  ->  -(-(1, 2), 3)
        self.assertEqual(
            _exp_of("1 - 2 - 3"),
            c99_ast.Binary(
                op=c99_ast.Subtract(),
                left=c99_ast.Binary(
                    op=c99_ast.Subtract(),
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                ),
                right=c99_ast.Constant(value=3),
            ),
        )

    def test_left_associative_divide(self):
        # 1 / 2 / 3  ->  /(/(1, 2), 3)
        self.assertEqual(
            _exp_of("1 / 2 / 3"),
            c99_ast.Binary(
                op=c99_ast.Divide(),
                left=c99_ast.Binary(
                    op=c99_ast.Divide(),
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                ),
                right=c99_ast.Constant(value=3),
            ),
        )

    def test_parens_override_precedence(self):
        # (1 + 2) * 3  ->  *(+(1, 2), 3)
        self.assertEqual(
            _exp_of("(1 + 2) * 3"),
            c99_ast.Binary(
                op=c99_ast.Multiply(),
                left=c99_ast.Binary(
                    op=c99_ast.Add(),
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                ),
                right=c99_ast.Constant(value=3),
            ),
        )

    def test_unary_binds_tighter_than_multiply(self):
        # -1 * 2  ->  *(-1, 2)
        self.assertEqual(
            _exp_of("-1 * 2"),
            c99_ast.Binary(
                op=c99_ast.Multiply(),
                left=c99_ast.Unary(
                    op=c99_ast.Negate(), exp=c99_ast.Constant(value=1),
                ),
                right=c99_ast.Constant(value=2),
            ),
        )

    def test_modulo(self):
        # 10 % 3  ->  %(10, 3)
        self.assertEqual(
            _exp_of("10 % 3"),
            c99_ast.Binary(
                op=c99_ast.Modulo(),
                left=c99_ast.Constant(value=10),
                right=c99_ast.Constant(value=3),
            ),
        )


class TestBitwiseAndShiftBinaryOps(unittest.TestCase):
    """Bitwise (&, |, ^) and shift (<<, >>) operators. Precedence
    relative to each other and to the arithmetic operators follows
    C99 §6.5: shifts bind tighter than bitwise, and within bitwise
    the order tightest-to-loosest is &, ^, |."""

    def test_each_op_builds_a_binary(self):
        cases = [
            ("&",  c99_ast.BitwiseAnd()),
            ("|",  c99_ast.BitwiseOr()),
            ("^",  c99_ast.BitwiseXor()),
            ("<<", c99_ast.LeftShift()),
            (">>", c99_ast.RightShift()),
        ]
        for sym, op in cases:
            with self.subTest(sym=sym):
                self.assertEqual(
                    _exp_of(f"5 {sym} 3"),
                    c99_ast.Binary(
                        op=op,
                        left=c99_ast.Constant(value=5),
                        right=c99_ast.Constant(value=3),
                    ),
                )

    def test_shift_binds_tighter_than_bitwise_and(self):
        # 1 & 2 << 3 -> &(1, <<(2, 3))
        self.assertEqual(
            _exp_of("1 & 2 << 3"),
            c99_ast.Binary(
                op=c99_ast.BitwiseAnd(),
                left=c99_ast.Constant(value=1),
                right=c99_ast.Binary(
                    op=c99_ast.LeftShift(),
                    left=c99_ast.Constant(value=2),
                    right=c99_ast.Constant(value=3),
                ),
            ),
        )

    def test_add_binds_tighter_than_shift(self):
        # 1 << 2 + 3 -> <<(1, +(2, 3))
        self.assertEqual(
            _exp_of("1 << 2 + 3"),
            c99_ast.Binary(
                op=c99_ast.LeftShift(),
                left=c99_ast.Constant(value=1),
                right=c99_ast.Binary(
                    op=c99_ast.Add(),
                    left=c99_ast.Constant(value=2),
                    right=c99_ast.Constant(value=3),
                ),
            ),
        )

    def test_and_binds_tighter_than_xor(self):
        # 1 ^ 2 & 3 -> ^(1, &(2, 3))
        self.assertEqual(
            _exp_of("1 ^ 2 & 3"),
            c99_ast.Binary(
                op=c99_ast.BitwiseXor(),
                left=c99_ast.Constant(value=1),
                right=c99_ast.Binary(
                    op=c99_ast.BitwiseAnd(),
                    left=c99_ast.Constant(value=2),
                    right=c99_ast.Constant(value=3),
                ),
            ),
        )

    def test_xor_binds_tighter_than_or(self):
        # 1 | 2 ^ 3 -> |(1, ^(2, 3))
        self.assertEqual(
            _exp_of("1 | 2 ^ 3"),
            c99_ast.Binary(
                op=c99_ast.BitwiseOr(),
                left=c99_ast.Constant(value=1),
                right=c99_ast.Binary(
                    op=c99_ast.BitwiseXor(),
                    left=c99_ast.Constant(value=2),
                    right=c99_ast.Constant(value=3),
                ),
            ),
        )

    def test_left_associative_shift(self):
        # 1 << 2 << 3 -> <<(<<(1, 2), 3)
        self.assertEqual(
            _exp_of("1 << 2 << 3"),
            c99_ast.Binary(
                op=c99_ast.LeftShift(),
                left=c99_ast.Binary(
                    op=c99_ast.LeftShift(),
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                ),
                right=c99_ast.Constant(value=3),
            ),
        )

    def test_left_associative_or(self):
        # 1 | 2 | 3 -> |(|(1, 2), 3)
        self.assertEqual(
            _exp_of("1 | 2 | 3"),
            c99_ast.Binary(
                op=c99_ast.BitwiseOr(),
                left=c99_ast.Binary(
                    op=c99_ast.BitwiseOr(),
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                ),
                right=c99_ast.Constant(value=3),
            ),
        )


class TestComparisonOps(unittest.TestCase):
    """Equality (==, !=) and relational (<, >, <=, >=) operators.
    Per C99 §6.5: relational binds tighter than equality, both bind
    looser than shift but tighter than bitwise AND. Each is left-
    associative."""

    def test_each_op_builds_a_binary(self):
        cases = [
            ("==", c99_ast.Equal()),
            ("!=", c99_ast.NotEqual()),
            ("<",  c99_ast.LessThan()),
            (">",  c99_ast.GreaterThan()),
            ("<=", c99_ast.LessOrEqual()),
            (">=", c99_ast.GreaterOrEqual()),
        ]
        for sym, op in cases:
            with self.subTest(sym=sym):
                self.assertEqual(
                    _exp_of(f"5 {sym} 3"),
                    c99_ast.Binary(
                        op=op,
                        left=c99_ast.Constant(value=5),
                        right=c99_ast.Constant(value=3),
                    ),
                )

    def test_relational_binds_tighter_than_equality(self):
        # 1 == 2 < 3 -> ==(1, <(2, 3))
        self.assertEqual(
            _exp_of("1 == 2 < 3"),
            c99_ast.Binary(
                op=c99_ast.Equal(),
                left=c99_ast.Constant(value=1),
                right=c99_ast.Binary(
                    op=c99_ast.LessThan(),
                    left=c99_ast.Constant(value=2),
                    right=c99_ast.Constant(value=3),
                ),
            ),
        )

    def test_shift_binds_tighter_than_relational(self):
        # 1 < 2 << 3 -> <(1, <<(2, 3))
        self.assertEqual(
            _exp_of("1 < 2 << 3"),
            c99_ast.Binary(
                op=c99_ast.LessThan(),
                left=c99_ast.Constant(value=1),
                right=c99_ast.Binary(
                    op=c99_ast.LeftShift(),
                    left=c99_ast.Constant(value=2),
                    right=c99_ast.Constant(value=3),
                ),
            ),
        )

    def test_equality_binds_tighter_than_bitwise_and(self):
        # 1 & 2 == 3 -> &(1, ==(2, 3))
        self.assertEqual(
            _exp_of("1 & 2 == 3"),
            c99_ast.Binary(
                op=c99_ast.BitwiseAnd(),
                left=c99_ast.Constant(value=1),
                right=c99_ast.Binary(
                    op=c99_ast.Equal(),
                    left=c99_ast.Constant(value=2),
                    right=c99_ast.Constant(value=3),
                ),
            ),
        )

    def test_left_associative_equal(self):
        # 1 == 2 == 3 -> ==(==(1, 2), 3) — well-formed C, parses
        # left-to-right; semantics aren't useful but the AST shape
        # should reflect left-associativity.
        self.assertEqual(
            _exp_of("1 == 2 == 3"),
            c99_ast.Binary(
                op=c99_ast.Equal(),
                left=c99_ast.Binary(
                    op=c99_ast.Equal(),
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                ),
                right=c99_ast.Constant(value=3),
            ),
        )


class TestLogicalNotUnary(unittest.TestCase):
    """! shares unary precedence with - and ~ (right-to-left)."""

    def test_basic(self):
        self.assertEqual(
            _exp_of("!5"),
            c99_ast.Unary(
                op=c99_ast.LogicalNot(),
                exp=c99_ast.Constant(value=5),
            ),
        )

    def test_double_not(self):
        # !!x -> !(!x); right-to-left associativity for unary prefix.
        self.assertEqual(
            _exp_of("!!5"),
            c99_ast.Unary(
                op=c99_ast.LogicalNot(),
                exp=c99_ast.Unary(
                    op=c99_ast.LogicalNot(),
                    exp=c99_ast.Constant(value=5),
                ),
            ),
        )

    def test_binds_tighter_than_multiply(self):
        # !1 * 2  ->  *(!1, 2)
        self.assertEqual(
            _exp_of("!1 * 2"),
            c99_ast.Binary(
                op=c99_ast.Multiply(),
                left=c99_ast.Unary(
                    op=c99_ast.LogicalNot(), exp=c99_ast.Constant(value=1),
                ),
                right=c99_ast.Constant(value=2),
            ),
        )

    def test_mixes_with_other_unaries(self):
        # !-5  ->  !(-5)
        self.assertEqual(
            _exp_of("!-5"),
            c99_ast.Unary(
                op=c99_ast.LogicalNot(),
                exp=c99_ast.Unary(
                    op=c99_ast.Negate(),
                    exp=c99_ast.Constant(value=5),
                ),
            ),
        )


class TestAssignment(unittest.TestCase):
    """Plain `=` and the ten compound assignments. The grammar's LHS is
    `logical_or_exp` rather than C99's `unary_exp`, so things like
    `1+2 = 3` parse here — variable_resolution rejects them later.
    Compound `OP=` desugars at parse time to `lval = lval OP rval`,
    sharing the `lval` node by reference (safe today because the only
    legal lval is a `Var`, which has no side effect when re-evaluated)."""

    def test_plain_assignment(self):
        self.assertEqual(
            _exp_of("a = 1"),
            c99_ast.Assignment(
                lval=c99_ast.Var(name="a"),
                rval=c99_ast.Constant(value=1),
            ),
        )

    def test_assignment_is_right_associative(self):
        # `a = b = 1` parses as `a = (b = 1)`.
        self.assertEqual(
            _exp_of("a = b = 1"),
            c99_ast.Assignment(
                lval=c99_ast.Var(name="a"),
                rval=c99_ast.Assignment(
                    lval=c99_ast.Var(name="b"),
                    rval=c99_ast.Constant(value=1),
                ),
            ),
        )

    def test_each_compound_op_desugars(self):
        # Every `a OP= 1` rewrites to `Assignment(a, Binary(OP, a, 1))`.
        cases = [
            ("+=",  c99_ast.Add()),
            ("-=",  c99_ast.Subtract()),
            ("*=",  c99_ast.Multiply()),
            ("/=",  c99_ast.Divide()),
            ("%=",  c99_ast.Modulo()),
            ("&=",  c99_ast.BitwiseAnd()),
            ("|=",  c99_ast.BitwiseOr()),
            ("^=",  c99_ast.BitwiseXor()),
            ("<<=", c99_ast.LeftShift()),
            (">>=", c99_ast.RightShift()),
        ]
        for sym, op in cases:
            with self.subTest(sym=sym):
                self.assertEqual(
                    _exp_of(f"a {sym} 1"),
                    c99_ast.Assignment(
                        lval=c99_ast.Var(name="a"),
                        rval=c99_ast.Binary(
                            op=op,
                            left=c99_ast.Var(name="a"),
                            right=c99_ast.Constant(value=1),
                        ),
                    ),
                )

    def test_compound_assign_is_right_associative(self):
        # `a += b += 1` parses as `a += (b += 1)`, then desugars to
        # `a = a + (b = b + 1)`. The inner Assignment is the rval-side
        # operand of the outer Add.
        self.assertEqual(
            _exp_of("a += b += 1"),
            c99_ast.Assignment(
                lval=c99_ast.Var(name="a"),
                rval=c99_ast.Binary(
                    op=c99_ast.Add(),
                    left=c99_ast.Var(name="a"),
                    right=c99_ast.Assignment(
                        lval=c99_ast.Var(name="b"),
                        rval=c99_ast.Binary(
                            op=c99_ast.Add(),
                            left=c99_ast.Var(name="b"),
                            right=c99_ast.Constant(value=1),
                        ),
                    ),
                ),
            ),
        )

    def test_compound_assign_rhs_is_full_expression(self):
        # The rval slot is `assignment_exp`, which means a full binary
        # expression goes in unparenthesized — `a += 1 + 2` desugars
        # to `a = a + (1 + 2)`, NOT `(a + 1) + 2`. Right-recursion at
        # the assignment level keeps the rval intact.
        self.assertEqual(
            _exp_of("a += 1 + 2"),
            c99_ast.Assignment(
                lval=c99_ast.Var(name="a"),
                rval=c99_ast.Binary(
                    op=c99_ast.Add(),
                    left=c99_ast.Var(name="a"),
                    right=c99_ast.Binary(
                        op=c99_ast.Add(),
                        left=c99_ast.Constant(value=1),
                        right=c99_ast.Constant(value=2),
                    ),
                ),
            ),
        )

    def test_compound_assign_invalid_lhs_still_parses(self):
        # `1 += 2` parses (LHS is `logical_or_exp`, which Constant
        # satisfies) — variable_resolution is what rejects it. The
        # desugared AST is `Assignment(Constant(1), Binary(Add,
        # Constant(1), Constant(2)))`.
        self.assertEqual(
            _exp_of("1 += 2"),
            c99_ast.Assignment(
                lval=c99_ast.Constant(value=1),
                rval=c99_ast.Binary(
                    op=c99_ast.Add(),
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                ),
            ),
        )


class TestIncrementDecrement(unittest.TestCase):
    """Prefix `++a` / `--a` desugar at parse time to `a = a ± 1`
    (same shape as compound assignment). Postfix `a++` / `a--` keep
    their own AST node because they evaluate to the *old* value of
    the operand. Postfix binds tighter than prefix and tighter than
    other unary ops."""

    def test_pre_increment_desugars_to_assignment(self):
        self.assertEqual(
            _exp_of("++a"),
            c99_ast.Assignment(
                lval=c99_ast.Var(name="a"),
                rval=c99_ast.Binary(
                    op=c99_ast.Add(),
                    left=c99_ast.Var(name="a"),
                    right=c99_ast.Constant(value=1),
                ),
            ),
        )

    def test_pre_decrement_desugars_to_assignment(self):
        self.assertEqual(
            _exp_of("--a"),
            c99_ast.Assignment(
                lval=c99_ast.Var(name="a"),
                rval=c99_ast.Binary(
                    op=c99_ast.Subtract(),
                    left=c99_ast.Var(name="a"),
                    right=c99_ast.Constant(value=1),
                ),
            ),
        )

    def test_post_increment_builds_postfix_node(self):
        self.assertEqual(
            _exp_of("a++"),
            c99_ast.Postfix(
                op=c99_ast.Increment(),
                operand=c99_ast.Var(name="a"),
            ),
        )

    def test_post_decrement_builds_postfix_node(self):
        self.assertEqual(
            _exp_of("a--"),
            c99_ast.Postfix(
                op=c99_ast.Decrement(),
                operand=c99_ast.Var(name="a"),
            ),
        )

    def test_postfix_binds_tighter_than_unary_minus(self):
        # `-a++` parses as `-(a++)`, NOT `(-a)++`. Postfix is at the
        # postfix-precedence level (one step inside unary).
        self.assertEqual(
            _exp_of("-a++"),
            c99_ast.Unary(
                op=c99_ast.Negate(),
                exp=c99_ast.Postfix(
                    op=c99_ast.Increment(),
                    operand=c99_ast.Var(name="a"),
                ),
            ),
        )

    def test_postfix_binds_tighter_than_prefix(self):
        # `++a++` parses as `++(a++)` — desugared, the prefix becomes
        # an Assignment whose lval is the Postfix node. (Semantically
        # invalid C — `a++` isn't an lvalue — but the grammar accepts
        # it and variable_resolution will catch it.)
        post = c99_ast.Postfix(
            op=c99_ast.Increment(), operand=c99_ast.Var(name="a"),
        )
        self.assertEqual(
            _exp_of("++a++"),
            c99_ast.Assignment(
                lval=post,
                rval=c99_ast.Binary(
                    op=c99_ast.Add(),
                    left=post,
                    right=c99_ast.Constant(value=1),
                ),
            ),
        )

    def test_plus_plus_plus_lexes_as_two_tokens(self):
        # `a+++b` is `a++ + b` (max-munch; `++` wins over `+ +`), so
        # the AST is Add(Postfix(Increment, a), b).
        self.assertEqual(
            _exp_of("a+++b"),
            c99_ast.Binary(
                op=c99_ast.Add(),
                left=c99_ast.Postfix(
                    op=c99_ast.Increment(),
                    operand=c99_ast.Var(name="a"),
                ),
                right=c99_ast.Var(name="b"),
            ),
        )

    def test_double_postfix_is_left_associative(self):
        # `a++--` parses as `(a++)--` — postfix_exp is left-recursive.
        # Semantically nonsense (a++ isn't an lvalue) but the grammar
        # accepts it.
        self.assertEqual(
            _exp_of("a++--"),
            c99_ast.Postfix(
                op=c99_ast.Decrement(),
                operand=c99_ast.Postfix(
                    op=c99_ast.Increment(),
                    operand=c99_ast.Var(name="a"),
                ),
            ),
        )

    def test_double_prefix_is_right_associative(self):
        # `++++a` is `++(++a)` — prefix is right-recursive.
        inner = c99_ast.Assignment(
            lval=c99_ast.Var(name="a"),
            rval=c99_ast.Binary(
                op=c99_ast.Add(),
                left=c99_ast.Var(name="a"),
                right=c99_ast.Constant(value=1),
            ),
        )
        self.assertEqual(
            _exp_of("++++a"),
            c99_ast.Assignment(
                lval=inner,
                rval=c99_ast.Binary(
                    op=c99_ast.Add(),
                    left=inner,
                    right=c99_ast.Constant(value=1),
                ),
            ),
        )


class TestConditional(unittest.TestCase):
    """Ternary `cond ? t : f`. Grammar: condition is `logical_or_exp`,
    true-clause is full `exp` (so assignments go in unparenthesised),
    false-clause is `conditional_exp` (right-associative, excludes
    assignments from the slot — so `1 ? 2 : a = 5` parses as
    `(1 ? 2 : a) = 5` via the outer assignment rule)."""

    def test_basic(self):
        self.assertEqual(
            _exp_of("1 ? 2 : 3"),
            c99_ast.Conditional(
                condition=c99_ast.Constant(value=1),
                true_clause=c99_ast.Constant(value=2),
                false_clause=c99_ast.Constant(value=3),
            ),
        )

    def test_assignment_binds_looser_than_ternary(self):
        # `a = 1 ? 2 : 3` parses as `a = (1 ? 2 : 3)`, not `(a = 1) ? 2 : 3`.
        self.assertEqual(
            _exp_of("a = 1 ? 2 : 3"),
            c99_ast.Assignment(
                lval=c99_ast.Var(name="a"),
                rval=c99_ast.Conditional(
                    condition=c99_ast.Constant(value=1),
                    true_clause=c99_ast.Constant(value=2),
                    false_clause=c99_ast.Constant(value=3),
                ),
            ),
        )

    def test_logical_or_binds_tighter_than_ternary_in_cond(self):
        # `a || b ? 2 : 3` parses as `(a || b) ? 2 : 3`.
        self.assertEqual(
            _exp_of("a || b ? 2 : 3"),
            c99_ast.Conditional(
                condition=c99_ast.Binary(
                    op=c99_ast.LogicalOr(),
                    left=c99_ast.Var(name="a"),
                    right=c99_ast.Var(name="b"),
                ),
                true_clause=c99_ast.Constant(value=2),
                false_clause=c99_ast.Constant(value=3),
            ),
        )

    def test_logical_or_binds_tighter_than_ternary_in_false_clause(self):
        # `1 ? 2 : 3 || 4` parses as `1 ? 2 : (3 || 4)`.
        self.assertEqual(
            _exp_of("1 ? 2 : 3 || 4"),
            c99_ast.Conditional(
                condition=c99_ast.Constant(value=1),
                true_clause=c99_ast.Constant(value=2),
                false_clause=c99_ast.Binary(
                    op=c99_ast.LogicalOr(),
                    left=c99_ast.Constant(value=3),
                    right=c99_ast.Constant(value=4),
                ),
            ),
        )

    def test_false_clause_excludes_assignment(self):
        # The false-clause slot is `conditional_exp`, which doesn't
        # include assignment. So `1 ? 2 : a = 5` can't parse as
        # `1 ? 2 : (a = 5)`; instead the outer assignment rule takes
        # the whole `1 ? 2 : a` as its LHS, giving `(1 ? 2 : a) = 5`.
        # (Semantic analysis rejects the conditional-as-lvalue later.)
        self.assertEqual(
            _exp_of("1 ? 2 : a = 5"),
            c99_ast.Assignment(
                lval=c99_ast.Conditional(
                    condition=c99_ast.Constant(value=1),
                    true_clause=c99_ast.Constant(value=2),
                    false_clause=c99_ast.Var(name="a"),
                ),
                rval=c99_ast.Constant(value=5),
            ),
        )

    def test_true_clause_is_full_expression(self):
        # The true-clause slot is `exp`, so an unparenthesised
        # assignment parses inside it: `x ? x = 1 : 2` is
        # `x ? (x = 1) : 2`.
        self.assertEqual(
            _exp_of("x ? x = 1 : 2"),
            c99_ast.Conditional(
                condition=c99_ast.Var(name="x"),
                true_clause=c99_ast.Assignment(
                    lval=c99_ast.Var(name="x"),
                    rval=c99_ast.Constant(value=1),
                ),
                false_clause=c99_ast.Constant(value=2),
            ),
        )

    def test_nested_ternary_in_true_clause(self):
        # `a ? b ? 1 : 2 : 3` parses as `a ? (b ? 1 : 2) : 3`. The
        # true-clause slot is `exp`, which reaches conditional_exp,
        # so an inner ternary lives there without parens.
        self.assertEqual(
            _exp_of("a ? b ? 1 : 2 : 3"),
            c99_ast.Conditional(
                condition=c99_ast.Var(name="a"),
                true_clause=c99_ast.Conditional(
                    condition=c99_ast.Var(name="b"),
                    true_clause=c99_ast.Constant(value=1),
                    false_clause=c99_ast.Constant(value=2),
                ),
                false_clause=c99_ast.Constant(value=3),
            ),
        )

    def test_ternary_is_right_associative(self):
        # `a ? 1 : b ? 2 : 3` parses as `a ? 1 : (b ? 2 : 3)` — the
        # false-clause slot is `conditional_exp`, so the rule is
        # right-recursive and ternary chains nest to the right.
        self.assertEqual(
            _exp_of("a ? 1 : b ? 2 : 3"),
            c99_ast.Conditional(
                condition=c99_ast.Var(name="a"),
                true_clause=c99_ast.Constant(value=1),
                false_clause=c99_ast.Conditional(
                    condition=c99_ast.Var(name="b"),
                    true_clause=c99_ast.Constant(value=2),
                    false_clause=c99_ast.Constant(value=3),
                ),
            ),
        )


class TestIfStatement(unittest.TestCase):
    """`if (exp) stmt` with an optional `else stmt`. Dangling else
    binds to the nearest preceding unmatched `if` (C99 §6.8.4.1) —
    Lark's LALR(1) backend resolves the shift-reduce conflict in
    favor of shifting, which gives that binding for free."""

    def _stmt_of(self, src):
        items = parse(f"int main(void) {{ {src} }}").function_definition.body.block_item
        assert len(items) == 1, items
        item = items[0]
        assert isinstance(item, c99_ast.S), item
        return item.statement

    def test_if_without_else(self):
        self.assertEqual(
            self._stmt_of("if (1) return 2;"),
            c99_ast.IfStmt(
                condition=c99_ast.Constant(value=1),
                then_clause=c99_ast.Return(exp=c99_ast.Constant(value=2)),
                else_clause=None,
            ),
        )

    def test_if_with_else(self):
        self.assertEqual(
            self._stmt_of("if (1) return 2; else return 3;"),
            c99_ast.IfStmt(
                condition=c99_ast.Constant(value=1),
                then_clause=c99_ast.Return(exp=c99_ast.Constant(value=2)),
                else_clause=c99_ast.Return(exp=c99_ast.Constant(value=3)),
            ),
        )

    def test_dangling_else_binds_to_inner_if(self):
        # `if (a) if (b) X; else Y;` — the `else Y` belongs to the
        # inner `if (b)`, not the outer `if (a)`. So the outer's
        # else_clause is None, and the inner has both branches.
        stmt = self._stmt_of(
            "if (1) if (2) return 3; else return 4;"
        )
        self.assertEqual(
            stmt,
            c99_ast.IfStmt(
                condition=c99_ast.Constant(value=1),
                then_clause=c99_ast.IfStmt(
                    condition=c99_ast.Constant(value=2),
                    then_clause=c99_ast.Return(
                        exp=c99_ast.Constant(value=3),
                    ),
                    else_clause=c99_ast.Return(
                        exp=c99_ast.Constant(value=4),
                    ),
                ),
                else_clause=None,
            ),
        )

    def test_if_with_compound_condition(self):
        # `if (a == 1)` exercises the condition slot accepting any
        # expression — here a Binary.
        stmt = self._stmt_of("if (1 == 2) return 3;")
        self.assertEqual(
            stmt.condition,
            c99_ast.Binary(
                op=c99_ast.Equal(),
                left=c99_ast.Constant(value=1),
                right=c99_ast.Constant(value=2),
            ),
        )

    def test_if_with_null_then_branch(self):
        # `if (1) ;` — the then-branch is a Null statement.
        self.assertEqual(
            self._stmt_of("if (1) ;"),
            c99_ast.IfStmt(
                condition=c99_ast.Constant(value=1),
                then_clause=c99_ast.Null(),
                else_clause=None,
            ),
        )


class TestCompoundStatement(unittest.TestCase):
    """`{ ... }` as a statement (C99 §6.8.3 compound statement). The
    grammar rule `statement: block -> compound_stmt` reuses the same
    `block` rule the function body uses; the only difference is the
    transformer wraps the resulting `Block` in a `Compound`."""

    def _stmt_of(self, src):
        items = parse(f"int main(void) {{ {src} }}").function_definition.body.block_item
        assert len(items) == 1, items
        item = items[0]
        assert isinstance(item, c99_ast.S), item
        return item.statement

    def test_empty_block_as_statement(self):
        # `{ }` — a Compound wrapping a Block with no items.
        self.assertEqual(
            self._stmt_of("{ }"),
            c99_ast.Compound(block=c99_ast.Block(block_item=[])),
        )

    def test_single_statement_block(self):
        # `{ return 0; }`.
        self.assertEqual(
            self._stmt_of("{ return 0; }"),
            c99_ast.Compound(block=c99_ast.Block(block_item=[
                c99_ast.S(statement=c99_ast.Return(
                    exp=c99_ast.Constant(value=0),
                )),
            ])),
        )

    def test_block_with_declaration_and_statement(self):
        # `{ int a = 1; return a; }` — both kinds of block_item.
        self.assertEqual(
            self._stmt_of("{ int a = 1; return a; }"),
            c99_ast.Compound(block=c99_ast.Block(block_item=[
                c99_ast.D(declaration=c99_ast.Declaration(
                    name="a", init=c99_ast.Constant(value=1),
                )),
                c99_ast.S(statement=c99_ast.Return(exp=c99_ast.Var(name="a"))),
            ])),
        )

    def test_nested_blocks(self):
        # `{ { ; } }` — outer Compound's block contains an inner
        # Compound, whose block contains a Null.
        self.assertEqual(
            self._stmt_of("{ { ; } }"),
            c99_ast.Compound(block=c99_ast.Block(block_item=[
                c99_ast.S(statement=c99_ast.Compound(
                    block=c99_ast.Block(block_item=[
                        c99_ast.S(statement=c99_ast.Null()),
                    ]),
                )),
            ])),
        )

    def test_block_as_if_branch(self):
        # `if (1) { return 2; }` — the then-clause is a Compound.
        self.assertEqual(
            self._stmt_of("if (1) { return 2; }"),
            c99_ast.IfStmt(
                condition=c99_ast.Constant(value=1),
                then_clause=c99_ast.Compound(block=c99_ast.Block(block_item=[
                    c99_ast.S(statement=c99_ast.Return(
                        exp=c99_ast.Constant(value=2),
                    )),
                ])),
                else_clause=None,
            ),
        )


class TestLabeledStmtAndGoto(unittest.TestCase):
    """C99 §6.8.1 labeled statements (`label: stmt`) and §6.8.6 `goto
    label;`. The grammar's labeled_stmt rule (`IDENTIFIER COLON
    statement`) introduces a shift-reduce conflict at statement-start
    on COLON lookahead — Lark's LALR(1) backend resolves it by
    shifting, picking the labeled-statement branch."""

    def _stmt_of(self, src):
        items = parse(f"int main(void) {{ {src} }}").function_definition.body.block_item
        assert len(items) == 1, items
        item = items[0]
        assert isinstance(item, c99_ast.S), item
        return item.statement

    def test_goto_basic(self):
        self.assertEqual(
            self._stmt_of("goto foo;"),
            c99_ast.Goto(label="foo"),
        )

    def test_labeled_statement_basic(self):
        # `foo: return 0;` — the labeled stmt's body is the Return.
        self.assertEqual(
            self._stmt_of("foo: return 0;"),
            c99_ast.LabeledStmt(
                label="foo",
                statement=c99_ast.Return(exp=c99_ast.Constant(value=0)),
            ),
        )

    def test_labeled_null_statement(self):
        # `foo: ;` — the body is a Null statement.
        self.assertEqual(
            self._stmt_of("foo: ;"),
            c99_ast.LabeledStmt(label="foo", statement=c99_ast.Null()),
        )

    def test_nested_labeled_statements(self):
        # `a: b: ;` — the outer label's body is the inner labeled stmt,
        # whose body is Null.
        self.assertEqual(
            self._stmt_of("a: b: ;"),
            c99_ast.LabeledStmt(
                label="a",
                statement=c99_ast.LabeledStmt(
                    label="b",
                    statement=c99_ast.Null(),
                ),
            ),
        )

    def test_label_inside_if_then(self):
        # Labels can appear inside an if-then or if-else (the branch
        # is a single statement, which can be a labeled statement).
        items = parse(
            "int main(void) { if (1) foo: return 0; }"
        ).function_definition.body.block_item
        self.assertEqual(
            items[0].statement,
            c99_ast.IfStmt(
                condition=c99_ast.Constant(value=1),
                then_clause=c99_ast.LabeledStmt(
                    label="foo",
                    statement=c99_ast.Return(exp=c99_ast.Constant(value=0)),
                ),
                else_clause=None,
            ),
        )

    def test_ternary_still_parses(self):
        # The labeled_stmt rule shouldn't disturb the ternary's COLON
        # — `a ? b : c` inside an expression context is still a
        # Conditional, not a goto-target. (LALR state at "after
        # IDENTIFIER inside a conditional_exp" doesn't include the
        # labeled_stmt option, so no conflict.)
        items = parse(
            "int main(void) { return a ? b : c; }"
        ).function_definition.body.block_item
        self.assertEqual(
            items[0].statement,
            c99_ast.Return(exp=c99_ast.Conditional(
                condition=c99_ast.Var(name="a"),
                true_clause=c99_ast.Var(name="b"),
                false_clause=c99_ast.Var(name="c"),
            )),
        )

    def test_goto_then_label_in_program(self):
        # End-to-end: `int main(void) { goto end; end: return 0; }`.
        prog = parse("int main(void) { goto end; end: return 0; }")
        items = prog.function_definition.body.block_item
        self.assertEqual(items[0].statement, c99_ast.Goto(label="end"))
        self.assertEqual(
            items[1].statement,
            c99_ast.LabeledStmt(
                label="end",
                statement=c99_ast.Return(exp=c99_ast.Constant(value=0)),
            ),
        )


class TestIterationStatements(unittest.TestCase):
    """C99 §6.8.5 iteration statements (`while`, `do-while`, `for`) and
    §6.8.6 jump statements (`break`, `continue`). All loop labels are
    minted by the loop_labeling pass; the parser leaves them as the
    empty string."""

    def _stmt_of(self, src):
        items = parse(f"int main(void) {{ {src} }}").function_definition.body.block_item
        assert len(items) == 1, items
        item = items[0]
        assert isinstance(item, c99_ast.S), item
        return item.statement

    def test_break(self):
        self.assertEqual(self._stmt_of("break;"), c99_ast.BreakStmt(label=""))

    def test_continue(self):
        self.assertEqual(
            self._stmt_of("continue;"), c99_ast.ContinueStmt(label=""),
        )

    def test_while_loop(self):
        self.assertEqual(
            self._stmt_of("while (1) break;"),
            c99_ast.WhileStmt(
                condition=c99_ast.Constant(value=1),
                body=c99_ast.BreakStmt(label=""),
                label="",
            ),
        )

    def test_do_while_loop(self):
        self.assertEqual(
            self._stmt_of("do continue; while (0);"),
            c99_ast.DoWhileStmt(
                body=c99_ast.ContinueStmt(label=""),
                condition=c99_ast.Constant(value=0),
                label="",
            ),
        )

    def test_for_loop_full(self):
        # `for (int i = 0; i < 10; i++) ;` — all three header slots
        # populated; init is a declaration.
        self.assertEqual(
            self._stmt_of("for (int i = 0; i < 10; i++) ;"),
            c99_ast.ForStmt(
                init=c99_ast.InitDecl(declaration=c99_ast.Declaration(
                    name="i", init=c99_ast.Constant(value=0),
                )),
                condition=c99_ast.Binary(
                    op=c99_ast.LessThan(),
                    left=c99_ast.Var(name="i"),
                    right=c99_ast.Constant(value=10),
                ),
                post_clause=c99_ast.Postfix(
                    op=c99_ast.Increment(),
                    operand=c99_ast.Var(name="i"),
                ),
                body=c99_ast.Null(),
                label="",
            ),
        )

    def test_for_loop_with_init_exp(self):
        # `for (i = 0; ...; ...)` — init is an expression, not a decl.
        items = parse(
            "int main(void) { int i; for (i = 0; i < 5; i++) break; }"
        ).function_definition.body.block_item
        for_stmt = items[1].statement
        self.assertIsInstance(for_stmt, c99_ast.ForStmt)
        self.assertEqual(
            for_stmt.init,
            c99_ast.InitExp(exp=c99_ast.Assignment(
                lval=c99_ast.Var(name="i"),
                rval=c99_ast.Constant(value=0),
            )),
        )

    def test_for_loop_empty_header(self):
        # `for (;;) break;` — all three slots empty; init is InitExp(None).
        self.assertEqual(
            self._stmt_of("for (;;) break;"),
            c99_ast.ForStmt(
                init=c99_ast.InitExp(exp=None),
                condition=None,
                post_clause=None,
                body=c99_ast.BreakStmt(label=""),
                label="",
            ),
        )

    def test_for_loop_condition_only(self):
        # `for (; cond;) ...` — only the condition slot is populated.
        self.assertEqual(
            self._stmt_of("for (; 1;) break;"),
            c99_ast.ForStmt(
                init=c99_ast.InitExp(exp=None),
                condition=c99_ast.Constant(value=1),
                post_clause=None,
                body=c99_ast.BreakStmt(label=""),
                label="",
            ),
        )

    def test_for_loop_post_only(self):
        # `for (;; post) ...` — only the post-iteration slot is populated.
        items = parse(
            "int main(void) { int i; for (;; i++) break; }"
        ).function_definition.body.block_item
        for_stmt = items[1].statement
        self.assertEqual(for_stmt.condition, None)
        self.assertEqual(
            for_stmt.post_clause,
            c99_ast.Postfix(
                op=c99_ast.Increment(),
                operand=c99_ast.Var(name="i"),
            ),
        )

    def test_for_loop_with_compound_body(self):
        # The body of a for-loop can be a compound statement.
        stmt = self._stmt_of("for (;;) { break; }")
        self.assertIsInstance(stmt, c99_ast.ForStmt)
        self.assertEqual(
            stmt.body,
            c99_ast.Compound(block=c99_ast.Block(block_item=[
                c99_ast.S(statement=c99_ast.BreakStmt(label="")),
            ])),
        )

    def test_nested_loops(self):
        stmt = self._stmt_of("while (1) for (;;) break;")
        self.assertIsInstance(stmt, c99_ast.WhileStmt)
        self.assertIsInstance(stmt.body, c99_ast.ForStmt)
        self.assertEqual(stmt.body.body, c99_ast.BreakStmt(label=""))


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestValidFiles(unittest.TestCase):
    """Each file in tests/valid/ must parse into an AST for `int main(void)`
    with a Return of an integer Constant. Most files have comments, so we
    pipe through pcpp first."""

    def test_each_valid_file_parses(self):
        paths = sorted((_TESTS_DIR / "valid").glob("*.c"))
        self.assertGreater(len(paths), 0, "no valid/*.c files")
        for path in paths:
            with self.subTest(file=path.name):
                ast = parse(_preprocess(path.read_text()))
                self.assertIsInstance(ast, c99_ast.Program)
                self.assertEqual(ast.function_definition.name, "main")
                stmt = _return_stmt(ast)
                self.assertIsInstance(stmt.exp, c99_ast.Constant)


@unittest.skipUnless(shutil.which("pcpp"), "pcpp not available on PATH")
class TestInvalidParseFiles(unittest.TestCase):
    """Each file in tests/invalid_parse/ must fail parsing (either at lex
    time or parse time)."""

    def test_each_invalid_parse_file_fails(self):
        paths = sorted((_TESTS_DIR / "invalid_parse").glob("*.c"))
        self.assertGreater(len(paths), 0, "no invalid_parse/*.c files")
        for path in paths:
            with self.subTest(file=path.name):
                src = _preprocess(path.read_text())
                with self.assertRaises((LexError, UnexpectedInput)):
                    parse(src)


if __name__ == "__main__":
    unittest.main()
