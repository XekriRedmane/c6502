"""TAC simulator: runs TAC programs in Python.

Each TAC instruction maps to a Python operation that mirrors the
6502 lowering's bit-level semantics. Scalar values live in the
environment as unsigned N-byte ints (the byte-pattern view, same
as what the soft stack would hold). Signedness is re-applied only
at instructions where it matters (Divide, Modulo, ordering
comparisons, RightShift, SignExtend). Address-taken locals and
static-storage variables live in a flat byte-addressed memory
map and are read / written through their addresses.

Scope (this module): integer types (Int / UInt / Long / ULong /
LongLong / ULongLong / Char / SChar / UChar), Pointer, Float,
Double; arithmetic + bitwise + comparison + shift on integers;
arithmetic + comparison + Negate + LogicalNot on FP (via
fp_arith); control flow with FP-aware truthiness; Copy,
FunctionCall, IndirectCall, Ret, integer cast nodes (SignExtend
/ ZeroExtend / Truncate), FP cast nodes (IntToFloat /
IntToDouble / FloatToInt / DoubleToInt / FloatToDouble /
DoubleToFloat), GetAddress / Load / Store, StaticVariable
initialization (including AddressInit, StringInit, ZeroInit,
FloatInit, DoubleInit), struct / union pass-by-value and sret
returns (when a TypeTable is provided to the constructor).

Intended use: exercise C semantics without depending on the
(still-landing) 6502 runtime helpers, and pin TAC behavior as a
fixed point against future TAC optimization passes — same input
program should yield the same simulator trace before and after
any TAC-level transform.
"""
from __future__ import annotations

from dataclasses import dataclass, field

import c99_ast
import fp_arith
import tac_ast
from passes.type_checking import (
    Char,
    Double,
    Float,
    Int,
    Long,
    LongLong,
    Pointer,
    SChar,
    StaticAttr,
    SymbolTable,
    Type,
    TypeTable,
    UChar,
    UInt,
    ULong,
    ULongLong,
)


class TacSimError(Exception):
    pass


# Scalar type → (signed, width_bytes, is_fp). Pointer is treated
# as unsigned (addresses are inherently unsigned in the runtime
# model). Char-types match c6502's "plain char is unsigned" rule
# (the C99 §6.2.5.15 implementation-defined choice — only `signed
# char` is signed in c6502; plain `char` matches `unsigned char`).
# For FP types `signed` is irrelevant (FP dispatch never consults
# it — sign / unsigned splits don't apply); width selects single
# vs. double precision.
_TYPE_INFO: dict[type, tuple[bool, int, bool]] = {
    Char:      (False, 1, False),
    SChar:     (True,  1, False),
    UChar:     (False, 1, False),
    Int:       (True,  2, False),
    UInt:      (False, 2, False),
    Pointer:   (False, 2, False),
    Long:      (True,  4, False),
    ULong:     (False, 4, False),
    Float:     (False, 4, True),
    LongLong:  (True,  8, False),
    ULongLong: (False, 8, False),
    Double:    (False, 8, True),
}


def _type_info(t: Type) -> tuple[bool, int, bool]:
    info = _TYPE_INFO.get(type(t))
    if info is None:
        raise TacSimError(f"unsupported type in simulator: {type(t).__name__}")
    return info


def _const_info(c: tac_ast.Type_const) -> tuple[bool, int, bool, int]:
    """(signed, width_bytes, is_fp, unsigned_value) for a TAC constant.

    For FP variants the value is the IEEE 754 bit pattern."""
    match c:
        case tac_ast.ConstChar(value=v):      return (True,  1, False, v & 0xFF)
        case tac_ast.ConstUChar(value=v):     return (False, 1, False, v & 0xFF)
        case tac_ast.ConstInt(value=v):       return (True,  2, False, v & 0xFFFF)
        case tac_ast.ConstUInt(value=v):      return (False, 2, False, v & 0xFFFF)
        case tac_ast.ConstLong(value=v):      return (True,  4, False, v & 0xFFFFFFFF)
        case tac_ast.ConstULong(value=v):     return (False, 4, False, v & 0xFFFFFFFF)
        case tac_ast.ConstLongLong(value=v):  return (True,  8, False, v & 0xFFFFFFFFFFFFFFFF)
        case tac_ast.ConstULongLong(value=v): return (False, 8, False, v & 0xFFFFFFFFFFFFFFFF)
        case tac_ast.ConstFloat(bits=b):      return (False, 4, True,  b & 0xFFFFFFFF)
        case tac_ast.ConstDouble(bits=b):     return (False, 8, True,  b & 0xFFFFFFFFFFFFFFFF)
    raise TacSimError(f"unsupported const variant: {type(c).__name__}")


def _mask(width: int) -> int:
    return (1 << (8 * width)) - 1


def _to_signed(u: int, width: int) -> int:
    half = 1 << (8 * width - 1)
    return u - 2 * half if u >= half else u


def _is_aggregate(t: Type) -> bool:
    return isinstance(t, (c99_ast.Structure, c99_ast.Union))


def _sizeof(t: Type, types: TypeTable | None = None) -> int:
    """Bytes occupied by an object of type `t`. Recursive for Array;
    Structure / Union require a TypeTable."""
    if isinstance(t, c99_ast.Array):
        return _sizeof(t.element_type, types) * t.size
    if _is_aggregate(t):
        if types is None:
            raise TacSimError(
                f"cannot size aggregate type without TypeTable: {t!r}"
            )
        layout = types.get(t.tag)
        if layout is None or not layout.complete:
            raise TacSimError(f"incomplete aggregate type: {t.tag!r}")
        return layout.size
    _, width, _ = _type_info(t)
    return width


def _init_size(init: tac_ast.Type_static_init) -> int:
    """Bytes laid down by a single static_init item."""
    match init:
        case tac_ast.CharInit() | tac_ast.UCharInit():         return 1
        case tac_ast.IntInit() | tac_ast.UIntInit():           return 2
        case tac_ast.LongInit() | tac_ast.ULongInit():         return 4
        case tac_ast.LongLongInit() | tac_ast.ULongLongInit(): return 8
        case tac_ast.FloatInit():                              return 4
        case tac_ast.DoubleInit():                             return 8
        case tac_ast.AddressInit():                            return 2
        case tac_ast.StringInit(bytes=n):                      return n
        case tac_ast.ZeroInit(bytes=n):                        return n
    raise TacSimError(f"unsupported static_init: {type(init).__name__}")


class Memory:
    """Sparse byte-addressed memory. Reads of unset addresses return 0
    (treats untouched memory as zero, which matches BSS semantics
    closely enough for simulation; the simulator doesn't try to be
    strict about uninitialized reads)."""

    def __init__(self, base: int = 0x1000) -> None:
        self.bytes: dict[int, int] = {}
        self.next_addr: int = base

    def allocate(self, size: int) -> int:
        addr = self.next_addr
        self.next_addr += size
        return addr

    def store(self, addr: int, value: int, width: int) -> None:
        for i in range(width):
            self.bytes[addr + i] = (value >> (8 * i)) & 0xFF

    def load(self, addr: int, width: int) -> int:
        v = 0
        for i in range(width):
            v |= self.bytes.get(addr + i, 0) << (8 * i)
        return v

    def write_bytes(self, addr: int, data: bytes) -> None:
        for i, b in enumerate(data):
            self.bytes[addr + i] = b


@dataclass
class _Frame:
    function: tac_ast.Function
    env: dict[str, int] = field(default_factory=dict)
    local_addr: dict[str, int] = field(default_factory=dict)
    pc: int = 0


@dataclass
class _Return:
    value: int | None
    width: int = 0


class Simulator:
    """Run TAC programs.

    >>> sim = Simulator(tac_program, symbols)
    >>> sim.call("main", [])

    After a call, static variables can be inspected with
    `sim.read_static(name)` (returns a Python int, signed if the
    type is signed) or `sim.memory.load(addr, width)` for
    finer-grained access.
    """

    def __init__(
        self,
        program: tac_ast.Program,
        symbols: SymbolTable,
        types: TypeTable | None = None,
    ) -> None:
        self.program = program
        self.symbols = symbols
        self.types = types
        self.memory = Memory()

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
        # Address-taken locals: each function's set of param /
        # local names that ever appear as the operand of GetAddress
        # in that function's body.
        self._address_taken: dict[str, set[str]] = {}
        for fn in self._functions.values():
            taken: set[str] = set()
            for ins in fn.instructions:
                if isinstance(ins, tac_ast.GetAddress) and isinstance(
                    ins.operand, tac_ast.Var
                ):
                    taken.add(ins.operand.name)
            self._address_taken[fn.name] = taken

        # Static-storage and function-address layout:
        #   pass 1: allocate an address per StaticVariable (sizes
        #     known up front from each item's _init_size).
        #   pass 2: allocate a fake 2-byte address per function so
        #     `&foo` → GetAddress and IndirectCall(ptr) →
        #     function-name lookup work without a real code segment.
        #     The bytes themselves stay zero — only the address is
        #     used as an identity token.
        #   pass 3: lay down static init bytes — runs after function
        #     addresses are known so AddressInit("foo", _) for a
        #     file-scope function pointer can resolve.
        self._static_addr: dict[str, int] = {}
        for top in program.top_level:
            if isinstance(top, tac_ast.StaticVariable):
                size = sum(_init_size(i) for i in top.init)
                self._static_addr[top.name] = self.memory.allocate(size)
        self._function_addr: dict[str, int] = {}
        self._addr_to_function: dict[int, str] = {}
        for fn_name in self._functions:
            addr = self.memory.allocate(2)
            self._function_addr[fn_name] = addr
            self._addr_to_function[addr] = fn_name
        for top in program.top_level:
            if isinstance(top, tac_ast.StaticVariable):
                self._lay_down_static(top)

    def _lay_down_static(self, sv: tac_ast.StaticVariable) -> None:
        addr = self._static_addr[sv.name]
        for item in sv.init:
            match item:
                case tac_ast.CharInit(value=v) | tac_ast.UCharInit(value=v):
                    self.memory.store(addr, v, 1); addr += 1
                case tac_ast.IntInit(value=v) | tac_ast.UIntInit(value=v):
                    self.memory.store(addr, v, 2); addr += 2
                case tac_ast.LongInit(value=v) | tac_ast.ULongInit(value=v):
                    self.memory.store(addr, v, 4); addr += 4
                case tac_ast.LongLongInit(value=v) | tac_ast.ULongLongInit(value=v):
                    self.memory.store(addr, v, 8); addr += 8
                case tac_ast.FloatInit(bits=b):
                    self.memory.store(addr, b, 4); addr += 4
                case tac_ast.DoubleInit(bits=b):
                    self.memory.store(addr, b, 8); addr += 8
                case tac_ast.AddressInit(name=n, offset=off):
                    target = self._static_addr.get(n)
                    if target is None:
                        target = self._function_addr.get(n)
                    if target is None:
                        raise TacSimError(f"AddressInit references unknown {n!r}")
                    self.memory.store(addr, target + off, 2); addr += 2
                case tac_ast.StringInit(str=s, bytes=n):
                    raw = s.encode("latin-1")
                    self.memory.write_bytes(addr, raw)
                    # Zero-pad the rest.
                    for i in range(len(raw), n):
                        self.memory.bytes[addr + i] = 0
                    addr += n
                case tac_ast.ZeroInit(bytes=n):
                    for i in range(n):
                        self.memory.bytes[addr + i] = 0
                    addr += n
                case _:
                    raise TacSimError(
                        f"unsupported static_init: {type(item).__name__}"
                    )

    def call(self, name: str, args: list[int]) -> int | None:
        fn = self._functions.get(name)
        if fn is None:
            raise TacSimError(f"unknown function: {name!r}")
        if len(args) != len(fn.params):
            raise TacSimError(
                f"{name}: expected {len(fn.params)} args, got {len(args)}"
            )
        frame = _Frame(fn)
        # Allocate memory for any address-taken local in this
        # function. Params and other locals share the same set
        # — params first because they get values right now;
        # other locals get zero-initialized regions on first
        # access (Memory's sparse-zero default handles that).
        # Aggregate-typed (struct / union) params are always
        # memory-resident: their value in `args` is the source
        # struct's address, and we copy `size` bytes from there
        # into a freshly allocated region for the callee.
        taken = self._address_taken[fn.name]
        for pname, raw in zip(fn.params, args):
            sym = self.symbols[pname]
            if _is_aggregate(sym.type):
                size = _sizeof(sym.type, self.types)
                addr = self.memory.allocate(size)
                frame.local_addr[pname] = addr
                self._memcpy(addr, raw, size)
                continue
            _, w, _ = _type_info(sym.type)
            masked = raw & _mask(w)
            if pname in taken:
                addr = self.memory.allocate(w)
                frame.local_addr[pname] = addr
                self.memory.store(addr, masked, w)
            else:
                frame.env[pname] = masked
        ret = self._run(frame)
        if ret.value is None:
            return None
        ret_type = self.symbols[name].type.ret
        signed, _, is_fp = _type_info(ret_type)
        if is_fp:
            # Return value is the raw IEEE 754 bit pattern.
            return ret.value
        return _to_signed(ret.value, ret.width) if signed else ret.value

    def _memcpy(self, dst: int, src: int, size: int) -> None:
        """Copy `size` bytes from `src` to `dst`. Source bytes
        outside the populated region read as 0 (matches Memory's
        sparse-zero default)."""
        for i in range(size):
            self.memory.bytes[dst + i] = self.memory.bytes.get(src + i, 0)

    def read_static(self, name: str) -> int:
        """Read the current value of a scalar static variable as a
        Python int (signed if the declared type is signed). For FP
        statics, returns the IEEE 754 bit pattern."""
        sym = self.symbols.get(name)
        if sym is None or not isinstance(sym.attrs, StaticAttr):
            raise TacSimError(f"not a static variable: {name!r}")
        addr = self._static_addr[name]
        signed, w, is_fp = _type_info(sym.type)
        raw = self.memory.load(addr, w)
        if is_fp:
            return raw
        return _to_signed(raw, w) if signed else raw

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
                if self._is_truthy(*self._read(frame, c)):
                    frame.pc = self._labels[frame.function.name][t]

            case tac_ast.JumpIfFalse(condition=c, target=t):
                if not self._is_truthy(*self._read(frame, c)):
                    frame.pc = self._labels[frame.function.name][t]

            case tac_ast.Copy(src=s, dst=d):
                if self._is_aggregate_var(d):
                    src_addr = self._address_of(frame, s)
                    dst_addr = self._address_of(frame, d)
                    self._memcpy(
                        dst_addr, src_addr,
                        _sizeof(self.symbols[d.name].type, self.types),
                    )
                else:
                    v, _ = self._read(frame, s)
                    self._write(frame, d, v)

            case tac_ast.Unary(op=op, src=s, dst=d):
                v, ta = self._read(frame, s)
                self._write(frame, d, self._eval_unary(op, v, ta))

            case tac_ast.Binary(op=op, src1=a, src2=b, dst=d):
                va, ta = self._read(frame, a)
                vb, tb = self._read(frame, b)
                # C99 §6.3.1.8 usual arithmetic conversions: when the
                # operands have the same width but different signedness,
                # the unsigned type wins (relevant for ordering, division
                # / modulo, and right shift). c99_to_tac elides the
                # same-width sign-changing cast, leaving the operands
                # tagged with their original symbol-table types — so
                # the dispatch has to combine both. This mirrors the
                # rule tac_to_asm uses for ordering at line ~1226.
                s_a, w, is_fp = ta
                s_b, _, _ = tb
                effective = (s_a and s_b, w, is_fp)
                self._write(frame, d, self._eval_binary(op, va, vb, effective))

            case tac_ast.SignExtend(src=s, dst=d):
                vu, (_, w_src, _) = self._read(frame, s)
                vs = _to_signed(vu, w_src)
                self._write(frame, d, vs & _mask(self._dst_width(d)))

            case tac_ast.ZeroExtend(src=s, dst=d) | tac_ast.Truncate(src=s, dst=d):
                vu, _ = self._read(frame, s)
                self._write(frame, d, vu & _mask(self._dst_width(d)))

            case tac_ast.IntToFloat(src=s, dst=d):
                vu, (signed, w, _) = self._read(frame, s)
                v_int = _to_signed(vu, w) if signed else vu
                self._write(frame, d, fp_arith.int_to_single_bits(v_int))

            case tac_ast.IntToDouble(src=s, dst=d):
                vu, (signed, w, _) = self._read(frame, s)
                v_int = _to_signed(vu, w) if signed else vu
                self._write(frame, d, fp_arith.int_to_double_bits(v_int))

            case tac_ast.FloatToInt(src=s, dst=d):
                vu, _ = self._read(frame, s)
                v_int = fp_arith.single_bits_to_int(vu)
                self._write(frame, d, v_int & _mask(self._dst_width(d)))

            case tac_ast.DoubleToInt(src=s, dst=d):
                vu, _ = self._read(frame, s)
                v_int = fp_arith.double_bits_to_int(vu)
                self._write(frame, d, v_int & _mask(self._dst_width(d)))

            case tac_ast.FloatToDouble(src=s, dst=d):
                vu, _ = self._read(frame, s)
                self._write(frame, d, fp_arith.single_bits_to_double_bits(vu))

            case tac_ast.DoubleToFloat(src=s, dst=d):
                vu, _ = self._read(frame, s)
                self._write(frame, d, fp_arith.double_bits_to_single_bits(vu))

            case tac_ast.GetAddress(operand=op, dst=d):
                addr = self._address_of(frame, op)
                self._write(frame, d, addr)

            case tac_ast.Load(src_ptr=p, dst=d):
                ptr, _ = self._read(frame, p)
                w = self._dst_width(d)
                self._write(frame, d, self.memory.load(ptr, w))

            case tac_ast.Store(src=s, dst_ptr=p):
                if self._is_aggregate_var(s):
                    src_addr = self._address_of(frame, s)
                    dst_addr, _ = self._read(frame, p)
                    self._memcpy(
                        dst_addr, src_addr,
                        _sizeof(self.symbols[s.name].type, self.types),
                    )
                else:
                    v, (_, w, _) = self._read(frame, s)
                    ptr, _ = self._read(frame, p)
                    self.memory.store(ptr, v, w)

            case tac_ast.IndexedLoad(name=name, index=i, dst=d):
                # Static-array indexed load: take name's static
                # address, add the byte index, read N bytes.
                base = self._static_addr.get(name)
                if base is None:
                    raise TacSimError(
                        f"IndexedLoad on unknown static {name!r}",
                    )
                idx_val, _ = self._read(frame, i)
                w = self._dst_width(d)
                self._write(
                    frame, d, self.memory.load(base + (idx_val & 0xFF), w),
                )

            case tac_ast.IndexedSymbolStore(name=name, index=i, src=s):
                # Mirror of IndexedLoad for stores: take name's
                # static address, add the byte index, write N
                # bytes from src.
                base = self._static_addr.get(name)
                if base is None:
                    raise TacSimError(
                        f"IndexedSymbolStore on unknown static {name!r}",
                    )
                idx_val, _ = self._read(frame, i)
                v, (_, w, _) = self._read(frame, s)
                self.memory.store(base + (idx_val & 0xFF), v, w)

            case tac_ast.Ret(val=None):
                return _Return(None, 0)

            case tac_ast.Ret(val=v):
                vu, (_, w, _) = self._read(frame, v)
                return _Return(vu, w)

            case tac_ast.FunctionCall(name=name, args=args, dst=dst):
                py_args = [self._read_call_arg(frame, a) for a in args]
                rv = self.call(name, py_args)
                self._capture_return(frame, dst, rv)

            case tac_ast.IndirectCall(ptr=p, args=args, dst=dst):
                addr, _ = self._read(frame, p)
                fn_name = self._addr_to_function.get(addr)
                if fn_name is None:
                    raise TacSimError(
                        f"IndirectCall to unknown address {addr:#x}"
                    )
                py_args = [self._read_call_arg(frame, a) for a in args]
                rv = self.call(fn_name, py_args)
                self._capture_return(frame, dst, rv)

            case _:
                raise TacSimError(
                    f"unsupported instruction: {type(ins).__name__}"
                )

        return None

    def _is_aggregate_var(self, val) -> bool:
        return (
            isinstance(val, tac_ast.Var)
            and _is_aggregate(self.symbols[val.name].type)
        )

    def _read_call_arg(self, frame: _Frame, val) -> int:
        """For struct-typed args, the "value" passed across the call
        is the source struct's address — the callee then memcpys
        from there into a fresh region. For scalars, return the
        ordinary unsigned-int value."""
        if self._is_aggregate_var(val):
            return self._address_of(frame, val)
        return self._read(frame, val)[0]

    def _capture_return(self, frame: _Frame, dst, rv) -> None:
        """Common tail of FunctionCall / IndirectCall: write the
        callee's return value into `dst` (if both are present).
        Mirrors the soft-stack convention's return-value capture
        — a no-op for void-returning callees or expression-stmt
        calls that drop the result."""
        if dst is not None and rv is not None:
            self._write(frame, dst, rv & _mask(self._dst_width(dst)))

    @staticmethod
    def _is_truthy(value: int, ta: tuple[bool, int, bool]) -> bool:
        """C99 §6.3.1.2 truthiness — non-zero for integers; for FP,
        anything that compares unequal to 0 (so NaN is truthy and
        -0.0 is falsy, both via fp_arith)."""
        _, w, is_fp = ta
        if is_fp:
            return (fp_arith.double_is_truthy(value) if w == 8
                    else fp_arith.single_is_truthy(value))
        return value != 0

    def _address_of(self, frame: _Frame, val: tac_ast.Type_val) -> int:
        if not isinstance(val, tac_ast.Var):
            raise TacSimError(f"GetAddress operand must be a Var, got {val}")
        name = val.name
        # Function names: addresses live in a parallel reverse map
        # so IndirectCall can recover the target function name from
        # the runtime pointer value.
        if name in self._function_addr:
            return self._function_addr[name]
        if name in self._static_addr:
            return self._static_addr[name]
        if name in frame.local_addr:
            return frame.local_addr[name]
        # Lazy allocation for address-taken locals: the pre-pass
        # marked the name; we allocate on first GetAddress. If the
        # local already had an env value (rare — usually the first
        # mention is the GetAddress itself), migrate it into memory.
        sym = self.symbols.get(name)
        if sym is None:
            raise TacSimError(f"unknown var: {name!r}")
        size = _sizeof(sym.type, self.types)
        addr = self.memory.allocate(size)
        frame.local_addr[name] = addr
        if name in frame.env:
            _, w, _ = _type_info(sym.type)
            self.memory.store(addr, frame.env[name], w)
            del frame.env[name]
        return addr

    def _read(self, frame: _Frame, val) -> tuple[int, tuple[bool, int, bool]]:
        match val:
            case tac_ast.Constant(const=c):
                signed, w, is_fp, vu = _const_info(c)
                return vu, (signed, w, is_fp)
            case tac_ast.Var(name=n):
                sym = self.symbols[n]
                ti = _type_info(sym.type)
                _, w, _ = ti
                if isinstance(sym.attrs, StaticAttr):
                    return self.memory.load(self._static_addr[n], w), ti
                if n in frame.local_addr:
                    return self.memory.load(frame.local_addr[n], w), ti
                if n not in frame.env:
                    raise TacSimError(f"uninitialized var: {n}")
                return frame.env[n], ti
        raise TacSimError(f"unsupported val: {type(val).__name__}")

    def _write(self, frame: _Frame, val, raw: int) -> None:
        if not isinstance(val, tac_ast.Var):
            raise TacSimError(f"can only write to Var, got {type(val).__name__}")
        sym = self.symbols[val.name]
        _, w, _ = _type_info(sym.type)
        masked = raw & _mask(w)
        if isinstance(sym.attrs, StaticAttr):
            self.memory.store(self._static_addr[val.name], masked, w)
        elif val.name in frame.local_addr:
            self.memory.store(frame.local_addr[val.name], masked, w)
        else:
            frame.env[val.name] = masked

    def _dst_width(self, val) -> int:
        if not isinstance(val, tac_ast.Var):
            raise TacSimError(f"expected Var as dst, got {type(val).__name__}")
        _, w, _ = _type_info(self.symbols[val.name].type)
        return w

    @staticmethod
    def _eval_unary(op, v_u: int, ta: tuple[bool, int, bool]) -> int:
        _, w, is_fp = ta
        m = _mask(w)
        if is_fp:
            match op:
                case tac_ast.Negate():
                    return (fp_arith.double_negate(v_u) if w == 8
                            else fp_arith.single_negate(v_u))
                case tac_ast.LogicalNot():
                    truthy = (fp_arith.double_is_truthy(v_u) if w == 8
                              else fp_arith.single_is_truthy(v_u))
                    return 0 if truthy else 1
                case tac_ast.Complement():
                    raise TacSimError("Complement is not defined for FP types")
            raise TacSimError(f"unsupported FP unary op: {type(op).__name__}")
        match op:
            case tac_ast.Negate():     return (-_to_signed(v_u, w)) & m
            case tac_ast.Complement(): return (~v_u) & m
            case tac_ast.LogicalNot(): return 1 if v_u == 0 else 0
        raise TacSimError(f"unsupported unary op: {type(op).__name__}")

    @staticmethod
    def _eval_binary(op, a_u: int, b_u: int, ta: tuple[bool, int, bool]) -> int:
        signed, w, is_fp = ta
        m = _mask(w)

        if is_fp:
            return Simulator._eval_binary_fp(op, a_u, b_u, w)

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

    @staticmethod
    def _eval_binary_fp(op, a_bits: int, b_bits: int, w: int) -> int:
        """FP arithmetic / comparison dispatch via fp_arith. `w` selects
        single (4) vs. double (8) precision. Comparison results are
        always 1-byte 0/1 (Int)."""
        if isinstance(op, (tac_ast.Equal, tac_ast.NotEqual,
                           tac_ast.LessThan, tac_ast.GreaterThan,
                           tac_ast.LessOrEqual, tac_ast.GreaterOrEqual)):
            tag = (fp_arith.double_compare(a_bits, b_bits) if w == 8
                   else fp_arith.single_compare(a_bits, b_bits))
            # Per C99 §6.5.8.5: `==` is true iff both equal (false on
            # unordered); ordering relations are false on unordered.
            # `!=` is the negation of `==`, so it's true on unordered.
            match op:
                case tac_ast.Equal():          return 1 if tag == "eq" else 0
                case tac_ast.NotEqual():       return 0 if tag == "eq" else 1
                case tac_ast.LessThan():       return 1 if tag == "lt" else 0
                case tac_ast.GreaterThan():    return 1 if tag == "gt" else 0
                case tac_ast.LessOrEqual():    return 1 if tag in ("lt", "eq") else 0
                case tac_ast.GreaterOrEqual(): return 1 if tag in ("gt", "eq") else 0
        if w == 8:
            match op:
                case tac_ast.Add():      return fp_arith.double_add(a_bits, b_bits)
                case tac_ast.Subtract(): return fp_arith.double_sub(a_bits, b_bits)
                case tac_ast.Multiply(): return fp_arith.double_mul(a_bits, b_bits)
                case tac_ast.Divide():   return fp_arith.double_div(a_bits, b_bits)
        else:
            match op:
                case tac_ast.Add():      return fp_arith.single_add(a_bits, b_bits)
                case tac_ast.Subtract(): return fp_arith.single_sub(a_bits, b_bits)
                case tac_ast.Multiply(): return fp_arith.single_mul(a_bits, b_bits)
                case tac_ast.Divide():   return fp_arith.single_div(a_bits, b_bits)
        raise TacSimError(f"unsupported FP binary op: {type(op).__name__}")
