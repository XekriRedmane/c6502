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


class _ASTBuilder(Transformer):
    @v_args(inline=True)
    def start(self, function):
        return c99_ast.Program(function_definition=function)

    @v_args(inline=True)
    def function(self, _int, name, _lparen, _void, _rparen, _lbrace, body, _rbrace):
        return c99_ast.Function(name=str(name), body=body)

    @v_args(inline=True)
    def statement(self, _return, exp, _semi):
        return c99_ast.Return(exp=exp)

    # Alternatives of `exp` — each named in c99.lark.
    @v_args(inline=True)
    def constant(self, token):
        return c99_ast.Constant(value=int(str(token)))

    @v_args(inline=True)
    def unary(self, op, inner):
        return c99_ast.Unary(unary_operator=op, exp=inner)

    @v_args(inline=True)
    def paren(self, _lp, inner, _rp):
        return inner

    # Alternatives of `unop` — tokens discarded, just produce the AST op.
    @v_args(inline=True)
    def negate(self, _minus):
        return c99_ast.Negate()

    @v_args(inline=True)
    def complement(self, _tilde):
        return c99_ast.Complement()


_BUILDER = _ASTBuilder()


def parse(source: str) -> c99_ast.Type_program:
    tree = _LARK.parse(source, start="start")
    return _BUILDER.transform(tree)
