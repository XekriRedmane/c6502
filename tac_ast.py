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
    name: str = ''
    is_global: bool = False
    params: list[str] = field(default_factory=list)
    instructions: list[Type_instruction] = field(default_factory=list)


@dataclass(kw_only=True)
class StaticVariable(Type_top_level):
    name: str = ''
    is_global: bool = False
    data_type: Type_data_type
    init: list[Type_static_init] = field(default_factory=list)


@dataclass
class Type_instruction:
    pass


@dataclass
class Ret(Type_instruction):
    val: Type_val | None = None


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
    is_volatile: bool = False


@dataclass
class Store(Type_instruction):
    src: Type_val
    dst_ptr: Type_val
    is_volatile: bool = False


@dataclass(kw_only=True)
class IndexedLoad(Type_instruction):
    name: str = ''
    index: Type_val
    dst: Type_val
    is_volatile: bool = False


@dataclass(kw_only=True)
class IndexedStore(Type_instruction):
    address: int = 0
    index: Type_val
    src: Type_val
    is_volatile: bool = False


@dataclass(kw_only=True)
class IndexedConstLoad(Type_instruction):
    address: int = 0
    index: Type_val
    dst: Type_val
    is_volatile: bool = False


@dataclass
class IndirectIndexedLoad(Type_instruction):
    ptr: Type_val
    index: Type_val
    dst: Type_val
    is_volatile: bool = False


@dataclass
class IndirectIndexedStore(Type_instruction):
    ptr: Type_val
    index: Type_val
    src: Type_val
    is_volatile: bool = False


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
    target: str = ''


@dataclass
class JumpIfTrue(Type_instruction):
    condition: Type_val
    target: str = ''


@dataclass
class JumpIfFalse(Type_instruction):
    condition: Type_val
    target: str = ''


@dataclass
class JumpIfCmp(Type_instruction):
    op: Type_binary_operator
    src1: Type_val
    src2: Type_val
    target: str = ''


@dataclass
class JumpIfMasked(Type_instruction):
    val: Type_val
    mask: int = 0
    jump_when_nonzero: bool = False
    target: str = ''


@dataclass
class Label(Type_instruction):
    name: str = ''


@dataclass
class FunctionCall(Type_instruction):
    name: str = ''
    args: list[Type_val] = field(default_factory=list)
    dst: Type_val | None = None


@dataclass
class IndirectCall(Type_instruction):
    ptr: Type_val
    args: list[Type_val] = field(default_factory=list)
    dst: Type_val | None = None


@dataclass
class Phi(Type_instruction):
    dst: Type_val
    args: list[Type_phi_arg] = field(default_factory=list)


@dataclass
class Type_phi_arg:
    pass


@dataclass(kw_only=True)
class PhiArg(Type_phi_arg):
    pred_label: str = ''
    source: Type_val


@dataclass
class Type_val:
    pass


@dataclass
class Constant(Type_val):
    const: Type_const


@dataclass
class Var(Type_val):
    name: str = ''


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
class Void(Type_data_type):
    pass


@dataclass
class Pointer(Type_data_type):
    pass


@dataclass(kw_only=True)
class FunType(Type_data_type):
    params: list[Type_data_type] = field(default_factory=list)
    ret: Type_data_type


@dataclass
class Type_const:
    pass


@dataclass
class ConstChar(Type_const):
    value: int = 0


@dataclass
class ConstUChar(Type_const):
    value: int = 0


@dataclass
class ConstInt(Type_const):
    value: int = 0


@dataclass
class ConstLong(Type_const):
    value: int = 0


@dataclass
class ConstLongLong(Type_const):
    value: int = 0


@dataclass
class ConstUInt(Type_const):
    value: int = 0


@dataclass
class ConstULong(Type_const):
    value: int = 0


@dataclass
class ConstULongLong(Type_const):
    value: int = 0


@dataclass
class ConstFloat(Type_const):
    bits: int = 0


@dataclass
class ConstDouble(Type_const):
    bits: int = 0


@dataclass
class Type_static_init:
    pass


@dataclass
class CharInit(Type_static_init):
    value: int = 0


@dataclass
class UCharInit(Type_static_init):
    value: int = 0


@dataclass
class IntInit(Type_static_init):
    value: int = 0


@dataclass
class LongInit(Type_static_init):
    value: int = 0


@dataclass
class LongLongInit(Type_static_init):
    value: int = 0


@dataclass
class UIntInit(Type_static_init):
    value: int = 0


@dataclass
class ULongInit(Type_static_init):
    value: int = 0


@dataclass
class ULongLongInit(Type_static_init):
    value: int = 0


@dataclass
class FloatInit(Type_static_init):
    bits: int = 0


@dataclass
class DoubleInit(Type_static_init):
    bits: int = 0


@dataclass
class AddressInit(Type_static_init):
    name: str = ''
    offset: int = 0


@dataclass
class StringInit(Type_static_init):
    str: str = ''
    bytes: int = 0


@dataclass
class ZeroInit(Type_static_init):
    bytes: int = 0
