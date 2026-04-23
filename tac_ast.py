# Generated from /project/c6502/tac.asdl. Do not edit.
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
class Ret(Type_instruction):
    val: Type_val


@dataclass
class Unary(Type_instruction):
    op: Type_unary_operator
    src: Type_val
    dst: Type_val


@dataclass
class Type_val:
    pass


@dataclass
class Constant(Type_val):
    value: int


@dataclass
class Var(Type_val):
    name: str


@dataclass
class Type_unary_operator:
    pass


@dataclass
class Complement(Type_unary_operator):
    pass


@dataclass
class Negate(Type_unary_operator):
    pass
