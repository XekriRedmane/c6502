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
        decls.append(c99_ast.FunctionDecl(
            function_decl=c99_ast.Type_function_decl(
                name=fn.name,
                params=list(fn.params),
                body=fn.body,
                storage_class=None,
            ),
        ))
    return c99_ast.Program(declaration=decls)


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
        # identifier_resolution should have rejected this; the runtime
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
            c99_ast.VarDecl(var_decl=c99_ast.Type_var_decl(
                name="@0.x", init=None,
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
                name="@0.x", init=c99_ast.Constant(value=5),
            )),
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
            c99_ast.VarDecl(var_decl=c99_ast.Type_var_decl(
                name="@0.x",
                init=c99_ast.Binary(
                    op=c99_ast.Add(),
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                ),
            )),
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
            c99_ast.D(declaration=c99_ast.VarDecl(
                var_decl=c99_ast.Type_var_decl(
                    name="@0.x", init=c99_ast.Constant(value=7),
                ),
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
                condition=c99_ast.Constant(value=1),
                then_clause=c99_ast.Return(exp=c99_ast.Constant(value=2)),
                else_clause=None,
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(value=1),
                target=".if_end@0",
            ),
            tac_ast.Ret(val=tac_ast.Constant(value=2)),
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
                condition=c99_ast.Constant(value=1),
                then_clause=c99_ast.Return(exp=c99_ast.Constant(value=2)),
                else_clause=c99_ast.Return(exp=c99_ast.Constant(value=3)),
            ),
            instrs,
        )
        # end_label is minted before else_label (translate_exp doesn't
        # mint labels for a Constant, so the first make_label call is
        # for if_end -> if_end@0, then if_else -> if_else@1).
        self.assertEqual(instrs, [
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(value=1),
                target=".if_else@1",
            ),
            tac_ast.Ret(val=tac_ast.Constant(value=2)),
            tac_ast.Jump(target=".if_end@0"),
            tac_ast.Label(name=".if_else@1"),
            tac_ast.Ret(val=tac_ast.Constant(value=3)),
            tac_ast.Label(name=".if_end@0"),
        ])

    def test_nested_if_each_gets_unique_labels(self):
        # `if (1) if (2) return 3;` — outer mints if_end@0, inner mints
        # if_end@1.
        t = Translator()
        instrs: list = []
        t.translate_statement(
            c99_ast.IfStmt(
                condition=c99_ast.Constant(value=1),
                then_clause=c99_ast.IfStmt(
                    condition=c99_ast.Constant(value=2),
                    then_clause=c99_ast.Return(
                        exp=c99_ast.Constant(value=3),
                    ),
                    else_clause=None,
                ),
                else_clause=None,
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(value=1),
                target=".if_end@0",
            ),
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(value=2),
                target=".if_end@1",
            ),
            tac_ast.Ret(val=tac_ast.Constant(value=3)),
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
                statement=c99_ast.Return(exp=c99_ast.Constant(value=0)),
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Label(name=".main@foo"),
            tac_ast.Ret(val=tac_ast.Constant(value=0)),
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
                tac_ast.Ret(val=tac_ast.Constant(value=0)),
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
                        name="@0.x", init=c99_ast.Constant(value=1),
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
                src=tac_ast.Constant(value=1),
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
                            exp=c99_ast.Constant(value=1),
                        )),
                    ]),
                )),
            ])),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Ret(val=tac_ast.Constant(value=1)),
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
                        exp=c99_ast.Constant(value=7),
                    )),
                ]),
            )),
        ]))
        self.assertEqual(
            Translator().translate_function(fn).instructions,
            [tac_ast.Ret(val=tac_ast.Constant(value=7))],
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
                        name="@1.x", init=c99_ast.Constant(value=2),
                    ),
                )),
            ]),
        ), instrs)
        # Just the inner Compound's effect.
        self.assertEqual(instrs, [
            tac_ast.Copy(
                src=tac_ast.Constant(value=2),
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
                condition=c99_ast.Constant(value=1),
                then_clause=c99_ast.Compound(
                    block=c99_ast.Block(block_item=[
                        c99_ast.S(statement=c99_ast.Return(
                            exp=c99_ast.Constant(value=2),
                        )),
                    ]),
                ),
                else_clause=None,
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(value=1),
                target=".if_end@0",
            ),
            tac_ast.Ret(val=tac_ast.Constant(value=2)),
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
                condition=c99_ast.Constant(value=1),
                true_clause=c99_ast.Constant(value=2),
                false_clause=c99_ast.Constant(value=3),
            ),
            instrs,
        )
        # Labels mint first (cond_else@0, cond_end@1), then the dst
        # temp (%0). Both arms Copy into %0; the outer expression
        # returns %0 so chained uses see the chosen value.
        self.assertEqual(val, tac_ast.Var(name="%0"))
        self.assertEqual(instrs, [
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(value=1),
                target=".cond_else@0",
            ),
            tac_ast.Copy(
                src=tac_ast.Constant(value=2),
                dst=tac_ast.Var(name="%0"),
            ),
            tac_ast.Jump(target=".cond_end@1"),
            tac_ast.Label(name=".cond_else@0"),
            tac_ast.Copy(
                src=tac_ast.Constant(value=3),
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
                condition=c99_ast.Constant(value=1),
                true_clause=c99_ast.Conditional(
                    condition=c99_ast.Constant(value=2),
                    true_clause=c99_ast.Constant(value=3),
                    false_clause=c99_ast.Constant(value=4),
                ),
                false_clause=c99_ast.Constant(value=5),
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
                instructions=[tac_ast.Ret(val=tac_ast.Constant(value=0))],
            ),
        )

    def test_body_without_return_gets_implicit_return_zero(self):
        fn = c99_ast.Function(name="main", body=c99_ast.Block(block_item=[
            c99_ast.D(declaration=c99_ast.VarDecl(
                var_decl=c99_ast.Type_var_decl(
                    name="@0.x", init=c99_ast.Constant(value=5),
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
        fn = c99_ast.Function(name="main", body=c99_ast.Block(block_item=[
            c99_ast.S(statement=c99_ast.Return(
                exp=c99_ast.Constant(value=7),
            )),
        ]))
        self.assertEqual(
            Translator().translate_function(fn).instructions,
            [tac_ast.Ret(val=tac_ast.Constant(value=7))],
        )

    def test_null_after_return_does_not_trigger_second_return(self):
        # Null emits nothing, so the last emitted instruction is still
        # Ret — no implicit zero-return appended.
        fn = c99_ast.Function(name="main", body=c99_ast.Block(block_item=[
            c99_ast.S(statement=c99_ast.Return(
                exp=c99_ast.Constant(value=7),
            )),
            c99_ast.S(statement=c99_ast.Null()),
        ]))
        self.assertEqual(
            Translator().translate_function(fn).instructions,
            [tac_ast.Ret(val=tac_ast.Constant(value=7))],
        )

    def test_block_items_processed_in_order(self):
        # Two declarations, then a return.
        fn = c99_ast.Function(name="main", body=c99_ast.Block(block_item=[
            c99_ast.D(declaration=c99_ast.VarDecl(
                var_decl=c99_ast.Type_var_decl(
                    name="@0.a", init=c99_ast.Constant(value=1),
                ),
            )),
            c99_ast.D(declaration=c99_ast.VarDecl(
                var_decl=c99_ast.Type_var_decl(
                    name="@1.b", init=c99_ast.Constant(value=2),
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
        # Both c99 and TAC programs are lists of Functions now, so
        # the `[Function(...)]` wrapping mirrors on both sides.
        prog = _c99_program_of(c99_ast.Function(
            name="main",
            body=c99_ast.Block(block_item=[c99_ast.S(statement=c99_ast.Return(
                exp=c99_ast.Constant(value=42),
            ))]),
        ))
        self.assertEqual(
            translate_program(prog),
            tac_ast.Program(top_level=[tac_ast.Function(
                name="main",
                is_global=True,
                params=[],
                instructions=[tac_ast.Ret(val=tac_ast.Constant(value=42))],
            )]),
        )

    def test_return_unary(self):
        tac = translate_program(_c99_program_of(c99_ast.Function(
                name="main",
                body=c99_ast.Block(block_item=[c99_ast.S(
                    statement=c99_ast.Return(exp=c99_ast.Unary(
                        op=c99_ast.Negate(),
                        exp=c99_ast.Constant(value=5),
                    )),
                )]),
            )))
        self.assertEqual(
            tac.top_level[0].instructions,
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
            tac.top_level[0].instructions,
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
                src2=tac_ast.Constant(value=1),
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
                src2=tac_ast.Constant(value=1),
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
                    operand=c99_ast.Constant(value=1),
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
            tac.top_level[0].instructions,
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
                condition=c99_ast.Constant(value=1),
                body=c99_ast.Null(),
                label=".loop@0",
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Label(name=".loop@0_continue"),
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(value=1),
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
                condition=c99_ast.Constant(value=1),
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
                condition=tac_ast.Constant(value=1),
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
                condition=c99_ast.Constant(value=1),
                label=".loop@0",
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Label(name=".loop@0_start"),
            tac_ast.Label(name=".loop@0_continue"),
            tac_ast.JumpIfTrue(
                condition=tac_ast.Constant(value=1),
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
                condition=c99_ast.Constant(value=1),
                label=".loop@0",
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Label(name=".loop@0_start"),
            tac_ast.Jump(target=".loop@0_continue"),
            tac_ast.Label(name=".loop@0_continue"),
            tac_ast.JumpIfTrue(
                condition=tac_ast.Constant(value=1),
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
                    rval=c99_ast.Constant(value=0),
                )),
                condition=c99_ast.Constant(value=1),
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
                src=tac_ast.Constant(value=0),
                dst=tac_ast.Var(name="i"),
            ),
            tac_ast.Label(name=".loop@0_start"),
            # No condition insns needed for a Constant — JumpIfFalse
            # takes the Constant directly.
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(value=1),
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
                src2=tac_ast.Constant(value=1),
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
                    name="i", init=c99_ast.Constant(value=0),
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
                src=tac_ast.Constant(value=0),
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
                condition=c99_ast.Constant(value=1),
                post_clause=None,
                body=c99_ast.Null(),
                label=".loop@0",
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Label(name=".loop@0_start"),
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(value=1),
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
                condition=c99_ast.Constant(value=1),
                body=c99_ast.Compound(block=c99_ast.Block(block_item=[
                    c99_ast.S(statement=c99_ast.WhileStmt(
                        condition=c99_ast.Constant(value=1),
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
                condition=tac_ast.Constant(value=1),
                target=".loop@0_break",
            ),
            # Inner while.
            tac_ast.Label(name=".loop@1_continue"),
            tac_ast.JumpIfFalse(
                condition=tac_ast.Constant(value=1),
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
                args=[c99_ast.Constant(value=1), c99_ast.Constant(value=2)],
            ),
            instrs,
        )
        self.assertEqual(result, tac_ast.Var(name="%0"))
        self.assertEqual(instrs, [
            tac_ast.FunctionCall(
                name="f",
                args=[
                    tac_ast.Constant(value=1),
                    tac_ast.Constant(value=2),
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
                    left=c99_ast.Constant(value=1),
                    right=c99_ast.Constant(value=2),
                )],
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.Binary(
                op=tac_ast.Add(),
                src1=tac_ast.Constant(value=1),
                src2=tac_ast.Constant(value=2),
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
                        args=[c99_ast.Constant(value=1)],
                    ),
                    c99_ast.FunctionCall(
                        name="g",
                        args=[c99_ast.Constant(value=2)],
                    ),
                ],
            ),
            instrs,
        )
        self.assertEqual(instrs, [
            tac_ast.FunctionCall(
                name="g",
                args=[tac_ast.Constant(value=1)],
                dst=tac_ast.Var(name="%0"),
            ),
            tac_ast.FunctionCall(
                name="g",
                args=[tac_ast.Constant(value=2)],
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
        self.assertEqual(call.args, [tac_ast.Constant(value=42)])
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
                [tac_ast.Ret(val=tac_ast.Constant(value=0))],
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


if __name__ == "__main__":
    unittest.main()
