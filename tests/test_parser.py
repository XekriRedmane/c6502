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
                body=[c99_ast.S(statement=c99_ast.Return(
                    exp=c99_ast.Constant(value=42),
                ))],
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
        # body is a list of block_items now.
        self.assertIsInstance(ast.function_definition.body, list)
        self.assertEqual(len(ast.function_definition.body), 1)
        item = ast.function_definition.body[0]
        self.assertIsInstance(item, c99_ast.Type_block_item)
        self.assertIsInstance(item, c99_ast.S)
        self.assertIsInstance(item.statement, c99_ast.Type_statement)
        self.assertIsInstance(item.statement, c99_ast.Return)
        self.assertIsInstance(item.statement.exp, c99_ast.Type_exp)
        self.assertIsInstance(item.statement.exp, c99_ast.Constant)


def _return_stmt(ast: c99_ast.Type_program) -> c99_ast.Return:
    """Extract the single Return statement from
    `int main(void) { return <exp>; }`. Function.body is now a list
    of block_items; here we expect exactly one S(statement=Return)."""
    body = ast.function_definition.body
    assert len(body) == 1, body
    item = body[0]
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
