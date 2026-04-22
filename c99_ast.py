# Generated from /project/c6502/c99.asdl. Do not edit.
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
    body: Type_statement


@dataclass
class Type_statement:
    pass


@dataclass
class Return(Type_statement):
    exp: Type_exp


@dataclass
class Type_exp:
    pass


@dataclass
class Constant(Type_exp):
    value: int
