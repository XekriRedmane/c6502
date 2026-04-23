# Generated from /project/c6502/asm.asdl. Do not edit.
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
class Type_operand:
    pass


@dataclass
class Imm(Type_operand):
    value: int


@dataclass
class Register(Type_operand):
    pass
