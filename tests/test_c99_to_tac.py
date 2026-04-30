import unittest

import c99_ast
import tac_ast
from parser import parse
from c99_to_tac import Translator, translate_program


def _c99_program_of(*functions) -> c99_ast.Type_program:
    """Wrap one or more legacy-shape `c99_ast.Function` nodes into
    a new-shape `c99_ast.Program(declaration=[FunctionDecl(...)])`.
    Lets tests build synthetic programs without spelling out the
    triple-nested wrap (`Program → declaration → FunctionDecl →
    function_decl`) every time. The legacy `Function` node still
    exists in c99_ast (kept for transitional convenience even though
    the parser no longer emits it); we lift each one into a
    `Type_function_decl` with `body` set and `storage_class=None`."""
    decls: list[c99_ast.Type_declaration] = []
    for fn in functions:
        ftype = c99_ast.FunType(
            params=[c99_ast.Int() for _ in fn.params],
            ret=c99_ast.Int(),
        )
        decls.append(c99_ast.FunctionDecl(
            function_decl=c99_ast.Type_function_decl(
                name=fn.name,
                params=list(fn.params),
                body=fn.body,
                data_type=ftype,
                storage_class=None,
            ),
        ))
    return c99_ast.Program(declaration=decls)


class TestTranslateExp(unittest.TestCase):
    def test_constant_returns_tac_constant_emits_nothing(self):
        t = Translator()
        instrs: list = []
        result = t.translate_exp(c99_ast.Constant(const=c99_ast.ConstInt(int=42)), instrs)
        self.assertEqual(result, tac_ast.Constant(const=tac_ast.ConstInt(int=42)))
        self.assertEqual(instrs, [])

    def test_unary_emits_instruction_and_returns_dst_var(self):
        t = Translator()
        instrs: list = []
        result = t.translate_exp(
            c99_ast.Unary(
                op=c99_ast.Negate(),
                exp=c99_ast.Constant(const=c99_ast.ConstInt(int=5)),
            ),
            instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="%0"))
        self.assertEqual(
            instrs,
            [tac_ast.Unary(
                op=tac_ast.Negate(),
                src=tac_ast.Constant(const=tac_ast.ConstInt(int=5)),
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
                    exp=c99_ast.Constant(const=c99_ast.ConstInt(int=5)),
                ),
            ),
            instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="%1"))
        self.assertEqual(instrs, [
            tac_ast.Unary(
                op=tac_ast.Complement(),
                src=tac_ast.Constant(const=tac_ast.ConstInt(int=5)),
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
                left=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                right=c99_ast.Constant(const=c99_ast.ConstInt(int=2)),
            ),
            instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="%0"))
        self.assertEqual(
            instrs,
            [tac_ast.Binary(
                op=tac_ast.Add(),
                src1=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                src2=tac_ast.Constant(const=tac_ast.ConstInt(int=2)),
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
                        left=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                        right=c99_ast.Constant(const=c99_ast.ConstInt(int=2)),
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
                    c99_ast.Unary(op=c99_op, exp=c99_ast.Constant(const=c99_ast.ConstInt(int=1))),
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
                    exp=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                ),
                right=c99_ast.Unary(
                    op=c99_ast.Negate(),
                    exp=c99_ast.Constant(const=c99_ast.ConstInt(int=2)),
                ),
            ),
            instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="%2"))
        self.assertEqual(instrs, [
            tac_ast.Unary(
                op=tac_ast.Negate(),
                src=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                dst=tac_ast.Var(name="%0"),
            ),
            tac_ast.Unary(
                op=tac_ast.Negate(),
                src=tac_ast.Constant(const=tac_ast.ConstInt(int=2)),
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
    identifier_resolution, so all `Var.name`s coming in are the unique
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
                rval=c99_ast.Constant(const=c99_ast.ConstInt(int=5)),
            ),
            instrs,
        )
        # Returns the lval so chained assignments compose.
        self.assertEqual(result, tac_ast.Var(name="@0.a"))
        self.assertEqual(
            instrs,
            [tac_ast.Copy(
                src=tac_ast.Constant(const=tac_ast.ConstInt(int=5)),
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
                    left=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(int=2)),
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
                    src1=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                    src2=tac_ast.Constant(const=tac_ast.ConstInt(int=2)),
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
                    rval=c99_ast.Constant(const=c99_ast.ConstInt(int=5)),
                ),
            ),
            instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="@1.b"))
        self.assertEqual(
            instrs,
            [
                tac_ast.Copy(
                    src=tac_ast.Constant(const=tac_ast.ConstInt(int=5)),
                    dst=tac_ast.Var(name="@0.a"),
                ),
                tac_ast.Copy(
                    src=tac_ast.Var(name="@0.a"),
                    dst=tac_ast.Var(name="@1.b"),
                ),
            ],
        )

    def test_non_var_lval_raises_type_error(self):
        # identifier_resolution should have rejected this; the runtime
        # check is defense-in-depth.
        t = Translator()
        with self.assertRaises(TypeError) as ctx:
            t.translate_exp(
                c99_ast.Assignment(
                    lval=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                    rval=c99_ast.Constant(const=c99_ast.ConstInt(int=2)),
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
            c99_ast.VarDecl(var_decl=c99_ast.Type_var_decl(
                name="@0.x", init=None,
                data_type=c99_ast.Int(),
            )),
            instrs,
        )
        self.assertEqual(instrs, [])

    def test_function_decl_emits_nothing(self):
        # A FunctionDecl block-item is a name-binding artifact for
        # identifier_resolution. By the time TAC translation runs it
        # has no runtime effect; the lowering emits zero instructions.
        t = Translator()
        instrs: list = []
        t.translate_declaration(
            c99_ast.FunctionDecl(function_decl=c99_ast.Type_function_decl(
                name="foo", params=[], body=None,
                data_type=c99_ast.FunType(params=[], ret=c99_ast.Int()),
            )),
            instrs,
        )
        self.assertEqual(instrs, [])

    def test_initialized_declaration_emits_copy(self):
        # `int x = 5;` lowers like the assignment `x = 5`.
        t = Translator()
        instrs: list = []
        t.translate_declaration(
            c99_ast.VarDecl(var_decl=c99_ast.Type_var_decl(
                name="@0.x", init=c99_ast.Constant(const=c99_ast.ConstInt(int=5)),
                data_type=c99_ast.Int(),
            )),
            instrs,
        )
        self.assertEqual(
            instrs,
            [tac_ast.Copy(
                src=tac_ast.Constant(const=tac_ast.ConstInt(int=5)),
                dst=tac_ast.Var(name="@0.x"),
            )],
        )

    def test_initialized_declaration_with_compound_init(self):
        # `int x = 1 + 2;` evaluates the initializer first.
        t = Translator()
        instrs: list = []
        t.translate_declaration(
            c99_ast.VarDecl(var_decl=c99_ast.Type_var_decl(
                name="@0.x",
                init=c99_ast.Binary(
                    op=c99_ast.Add(),
                    left=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(int=2)),
                ),
                data_type=c99_ast.Int(),
            )),
            instrs,
        )
        self.assertEqual(
            instrs,
            [
                tac_ast.Binary(
                    op=tac_ast.Add(),
                    src1=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                    src2=tac_ast.Constant(const=tac_ast.ConstInt(int=2)),
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
                rval=c99_ast.Constant(const=c99_ast.ConstInt(int=5)),
            )),
            instrs,
        )
        self.assertEqual(
            instrs,
            [tac_ast.Copy(
                src=tac_ast.Constant(const=tac_ast.ConstInt(int=5)),
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
            c99_ast.D(declaration=c99_ast.VarDecl(
                var_decl=c99_ast.Type_var_decl(
                    name="@0.x", init=c99_ast.Constant(const=c99_ast.ConstInt(int=7)),
                    data_type=c99_ast.Int(),
                ),
            )),
            instrs,
        )
        self.assertEqual(
            instrs,
            [tac_ast.Copy(
                src=tac_ast.Constant(const=tac_ast.ConstInt(int=7)),
                dst=tac_ast.Var(name="@0.x"),
            )],
        )


class TestTranslateIfStatement(unittest.TestCase):
    """`if` lowers to JumpIfFalse + Label (no else) or JumpIfFalse +
    Jump + two Labels (with else). The labels share the Translator's
    label counter with the short-circuit and inline-comparison
    lowerings."""

    def test_if_without_else_emits_jump_around_then(self):
        # `if (1) return 2;` -> JumpIfFalse(1, end); Ret(2); Label(end)
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.IfStmt(
                condition=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                then_clause=c99_ast.Return(exp=c99_ast.Constant(const=c99_ast.ConstInt(int=2))),
                else_clause=None,
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                target=".if_end@0",
            ),
            tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=2))),
            tac_ast.Label(name=".if_end@0"),
        ])

    def test_if_with_else_emits_split_branches(self):
        # `if (1) return 2; else return 3;`:
        #   JumpIfFalse(1, else_label); Ret(2); Jump(end_label);
        #   Label(else_label); Ret(3); Label(end_label)
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.IfStmt(
                condition=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                then_clause=c99_ast.Return(exp=c99_ast.Constant(const=c99_ast.ConstInt(int=2))),
                else_clause=c99_ast.Return(exp=c99_ast.Constant(const=c99_ast.ConstInt(int=3))),
            ),
            instrs,
        )
        # end_label is minted before else_label (translate_exp doesn't
        # mint labels for a Constant, so the first make_label call is
        # for if_end -> if_end@0, then if_else -> if_else@1).
        self.assertEqual(instrs, [
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                target=".if_else@1",
            ),
            tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=2))),
            tac_ast.Jump(target=".if_end@0"),
            tac_ast.Label(name=".if_else@1"),
            tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=3))),
            tac_ast.Label(name=".if_end@0"),
        ])

    def test_nested_if_each_gets_unique_labels(self):
        # `if (1) if (2) return 3;` — outer mints if_end@0, inner mints
        # if_end@1.
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.IfStmt(
                condition=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                then_clause=c99_ast.IfStmt(
                    condition=c99_ast.Constant(const=c99_ast.ConstInt(int=2)),
                    then_clause=c99_ast.Return(
                        exp=c99_ast.Constant(const=c99_ast.ConstInt(int=3)),
                    ),
                    else_clause=None,
                ),
                else_clause=None,
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                target=".if_end@0",
            ),
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(const=tac_ast.ConstInt(int=2)),
                target=".if_end@1",
            ),
            tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=3))),
            tac_ast.Label(name=".if_end@1"),
            tac_ast.Label(name=".if_end@0"),
        ])

    def test_if_with_var_condition_evaluates_first(self):
        # The condition is evaluated for its result before the
        # JumpIfFalse fires. With a Var, no extra instructions — the
        # JumpIfFalse takes the Var directly.
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.IfStmt(
                condition=c99_ast.Var(name="a"),
                then_clause=c99_ast.Null(),
                else_clause=None,
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.JumpIfFalse(
                condition=tac_ast.Var(name="a"),
                target=".if_end@0",
            ),
            tac_ast.Label(name=".if_end@0"),
        ])


class TestTranslateGotoAndLabeled(unittest.TestCase):
    """`goto label;` lowers to a TAC `Jump(label)`. `label: stmt`
    emits a TAC `Label(label)` then lowers the inner statement.
    Label names come in pre-mangled by label_resolution
    (`.<funcname>@<orig>`); the translator just passes them through."""

    def test_goto_emits_jump(self):
        t = Translator()
        instrs: list = []
        t.translate_statement(c99_ast.Goto(label=".main@foo"), instrs)
        self.assertEqual(instrs, [tac_ast.Jump(target=".main@foo")])

    def test_labeled_statement_emits_label_then_inner(self):
        # `foo: return 0;` -> Label(".main@foo"); Ret(0).
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.LabeledStmt(
                label=".main@foo",
                statement=c99_ast.Return(exp=c99_ast.Constant(const=c99_ast.ConstInt(int=0))),
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Label(name=".main@foo"),
            tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=0))),
        ])

    def test_labeled_null_statement_emits_just_the_label(self):
        # `foo: ;` -> Label(".main@foo") and nothing else, since Null
        # itself emits nothing.
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.LabeledStmt(label=".main@foo", statement=c99_ast.Null()),
            instrs,
        )
        self.assertEqual(instrs, [tac_ast.Label(name=".main@foo")])

    def test_nested_labeled_statements(self):
        # `a: b: ;` -> Label(".main@a"); Label(".main@b").
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.LabeledStmt(
                label=".main@a",
                statement=c99_ast.LabeledStmt(
                    label=".main@b", statement=c99_ast.Null(),
                ),
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Label(name=".main@a"),
            tac_ast.Label(name=".main@b"),
        ])

    def test_goto_in_function_body(self):
        # End-to-end through translate_function: `int main(void) {
        # foo: goto foo; }` -> Label, Jump, then implicit Ret(0).
        fn = c99_ast.Function(name="main", body=c99_ast.Block(block_item=[
            c99_ast.S(statement=c99_ast.LabeledStmt(
                label=".main@foo",
                statement=c99_ast.Goto(label=".main@foo"),
            )),
        ]))
        self.assertEqual(
            Translator().translate_function(fn).instructions,
            [
                tac_ast.Label(name=".main@foo"),
                tac_ast.Jump(target=".main@foo"),
                tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=0))),
            ],
        )


class TestTranslateCompound(unittest.TestCase):
    """`Compound(block)` lowers as if its block items were inlined
    into the surrounding instruction stream — TAC is flat, so block
    boundaries don't survive into the IR. Variable names arrive
    pre-resolved (identifier_resolution has already given each
    declaration its globally-unique `@N.orig` form), so there's
    nothing scope-related left to express."""

    def test_compound_emits_inner_items_in_order(self):
        # `{ int x = 1; return x; }`:
        #   Copy(1, @0.x); Ret(@0.x)
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.Compound(block=c99_ast.Block(block_item=[
                c99_ast.D(declaration=c99_ast.VarDecl(
                    var_decl=c99_ast.Type_var_decl(
                        name="@0.x", init=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                        data_type=c99_ast.Int(),
                    ),
                )),
                c99_ast.S(statement=c99_ast.Return(
                    exp=c99_ast.Var(name="@0.x"),
                )),
            ])),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Copy(
                src=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                dst=tac_ast.Var(name="@0.x"),
            ),
            tac_ast.Ret(val=tac_ast.Var(name="@0.x")),
        ])

    def test_empty_compound_emits_nothing(self):
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.Compound(block=c99_ast.Block(block_item=[])),
            instrs,
        )
        self.assertEqual(instrs, [])

    def test_nested_compound_flattens(self):
        # `{ { return 1; } }` — both braces disappear at the IR
        # level; the inner Return is the only TAC produced.
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.Compound(block=c99_ast.Block(block_item=[
                c99_ast.S(statement=c99_ast.Compound(
                    block=c99_ast.Block(block_item=[
                        c99_ast.S(statement=c99_ast.Return(
                            exp=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                        )),
                    ]),
                )),
            ])),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=1))),
        ])

    def test_compound_inside_function_body(self):
        # End-to-end through translate_function: `int main(void) { {
        # return 7; } }`. The outer block is the function body, the
        # inner Compound is just a nested block that lowers
        # transparently.
        fn = c99_ast.Function(name="main", body=c99_ast.Block(block_item=[
            c99_ast.S(statement=c99_ast.Compound(
                block=c99_ast.Block(block_item=[
                    c99_ast.S(statement=c99_ast.Return(
                        exp=c99_ast.Constant(const=c99_ast.ConstInt(int=7)),
                    )),
                ]),
            )),
        ]))
        self.assertEqual(
            Translator().translate_function(fn).instructions,
            [tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=7)))],
        )

    def test_compound_with_distinct_shadowed_decls(self):
        # The two `x` decls have already been given distinct unique
        # names by identifier_resolution (@0.x outer, @1.x inner), so
        # the TAC has two separate Copy targets — there's no
        # collision and no scope concept needed at this stage.
        # Source equivalent: `int x = 1; { int x = 2; }`.
        t = Translator()
        instrs: list = []
        t.translate_statement(c99_ast.Compound(
            block=c99_ast.Block(block_item=[
                c99_ast.D(declaration=c99_ast.VarDecl(
                    var_decl=c99_ast.Type_var_decl(
                        name="@1.x", init=c99_ast.Constant(const=c99_ast.ConstInt(int=2)),
                        data_type=c99_ast.Int(),
                    ),
                )),
            ]),
        ), instrs)
        # Just the inner Compound's effect.
        self.assertEqual(instrs, [
            tac_ast.Copy(
                src=tac_ast.Constant(const=tac_ast.ConstInt(int=2)),
                dst=tac_ast.Var(name="@1.x"),
            ),
        ])

    def test_compound_in_if_branch(self):
        # `if (1) { return 2; }` — the then-clause is a Compound. The
        # if-stmt mints its own labels around the Compound's body.
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.IfStmt(
                condition=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                then_clause=c99_ast.Compound(
                    block=c99_ast.Block(block_item=[
                        c99_ast.S(statement=c99_ast.Return(
                            exp=c99_ast.Constant(const=c99_ast.ConstInt(int=2)),
                        )),
                    ]),
                ),
                else_clause=None,
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                target=".if_end@0",
            ),
            tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=2))),
            tac_ast.Label(name=".if_end@0"),
        ])


class TestTranslateConditional(unittest.TestCase):
    """Ternary `cond ? t : f` lowers to an if/else-shaped sequence
    that also produces a value: both arms Copy into a shared dst
    temp, and the Conditional expression returns that Var. Labels
    share the Translator's counter with `if`/short-circuit/inline-
    comparison lowerings, so numbering stays globally unique."""

    def test_basic_lowers_to_jump_copy_copy(self):
        t = Translator()
        instrs: list = []
        val = t.translate_exp(
            c99_ast.Conditional(
                condition=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                true_clause=c99_ast.Constant(const=c99_ast.ConstInt(int=2)),
                false_clause=c99_ast.Constant(const=c99_ast.ConstInt(int=3)),
            ),
            instrs,
        )
        # Labels mint first (cond_else@0, cond_end@1), then the dst
        # temp (%0). Both arms Copy into %0; the outer expression
        # returns %0 so chained uses see the chosen value.
        self.assertEqual(val, tac_ast.Var(name="%0"))
        self.assertEqual(instrs, [
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                target=".cond_else@0",
            ),
            tac_ast.Copy(
                src=tac_ast.Constant(const=tac_ast.ConstInt(int=2)),
                dst=tac_ast.Var(name="%0"),
            ),
            tac_ast.Jump(target=".cond_end@1"),
            tac_ast.Label(name=".cond_else@0"),
            tac_ast.Copy(
                src=tac_ast.Constant(const=tac_ast.ConstInt(int=3)),
                dst=tac_ast.Var(name="%0"),
            ),
            tac_ast.Label(name=".cond_end@1"),
        ])

    def test_nested_conditional_gets_unique_labels(self):
        # Each ?: mints its own pair of labels — outer and inner
        # don't collide.
        t = Translator()
        instrs: list = []
        t.translate_exp(
            c99_ast.Conditional(
                condition=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                true_clause=c99_ast.Conditional(
                    condition=c99_ast.Constant(const=c99_ast.ConstInt(int=2)),
                    true_clause=c99_ast.Constant(const=c99_ast.ConstInt(int=3)),
                    false_clause=c99_ast.Constant(const=c99_ast.ConstInt(int=4)),
                ),
                false_clause=c99_ast.Constant(const=c99_ast.ConstInt(int=5)),
            ),
            instrs,
        )
        labels = sorted({
            i.name for i in instrs if isinstance(i, tac_ast.Label)
        })
        self.assertEqual(
            labels,
            [".cond_else@0", ".cond_else@2", ".cond_end@1", ".cond_end@3"],
        )


class TestTranslateFunctionFallThrough(unittest.TestCase):
    """translate_function appends an implicit `Ret(Constant(0))` if
    the body doesn't already end in a Ret. C99 §5.1.2.2.3 specifies
    this for `main`; we apply it generally so every TAC function
    terminates."""

    def test_empty_function_gets_implicit_return_zero(self):
        fn = c99_ast.Function(
            name="main", body=c99_ast.Block(block_item=[]),
        )
        self.assertEqual(
            Translator().translate_function(fn),
            tac_ast.Function(
                name="main",
                is_global=True,
                instructions=[tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=0)))],
            ),
        )

    def test_body_without_return_gets_implicit_return_zero(self):
        fn = c99_ast.Function(name="main", body=c99_ast.Block(block_item=[
            c99_ast.D(declaration=c99_ast.VarDecl(
                var_decl=c99_ast.Type_var_decl(
                    name="@0.x", init=c99_ast.Constant(const=c99_ast.ConstInt(int=5)),
                    data_type=c99_ast.Int(),
                ),
            )),
        ]))
        self.assertEqual(
            Translator().translate_function(fn),
            tac_ast.Function(
                name="main",
                is_global=True,
                instructions=[
                    tac_ast.Copy(
                        src=tac_ast.Constant(const=tac_ast.ConstInt(int=5)),
                        dst=tac_ast.Var(name="@0.x"),
                    ),
                    tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=0))),
                ],
            ),
        )

    def test_explicit_return_does_not_append_a_second_one(self):
        # If the body already ends in a Return, the implicit Ret(0)
        # would be dead code — skip it.
        fn = c99_ast.Function(name="main", body=c99_ast.Block(block_item=[
            c99_ast.S(statement=c99_ast.Return(
                exp=c99_ast.Constant(const=c99_ast.ConstInt(int=7)),
            )),
        ]))
        self.assertEqual(
            Translator().translate_function(fn).instructions,
            [tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=7)))],
        )

    def test_null_after_return_does_not_trigger_second_return(self):
        # Null emits nothing, so the last emitted instruction is still
        # Ret — no implicit zero-return appended.
        fn = c99_ast.Function(name="main", body=c99_ast.Block(block_item=[
            c99_ast.S(statement=c99_ast.Return(
                exp=c99_ast.Constant(const=c99_ast.ConstInt(int=7)),
            )),
            c99_ast.S(statement=c99_ast.Null()),
        ]))
        self.assertEqual(
            Translator().translate_function(fn).instructions,
            [tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=7)))],
        )

    def test_block_items_processed_in_order(self):
        # Two declarations, then a return.
        fn = c99_ast.Function(name="main", body=c99_ast.Block(block_item=[
            c99_ast.D(declaration=c99_ast.VarDecl(
                var_decl=c99_ast.Type_var_decl(
                    name="@0.a", init=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                    data_type=c99_ast.Int(),
                ),
            )),
            c99_ast.D(declaration=c99_ast.VarDecl(
                var_decl=c99_ast.Type_var_decl(
                    name="@1.b", init=c99_ast.Constant(const=c99_ast.ConstInt(int=2)),
                    data_type=c99_ast.Int(),
                ),
            )),
            c99_ast.S(statement=c99_ast.Return(
                exp=c99_ast.Var(name="@1.b"),
            )),
        ]))
        self.assertEqual(
            Translator().translate_function(fn).instructions,
            [
                tac_ast.Copy(
                    src=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                    dst=tac_ast.Var(name="@0.a"),
                ),
                tac_ast.Copy(
                    src=tac_ast.Constant(const=tac_ast.ConstInt(int=2)),
                    dst=tac_ast.Var(name="@1.b"),
                ),
                tac_ast.Ret(val=tac_ast.Var(name="@1.b")),
            ],
        )


class TestTranslateProgram(unittest.TestCase):
    def test_return_constant(self):
        # Both c99 and TAC programs are lists of Functions now, so
        # the `[Function(...)]` wrapping mirrors on both sides.
        prog = _c99_program_of(c99_ast.Function(
            name="main",
            body=c99_ast.Block(block_item=[c99_ast.S(statement=c99_ast.Return(
                exp=c99_ast.Constant(const=c99_ast.ConstInt(int=42)),
            ))]),
        ))
        self.assertEqual(
            translate_program(prog),
            tac_ast.Program(top_level=[tac_ast.Function(
                name="main",
                is_global=True,
                params=[],
                instructions=[tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=42)))],
            )]),
        )

    def test_return_unary(self):
        tac = translate_program(_c99_program_of(c99_ast.Function(
                name="main",
                body=c99_ast.Block(block_item=[c99_ast.S(
                    statement=c99_ast.Return(exp=c99_ast.Unary(
                        op=c99_ast.Negate(),
                        exp=c99_ast.Constant(const=c99_ast.ConstInt(int=5)),
                    )),
                )]),
            )))
        self.assertEqual(
            tac.top_level[0].instructions,
            [
                tac_ast.Unary(
                    op=tac_ast.Negate(),
                    src=tac_ast.Constant(const=tac_ast.ConstInt(int=5)),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Ret(val=tac_ast.Var(name="%0")),
            ],
        )

    def test_end_to_end_nested_from_source(self):
        tac = translate_program(parse("int main(void) { return -(~5); }"))
        self.assertEqual(
            tac.top_level[0].instructions,
            [
                tac_ast.Unary(
                    op=tac_ast.Complement(),
                    src=tac_ast.Constant(const=tac_ast.ConstInt(int=5)),
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

    def test_postfix_increment_captures_old_value_then_updates(self):
        # `a++` lowers to: Copy(a, %old) — capture before mutation;
        # Binary(Add, a, 1, %new) — compute updated value;
        # Copy(%new, a) — store back. Returns Var(%old) so callers
        # see the *old* value (postfix semantics).
        t = Translator()
        instrs: list = []
        result = t.translate_exp(
            c99_ast.Postfix(
                op=c99_ast.Increment(),
                operand=c99_ast.Var(name="a"),
            ),
            instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="%0"))
        self.assertEqual(instrs, [
            tac_ast.Copy(
                src=tac_ast.Var(name="a"),
                dst=tac_ast.Var(name="%0"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=tac_ast.Var(name="a"),
                src2=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                dst=tac_ast.Var(name="%1"),
            ),
            tac_ast.Copy(
                src=tac_ast.Var(name="%1"),
                dst=tac_ast.Var(name="a"),
            ),
        ])

    def test_postfix_decrement_uses_subtract(self):
        t = Translator()
        instrs: list = []
        t.translate_exp(
            c99_ast.Postfix(
                op=c99_ast.Decrement(),
                operand=c99_ast.Var(name="a"),
            ),
            instrs,
        )
        # Just check the binary op chosen is Subtract; the surrounding
        # shape is the same as Increment.
        self.assertEqual(instrs[1].op, tac_ast.Subtract())

    def test_postfix_in_assignment_returns_old_value(self):
        # `b = a++` — the inner Postfix returns Var(%0) (the captured
        # old value), which the outer Assignment Copies into b. So `b`
        # ends up with the value `a` had *before* the increment.
        t = Translator()
        instrs: list = []
        t.translate_exp(
            c99_ast.Assignment(
                lval=c99_ast.Var(name="b"),
                rval=c99_ast.Postfix(
                    op=c99_ast.Increment(),
                    operand=c99_ast.Var(name="a"),
                ),
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Copy(
                src=tac_ast.Var(name="a"),
                dst=tac_ast.Var(name="%0"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=tac_ast.Var(name="a"),
                src2=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                dst=tac_ast.Var(name="%1"),
            ),
            tac_ast.Copy(
                src=tac_ast.Var(name="%1"),
                dst=tac_ast.Var(name="a"),
            ),
            tac_ast.Copy(
                src=tac_ast.Var(name="%0"),
                dst=tac_ast.Var(name="b"),
            ),
        ])

    def test_postfix_non_var_operand_raises_type_error(self):
        # identifier_resolution should have rejected this; the runtime
        # check is defense-in-depth.
        t = Translator()
        with self.assertRaises(TypeError) as ctx:
            t.translate_exp(
                c99_ast.Postfix(
                    op=c99_ast.Increment(),
                    operand=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                ),
                [],
            )
        self.assertIn("Var", str(ctx.exception))

    def test_prefix_lowers_via_assignment_branch(self):
        # `++a` is desugared by the parser to `a = a + 1`, which is
        # an Assignment. The TAC therefore has just the Binary +
        # Copy — no extra "%old" capture, because prefix returns the
        # *new* value, not the old one. End-to-end through parse +
        # translate to confirm the sequence.
        tac = translate_program(parse(
            "int main(void) { int a; ++a; }"
        ))
        self.assertEqual(
            tac.top_level[0].instructions,
            [
                tac_ast.Binary(
                    op=tac_ast.Add(),
                    src1=tac_ast.Var(name="a"),
                    src2=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Copy(
                    src=tac_ast.Var(name="%0"),
                    dst=tac_ast.Var(name="a"),
                ),
                tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=0))),
            ],
        )

    def test_compound_assignment_lowers_like_desugared_form(self):
        # `a += 1` is desugared by the parser to `a = a + 1`. The TAC
        # is therefore: read `a` and `1` into a Binary(Add) producing
        # %0, then Copy %0 back into a. The implicit `Ret(0)` from
        # translate_function tails it. (No identifier_resolution here, so
        # the name stays as user-written `a` rather than `@0.a`.)
        tac = translate_program(parse(
            "int main(void) { int a; a += 1; }"
        ))
        self.assertEqual(
            tac.top_level[0].instructions,
            [
                tac_ast.Binary(
                    op=tac_ast.Add(),
                    src1=tac_ast.Var(name="a"),
                    src2=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Copy(
                    src=tac_ast.Var(name="%0"),
                    dst=tac_ast.Var(name="a"),
                ),
                tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=0))),
            ],
        )

    def test_end_to_end_binary_precedence(self):
        # 1 + 2 * 3 — the parser puts Multiply on the right of Add.
        # c99_to_tac translates left first, so the constant 1 is
        # the first operand to flow through; then the multiply (which
        # allocates %0); then the add (allocating %1).
        tac = translate_program(parse("int main(void) { return 1 + 2 * 3; }"))
        self.assertEqual(
            tac.top_level[0].instructions,
            [
                tac_ast.Binary(
                    op=tac_ast.Multiply(),
                    src1=tac_ast.Constant(const=tac_ast.ConstInt(int=2)),
                    src2=tac_ast.Constant(const=tac_ast.ConstInt(int=3)),
                    dst=tac_ast.Var(name="%0"),
                ),
                tac_ast.Binary(
                    op=tac_ast.Add(),
                    src1=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
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
        self.assertEqual(a.top_level[0].instructions[0].dst,
                         tac_ast.Var(name="%0"))
        self.assertEqual(b.top_level[0].instructions[0].dst,
                         tac_ast.Var(name="%0"))


class TestTranslateLoops(unittest.TestCase):
    """Each iteration statement (`while`, `do-while`, `for`) lowers to
    a fixed sequence of labels and jumps derived from the loop's
    base label (set by the loop_labeling pass) plus suffixes
    `_start` / `_continue` / `_break`. Inside the body, `break;` and
    `continue;` lower to a single Jump to the matching sub-label."""

    def test_while_basic_shape(self):
        # while (1) ;  — Label(continue), eval cond, JumpIfFalse,
        # body (Null = nothing), Jump(continue), Label(break).
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.WhileStmt(
                condition=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                body=c99_ast.Null(),
                label=".loop@0",
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Label(name=".loop@0_continue"),
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                target=".loop@0_break",
            ),
            tac_ast.Jump(target=".loop@0_continue"),
            tac_ast.Label(name=".loop@0_break"),
        ])

    def test_while_break_continue_jump_to_sub_labels(self):
        # while (1) { break; continue; } — break -> _break, continue
        # -> _continue. The break/continue carry the loop's base
        # label (the loop_labeling pass set them up that way).
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.WhileStmt(
                condition=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                body=c99_ast.Compound(block=c99_ast.Block(block_item=[
                    c99_ast.S(statement=c99_ast.BreakStmt(label=".loop@0")),
                    c99_ast.S(statement=c99_ast.ContinueStmt(label=".loop@0")),
                ])),
                label=".loop@0",
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Label(name=".loop@0_continue"),
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                target=".loop@0_break",
            ),
            tac_ast.Jump(target=".loop@0_break"),
            tac_ast.Jump(target=".loop@0_continue"),
            tac_ast.Jump(target=".loop@0_continue"),
            tac_ast.Label(name=".loop@0_break"),
        ])

    def test_do_while_basic_shape(self):
        # do ; while (1);  — Label(start), body (nothing for Null),
        # Label(continue), eval cond, JumpIfTrue(start), Label(break).
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.DoWhileStmt(
                body=c99_ast.Null(),
                condition=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                label=".loop@0",
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Label(name=".loop@0_start"),
            tac_ast.Label(name=".loop@0_continue"),
            tac_ast.JumpIfTrue(
                condition=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                target=".loop@0_start",
            ),
            tac_ast.Label(name=".loop@0_break"),
        ])

    def test_do_while_continue_targets_continue_label(self):
        # do continue; while (1);  — the continue jumps to _continue,
        # which sits between body and condition test, so the test
        # still runs.
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.DoWhileStmt(
                body=c99_ast.ContinueStmt(label=".loop@0"),
                condition=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                label=".loop@0",
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Label(name=".loop@0_start"),
            tac_ast.Jump(target=".loop@0_continue"),
            tac_ast.Label(name=".loop@0_continue"),
            tac_ast.JumpIfTrue(
                condition=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                target=".loop@0_start",
            ),
            tac_ast.Label(name=".loop@0_break"),
        ])

    def test_for_basic_shape(self):
        # for (;;) ;   — empty header, no condition, no post, Null body.
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.ForStmt(
                init=c99_ast.InitExp(exp=None),
                condition=None,
                post_clause=None,
                body=c99_ast.Null(),
                label=".loop@0",
            ),
            instrs,
        )
        # No init insns, no condition test (and no JumpIfFalse), no
        # post insns. Just start/continue/break wrapping the (empty)
        # body and a Jump back to start.
        self.assertEqual(instrs, [
            tac_ast.Label(name=".loop@0_start"),
            tac_ast.Label(name=".loop@0_continue"),
            tac_ast.Jump(target=".loop@0_start"),
            tac_ast.Label(name=".loop@0_break"),
        ])

    def test_for_full_header(self):
        # for (i = 0; 1; i++) break;  — exercise init, condition,
        # post-clause, and a break inside the body.
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.ForStmt(
                init=c99_ast.InitExp(exp=c99_ast.Assignment(
                    lval=c99_ast.Var(name="i"),
                    rval=c99_ast.Constant(const=c99_ast.ConstInt(int=0)),
                )),
                condition=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                post_clause=c99_ast.Postfix(
                    op=c99_ast.Increment(),
                    operand=c99_ast.Var(name="i"),
                ),
                body=c99_ast.BreakStmt(label=".loop@0"),
                label=".loop@0",
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            # init: i = 0.
            tac_ast.Copy(
                src=tac_ast.Constant(const=tac_ast.ConstInt(int=0)),
                dst=tac_ast.Var(name="i"),
            ),
            tac_ast.Label(name=".loop@0_start"),
            # No condition insns needed for a Constant — JumpIfFalse
            # takes the Constant directly.
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                target=".loop@0_break",
            ),
            # body: break -> Jump(_break).
            tac_ast.Jump(target=".loop@0_break"),
            tac_ast.Label(name=".loop@0_continue"),
            # post: i++ — captures old value into %0, computes %1 =
            # i+1, stores %1 back into i. Result %0 is unused (the
            # post-clause's value is discarded for side effects).
            tac_ast.Copy(
                src=tac_ast.Var(name="i"),
                dst=tac_ast.Var(name="%0"),
            ),
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=tac_ast.Var(name="i"),
                src2=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                dst=tac_ast.Var(name="%1"),
            ),
            tac_ast.Copy(
                src=tac_ast.Var(name="%1"),
                dst=tac_ast.Var(name="i"),
            ),
            tac_ast.Jump(target=".loop@0_start"),
            tac_ast.Label(name=".loop@0_break"),
        ])

    def test_for_init_decl(self):
        # for (int i = 0; ;) ;  — init is a var_decl, lowers like
        # `int i = 0;` (Copy of 0 into i).
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.ForStmt(
                init=c99_ast.InitDecl(var_decl=c99_ast.Type_var_decl(
                    name="i", init=c99_ast.Constant(const=c99_ast.ConstInt(int=0)),
                    data_type=c99_ast.Int(),
                )),
                condition=None,
                post_clause=None,
                body=c99_ast.Null(),
                label=".loop@0",
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Copy(
                src=tac_ast.Constant(const=tac_ast.ConstInt(int=0)),
                dst=tac_ast.Var(name="i"),
            ),
            tac_ast.Label(name=".loop@0_start"),
            tac_ast.Label(name=".loop@0_continue"),
            tac_ast.Jump(target=".loop@0_start"),
            tac_ast.Label(name=".loop@0_break"),
        ])

    def test_for_init_decl_without_initializer_emits_nothing(self):
        # for (int i; ;) ;  — bare declaration, no initializer, so the
        # for-init contributes zero TAC instructions (matches a top-
        # level `int i;`).
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.ForStmt(
                init=c99_ast.InitDecl(var_decl=c99_ast.Type_var_decl(
                    name="i", init=None,
                    data_type=c99_ast.Int(),
                )),
                condition=None,
                post_clause=None,
                body=c99_ast.Null(),
                label=".loop@0",
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Label(name=".loop@0_start"),
            tac_ast.Label(name=".loop@0_continue"),
            tac_ast.Jump(target=".loop@0_start"),
            tac_ast.Label(name=".loop@0_break"),
        ])

    def test_for_no_condition_omits_jump_if_false(self):
        # for (i = 0;; i++) break;  — no condition. The TAC must have
        # NO JumpIfFalse and NO condition-evaluation instructions
        # between start label and body.
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.ForStmt(
                init=c99_ast.InitExp(exp=None),
                condition=None,
                post_clause=None,
                body=c99_ast.BreakStmt(label=".loop@0"),
                label=".loop@0",
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Label(name=".loop@0_start"),
            tac_ast.Jump(target=".loop@0_break"),
            tac_ast.Label(name=".loop@0_continue"),
            tac_ast.Jump(target=".loop@0_start"),
            tac_ast.Label(name=".loop@0_break"),
        ])
        # Belt-and-braces: no JumpIfFalse anywhere.
        self.assertFalse(any(
            isinstance(i, tac_ast.JumpIfFalse) for i in instrs
        ))

    def test_for_no_post_omits_post_insns(self):
        # for (;1;) ;  — condition present, post-clause absent. Only
        # the condition test runs each iteration; nothing between
        # _continue and the back-jump to _start.
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.ForStmt(
                init=c99_ast.InitExp(exp=None),
                condition=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                post_clause=None,
                body=c99_ast.Null(),
                label=".loop@0",
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Label(name=".loop@0_start"),
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                target=".loop@0_break",
            ),
            tac_ast.Label(name=".loop@0_continue"),
            tac_ast.Jump(target=".loop@0_start"),
            tac_ast.Label(name=".loop@0_break"),
        ])

    def test_for_post_clause_result_is_discarded(self):
        # `i++` as the post-clause emits its mutation instructions
        # but no Copy of the result anywhere. The %old temp it
        # produces is dead. Confirm by checking no instruction
        # references the temp the post-clause returned.
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.ForStmt(
                init=c99_ast.InitExp(exp=None),
                condition=None,
                post_clause=c99_ast.Postfix(
                    op=c99_ast.Increment(),
                    operand=c99_ast.Var(name="i"),
                ),
                body=c99_ast.Null(),
                label=".loop@0",
            ),
            instrs,
        )
        # The post-clause emits Copy(i, %0); Binary(Add, i, 1, %1);
        # Copy(%1, i). The %0 temp it returns (the old value) is
        # unused — no later instruction reads it.
        copy_old, binary_new, copy_back = instrs[2], instrs[3], instrs[4]
        self.assertEqual(copy_old.dst, tac_ast.Var(name="%0"))
        self.assertEqual(binary_new.dst, tac_ast.Var(name="%1"))
        self.assertEqual(copy_back.src, tac_ast.Var(name="%1"))


class TestTranslateBreakContinueDirectly(unittest.TestCase):
    """Outside of a loop construct, BreakStmt and ContinueStmt should
    still translate (the loop_labeling pass would have errored on a
    truly orphaned break/continue; here we test the translate step in
    isolation, which just needs to honor whatever label is on the
    node)."""

    def test_break_emits_jump_to_break_label(self):
        t = Translator()
        instrs: list = []
        t.translate_statement(c99_ast.BreakStmt(label=".loop@3"), instrs)
        self.assertEqual(instrs, [tac_ast.Jump(target=".loop@3_break")])

    def test_continue_emits_jump_to_continue_label(self):
        t = Translator()
        instrs: list = []
        t.translate_statement(c99_ast.ContinueStmt(label=".loop@3"), instrs)
        self.assertEqual(instrs, [tac_ast.Jump(target=".loop@3_continue")])


class TestTranslateNestedLoops(unittest.TestCase):
    """When loops nest, each loop has its own base label (set by
    loop_labeling), so the inner break/continue target sub-labels
    that are disjoint from the outer's. The TAC stays correctly
    interleaved: inner labels appear inside the outer's
    body-instructions section."""

    def test_inner_break_targets_inner_outer_break_targets_outer(self):
        # while (1) { while (1) break; break; }
        # outer label .loop@0, inner label .loop@1.
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.WhileStmt(
                condition=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                body=c99_ast.Compound(block=c99_ast.Block(block_item=[
                    c99_ast.S(statement=c99_ast.WhileStmt(
                        condition=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                        body=c99_ast.BreakStmt(label=".loop@1"),
                        label=".loop@1",
                    )),
                    c99_ast.S(statement=c99_ast.BreakStmt(label=".loop@0")),
                ])),
                label=".loop@0",
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Label(name=".loop@0_continue"),
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                target=".loop@0_break",
            ),
            # Inner while.
            tac_ast.Label(name=".loop@1_continue"),
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                target=".loop@1_break",
            ),
            tac_ast.Jump(target=".loop@1_break"),  # inner break
            tac_ast.Jump(target=".loop@1_continue"),
            tac_ast.Label(name=".loop@1_break"),
            # Outer break, after the inner loop's instructions.
            tac_ast.Jump(target=".loop@0_break"),
            tac_ast.Jump(target=".loop@0_continue"),
            tac_ast.Label(name=".loop@0_break"),
        ])


class TestEndToEndLoops(unittest.TestCase):
    """Pipe through parse + identifier_resolution + label_resolution +
    loop_labeling + c99_to_tac. Spot-check that the loop labels in
    the emitted TAC match the loop_labeling pass's `.loop@<N>` scheme
    and that break/continue jump to the right sub-labels."""

    def _translate(self, src):
        from passes.label_resolution import resolve_program as resolve_labels
        from passes.loop_labeling import label_program as label_loops
        from passes.identifier_resolution import (
            resolve_program as resolve_identifiers,
        )
        return translate_program(label_loops(
            resolve_labels(resolve_identifiers(parse(src)))
        ))

    def test_while_with_break_jumps_to_break_label(self):
        prog = self._translate(
            "int main(void) { while (1) break; return 0; }"
        )
        instrs = prog.top_level[0].instructions
        self.assertEqual(instrs[0], tac_ast.Label(name=".loop@0_continue"))
        self.assertEqual(
            instrs[2], tac_ast.Jump(target=".loop@0_break"),
        )

    def test_for_with_continue_jumps_to_continue_label(self):
        prog = self._translate(
            "int main(void) { for (;;) continue; return 0; }"
        )
        instrs = prog.top_level[0].instructions
        # No condition → no JumpIfFalse anywhere.
        self.assertFalse(any(
            isinstance(i, tac_ast.JumpIfFalse) for i in instrs
        ))
        # The continue lowers to Jump to _continue.
        self.assertIn(
            tac_ast.Jump(target=".loop@0_continue"), instrs,
        )

    def test_nested_loops_get_distinct_sub_labels(self):
        prog = self._translate(
            "int main(void) { while (1) { while (1) break; break; } "
            "return 0; }"
        )
        targets = {
            i.target for i in prog.top_level[0].instructions
            if isinstance(i, tac_ast.Jump)
        }
        # Both outer (.loop@0_*) and inner (.loop@1_*) sub-labels
        # appear as Jump targets.
        self.assertIn(".loop@0_break", targets)
        self.assertIn(".loop@0_continue", targets)
        self.assertIn(".loop@1_break", targets)
        self.assertIn(".loop@1_continue", targets)


class TestEndToEndSwitch(unittest.TestCase):
    """Pipe through parse + identifier_resolution + label_resolution +
    loop_labeling + type_checking + c99_to_tac. Switch lowers to a
    compare-and-jump dispatch chain followed by the body and a
    trailing break label; case/default labels are inlined where they
    appear in source."""

    def _translate(self, src):
        from passes.identifier_resolution import (
            resolve_program as resolve_identifiers,
        )
        from passes.label_resolution import resolve_program as resolve_labels
        from passes.loop_labeling import label_program as label_loops
        from passes.type_checking import check_program
        ast = label_loops(
            resolve_labels(resolve_identifiers(parse(src)))
        )
        ast, syms = check_program(ast)
        return Translator(syms).translate_program(ast)

    def test_dispatch_chain_then_body_then_break(self):
        prog = self._translate(
            "int main(void) { switch (3) {"
            " case 0: return 0;"
            " case 5: return 5;"
            " default: return 99; } }"
        )
        instrs = prog.top_level[0].instructions
        # First: comparisons + JumpIfTrue for each case, then
        # unconditional Jump to the default.
        jumps = [i for i in instrs
                 if isinstance(i, (tac_ast.JumpIfTrue, tac_ast.Jump))]
        targets = [j.target for j in jumps]
        self.assertEqual(targets[0], ".case@1")
        self.assertEqual(targets[1], ".case@2")
        # Third entry is the unconditional fallthrough Jump.
        self.assertEqual(targets[2], ".default@3")
        # The trailing break label is emitted at the end.
        labels = [i.name for i in instrs if isinstance(i, tac_ast.Label)]
        self.assertIn(".case@1", labels)
        self.assertIn(".case@2", labels)
        self.assertIn(".default@3", labels)
        self.assertEqual(labels[-1], ".switch@0_break")

    def test_no_default_falls_through_to_break_label(self):
        prog = self._translate(
            "int main(void) { switch (3) { case 0: return 0; } return 9; }"
        )
        instrs = prog.top_level[0].instructions
        # The fallthrough Jump (after the dispatch chain) goes to the
        # switch's break label, not a default.
        unconditional_jumps = [
            j.target for j in instrs if isinstance(j, tac_ast.Jump)
        ]
        self.assertEqual(unconditional_jumps[0], ".switch@0_break")

    def test_break_in_switch_targets_break_label(self):
        prog = self._translate(
            "int main(void) { switch (1) {"
            " case 1: break; default: return 0; } return 9; }"
        )
        targets = [
            i.target for i in prog.top_level[0].instructions
            if isinstance(i, tac_ast.Jump)
        ]
        # The case body is `break;`, which lowers to Jump to the
        # switch's break label.
        self.assertIn(".switch@0_break", targets)


class TestTranslateFunctionCall(unittest.TestCase):
    """`f(a, b, ...)` lowers to: evaluate each arg in source order
    (so its temporaries get the lower numbers), collect the resulting
    TAC vals, mint a fresh dst temp for the return value, emit
    `FunctionCall(name, [args], dst)`, and return dst so the caller
    can thread the value through into a later instruction."""

    def test_no_args_emits_single_call(self):
        # `f()` — args list empty; the only emitted instruction is
        # the FunctionCall itself, with a fresh dst.
        t = Translator()
        instrs: list = []
        result = t.translate_exp(
            c99_ast.FunctionCall(name="f", args=[]), instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="%0"))
        self.assertEqual(instrs, [
            tac_ast.FunctionCall(
                name="f", args=[], dst=tac_ast.Var(name="%0"),
            ),
        ])

    def test_constant_args_pass_through_as_constants(self):
        # `f(1, 2)` — each Constant arg is its own TAC val (no
        # temporary materialization needed), so the FunctionCall
        # carries Constants directly.
        t = Translator()
        instrs: list = []
        result = t.translate_exp(
            c99_ast.FunctionCall(
                name="f",
                args=[c99_ast.Constant(const=c99_ast.ConstInt(int=1)), c99_ast.Constant(const=c99_ast.ConstInt(int=2))],
            ),
            instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="%0"))
        self.assertEqual(instrs, [
            tac_ast.FunctionCall(
                name="f",
                args=[
                    tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                    tac_ast.Constant(const=tac_ast.ConstInt(int=2)),
                ],
                dst=tac_ast.Var(name="%0"),
            ),
        ])

    def test_compound_arg_evaluated_first(self):
        # `f(1 + 2)` — the arg expression is evaluated into %0
        # *before* the FunctionCall instruction is emitted; the
        # call uses the captured Var(%0) and writes its return
        # value to a fresh %1.
        t = Translator()
        instrs: list = []
        t.translate_exp(
            c99_ast.FunctionCall(
                name="f",
                args=[c99_ast.Binary(
                    op=c99_ast.Add(),
                    left=c99_ast.Constant(const=c99_ast.ConstInt(int=1)),
                    right=c99_ast.Constant(const=c99_ast.ConstInt(int=2)),
                )],
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
                src2=tac_ast.Constant(const=tac_ast.ConstInt(int=2)),
                dst=tac_ast.Var(name="%0"),
            ),
            tac_ast.FunctionCall(
                name="f",
                args=[tac_ast.Var(name="%0")],
                dst=tac_ast.Var(name="%1"),
            ),
        ])

    def test_args_evaluated_left_to_right(self):
        # `f(g(1), g(2))` — `g(1)` evaluates first (its dst is %0),
        # then `g(2)` (dst %1), then the outer `f` call (dst %2).
        # Reading the temp numbers gives the source-order trace.
        t = Translator()
        instrs: list = []
        t.translate_exp(
            c99_ast.FunctionCall(
                name="f",
                args=[
                    c99_ast.FunctionCall(
                        name="g",
                        args=[c99_ast.Constant(const=c99_ast.ConstInt(int=1))],
                    ),
                    c99_ast.FunctionCall(
                        name="g",
                        args=[c99_ast.Constant(const=c99_ast.ConstInt(int=2))],
                    ),
                ],
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.FunctionCall(
                name="g",
                args=[tac_ast.Constant(const=tac_ast.ConstInt(int=1))],
                dst=tac_ast.Var(name="%0"),
            ),
            tac_ast.FunctionCall(
                name="g",
                args=[tac_ast.Constant(const=tac_ast.ConstInt(int=2))],
                dst=tac_ast.Var(name="%1"),
            ),
            tac_ast.FunctionCall(
                name="f",
                args=[
                    tac_ast.Var(name="%0"),
                    tac_ast.Var(name="%1"),
                ],
                dst=tac_ast.Var(name="%2"),
            ),
        ])

    def test_call_in_return_position_passes_value(self):
        # `return f();` — the FunctionCall is lowered, and its dst
        # temp is what the Ret references. End-to-end via the
        # statement translator.
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.Return(exp=c99_ast.FunctionCall(name="f", args=[])),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.FunctionCall(
                name="f", args=[], dst=tac_ast.Var(name="%0"),
            ),
            tac_ast.Ret(val=tac_ast.Var(name="%0")),
        ])

    def test_call_via_full_pipeline(self):
        # Through parse + identifier_resolution: `int foo(int a) {
        # return a; } int main(void) { return foo(42); }`. Two
        # functions in the c99 program; two functions in the TAC
        # program. The call lowers to a single FunctionCall with
        # `args=[Constant(42)]` and a fresh dst.
        from passes.identifier_resolution import (
            resolve_program as resolve_identifiers,
        )
        prog = parse(
            "int foo(int a) { return a; } "
            "int main(void) { return foo(42); }"
        )
        prog = resolve_identifiers(prog)
        tac = translate_program(prog)
        self.assertEqual(len(tac.top_level), 2)
        foo_fn, main_fn = tac.top_level
        self.assertEqual(foo_fn.name, "foo")
        self.assertEqual(foo_fn.params, ["@0.a"])
        # `foo`'s body lowered: Ret(Var(@0.a)).
        self.assertEqual(
            foo_fn.instructions,
            [tac_ast.Ret(val=tac_ast.Var(name="@0.a"))],
        )
        # `main`'s body lowered: FunctionCall(foo, [42], %N) then
        # Ret(%N). The temp counter is shared with `foo`'s body
        # (the Translator is one-per-program), so the dst index
        # depends on whatever foo's translation consumed.
        self.assertEqual(main_fn.name, "main")
        self.assertEqual(main_fn.params, [])
        # Two instructions: the call and the return.
        self.assertEqual(len(main_fn.instructions), 2)
        call, ret = main_fn.instructions
        self.assertIsInstance(call, tac_ast.FunctionCall)
        self.assertEqual(call.name, "foo")
        self.assertEqual(call.args, [tac_ast.Constant(const=tac_ast.ConstInt(int=42))])
        self.assertEqual(ret, tac_ast.Ret(val=call.dst))


class TestTranslateMultipleFunctions(unittest.TestCase):
    """Top-level c99 functions go through one at a time, each
    yielding a TAC top_level entry. The order of TAC functions
    matches the source order. Each function gets its own implicit
    Ret(0) safety net at the end if the body falls off without
    returning."""

    def test_two_functions_appear_in_source_order(self):
        from passes.identifier_resolution import (
            resolve_program as resolve_identifiers,
        )
        prog = parse(
            "int foo(void) { return 1; } "
            "int bar(void) { return 2; } "
            "int main(void) { return 0; }"
        )
        prog = resolve_identifiers(prog)
        tac = translate_program(prog)
        names = [fn.name for fn in tac.top_level]
        self.assertEqual(names, ["foo", "bar", "main"])

    def test_each_function_gets_its_own_implicit_ret_zero(self):
        # Both bodies fall off without an explicit return; each
        # should pick up a `Ret(Constant(0))` at the end of its
        # own instruction list.
        from passes.identifier_resolution import (
            resolve_program as resolve_identifiers,
        )
        prog = parse(
            "int foo(void) { } "
            "int main(void) { }"
        )
        prog = resolve_identifiers(prog)
        tac = translate_program(prog)
        for fn in tac.top_level:
            self.assertEqual(
                fn.instructions,
                [tac_ast.Ret(val=tac_ast.Constant(const=tac_ast.ConstInt(int=0)))],
            )

    def test_function_params_pass_through_to_tac(self):
        # `int foo(int a, int b) { ... }` — the TAC function
        # carries the same renamed param names as the c99 Function
        # node. Body refs to the params resolve to those same
        # names because the body has been through identifier
        # resolution already.
        from passes.identifier_resolution import (
            resolve_program as resolve_identifiers,
        )
        prog = parse("int foo(int a, int b) { return a + b; }")
        prog = resolve_identifiers(prog)
        tac = translate_program(prog)
        fn = tac.top_level[0]
        self.assertEqual(fn.params, ["@0.a", "@1.b"])
        # Body sees Var("@0.a") and Var("@1.b") in the Add.
        binary = fn.instructions[0]
        self.assertIsInstance(binary, tac_ast.Binary)
        self.assertEqual(binary.src1, tac_ast.Var(name="@0.a"))
        self.assertEqual(binary.src2, tac_ast.Var(name="@1.b"))


class TestMakeTemporaryVariableName(unittest.TestCase):
    def test_sequential_counter(self):
        t = Translator()
        self.assertEqual(t.make_temporary_variable_name(), "%0")
        self.assertEqual(t.make_temporary_variable_name(), "%1")
        self.assertEqual(t.make_temporary_variable_name(), "%2")

    def test_temp_registers_in_symbol_table_with_passed_type(self):
        # Each call to `make_temporary_variable_name` adds a
        # LocalAttr entry to the Translator's symbol table, keyed by
        # the minted name and typed by the caller-provided type.
        from passes.type_checking import LocalAttr
        from passes.type_checking import SymbolTable as ST
        symbols = ST()
        t = Translator(symbols)
        name_int = t.make_temporary_variable_name(c99_ast.Int())
        name_long = t.make_temporary_variable_name(c99_ast.Long())
        self.assertEqual(symbols[name_int].type, c99_ast.Int())
        self.assertIsInstance(symbols[name_int].attrs, LocalAttr)
        self.assertEqual(symbols[name_long].type, c99_ast.Long())
        self.assertIsInstance(symbols[name_long].attrs, LocalAttr)

    def test_temp_without_type_defaults_to_int(self):
        # The optional-type backstop is for unit tests that want
        # the bare counter without going through type-checking;
        # the temp still gets registered, with a default Int type.
        from passes.type_checking import LocalAttr
        from passes.type_checking import SymbolTable as ST
        symbols = ST()
        t = Translator(symbols)
        name = t.make_temporary_variable_name()
        self.assertEqual(symbols[name].type, c99_ast.Int())
        self.assertIsInstance(symbols[name].attrs, LocalAttr)


class TestCastAndStaticVariableTypes(unittest.TestCase):
    """The TAC lowering for `Cast`, the typed `Constant` shape, and
    the typed `StaticVariable` initial value all bridge the c99
    AST's typed nodes to the TAC AST's typed nodes. End-to-end
    cases through the full pipeline (parse → resolve → type-check
    → translate_to_tac) verify the wiring."""

    def _tac(self, src):
        from compile import _resolved
        from passes.type_checking import check_program
        prog, symbols = check_program(_resolved(src))
        return translate_program(prog, symbols)

    def test_int_to_long_cast_emits_sign_extend(self):
        tac = self._tac(
            "int main(void) { int x = 5; long y = (long)x; "
            "return (int)y; }"
        )
        instrs = tac.top_level[0].instructions
        kinds = [type(i).__name__ for i in instrs]
        # Int→Long cast on `x` becomes SignExtend; Long→Int cast on
        # `y` for the return becomes Truncate.
        self.assertIn("SignExtend", kinds)
        self.assertIn("Truncate", kinds)

    def test_identity_cast_emits_no_conversion(self):
        # `(int)x` where `x` is already Int — same source and target
        # types, so no SignExtend / Truncate gets emitted.
        tac = self._tac("int main(void) { int x = 5; return (int)x; }")
        instrs = tac.top_level[0].instructions
        kinds = [type(i).__name__ for i in instrs]
        self.assertNotIn("SignExtend", kinds)
        self.assertNotIn("Truncate", kinds)

    def test_constant_lowers_with_const_variant_preserved(self):
        # ConstInt and ConstLong on the c99 side translate to the
        # matching variant on the TAC side.
        tac = self._tac("int main(void) { return 5; }")
        ret = tac.top_level[0].instructions[0]
        self.assertIsInstance(ret, tac_ast.Ret)
        self.assertIsInstance(ret.val, tac_ast.Constant)
        self.assertIsInstance(ret.val.const, tac_ast.ConstInt)
        self.assertEqual(ret.val.const.int, 5)

    def test_static_variable_carries_data_type_and_typed_init(self):
        tac = self._tac(
            "long g_long = 7; "
            "int g_int = 3; "
            "int main(void) { return g_int; }"
        )
        statics = {
            tl.name: tl for tl in tac.top_level
            if isinstance(tl, tac_ast.StaticVariable)
        }
        self.assertEqual(statics["g_long"].data_type, tac_ast.Long())
        self.assertEqual(
            statics["g_long"].init, [tac_ast.LongInit(int=7)],
        )
        self.assertEqual(statics["g_int"].data_type, tac_ast.Int())
        self.assertEqual(
            statics["g_int"].init, [tac_ast.IntInit(int=3)],
        )

    def test_long_long_static_carries_long_long_init(self):
        # `long long g = 1234567890LL;` lays down a typed
        # LongLongInit (signed 4B). ULongLong gets ULongLongInit.
        tac = self._tac(
            "long long g = 1234567890LL; "
            "unsigned long long u = 4000000000ULL; "
            "int main(void) { return 0; }"
        )
        statics = {
            tl.name: tl for tl in tac.top_level
            if isinstance(tl, tac_ast.StaticVariable)
        }
        self.assertEqual(statics["g"].data_type, tac_ast.LongLong())
        self.assertEqual(
            statics["g"].init, [tac_ast.LongLongInit(int=1234567890)],
        )
        self.assertEqual(statics["u"].data_type, tac_ast.ULongLong())
        self.assertEqual(
            statics["u"].init,
            [tac_ast.ULongLongInit(int=4000000000)],
        )

    def test_int_to_long_long_cast_emits_sign_extend(self):
        # Int → LongLong widening via the same SignExtend node as
        # Int → Long; tac_to_asm reads the dst width from the
        # symbol table to fan out per byte.
        tac = self._tac(
            "int main(void) { int x = 5; long long y = (long long)x; "
            "return (int)y; }"
        )
        instrs = tac.top_level[0].instructions
        kinds = [type(i).__name__ for i in instrs]
        self.assertIn("SignExtend", kinds)
        self.assertIn("Truncate", kinds)

    def test_uint_to_unsigned_long_long_cast_emits_zero_extend(self):
        # Unsigned narrower → unsigned wider goes through ZeroExtend
        # instead of SignExtend, because the new high bytes are
        # unconditionally zero.
        tac = self._tac(
            "int main(void) { unsigned int x = 5U; "
            "unsigned long long y = (unsigned long long)x; "
            "return 0; }"
        )
        instrs = tac.top_level[0].instructions
        kinds = [type(i).__name__ for i in instrs]
        self.assertIn("ZeroExtend", kinds)

    def test_tentative_long_resolves_to_zero_init(self):
        # `long x;` at file scope is a tentative definition →
        # zero-initialized at end-of-TU. The zero collapses into a
        # `ZeroInit(2)` rather than a typed `LongInit(0)` so codegen
        # can emit `DS.B 2`.
        tac = self._tac("long x; int main(void) { return 0; }")
        statics = {
            tl.name: tl for tl in tac.top_level
            if isinstance(tl, tac_ast.StaticVariable)
        }
        self.assertEqual(statics["x"].data_type, tac_ast.Long())
        self.assertEqual(
            statics["x"].init, [tac_ast.ZeroInit(bytes=2)],
        )

    def test_tentative_int_resolves_to_zero_init(self):
        tac = self._tac("int x; int main(void) { return 0; }")
        statics = {
            tl.name: tl for tl in tac.top_level
            if isinstance(tl, tac_ast.StaticVariable)
        }
        self.assertEqual(statics["x"].data_type, tac_ast.Int())
        self.assertEqual(
            statics["x"].init, [tac_ast.ZeroInit(bytes=1)],
        )

    def test_array_partial_init_coalesces_trailing_zeros(self):
        # `int a[5] = {1};` → IntInit(1) followed by 4 zero bytes
        # (one ZeroInit aggregating four IntInit(0) elements).
        tac = self._tac(
            "int main(void) { static int a[5] = {1}; return 0; }"
        )
        statics = {
            tl.name: tl for tl in tac.top_level
            if isinstance(tl, tac_ast.StaticVariable)
        }
        self.assertEqual(
            statics["@0.a"].init,
            [tac_ast.IntInit(int=1), tac_ast.ZeroInit(bytes=4)],
        )

    def test_multi_dim_array_init_with_zero_holes(self):
        # `long a[3][2] = {{100}, {200, 300}}` — c6502 longs are
        # 2 bytes, so the holes are 2 bytes (a[0][1]) and 4 bytes
        # (entire a[2]).
        tac = self._tac(
            "int main(void) { "
            "static long a[3][2] = {{100}, {200, 300}}; "
            "return 0; }"
        )
        statics = {
            tl.name: tl for tl in tac.top_level
            if isinstance(tl, tac_ast.StaticVariable)
        }
        self.assertEqual(
            statics["@0.a"].init,
            [
                tac_ast.LongInit(int=100),
                tac_ast.ZeroInit(bytes=2),    # a[0][1] hole
                tac_ast.LongInit(int=200),
                tac_ast.LongInit(int=300),
                tac_ast.ZeroInit(bytes=4),    # a[2] entirely missing
            ],
        )

    def test_user_written_zeros_also_coalesce(self):
        # The coalescing is value-driven, not source-tagged: an
        # explicit `{1, 0, 0, 0, 0}` produces the same byte layout
        # as `{1}`, so it folds the same way.
        tac = self._tac(
            "int main(void) { "
            "static int a[5] = {1, 0, 0, 0, 0}; return 0; }"
        )
        statics = {
            tl.name: tl for tl in tac.top_level
            if isinstance(tl, tac_ast.StaticVariable)
        }
        self.assertEqual(
            statics["@0.a"].init,
            [tac_ast.IntInit(int=1), tac_ast.ZeroInit(bytes=4)],
        )

    def test_address_init_does_not_merge_with_zeros(self):
        # AddressInit is symbolic — the assembler resolves it to an
        # address that's not necessarily zero, so it can't fold
        # into a ZeroInit run.
        tac = self._tac(
            "int x; int *p = &x; int main(void) { return 0; }"
        )
        statics = {
            tl.name: tl for tl in tac.top_level
            if isinstance(tl, tac_ast.StaticVariable)
        }
        self.assertEqual(
            statics["p"].init,
            [tac_ast.AddressInit(name="x", offset=0)],
        )

    def test_static_float_zero_uses_zero_init(self):
        # `static float x = 0;` — the int literal converts to a
        # FloatInit(0.0), which folds to ZeroInit(4). Both
        # `= 0` and `= 0.0` produce the same shape.
        for src in (
            "static float x = 0; int main(void) { return 0; }",
            "static float x = 0.0; int main(void) { return 0; }",
        ):
            with self.subTest(src=src):
                tac = self._tac(src)
                statics = {
                    tl.name: tl for tl in tac.top_level
                    if isinstance(tl, tac_ast.StaticVariable)
                }
                self.assertEqual(
                    statics["x"].init, [tac_ast.ZeroInit(bytes=4)],
                )

    def test_static_double_zero_uses_zero_init(self):
        # Same shape for double — DoubleInit(0.0) folds to
        # ZeroInit(8).
        for src in (
            "static double x = 0; int main(void) { return 0; }",
            "static double x = 0.0; int main(void) { return 0; }",
        ):
            with self.subTest(src=src):
                tac = self._tac(src)
                statics = {
                    tl.name: tl for tl in tac.top_level
                    if isinstance(tl, tac_ast.StaticVariable)
                }
                self.assertEqual(
                    statics["x"].init, [tac_ast.ZeroInit(bytes=8)],
                )

    def test_tentative_float_resolves_to_zero_init(self):
        tac = self._tac("float x; int main(void) { return 0; }")
        statics = {
            tl.name: tl for tl in tac.top_level
            if isinstance(tl, tac_ast.StaticVariable)
        }
        self.assertEqual(
            statics["x"].init, [tac_ast.ZeroInit(bytes=4)],
        )

    def test_tentative_double_resolves_to_zero_init(self):
        tac = self._tac("double x; int main(void) { return 0; }")
        statics = {
            tl.name: tl for tl in tac.top_level
            if isinstance(tl, tac_ast.StaticVariable)
        }
        self.assertEqual(
            statics["x"].init, [tac_ast.ZeroInit(bytes=8)],
        )

    def test_static_float_array_zero_init(self):
        # `static float a[3] = {1.5f};` — non-zero leader, then 8
        # bytes of zero pad (two missing FloatInit(0.0)s = 8 bytes).
        tac = self._tac(
            "int main(void) { static float a[3] = {1.5f}; return 0; }"
        )
        statics = {
            tl.name: tl for tl in tac.top_level
            if isinstance(tl, tac_ast.StaticVariable)
        }
        self.assertEqual(
            statics["@0.a"].init,
            [tac_ast.FloatInit(float=1.5), tac_ast.ZeroInit(bytes=8)],
        )

    def test_long_returning_function_implicit_zero_uses_const_long(self):
        # If a Long-returning function falls off the end without a
        # return, the implicit `Ret(0)` uses ConstLong(0) rather
        # than ConstInt(0) so the val matches the declared return
        # type.
        tac = self._tac("long main(void) { }")
        instrs = tac.top_level[0].instructions
        ret = instrs[-1]
        self.assertIsInstance(ret, tac_ast.Ret)
        self.assertIsInstance(ret.val, tac_ast.Constant)
        self.assertIsInstance(ret.val.const, tac_ast.ConstLong)
        self.assertEqual(ret.val.const.int, 0)


class TestTempSymbolTableRegistration(unittest.TestCase):
    """Every `%N` temporary minted during TAC translation is recorded
    in the Translator's symbol table as a `LocalAttr` automatic-
    storage object. The recorded type matches the surrounding
    expression's `data_type` (set by the type checker), so codegen
    can size each temp's frame slot correctly."""

    def _typecheck_and_lower(self, src):
        from compile import _resolved
        from passes.type_checking import check_program
        prog, symbols = check_program(_resolved(src))
        # Reuse the type-checked program's symbol table — the
        # Translator extends it with each temp it mints.
        translate_program(prog, symbols)
        return symbols

    def test_int_binary_temp_registered_as_int(self):
        symbols = self._typecheck_and_lower(
            "int main(void) { int a = 1; int b = 2; return a + b; }"
        )
        # The Binary `a + b` produces a temp; both operands and the
        # result are Int.
        from passes.type_checking import LocalAttr
        for name, sym in symbols.items():
            if not name.startswith("%"):
                continue
            self.assertEqual(sym.type, c99_ast.Int())
            self.assertIsInstance(sym.attrs, LocalAttr)

    def test_long_promotion_temp_registered_as_long(self):
        # `(long)a + b` — a is Int (gets wrapped in implicit
        # Cast(Long, …)), then the Binary's result is Long. The temp
        # for the Cast result and the temp for the Binary result are
        # both Long.
        symbols = self._typecheck_and_lower(
            "int main(void) { int a = 1; long b = (long)2; "
            "return (int)((long)a + b); }"
        )
        # The four temps minted: SignExtend(a)→Long; Binary(+)→Long
        # (after promotion); Truncate of the outer (int) cast→Int;
        # the Cast(long)2 wrapping a constant didn't actually need a
        # SignExtend (no-op identity for that path's Cast on a
        # Constant — the existing _tac const ConstInt(2) is wrapped
        # in `(long)` which the type checker handles as Cast). The
        # exact count isn't load-bearing for the test; we just
        # verify each temp is registered with its right type.
        from passes.type_checking import LocalAttr
        temp_types = {
            name: sym.type for name, sym in symbols.items()
            if name.startswith("%")
        }
        # At least one Long temp (the SignExtend / Binary result)
        # and at least one Int temp (the Truncate result).
        self.assertIn(c99_ast.Long(), temp_types.values())
        self.assertIn(c99_ast.Int(), temp_types.values())
        for sym in (symbols[name] for name in temp_types):
            self.assertIsInstance(sym.attrs, LocalAttr)


class TestPointerArithmeticLowering(unittest.TestCase):
    """C99 §6.5.6 pointer arithmetic lowers via `_pointee_size`-driven
    scaling. ptr ± int multiplies the int by sizeof(pointee) (skipped
    when that's 1) and then does a normal Add/Subtract on the two
    2-byte values. ptr - ptr subtracts the addresses and divides the
    byte-difference by sizeof(pointee) to yield an element count
    (also skipped when sizeof(pointee) == 1)."""

    def _tac(self, src):
        from compile import _resolved
        from passes.type_checking import check_program
        prog, symbols = check_program(_resolved(src))
        return translate_program(prog, symbols)

    def _binary_ops(self, instrs):
        """Pull just the Binary instructions out (filters out the
        Copies / GetAddress / Ret framing that the rest of the body
        produces)."""
        return [i for i in instrs if isinstance(i, tac_ast.Binary)]

    def test_int_ptr_plus_int_no_scaling(self):
        # `int *p + 1` — sizeof(int) is 1, so no Multiply is emitted.
        # The constant 1 is wrapped in an implicit Cast(Long) by the
        # type checker (to match the pointer's 2-byte width); that
        # Cast lowers into a SignExtend producing a Long-typed temp,
        # which then becomes the Add's src2.
        tac = self._tac(
            "long main(void) { int a = 0; int *p = &a; "
            "return (long)(p + 1); }"
        )
        binaries = self._binary_ops(tac.top_level[0].instructions)
        adds = [b for b in binaries if isinstance(b.op, tac_ast.Add)]
        muls = [b for b in binaries if isinstance(b.op, tac_ast.Multiply)]
        self.assertEqual(len(adds), 1)
        self.assertEqual(len(muls), 0)

    def test_long_ptr_plus_int_scales_by_two(self):
        # `long *p + 1` — sizeof(long) is 2, so the int gets
        # multiplied by 2 first, then added to the pointer.
        tac = self._tac(
            "long main(void) { long a = (long)0; long *p = &a; "
            "return (long)(p + 1); }"
        )
        binaries = self._binary_ops(tac.top_level[0].instructions)
        muls = [b for b in binaries if isinstance(b.op, tac_ast.Multiply)]
        adds = [b for b in binaries if isinstance(b.op, tac_ast.Add)]
        self.assertEqual(len(muls), 1)
        self.assertEqual(len(adds), 1)
        # Multiply's src2 is the size constant (2 for long).
        self.assertEqual(
            muls[0].src2,
            tac_ast.Constant(const=tac_ast.ConstLong(int=2)),
        )
        # The Add consumes the multiply's dst as its scaled operand.
        self.assertEqual(adds[0].src2, muls[0].dst)

    def test_double_ptr_plus_int_scales_by_eight(self):
        tac = self._tac(
            "long main(void) { double a = 0.0; double *p = &a; "
            "return (long)(p + 1); }"
        )
        muls = [
            i for i in tac.top_level[0].instructions
            if isinstance(i, tac_ast.Binary)
            and isinstance(i.op, tac_ast.Multiply)
        ]
        self.assertEqual(len(muls), 1)
        self.assertEqual(
            muls[0].src2,
            tac_ast.Constant(const=tac_ast.ConstLong(int=8)),
        )

    def test_int_plus_int_ptr_swaps_operand_order(self):
        # `1 + p` is commutative; the lowering keeps the pointer on
        # the lhs of the underlying Add (consistency, not semantics).
        # The pointer-typed temp is the AddressOf result, so we check
        # by symbol-table type that src1 is Pointer-typed.
        from compile import _resolved
        from passes.type_checking import check_program
        prog, symbols = check_program(_resolved(
            "long main(void) { int a = 0; int *p = &a; "
            "return (long)(1 + p); }"
        ))
        tac = translate_program(prog, symbols)
        adds = [
            i for i in tac.top_level[0].instructions
            if isinstance(i, tac_ast.Binary)
            and isinstance(i.op, tac_ast.Add)
        ]
        self.assertEqual(len(adds), 1)
        # src1 is whatever val produced `p` — could be a Var (the
        # GetAddress dst) or the variable itself. Either way it's
        # pointer-typed.
        src1 = adds[0].src1
        self.assertIsInstance(src1, tac_ast.Var)
        self.assertIsInstance(symbols[src1.name].type, c99_ast.Pointer)

    def test_int_ptr_minus_int_emits_subtract(self):
        tac = self._tac(
            "long main(void) { int a = 0; int *p = &a; "
            "return (long)(p - 1); }"
        )
        binaries = self._binary_ops(tac.top_level[0].instructions)
        subs = [b for b in binaries if isinstance(b.op, tac_ast.Subtract)]
        adds = [b for b in binaries if isinstance(b.op, tac_ast.Add)]
        self.assertEqual(len(subs), 1)
        self.assertEqual(len(adds), 0)

    def test_int_ptr_minus_int_ptr_subtracts_no_divide(self):
        # `int *p - int *q` — sizeof(int) is 1, so the byte-difference
        # IS the element count; no Divide is emitted.
        tac = self._tac(
            "long main(void) { int a = 0; int *p = &a; int *q = &a; "
            "return p - q; }"
        )
        binaries = self._binary_ops(tac.top_level[0].instructions)
        subs = [b for b in binaries if isinstance(b.op, tac_ast.Subtract)]
        divs = [b for b in binaries if isinstance(b.op, tac_ast.Divide)]
        self.assertEqual(len(subs), 1)
        self.assertEqual(len(divs), 0)

    def test_long_ptr_minus_long_ptr_subtracts_then_divides_by_two(self):
        tac = self._tac(
            "long main(void) { long a = (long)0; "
            "long *p = &a; long *q = &a; return p - q; }"
        )
        binaries = self._binary_ops(tac.top_level[0].instructions)
        subs = [b for b in binaries if isinstance(b.op, tac_ast.Subtract)]
        divs = [b for b in binaries if isinstance(b.op, tac_ast.Divide)]
        self.assertEqual(len(subs), 1)
        self.assertEqual(len(divs), 1)
        # Divisor is sizeof(long) = 2.
        self.assertEqual(
            divs[0].src2,
            tac_ast.Constant(const=tac_ast.ConstLong(int=2)),
        )
        # The Divide's src1 is the Sub's dst (chained byte-difference
        # → element-count).
        self.assertEqual(divs[0].src1, subs[0].dst)

    def test_pointer_arithmetic_temp_is_pointer_typed(self):
        # The Binary's dst temp should be Pointer-typed (so codegen
        # sizes it as 2 bytes), not Long.
        from compile import _resolved
        from passes.type_checking import check_program
        prog, symbols = check_program(_resolved(
            "long main(void) { int a = 0; int *p = &a; "
            "return (long)(p + 1); }"
        ))
        translate_program(prog, symbols)
        # Look for any temp typed as Pointer(Int).
        ptr_temps = [
            (name, sym.type) for name, sym in symbols.items()
            if name.startswith("%")
            and isinstance(sym.type, c99_ast.Pointer)
            and sym.type == c99_ast.Pointer(referenced_type=c99_ast.Int())
        ]
        self.assertGreaterEqual(len(ptr_temps), 1)


class TestSubscriptLowering(unittest.TestCase):
    """`a[i]` lowers to address-arithmetic + Load on the rvalue side
    and address-arithmetic + Store on the lvalue side, sharing the
    pointer-arithmetic infrastructure (scaling by sizeof(elem),
    Add to compute byte address). The type checker has already
    decayed array operands and widened indices, so c99_to_tac sees
    the same shape regardless of whether the source was `arr[i]`
    or `ptr[i]`."""

    def _tac(self, src):
        from compile import _resolved
        from passes.type_checking import check_program
        prog, symbols = check_program(_resolved(src))
        return translate_program(prog, symbols)

    def test_subscript_read_emits_load(self):
        # `int a[10]; ... a[3]` — Compute address (GetAddress on a
        # plus an Add of the scaled index), then Load through it.
        # Element type Int has size 1, so no Multiply is needed for
        # the scale.
        tac = self._tac(
            "int main(void) { int a[10]; return a[3]; }"
        )
        instrs = tac.top_level[0].instructions
        kinds = [type(i).__name__ for i in instrs]
        self.assertIn("GetAddress", kinds)
        self.assertIn("Load", kinds)

    def test_subscript_write_emits_store(self):
        # `a[3] = 5;` — same address computation, then Store of the
        # rval through the address.
        tac = self._tac(
            "int main(void) { int a[10]; a[3] = 5; return 0; }"
        )
        instrs = tac.top_level[0].instructions
        kinds = [type(i).__name__ for i in instrs]
        self.assertIn("GetAddress", kinds)
        self.assertIn("Store", kinds)
        # No Load on the write path — we only need the address.
        # (There's a Load if the rval mentions any subscript /
        # dereference, but `5` is a Constant.)
        self.assertNotIn("Load", kinds)

    def test_long_subscript_scales_by_two(self):
        # `long a[10]; ... a[3]` — sizeof(long) = 2, so the index
        # gets a Multiply by 2 before the Add.
        tac = self._tac(
            "long main(void) { long a[10]; return a[3]; }"
        )
        instrs = tac.top_level[0].instructions
        muls = [
            i for i in instrs
            if isinstance(i, tac_ast.Binary)
            and isinstance(i.op, tac_ast.Multiply)
        ]
        self.assertEqual(len(muls), 1)
        self.assertEqual(
            muls[0].src2,
            tac_ast.Constant(const=tac_ast.ConstLong(int=2)),
        )

    def test_array_decay_in_pointer_init(self):
        # `int *p = a;` where a is `int[10]` — the rval decays to
        # `&a`, lowered as a GetAddress instruction. The result is
        # a pointer-typed temp; Copy into `p`.
        tac = self._tac(
            "int main(void) { int a[10]; int *p = a; return 0; }"
        )
        instrs = tac.top_level[0].instructions
        # First instruction is the GetAddress for the decay; second
        # is the Copy into `p`.
        self.assertIsInstance(instrs[0], tac_ast.GetAddress)
        self.assertIsInstance(instrs[1], tac_ast.Copy)


class TestIncrementDecrementLowering(unittest.TestCase):
    """Postfix `a++` / `a--` and prefix `++a` / `--a` share a
    read-modify-write lowering path. For Var operands the operand IS
    the storage; for Subscript / Dereference operands the address is
    computed exactly once (via _translate_subscript_address or
    translate_exp on the pointer expression) and reused for both the
    Load and the Store, so any side effects in the address
    computation fire once."""

    def _tac(self, src):
        from compile import _resolved
        from passes.type_checking import check_program
        prog, symbols = check_program(_resolved(src))
        return translate_program(prog, symbols)

    def test_postfix_on_var_returns_old_value(self):
        tac = self._tac("int main(void) { int a = 0; a++; return a; }")
        instrs = tac.top_level[0].instructions
        # Old value captured into a temp via Copy; new value via
        # Binary; new written back via Copy.
        kinds = [type(i).__name__ for i in instrs]
        self.assertEqual(
            kinds.count("Copy"), 3,  # init, Copy old, Copy new->var
        )

    def test_prefix_on_var_returns_new_value(self):
        # `++a;` — should NOT capture an old value; just Binary +
        # Copy back. So one fewer Copy than postfix.
        tac = self._tac("int main(void) { int a = 0; ++a; return a; }")
        instrs = tac.top_level[0].instructions
        # init copy + new->var copy = 2 (vs. 3 for postfix).
        kinds = [type(i).__name__ for i in instrs]
        self.assertEqual(kinds.count("Copy"), 2)

    def test_postfix_on_subscript_loads_and_stores_through_one_address(self):
        # `a[3]++;` — compute address once, Load, Binary, Store.
        # No second address recomputation.
        tac = self._tac(
            "int main(void) { int a[10]; a[3]++; return 0; }"
        )
        instrs = tac.top_level[0].instructions
        kinds = [type(i).__name__ for i in instrs]
        self.assertEqual(kinds.count("Load"), 1)
        self.assertEqual(kinds.count("Store"), 1)
        self.assertEqual(kinds.count("GetAddress"), 1)

    def test_prefix_on_subscript_with_side_effecting_index_fires_once(self):
        # `++a[i++];` — the `i++` postfix on the index must fire
        # exactly once, even though the subscript is both read and
        # written. Counting Binary(Add)/Binary(Subtract) involving
        # `@N.i` is the cleanest assertion: there should be exactly
        # one (the i++).
        tac = self._tac(
            "int main(void) { int a[10]; int i = 0; ++a[i++]; "
            "return 0; }"
        )
        instrs = tac.top_level[0].instructions
        # Count Binary ops whose either src references the renamed
        # `i` (identifier_resolution renames it to `@<N>.i`).
        i_binaries = [
            i for i in instrs
            if isinstance(i, tac_ast.Binary)
            and any(
                isinstance(s, tac_ast.Var) and s.name.endswith(".i")
                for s in (i.src1, i.src2)
            )
        ]
        # Exactly two Binary ops touch `i`: one is the i++ (Add)
        # that produces the new value of i, and one is the
        # pointer-arithmetic Add that computes a + i_old. (No
        # second i++ — the side effect fired only once.) But the
        # pointer arithmetic uses the captured old value (a temp,
        # not `i` itself), so we expect exactly ONE Binary that
        # references `i` directly: the i++ itself.
        self.assertEqual(len(i_binaries), 1)
        self.assertIsInstance(i_binaries[0].op, tac_ast.Add)

    def test_postfix_on_dereference(self):
        # `(*p)++;` — evaluate `p` once, Load, Binary, Store through
        # the same pointer.
        tac = self._tac(
            "int main(void) { int a; int *p = &a; (*p)++; return 0; }"
        )
        instrs = tac.top_level[0].instructions
        kinds = [type(i).__name__ for i in instrs]
        self.assertGreaterEqual(kinds.count("Load"), 1)
        self.assertGreaterEqual(kinds.count("Store"), 1)


class TestArrayInitList(unittest.TestCase):
    """`int a[N] = {e1, e2, ...}` lowers to GetAddress + a sequence
    of Stores at compile-time offsets into the array's frame slot.
    Missing trailing items are zero-padded per C99 §6.7.8.21."""

    def _tac(self, src):
        from compile import _resolved
        from passes.type_checking import check_program
        prog, symbols = check_program(_resolved(src))
        return translate_program(prog, symbols)

    def test_full_init_emits_n_stores(self):
        # `int a[3] = {1, 2, 3}` — exactly 3 Stores, one per item.
        tac = self._tac(
            "int main(void) { int a[3] = {1, 2, 3}; return 0; }"
        )
        instrs = tac.top_level[0].instructions
        stores = [i for i in instrs if isinstance(i, tac_ast.Store)]
        self.assertEqual(len(stores), 3)
        # Stored values are the three constants in order.
        self.assertEqual(
            stores[0].src,
            tac_ast.Constant(const=tac_ast.ConstInt(int=1)),
        )
        self.assertEqual(
            stores[1].src,
            tac_ast.Constant(const=tac_ast.ConstInt(int=2)),
        )
        self.assertEqual(
            stores[2].src,
            tac_ast.Constant(const=tac_ast.ConstInt(int=3)),
        )

    def test_partial_init_pads_with_zeros(self):
        # `int a[5] = {1, 2}` — 5 Stores: items 0/1 from the source,
        # items 2/3/4 are ConstInt(0).
        tac = self._tac(
            "int main(void) { int a[5] = {1, 2}; return 0; }"
        )
        stores = [
            i for i in tac.top_level[0].instructions
            if isinstance(i, tac_ast.Store)
        ]
        self.assertEqual(len(stores), 5)
        for i, expected_val in enumerate([1, 2, 0, 0, 0]):
            self.assertEqual(
                stores[i].src,
                tac_ast.Constant(const=tac_ast.ConstInt(int=expected_val)),
            )

    def test_init_emits_get_address(self):
        # The first instruction is a GetAddress for the array.
        tac = self._tac(
            "int main(void) { int a[3] = {1, 2, 3}; return 0; }"
        )
        first = tac.top_level[0].instructions[0]
        self.assertIsInstance(first, tac_ast.GetAddress)
        self.assertEqual(first.operand, tac_ast.Var(name="@0.a"))

    def test_long_array_init_uses_long_offsets(self):
        # `long a[3] = {1L, 2L, 3L}` — each element occupies 2 bytes,
        # so the address Adds use offsets 2 and 4 (the first
        # element's address is the base; no Add for it).
        tac = self._tac(
            "int main(void) { long a[3] = {1L, 2L, 3L}; return 0; }"
        )
        adds = [
            i for i in tac.top_level[0].instructions
            if isinstance(i, tac_ast.Binary)
            and isinstance(i.op, tac_ast.Add)
        ]
        # Two Adds: base+2 (for index 1) and base+4 (for index 2).
        self.assertEqual(len(adds), 2)
        offsets = sorted(a.src2.const.int for a in adds)
        self.assertEqual(offsets, [2, 4])


class TestAddressOfSubscript(unittest.TestCase):
    """`&a[i]` ≡ `a + i` per C99 §6.5.3.2.3. Lowers via the same
    `_translate_subscript_address` helper as the rvalue Subscript
    path, but skips the trailing Load — so it produces exactly the
    TAC shape that `a + i` would (scaled Add of the index to the
    pointer base, no Load)."""

    def _tac(self, src):
        from compile import _resolved
        from passes.type_checking import check_program
        prog, symbols = check_program(_resolved(src))
        return translate_program(prog, symbols)

    def test_address_of_int_subscript_no_load(self):
        # `int *p; &p[3]` — sizeof(int)=1, so no Multiply; one Add
        # for the scaled index; no Load (we want the address, not
        # the value).
        tac = self._tac(
            "long main(void) { int x = 0; int *p = &x; "
            "int *q = &p[3]; return (long)q; }"
        )
        instrs = tac.top_level[0].instructions
        kinds = [type(i).__name__ for i in instrs]
        # No Load anywhere — `&p[3]` is just an address calculation.
        self.assertNotIn("Load", kinds)

    def test_address_of_long_subscript_emits_scale(self):
        # `long *p; &p[3]` — sizeof(long)=2, so a Multiply scales
        # the index by 2; one Add adds it to the base; no Load.
        tac = self._tac(
            "long main(void) { long x = 0L; long *p = &x; "
            "long *q = &p[3]; return (long)q; }"
        )
        muls = [
            i for i in tac.top_level[0].instructions
            if isinstance(i, tac_ast.Binary)
            and isinstance(i.op, tac_ast.Multiply)
        ]
        self.assertEqual(len(muls), 1)
        self.assertEqual(
            muls[0].src2,
            tac_ast.Constant(const=tac_ast.ConstLong(int=2)),
        )
        # No Load.
        loads = [
            i for i in tac.top_level[0].instructions
            if isinstance(i, tac_ast.Load)
        ]
        self.assertEqual(len(loads), 0)

    def test_address_of_subscript_matches_pointer_add(self):
        # `&p[3]` and `p + 3` should produce structurally identical
        # TAC instruction sequences — both lower to "scale 3 by
        # sizeof(*p), add to p". Compare the two functions to verify.
        tac = self._tac(
            "int main(void) { "
            "long x = 0L; long *p = &x; "
            "long *q1 = &p[3]; "
            "long *q2 = p + 3; "
            "return 0; }"
        )
        instrs = tac.top_level[0].instructions
        # Pull the q1 and q2 init segments by tracking Copy
        # destinations to @0.q1 / @0.q2. Easier: just compare the
        # set of Binary opcodes used for each — both must have the
        # same Multiply + Add pair scaling by 2.
        binaries = [i for i in instrs if isinstance(i, tac_ast.Binary)]
        muls = [b for b in binaries if isinstance(b.op, tac_ast.Multiply)]
        adds = [b for b in binaries if isinstance(b.op, tac_ast.Add)]
        # Two scales (one per init), two adds (one per init).
        self.assertEqual(len(muls), 2)
        self.assertEqual(len(adds), 2)
        # Both Multiply scales are by 2.
        for m in muls:
            self.assertEqual(
                m.src2,
                tac_ast.Constant(const=tac_ast.ConstLong(int=2)),
            )


class TestMultiDimArrays(unittest.TestCase):
    """Multi-dim subscript chains and nested init lists. Subscript
    on a multi-dim array threads through the type checker's array
    decay (each inner result decays to a pointer for the outer
    Subscript), which c99_to_tac handles via a new `AddressOf(
    Subscript)` case in `translate_exp`. Nested init lists recurse
    in `_emit_init_stores`, accumulating byte offsets for each
    leaf."""

    def _tac(self, src):
        from compile import _resolved
        from passes.type_checking import check_program
        prog, symbols = check_program(_resolved(src))
        return translate_program(prog, symbols)

    def test_two_dim_subscript_lowers_with_two_scales(self):
        # `int a[3][4]; a[i][j]` — outer dimension scales by
        # sizeof(int[4]) = 4, inner by sizeof(int) = 1 (no scale
        # emitted at the inner level since size 1).
        tac = self._tac(
            "int main(void) { int a[3][4]; return a[1][2]; }"
        )
        instrs = tac.top_level[0].instructions
        muls = [
            i for i in instrs
            if isinstance(i, tac_ast.Binary)
            and isinstance(i.op, tac_ast.Multiply)
        ]
        # Exactly one Multiply — for the outer dimension's index 1 ×
        # sizeof(int[4]) = 4. The inner index doesn't need a Multiply
        # because sizeof(int) is 1.
        self.assertEqual(len(muls), 1)
        self.assertEqual(
            muls[0].src2,
            tac_ast.Constant(const=tac_ast.ConstLong(int=4)),
        )
        # Final Load is for sizeof(int) = 1 byte (size dispatched
        # by the dst's type).
        loads = [i for i in instrs if isinstance(i, tac_ast.Load)]
        self.assertEqual(len(loads), 1)

    def test_two_dim_long_subscript_scales_outer_by_six(self):
        # `long a[2][3]; a[i][j]` — outer dimension scales by
        # sizeof(long[3]) = 6, inner by sizeof(long) = 2.
        tac = self._tac(
            "long main(void) { long a[2][3]; return a[1][2]; }"
        )
        muls = [
            i for i in tac.top_level[0].instructions
            if isinstance(i, tac_ast.Binary)
            and isinstance(i.op, tac_ast.Multiply)
        ]
        # Two Multiplies — outer × 6, inner × 2.
        self.assertEqual(len(muls), 2)
        scales = sorted(m.src2.const.int for m in muls)
        self.assertEqual(scales, [2, 6])

    def test_two_dim_init_emits_six_stores(self):
        # `int a[2][3] = {{1,2,3},{4,5,6}};` — six leaf Stores at
        # offsets 0, 1, 2, 3, 4, 5.
        tac = self._tac(
            "int main(void) { int a[2][3] = {{1,2,3},{4,5,6}}; return 0; }"
        )
        stores = [
            i for i in tac.top_level[0].instructions
            if isinstance(i, tac_ast.Store)
        ]
        self.assertEqual(len(stores), 6)
        # Stored values are 1..6 in row-major order.
        for i, expected in enumerate([1, 2, 3, 4, 5, 6]):
            self.assertEqual(
                stores[i].src,
                tac_ast.Constant(const=tac_ast.ConstInt(int=expected)),
            )

    def test_partial_two_dim_init_zero_pads(self):
        # `int a[2][3] = {{1, 2}};` — outer item 0 has only 2 inner
        # items (third zero-padded); outer item 1 missing entirely
        # (all three inner zero-padded). Total: 6 Stores with
        # values [1, 2, 0, 0, 0, 0].
        tac = self._tac(
            "int main(void) { int a[2][3] = {{1, 2}}; return 0; }"
        )
        stores = [
            i for i in tac.top_level[0].instructions
            if isinstance(i, tac_ast.Store)
        ]
        self.assertEqual(len(stores), 6)
        for i, expected in enumerate([1, 2, 0, 0, 0, 0]):
            self.assertEqual(
                stores[i].src,
                tac_ast.Constant(const=tac_ast.ConstInt(int=expected)),
            )

    def test_two_dim_init_uses_correct_byte_offsets(self):
        # `long a[2][3] = {{1L,2L,3L},{4L,5L,6L}};` — sizeof(long)=2,
        # so leaves are at offsets 0, 2, 4, 6, 8, 10.
        tac = self._tac(
            "int main(void) { "
            "long a[2][3] = {{1L,2L,3L},{4L,5L,6L}}; return 0; }"
        )
        adds = [
            i for i in tac.top_level[0].instructions
            if isinstance(i, tac_ast.Binary)
            and isinstance(i.op, tac_ast.Add)
        ]
        # Five Adds for the five non-zero offsets (offset 0 is the
        # base itself, no Add).
        self.assertEqual(len(adds), 5)
        offsets = sorted(a.src2.const.int for a in adds)
        self.assertEqual(offsets, [2, 4, 6, 8, 10])


class TestArrayFrameLayout(unittest.TestCase):
    """An array's frame slot is `sizeof(elem) * count` contiguous
    bytes; replace_pseudoregisters' `_size_of_name` reads the c99
    Array type from the symbol table to get this. End-to-end check
    via the prologue's `local_bytes`."""

    def _local_bytes(self, src):
        from compile import _resolved
        from passes.type_checking import check_program
        from passes.replace_pseudoregisters import replace_program
        from c99_to_tac import translate_program as c99_to_tac_translate
        from tac_to_asm import translate_program as tac_to_asm_translate
        prog, symbols = check_program(_resolved(src))
        tac = c99_to_tac_translate(prog, symbols)
        asm = tac_to_asm_translate(tac, symbols)
        asm = replace_program(asm, symbols=symbols)
        prologue = next(
            i for i in asm.top_level[0].instructions
            if hasattr(i, "local_bytes")
        )
        return prologue.local_bytes

    def test_int_array_takes_at_least_count_bytes(self):
        # `int a[10]` — 10 bytes for the array, plus pointer-typed
        # temps from the address arithmetic. Just check the array's
        # contribution is included (>= 10 bytes total).
        n = self._local_bytes(
            "int main(void) { int a[10]; a[0] = 1; return 0; }"
        )
        self.assertGreaterEqual(n, 10)

    def test_long_array_takes_at_least_two_x_count_bytes(self):
        # `long a[5]` — 10 bytes for the array (5 × sizeof(long)).
        n = self._local_bytes(
            "long main(void) { long a[5]; a[0] = (long)1; return (long)0; }"
        )
        self.assertGreaterEqual(n, 10)

    def test_arrays_of_different_sizes_differ_by_size_difference(self):
        # `int a[10]` vs. `int a[20]` — same body shape, only the
        # array size differs. The `local_bytes` difference must be
        # exactly 10 bytes (the extra elements).
        small = self._local_bytes(
            "int main(void) { int a[10]; a[0] = 1; return 0; }"
        )
        big = self._local_bytes(
            "int main(void) { int a[20]; a[0] = 1; return 0; }"
        )
        self.assertEqual(big - small, 10)


if __name__ == "__main__":
    unittest.main()
