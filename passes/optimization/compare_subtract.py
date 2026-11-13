"""TAC pass: move a subtract of a compare's operands up to the compare.

# What it does

`while (s >= k) s -= k;` compares `s` with `k` and then, on one side
of the branch, subtracts `k` from `s`:

    .loop:
        JumpIfCmp(LessThan, s, k, .break)
        Binary(Subtract, s, k, t)
        ...

On the 6502 both are the same subtraction: an unsigned `s < k` is the
borrow out of `s - k`, and a signed one is the sign of the difference
corrected for overflow. This pass moves the `Binary` to just before
the `JumpIfCmp`:

    .loop:
        Binary(Subtract, s, k, t)
        JumpIfCmp(LessThan, s, k, .break)
        ...

and `tac_to_asm` (`Translator._fused_subtract`) then lowers the pair as
the subtract followed by a branch on its flags, `LDA s; SEC; SBC k;
STA t; ...; BCC .break`, with no separate compare. That's Zork's
`SBC SECTORS_PER_TRACK; BCS` in `do_rwts_on_sector`.

# Pattern

  * A block ends in `JumpIfCmp` with an ordering operator (`<`, `<=`,
    `>`, `>=`) on integer or pointer operands.
  * One of its successors has that block as its only predecessor and
    contains `Binary(Subtract, a, b, t)`, where `a - b` is the
    difference the compare looks at: `src1 - src2` for `<` and `>=`,
    `src2 - src1` for `>` and `<=` (the operands `tac_to_asm` swaps).
  * `a` and `b` are SSA values or constants, so they hold the same
    values at the compare, and `t` is an SSA value of the operands'
    type. An operand of the subtract may also be a second read of
    the same memory as the compare's, such as `ZeroExtend(k)` of a
    global `k` in each block, when nothing writes memory between the
    two; the subtract then uses the compare's. (CSE leaves such reads
    alone, since a second read of a global costs no more than keeping
    the first in a temporary.)

The move is always sound in SSA form: the subtract is pure, its
operands are defined before the compare, and the compare's block
dominates every use of `t`. On the other side of the branch `t` is
computed and not used, which costs a subtract's stores where the
compare used to be.

# Where it runs

Once, after the fixed-point loop, so `SinkIncrements` and the other
passes that move instructions don't move it back.
"""

from __future__ import annotations

import c99_ast
import tac_ast
from passes.optimization.cfg import (
    ENTRY_ID, EXIT_ID, build_cfg, cfg_to_function,
)
from passes.optimization.framework import PostFixedpointPass

# Reads of one variable in memory, which match when the variable
# isn't written between them.
_READS = (
    tac_ast.ZeroExtend, tac_ast.SignExtend, tac_ast.Truncate, tac_ast.Copy,
)
# Instructions that don't write memory, unless their dst isn't an SSA
# value (as in `cse`).
_NO_MEMORY_WRITE = _READS + (
    tac_ast.Binary, tac_ast.Unary, tac_ast.Label, tac_ast.Phi,
    tac_ast.GetAddress, tac_ast.JumpIfCmp,
    tac_ast.Load, tac_ast.IndexedLoad, tac_ast.IndexedConstLoad,
    tac_ast.IndirectIndexedLoad,
)

_ORDERING = (
    tac_ast.LessThan, tac_ast.GreaterOrEqual,
    tac_ast.GreaterThan, tac_ast.LessOrEqual,
)


def compared_difference(op, src1, src2):
    """The `(a, b)` of the subtraction `a - b` whose flags decide an
    ordering compare `src1 op src2`, as `tac_to_asm` lowers it."""
    if isinstance(op, (tac_ast.LessThan, tac_ast.GreaterOrEqual)):
        return src1, src2
    return src2, src1


class HoistSubtractToCompare(PostFixedpointPass):
    name = "hoist_subtract_to_compare"

    def run(self, fn, ctx):
        return hoist_subtract_to_compare(fn, ctx)


def hoist_subtract_to_compare(fn: tac_ast.Function, ctx) -> tac_ast.Function:
    ssa = ctx.ssa_dsts
    if not ssa or ctx.symbols is None:
        return fn

    def type_of(v):
        if isinstance(v, tac_ast.Var):
            sym = ctx.symbols.get(v.name)
            return None if sym is None else _strip(sym.type)
        return None

    def stable(v) -> bool:
        if isinstance(v, tac_ast.Constant):
            return not isinstance(
                v.const, (tac_ast.ConstFloat, tac_ast.ConstDouble),
            )
        return isinstance(v, tac_ast.Var) and v.name in ssa

    def writes_memory(instr) -> bool:
        if not isinstance(instr, _NO_MEMORY_WRITE):
            return True
        if getattr(instr, "is_volatile", False):
            return True
        dst = getattr(instr, "dst", None)
        return isinstance(dst, tac_ast.Var) and dst.name not in ssa

    def memory_read(v, instrs):
        """`(kind, variable, type)` when `v` is defined in `instrs` by
        a read of a non-volatile variable in memory that nothing after
        it in `instrs` writes, else None."""
        if not (isinstance(v, tac_ast.Var) and v.name in ssa):
            return None
        for k, ins in enumerate(instrs):
            if isinstance(ins, _READS) and ins.dst == v:
                src = ins.src
                if (not isinstance(src, tac_ast.Var) or src.name in ssa
                        or _volatile(ctx.symbols.get(src.name))
                        or getattr(ins, "is_volatile", False)
                        or any(writes_memory(i) for i in instrs[k + 1:])):
                    return None
                return (type(ins).__name__, src.name, type_of(v))
        return None

    def matching(want, have, head, prefix):
        """`want` (the compare's operand) if `have` (the subtract's)
        is the same value: identical, or the same memory read, the
        compare's in `head` and the subtract's in `prefix`."""
        if want == have:
            return want
        key = memory_read(want, head)
        if key is not None and key == memory_read(have, prefix):
            return want
        return None

    def _hoist_from(body, instrs, a, b, t) -> bool:
        """Moves the first subtract in `body` of `a - b` (as the compare
        at the end of `instrs` sees them) to just before that compare.
        Returns whether it found one."""
        head = instrs[:-1]
        for k, ins in enumerate(body):
            if not (isinstance(ins, tac_ast.Binary)
                    and isinstance(ins.op, tac_ast.Subtract)
                    and isinstance(ins.dst, tac_ast.Var)
                    and ins.dst.name in ssa and type_of(ins.dst) == t):
                continue
            src1 = matching(a, ins.src1, head, body[:k])
            src2 = matching(b, ins.src2, head, body[:k])
            if src1 is None or src2 is None:
                continue
            del body[k]
            instrs.insert(-1, tac_ast.Binary(
                op=ins.op, src1=src1, src2=src2, dst=ins.dst,
            ))
            return True
        return False

    cfg = build_cfg(fn)
    changed = False
    for bid in cfg.block_order:
        block = cfg.blocks[bid]
        last = block.instructions[-1]
        if not (isinstance(last, tac_ast.JumpIfCmp)
                and isinstance(last.op, _ORDERING)):
            continue
        a, b = compared_difference(last.op, last.src1, last.src2)
        if not (stable(a) and stable(b)):
            continue
        t = type_of(a) or type_of(b)
        if not _integer(t) or _one_byte(t):
            continue
        for succ in block.successors:
            if succ in (ENTRY_ID, EXIT_ID):
                continue
            body = cfg.blocks[succ].instructions
            if cfg.blocks[succ].predecessors != [bid]:
                continue
            if _hoist_from(body, block.instructions, a, b, t):
                changed = True
                break
    if not changed:
        return fn
    return cfg_to_function(fn, cfg)


def _volatile(sym) -> bool:
    """True unless `sym` is a known, non-volatile variable."""
    if sym is None:
        return True
    t = sym.type
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        if isinstance(t, c99_ast.Volatile):
            return True
        t = t.referenced_type
    return False


def _strip(t):
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return t


def _one_byte(t) -> bool:
    return isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar))


def _integer(t) -> bool:
    return isinstance(t, (
        c99_ast.Char, c99_ast.SChar, c99_ast.UChar, c99_ast.Int,
        c99_ast.UInt, c99_ast.Long, c99_ast.ULong, c99_ast.LongLong,
        c99_ast.ULongLong, c99_ast.Pointer,
    ))
