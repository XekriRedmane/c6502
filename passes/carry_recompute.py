"""Drop a shift whose only use is to set C to what C already holds.

# Pattern

`load_packed_address` in `examples/zork_print_string_literal.c` takes
three bytes of `SCRATCH2 << 1` in three statements:

    Z_PC2_L = (uint8_t)(SCRATCH2 << 1);
    Z_PC2_H = (uint8_t)(SCRATCH2 >> 7);
    Z_PC2_HH = (uint8_t)(SCRATCH2 >> 15);

`tac_to_asm` lowers `x >> 7` as the high byte of `x << 1`, so each
statement starts by putting the bit shifted out of the byte below
into C again:

    LDA   SCRATCH2
    ASL                     ; C = bit 7 of SCRATCH2
    STA   Z_PC2_L
    LDA   SCRATCH2          ; C = bit 7 of SCRATCH2, again
    ASL
    LDA   SCRATCH2+1
    ROL                     ; C = bit 7 of SCRATCH2+1
    STA   Z_PC2_H
    LDA   SCRATCH2+1        ; C = bit 7 of SCRATCH2+1, again
    ASL
    LDA   #$00
    ROL
    STA   Z_PC2_HH

The pass tracks what C holds: bit 7 of a byte `M` after `LDA M` and
`ASL A` or `ROL A`, bit 0 after `LSR A` or `ROR A`. A later `LDA M;
ASL A` (or `ROL A`, `LSR A`, `ROR A`) that would set C the same way,
with A and N/Z dead after it, goes, leaving Zork's own chain:

    LDA   SCRATCH2
    ASL
    STA   Z_PC2_L
    LDA   SCRATCH2+1
    ROL
    STA   Z_PC2_H
    LDA   #$00
    ROL
    STA   Z_PC2_HH

# What it tracks

Within a basic block (a label forgets everything): the byte A was last
loaded from, unchanged, and what C holds. A write to that byte, or to
anything that might be it (a store through a pointer, a `Data` byte
for a `ZP` one), forgets the fact, and so does an instruction that
writes C (`ADC`, `SBC`, a compare, a shift of memory, `SEC`, `CLC`, a
call). A volatile load sets nothing.

# Where to run

In the asm peephole fixed point, which both pipelines run.
"""
from __future__ import annotations

import asm_ast
from passes.asm_liveness import a_dead_at, flags_dead_at

_HI = (asm_ast.ArithmeticShiftLeft, asm_ast.RotateLeft)
_LO = (asm_ast.LogicalShiftRight, asm_ast.RotateRight)
_A = asm_ast.Reg(reg=asm_ast.A())
# Instructions that leave C alone (a `Mov` to memory may still change
# the byte a fact is about, which is checked separately).
_KEEP_C = (
    asm_ast.Mov, asm_ast.Inc, asm_ast.Dec, asm_ast.And, asm_ast.Or,
    asm_ast.Xor, asm_ast.Push, asm_ast.Pop, asm_ast.BitTest,
    asm_ast.Branch,
)


def apply_carry_recompute_drop(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            tl = _rewrite_function(tl)
        new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _mem(op) -> bool:
    return isinstance(op, (asm_ast.Data, asm_ast.ZP))


def _distinct(a, b) -> bool:
    return type(a) is type(b) and a != b


def _writes(ins):
    """The memory operand `ins` writes, or "?" for an instruction that
    may write memory it doesn't name, or None."""
    if isinstance(ins, (asm_ast.Call, asm_ast.FunctionPrologue,
                        asm_ast.AllocateStack, asm_ast.LoadAddress)):
        return "?"
    dst = getattr(ins, "dst", None)
    if dst is None or isinstance(dst, asm_ast.Reg):
        return None
    if isinstance(ins, (asm_ast.Mov, asm_ast.Inc, asm_ast.Dec)) or isinstance(
        ins, _HI + _LO,
    ):
        return dst
    return None


def _shift_kind(ins):
    if isinstance(ins, _HI) and ins.dst == _A:
        return "hi"
    if isinstance(ins, _LO) and ins.dst == _A:
        return "lo"
    return None


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    drop: set[int] = set()
    fact = None      # ("hi" | "lo", byte) that C holds
    a_src = None     # the byte A holds, unchanged since its load
    i = 0
    while i < len(instrs):
        ins = instrs[i]
        nxt = instrs[i + 1] if i + 1 < len(instrs) else None
        # A load and shift that would set C to what it holds.
        if (isinstance(ins, asm_ast.Mov) and ins.dst == _A
                and not ins.is_volatile and _mem(ins.src)
                and nxt is not None and _shift_kind(nxt) is not None
                and fact == (_shift_kind(nxt), ins.src)
                and a_dead_at(instrs, i + 2)
                and flags_dead_at(instrs, i + 2)):
            drop.update({i, i + 1})
            i += 2
            continue
        if isinstance(ins, asm_ast.Label):
            fact = a_src = None
        elif isinstance(ins, (asm_ast.Jump, asm_ast.Return, asm_ast.Ret)):
            fact = a_src = None
        else:
            kind = _shift_kind(ins)
            if kind is not None:
                fact = (kind, a_src) if a_src is not None else None
                a_src = None
            else:
                if not isinstance(ins, _KEEP_C):
                    fact = None
                written = _writes(ins)
                if written is not None:
                    for held in (fact[1] if fact else None, a_src):
                        if held is not None and (
                            written == "?" or not _distinct(written, held)
                        ):
                            if fact is not None and held == fact[1]:
                                fact = None
                            if held == a_src:
                                a_src = None
                if isinstance(ins, asm_ast.Mov) and ins.dst == _A:
                    a_src = (
                        ins.src if _mem(ins.src) and not ins.is_volatile
                        else None
                    )
                elif _writes_a(ins):
                    a_src = None
        i += 1
    if not drop:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global, params=list(fn.params),
        instructions=[x for k, x in enumerate(instrs) if k not in drop],
    )


def _writes_a(ins) -> bool:
    if isinstance(ins, asm_ast.Mov):
        # A store from anything but a register (`Mov(M, D)`,
        # `Mov(#k, D)`) emits as `LDA; STA` through A.
        return not isinstance(ins.src, asm_ast.Reg)
    if isinstance(ins, (asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
                        asm_ast.Xor)):
        return ins.dst == _A
    if isinstance(ins, asm_ast.Pop):
        return ins.dst == _A
    return isinstance(ins, (asm_ast.Call, asm_ast.FunctionPrologue,
                            asm_ast.Ret, asm_ast.AllocateStack,
                            asm_ast.LoadAddress))
