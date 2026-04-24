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
    arg_bytes: int
    local_bytes: int


@dataclass
class FunctionPrologue(Type_instruction):
    arg_bytes: int
    local_bytes: int


@dataclass
class Add(Type_instruction):
    src: Type_operand
    dst: Type_operand


@dataclass
class Sub(Type_instruction):
    src: Type_operand
    dst: Type_operand


@dataclass
class Call(Type_instruction):
    name: str


@dataclass
class ClearCarry(Type_instruction):
    pass


@dataclass
class SetCarry(Type_instruction):
    pass


@dataclass
class Inc(Type_instruction):
    dst: Type_operand


@dataclass
class Dec(Type_instruction):
    dst: Type_operand


@dataclass
class Push(Type_instruction):
    src: Type_operand


@dataclass
class Pop(Type_instruction):
    dst: Type_operand


@dataclass
class Xor(Type_instruction):
    src1: Type_operand
    src2: Type_operand
    dst: Type_operand


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
class Frame(Type_operand):
    offset: int


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
