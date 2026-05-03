# Generated from c99.asdl. Do not edit.
from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class Type_program:
    pass


@dataclass
class Program(Type_program):
    declaration: list[Type_declaration] = field(default_factory=list)


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
    exp: Type_exp | None = None


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
class SwitchStmt(Type_statement):
    control: Type_exp
    body: Type_statement
    label: str
    cases: list[Type_switch_case] = field(default_factory=list)
    default_label: str | None = None
    promoted_type: Type_data_type | None = None


@dataclass
class CaseStmt(Type_statement):
    value: Type_exp
    body: Type_statement
    label: str


@dataclass
class DefaultStmt(Type_statement):
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
class Type_switch_case:
    value: Type_exp
    label: str


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
class StructDecl(Type_declaration):
    struct_decl: Type_struct_decl


@dataclass(kw_only=True)
class Type_var_decl:
    name: str
    init: Type_exp | None = None
    data_type: Type_data_type
    storage_class: Type_storage_class | None = None
    abi_annotation: str | None = None


@dataclass(kw_only=True)
class Type_function_decl:
    name: str
    params: list[str] = field(default_factory=list)
    body: Type_block | None = None
    data_type: Type_data_type
    storage_class: Type_storage_class | None = None
    abi_annotation: str | None = None


@dataclass
class Type_struct_decl:
    tag: str
    is_union: bool
    members: list[Type_member_decl] = field(default_factory=list)


@dataclass
class Type_member_decl:
    name: str
    data_type: Type_data_type


@dataclass
class Type_storage_class:
    pass


@dataclass
class Static(Type_storage_class):
    pass


@dataclass
class Extern(Type_storage_class):
    pass


@dataclass
class Type_data_type:
    pass


@dataclass
class Int(Type_data_type):
    pass


@dataclass
class Long(Type_data_type):
    pass


@dataclass
class LongLong(Type_data_type):
    pass


@dataclass
class UInt(Type_data_type):
    pass


@dataclass
class ULong(Type_data_type):
    pass


@dataclass
class ULongLong(Type_data_type):
    pass


@dataclass
class Float(Type_data_type):
    pass


@dataclass
class Double(Type_data_type):
    pass


@dataclass
class Char(Type_data_type):
    pass


@dataclass
class SChar(Type_data_type):
    pass


@dataclass
class UChar(Type_data_type):
    pass


@dataclass
class Void(Type_data_type):
    pass


@dataclass(kw_only=True)
class FunType(Type_data_type):
    params: list[Type_data_type] = field(default_factory=list)
    ret: Type_data_type


@dataclass
class Pointer(Type_data_type):
    referenced_type: Type_data_type


@dataclass
class Array(Type_data_type):
    element_type: Type_data_type
    size: int


@dataclass
class Structure(Type_data_type):
    tag: str


@dataclass
class Union(Type_data_type):
    tag: str


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
    const: Type_const
    data_type: Type_data_type | None = None


@dataclass
class String(Type_exp):
    str: str


@dataclass
class Var(Type_exp):
    name: str
    data_type: Type_data_type | None = None


@dataclass
class Cast(Type_exp):
    target_type: Type_data_type
    exp: Type_exp
    data_type: Type_data_type | None = None


@dataclass
class Unary(Type_exp):
    op: Type_unary_operator
    exp: Type_exp
    data_type: Type_data_type | None = None


@dataclass
class Binary(Type_exp):
    op: Type_binary_operator
    left: Type_exp
    right: Type_exp
    data_type: Type_data_type | None = None


@dataclass
class Assignment(Type_exp):
    lval: Type_exp
    rval: Type_exp
    data_type: Type_data_type | None = None


@dataclass
class CompoundAssignment(Type_exp):
    op: Type_binary_operator
    lval: Type_exp
    rval: Type_exp
    intermediate_type: Type_data_type | None = None
    data_type: Type_data_type | None = None


@dataclass
class Postfix(Type_exp):
    op: Type_incdec_op
    operand: Type_exp
    data_type: Type_data_type | None = None


@dataclass
class Prefix(Type_exp):
    op: Type_incdec_op
    operand: Type_exp
    data_type: Type_data_type | None = None


@dataclass
class Conditional(Type_exp):
    condition: Type_exp
    true_clause: Type_exp
    false_clause: Type_exp
    data_type: Type_data_type | None = None


@dataclass
class FunctionCall(Type_exp):
    name: str
    args: list[Type_exp] = field(default_factory=list)
    data_type: Type_data_type | None = None


@dataclass
class Dereference(Type_exp):
    exp: Type_exp
    data_type: Type_data_type | None = None


@dataclass
class AddressOf(Type_exp):
    exp: Type_exp
    data_type: Type_data_type | None = None


@dataclass
class Subscript(Type_exp):
    array: Type_exp
    index: Type_exp
    data_type: Type_data_type | None = None


@dataclass
class InitList(Type_exp):
    items: list[Type_exp] = field(default_factory=list)
    data_type: Type_data_type | None = None


@dataclass
class SizeOfExp(Type_exp):
    exp: Type_exp
    data_type: Type_data_type | None = None


@dataclass
class SizeOfType(Type_exp):
    target_type: Type_data_type
    data_type: Type_data_type | None = None


@dataclass
class Dot(Type_exp):
    operand: Type_exp
    member: str
    data_type: Type_data_type | None = None


@dataclass
class Arrow(Type_exp):
    operand: Type_exp
    member: str
    data_type: Type_data_type | None = None


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


@dataclass
class Type_const:
    pass


@dataclass
class ConstInt(Type_const):
    value: int


@dataclass
class ConstLong(Type_const):
    value: int


@dataclass
class ConstLongLong(Type_const):
    value: int


@dataclass
class ConstUInt(Type_const):
    value: int


@dataclass
class ConstULong(Type_const):
    value: int


@dataclass
class ConstULongLong(Type_const):
    value: int


@dataclass
class ConstFloat(Type_const):
    bits: int


@dataclass
class ConstDouble(Type_const):
    bits: int


@dataclass
class ConstChar(Type_const):
    value: int


@dataclass
class ConstUChar(Type_const):
    value: int
