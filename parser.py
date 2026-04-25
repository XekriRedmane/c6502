"""Parser for C99 — builds c99_ast nodes from the Lark parse tree.

Adding a grammar rule:
  1. Add/modify the rule in c99.lark.
  2. If the rule has alternatives that map to different AST constructors,
     give each alternative a name with `-> name`:
         statement: RETURN exp SEMICOLON            -> return_stmt
                  | IF LPAREN exp RPAREN statement  -> if_stmt
  3. Add a Transformer method in `_ASTBuilder` with the same name as the
     rule (or the alternative). The method receives the rule's items —
     subtrees already converted to AST nodes, terminals as Lark tokens —
     and returns the AST node for that rule.

@v_args(inline=True) spreads the rule's items into named parameters so
each method's signature mirrors the rule body. Unused items (the
punctuator terminals) are conventionally prefixed with `_`.
"""

from __future__ import annotations

from pathlib import Path

from lark import Lark, Transformer
from lark.visitors import v_args

import c99_ast


_GRAMMAR_PATH = Path(__file__).parent / "c99.lark"
_LARK = Lark.open(
    str(_GRAMMAR_PATH),
    parser="lalr",
    lexer="basic",
    start=["start", "lex_only"],
)


# Compound-assignment operator tokens → AST binary-operator class. The
# parser desugars `lval OP= rval` into `lval = lval OP rval`, so each
# compound operator just needs to name the binary op it expands to.
_COMPOUND_ASSIGN_OPS = {
    "PLUS_ASSIGN":    c99_ast.Add,
    "MINUS_ASSIGN":   c99_ast.Subtract,
    "STAR_ASSIGN":    c99_ast.Multiply,
    "SLASH_ASSIGN":   c99_ast.Divide,
    "PERCENT_ASSIGN": c99_ast.Modulo,
    "AMP_ASSIGN":     c99_ast.BitwiseAnd,
    "PIPE_ASSIGN":    c99_ast.BitwiseOr,
    "CARET_ASSIGN":   c99_ast.BitwiseXor,
    "LSHIFT_ASSIGN":  c99_ast.LeftShift,
    "RSHIFT_ASSIGN":  c99_ast.RightShift,
}


class _ASTBuilder(Transformer):
    @v_args(inline=True)
    def start(self, function):
        return c99_ast.Program(function_definition=function)

    @v_args(inline=True)
    def function(self, _int, name, _lp, _void, _rp, body):
        # The grammar's `function: INT IDENTIFIER LPAREN VOID RPAREN
        # block` rule hands us a fully-built Block as the body.
        return c99_ast.Function(name=str(name), body=body)

    def block(self, items):
        # `block: LBRACE block_item* RBRACE`. Non-inline because
        # block_item* expands to a variable number of children.
        return c99_ast.Block(block_item=list(items[1:-1]))

    # Alternatives of `block_item` — wrap a statement / declaration.
    @v_args(inline=True)
    def stmt_item(self, statement):
        return c99_ast.S(statement=statement)

    @v_args(inline=True)
    def decl_item(self, declaration):
        return c99_ast.D(declaration=declaration)

    # `declaration: INT IDENTIFIER (ASSIGN exp)? SEMICOLON`. The optional
    # initializer makes the children variable-length (5 tokens with init,
    # 3 without), so non-inline again.
    def declaration(self, items):
        name = items[1]
        init = items[3] if len(items) == 5 else None
        return c99_ast.Declaration(name=str(name), init=init)

    # Alternatives of `statement` — each named in c99.lark.
    @v_args(inline=True)
    def return_stmt(self, _return, exp, _semi):
        return c99_ast.Return(exp=exp)

    @v_args(inline=True)
    def expression_stmt(self, exp, _semi):
        return c99_ast.Expression(exp=exp)

    # `if (exp) stmt` (4 children) or `if (exp) stmt else stmt` (6
    # children). The else-branch is variable, so non-inline.
    def if_stmt(self, items):
        condition = items[2]
        then_clause = items[4]
        else_clause = items[6] if len(items) == 7 else None
        return c99_ast.IfStmt(
            condition=condition,
            then_clause=then_clause,
            else_clause=else_clause,
        )

    @v_args(inline=True)
    def goto_stmt(self, _goto, identifier, _semi):
        return c99_ast.Goto(label=str(identifier))

    @v_args(inline=True)
    def labeled_stmt(self, identifier, _colon, stmt):
        return c99_ast.LabeledStmt(label=str(identifier), statement=stmt)

    @v_args(inline=True)
    def compound_stmt(self, block):
        # `{ ... }` as a statement. `block` has already been built
        # into a `Block` by the `block` transformer; wrap it in a
        # `Compound` so it fits the `statement` sum.
        return c99_ast.Compound(block=block)

    @v_args(inline=True)
    def null_stmt(self, _semi):
        return c99_ast.Null()

    # Loop and jump statements. Loop labels are minted by the
    # loop_labeling pass that runs after variable_resolution; the
    # parser leaves them as empty strings.
    @v_args(inline=True)
    def break_stmt(self, _break, _semi):
        return c99_ast.BreakStmt(label="")

    @v_args(inline=True)
    def continue_stmt(self, _continue, _semi):
        return c99_ast.ContinueStmt(label="")

    @v_args(inline=True)
    def while_stmt(self, _while, _lp, cond, _rp, body):
        return c99_ast.WhileStmt(condition=cond, body=body, label="")

    @v_args(inline=True)
    def do_stmt(self, _do, body, _while, _lp, cond, _rp, _semi):
        return c99_ast.DoWhileStmt(body=body, condition=cond, label="")

    # `for_init: declaration | exp? SEMICOLON`. The declaration alternative
    # already consumes its own SEMICOLON, so a declaration arrives as the
    # only child. The exp-or-empty alternative carries an explicit
    # SEMICOLON token: zero or one preceding exp child, then SEMICOLON.
    def for_init(self, items):
        if len(items) == 1:
            child = items[0]
            if isinstance(child, c99_ast.Declaration):
                return c99_ast.InitDecl(declaration=child)
            # Bare SEMICOLON — empty for-init clause.
            return c99_ast.InitExp(exp=None)
        # exp + SEMICOLON.
        return c99_ast.InitExp(exp=items[0])

    # `for (for_init exp? ; exp?) statement` — for_init contributes one
    # child that already swallowed the first SEMICOLON; the middle SEMI
    # between the condition and post_clause is in our items list. Each
    # of condition / post_clause is independently optional, so we scan
    # for the SEMICOLON to know which side each remaining child is on.
    def for_stmt(self, items):
        # items: [FOR, LPAREN, for_init, condition?, SEMICOLON, post_clause?,
        #         RPAREN, statement]
        init = items[2]
        body = items[-1]
        middle = items[3:-2]
        semi_idx = next(
            i for i, c in enumerate(middle)
            if hasattr(c, "type") and c.type == "SEMICOLON"
        )
        condition = middle[0] if semi_idx > 0 else None
        post_clause = (
            middle[semi_idx + 1] if semi_idx + 1 < len(middle) else None
        )
        return c99_ast.ForStmt(
            init=init,
            condition=condition,
            post_clause=post_clause,
            body=body,
            label="",
        )

    # Alternatives of `exp` — each named in c99.lark.
    @v_args(inline=True)
    def constant(self, token):
        return c99_ast.Constant(value=int(str(token)))

    @v_args(inline=True)
    def identifier(self, token):
        return c99_ast.Var(name=str(token))

    @v_args(inline=True)
    def assignment(self, lval, _assign, rval):
        return c99_ast.Assignment(lval=lval, rval=rval)

    @v_args(inline=True)
    def compound_assign(self, lval, op_token, rval):
        # `lval OP= rval` desugars at parse time to `lval = lval OP rval`.
        # The lval node is duplicated as a tree reference (Assignment.lval
        # and Binary.left point at the same Python object). That's safe
        # today because the only legal lval is a `Var`, which has no
        # side effect when re-evaluated. When richer lvalues (`*p`,
        # `a[i]`, `s.f`) land, this rewrite has to materialize the
        # address into a temp instead so the lval is evaluated once.
        op_cls = _COMPOUND_ASSIGN_OPS[op_token.type]
        return c99_ast.Assignment(
            lval=lval,
            rval=c99_ast.Binary(op=op_cls(), left=lval, right=rval),
        )

    @v_args(inline=True)
    def unary(self, op, inner):
        return c99_ast.Unary(op=op, exp=inner)

    # Prefix `++a` / `--a` desugar to `a = a ± 1` (same shape as
    # `a += 1` / `a -= 1`). The lval node is duplicated by reference
    # — safe today because the only legal lval is a `Var`, which has
    # no side effect when re-evaluated. Future richer lvalues need a
    # rewrite that materializes the address into a temp first.
    @v_args(inline=True)
    def pre_increment(self, _op, operand):
        return self._prefix_incdec(c99_ast.Add(), operand)

    @v_args(inline=True)
    def pre_decrement(self, _op, operand):
        return self._prefix_incdec(c99_ast.Subtract(), operand)

    def _prefix_incdec(self, op, operand):
        return c99_ast.Assignment(
            lval=operand,
            rval=c99_ast.Binary(
                op=op, left=operand, right=c99_ast.Constant(value=1),
            ),
        )

    # Postfix `a++` / `a--` keep their own AST node because they have
    # to return the *old* value of the operand while also mutating
    # it. The lvalue check (operand must be a `Var`) lives in
    # variable_resolution alongside the Assignment check.
    @v_args(inline=True)
    def post_increment(self, operand, _op):
        return c99_ast.Postfix(op=c99_ast.Increment(), operand=operand)

    @v_args(inline=True)
    def post_decrement(self, operand, _op):
        return c99_ast.Postfix(op=c99_ast.Decrement(), operand=operand)

    @v_args(inline=True)
    def paren(self, _lp, inner, _rp):
        return inner

    @v_args(inline=True)
    def conditional(self, condition, _q, true_clause, _c, false_clause):
        return c99_ast.Conditional(
            condition=condition,
            true_clause=true_clause,
            false_clause=false_clause,
        )

    # Binary alternatives of `exp` — tokens discarded, build a Binary node.
    @v_args(inline=True)
    def multiply(self, left, _star, right):
        return c99_ast.Binary(op=c99_ast.Multiply(), left=left, right=right)

    @v_args(inline=True)
    def divide(self, left, _slash, right):
        return c99_ast.Binary(op=c99_ast.Divide(), left=left, right=right)

    @v_args(inline=True)
    def modulo(self, left, _percent, right):
        return c99_ast.Binary(op=c99_ast.Modulo(), left=left, right=right)

    @v_args(inline=True)
    def add(self, left, _plus, right):
        return c99_ast.Binary(op=c99_ast.Add(), left=left, right=right)

    @v_args(inline=True)
    def subtract(self, left, _minus, right):
        return c99_ast.Binary(op=c99_ast.Subtract(), left=left, right=right)

    @v_args(inline=True)
    def bitwise_and(self, left, _amp, right):
        return c99_ast.Binary(op=c99_ast.BitwiseAnd(), left=left, right=right)

    @v_args(inline=True)
    def bitwise_or(self, left, _pipe, right):
        return c99_ast.Binary(op=c99_ast.BitwiseOr(), left=left, right=right)

    @v_args(inline=True)
    def bitwise_xor(self, left, _caret, right):
        return c99_ast.Binary(op=c99_ast.BitwiseXor(), left=left, right=right)

    @v_args(inline=True)
    def left_shift(self, left, _lshift, right):
        return c99_ast.Binary(op=c99_ast.LeftShift(), left=left, right=right)

    @v_args(inline=True)
    def right_shift(self, left, _rshift, right):
        return c99_ast.Binary(op=c99_ast.RightShift(), left=left, right=right)

    @v_args(inline=True)
    def equal(self, left, _eq, right):
        return c99_ast.Binary(op=c99_ast.Equal(), left=left, right=right)

    @v_args(inline=True)
    def not_equal(self, left, _ne, right):
        return c99_ast.Binary(op=c99_ast.NotEqual(), left=left, right=right)

    @v_args(inline=True)
    def less_than(self, left, _lt, right):
        return c99_ast.Binary(op=c99_ast.LessThan(), left=left, right=right)

    @v_args(inline=True)
    def greater_than(self, left, _gt, right):
        return c99_ast.Binary(op=c99_ast.GreaterThan(), left=left, right=right)

    @v_args(inline=True)
    def less_or_equal(self, left, _le, right):
        return c99_ast.Binary(op=c99_ast.LessOrEqual(), left=left, right=right)

    @v_args(inline=True)
    def greater_or_equal(self, left, _ge, right):
        return c99_ast.Binary(op=c99_ast.GreaterOrEqual(), left=left, right=right)

    @v_args(inline=True)
    def logical_and(self, left, _andand, right):
        return c99_ast.Binary(op=c99_ast.LogicalAnd(), left=left, right=right)

    @v_args(inline=True)
    def logical_or(self, left, _oror, right):
        return c99_ast.Binary(op=c99_ast.LogicalOr(), left=left, right=right)

    # Alternatives of `unop` — tokens discarded, just produce the AST op.
    @v_args(inline=True)
    def negate(self, _minus):
        return c99_ast.Negate()

    @v_args(inline=True)
    def complement(self, _tilde):
        return c99_ast.Complement()

    @v_args(inline=True)
    def logical_not(self, _bang):
        return c99_ast.LogicalNot()


_BUILDER = _ASTBuilder()


def parse(source: str) -> c99_ast.Type_program:
    tree = _LARK.parse(source, start="start")
    return _BUILDER.transform(tree)
