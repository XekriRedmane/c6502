"""Drops a store through a zero-page pointer of the byte read there.

# Pattern

`P->buffer += 0x100` on a pointer member reads both bytes of the
member through `P`, adds 1 to the high byte, and writes both bytes
back. The low byte goes back unchanged:

    LDY #8
    LDA (P),Y        ; the low byte
    STA t1
    INY
    LDA (P),Y        ; the high byte
    STA t0
    INC t0
    LDA t1
    DEY
    STA (P),Y        ; the low byte, as it was read
    LDA t0
    INY
    STA (P),Y

Within a basic block, the pass follows which value A and each
zero-page or absolute byte hold when that value is a byte read
through a pointer, `IndirectZp(N, k)`, the byte at `(N),Y` with `Y =
k`. A non-volatile store `IndirectZp(N, k)` of A, when A holds the
byte read from `IndirectZp(N, k)` and nothing since could have
changed that byte or the pointer, writes the value the byte already
holds, and goes. `asm_dead_store` and `dead_a_arith` then remove the
copy into `t1` and the read, and `shift_in_a` makes the rest `LDY #9;
LDA (P),Y; CLC; ADC #1; STA (P),Y`.

# What ends a fact

A fact that A or a byte holds the byte at `(N)+k` ends at

- a write of `N` or `N + 1`, which moves the pointer,
- a write that may reach `(N)+k` (`asm_aliasing.may_alias`, with the
  unit's bytes that no pointer reaches installed by `pointer_proof`).
  A store through another pointer, or to a byte a pointer can reach
  (a user zero-page object, a non-zero-page static), may. A store
  to a compiler slot no pointer reaches, or through `N` at another
  offset `k'`, doesn't, since `(N)+k'` differs from `(N)+k` while `N`
  is unchanged,
- a call, a compound atom, a label, or a branch, since the pass looks
  at one basic block at a time.

A write of a byte ends what that byte held, and a write of A ends
what A held. A volatile access never makes a fact, and a volatile
store is never dropped. Dropping the store changes no register or
flag, since `STA` writes neither, and the value in memory is the
value it held.

# Where to run

In the peephole fixed point after `memory_value_propagation`, which
makes the `IndirectZp` operands, and before `asm_dead_store`.
"""
from __future__ import annotations

from typing import Optional

import asm_ast
from passes import decimal_arith
from passes.asm_aliasing import (
    may_alias, pointer_proof, runtime_zp_addrs, unreachable_by_pointer,
)


def apply_pointer_self_store_drop(
    prog: asm_ast.Program, *, zp_symbol_addrs: dict[str, int] | None = None,
) -> asm_ast.Program:
    """Drops each store through a zero-page pointer of the byte read
    from the same place, when nothing in between can have changed it.
    `zp_symbol_addrs` gives the zero-page addresses of the slot
    symbols and zero-page objects."""
    addrs = dict(runtime_zp_addrs())
    addrs.update(zp_symbol_addrs or {})
    with pointer_proof(unreachable_by_pointer(prog)):
        top = [
            _rewrite_function(tl, addrs) if isinstance(tl, asm_ast.Function)
            else tl
            for tl in prog.top_level
        ]
    return asm_ast.Program(top_level=top)


# The value of a byte read through a pointer: (N, k) for the byte at
# `(N),Y` with `Y = k`.
_Value = tuple[int, int]

_BARRIERS = (
    asm_ast.Label, asm_ast.Jump, asm_ast.Branch, asm_ast.JumpTable,
    asm_ast.Call, asm_ast.Ret, asm_ast.Return, asm_ast.FunctionPrologue,
    asm_ast.AllocateStack, asm_ast.LoadAddress, asm_ast.Phi,
)
_KEEP_A = (
    asm_ast.Compare, asm_ast.BitTest, asm_ast.SetCarry, asm_ast.ClearCarry,
    asm_ast.Push,
)
_RMW = (
    asm_ast.Inc, asm_ast.Dec, asm_ast.ArithmeticShiftLeft,
    asm_ast.LogicalShiftRight, asm_ast.RotateLeft, asm_ast.RotateRight,
)
_ALU = (asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or, asm_ast.Xor)


def _is_a(op) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _cell(op) -> Optional[tuple]:
    """A key for a byte with a fixed address, or None."""
    if isinstance(op, asm_ast.ZP):
        return ("zp", op.address + op.offset)
    if isinstance(op, asm_ast.Data):
        return ("data", op.name, op.offset)
    return None


class _State:
    def __init__(self, zp_addrs: dict[str, int]):
        self.zp_addrs = zp_addrs
        self.a: Optional[_Value] = None
        self.cells: dict[tuple, _Value] = {}
        # The index of the load through a pointer that set A, when A
        # holds what it read.
        self.a_from: Optional[int] = None
        # For a byte whose fact a store right after such a load made,
        # the store's index, until something reads the byte.
        self.origin: dict[tuple, int] = {}

    def reset(self) -> None:
        self.a = None
        self.a_from = None
        self.cells.clear()
        self.origin.clear()

    def read(self, ins) -> None:
        """Ends the `origin` of each byte `ins` reads."""
        from passes.asm_dead_store import _read_operands
        for op in _read_operands(ins):
            key = _cell(op)
            if key is not None:
                self.origin.pop(key, None)

    def _address(self, op) -> Optional[int]:
        if isinstance(op, asm_ast.ZP):
            return op.address + op.offset
        if isinstance(op, asm_ast.Data):
            base = self.zp_addrs.get(op.name)
            return None if base is None else base + op.offset
        return None

    def write(self, dst) -> None:
        """Ends the facts a write of `dst` may change."""
        key = _cell(dst)
        if key is not None:
            self.cells.pop(key, None)
            self.origin.pop(key, None)
        addr = self._address(dst)
        unresolved = isinstance(dst, asm_ast.Data) and addr is None and (
            dst.name.startswith(("__local_", "__zpabi_"))
            or dst.name in self.zp_addrs)

        def survives(v: _Value) -> bool:
            n, k = v
            if unresolved or (addr is not None and addr in (n, n + 1)):
                return False
            return not may_alias(
                asm_ast.IndirectZp(address=n, offset=k), dst,
                zp_slot_symbols=self.zp_addrs,
            )

        if self.a is not None and not survives(self.a):
            self.a = None
            self.a_from = None
        for c, v in list(self.cells.items()):
            if not survives(v):
                del self.cells[c]
                self.origin.pop(c, None)


def _rewrite_function(fn: asm_ast.Function, zp_addrs) -> asm_ast.Function:
    state = _State(zp_addrs)
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    changed = False
    for i, ins in enumerate(instrs):
        if isinstance(ins, _BARRIERS):
            state.reset()
            out.append(ins)
            continue
        if isinstance(ins, asm_ast.Mov):
            if _drops(ins, state):
                changed = True
                continue
            new = _reload(fn, i, state)
            if new is not None:
                ins = new
                changed = True
            else:
                state.read(ins)
            _transfer_mov(ins, state, i)
            out.append(ins)
            continue
        state.read(ins)
        if isinstance(ins, asm_ast.DecimalArith):
            for w in decimal_arith.writes(ins):
                state.write(w)
            state.a = None
        elif isinstance(ins, _KEEP_A):
            pass
        elif isinstance(ins, (*_RMW, *_ALU, asm_ast.Pop)):
            if _is_a(ins.dst):
                state.a = None
            elif isinstance(ins.dst, asm_ast.Reg):
                pass
            else:
                state.write(ins.dst)
        else:
            state.reset()
        state.a_from = None
        out.append(ins)
    if not changed:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _drops(ins: asm_ast.Mov, state: _State) -> bool:
    """Whether `ins` stores A through a pointer where A holds the byte
    read from there."""
    return (not ins.is_volatile and _is_a(ins.src)
            and isinstance(ins.dst, asm_ast.IndirectZp)
            and state.a == (ins.dst.address, ins.dst.offset))


def _reload(fn: asm_ast.Function, i: int, state: _State):
    """`LDA (N),Y` with `Y = k` in place of `instrs[i]`, `LDA t`, when
    `t` holds the byte at `(N)+k`, read just before the store into `t`
    with A dead after that store, nothing has read `t` since, and `t`
    is dead after `instrs[i]`, and so is Y. The load and the store
    into `t` are then dead too, and later sweeps remove them, so that
    the byte is read once, where it's used. None otherwise."""
    from passes.asm_dead_store import mem_dead_after
    from passes.asm_liveness import a_dead_at, y_dead_at
    ins = fn.instructions[i]
    if not (isinstance(ins, asm_ast.Mov) and _is_a(ins.dst)
            and not ins.is_volatile):
        return None
    key = _cell(ins.src)
    if key is None or key not in state.origin or key not in state.cells:
        return None
    if not (isinstance(ins.src, asm_ast.Data)
            and ins.src.name.startswith(f"__local_{fn.name}__")):
        return None
    s = state.origin[key]
    instrs = fn.instructions
    if not (a_dead_at(instrs, s + 1) and y_dead_at(instrs, i + 1)
            and mem_dead_after(instrs, i, ins.src, fn_name=fn.name,
                               zp_slot_symbols=state.zp_addrs)):
        return None
    n, k = state.cells[key]
    return asm_ast.Mov(src=asm_ast.IndirectZp(address=n, offset=k),
                       dst=ins.dst)


def _transfer_mov(ins: asm_ast.Mov, state: _State, i: int) -> None:
    src, dst = ins.src, ins.dst
    if isinstance(dst, asm_ast.Reg):
        if not _is_a(dst):
            return
        state.a_from = None
        if ins.is_volatile:
            state.a = None
        elif isinstance(src, asm_ast.IndirectZp):
            state.a = (src.address, src.offset)
            state.a_from = i
        else:
            key = _cell(src)
            state.a = state.cells.get(key) if key is not None else None
        return
    # A write of memory. A store of A, X, or Y leaves A; any other move
    # into memory goes through A (`LDA src; STA dst`), except that a
    # move of X or Y through a pointer goes through A too.
    if isinstance(src, asm_ast.Reg):
        value = state.a if _is_a(src) else None
        if not _is_a(src) and _cell(dst) is None:
            state.a = None
    else:
        key = _cell(src)
        value = None
        if not ins.is_volatile:
            if isinstance(src, asm_ast.IndirectZp):
                value = (src.address, src.offset)
            elif key is not None:
                value = state.cells.get(key)
        state.a = value
    state.write(dst)
    if isinstance(src, asm_ast.Reg) and _is_a(src):
        # The write may have ended A's own fact (a store through
        # another pointer), and then the stored byte holds A's value
        # only as it was.
        value = state.a
    key = _cell(dst)
    if key is not None and value is not None and not ins.is_volatile:
        state.cells[key] = value
        if (isinstance(src, asm_ast.Reg) and _is_a(src)
                and state.a_from == i - 1):
            state.origin[key] = i
