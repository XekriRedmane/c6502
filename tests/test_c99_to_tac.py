import unittest

import c99_ast
import tac_ast
from parser import parse
from c99_to_tac import Translator, translate_program


class TestTranslateExp(unittest.TestCase):
    def test_constant_returns_tac_constant_emits_nothing(self):
        t = Translator()
        instrs: list = []
        result = t.translate_exp(c99_ast.Constant(value=42), instrs)
        self.assertEqual(result, tac_ast.Constant(value=42))
        self.assertEqual(instrs, [])

    def test_unary_emits_instruction_and_returns_dst_var(self):
        t = Translator()
        instrs: list = []
        result = t.translate_exp(
            c99_ast.Unary(
                op=c99_ast.Negate(),
                exp=c99_ast.Constant(value=5),
            ),
            instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="%0"))
        self.assertEqual(
            instrs,
            [tac_ast.Unary(
                op=tac_ast.Negate(),
                src=tac_ast.Constant(value=5),
                dst=tac_ast.Var(name="%0"),
            )],
        )

    def test_nested_unary_chains_temps(self):
        t = Translator()
        instrs: list = []
        result = t.translate_exp(
            c99_ast.Unary(
                op=c99_ast.Negate(),
                exp=c99_ast.Unary(
                    op=c99_ast.Complement(),
                    exp=c99_ast.Constant(value=5),
                ),
            ),
            instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="%1"))
        self.assertEqual(instrs, [
            tac_ast.Unary(
                op=tac_ast.Complement(),
                src=tac_ast.Constant(value=5),
                dst=tac_ast.Var(name="%0"),
            ),
            tac_ast.Unary(
                op=tac_ast.Negate(),
                src=tac_ast.Var(name="%0"),
                dst=tac_ast.Var(name="%1"),
            ),
        ])

    def test_binary_emits_instruction_and_returns_dst_var(self):
        t = Translator()
        instrs: list = []
        result = t.translate_exp(
            c99_ast.Binary(
                op=c99_ast.Add(),
                left=c99_ast.Constant(value=1),
                right=c99_ast.Constant(value=2),
            ),
            instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="%0"))
        self.assertEqual(
            instrs,
            [tac_ast.Binary(
                op=tac_ast.Add(),
                src1=tac_ast.Constant(value=1),
                src2=tac_ast.Constant(value=2),
                dst=tac_ast.Var(name="%0"),
            )],
        )

    def test_each_binary_op_translates(self):
        cases = [
            (c99_ast.Add(),            tac_ast.Add()),
            (c99_ast.Subtract(),       tac_ast.Subtract()),
            (c99_ast.Multiply(),       tac_ast.Multiply()),
            (c99_ast.Divide(),         tac_ast.Divide()),
            (c99_ast.Modulo(),         tac_ast.Modulo()),
            (c99_ast.BitwiseAnd(),     tac_ast.BitwiseAnd()),
            (c99_ast.BitwiseOr(),      tac_ast.BitwiseOr()),
            (c99_ast.BitwiseXor(),     tac_ast.BitwiseXor()),
            (c99_ast.LeftShift(),      tac_ast.LeftShift()),
            (c99_ast.RightShift(),     tac_ast.RightShift()),
            (c99_ast.Equal(),          tac_ast.Equal()),
            (c99_ast.NotEqual(),       tac_ast.NotEqual()),
            (c99_ast.LessThan(),       tac_ast.LessThan()),
            (c99_ast.GreaterThan(),    tac_ast.GreaterThan()),
            (c99_ast.LessOrEqual(),    tac_ast.LessOrEqual()),
            (c99_ast.GreaterOrEqual(), tac_ast.GreaterOrEqual()),
        ]
        for c99_op, tac_op in cases:
            with self.subTest(op=type(c99_op).__name__):
                t = Translator()
                instrs: list = []
                t.translate_exp(
                    c99_ast.Binary(
                        op=c99_op,
                        left=c99_ast.Constant(value=1),
                        right=c99_ast.Constant(value=2),
                    ),
                    instrs,
                )
                self.assertEqual(instrs[0].op, tac_op)

    def test_each_unary_op_translates(self):
        cases = [
            (c99_ast.Negate(),     tac_ast.Negate()),
            (c99_ast.Complement(), tac_ast.Complement()),
            (c99_ast.LogicalNot(), tac_ast.LogicalNot()),
        ]
        for c99_op, tac_op in cases:
            with self.subTest(op=type(c99_op).__name__):
                t = Translator()
                instrs: list = []
                t.translate_exp(
                    c99_ast.Unary(op=c99_op, exp=c99_ast.Constant(value=1)),
                    instrs,
                )
                self.assertEqual(instrs[0].op, tac_op)

    def test_binary_left_translated_before_right(self):
        # A binary whose left side itself contains a Unary (which
        # allocates a temp) and whose right side is also a Unary —
        # left's temp should be %0, right's %1, and the binary's
        # destination %2.
        t = Translator()
        instrs: list = []
        result = t.translate_exp(
            c99_ast.Binary(
                op=c99_ast.Add(),
                left=c99_ast.Unary(
                    op=c99_ast.Negate(),
                    exp=c99_ast.Constant(value=1),
                ),
                right=c99_ast.Unary(
                    op=c99_ast.Negate(),
                    exp=c99_ast.Constant(value=2),
                ),
            ),
            instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="%2"))
        self.assertEqual(instrs, [
            tac_ast.Unary(
                op=tac_ast.Negate(),
                src=tac_ast.Constant(value=1),
                dst=tac_ast.Var(name="%0"),
            ),
            tac_ast.Unary(
                op=tac_ast.Negate(),
                src=tac_ast.Constant(value=2),
                dst=tac_ast.Var(name="%1"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=tac_ast.Var(name="%0"),
                src2=tac_ast.Var(name="%1"),
                dst=tac_ast.Var(name="%2"),
            ),
        ])


class TestTranslateVarAndAssignment(unittest.TestCase):
    """`Var` passes through verbatim; `Assignment` evaluates rval, then
    Copies the result into the lval Var. The translator runs after
    variable_resolution, so all `Var.name`s coming in are the unique
    `@N.orig` strings, which TAC accepts as-is."""

    def test_var_passthrough_emits_no_instructions(self):
        t = Translator()
        instrs: list = []
        result = t.translate_exp(c99_ast.Var(name="@0.x"), instrs)
        self.assertEqual(result, tac_ast.Var(name="@0.x"))
        self.assertEqual(instrs, [])

    def test_assignment_constant_to_var_emits_copy(self):
        t = Translator()
        instrs: list = []
        result = t.translate_exp(
            c99_ast.Assignment(
                lval=c99_ast.Var(name="@0.a"),
                rval=c99_ast.Constant(value=5),
            ),
            instrs,
        )
        # Returns the lval so chained assignments compose.
        self.assertEqual(result, tac_ast.Var(name="@0.a"))
        self.assertEqual(
            instrs,
            [tac_ast.Copy(
                src=tac_ast.Constant(value=5),
                dst=tac_ast.Var(name="@0.a"),
            )],
        )

    def test_assignment_var_to_var_emits_copy(self):
        t = Translator()
        instrs: list = []
        result = t.translate_exp(
            c99_ast.Assignment(
                lval=c99_ast.Var(name="@0.a"),
                rval=c99_ast.Var(name="@1.b"),
            ),
            instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="@0.a"))
        self.assertEqual(
            instrs,
            [tac_ast.Copy(
                src=tac_ast.Var(name="@1.b"),
                dst=tac_ast.Var(name="@0.a"),
            )],
        )

    def test_assignment_with_compound_rval_emits_eval_then_copy(self):
        # `a = 1 + 2` -> evaluate `1 + 2` into %0, then Copy(%0, @0.a).
        t = Translator()
        instrs: list = []
        result = t.translate_exp(
            c99_ast.Assignment(
                lval=c99_ast.Var(name="@0.a"),
                rval=c99_ast.Binary(
                    op=c99_ast.Add(),
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                ),
            ),
            instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="@0.a"))
        self.assertEqual(
            instrs,
            [
                tac_ast.Binary(
                    op=tac_ast.Add(),
                    src1=tac_ast.Constant(value=1),
                    src2=tac_ast.Constant(value=2),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Copy(
                    src=tac_ast.Var(name="%0"),
                    dst=tac_ast.Var(name="@0.a"),
                ),
            ],
        )

    def test_chained_assignment_composes_right_to_left(self):
        # `b = a = 5` after resolution:
        #   Assignment(Var(@1.b), Assignment(Var(@0.a), Constant(5)))
        # Inner assignment Copies 5 -> @0.a and returns Var(@0.a);
        # outer Copies that into @1.b. Two Copies, no temps.
        t = Translator()
        instrs: list = []
        result = t.translate_exp(
            c99_ast.Assignment(
                lval=c99_ast.Var(name="@1.b"),
                rval=c99_ast.Assignment(
                    lval=c99_ast.Var(name="@0.a"),
                    rval=c99_ast.Constant(value=5),
                ),
            ),
            instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="@1.b"))
        self.assertEqual(
            instrs,
            [
                tac_ast.Copy(
                    src=tac_ast.Constant(value=5),
                    dst=tac_ast.Var(name="@0.a"),
                ),
                tac_ast.Copy(
                    src=tac_ast.Var(name="@0.a"),
                    dst=tac_ast.Var(name="@1.b"),
                ),
            ],
        )

    def test_non_var_lval_raises_type_error(self):
        # variable_resolution should have rejected this; the runtime
        # check is defense-in-depth.
        t = Translator()
        with self.assertRaises(TypeError) as ctx:
            t.translate_exp(
                c99_ast.Assignment(
                    lval=c99_ast.Constant(value=1),
                    rval=c99_ast.Constant(value=2),
                ),
                [],
            )
        self.assertIn("Var", str(ctx.exception))


class TestTranslateBlockItems(unittest.TestCase):
    """Block-item-level lowerings: declarations (with and without
    initializer), expression statements (instructions emitted but the
    result temp is unused), and null statements (emit nothing). Tests
    drive the methods directly so they don't depend on the implicit
    Ret(0) that translate_function appends."""

    def test_bare_declaration_emits_nothing(self):
        t = Translator()
        instrs: list = []
        t.translate_declaration(
            c99_ast.Declaration(name="@0.x", init=None), instrs,
        )
        self.assertEqual(instrs, [])

    def test_initialized_declaration_emits_copy(self):
        # `int x = 5;` lowers like the assignment `x = 5`.
        t = Translator()
        instrs: list = []
        t.translate_declaration(
            c99_ast.Declaration(
                name="@0.x", init=c99_ast.Constant(value=5),
            ),
            instrs,
        )
        self.assertEqual(
            instrs,
            [tac_ast.Copy(
                src=tac_ast.Constant(value=5),
                dst=tac_ast.Var(name="@0.x"),
            )],
        )

    def test_initialized_declaration_with_compound_init(self):
        # `int x = 1 + 2;` evaluates the initializer first.
        t = Translator()
        instrs: list = []
        t.translate_declaration(
            c99_ast.Declaration(
                name="@0.x",
                init=c99_ast.Binary(
                    op=c99_ast.Add(),
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                ),
            ),
            instrs,
        )
        self.assertEqual(
            instrs,
            [
                tac_ast.Binary(
                    op=tac_ast.Add(),
                    src1=tac_ast.Constant(value=1),
                    src2=tac_ast.Constant(value=2),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Copy(
                    src=tac_ast.Var(name="%0"),
                    dst=tac_ast.Var(name="@0.x"),
                ),
            ],
        )

    def test_expression_statement_emits_inner_instructions(self):
        # `a = 5;` as a statement: the assignment emits its Copy and
        # returns Var(@0.a), which the statement discards.
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.Expression(exp=c99_ast.Assignment(
                lval=c99_ast.Var(name="@0.a"),
                rval=c99_ast.Constant(value=5),
            )),
            instrs,
        )
        self.assertEqual(
            instrs,
            [tac_ast.Copy(
                src=tac_ast.Constant(value=5),
                dst=tac_ast.Var(name="@0.a"),
            )],
        )

    def test_null_statement_emits_nothing(self):
        t = Translator()
        instrs: list = []
        t.translate_statement(c99_ast.Null(), instrs)
        self.assertEqual(instrs, [])

    def test_block_item_dispatches_to_statement(self):
        t = Translator()
        instrs: list = []
        t.translate_block_item(
            c99_ast.S(statement=c99_ast.Null()), instrs,
        )
        self.assertEqual(instrs, [])

    def test_block_item_dispatches_to_declaration(self):
        t = Translator()
        instrs: list = []
        t.translate_block_item(
            c99_ast.D(declaration=c99_ast.Declaration(
                name="@0.x", init=c99_ast.Constant(value=7),
            )),
            instrs,
        )
        self.assertEqual(
            instrs,
            [tac_ast.Copy(
                src=tac_ast.Constant(value=7),
                dst=tac_ast.Var(name="@0.x"),
            )],
        )


class TestTranslateFunctionFallThrough(unittest.TestCase):
    """translate_function appends an implicit `Ret(Constant(0))` if
    the body doesn't already end in a Ret. C99 §5.1.2.2.3 specifies
    this for `main`; we apply it generally so every TAC function
    terminates."""

    def test_empty_function_gets_implicit_return_zero(self):
        fn = c99_ast.Function(name="main", body=[])
        self.assertEqual(
            Translator().translate_function(fn),
            tac_ast.Function(
                name="main",
                instructions=[tac_ast.Ret(val=tac_ast.Constant(value=0))],
            ),
        )

    def test_body_without_return_gets_implicit_return_zero(self):
        fn = c99_ast.Function(name="main", body=[
            c99_ast.D(declaration=c99_ast.Declaration(
                name="@0.x", init=c99_ast.Constant(value=5),
            )),
        ])
        self.assertEqual(
            Translator().translate_function(fn),
            tac_ast.Function(
                name="main",
                instructions=[
                    tac_ast.Copy(
                        src=tac_ast.Constant(value=5),
                        dst=tac_ast.Var(name="@0.x"),
                    ),
                    tac_ast.Ret(val=tac_ast.Constant(value=0)),
                ],
            ),
        )

    def test_explicit_return_does_not_append_a_second_one(self):
        # If the body already ends in a Return, the implicit Ret(0)
        # would be dead code — skip it.
        fn = c99_ast.Function(name="main", body=[
            c99_ast.S(statement=c99_ast.Return(
                exp=c99_ast.Constant(value=7),
            )),
        ])
        self.assertEqual(
            Translator().translate_function(fn).instructions,
            [tac_ast.Ret(val=tac_ast.Constant(value=7))],
        )

    def test_null_after_return_does_not_trigger_second_return(self):
        # Null emits nothing, so the last emitted instruction is still
        # Ret — no implicit zero-return appended.
        fn = c99_ast.Function(name="main", body=[
            c99_ast.S(statement=c99_ast.Return(
                exp=c99_ast.Constant(value=7),
            )),
            c99_ast.S(statement=c99_ast.Null()),
        ])
        self.assertEqual(
            Translator().translate_function(fn).instructions,
            [tac_ast.Ret(val=tac_ast.Constant(value=7))],
        )

    def test_block_items_processed_in_order(self):
        # Two declarations, then a return.
        fn = c99_ast.Function(name="main", body=[
            c99_ast.D(declaration=c99_ast.Declaration(
                name="@0.a", init=c99_ast.Constant(value=1),
            )),
            c99_ast.D(declaration=c99_ast.Declaration(
                name="@1.b", init=c99_ast.Constant(value=2),
            )),
            c99_ast.S(statement=c99_ast.Return(
                exp=c99_ast.Var(name="@1.b"),
            )),
        ])
        self.assertEqual(
            Translator().translate_function(fn).instructions,
            [
                tac_ast.Copy(
                    src=tac_ast.Constant(value=1),
                    dst=tac_ast.Var(name="@0.a"),
                ),
                tac_ast.Copy(
                    src=tac_ast.Constant(value=2),
                    dst=tac_ast.Var(name="@1.b"),
                ),
                tac_ast.Ret(val=tac_ast.Var(name="@1.b")),
            ],
        )


class TestTranslateProgram(unittest.TestCase):
    def test_return_constant(self):
        prog = c99_ast.Program(function_definition=c99_ast.Function(
            name="main",
            body=[c99_ast.S(statement=c99_ast.Return(
                exp=c99_ast.Constant(value=42),
            ))],
        ))
        self.assertEqual(
            translate_program(prog),
            tac_ast.Program(function_definition=tac_ast.Function(
                name="main",
                instructions=[tac_ast.Ret(val=tac_ast.Constant(value=42))],
            )),
        )

    def test_return_unary(self):
        tac = translate_program(c99_ast.Program(
            function_definition=c99_ast.Function(
                name="main",
                body=[c99_ast.S(statement=c99_ast.Return(exp=c99_ast.Unary(
                    op=c99_ast.Negate(),
                    exp=c99_ast.Constant(value=5),
                )))],
            ),
        ))
        self.assertEqual(
            tac.function_definition.instructions,
            [
                tac_ast.Unary(
                    op=tac_ast.Negate(),
                    src=tac_ast.Constant(value=5),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Ret(val=tac_ast.Var(name="%0")),
            ],
        )

    def test_end_to_end_nested_from_source(self):
        tac = translate_program(parse("int main(void) { return -(~5); }"))
        self.assertEqual(
            tac.function_definition.instructions,
            [
                tac_ast.Unary(
                    op=tac_ast.Complement(),
                    src=tac_ast.Constant(value=5),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Unary(
                    op=tac_ast.Negate(),
                    src=tac_ast.Var(name="%0"),
                    dst=tac_ast.Var(name="%1"),
                ),
                tac_ast.Ret(val=tac_ast.Var(name="%1")),
            ],
        )

    def test_compound_assignment_lowers_like_desugared_form(self):
        # `a += 1` is desugared by the parser to `a = a + 1`. The TAC
        # is therefore: read `a` and `1` into a Binary(Add) producing
        # %0, then Copy %0 back into a. The implicit `Ret(0)` from
        # translate_function tails it. (No variable_resolution here, so
        # the name stays as user-written `a` rather than `@0.a`.)
        tac = translate_program(parse(
            "int main(void) { int a; a += 1; }"
        ))
        self.assertEqual(
            tac.function_definition.instructions,
            [
                tac_ast.Binary(
                    op=tac_ast.Add(),
                    src1=tac_ast.Var(name="a"),
                    src2=tac_ast.Constant(value=1),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Copy(
                    src=tac_ast.Var(name="%0"),
                    dst=tac_ast.Var(name="a"),
                ),
                tac_ast.Ret(val=tac_ast.Constant(value=0)),
            ],
        )

    def test_end_to_end_binary_precedence(self):
        # 1 + 2 * 3 — the parser puts Multiply on the right of Add.
        # c99_to_tac translates left first, so the constant 1 is
        # the first operand to flow through; then the multiply (which
        # allocates %0); then the add (allocating %1).
        tac = translate_program(parse("int main(void) { return 1 + 2 * 3; }"))
        self.assertEqual(
            tac.function_definition.instructions,
            [
                tac_ast.Binary(
                    op=tac_ast.Multiply(),
                    src1=tac_ast.Constant(value=2),
                    src2=tac_ast.Constant(value=3),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Binary(
                    op=tac_ast.Add(),
                    src1=tac_ast.Constant(value=1),
                    src2=tac_ast.Var(name="%0"),
                    dst=tac_ast.Var(name="%1"),
                ),
                tac_ast.Ret(val=tac_ast.Var(name="%1")),
            ],
        )

    def test_counter_resets_per_translate_program_call(self):
        # translate_program uses a fresh Translator each call, so both
        # programs get %0 as the first temporary.
        src = "int main(void) { return -5; }"
        a = translate_program(parse(src))
        b = translate_program(parse(src))
        self.assertEqual(a.function_definition.instructions[0].dst,
                         tac_ast.Var(name="%0"))
        self.assertEqual(b.function_definition.instructions[0].dst,
                         tac_ast.Var(name="%0"))


class TestMakeTemporaryVariableName(unittest.TestCase):
    def test_sequential_counter(self):
        t = Translator()
        self.assertEqual(t.make_temporary_variable_name(), "%0")
        self.assertEqual(t.make_temporary_variable_name(), "%1")
        self.assertEqual(t.make_temporary_variable_name(), "%2")


if __name__ == "__main__":
    unittest.main()
