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
  C99 Program(fn)             -> TAC Program(translate_function(fn))
  C99 Function(name, body)    -> TAC Function(name, <instrs built from body>)
  C99 Return(exp)             -> emit Ret(translate_exp(exp))
  C99 Constant(v)             -> TAC Constant(v)
  C99 Unary(op, inner)        -> emit Unary(op', translate(inner), Var(t))
                                 and return Var(t), where t is a fresh temp
  C99 Binary(op, left, right) -> emit Binary(op', translate(left),
                                 translate(right), Var(t))
                                 and return Var(t); left is translated
                                 before right so any temps it needs are
                                 numbered first.
  C99 Negate / Complement /   -> TAC Negate / Complement / LogicalNot
    LogicalNot
  C99 Add / Subtract /        -> TAC Add / Subtract / Multiply / Divide
    Multiply / Divide /          / Modulo / BitwiseAnd / BitwiseOr /
    Modulo / BitwiseAnd /        BitwiseXor / LeftShift / RightShift /
    BitwiseOr / BitwiseXor /     Equal / NotEqual / LessThan /
    LeftShift / RightShift /     GreaterThan / LessOrEqual /
    Equal / NotEqual /           GreaterOrEqual
    LessThan / GreaterThan /
    LessOrEqual / GreaterOrEqual
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
            case c99_ast.Unary(op=op, exp=inner):
                src = self.translate_exp(inner, instrs)
                dst = tac_ast.Var(name=self.make_temporary_variable_name())
                instrs.append(tac_ast.Unary(
                    op=self.translate_unop(op),
                    src=src,
                    dst=dst,
                ))
                return dst
            case c99_ast.Binary(op=op, left=left, right=right):
                # Translate left first so its temps get the lower
                # numbers — matches a left-to-right evaluation order
                # readers will expect.
                src1 = self.translate_exp(left, instrs)
                src2 = self.translate_exp(right, instrs)
                dst = tac_ast.Var(name=self.make_temporary_variable_name())
                instrs.append(tac_ast.Binary(
                    op=self.translate_binop(op),
                    src1=src1,
                    src2=src2,
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
            case c99_ast.LogicalNot():
                return tac_ast.LogicalNot()
        raise TypeError(f"unexpected unop: {op!r}")

    def translate_binop(
        self, op: c99_ast.Type_binary_operator,
    ) -> tac_ast.Type_binary_operator:
        match op:
            case c99_ast.Add():
                return tac_ast.Add()
            case c99_ast.Subtract():
                return tac_ast.Subtract()
            case c99_ast.Multiply():
                return tac_ast.Multiply()
            case c99_ast.Divide():
                return tac_ast.Divide()
            case c99_ast.Modulo():
                return tac_ast.Modulo()
            case c99_ast.BitwiseAnd():
                return tac_ast.BitwiseAnd()
            case c99_ast.BitwiseOr():
                return tac_ast.BitwiseOr()
            case c99_ast.BitwiseXor():
                return tac_ast.BitwiseXor()
            case c99_ast.LeftShift():
                return tac_ast.LeftShift()
            case c99_ast.RightShift():
                return tac_ast.RightShift()
            case c99_ast.Equal():
                return tac_ast.Equal()
            case c99_ast.NotEqual():
                return tac_ast.NotEqual()
            case c99_ast.LessThan():
                return tac_ast.LessThan()
            case c99_ast.GreaterThan():
                return tac_ast.GreaterThan()
            case c99_ast.LessOrEqual():
                return tac_ast.LessOrEqual()
            case c99_ast.GreaterOrEqual():
                return tac_ast.GreaterOrEqual()
        raise TypeError(f"unexpected binop: {op!r}")


def translate_program(prog: c99_ast.Type_program) -> tac_ast.Type_program:
    """Convenience wrapper: builds a fresh Translator per call (so the
    temporary counter starts at 0 every time)."""
    return Translator().translate_program(prog)
