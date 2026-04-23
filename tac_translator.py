"""Translate a c99_ast tree into a tac_ast tree (three-address code).

Every C99 expression becomes a tac_ast `val` (either a Constant or a Var
holding the result of an earlier instruction). Compound expressions get
flattened: nested operators materialize their intermediate results into
fresh Var-typed temporaries and emit the corresponding TAC instruction.

State:
  - Translator owns the temporary-name counter (`%0`, `%1`, ...).
  - The per-function instruction list is passed down explicitly as an
    argument so there's no implicit "current function" on the instance.

Mapping:
  C99 Program(fn)          -> TAC Program(translate_function(fn))
  C99 Function(name, body) -> TAC Function(name, <instrs built from body>)
  C99 Return(exp)          -> emit Ret(translate_exp(exp))
  C99 Constant(v)          -> TAC Constant(v)
  C99 Unary(op, inner)     -> emit Unary(op', translate(inner), Var(t))
                              and return Var(t), where t is a fresh temp
  C99 Negate / Complement  -> TAC Negate / Complement
"""

from __future__ import annotations

import c99_ast
import tac_ast


class Translator:
    def __init__(self) -> None:
        self._temp_counter = 0

    def make_temporary_variable_name(self) -> str:
        name = f"%{self._temp_counter}"
        self._temp_counter += 1
        return name

    def translate_program(self, prog: c99_ast.Type_program) -> tac_ast.Type_program:
        match prog:
            case c99_ast.Program(function_definition=fn):
                return tac_ast.Program(
                    function_definition=self.translate_function(fn),
                )
        raise TypeError(f"unexpected program: {prog!r}")

    def translate_function(
        self, fn: c99_ast.Type_function_definition,
    ) -> tac_ast.Type_function_definition:
        match fn:
            case c99_ast.Function(name=name, body=body):
                instrs: list[tac_ast.Type_instruction] = []
                self.translate_statement(body, instrs)
                return tac_ast.Function(name=name, instructions=instrs)
        raise TypeError(f"unexpected function: {fn!r}")

    def translate_statement(
        self,
        stmt: c99_ast.Type_statement,
        instrs: list[tac_ast.Type_instruction],
    ) -> None:
        match stmt:
            case c99_ast.Return(exp=exp):
                instrs.append(tac_ast.Ret(val=self.translate_exp(exp, instrs)))
                return
        raise TypeError(f"unexpected statement: {stmt!r}")

    def translate_exp(
        self,
        exp: c99_ast.Type_exp,
        instrs: list[tac_ast.Type_instruction],
    ) -> tac_ast.Type_val:
        match exp:
            case c99_ast.Constant(value=v):
                return tac_ast.Constant(value=v)
            case c99_ast.Unary(unary_operator=op, exp=inner):
                src = self.translate_exp(inner, instrs)
                dst = tac_ast.Var(name=self.make_temporary_variable_name())
                instrs.append(tac_ast.Unary(
                    op=self.translate_unop(op),
                    src=src,
                    dst=dst,
                ))
                return dst
        raise TypeError(f"unexpected exp: {exp!r}")

    def translate_unop(
        self, op: c99_ast.Type_unary_operator,
    ) -> tac_ast.Type_unary_operator:
        match op:
            case c99_ast.Complement():
                return tac_ast.Complement()
            case c99_ast.Negate():
                return tac_ast.Negate()
        raise TypeError(f"unexpected unop: {op!r}")


def translate_program(prog: c99_ast.Type_program) -> tac_ast.Type_program:
    """Convenience wrapper: builds a fresh Translator per call (so the
    temporary counter starts at 0 every time)."""
    return Translator().translate_program(prog)
