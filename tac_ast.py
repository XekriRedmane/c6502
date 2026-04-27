# Generated from tac.asdl. Do not edit.
from __future__ import annotations

from dataclasses import dataclass, field


@dataclass
class Type_program:
    pass


@dataclass
class Program(Type_program):
    top_level: list[Type_top_level] = field(default_factory=list)


@dataclass
class Type_top_level:
    pass


@dataclass
class Function(Type_top_level):
    name: str
    is_global: bool
    params: list[str] = field(default_factory=list)
    instructions: list[Type_instruction] = field(default_factory=list)


@dataclass
class StaticVariable(Type_top_level):
    name: str
    is_global: bool
    data_type: Type_data_type
    init: Type_static_init


@dataclass
class Type_instruction:
    pass


@dataclass
class Ret(Type_instruction):
    val: Type_val


@dataclass
class SignExtend(Type_instruction):
    src: Type_val
    dst: Type_val


@dataclass
class ZeroExtend(Type_instruction):
    src: Type_val
    dst: Type_val


@dataclass
class Truncate(Type_instruction):
    src: Type_val
    dst: Type_val


@dataclass
class IntToFloat(Type_instruction):
    src: Type_val
    dst: Type_val


@dataclass
class IntToDouble(Type_instruction):
    src: Type_val
    dst: Type_val


@dataclass
class FloatToInt(Type_instruction):
    src: Type_val
    dst: Type_val


@dataclass
class DoubleToInt(Type_instruction):
    src: Type_val
    dst: Type_val


@dataclass
class FloatToDouble(Type_instruction):
    src: Type_val
    dst: Type_val


@dataclass
class DoubleToFloat(Type_instruction):
    src: Type_val
    dst: Type_val


@dataclass
class GetAddress(Type_instruction):
    operand: Type_val
    dst: Type_val


@dataclass
class Load(Type_instruction):
    src_ptr: Type_val
    dst: Type_val


@dataclass
class Store(Type_instruction):
    src: Type_val
    dst_ptr: Type_val


@dataclass
class Unary(Type_instruction):
    op: Type_unary_operator
    src: Type_val
    dst: Type_val


@dataclass
class Binary(Type_instruction):
    op: Type_binary_operator
    src1: Type_val
    src2: Type_val
    dst: Type_val


@dataclass
class Copy(Type_instruction):
    src: Type_val
    dst: Type_val


@dataclass
class Jump(Type_instruction):
    target: str


@dataclass
class JumpIfTrue(Type_instruction):
    condition: Type_val
    target: str


@dataclass
class JumpIfFalse(Type_instruction):
    condition: Type_val
    target: str


@dataclass
class Label(Type_instruction):
    name: str


@dataclass(kw_only=True)
class FunctionCall(Type_instruction):
    name: str
    args: list[Type_val] = field(default_factory=list)
    dst: Type_val


@dataclass
class Type_val:
    pass


@dataclass
class Constant(Type_val):
    const: Type_const


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
class Type_data_type:
    pass


@dataclass
class Int(Type_data_type):
    pass


@dataclass
class Long(Type_data_type):
    pass


@dataclass
class UInt(Type_data_type):
    pass


@dataclass
class ULong(Type_data_type):
    pass


@dataclass
class Float(Type_data_type):
    pass


@dataclass
class Double(Type_data_type):
    pass


@dataclass(kw_only=True)
class FunType(Type_data_type):
    params: list[Type_data_type] = field(default_factory=list)
    ret: Type_data_type


@dataclass
class Type_const:
    pass


@dataclass
class ConstInt(Type_const):
    int: int


@dataclass
class ConstLong(Type_const):
    int: int


@dataclass
class ConstFloat(Type_const):
    float: float


@dataclass
class ConstDouble(Type_const):
    float: float


@dataclass
class Type_static_init:
    pass


@dataclass
class IntInit(Type_static_init):
    int: int


@dataclass
class LongInit(Type_static_init):
    int: int


@dataclass
class UIntInit(Type_static_init):
    int: int


@dataclass
class ULongInit(Type_static_init):
    int: int


@dataclass
class FloatInit(Type_static_init):
    float: float


@dataclass
class DoubleInit(Type_static_init):
    float: float


@dataclass
class AddressInit(Type_static_init):
    name: str
    offset: int
