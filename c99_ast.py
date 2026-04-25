# Generated from c99.asdl. Do not edit.
from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class Type_program:
    pass


@dataclass
class Program(Type_program):
    function_definition: list[Type_function_definition] = field(default_factory=list)


@dataclass
class Type_function_definition:
    pass


@dataclass(kw_only=True)
class Function(Type_function_definition):
    name: str
    params: list[str] = field(default_factory=list)
    body: Type_block


@dataclass
class Type_block:
    pass


@dataclass
class Block(Type_block):
    block_item: list[Type_block_item] = field(default_factory=list)


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
class IfStmt(Type_statement):
    condition: Type_exp
    then_clause: Type_statement
    else_clause: Type_statement | None = None


@dataclass
class Compound(Type_statement):
    block: Type_block


@dataclass
class BreakStmt(Type_statement):
    label: str


@dataclass
class ContinueStmt(Type_statement):
    label: str


@dataclass
class WhileStmt(Type_statement):
    condition: Type_exp
    body: Type_statement
    label: str


@dataclass
class DoWhileStmt(Type_statement):
    body: Type_statement
    condition: Type_exp
    label: str


@dataclass(kw_only=True)
class ForStmt(Type_statement):
    init: Type_for_init
    condition: Type_exp | None = None
    post_clause: Type_exp | None = None
    body: Type_statement
    label: str


@dataclass
class Goto(Type_statement):
    label: str


@dataclass
class LabeledStmt(Type_statement):
    label: str
    statement: Type_statement


@dataclass
class Null(Type_statement):
    pass


@dataclass
class Type_declaration:
    pass


@dataclass
class FunctionDecl(Type_declaration):
    function_decl: Type_function_decl


@dataclass
class VarDecl(Type_declaration):
    var_decl: Type_var_decl


@dataclass
class Type_var_decl:
    name: str
    init: Type_exp | None = None


@dataclass
class Type_function_decl:
    name: str
    params: list[str] = field(default_factory=list)
    body: Type_block | None = None


@dataclass
class Type_for_init:
    pass


@dataclass
class InitDecl(Type_for_init):
    var_decl: Type_var_decl


@dataclass
class InitExp(Type_for_init):
    exp: Type_exp | None = None


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
class Postfix(Type_exp):
    op: Type_incdec_op
    operand: Type_exp


@dataclass
class Conditional(Type_exp):
    condition: Type_exp
    true_clause: Type_exp
    false_clause: Type_exp


@dataclass
class FunctionCall(Type_exp):
    name: str
    args: list[Type_exp] = field(default_factory=list)


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
class Type_incdec_op:
    pass


@dataclass
class Increment(Type_incdec_op):
    pass


@dataclass
class Decrement(Type_incdec_op):
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
