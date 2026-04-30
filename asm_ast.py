# Generated from asm.asdl. Do not edit.
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
    init: list[Type_static_init] = field(default_factory=list)


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
class LongLongInit(Type_static_init):
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


@dataclass
class ZeroInit(Type_static_init):
    bytes: int


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
    save_a: bool


@dataclass
class FunctionPrologue(Type_instruction):
    arg_bytes: int
    local_bytes: int


@dataclass
class AllocateStack(Type_instruction):
    bytes: int


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
class And(Type_instruction):
    src: Type_operand
    dst: Type_operand


@dataclass
class Or(Type_instruction):
    src: Type_operand
    dst: Type_operand


@dataclass
class ArithmeticShiftLeft(Type_instruction):
    dst: Type_operand


@dataclass
class LogicalShiftRight(Type_instruction):
    dst: Type_operand


@dataclass
class RotateLeft(Type_instruction):
    dst: Type_operand


@dataclass
class RotateRight(Type_instruction):
    dst: Type_operand


@dataclass
class Label(Type_instruction):
    name: str


@dataclass
class Jump(Type_instruction):
    target: str


@dataclass
class Branch(Type_instruction):
    cond: Type_condition
    target: str


@dataclass
class Compare(Type_instruction):
    left: Type_operand
    right: Type_operand


@dataclass
class LoadAddress(Type_instruction):
    src: Type_operand
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
    offset: int


@dataclass
class Stack(Type_operand):
    offset: int


@dataclass
class Frame(Type_operand):
    offset: int


@dataclass
class Data(Type_operand):
    name: str
    offset: int


@dataclass
class Indirect(Type_operand):
    offset: int


@dataclass
class ImmLabelLow(Type_operand):
    name: str
    offset: int


@dataclass
class ImmLabelHigh(Type_operand):
    name: str
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


@dataclass
class Type_condition:
    pass


@dataclass
class CC(Type_condition):
    pass


@dataclass
class CS(Type_condition):
    pass


@dataclass
class EQ(Type_condition):
    pass


@dataclass
class MI(Type_condition):
    pass


@dataclass
class NE(Type_condition):
    pass


@dataclass
class PL(Type_condition):
    pass


@dataclass
class VC(Type_condition):
    pass


@dataclass
class VS(Type_condition):
    pass
