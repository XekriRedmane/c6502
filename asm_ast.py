# Generated from asm.asdl. Do not edit.
from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class Type_program:
    pass


@dataclass
class Program(Type_program):
    function_definition: Type_function_definition


@dataclass
class Type_function_definition:
    pass


@dataclass
class Function(Type_function_definition):
    name: str
    instructions: list[Type_instruction] = field(default_factory=list)


@dataclass
class Type_instruction:
    pass


@dataclass
class Mov(Type_instruction):
    src: Type_operand
    dst: Type_operand


@dataclass
class Ret(Type_instruction):
    pass


@dataclass
class AllocateStack(Type_instruction):
    amt: int


@dataclass
class Unary(Type_instruction):
    op: Type_unary_operator
    src_dst: Type_operand


@dataclass
class Type_operand:
    pass


@dataclass
class Imm(Type_operand):
    value: int


@dataclass
class Reg(Type_operand):
    reg: Type_reg


@dataclass
class Pseudo(Type_operand):
    name: str


@dataclass
class Stack(Type_operand):
    offset: int


@dataclass
class Type_unary_operator:
    pass


@dataclass
class Neg(Type_unary_operator):
    pass


@dataclass
class Not(Type_unary_operator):
    pass


@dataclass
class Type_reg:
    pass


@dataclass
class A(Type_reg):
    pass


@dataclass
class X(Type_reg):
    pass


@dataclass
class Y(Type_reg):
    pass
