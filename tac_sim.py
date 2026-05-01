"""TAC simulator: runs TAC programs in Python.

Each TAC instruction maps to a Python operation that mirrors the
6502 lowering's bit-level semantics — operand values live in the
environment as unsigned N-byte ints (the byte-pattern view, same
as what the soft stack would hold), and signedness is re-applied
only at instructions where it matters (Divide, Modulo, ordering
comparisons, RightShift, SignExtend).

Minimal scope (this module): integer types, arithmetic, control
flow, Copy, FunctionCall, Ret, Cast nodes (SignExtend / ZeroExtend
/ Truncate). NOT YET: Load/Store/GetAddress, Float/Double, IntToFP
conversions, StaticVariable, IndirectCall.

Intended use: exercise C semantics without depending on the (still-
landing) 6502 runtime helpers, and pin TAC behavior as a fixed
point against future TAC optimization passes — same input
program should yield the same simulator trace before and after
any TAC-level transform.
"""
from __future__ import annotations

from dataclasses import dataclass

import tac_ast
from passes.type_checking import (
    Int,
    Long,
    LongLong,
    SymbolTable,
    Type,
    UInt,
    ULong,
    ULongLong,
)


class TacSimError(Exception):
    pass


_INT_TYPES: dict[type, tuple[bool, int]] = {
    Int: (True, 1),
    Long: (True, 2),
    LongLong: (True, 4),
    UInt: (False, 1),
    ULong: (False, 2),
    ULongLong: (False, 4),
}


def _type_info(t: Type) -> tuple[bool, int]:
    info = _INT_TYPES.get(type(t))
    if info is None:
        raise TacSimError(f"unsupported type in simulator: {type(t).__name__}")
    return info


def _const_info(c: tac_ast.Type_const) -> tuple[bool, int, int]:
    """(signed, width_bytes, unsigned_value) for a TAC integer constant."""
    match c:
        case tac_ast.ConstInt(value=v):       return (True,  1, v & 0xFF)
        case tac_ast.ConstLong(value=v):      return (True,  2, v & 0xFFFF)
        case tac_ast.ConstLongLong(value=v):  return (True,  4, v & 0xFFFFFFFF)
        case tac_ast.ConstUInt(value=v):      return (False, 1, v & 0xFF)
        case tac_ast.ConstULong(value=v):     return (False, 2, v & 0xFFFF)
        case tac_ast.ConstULongLong(value=v): return (False, 4, v & 0xFFFFFFFF)
    raise TacSimError(f"unsupported const variant: {type(c).__name__}")


def _mask(width: int) -> int:
    return (1 << (8 * width)) - 1


def _to_signed(u: int, width: int) -> int:
    half = 1 << (8 * width - 1)
    return u - 2 * half if u >= half else u


@dataclass
class _Frame:
    function: tac_ast.Function
    env: dict[str, int]
    pc: int = 0


@dataclass
class _Return:
    value: int | None
    width: int = 0


class Simulator:
    """Run TAC programs.

    >>> sim = Simulator(tac_program, symbols)
    >>> sim.call("main", [])
    """

    def __init__(self, program: tac_ast.Program, symbols: SymbolTable) -> None:
        self.program = program
        self.symbols = symbols
        self._functions: dict[str, tac_ast.Function] = {
            t.name: t
            for t in program.top_level
            if isinstance(t, tac_ast.Function)
        }
        self._labels: dict[str, dict[str, int]] = {
            fn.name: {
                ins.name: i
                for i, ins in enumerate(fn.instructions)
                if isinstance(ins, tac_ast.Label)
            }
            for fn in self._functions.values()
        }

    def call(self, name: str, args: list[int]) -> int | None:
        fn = self._functions.get(name)
        if fn is None:
            raise TacSimError(f"unknown function: {name!r}")
        if len(args) != len(fn.params):
            raise TacSimError(
                f"{name}: expected {len(fn.params)} args, got {len(args)}"
            )
        env: dict[str, int] = {}
        for pname, raw in zip(fn.params, args):
            _, w = _type_info(self.symbols[pname].type)
            env[pname] = raw & _mask(w)
        ret = self._run(_Frame(fn, env))
        if ret.value is None:
            return None
        ret_type = self.symbols[name].type.ret
        signed, _ = _type_info(ret_type)
        return _to_signed(ret.value, ret.width) if signed else ret.value

    def _run(self, frame: _Frame) -> _Return:
        ins_list = frame.function.instructions
        while frame.pc < len(ins_list):
            ins = ins_list[frame.pc]
            frame.pc += 1
            result = self._step(frame, ins)
            if isinstance(result, _Return):
                return result
        raise TacSimError(
            f"{frame.function.name}: ran past end without Ret"
        )

    def _step(self, frame: _Frame, ins) -> _Return | None:
        match ins:
            case tac_ast.Label():
                return None

            case tac_ast.Jump(target=t):
                frame.pc = self._labels[frame.function.name][t]

            case tac_ast.JumpIfTrue(condition=c, target=t):
                if self._read(frame, c)[0] != 0:
                    frame.pc = self._labels[frame.function.name][t]

            case tac_ast.JumpIfFalse(condition=c, target=t):
                if self._read(frame, c)[0] == 0:
                    frame.pc = self._labels[frame.function.name][t]

            case tac_ast.Copy(src=s, dst=d):
                v, _ = self._read(frame, s)
                self._write(frame, d, v)

            case tac_ast.Unary(op=op, src=s, dst=d):
                v, (_, w) = self._read(frame, s)
                self._write(frame, d, self._eval_unary(op, v, w))

            case tac_ast.Binary(op=op, src1=a, src2=b, dst=d):
                va, ta = self._read(frame, a)
                vb, _ = self._read(frame, b)
                self._write(frame, d, self._eval_binary(op, va, vb, ta))

            case tac_ast.SignExtend(src=s, dst=d):
                vu, (_, w_src) = self._read(frame, s)
                vs = _to_signed(vu, w_src)
                self._write(frame, d, vs & _mask(self._dst_width(d)))

            case tac_ast.ZeroExtend(src=s, dst=d) | tac_ast.Truncate(src=s, dst=d):
                vu, _ = self._read(frame, s)
                self._write(frame, d, vu & _mask(self._dst_width(d)))

            case tac_ast.Ret(val=None):
                return _Return(None, 0)

            case tac_ast.Ret(val=v):
                vu, (_, w) = self._read(frame, v)
                return _Return(vu, w)

            case tac_ast.FunctionCall(name=name, args=args, dst=dst):
                py_args = [self._read(frame, a)[0] for a in args]
                rv = self.call(name, py_args)
                if dst is not None and rv is not None:
                    self._write(frame, dst, rv & _mask(self._dst_width(dst)))

            case _:
                raise TacSimError(
                    f"unsupported instruction: {type(ins).__name__}"
                )

        return None

    def _read(self, frame: _Frame, val) -> tuple[int, tuple[bool, int]]:
        match val:
            case tac_ast.Constant(const=c):
                signed, w, vu = _const_info(c)
                return vu, (signed, w)
            case tac_ast.Var(name=n):
                signed, w = _type_info(self.symbols[n].type)
                if n not in frame.env:
                    raise TacSimError(f"uninitialized var: {n}")
                return frame.env[n], (signed, w)
        raise TacSimError(f"unsupported val: {type(val).__name__}")

    def _write(self, frame: _Frame, val, raw: int) -> None:
        if not isinstance(val, tac_ast.Var):
            raise TacSimError(f"can only write to Var, got {type(val).__name__}")
        _, w = _type_info(self.symbols[val.name].type)
        frame.env[val.name] = raw & _mask(w)

    def _dst_width(self, val) -> int:
        if not isinstance(val, tac_ast.Var):
            raise TacSimError(f"expected Var as dst, got {type(val).__name__}")
        _, w = _type_info(self.symbols[val.name].type)
        return w

    @staticmethod
    def _eval_unary(op, v_u: int, w: int) -> int:
        m = _mask(w)
        match op:
            case tac_ast.Negate():     return (-_to_signed(v_u, w)) & m
            case tac_ast.Complement(): return (~v_u) & m
            case tac_ast.LogicalNot(): return 1 if v_u == 0 else 0
        raise TacSimError(f"unsupported unary op: {type(op).__name__}")

    @staticmethod
    def _eval_binary(op, a_u: int, b_u: int, ta: tuple[bool, int]) -> int:
        signed, w = ta
        m = _mask(w)

        if isinstance(op, tac_ast.Equal):
            return 1 if a_u == b_u else 0
        if isinstance(op, tac_ast.NotEqual):
            return 1 if a_u != b_u else 0
        if isinstance(op, (tac_ast.LessThan, tac_ast.GreaterThan,
                           tac_ast.LessOrEqual, tac_ast.GreaterOrEqual)):
            a = _to_signed(a_u, w) if signed else a_u
            b = _to_signed(b_u, w) if signed else b_u
            cmp = {
                tac_ast.LessThan:       a <  b,
                tac_ast.GreaterThan:    a >  b,
                tac_ast.LessOrEqual:    a <= b,
                tac_ast.GreaterOrEqual: a >= b,
            }[type(op)]
            return 1 if cmp else 0

        match op:
            case tac_ast.Add():        return (a_u + b_u) & m
            case tac_ast.Subtract():   return (a_u - b_u) & m
            case tac_ast.Multiply():   return (a_u * b_u) & m
            case tac_ast.BitwiseAnd(): return a_u & b_u
            case tac_ast.BitwiseOr():  return a_u | b_u
            case tac_ast.BitwiseXor(): return a_u ^ b_u
            case tac_ast.LeftShift():
                return (a_u << (b_u & 0xFF)) & m
            case tac_ast.RightShift():
                count = b_u & 0xFF
                if signed:
                    return (_to_signed(a_u, w) >> count) & m
                return a_u >> count
            case tac_ast.Divide():
                if b_u == 0:
                    raise TacSimError("division by zero")
                if signed:
                    a_s = _to_signed(a_u, w)
                    b_s = _to_signed(b_u, w)
                    q = abs(a_s) // abs(b_s)
                    if (a_s < 0) ^ (b_s < 0):
                        q = -q
                    return q & m
                return (a_u // b_u) & m
            case tac_ast.Modulo():
                if b_u == 0:
                    raise TacSimError("modulo by zero")
                if signed:
                    a_s = _to_signed(a_u, w)
                    b_s = _to_signed(b_u, w)
                    r = abs(a_s) % abs(b_s)
                    if a_s < 0:
                        r = -r
                    return r & m
                return (a_u % b_u) & m

        raise TacSimError(f"unsupported binary op: {type(op).__name__}")
