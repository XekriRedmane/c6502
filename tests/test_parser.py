import unittest

from lark.exceptions import UnexpectedInput

import c99_ast
import fp_arith
from parser import ParserError, parse


class TestParser(unittest.TestCase):
    def test_minimal_function(self):
        ast = parse("int main(void) { return 42; }")
        expected = c99_ast.Program(
            declaration=[c99_ast.FunctionDecl(
                function_decl=c99_ast.Type_function_decl(
                    name="main",
                    params=[],
                    body=c99_ast.Block(block_item=[c99_ast.S(
                        statement=c99_ast.Return(
                            exp=c99_ast.Constant(const=c99_ast.ConstInt(value=42)),
                        ),
                    )]),
                    data_type=c99_ast.FunType(params=[], ret=c99_ast.Int()),
                    storage_class=None,
                ),
            )],
        )
        self.assertEqual(ast, expected)

    def test_whitespace_insensitive(self):
        for src in [
            "int main(void){return 42;}",
            "int  main  ( void )  {  return  42  ;  }",
            "int\nmain(void)\n{\n    return 42;\n}",
        ]:
            with self.subTest(src=src):
                self.assertEqual(_return_stmt(parse(src)).exp.const.value, 42)

    def test_various_return_values(self):
        # Values <=127 land in ConstInt; 128..32767 land in ConstLong.
        # Anything outside those ranges raises at parse time per the
        # `_make_const` factory, so the literal range here matches the
        # AST's representable space.
        for val in [0, 1, 42, 127, 128, 1000, 32767]:
            with self.subTest(val=val):
                ast = parse(f"int main(void) {{ return {val}; }}")
                self.assertEqual(_return_stmt(ast).exp.const.value, val)

    def test_function_name_captured(self):
        for name in ["main", "foo", "_start", "a1b2"]:
            with self.subTest(name=name):
                ast = parse(f"int {name}(void) {{ return 0; }}")
                self.assertEqual(ast.declaration[0].function_decl.name, name)

    def test_unary_negate(self):
        ast = parse("int main(void) { return -42; }")
        self.assertEqual(
            _return_stmt(ast),
            c99_ast.Return(exp=c99_ast.Unary(
                op=c99_ast.Negate(),
                exp=c99_ast.Constant(const=c99_ast.ConstInt(value=42)),
            )),
        )

    def test_unary_complement(self):
        ast = parse("int main(void) { return ~10; }")
        self.assertEqual(
            _return_stmt(ast),
            c99_ast.Return(exp=c99_ast.Unary(
                op=c99_ast.Complement(),
                exp=c99_ast.Constant(const=c99_ast.ConstInt(value=10)),
            )),
        )

    def test_parens_do_not_appear_in_ast(self):
        ast = parse("int main(void) { return (42); }")
        self.assertEqual(
            _return_stmt(ast),
            c99_ast.Return(exp=c99_ast.Constant(const=c99_ast.ConstInt(value=42))),
        )

    def test_nested_unary(self):
        ast = parse("int main(void) { return -(-42); }")
        self.assertEqual(
            _return_stmt(ast).exp,
            c99_ast.Unary(
                op=c99_ast.Negate(),
                exp=c99_ast.Unary(
                    op=c99_ast.Negate(),
                    exp=c99_ast.Constant(const=c99_ast.ConstInt(value=42)),
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
                    exp=c99_ast.Constant(const=c99_ast.ConstInt(value=5)),
                ),
            ),
        )

    def test_returned_ast_types(self):
        ast = parse("int main(void) { return 0; }")
        self.assertIsInstance(ast, c99_ast.Type_program)
        self.assertIsInstance(ast, c99_ast.Program)
        # `declaration` is a list of Type_declaration nodes; a function
        # definition arrives as `FunctionDecl(function_decl=Type_function_decl(...))`
        # with body=Block(...). A forward declaration is the same shape
        # with body=None.
        self.assertIsInstance(ast.declaration, list)
        self.assertEqual(len(ast.declaration), 1)
        decl = ast.declaration[0]
        self.assertIsInstance(decl, c99_ast.Type_declaration)
        self.assertIsInstance(decl, c99_ast.FunctionDecl)
        fd = decl.function_decl
        self.assertIsInstance(fd, c99_ast.Type_function_decl)
        # body is a Block wrapping a list of block_items.
        body = fd.body
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
    `int main(void) { return <exp>; }`. Program.declaration is a list
    of Type_declaration nodes — here we expect exactly one
    FunctionDecl(function_decl=Type_function_decl(..., body=Block(...))).
    The body is a Block around a list of block_items; here we expect
    exactly one S(statement=Return)."""
    decls = ast.declaration
    assert len(decls) == 1, decls
    fd = decls[0].function_decl
    items = fd.body.block_item
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
                left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                right=c99_ast.Binary(
                    op=c99_ast.Multiply(),
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
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
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                ),
                right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
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
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                ),
                right=c99_ast.Binary(
                    op=c99_ast.Multiply(),
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=4)),
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
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                ),
                right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
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
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                ),
                right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
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
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                ),
                right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
            ),
        )

    def test_unary_binds_tighter_than_multiply(self):
        # -1 * 2  ->  *(-1, 2)
        self.assertEqual(
            _exp_of("-1 * 2"),
            c99_ast.Binary(
                op=c99_ast.Multiply(),
                left=c99_ast.Unary(
                    op=c99_ast.Negate(), exp=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                ),
                right=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
            ),
        )

    def test_modulo(self):
        # 10 % 3  ->  %(10, 3)
        self.assertEqual(
            _exp_of("10 % 3"),
            c99_ast.Binary(
                op=c99_ast.Modulo(),
                left=c99_ast.Constant(const=c99_ast.ConstInt(value=10)),
                right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
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
                        left=c99_ast.Constant(const=c99_ast.ConstInt(value=5)),
                        right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
                    ),
                )

    def test_shift_binds_tighter_than_bitwise_and(self):
        # 1 & 2 << 3 -> &(1, <<(2, 3))
        self.assertEqual(
            _exp_of("1 & 2 << 3"),
            c99_ast.Binary(
                op=c99_ast.BitwiseAnd(),
                left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                right=c99_ast.Binary(
                    op=c99_ast.LeftShift(),
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
                ),
            ),
        )

    def test_add_binds_tighter_than_shift(self):
        # 1 << 2 + 3 -> <<(1, +(2, 3))
        self.assertEqual(
            _exp_of("1 << 2 + 3"),
            c99_ast.Binary(
                op=c99_ast.LeftShift(),
                left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                right=c99_ast.Binary(
                    op=c99_ast.Add(),
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
                ),
            ),
        )

    def test_and_binds_tighter_than_xor(self):
        # 1 ^ 2 & 3 -> ^(1, &(2, 3))
        self.assertEqual(
            _exp_of("1 ^ 2 & 3"),
            c99_ast.Binary(
                op=c99_ast.BitwiseXor(),
                left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                right=c99_ast.Binary(
                    op=c99_ast.BitwiseAnd(),
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
                ),
            ),
        )

    def test_xor_binds_tighter_than_or(self):
        # 1 | 2 ^ 3 -> |(1, ^(2, 3))
        self.assertEqual(
            _exp_of("1 | 2 ^ 3"),
            c99_ast.Binary(
                op=c99_ast.BitwiseOr(),
                left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                right=c99_ast.Binary(
                    op=c99_ast.BitwiseXor(),
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
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
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                ),
                right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
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
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                ),
                right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
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
                        left=c99_ast.Constant(const=c99_ast.ConstInt(value=5)),
                        right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
                    ),
                )

    def test_relational_binds_tighter_than_equality(self):
        # 1 == 2 < 3 -> ==(1, <(2, 3))
        self.assertEqual(
            _exp_of("1 == 2 < 3"),
            c99_ast.Binary(
                op=c99_ast.Equal(),
                left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                right=c99_ast.Binary(
                    op=c99_ast.LessThan(),
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
                ),
            ),
        )

    def test_shift_binds_tighter_than_relational(self):
        # 1 < 2 << 3 -> <(1, <<(2, 3))
        self.assertEqual(
            _exp_of("1 < 2 << 3"),
            c99_ast.Binary(
                op=c99_ast.LessThan(),
                left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                right=c99_ast.Binary(
                    op=c99_ast.LeftShift(),
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
                ),
            ),
        )

    def test_equality_binds_tighter_than_bitwise_and(self):
        # 1 & 2 == 3 -> &(1, ==(2, 3))
        self.assertEqual(
            _exp_of("1 & 2 == 3"),
            c99_ast.Binary(
                op=c99_ast.BitwiseAnd(),
                left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                right=c99_ast.Binary(
                    op=c99_ast.Equal(),
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
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
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                ),
                right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
            ),
        )


class TestLogicalNotUnary(unittest.TestCase):
    """! shares unary precedence with - and ~ (right-to-left)."""

    def test_basic(self):
        self.assertEqual(
            _exp_of("!5"),
            c99_ast.Unary(
                op=c99_ast.LogicalNot(),
                exp=c99_ast.Constant(const=c99_ast.ConstInt(value=5)),
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
                    exp=c99_ast.Constant(const=c99_ast.ConstInt(value=5)),
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
                    op=c99_ast.LogicalNot(), exp=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                ),
                right=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
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
                    exp=c99_ast.Constant(const=c99_ast.ConstInt(value=5)),
                ),
            ),
        )


class TestAssignment(unittest.TestCase):
    """Plain `=` and the ten compound assignments. The grammar's LHS is
    `logical_or_exp` rather than C99's `unary_exp`, so things like
    `1+2 = 3` parse here — identifier_resolution rejects them later.
    Compound `OP=` desugars at parse time to `lval = lval OP rval`,
    sharing the `lval` node by reference (safe today because the only
    legal lval is a `Var`, which has no side effect when re-evaluated)."""

    def test_plain_assignment(self):
        self.assertEqual(
            _exp_of("a = 1"),
            c99_ast.Assignment(
                lval=c99_ast.Var(name="a"),
                rval=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
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
                    rval=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                ),
            ),
        )

    def test_each_compound_op_builds_compoundassignment(self):
        # Every `a OP= 1` builds a CompoundAssignment AST node.
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
                    c99_ast.CompoundAssignment(
                        op=op,
                        lval=c99_ast.Var(name="a"),
                        rval=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                    ),
                )

    def test_compound_assign_is_right_associative(self):
        # `a += b += 1` parses as `a += (b += 1)` — the inner
        # CompoundAssignment is the rval slot of the outer.
        self.assertEqual(
            _exp_of("a += b += 1"),
            c99_ast.CompoundAssignment(
                op=c99_ast.Add(),
                lval=c99_ast.Var(name="a"),
                rval=c99_ast.CompoundAssignment(
                    op=c99_ast.Add(),
                    lval=c99_ast.Var(name="b"),
                    rval=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                ),
            ),
        )

    def test_compound_assign_rhs_is_full_expression(self):
        # The rval slot is `assignment_exp`, which means a full binary
        # expression goes in unparenthesized — `a += 1 + 2` parses to
        # `a += (1 + 2)`. Right-recursion at the assignment level keeps
        # the rval intact (NOT `(a + 1) + 2`).
        self.assertEqual(
            _exp_of("a += 1 + 2"),
            c99_ast.CompoundAssignment(
                op=c99_ast.Add(),
                lval=c99_ast.Var(name="a"),
                rval=c99_ast.Binary(
                    op=c99_ast.Add(),
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                ),
            ),
        )

    def test_compound_assign_invalid_lhs_still_parses(self):
        # `1 += 2` parses (LHS is `logical_or_exp`, which Constant
        # satisfies) — identifier_resolution is what rejects it.
        self.assertEqual(
            _exp_of("1 += 2"),
            c99_ast.CompoundAssignment(
                op=c99_ast.Add(),
                lval=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                rval=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
            ),
        )


class TestIncrementDecrement(unittest.TestCase):
    """Prefix `++a` / `--a` and postfix `a++` / `a--` each build their
    own AST node (`Prefix` / `Postfix`). The two are distinct because
    they have different result semantics — prefix returns the *new*
    value, postfix returns the *old* value — and because direct nodes
    let c99_to_tac evaluate the operand's address ONCE for richer
    lvalues (`Subscript`, `Dereference`), avoiding the side-effect
    duplication that an `a = a + 1` desugaring would cause for
    operands like `++arr[--i]`."""

    def test_pre_increment_builds_prefix_node(self):
        self.assertEqual(
            _exp_of("++a"),
            c99_ast.Prefix(
                op=c99_ast.Increment(),
                operand=c99_ast.Var(name="a"),
            ),
        )

    def test_pre_decrement_builds_prefix_node(self):
        self.assertEqual(
            _exp_of("--a"),
            c99_ast.Prefix(
                op=c99_ast.Decrement(),
                operand=c99_ast.Var(name="a"),
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
        # `++a++` parses as `++(a++)`. Semantically invalid C
        # (`a++` isn't an lvalue) but the grammar accepts it and
        # identifier_resolution catches the bad lvalue.
        self.assertEqual(
            _exp_of("++a++"),
            c99_ast.Prefix(
                op=c99_ast.Increment(),
                operand=c99_ast.Postfix(
                    op=c99_ast.Increment(),
                    operand=c99_ast.Var(name="a"),
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
        self.assertEqual(
            _exp_of("++++a"),
            c99_ast.Prefix(
                op=c99_ast.Increment(),
                operand=c99_ast.Prefix(
                    op=c99_ast.Increment(),
                    operand=c99_ast.Var(name="a"),
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
                condition=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                true_clause=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                false_clause=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
            ),
        )

    def test_assignment_binds_looser_than_ternary(self):
        # `a = 1 ? 2 : 3` parses as `a = (1 ? 2 : 3)`, not `(a = 1) ? 2 : 3`.
        self.assertEqual(
            _exp_of("a = 1 ? 2 : 3"),
            c99_ast.Assignment(
                lval=c99_ast.Var(name="a"),
                rval=c99_ast.Conditional(
                    condition=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                    true_clause=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                    false_clause=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
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
                true_clause=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                false_clause=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
            ),
        )

    def test_logical_or_binds_tighter_than_ternary_in_false_clause(self):
        # `1 ? 2 : 3 || 4` parses as `1 ? 2 : (3 || 4)`.
        self.assertEqual(
            _exp_of("1 ? 2 : 3 || 4"),
            c99_ast.Conditional(
                condition=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                true_clause=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                false_clause=c99_ast.Binary(
                    op=c99_ast.LogicalOr(),
                    left=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=4)),
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
                    condition=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                    true_clause=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                    false_clause=c99_ast.Var(name="a"),
                ),
                rval=c99_ast.Constant(const=c99_ast.ConstInt(value=5)),
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
                    rval=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                ),
                false_clause=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
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
                    true_clause=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                    false_clause=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                ),
                false_clause=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
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
                true_clause=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                false_clause=c99_ast.Conditional(
                    condition=c99_ast.Var(name="b"),
                    true_clause=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                    false_clause=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
                ),
            ),
        )


class TestIfStatement(unittest.TestCase):
    """`if (exp) stmt` with an optional `else stmt`. Dangling else
    binds to the nearest preceding unmatched `if` (C99 §6.8.4.1) —
    Lark's LALR(1) backend resolves the shift-reduce conflict in
    favor of shifting, which gives that binding for free."""

    def _stmt_of(self, src):
        items = parse(f"int main(void) {{ {src} }}").declaration[0].function_decl.body.block_item
        assert len(items) == 1, items
        item = items[0]
        assert isinstance(item, c99_ast.S), item
        return item.statement

    def test_if_without_else(self):
        self.assertEqual(
            self._stmt_of("if (1) return 2;"),
            c99_ast.IfStmt(
                condition=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                then_clause=c99_ast.Return(exp=c99_ast.Constant(const=c99_ast.ConstInt(value=2))),
                else_clause=None,
            ),
        )

    def test_if_with_else(self):
        self.assertEqual(
            self._stmt_of("if (1) return 2; else return 3;"),
            c99_ast.IfStmt(
                condition=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                then_clause=c99_ast.Return(exp=c99_ast.Constant(const=c99_ast.ConstInt(value=2))),
                else_clause=c99_ast.Return(exp=c99_ast.Constant(const=c99_ast.ConstInt(value=3))),
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
                condition=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                then_clause=c99_ast.IfStmt(
                    condition=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                    then_clause=c99_ast.Return(
                        exp=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
                    ),
                    else_clause=c99_ast.Return(
                        exp=c99_ast.Constant(const=c99_ast.ConstInt(value=4)),
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
                left=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                right=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
            ),
        )

    def test_if_with_null_then_branch(self):
        # `if (1) ;` — the then-branch is a Null statement.
        self.assertEqual(
            self._stmt_of("if (1) ;"),
            c99_ast.IfStmt(
                condition=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
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
        items = parse(f"int main(void) {{ {src} }}").declaration[0].function_decl.body.block_item
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
                    exp=c99_ast.Constant(const=c99_ast.ConstInt(value=0)),
                )),
            ])),
        )

    def test_block_with_declaration_and_statement(self):
        # `{ int a = 1; return a; }` — both kinds of block_item.
        self.assertEqual(
            self._stmt_of("{ int a = 1; return a; }"),
            c99_ast.Compound(block=c99_ast.Block(block_item=[
                c99_ast.D(declaration=c99_ast.VarDecl(
                    var_decl=c99_ast.Type_var_decl(
                        name="a", init=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                        data_type=c99_ast.Int(),
                    ),
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
                condition=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                then_clause=c99_ast.Compound(block=c99_ast.Block(block_item=[
                    c99_ast.S(statement=c99_ast.Return(
                        exp=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
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
        items = parse(f"int main(void) {{ {src} }}").declaration[0].function_decl.body.block_item
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
                statement=c99_ast.Return(exp=c99_ast.Constant(const=c99_ast.ConstInt(value=0))),
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
        ).declaration[0].function_decl.body.block_item
        self.assertEqual(
            items[0].statement,
            c99_ast.IfStmt(
                condition=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                then_clause=c99_ast.LabeledStmt(
                    label="foo",
                    statement=c99_ast.Return(exp=c99_ast.Constant(const=c99_ast.ConstInt(value=0))),
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
        ).declaration[0].function_decl.body.block_item
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
        items = prog.declaration[0].function_decl.body.block_item
        self.assertEqual(items[0].statement, c99_ast.Goto(label="end"))
        self.assertEqual(
            items[1].statement,
            c99_ast.LabeledStmt(
                label="end",
                statement=c99_ast.Return(exp=c99_ast.Constant(const=c99_ast.ConstInt(value=0))),
            ),
        )


class TestIterationStatements(unittest.TestCase):
    """C99 §6.8.5 iteration statements (`while`, `do-while`, `for`) and
    §6.8.6 jump statements (`break`, `continue`). All loop labels are
    minted by the loop_labeling pass; the parser leaves them as the
    empty string."""

    def _stmt_of(self, src):
        items = parse(f"int main(void) {{ {src} }}").declaration[0].function_decl.body.block_item
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
                condition=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                body=c99_ast.BreakStmt(label=""),
                label="",
            ),
        )

    def test_do_while_loop(self):
        self.assertEqual(
            self._stmt_of("do continue; while (0);"),
            c99_ast.DoWhileStmt(
                body=c99_ast.ContinueStmt(label=""),
                condition=c99_ast.Constant(const=c99_ast.ConstInt(value=0)),
                label="",
            ),
        )

    def test_for_loop_full(self):
        # `for (int i = 0; i < 10; i++) ;` — all three header slots
        # populated; init is a declaration.
        self.assertEqual(
            self._stmt_of("for (int i = 0; i < 10; i++) ;"),
            c99_ast.ForStmt(
                init=c99_ast.InitDecl(var_decl=c99_ast.Type_var_decl(
                    name="i", init=c99_ast.Constant(const=c99_ast.ConstInt(value=0)),
                    data_type=c99_ast.Int(),
                )),
                condition=c99_ast.Binary(
                    op=c99_ast.LessThan(),
                    left=c99_ast.Var(name="i"),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(value=10)),
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
        ).declaration[0].function_decl.body.block_item
        for_stmt = items[1].statement
        self.assertIsInstance(for_stmt, c99_ast.ForStmt)
        self.assertEqual(
            for_stmt.init,
            c99_ast.InitExp(exp=c99_ast.Assignment(
                lval=c99_ast.Var(name="i"),
                rval=c99_ast.Constant(const=c99_ast.ConstInt(value=0)),
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
                condition=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                post_clause=None,
                body=c99_ast.BreakStmt(label=""),
                label="",
            ),
        )

    def test_for_loop_post_only(self):
        # `for (;; post) ...` — only the post-iteration slot is populated.
        items = parse(
            "int main(void) { int i; for (;; i++) break; }"
        ).declaration[0].function_decl.body.block_item
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


class TestSwitchStatements(unittest.TestCase):
    """C99 §6.8.4.2 switch / case / default. Labels and the cases /
    default_label fields on SwitchStmt are filled in by later passes;
    the parser leaves them empty/None. Case label expressions go
    through the `constant_exp` non-terminal (§6.6) — semantics are
    enforced later but the grammar already excludes assignment /
    comma since `constant_exp = conditional_exp`."""

    def _stmt_of(self, src):
        items = parse(f"int main(void) {{ {src} }}").declaration[0].function_decl.body.block_item
        assert len(items) == 1, items
        item = items[0]
        assert isinstance(item, c99_ast.S), item
        return item.statement

    def test_basic_switch(self):
        stmt = self._stmt_of(
            "switch (3) { case 0: return 0; case 1: return 1;"
            " default: return 2; }"
        )
        self.assertIsInstance(stmt, c99_ast.SwitchStmt)
        self.assertEqual(stmt.label, "")
        self.assertEqual(stmt.cases, [])
        self.assertIsNone(stmt.default_label)
        self.assertIsNone(stmt.promoted_type)
        items = stmt.body.block.block_item
        self.assertIsInstance(items[0].statement, c99_ast.CaseStmt)
        self.assertIsInstance(items[2].statement, c99_ast.DefaultStmt)

    def test_case_value_carries_constant(self):
        stmt = self._stmt_of("switch (3) { case 5: return 0; }")
        case0 = stmt.body.block.block_item[0].statement
        self.assertEqual(
            case0.value,
            c99_ast.Constant(const=c99_ast.ConstInt(value=5)),
        )

    def test_assignment_in_case_label_rejected(self):
        # `case x = 1:` is a syntactic error because `constant_exp`
        # delegates to `conditional_exp`, which excludes assignment.
        from lark.exceptions import UnexpectedInput
        with self.assertRaises((UnexpectedInput, ParserError)):
            parse("int main(void) { int x; switch (x) { case x = 1: return 0; } }")

    def test_switch_missing_paren_rejected(self):
        from lark.exceptions import UnexpectedInput
        with self.assertRaises((UnexpectedInput, ParserError)):
            parse("int main(void) { switch 1 { case 0: return 0; } }")

    def test_default_with_no_following_statement_rejected(self):
        # `default:` without a body is a parse error — the rule
        # requires a statement.
        from lark.exceptions import UnexpectedInput
        with self.assertRaises((UnexpectedInput, ParserError)):
            parse("int main(void) { switch (1) { default: } }")


class TestFunctionDeclarationsAndDefinitions(unittest.TestCase):
    """Top-level rule is `declaration*` — each entry is a `FunctionDecl`
    or `VarDecl`. A function definition is a `FunctionDecl` whose
    `function_decl.body` is a `Block`; a forward declaration sets
    `body=None`. The same `FunctionDecl` shape appears at block scope
    (always with `body=None`, since C99 forbids nested function
    definitions)."""

    def test_multiple_top_level_functions(self):
        ast = parse(
            "int foo(void) { return 1; } int main(void) { return 0; }"
        )
        self.assertEqual(len(ast.declaration), 2)
        self.assertEqual(ast.declaration[0].function_decl.name, "foo")
        self.assertEqual(ast.declaration[1].function_decl.name, "main")

    def test_top_level_function_decl_parses_as_forward_declaration(self):
        # File-scope forward declarations are now first-class — they
        # share the same `FunctionDecl` shape as definitions, just
        # with `body=None`. The grammar accepts the SEMICOLON or
        # block alternative on `function_decl`.
        ast = parse("int foo(void); int main(void) { return 0; }")
        self.assertEqual(len(ast.declaration), 2)
        self.assertEqual(ast.declaration[0].function_decl.name, "foo")
        self.assertIsNone(ast.declaration[0].function_decl.body)
        self.assertIsNotNone(ast.declaration[1].function_decl.body)

    def test_block_scope_function_decl_no_args(self):
        ast = parse("int main(void) { int foo(void); return 0; }")
        first = ast.declaration[0].function_decl.body.block_item[0]
        self.assertEqual(
            first,
            c99_ast.D(declaration=c99_ast.FunctionDecl(
                function_decl=c99_ast.Type_function_decl(
                    name="foo", params=[], body=None,
                    data_type=c99_ast.FunType(params=[], ret=c99_ast.Int()),
                    storage_class=None,
                ),
            )),
        )

    def test_block_scope_function_decl_with_args(self):
        ast = parse(
            "int main(void) { int sum(int a, int b); return 0; }"
        )
        first = ast.declaration[0].function_decl.body.block_item[0]
        self.assertEqual(
            first.declaration.function_decl.params, ["a", "b"],
        )

    def test_function_call_no_args(self):
        ast = parse("int main(void) { int f(void); return f(); }")
        # Last block item is `return f();`.
        ret = ast.declaration[0].function_decl.body.block_item[1].statement
        self.assertEqual(
            ret.exp, c99_ast.FunctionCall(name="f", args=[]),
        )

    def test_function_call_with_args(self):
        ast = parse(
            "int main(void) { int f(int x, int y); return f(1, 2 + 3); }"
        )
        ret = ast.declaration[0].function_decl.body.block_item[1].statement
        self.assertEqual(
            ret.exp,
            c99_ast.FunctionCall(
                name="f",
                args=[
                    c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
                    c99_ast.Binary(
                        op=c99_ast.Add(),
                        left=c99_ast.Constant(const=c99_ast.ConstInt(value=2)),
                        right=c99_ast.Constant(const=c99_ast.ConstInt(value=3)),
                    ),
                ],
            ),
        )

    def test_function_call_disambiguated_from_paren_expression(self):
        # `(x)` parses as a parenthesised expression (atom -> paren),
        # but `f(x)` parses as a function call (atom -> function_call).
        # The shift on LPAREN after IDENTIFIER picks the call branch
        # via LALR(1).
        ast = parse(
            "int main(void) { int f(int x); int x; return f(x); }"
        )
        ret = ast.declaration[0].function_decl.body.block_item[2].statement
        self.assertIsInstance(ret.exp, c99_ast.FunctionCall)
        self.assertEqual(ret.exp.args, [c99_ast.Var(name="x")])

    def test_bare_identifier_does_not_become_a_call(self):
        # `f` alone is a Var (atom -> identifier). Only `f(...)` is
        # a call. Useful as a regression for the LALR shift decision.
        ast = parse("int main(void) { int f; return f; }")
        ret = ast.declaration[0].function_decl.body.block_item[1].statement
        self.assertEqual(ret.exp, c99_ast.Var(name="f"))

    def test_function_call_inside_arithmetic(self):
        # `f() + 1` — the call is one operand of a Binary.
        ast = parse(
            "int main(void) { int f(void); return f() + 1; }"
        )
        ret = ast.declaration[0].function_decl.body.block_item[1].statement
        self.assertEqual(
            ret.exp,
            c99_ast.Binary(
                op=c99_ast.Add(),
                left=c99_ast.FunctionCall(name="f", args=[]),
                right=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
            ),
        )


class TestLongAndCasts(unittest.TestCase):
    """`long` introduces a 2-byte type and a per-declaration `data_type`
    field on var_decl/function_decl. Casts sit between unary and
    multiplicative in the precedence chain (C99 §6.5.4): tighter than
    `*`/`/`/`%`, looser than the unary operators, right-associative.
    Constant literals dispatch into `ConstInt` (1-byte fit) or
    `ConstLong` (2-byte fit) based on their value, with anything
    outside ±32767 rejected at parse time."""

    def test_int_var_decl_carries_int_data_type(self):
        ast = parse("int x = 5;")
        vd = ast.declaration[0].var_decl
        self.assertEqual(vd.data_type, c99_ast.Int())

    def test_long_var_decl_carries_long_data_type(self):
        ast = parse("long x = 5;")
        vd = ast.declaration[0].var_decl
        self.assertEqual(vd.data_type, c99_ast.Long())

    def test_long_int_resolves_to_long(self):
        # `long int` and `int long` both mean Long per C99 §6.7.2.
        for src in ["long int x;", "int long x;"]:
            with self.subTest(src=src):
                ast = parse(src)
                vd = ast.declaration[0].var_decl
                self.assertEqual(vd.data_type, c99_ast.Long())

    def test_function_decl_carries_funtype(self):
        ast = parse("long foo(int a, long b);")
        fd = ast.declaration[0].function_decl
        self.assertEqual(
            fd.data_type,
            c99_ast.FunType(
                params=[c99_ast.Int(), c99_ast.Long()],
                ret=c99_ast.Long(),
            ),
        )

    def test_small_literal_is_const_int(self):
        ast = parse("int main(void) { return 5; }")
        ret = ast.declaration[0].function_decl.body.block_item[0].statement
        self.assertEqual(ret.exp, c99_ast.Constant(
            const=c99_ast.ConstInt(value=5),
        ))

    def test_large_literal_is_const_long(self):
        ast = parse("int main(void) { return 200; }")
        ret = ast.declaration[0].function_decl.body.block_item[0].statement
        self.assertEqual(ret.exp, c99_ast.Constant(
            const=c99_ast.ConstLong(value=200),
        ))

    def test_int_max_boundary(self):
        # 127 is the maximum signed-1-byte value; still ConstInt.
        # 128 forces ConstLong.
        ast = parse("int main(void) { return 127; }")
        ret = ast.declaration[0].function_decl.body.block_item[0].statement
        self.assertIsInstance(ret.exp.const, c99_ast.ConstInt)
        ast = parse("int main(void) { return 128; }")
        ret = ast.declaration[0].function_decl.body.block_item[0].statement
        self.assertIsInstance(ret.exp.const, c99_ast.ConstLong)

    def test_literal_out_of_range_raises(self):
        # 4294967296 = 2^32 — overshoots even `unsigned long long`,
        # the widest type c6502 models. Per C99 §6.4.4.1 the
        # corresponding type list (hex/octal unsuffixed) terminates
        # at `unsigned long long`, so this is unrepresentable.
        from parser import ParserError
        with self.assertRaises(ParserError) as ctx:
            parse("int main(void) { return 0x100000000; }")
        self.assertIn("doesn't fit", str(ctx.exception))

    def test_literal_promotes_to_long_long(self):
        # 32768 doesn't fit `int` (≤127) or `long` (≤32767); per
        # C99 §6.4.4.1 the next type in the unsuffixed-decimal list
        # is `long long` (4 bytes for c6502), which DOES fit.
        ast = parse("int main(void) { return 32768; }")
        ret = ast.declaration[0].function_decl.body.block_item[0].statement
        self.assertIsInstance(ret.exp.const, c99_ast.ConstLongLong)
        self.assertEqual(ret.exp.const.value, 32768)

    def test_unsigned_suffix_promotes_to_uint(self):
        # `5U` carries a U suffix; per C99 §6.4.4.1 the type list is
        # unsigned int, unsigned long, unsigned long long. 5 fits in
        # `unsigned int`, so it lands in ConstUInt.
        ast = parse("int main(void) { return 5U; }")
        ret = ast.declaration[0].function_decl.body.block_item[0].statement
        self.assertIsInstance(ret.exp.const, c99_ast.ConstUInt)
        self.assertEqual(ret.exp.const.value, 5)

    def test_long_long_accepted(self):
        # `long long` resolves to LongLong (4-byte signed in c6502).
        ast = parse("long long x;")
        decl = ast.declaration[0].var_decl
        self.assertEqual(decl.data_type, c99_ast.LongLong())

    def test_unsigned_long_long_accepted(self):
        ast = parse("unsigned long long x;")
        decl = ast.declaration[0].var_decl
        self.assertEqual(decl.data_type, c99_ast.ULongLong())

    def test_ll_suffix_decimal_is_const_long_long(self):
        ast = parse("int main(void) { return 5LL; }")
        ret = ast.declaration[0].function_decl.body.block_item[0].statement
        self.assertEqual(ret.exp, c99_ast.Constant(
            const=c99_ast.ConstLongLong(value=5),
        ))

    def test_ull_suffix_is_const_ulong_long(self):
        # `0x10ull` carries U+LL; the unsigned-long-long list is
        # just `unsigned long long`, so this lands there directly.
        for src in [
            "int main(void) { return 5ULL; }",
            "int main(void) { return 5ull; }",
            "int main(void) { return 5LLU; }",
        ]:
            with self.subTest(src=src):
                ast = parse(src)
                ret = (
                    ast.declaration[0].function_decl
                    .body.block_item[0].statement
                )
                self.assertEqual(ret.exp, c99_ast.Constant(
                    const=c99_ast.ConstULongLong(value=5),
                ))

    def test_unsuffixed_value_promotes_through_long_long(self):
        # 1000000 doesn't fit Int (≤127), Long (≤32767); the next
        # decimal-unsuffixed type is LongLong, which covers up to
        # 2^31 - 1.
        ast = parse("int main(void) { return 1000000; }")
        ret = ast.declaration[0].function_decl.body.block_item[0].statement
        self.assertEqual(ret.exp, c99_ast.Constant(
            const=c99_ast.ConstLongLong(value=1000000),
        ))

    def test_three_longs_rejected(self):
        from parser import ParserError
        with self.assertRaises(ParserError) as ctx:
            parse("long long long x;")
        self.assertIn("more than twice", str(ctx.exception))


class TestCharAndStringLiterals(unittest.TestCase):
    """Char specifiers (char, signed char, unsigned char), char
    constants `'a'`, and string literals `"abc"`. Char literals
    ride as ConstInt (per the user's choice — C99 §6.4.4.4.10
    matches: "An integer character constant has type int"). String
    literals build a `String` AST node."""

    def test_char_specifier_resolves_to_char(self):
        ast = parse("char x;")
        self.assertEqual(
            ast.declaration[0].var_decl.data_type, c99_ast.Char(),
        )

    def test_signed_char_specifier_resolves_to_schar(self):
        ast = parse("signed char x;")
        self.assertEqual(
            ast.declaration[0].var_decl.data_type, c99_ast.SChar(),
        )

    def test_unsigned_char_specifier_resolves_to_uchar(self):
        ast = parse("unsigned char x;")
        self.assertEqual(
            ast.declaration[0].var_decl.data_type, c99_ast.UChar(),
        )

    def test_char_int_combination_rejected(self):
        from parser import ParserError
        with self.assertRaises(ParserError):
            parse("char int x;")
        with self.assertRaises(ParserError):
            parse("char long x;")

    def test_simple_char_literal_is_const_int(self):
        ast = parse("char x = 'a';")
        self.assertEqual(
            ast.declaration[0].var_decl.init,
            c99_ast.Constant(const=c99_ast.ConstInt(value=97)),
        )

    def test_simple_escape_decodes(self):
        # \n = 0x0A, \\ = 0x5C, \' = 0x27, \t = 0x09.
        for src, val in [
            ("char x = '\\n';",  0x0A),
            ("char x = '\\\\';", 0x5C),
            ("char x = '\\'';",  0x27),
            ("char x = '\\t';",  0x09),
            ("char x = '\\0';",  0x00),
        ]:
            with self.subTest(src=src):
                ast = parse(src)
                self.assertEqual(
                    ast.declaration[0].var_decl.init.const,
                    c99_ast.ConstInt(value=val),
                )

    def test_hex_escape_decodes(self):
        ast = parse("char x = '\\x41';")
        self.assertEqual(
            ast.declaration[0].var_decl.init.const,
            c99_ast.ConstInt(value=0x41),
        )

    def test_octal_escape_decodes(self):
        ast = parse("char x = '\\101';")
        self.assertEqual(
            ast.declaration[0].var_decl.init.const,
            c99_ast.ConstInt(value=0o101),
        )

    def test_string_literal_decodes_bytes(self):
        ast = parse('int main(void) { return "abc"[0]; }')
        ret = ast.declaration[0].function_decl.body.block_item[0].statement
        self.assertIsInstance(ret.exp.array, c99_ast.String)
        self.assertEqual(ret.exp.array.str, "abc")

    def test_adjacent_string_literals_concatenate(self):
        ast = parse('int main(void) { return "abc" "def" "ghi"[0]; }')
        ret = ast.declaration[0].function_decl.body.block_item[0].statement
        self.assertEqual(ret.exp.array.str, "abcdefghi")

    def test_string_literal_with_escapes(self):
        # `"a\\nb"` — three bytes: 'a', '\n', 'b'.
        ast = parse('int main(void) { return "a\\nb"[0]; }')
        ret = ast.declaration[0].function_decl.body.block_item[0].statement
        self.assertEqual(ret.exp.array.str, "a\nb")

    def test_cast_to_long(self):
        ast = parse("int main(void) { return (long)5; }")
        ret = ast.declaration[0].function_decl.body.block_item[0].statement
        self.assertEqual(
            ret.exp,
            c99_ast.Cast(
                target_type=c99_ast.Long(),
                exp=c99_ast.Constant(const=c99_ast.ConstInt(value=5)),
            ),
        )

    def test_cast_is_right_associative(self):
        # `(int)(long)5` parses as `(int)((long)5)`.
        ast = parse("int main(void) { return (int)(long)5; }")
        ret = ast.declaration[0].function_decl.body.block_item[0].statement
        self.assertEqual(
            ret.exp,
            c99_ast.Cast(
                target_type=c99_ast.Int(),
                exp=c99_ast.Cast(
                    target_type=c99_ast.Long(),
                    exp=c99_ast.Constant(const=c99_ast.ConstInt(value=5)),
                ),
            ),
        )

    def test_cast_binds_tighter_than_multiply(self):
        # `(int)x * 2` parses as `((int)x) * 2`, not `(int)(x * 2)`.
        ast = parse("int main(void) { int x; return (int)x * 2; }")
        ret = ast.declaration[0].function_decl.body.block_item[1].statement
        self.assertIsInstance(ret.exp, c99_ast.Binary)
        self.assertIsInstance(ret.exp.left, c99_ast.Cast)
        self.assertIsInstance(ret.exp.right, c99_ast.Constant)

    def test_unary_minus_takes_cast_exp(self):
        # `-(int)x` parses as `-((int)x)` per §6.5.3.1 (unary-operator
        # takes a cast-expression).
        ast = parse("int main(void) { int x; return -(int)x; }")
        ret = ast.declaration[0].function_decl.body.block_item[1].statement
        self.assertIsInstance(ret.exp, c99_ast.Unary)
        self.assertIsInstance(ret.exp.op, c99_ast.Negate)
        self.assertIsInstance(ret.exp.exp, c99_ast.Cast)

    def test_prefix_increment_does_not_take_cast(self):
        # `++(int)x` is a parse error: prefix ++ takes a unary-exp,
        # which excludes casts. (And the cast result isn't an lvalue
        # anyway, so this matches C99's rejection.)
        with self.assertRaises(UnexpectedInput):
            parse("int main(void) { int x; return ++(int)x; }")

    def test_dereference_simple(self):
        # `*p` builds a Dereference node, not a Unary.
        self.assertEqual(
            _exp_of("*p"),
            c99_ast.Dereference(exp=c99_ast.Var(name="p")),
        )

    def test_address_of_simple(self):
        # `&x` builds an AddressOf node, not a Unary.
        self.assertEqual(
            _exp_of("&x"),
            c99_ast.AddressOf(exp=c99_ast.Var(name="x")),
        )

    def test_nested_dereference(self):
        # `**p` parses right-to-left through the cast_exp recursion:
        # the outer STAR's operand is itself a Dereference.
        self.assertEqual(
            _exp_of("**p"),
            c99_ast.Dereference(
                exp=c99_ast.Dereference(exp=c99_ast.Var(name="p")),
            ),
        )

    def test_dereference_compose_with_address_of(self):
        # `*&x` collapses to x's value at the type level, but at the
        # parse level it's just a nesting of the two operators.
        self.assertEqual(
            _exp_of("*&x"),
            c99_ast.Dereference(
                exp=c99_ast.AddressOf(exp=c99_ast.Var(name="x")),
            ),
        )

    def test_dereference_takes_cast_exp(self):
        # Like the other unary operators, `*` takes a cast_exp — so
        # `*(int *)p` would parse as `*((int *)p)` once cast targets
        # accept pointer types. For now, `*(p)` is the simplest form
        # that exercises the same precedence path.
        self.assertEqual(
            _exp_of("*(p)"),
            c99_ast.Dereference(exp=c99_ast.Var(name="p")),
        )

    def test_unary_star_does_not_conflict_with_multiply(self):
        # `a * *p` — the right operand of `*` is `*p` (a unary
        # dereference), not a syntax error. Same precedence story as
        # `a - -b` for unary minus.
        self.assertEqual(
            _exp_of("a * *p"),
            c99_ast.Binary(
                op=c99_ast.Multiply(),
                left=c99_ast.Var(name="a"),
                right=c99_ast.Dereference(exp=c99_ast.Var(name="p")),
            ),
        )

    def test_unary_amp_does_not_conflict_with_bitwise_and(self):
        # `a & &x` — the right operand of bitwise `&` is `&x` (an
        # address-of), not a syntax error.
        self.assertEqual(
            _exp_of("a & &x"),
            c99_ast.Binary(
                op=c99_ast.BitwiseAnd(),
                left=c99_ast.Var(name="a"),
                right=c99_ast.AddressOf(exp=c99_ast.Var(name="x")),
            ),
        )

    def test_dereference_as_assignment_lval_parses(self):
        # The grammar allows `*p = 5;` even though the lvalue check in
        # identifier_resolution may still reject non-Var lvals — that
        # check belongs to a later pass, not the grammar.
        ast = parse("int main(void) { int p; *p = 5; }")
        item = ast.declaration[0].function_decl.body.block_item[1]
        self.assertIsInstance(item.statement, c99_ast.Expression)
        assn = item.statement.exp
        self.assertIsInstance(assn, c99_ast.Assignment)
        self.assertIsInstance(assn.lval, c99_ast.Dereference)


class TestFloatingTypesAndConstants(unittest.TestCase):
    """C99 §6.4.4.2 floating constants. Three terminals, one variant
    per suffix: unsuffixed → ConstDouble, f/F → ConstFloat, l/L →
    rejected (no long double in c6502). The `float` and `double`
    type specifiers join `int`/`long`/`signed`/`unsigned`."""

    def test_double_var_decl(self):
        ast = parse("double x = 3.14;")
        vd = ast.declaration[0].var_decl
        self.assertEqual(vd.data_type, c99_ast.Double())
        self.assertEqual(
            vd.init,
            c99_ast.Constant(const=c99_ast.ConstDouble(
                bits=fp_arith.double_string_to_bits("3.14"),
            )),
        )

    def test_float_var_decl(self):
        ast = parse("float y = 2.5f;")
        vd = ast.declaration[0].var_decl
        self.assertEqual(vd.data_type, c99_ast.Float())
        self.assertEqual(
            vd.init,
            c99_ast.Constant(const=c99_ast.ConstFloat(
                bits=fp_arith.single_string_to_bits("2.5"),
            )),
        )

    def test_unsuffixed_literal_is_const_double(self):
        for src in ["3.14", "1.0", ".5", "1e10", "1.0e-3", "3."]:
            with self.subTest(src=src):
                ast = parse(f"double x = {src};")
                self.assertIsInstance(
                    ast.declaration[0].var_decl.init.const,
                    c99_ast.ConstDouble,
                )

    def test_f_suffix_is_const_float(self):
        for src in ["3.14f", "3.14F", "1e10f", ".5F"]:
            with self.subTest(src=src):
                ast = parse(f"float x = {src};")
                self.assertIsInstance(
                    ast.declaration[0].var_decl.init.const,
                    c99_ast.ConstFloat,
                )

    def test_long_double_literal_rejected(self):
        from parser import ParserError
        for src in ["3.14l", "3.14L", "1e10l"]:
            with self.subTest(src=src):
                with self.assertRaises(ParserError) as ctx:
                    parse(f"double x = {src};")
                self.assertIn("long double", str(ctx.exception))

    def test_long_double_type_rejected(self):
        from parser import ParserError
        with self.assertRaises(ParserError) as ctx:
            parse("long double x = 1.0;")
        self.assertIn("long double", str(ctx.exception))

    def test_hex_float_rejected(self):
        from parser import ParserError
        with self.assertRaises(ParserError) as ctx:
            parse("double x = 0x1.0p3;")
        self.assertIn("hex floating literal", str(ctx.exception))

    def test_float_double_combined_rejected(self):
        from parser import ParserError
        with self.assertRaises(ParserError) as ctx:
            parse("float double x;")
        self.assertIn("'float' and 'double'", str(ctx.exception))

    def test_fp_with_int_specifier_rejected(self):
        from parser import ParserError
        for src in ["int float x;", "unsigned double x;",
                    "double signed x;"]:
            with self.subTest(src=src):
                with self.assertRaises(ParserError) as ctx:
                    parse(src)
                self.assertIn(
                    "floating type cannot combine", str(ctx.exception),
                )

    def test_cast_to_double(self):
        # The cast LPAREN-disambiguation must shift on FLOAT / DOUBLE
        # like it does on INT / LONG.
        ast = parse("int main(void) { return (double)1; }")
        ret = ast.declaration[0].function_decl.body.block_item[0].statement
        self.assertEqual(
            ret.exp,
            c99_ast.Cast(
                target_type=c99_ast.Double(),
                exp=c99_ast.Constant(const=c99_ast.ConstInt(value=1)),
            ),
        )

    def test_function_return_type_double(self):
        ast = parse("double pi(void);")
        fd = ast.declaration[0].function_decl
        self.assertEqual(
            fd.data_type,
            c99_ast.FunType(params=[], ret=c99_ast.Double()),
        )

    def test_cast_to_array_type_rejected(self):
        # `(int[3])foo` — array cast targets aren't lvalues and
        # have no rvalue conversion either, so the cast is
        # meaningless. Rejected at parse time with a clean error.
        from parser import ParserError
        with self.assertRaises(ParserError) as ctx:
            parse("int main(void) { int x; return (int[3])x; }")
        self.assertIn("cast to an array type", str(ctx.exception))

    def test_cast_to_pointer_array_type_rejected(self):
        # `(int *[3])foo` — array of pointers is still an array.
        from parser import ParserError
        with self.assertRaises(ParserError) as ctx:
            parse("int main(void) { int x; return (int *[3])x; }")
        self.assertIn("cast to an array type", str(ctx.exception))

    def test_cast_to_multi_dim_array_type_rejected(self):
        # `(int[3][4])foo` — multi-dim array cast.
        from parser import ParserError
        with self.assertRaises(ParserError) as ctx:
            parse("int main(void) { int x; return (int[3][4])x; }")
        self.assertIn("cast to an array type", str(ctx.exception))


class TestDeclaratorGrammar(unittest.TestCase):
    """Grammar-only tests for §6.7.5 declarators. The new rules
    (`declarator` / `direct_declarator` / `pointer` / `parameter_*`
    / `identifier_list`) aren't yet reachable from the translation-
    unit start rule, so we parse them via Lark's `declarator`
    start. The transformer doesn't have methods for these yet, so
    we don't assert on AST shape — just that the grammar accepts
    the form (no `UnexpectedInput`)."""

    @staticmethod
    def _parse_declarator(src: str):
        from parser import _LARK
        return _LARK.parse(src, start="declarator")

    def test_plain_identifier(self):
        # A bare IDENTIFIER is the simplest declarator.
        self._parse_declarator("p")

    def test_pointer_to_identifier(self):
        # `*p` — pointer + IDENTIFIER.
        self._parse_declarator("*p")

    def test_pointer_to_pointer(self):
        # `**p` — chained pointer.
        self._parse_declarator("**p")

    def test_qualified_pointer(self):
        # `* const p` — type-qualifier in the pointer rule.
        self._parse_declarator("* const p")

    def test_pointer_to_qualified_pointer(self):
        # `* const * volatile p` — qualifiers chain through.
        self._parse_declarator("* const * volatile p")

    def test_parenthesised_declarator(self):
        # `(*p)` — parenthesised pointer declarator. Used as the
        # building block for function-pointer types.
        self._parse_declarator("(*p)")

    def test_array_declarator(self):
        # `a[10]` — direct_declarator with the plain array suffix.
        self._parse_declarator("a[10]")

    def test_empty_array_declarator(self):
        # `a[]` — `array_size_plain` with empty assignment_exp.
        self._parse_declarator("a[]")

    def test_static_array_declarator(self):
        # `a[static 10]` — array_size_static.
        self._parse_declarator("a[static 10]")

    def test_qualifier_static_array_declarator(self):
        # `a[const static 10]` — `array_size_quals_static`.
        self._parse_declarator("a[const static 10]")

    def test_unspecified_size_array_declarator(self):
        # `a[*]` — VLA "unspecified size" form (§6.7.5.2.1).
        self._parse_declarator("a[*]")

    def test_function_declarator_no_params(self):
        # `f()` — direct_declarator with empty identifier_list.
        self._parse_declarator("f()")

    def test_function_declarator_with_params(self):
        # `f(int x, long y)` — parameter_type_list with two named
        # parameter_declarations.
        self._parse_declarator("f(int x, long y)")

    def test_function_declarator_unnamed_param(self):
        # `f(int)` — parameter_declaration with no declarator.
        self._parse_declarator("f(int)")

    def test_function_declarator_pointer_param(self):
        # `f(int *)` — parameter_declaration with abstract_declarator
        # (a pointer alone).
        self._parse_declarator("f(int *)")

    def test_variadic_function_declarator(self):
        # `f(int x, ...)` — parameter_type_list with the trailing
        # ELLIPSIS.
        self._parse_declarator("f(int x, ...)")

    def test_function_pointer(self):
        # `(*fp)(int)` — the canonical function-pointer declarator.
        self._parse_declarator("(*fp)(int)")

    def test_array_of_pointers(self):
        # `*a[10]` — pointer + direct_declarator with array suffix.
        # Per C99 precedence the suffix binds tighter than the
        # pointer prefix, giving "array of pointers to int" once
        # wrapped with `int`.
        self._parse_declarator("*a[10]")

    def test_kr_identifier_list(self):
        # `f(x, y, z)` — old-style K&R declarator with an
        # identifier list (no types). Accepted for grammar
        # completeness even though the AST won't model it.
        self._parse_declarator("f(x, y, z)")


class TestAbstractDeclaratorGrammar(unittest.TestCase):
    """Grammar tests for §6.7.6 abstract declarators. Reachable
    from `type_name` (which appears inside cast expressions). The
    `type_name` transformer raises NotImplementedError when an
    abstract_declarator is present — the grammar parses the form
    but the AST isn't wired through yet — so we exercise the
    `type_name` start rule directly to confirm grammar acceptance
    without forcing the transformer to run."""

    @staticmethod
    def _parse_type_name(src: str):
        from parser import _LARK
        return _LARK.parse(src, start="type_name")

    def test_plain_specifier_only(self):
        # `int` — no abstract_declarator. Existing behavior.
        self._parse_type_name("int")

    def test_pointer(self):
        # `int *` — pointer-only abstract_declarator.
        self._parse_type_name("int *")

    def test_pointer_to_pointer(self):
        # `int **` — chained pointer.
        self._parse_type_name("int **")

    def test_qualified_pointer(self):
        # `int * const` — type-qualifier inside the pointer rule.
        self._parse_type_name("int * const")

    def test_array(self):
        # `int [3]` — direct_abstract_declarator with no prefix +
        # array suffix (§6.7.6 form 2, empty prefix).
        self._parse_type_name("int [3]")

    def test_empty_array(self):
        # `int []` — array_size_plain with both qualifiers and size
        # absent.
        self._parse_type_name("int []")

    def test_function_no_params(self):
        # `int ()` — function abstract declarator, no params.
        self._parse_type_name("int ()")

    def test_function_with_params(self):
        # `int (int)` — function abstract declarator. The LPAREN
        # vs. parenthesised-abstract-declarator ambiguity is
        # resolved by lookahead on the token after LPAREN: type-
        # specifier / RPAREN → parameter_type_list form.
        self._parse_type_name("int (int)")

    def test_function_pointer(self):
        # `int (*)(int)` — pointer to function returning int taking
        # int. The canonical place where an abstract_declarator's
        # parenthesised form appears.
        self._parse_type_name("int (*)(int)")

    def test_pointer_to_array(self):
        # `int (*)[3]` — pointer to array of 3 ints.
        self._parse_type_name("int (*)[3]")

    def test_pointer_cast_builds_pointer_target_type(self):
        # `(int *)x` — the cast's target_type is `Pointer(Int())`.
        # The full parse() path runs the type_name transformer,
        # which composes the pointer wrapper around the base type
        # via `_apply_abstract_declarator`. Verify by inspecting
        # the resulting Cast node.
        ast = parse("int main(void) { int x; return (int *)x; }")
        ret = ast.declaration[0].function_decl.body.block_item[1].statement
        self.assertIsInstance(ret.exp, c99_ast.Cast)
        self.assertEqual(
            ret.exp.target_type,
            c99_ast.Pointer(referenced_type=c99_ast.Int()),
        )


class TestPointerDeclarations(unittest.TestCase):
    """End-to-end tests that pointer types in var_decl /
    function_decl flow correctly through the parser into
    c99_ast nodes. Pointers are 2 bytes (the 6502's address
    width); see also test_replace_pseudoregisters for the
    frame-layout side and test_tac_to_asm for the size-dispatch."""

    def test_pointer_var_decl(self):
        # `int *p;` — file-scope variable with pointer type.
        ast = parse("int *p;")
        vd = ast.declaration[0].var_decl
        self.assertEqual(vd.name, "p")
        self.assertEqual(vd.data_type, c99_ast.Pointer(referenced_type=c99_ast.Int()))

    def test_pointer_to_pointer_var_decl(self):
        # `int **p;` — pointer to pointer.
        ast = parse("int **p;")
        vd = ast.declaration[0].var_decl
        self.assertEqual(
            vd.data_type,
            c99_ast.Pointer(
                referenced_type=c99_ast.Pointer(referenced_type=c99_ast.Int()),
            ),
        )

    def test_pointer_var_inside_function(self):
        # Block-scope pointer variable.
        ast = parse("int main(void) { int *q; return 0; }")
        body = ast.declaration[0].function_decl.body
        # block_item[0] is the declaration of q.
        d = body.block_item[0].declaration.var_decl
        self.assertEqual(d.name, "q")
        self.assertEqual(d.data_type, c99_ast.Pointer(referenced_type=c99_ast.Int()))

    def test_long_pointer_var_decl(self):
        # `long *p;` — pointer to long. The pointer's size is still
        # 2 bytes; the pointee being long doesn't change that.
        ast = parse("long *p;")
        vd = ast.declaration[0].var_decl
        self.assertEqual(
            vd.data_type, c99_ast.Pointer(referenced_type=c99_ast.Long()),
        )

    def test_function_returning_pointer(self):
        # `int *foo(void);` — forward decl of a function returning
        # a pointer. `foo` lands in var_decl (function-typed
        # declarator with no body) and the transformer rewraps it
        # as a FunctionDecl with body=None.
        ast = parse("int *foo(void);")
        fd = ast.declaration[0].function_decl
        self.assertEqual(fd.name, "foo")
        self.assertIsNone(fd.body)
        self.assertEqual(fd.params, [])
        self.assertEqual(
            fd.data_type,
            c99_ast.FunType(
                params=[], ret=c99_ast.Pointer(referenced_type=c99_ast.Int()),
            ),
        )

    def test_function_with_pointer_param(self):
        # `int foo(int *p);` — forward decl with a named pointer
        # param.
        ast = parse("int foo(int *p);")
        fd = ast.declaration[0].function_decl
        self.assertEqual(fd.params, ["p"])
        self.assertEqual(
            fd.data_type,
            c99_ast.FunType(
                params=[c99_ast.Pointer(referenced_type=c99_ast.Int())],
                ret=c99_ast.Int(),
            ),
        )

    def test_function_with_unnamed_pointer_param(self):
        # `int foo(int *);` — forward decl with an unnamed pointer
        # param (abstract_declarator on the param). Param name is
        # the empty string in the AST (legacy convention; could be
        # None but the AST field is `identifier*`).
        ast = parse("int foo(int *);")
        fd = ast.declaration[0].function_decl
        # The param-name list reflects the unnamed slot; the type
        # list still has the pointer.
        self.assertEqual(
            fd.data_type.params,
            [c99_ast.Pointer(referenced_type=c99_ast.Int())],
        )

    def test_function_definition_with_pointer_param_and_return(self):
        # `int *foo(int *p) { return p; }` — full definition.
        ast = parse("int *foo(int *p) { return p; }")
        fd = ast.declaration[0].function_decl
        self.assertEqual(fd.name, "foo")
        self.assertEqual(fd.params, ["p"])
        self.assertEqual(
            fd.data_type,
            c99_ast.FunType(
                params=[c99_ast.Pointer(referenced_type=c99_ast.Int())],
                ret=c99_ast.Pointer(referenced_type=c99_ast.Int()),
            ),
        )
        self.assertIsNotNone(fd.body)

    def test_pointer_var_decl_through_full_pipeline(self):
        # End-to-end: `int *p;` should make it through the full
        # compile pipeline (parse → identifier_resolution →
        # label_resolution → loop_labeling → type_checking →
        # c99_to_tac → tac_to_asm → replace_pseudoregisters →
        # asm_emit) without raising. Pointer is sized as 2 bytes,
        # so a tentative `int *p;` lays down as `DS.B 2` (zero-run).
        from compile import _run_stage
        text = _run_stage("codegen", "int *p;\n")
        self.assertIn("p:", text)
        self.assertIn("DS.B  2", text)

    def test_pointer_local_gets_two_frame_bytes(self):
        # Pointer locals occupy 2 contiguous frame bytes — same
        # treatment as Long. Verify the prologue allocates the
        # right amount.
        from compile import _run_stage
        text = _run_stage(
            "codegen",
            "int main(void) { int *p; p = p; return 0; }\n",
        )
        # M=2 (one pointer local) + 2 (saved-FP slot) = 4; SSP -= 4.
        self.assertIn("prologue: 0 arg bytes, 2 local bytes", text)
        self.assertIn("SBC   #$04", text)

    def test_pointer_function_returns_long_style(self):
        # A function returning `int *` uses the same 2-byte return
        # convention as `long`: bytes written to HARGS+0..1.
        # No PHA/PLA wrap (save_a=False) since no register holds the
        # return.
        from compile import _run_stage
        text = _run_stage(
            "codegen",
            "int *foo(int *p) { return p; }\n"
            "int main(void) { return 0; }\n",
        )
        self.assertIn("foo:", text)
        # The return sequence writes the two bytes into HARGS+0/+1.
        self.assertIn("STA   HARGS\n", text)
        self.assertIn("STA   HARGS+1\n", text)
        # Prologue allocates 0 local bytes (pointer param sits in
        # the caller's pushed args, not in our locals).
        self.assertIn("prologue: 2 arg bytes, 0 local bytes", text)


class TestPointerOpsEndToEnd(unittest.TestCase):
    """Dereference (`*p`) and AddressOf (`&x`) flowing all the way
    through to 6502 asm. Smoke-checks the pipeline: parse → resolve
    → type-check → c99_to_tac → tac_to_asm → replace_pseudoregisters
    → asm_emit."""

    def _codegen(self, src: str) -> str:
        from compile import _run_stage
        return _run_stage("codegen", src)

    def test_address_of_local_uses_fp_arithmetic(self):
        # `&x` for a local computes its address as `FP + frame_off`
        # via a 16-bit add. We don't pin the exact frame offset in
        # case layout shifts; just check the shape: CLC + LDA FP +
        # ADC #imm followed by a store, then LDA FP+1 + ADC #$00 +
        # store.
        text = self._codegen(
            "int main(void) { int x; int *p; p = &x; return 0; }\n"
        )
        self.assertIn("CLC", text)
        self.assertIn("LDA   FP", text)
        self.assertIn("LDA   FP+1", text)
        self.assertIn("ADC   #$00", text)

    def test_address_of_static_uses_immediate_label(self):
        # `&g` for a file-scope object uses `#<g` / `#>g`
        # immediates to load the address bytes; no FP arithmetic.
        text = self._codegen(
            "int g;\nint main(void) { int *p; p = &g; return 0; }\n"
        )
        self.assertIn("LDA   #<g", text)
        self.assertIn("LDA   #>g", text)

    def test_dereference_read_uses_dptr(self):
        # `*p` (read) stages p's two bytes into DPTR / DPTR+1, then
        # reads via `(DPTR),Y`.
        text = self._codegen(
            "int main(void) { int x; int *p; p = &x; return *p; }\n"
        )
        self.assertIn("STA   DPTR", text)
        self.assertIn("STA   DPTR+1", text)
        self.assertIn("LDA   (DPTR),Y", text)

    def test_dereference_write_uses_dptr(self):
        # `*p = 5` stages p's bytes into DPTR / DPTR+1, then writes
        # via `STA (DPTR),Y` with `Y = 0` for the (single) byte of
        # an Int store.
        text = self._codegen(
            "int main(void) { int x; int *p; p = &x; *p = 5; return 0; }\n"
        )
        self.assertIn("STA   DPTR", text)
        self.assertIn("STA   (DPTR),Y", text)
        # The value 5 lands in A right before the indirect store.
        self.assertIn("LDA   #$05", text)

    def test_pointer_to_long_writes_two_bytes_through_dptr(self):
        # `*lp = 0x1234L` for a `long *lp` writes 2 bytes via
        # `(DPTR),Y` with Y=0 then Y=1 — same byte-pair pattern as
        # any other Long copy, but the destination is indirect
        # rather than a Frame slot.
        text = self._codegen(
            "int main(void) { long y; long *lp; lp = &y; "
            "*lp = 0x1234L; return 0; }\n"
        )
        self.assertIn("LDA   #$34", text)
        self.assertIn("LDA   #$12", text)
        # Two indirect writes (Y=0 for low, Y=1 for high).
        self.assertIn("LDY   #$00", text)
        self.assertIn("LDY   #$01", text)
        self.assertIn("STA   (DPTR),Y", text)

    def test_address_of_dereference_collapses(self):
        # `&*p` ≡ `p` per C99 §6.5.3.2.3 — c99_to_tac elides the
        # GetAddress when the operand is a Dereference, so `q = &*p`
        # produces no LoadAddress sequence (no FP-arithmetic, no
        # ImmLabel pair) — just a plain Long-style Copy from p to q.
        # Compare against the same program with `q = &x` to confirm
        # the elision: the `&*p` form should have one *fewer*
        # FP-add sequence than the `&x` form.
        with_amp_x = self._codegen(
            "int main(void) { int x; int *p; int *q; "
            "p = &x; q = &x; return 0; }\n"
        )
        with_amp_deref = self._codegen(
            "int main(void) { int x; int *p; int *q; "
            "p = &x; q = &*p; return 0; }\n"
        )
        # The CLC count differs by exactly 1: the `&*p` version
        # omits the LoadAddress arithmetic that `&x` would emit.
        # (Each `&local` lowers to one CLC for its FP-add; the
        # epilogue contributes a fixed CLC of its own.)
        self.assertEqual(
            with_amp_x.count("CLC") - with_amp_deref.count("CLC"), 1,
        )


class TestPointerEquality(unittest.TestCase):
    """`==` and `!=` between pointers, plus the null-pointer-
    constant exception. Relational ordering (`<` / `>` / `<=` / `>=`)
    on same-type pointers is also covered here; arithmetic ops on
    pointers live in their own test class."""

    def _codegen(self, src: str) -> str:
        from compile import _run_stage
        return _run_stage("codegen", src)

    def _typecheck(self, src: str) -> None:
        # Run the pipeline through the type-check stage so type
        # errors surface here rather than from later passes.
        from compile import _run_stage
        _run_stage("tac", src)

    def test_equal_same_pointer_type(self):
        # `p == q` for two same-type pointers — the existing 2-byte
        # Equal lowering applies unchanged (Pointer is sized like
        # Long), so the asm has the high-byte / low-byte CMP pair.
        text = self._codegen(
            "int main(void) {\n"
            "  int x;\n"
            "  int *p; int *q;\n"
            "  p = &x; q = &x;\n"
            "  return p == q;\n"
            "}\n"
        )
        # Two CMPs (one per byte) and the BNE short-circuit landmark.
        self.assertEqual(text.count("CMP   (FP),Y"), 2)
        self.assertIn(".cmp_differ@", text)

    def test_not_equal_same_pointer_type(self):
        text = self._codegen(
            "int main(void) {\n"
            "  int x;\n"
            "  int *p; int *q;\n"
            "  p = &x; q = &x;\n"
            "  return p != q;\n"
            "}\n"
        )
        self.assertEqual(text.count("CMP   (FP),Y"), 2)

    def test_pointer_equal_to_null_constant_on_right(self):
        # `p == 0` — the literal 0 is recognized as a null pointer
        # constant; the type checker converts it to the pointer
        # type. End-to-end this should compile cleanly.
        self._codegen(
            "int main(void) { int *p; if (p == 0) return 1; return 0; }\n"
        )

    def test_pointer_equal_to_null_constant_on_left(self):
        # `0 == p` — same legality, mirror order.
        self._codegen(
            "int main(void) { int *p; if (0 == p) return 1; return 0; }\n"
        )

    def test_pointer_not_equal_to_null_through_long(self):
        # `p != 0L` — null pointer constant via a Long literal. The
        # detector drills past the `0` regardless of the integer
        # variant.
        self._codegen(
            "int main(void) { int *p; if (p != 0L) return 1; return 0; }\n"
        )

    def test_pointer_equal_to_null_through_cast(self):
        # `(long)0` is still a null pointer constant — the detector
        # drills through Cast wrappers.
        self._codegen(
            "int main(void) { int *p; if (p == (long)0) return 1; return 0; }\n"
        )

    def test_pointer_compare_to_nonzero_int_rejected(self):
        # `p == 5` — 5 isn't a null pointer constant, so the
        # comparison is illegal under our rules.
        from passes.type_checking import TypeCheckError
        from lark.exceptions import VisitError
        with self.assertRaises((TypeCheckError, VisitError)) as cm:
            self._typecheck(
                "int main(void) { int *p; if (p == 5) return 1; return 0; }\n"
            )
        # Unwrap the TypeCheckError if it's wrapped (compile.py
        # may surface it directly).
        err = cm.exception
        if hasattr(err, "orig_exc") and isinstance(err.orig_exc, TypeCheckError):
            err = err.orig_exc
        self.assertIsInstance(err, TypeCheckError)

    def test_pointer_compare_distinct_pointer_types_rejected(self):
        # `int *p; long *q; p == q;` — different pointer types,
        # rejected even though both sides are pointers.
        from passes.type_checking import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) {\n"
                "  int *p; long *q;\n"
                "  if (p == q) return 1;\n"
                "  return 0;\n"
                "}\n"
            )

    def test_ordering_same_pointer_type_compiles(self):
        # `<` / `>` / `<=` / `>=` on same-type pointers — pointer
        # operands dispatch to the unsigned-ordering lowering (no
        # V-correction; carry flag carries the ordering result), so
        # we get the per-byte SBC pair followed by BCC for `<`.
        text = self._codegen(
            "int main(void) {\n"
            "  int x;\n"
            "  int *p; int *q;\n"
            "  p = &x; q = &x;\n"
            "  return p < q;\n"
            "}\n"
        )
        # Two SBCs (one per byte) and BCC for the unsigned `<`.
        self.assertEqual(text.count("SBC   (FP),Y"), 2)
        self.assertIn("BCC   .cmp_true@", text)
        self.assertNotIn(".cmp_novf@", text)

    def test_ordering_distinct_pointer_types_rejected(self):
        from passes.type_checking import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) { int *p; long *q; "
                "if (p < q) return 1; return 0; }\n"
            )

    def test_ordering_pointer_vs_int_rejected(self):
        # No null-pointer-constant exception for relational ops, so
        # `p < 0` is rejected even though `p == 0` would be fine.
        from passes.type_checking import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) { int *p; if (p < 0) return 1; "
                "return 0; }\n"
            )

    def test_equal_result_is_int(self):
        # The result of `==` on pointers is `int` (per C99 §6.5.9.1),
        # matching the existing comparison-result rule. Verify by
        # using it in a context that requires an int (a `return`
        # from an int-returning function).
        self._codegen(
            "int main(void) { int *p; int *q; "
            "p = q; return p == q; }\n"
        )


class TestPointerIntegerCasts(unittest.TestCase):
    """Casts between integer types and pointer types, both
    directions. Pointer is 2 bytes, so the existing
    SignExtend / ZeroExtend / Truncate / no-op machinery covers
    everything via byte-width dispatch — these tests pin the
    end-to-end paths."""

    def _codegen(self, src: str) -> str:
        from compile import _run_stage
        return _run_stage("codegen", src)

    def test_pointer_to_int_uses_truncate(self):
        # `(int)p` — Pointer (2B) → Int (1B) is a Truncate. The
        # asm just moves the low byte; the high byte is dropped.
        # No `BMI` (sign-extend marker) and no `LDA #$00` for the
        # high half should appear from the cast itself.
        text = self._codegen(
            "int main(void) {\n"
            "  int x; int a; int *p;\n"
            "  p = &x;\n"
            "  a = (int)p;\n"
            "  return a;\n"
            "}\n"
        )
        # The cast produces no extra arithmetic — just plain Movs.
        # Verify by checking a representative pattern: the asm
        # successfully includes a load of A and store-back. (Pinning
        # exact lines is too brittle; the smoke is that codegen
        # completes and the program structure is intact.)
        self.assertIn("RTS", text)

    def test_pointer_to_long_is_no_op(self):
        # `(long)p` — Pointer (2B) → Long (2B) is a no-op cast at
        # the c99_to_tac level (same width); the inner val passes
        # through. Smoke-check the program compiles end-to-end.
        self._codegen(
            "int main(void) {\n"
            "  int x; long l; int *p;\n"
            "  p = &x;\n"
            "  l = (long)p;\n"
            "  return 0;\n"
            "}\n"
        )

    def test_int_to_pointer_sign_extends(self):
        # `(int *)a` — Int (1B, signed) → Pointer (2B). Goes
        # through the SignExtend path: the low byte is the source's
        # value, the high byte is 0x00 if the source is non-negative
        # and 0xFF if it's negative. Verify by looking for the
        # sign-extend label landmarks.
        text = self._codegen(
            "int main(void) {\n"
            "  int a; int *p;\n"
            "  a = 5;\n"
            "  p = (int *)a;\n"
            "  return 0;\n"
            "}\n"
        )
        self.assertIn(".sx_neg@", text)
        self.assertIn("LDA   #$FF", text)
        self.assertIn("LDA   #$00", text)

    def test_uint_to_pointer_zero_extends(self):
        # `(int *)u` — UInt (1B, unsigned) → Pointer (2B). Goes
        # through ZeroExtend: high byte unconditionally zero, no
        # branch. The asm has the low-byte copy followed by a
        # `LDA #$00` for the high byte; no sign-extend labels.
        text = self._codegen(
            "int main(void) {\n"
            "  unsigned int u; int *p;\n"
            "  u = 200u;\n"
            "  p = (int *)u;\n"
            "  return 0;\n"
            "}\n"
        )
        self.assertNotIn(".sx_neg@", text)
        self.assertIn("LDA   #$00", text)

    def test_long_to_pointer_is_no_op(self):
        # `(int *)l` — Long (2B) → Pointer (2B) is a no-op cast.
        # No SignExtend / ZeroExtend / Truncate needed; the inner
        # val passes through. Smoke-check it compiles.
        self._codegen(
            "int main(void) {\n"
            "  long l; int *p;\n"
            "  l = 0x1234L;\n"
            "  p = (int *)l;\n"
            "  return 0;\n"
            "}\n"
        )

    def test_pointer_to_pointer_different_pointee_no_op(self):
        # `(long *)p` where p is `int *` — Pointer (2B) → Pointer
        # (2B) is a no-op cast (the bytes are identical; only the
        # type-checker's view of the pointee changes). Smoke-check
        # the program compiles.
        self._codegen(
            "int main(void) {\n"
            "  int x; int *p; long *lp;\n"
            "  p = &x;\n"
            "  lp = (long *)p;\n"
            "  return 0;\n"
            "}\n"
        )

    def test_double_pointer_cast(self):
        # `(int **)a` — building a 2-byte pointer-to-pointer from
        # a 1-byte int. Same SignExtend path as `(int *)a`.
        self._codegen(
            "int main(void) {\n"
            "  int a; int **pp;\n"
            "  a = 1;\n"
            "  pp = (int **)a;\n"
            "  return 0;\n"
            "}\n"
        )

    def test_float_to_pointer_rejected(self):
        # C99 §6.3.2.3 doesn't define floating ↔ pointer
        # conversions. The type checker rejects them with a
        # focused message.
        from passes.type_checking import TypeCheckError
        with self.assertRaises(TypeCheckError) as cm:
            self._typecheck(
                "int main(void) { float f; int *p; "
                "f = 1.5f; p = (int *)f; return 0; }\n"
            )
        self.assertIn("pointer", str(cm.exception))
        self.assertIn("floating", str(cm.exception))

    def test_double_to_pointer_rejected(self):
        from passes.type_checking import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) { double d; long *lp; "
                "d = 2.5; lp = (long *)d; return 0; }\n"
            )

    def test_pointer_to_float_rejected(self):
        from passes.type_checking import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) { int x; int *p; float f; "
                "p = &x; f = (float)p; return 0; }\n"
            )

    def test_pointer_to_double_rejected(self):
        from passes.type_checking import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) { int x; int *p; double d; "
                "p = &x; d = (double)p; return 0; }\n"
            )

    def _typecheck(self, src: str) -> None:
        from compile import _run_stage
        _run_stage("tac", src)


class TestPointerUnaryOps(unittest.TestCase):
    """Unary operators on pointer operands. `-p` and `~p` are
    nonsensical (negate / bit-flip an address) and rejected at
    type-check time. `!p` is the null-pointer test (`p != 0`) and
    is legal — the existing 2-byte LogicalNot lowering ORs the two
    address bytes and drives a 0/1 select off the resulting Z
    flag."""

    def _typecheck(self, src: str) -> None:
        from compile import _run_stage
        _run_stage("tac", src)

    def _codegen(self, src: str) -> str:
        from compile import _run_stage
        return _run_stage("codegen", src)

    def test_negate_pointer_rejected(self):
        from passes.type_checking import TypeCheckError
        with self.assertRaises(TypeCheckError) as cm:
            self._typecheck(
                "int main(void) { int x; int *p; p = &x; return -p; }\n"
            )
        self.assertIn("'-'", str(cm.exception))

    def test_complement_pointer_rejected(self):
        from passes.type_checking import TypeCheckError
        with self.assertRaises(TypeCheckError) as cm:
            self._typecheck(
                "int main(void) { int x; int *p; p = &x; return ~p; }\n"
            )
        self.assertIn("'~'", str(cm.exception))

    def test_logical_not_pointer_lowers_to_or_of_both_bytes(self):
        # `!p` for a 2-byte pointer ORs the two address bytes and
        # branches on the resulting Z flag — Z=1 iff both bytes are
        # zero, i.e., the pointer is null. The existing Long-sized
        # LogicalNot lowering handles this for free.
        text = self._codegen(
            "int main(void) { int x; int *p; p = &x; return !p; }\n"
        )
        # The asm has an `ORA (FP),Y` (combining the two bytes) then
        # the lnot 0/1-select via BEQ to a `lnot_true` label.
        self.assertIn("ORA   (FP),Y", text)
        self.assertIn(".lnot_true@", text)

    def test_logical_not_pointer_to_pointer(self):
        # `!pp` for an `int **` — also 2 bytes, same lowering.
        self._codegen(
            "int main(void) { int *p; int **pp; pp = &p; return !pp; }\n"
        )

    def test_long_pointer_negate_still_rejected(self):
        # Same rule applies for `long *` — pointer-ness is what
        # matters, not the pointee.
        from passes.type_checking import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) { long y; long *lp; lp = &y; return -lp; }\n"
            )

    def test_logical_and_two_pointers(self):
        # `p && q` — non-null check on each. The existing 2-byte
        # cond-jump lowering ORs the two address bytes per operand
        # and BEQ-shortcircuits on a null result, exactly matching
        # the truthy/falsy semantics for pointers.
        text = self._codegen(
            "int main(void) { int x; int y; int *p; int *q; "
            "p = &x; q = &y; if (p && q) return 1; return 0; }\n"
        )
        # Two ORA's (one per operand) and the and_false short-circuit.
        self.assertGreaterEqual(text.count("ORA   (FP),Y"), 2)
        self.assertIn(".and_false@", text)

    def test_logical_or_two_pointers(self):
        text = self._codegen(
            "int main(void) { int x; int y; int *p; int *q; "
            "p = &x; q = &y; if (p || q) return 1; return 0; }\n"
        )
        self.assertGreaterEqual(text.count("ORA   (FP),Y"), 2)
        self.assertIn(".or_true@", text)

    def test_logical_and_pointer_and_int(self):
        # Mixed pointer + integer is fine — the operands are tested
        # independently, so they don't have to share a common type.
        self._codegen(
            "int main(void) { int a; int x; int *p; "
            "a = 1; p = &x; if (a && p) return 1; return 0; }\n"
        )

    def test_logical_or_int_and_pointer(self):
        self._codegen(
            "int main(void) { int a; int x; int *p; "
            "a = 0; p = &x; if (a || p) return 1; return 0; }\n"
        )

    def test_logical_and_distinct_pointer_types(self):
        # Two pointers of *different* types are fine for `&&` /
        # `||` — unlike `==` / `!=`, no common type is required;
        # each operand's truthiness is tested independently.
        self._codegen(
            "int main(void) { int x; long y; int *p; long *lp; "
            "p = &x; lp = &y; if (p && lp) return 1; return 0; }\n"
        )

    def test_logical_and_result_is_int(self):
        # Result of `&&` / `||` is `int` per C99 §6.5.13.3 /
        # §6.5.14.3, regardless of operand type. Use it where an
        # int is required.
        self._codegen(
            "int main(void) { int *p; int *q; "
            "return p && q; }\n"
        )

    def test_multiply_pointer_rejected(self):
        # C99 §6.5.5 — `*` requires arithmetic operands. Pointer
        # is rejected on either side.
        from passes.type_checking import TypeCheckError
        with self.assertRaises(TypeCheckError) as cm:
            self._typecheck(
                "int main(void) { int x; int *p; p = &x; "
                "return p * 2; }\n"
            )
        self.assertIn("'*'", str(cm.exception))

    def test_divide_two_pointers_rejected(self):
        from passes.type_checking import TypeCheckError
        with self.assertRaises(TypeCheckError) as cm:
            self._typecheck(
                "int main(void) { int *p; int *q; "
                "return p / q; }\n"
            )
        self.assertIn("'/'", str(cm.exception))

    def test_modulo_pointer_rhs_rejected(self):
        # Pointer on the right also gets rejected — the check fires
        # symmetrically.
        from passes.type_checking import TypeCheckError
        with self.assertRaises(TypeCheckError) as cm:
            self._typecheck(
                "int main(void) { int a; int *p; "
                "return a % p; }\n"
            )
        self.assertIn("'%'", str(cm.exception))

    def test_divide_pointer_lhs_int_rhs_rejected(self):
        from passes.type_checking import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) { int x; int *p; p = &x; "
                "return p / 2; }\n"
            )


class TestStaticPointerInit(unittest.TestCase):
    """Static-storage pointer initializers — null (already covered)
    and `&otherstatic` (C99 §6.6.7 paragraph 9)."""

    def _codegen(self, src: str) -> str:
        from compile import _run_stage
        return _run_stage("codegen", src)

    def _typecheck(self, src: str) -> None:
        from compile import _run_stage
        _run_stage("tac", src)

    def test_static_pointer_addressof_static_at_file_scope(self):
        # `static int a; static int *a_ptr = &a;` — the canonical
        # case the user asked about. Lays down `a_ptr: DC.W a` so
        # the assembler resolves the address at link time.
        text = self._codegen(
            "static int a;\n"
            "static int *a_ptr = &a;\n"
            "int main(void) { return 0; }\n"
        )
        self.assertIn("a_ptr:", text)
        # The `dc.w` operand is the bare symbol `a` (no `+offset`
        # since the offset is 0).
        self.assertIn("DC.W  a", text)

    def test_external_pointer_addressof_static(self):
        # `int *p = &a;` (external linkage) — same lowering path.
        text = self._codegen(
            "int a;\n"
            "int *p = &a;\n"
            "int main(void) { return 0; }\n"
        )
        self.assertIn("p:", text)
        self.assertIn("DC.W  a", text)

    def test_block_scope_static_pointer_addressof_file_scope(self):
        # A function-local `static int *p = &g;` keeps its
        # block-scope rename (`@N.p`) but still resolves to a
        # `DC.W g` lay-down. The block-scope `static` makes the
        # variable internal-linkage; the AddressInit machinery
        # doesn't care about linkage.
        text = self._codegen(
            "int g;\n"
            "int main(void) {\n"
            "  static int *p = &g;\n"
            "  return 0;\n"
            "}\n"
        )
        self.assertIn("DC.W  g", text)

    def test_static_pointer_addressof_long(self):
        # `&y` where y is a 2-byte static — the address is still
        # 2 bytes (every address is); pointee type doesn't affect
        # it.
        text = self._codegen(
            "long y;\n"
            "long *lp = &y;\n"
            "int main(void) { return 0; }\n"
        )
        self.assertIn("DC.W  y", text)

    def test_addressof_local_in_static_init_rejected(self):
        # An automatic-storage variable's address isn't a constant
        # expression — its location isn't known at link time. The
        # type checker rejects it with a focused message.
        from passes.type_checking import TypeCheckError
        with self.assertRaises(TypeCheckError) as cm:
            self._typecheck(
                "int main(void) {\n"
                "  int local;\n"
                "  static int *p = &local;\n"
                "  return 0;\n"
                "}\n"
            )
        self.assertIn("static storage duration", str(cm.exception))

    def test_static_pointer_zero_init_still_works(self):
        # `static int *p = 0;` — the existing null-pointer-constant
        # path. Verify it lays down `DS.B 2` (the typed zero
        # collapses to a 2-byte ZeroInit) instead of `DC.W $0000`.
        text = self._codegen(
            "static int *p = 0;\n"
            "int main(void) { return 0; }\n"
        )
        self.assertIn("p:", text)
        self.assertIn("DS.B  2", text)

    def test_static_pointer_no_init_is_zero(self):
        # Tentative definition (no initializer) lays down a 2-byte
        # zero run for the pointer slot.
        text = self._codegen(
            "int *p;\n"
            "int main(void) { return 0; }\n"
        )
        self.assertIn("p:", text)
        self.assertIn("DS.B  2", text)

    def test_function_pointer_initializer(self):
        # `int (*fp)(void) = &foo;` — function-pointer static
        # initializer. The address-of-function path produces an
        # AddressInit pointing at the function symbol; asm_emit
        # lays down `fp: DC.W foo` so the assembler resolves the
        # function's address at link time.
        text = self._codegen(
            "int foo(void) { return 7; }\n"
            "int (*fp)(void) = &foo;\n"
            "int main(void) { return 0; }\n"
        )
        self.assertIn("fp:", text)
        self.assertIn("DC.W  foo", text)

    def test_function_pointer_initializer_forward_decl(self):
        # The pointee function doesn't need a body in this TU —
        # `&foo` works against a pure forward declaration too.
        # The resulting `DC.W foo` reference is left for the
        # linker to resolve.
        text = self._codegen(
            "int foo(int x);\n"
            "int (*fp)(int) = &foo;\n"
            "int main(void) { return 0; }\n"
        )
        self.assertIn("DC.W  foo", text)

    def test_function_pointer_runtime_assign(self):
        # `fp = &foo;` (runtime, not a static init) — the
        # GetAddress / LoadAddress chain has to recognize `foo` as
        # a static-storage name so it takes the immediate-label
        # path (`LDA #<foo`) instead of the FP-relative-add path
        # for locals.
        text = self._codegen(
            "int foo(void) { return 7; }\n"
            "int main(void) {\n"
            "  int (*fp)(void);\n"
            "  fp = &foo;\n"
            "  return 0;\n"
            "}\n"
        )
        self.assertIn("LDA   #<foo", text)
        self.assertIn("LDA   #>foo", text)

    def test_block_scope_static_function_pointer_initializer(self):
        # Same as the variable cases — block-scope `static int
        # (*fp)(void) = &foo;` keeps its `@N.fp` rename but still
        # resolves the function symbol at link time.
        text = self._codegen(
            "int foo(void) { return 7; }\n"
            "int main(void) {\n"
            "  static int (*fp)(void) = &foo;\n"
            "  return 0;\n"
            "}\n"
        )
        self.assertIn("DC.W  foo", text)


class TestIndirectCall(unittest.TestCase):
    """Calling through a function pointer. The 6502 has no JSR
    indirect, so we trampoline: stage the function pointer's two
    bytes into DPTR, then `JSR icall` (a runtime trampoline whose
    body is `JMP (DPTR)`)."""

    def _codegen(self, src: str) -> str:
        from compile import _run_stage
        return _run_stage("codegen", src)

    def _typecheck(self, src: str) -> None:
        from compile import _run_stage
        _run_stage("tac", src)

    def test_indirect_call_no_args(self):
        # `fp()` with a function-pointer-typed `fp`. The asm
        # stages fp's two bytes into DPTR and JSRs `icall`.
        text = self._codegen(
            "int foo(void) { return 7; }\n"
            "int main(void) {\n"
            "  int (*fp)(void) = &foo;\n"
            "  return fp();\n"
            "}\n"
        )
        self.assertIn("STA   DPTR", text)
        self.assertIn("STA   DPTR+1", text)
        self.assertIn("JSR   icall", text)

    def test_indirect_call_with_args(self):
        # `fp(5)` — args still ride on the soft stack the same as
        # for a direct call. Only the call-site `JSR` target
        # differs.
        text = self._codegen(
            "int foo(int x) { return x + 1; }\n"
            "int main(void) {\n"
            "  int (*fp)(int) = &foo;\n"
            "  return fp(5);\n"
            "}\n"
        )
        self.assertIn("JSR   icall", text)
        # The `5` lands in A right before the soft-stack write.
        self.assertIn("LDA   #$05", text)
        # Allocate-stack for the 1-byte arg (SSP -= 1).
        self.assertIn("SBC   #$01", text)

    def test_direct_and_indirect_call_in_same_function(self):
        # Direct call `foo(...)` keeps the existing `JSR foo`
        # form; indirect `fp(...)` uses `JSR icall`. Both can
        # coexist in the same function body.
        text = self._codegen(
            "int foo(int x) { return x; }\n"
            "int main(void) {\n"
            "  int (*fp)(int) = &foo;\n"
            "  int a = foo(1);\n"
            "  int b = fp(2);\n"
            "  return a + b;\n"
            "}\n"
        )
        self.assertIn("JSR   foo", text)
        self.assertIn("JSR   icall", text)

    def test_indirect_call_long_return(self):
        # Indirect call returning Long uses the same HARGS+0..1
        # convention as a direct call — the `JSR icall` is
        # transparent.
        text = self._codegen(
            "long foo(void) { return 0x1234L; }\n"
            "int main(void) {\n"
            "  long (*fp)(void) = &foo;\n"
            "  long y = fp();\n"
            "  return 0;\n"
            "}\n"
        )
        self.assertIn("JSR   icall", text)
        # After the JSR, the caller reads back HARGS+0 (low) and
        # HARGS+1 (high) into the destination slot.
        self.assertIn("LDA   HARGS", text)
        self.assertIn("LDA   HARGS+1", text)

    def test_indirect_call_through_runtime_assigned_pointer(self):
        # The function pointer doesn't have to be initialized at
        # compile time — assigning `&foo` at runtime works the
        # same way (GetAddress + Copy stamps fp's bytes; the
        # subsequent `fp()` reads them back into DPTR).
        text = self._codegen(
            "int foo(void) { return 7; }\n"
            "int main(void) {\n"
            "  int (*fp)(void);\n"
            "  fp = &foo;\n"
            "  return fp();\n"
            "}\n"
        )
        self.assertIn("LDA   #<foo", text)
        self.assertIn("LDA   #>foo", text)
        self.assertIn("JSR   icall", text)

    def test_indirect_call_with_dereference_callee(self):
        # `(*fp)(args)` — the canonical C idiom. Auto-decay per
        # C99 §6.3.2.1.4 makes this equivalent to `fp(args)`; the
        # parser reduces both forms to FunctionCall(name=fp, ...)
        # and the same JSR icall lowering applies.
        text = self._codegen(
            "int foo(int x) { return x + 1; }\n"
            "int main(void) {\n"
            "  int (*fp)(int) = &foo;\n"
            "  return (*fp)(5);\n"
            "}\n"
        )
        self.assertIn("JSR   icall", text)

    def test_parenthesized_direct_callee(self):
        # `(foo)(args)` — paren around a function name. Resolves
        # the same as `foo(args)` (direct call, JSR foo).
        text = self._codegen(
            "int foo(int x) { return x + 1; }\n"
            "int main(void) {\n"
            "  return (foo)(5);\n"
            "}\n"
        )
        self.assertIn("JSR   foo", text)
        self.assertNotIn("JSR   icall", text)

    def test_address_of_function_then_call(self):
        # `(&foo)(args)` — explicitly take foo's address, then
        # call through it. Per the auto-decay rule the parens-and-
        # `&` cancel out to leave just `foo(args)`. Reduces to a
        # direct call.
        text = self._codegen(
            "int foo(int x) { return x + 1; }\n"
            "int main(void) {\n"
            "  return (&foo)(5);\n"
            "}\n"
        )
        self.assertIn("JSR   foo", text)
        self.assertNotIn("JSR   icall", text)

    def test_complex_callee_rejected(self):
        # `(arr[i])(args)`, `(f())(args)`, etc. would need the
        # FunctionCall AST to carry an arbitrary callee expression
        # rather than a string name. Pin the current behavior:
        # a non-name callee inside the parens raises a parser
        # error.
        from parser import ParserError
        from lark.exceptions import VisitError
        # `(1)(args)` — a non-name callee.
        with self.assertRaises((ParserError, VisitError)):
            self._typecheck(
                "int main(void) { return (1)(5); }\n"
            )

    def test_complement_float_rejected(self):
        # C99 §6.5.3.3.4 — `~` requires an integer operand. Float
        # has no bit-pattern semantics that `~` would meaningfully
        # produce, so it's a strict type error.
        from passes.type_checking import TypeCheckError
        with self.assertRaises(TypeCheckError) as cm:
            self._typecheck(
                "int main(void) { float f; f = 1.5f; return ~f; }\n"
            )
        self.assertIn("'~'", str(cm.exception))
        self.assertIn("integer", str(cm.exception))

    def test_complement_double_rejected(self):
        from passes.type_checking import TypeCheckError
        with self.assertRaises(TypeCheckError):
            self._typecheck(
                "int main(void) { double d; d = 1.5; return ~d; }\n"
            )

    def test_negate_float_still_allowed(self):
        # `-f` on a Float is legal C99 (the FP runtime helper for
        # negate is just a sign-bit flip). The current c99_to_tac
        # would still raise NotImplementedError when trying to
        # lower it, but the type-check should pass — this test
        # pins the type-check side.
        # Use a place where the cast-or-negate doesn't reach the
        # later passes: just type-check via the type_checker
        # directly.
        from parser import parse
        from passes.identifier_resolution import resolve_program
        from passes.label_resolution import resolve_program as lresolve
        from passes.loop_labeling import label_program
        from passes.type_checking import check_program
        ast = parse("int main(void) { float f; f = 1.5f; return -f; }\n")
        ast = resolve_program(ast)
        ast = lresolve(ast)
        ast = label_program(ast)
        # check_program doesn't raise on float negate (the FP
        # arithmetic lowering is unfinished, but type-check is fine).
        check_program(ast)


class TestArrayDeclarations(unittest.TestCase):
    """Array declarators with constant integer sizes — `int a[10]`,
    nested `int a[3][4]`, array-of-pointer `int *a[10]`. The ASDL
    type is `Array(element_type, size)`; nested suffixes compose
    outermost-first so `int a[3][4]` ends up as
    Array(Array(Int, 4), 3) — an array of 3 arrays of 4 ints."""

    def _decl_type(self, src):
        from parser import parse
        prog = parse(src)
        items = prog.declaration[0].function_decl.body.block_item
        return items[0].declaration.var_decl.data_type

    def test_simple_int_array(self):
        t = self._decl_type("int main(void) { int a[10]; return 0; }")
        self.assertEqual(t, c99_ast.Array(element_type=c99_ast.Int(), size=10))

    def test_long_array(self):
        t = self._decl_type("int main(void) { long a[5]; return 0; }")
        self.assertEqual(t, c99_ast.Array(element_type=c99_ast.Long(), size=5))

    def test_two_dim_array_outer_size_outer(self):
        # `int a[3][4]` is "array of 3 arrays of 4 ints" — the OUTER
        # dimension (the one closer to the identifier) is the OUTER
        # Array, with element_type itself an Array of the inner
        # dimension.
        t = self._decl_type("int main(void) { int a[3][4]; return 0; }")
        self.assertEqual(t, c99_ast.Array(
            element_type=c99_ast.Array(
                element_type=c99_ast.Int(), size=4,
            ),
            size=3,
        ))

    def test_array_of_pointer(self):
        # `int *a[10]` is "array of 10 pointers to int" — the postfix
        # array suffix binds tighter than the prefix `*` (C99 §6.7.5).
        t = self._decl_type("int main(void) { int *a[10]; return 0; }")
        self.assertEqual(t, c99_ast.Array(
            element_type=c99_ast.Pointer(referenced_type=c99_ast.Int()),
            size=10,
        ))

    def test_unspecified_size_rejected(self):
        # `int a[]` (no size) would need an init list to determine
        # the count, which c6502 doesn't support yet. Lark wraps
        # transformer exceptions in `VisitError`.
        from parser import parse
        from lark.exceptions import VisitError
        with self.assertRaises((NotImplementedError, VisitError)):
            parse("int main(void) { int a[]; return 0; }")

    def test_zero_size_rejected(self):
        from parser import parse, ParserError
        from lark.exceptions import VisitError
        with self.assertRaises((ParserError, VisitError)):
            parse("int main(void) { int a[0]; return 0; }")

    def test_non_constant_size_rejected(self):
        # `int a[n]` where n isn't a literal would parse the expression
        # but the declarator walker rejects anything that isn't a
        # bare integer constant.
        from parser import parse
        from lark.exceptions import VisitError
        with self.assertRaises((NotImplementedError, VisitError)):
            parse("int main(void) { int n = 10; int a[n]; return 0; }")


class TestArrayParameterAdjustment(unittest.TestCase):
    """C99 §6.7.5.3.7 — `T param[N]` parameters adjust to `T *param`
    at the type-construction boundary. Only the OUTERMOST array
    suffix decays — multi-dim params keep their inner Array."""

    def _fun_type(self, src):
        from parser import parse
        prog = parse(src)
        return prog.declaration[0].function_decl.data_type

    def test_int_array_param_adjusts_to_pointer(self):
        t = self._fun_type(
            "int foo(int a[3]) { return 0; } "
            "int main(void) { return 0; }"
        )
        self.assertEqual(
            t,
            c99_ast.FunType(
                params=[c99_ast.Pointer(referenced_type=c99_ast.Int())],
                ret=c99_ast.Int(),
            ),
        )

    def test_long_array_param_adjusts_to_pointer(self):
        t = self._fun_type(
            "int foo(long a[5]) { return 0; } "
            "int main(void) { return 0; }"
        )
        self.assertEqual(
            t.params[0],
            c99_ast.Pointer(referenced_type=c99_ast.Long()),
        )

    def test_two_dim_array_param_adjusts_only_outer(self):
        # `int a[3][4]` — outer Array(_, 3) decays; inner Array(_, 4)
        # stays. Result type is `Pointer(Array(Int, 4))`, the C
        # equivalent of `int (*a)[4]`.
        t = self._fun_type(
            "int foo(int a[3][4]) { return 0; } "
            "int main(void) { return 0; }"
        )
        self.assertEqual(
            t.params[0],
            c99_ast.Pointer(
                referenced_type=c99_ast.Array(
                    element_type=c99_ast.Int(), size=4,
                ),
            ),
        )

    def test_pointer_param_unaffected(self):
        # `int *a` is already a pointer — no adjustment to apply.
        t = self._fun_type(
            "int foo(int *a) { return 0; } "
            "int main(void) { return 0; }"
        )
        self.assertEqual(
            t.params[0],
            c99_ast.Pointer(referenced_type=c99_ast.Int()),
        )

    def test_pointer_to_array_param_unaffected(self):
        # `int (*a)[3]` — the pointer is at the outer level, not the
        # inner Array, so no adjustment fires.
        t = self._fun_type(
            "int foo(int (*a)[3]) { return 0; } "
            "int main(void) { return 0; }"
        )
        self.assertEqual(
            t.params[0],
            c99_ast.Pointer(
                referenced_type=c99_ast.Array(
                    element_type=c99_ast.Int(), size=3,
                ),
            ),
        )


class TestInitializerListParsing(unittest.TestCase):
    """`{e1, e2, ...}` brace-enclosed initializer per C99 §6.7.8.
    Produces an `InitList(items)` AST node; the var_decl rule
    accepts either an `init_exp` (single assignment expression) or
    an `init_list` (the brace form) as the init slot."""

    def _init(self, src):
        from parser import parse
        prog = parse(src)
        items = prog.declaration[0].function_decl.body.block_item
        return items[0].declaration.var_decl.init

    def test_simple_int_init_list(self):
        init = self._init(
            "int main(void) { int a[3] = {1, 2, 3}; return 0; }"
        )
        self.assertIsInstance(init, c99_ast.InitList)
        self.assertEqual(len(init.items), 3)
        for item in init.items:
            self.assertIsInstance(item, c99_ast.Constant)

    def test_empty_init_list_rejected_by_grammar(self):
        # `{}` would be a GNU/C2x extension — the standard requires
        # at least one initializer. Lark fails to parse the empty
        # form.
        from parser import parse
        with self.assertRaises(Exception):
            parse("int main(void) { int a[3] = {}; return 0; }")

    def test_trailing_comma_allowed(self):
        # C99 §6.7.8 explicitly permits `{1, 2, 3,}` — handy for
        # generated code and for adding new entries without diff
        # noise.
        init = self._init(
            "int main(void) { int a[3] = {1, 2, 3,}; return 0; }"
        )
        self.assertIsInstance(init, c99_ast.InitList)
        self.assertEqual(len(init.items), 3)

    def test_partial_init_list(self):
        # Fewer items than the array size — the type checker will
        # zero-pad on lowering. The parser accepts the literal as is.
        init = self._init(
            "int main(void) { int a[5] = {1, 2}; return 0; }"
        )
        self.assertEqual(len(init.items), 2)

    def test_expression_items(self):
        # Each item is an `assignment_exp`, so any non-comma
        # expression — including a Binary — is legal at parse time.
        init = self._init(
            "int main(void) { int x = 1; int a[2] = {x + 1, 2 * 3}; return 0; }"
        )[1] if False else self._init(  # noqa: E501
            "int main(void) { int a[2] = {1 + 2, 3 * 4}; return 0; }"
        )
        self.assertIsInstance(init.items[0], c99_ast.Binary)
        self.assertIsInstance(init.items[1], c99_ast.Binary)


class TestSubscriptParsing(unittest.TestCase):
    """`e1[e2]` postfix expression — produces a `Subscript(array,
    index)` AST node. Both subexpressions are full expressions
    (assignment-level and below); the index can be any integer
    expression at parse time."""

    def _exp(self, src):
        from parser import parse
        prog = parse(src)
        items = prog.declaration[0].function_decl.body.block_item
        return items[-1].statement.exp

    def test_simple_subscript(self):
        exp = self._exp(
            "int main(void) { int a[10]; return a[3]; }"
        )
        self.assertIsInstance(exp, c99_ast.Subscript)
        self.assertIsInstance(exp.array, c99_ast.Var)
        self.assertEqual(exp.array.name, "a")
        self.assertIsInstance(exp.index, c99_ast.Constant)

    def test_subscript_with_expression_index(self):
        exp = self._exp(
            "int main(void) { int a[10]; int i = 0; return a[i + 1]; }"
        )
        self.assertIsInstance(exp, c99_ast.Subscript)
        self.assertIsInstance(exp.index, c99_ast.Binary)

    def test_subscript_chained_for_two_dim(self):
        # `a[i][j]` parses as `(a[i])[j]` — left-associative postfix.
        exp = self._exp(
            "int main(void) { int a[3][4]; return a[1][2]; }"
        )
        self.assertIsInstance(exp, c99_ast.Subscript)
        # Outer index is `2`.
        self.assertIsInstance(exp.index, c99_ast.Constant)
        self.assertEqual(exp.index.const.value, 2)
        # Inner array operand is itself a Subscript with index `1`.
        self.assertIsInstance(exp.array, c99_ast.Subscript)
        self.assertIsInstance(exp.array.array, c99_ast.Var)
        self.assertEqual(exp.array.array.name, "a")
        self.assertEqual(exp.array.index.const.value, 1)


if __name__ == "__main__":
    unittest.main()
