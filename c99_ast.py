# Generated from c99.asdl. Do not edit.
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
    body: list[Type_block_item] = field(default_factory=list)


@dataclass
class Type_block_item:
    pass


@dataclass
class S(Type_block_item):
    statement: Type_statement


@dataclass
class D(Type_block_item):
    declaration: Type_declaration


@dataclass
class Type_statement:
    pass


@dataclass
class Return(Type_statement):
    exp: Type_exp


@dataclass
class Expression(Type_statement):
    exp: Type_exp


@dataclass
class Null(Type_statement):
    pass


@dataclass
class Type_declaration:
    pass


@dataclass
class Declaration(Type_declaration):
    name: str
    init: Type_exp | None = None


@dataclass
class Type_exp:
    pass


@dataclass
class Constant(Type_exp):
    value: int


@dataclass
class Var(Type_exp):
    name: str


@dataclass
class Unary(Type_exp):
    op: Type_unary_operator
    exp: Type_exp


@dataclass
class Binary(Type_exp):
    op: Type_binary_operator
    left: Type_exp
    right: Type_exp


@dataclass
class Assignment(Type_exp):
    lval: Type_exp
    rval: Type_exp


@dataclass
class Type_unary_operator:
    pass


@dataclass
class Complement(Type_unary_operator):
    pass


@dataclass
class Negate(Type_unary_operator):
    pass


@dataclass
class LogicalNot(Type_unary_operator):
    pass


@dataclass
class Type_binary_operator:
    pass


@dataclass
class Add(Type_binary_operator):
    pass


@dataclass
class Subtract(Type_binary_operator):
    pass


@dataclass
class Multiply(Type_binary_operator):
    pass


@dataclass
class Divide(Type_binary_operator):
    pass


@dataclass
class Modulo(Type_binary_operator):
    pass


@dataclass
class BitwiseAnd(Type_binary_operator):
    pass


@dataclass
class BitwiseOr(Type_binary_operator):
    pass


@dataclass
class BitwiseXor(Type_binary_operator):
    pass


@dataclass
class LeftShift(Type_binary_operator):
    pass


@dataclass
class RightShift(Type_binary_operator):
    pass


@dataclass
class Equal(Type_binary_operator):
    pass


@dataclass
class NotEqual(Type_binary_operator):
    pass


@dataclass
class LessThan(Type_binary_operator):
    pass


@dataclass
class GreaterThan(Type_binary_operator):
    pass


@dataclass
class LessOrEqual(Type_binary_operator):
    pass


@dataclass
class GreaterOrEqual(Type_binary_operator):
    pass


@dataclass
class LogicalAnd(Type_binary_operator):
    pass


@dataclass
class LogicalOr(Type_binary_operator):
    pass
