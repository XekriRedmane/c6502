"""Rewrite `LDA M` to `TXA` when M is an X-save slot.

# Motivating shape

When the asm-SSA regalloc colors a loop counter (or any Pseudo
with HwReg-X-eligible uses) to `Reg(X)`, it ALSO uses a memory
slot `M = __local_<fn>__<name>` as the save/restore home for X
across calls — the standard `STX M; JSR callee; LDX M` wrap.

If the same Pseudo also appears as a value (e.g., passed as a
zp_abi callee arg via `LDA M; STA __zpabi_<callee>__<slot>`), the
post-coloring IR ends up with reads from M alongside the in-place
modifications of X (DEX / INX) which leave M stale until the next
STX M. The pattern looks like:

    LDA M                       ; ← reads stale M after DEX
    STA __zpabi_callee_slot
    STX M                       ; ← sync M (too late — LDA M already
                                ;    read the stale value)
    JSR callee
    LDX M

The intended regalloc invariant is "M and X represent the same
logical value (Reg(X) holds the live copy, M is the spill home)."
Under that invariant `LDA M` is value-equivalent to `TXA` — and
TXA is what we want, because TXA reads X (the live copy) instead
of M (which can be stale between a DEX/INX and the next STX M).

# Rewrite

For each function: enumerate the memory operands that appear as
the destination of `Mov(Reg(X), M)` (STX M) anywhere in the
function. These are the "X-save slots". Then for each
`Mov(M, Reg(A))` (LDA M) instruction whose source is one of those
slots, rewrite to `Mov(Reg(X), Reg(A))` (TXA).

Why "anywhere in the function": the invariant is global to the
function. If M is ever stored from X, the regalloc has committed
to treating M as X's spill home for the entire function. Any read
of M should therefore read X's current value.

# Soundness

The rewrite is sound when M's value at the LDA M point is supposed
to match X's value at the same point. Specifically:

- M is initialized to match X (either by direct STX M or by an
  init pattern like `STA M; TAX` that writes the same value to
  both).
- All writes to M are STX M (which trivially maintains M == X).
- All writes to X are DEX / INX (which preserves the slot
  semantics — the new X is "slot-1" or "slot+1", which is the
  correct next value of the slot).

If a non-STX write to M exists somewhere in the function, that
write could break the M == X invariant. The pass conservatively
skips any function where a non-STX write to an X-save slot exists.

# Where to run

After the asm peephole fixedpoint and `apply_loop_counter_to_x`,
before `expand_long_branches`. The rewrite produces one fewer
byte per occurrence (TXA = 1 byte vs LDA abs = 3 bytes / LDA zp =
2 bytes), so no long-branch displacements grow.
"""
from __future__ import annotations

import asm_ast


def apply_x_save_slot_load(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _is_reg(op, regtype) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, regtype)


def _is_reg_x(op) -> bool:
    return _is_reg(op, asm_ast.X)


def _is_reg_a(op) -> bool:
    return _is_reg(op, asm_ast.A)


def _op_key(op):
    if isinstance(op, asm_ast.Data):
        return ("data", op.name, op.offset)
    if isinstance(op, asm_ast.ZP):
        return ("zp", op.address, op.offset)
    return None


def _followed_by_tax(instrs, i: int) -> bool:
    """True iff `instrs[i]` is followed (skipping Labels, ignoring
    other instructions that don't touch A or X) by a `Mov(Reg(A),
    Reg(X))` (TAX) before any instruction writes A or X.

    Used to accept the init shape `LDA c; STA M; TAX` as a
    legitimate write to M that re-syncs M with X."""
    j = i + 1
    while j < len(instrs):
        instr = instrs[j]
        if isinstance(instr, asm_ast.Label):
            j += 1
            continue
        if isinstance(instr, asm_ast.Mov):
            if _is_reg_a(instr.src) and _is_reg_x(instr.dst):
                return True
            if _is_reg_a(instr.dst) or _is_reg_x(instr.dst):
                return False
            j += 1
            continue
        # Anything else: bail. Conservative.
        return False
    return False


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    # Pass 1: collect candidates — every memory operand that appears
    # as the dst of `Mov(Reg(X), M)` (STX M).
    candidates: set[tuple] = set()
    for instr in instrs:
        if isinstance(instr, asm_ast.Mov) and _is_reg_x(instr.src):
            key = _op_key(instr.dst)
            if key is not None:
                candidates.add(key)
    if not candidates:
        return fn

    # Pass 2: reject any candidate that has a write to it which
    # could leave M != X. A bare `Mov(<something>, M)` that isn't
    # `STX M` is acceptable IF it's paired with a downstream
    # `TAX` (`Mov(Reg(A), Reg(X))`) that re-syncs X with A — i.e.
    # the init shape `LDA c; STA M; TAX`, which makes M = X = c.
    # Any in-place RMW (Inc / Dec / shift / arith) into M, or
    # a Pop into M, breaks the invariant.
    disqualified: set[tuple] = set()
    for i, instr in enumerate(instrs):
        if isinstance(instr, asm_ast.Mov):
            key = _op_key(instr.dst)
            if key is None or key not in candidates:
                continue
            if _is_reg_x(instr.src):
                continue   # STX M — fine.
            if _is_reg(instr.src, asm_ast.Y):
                # STY M leaves A unchanged, so a following TAX
                # wouldn't pick up M's value. Disqualify.
                disqualified.add(key)
                continue
            if _followed_by_tax(instrs, i):
                # `Mov(src, M)` lowers at emit to `LDA src; STA M`
                # (when src isn't a register) or `STA M` (when src
                # is Reg(A)); either way A ends up holding M's
                # value. A subsequent TAX makes X = A = M. This
                # is the canonical init pattern `Mov(<v>, M);
                # Mov(Reg(A), Reg(X))`. The mem-to-mem Mov's
                # implicit A-load is invisible at the IR level
                # ([[mem-to-mem-mov-hides-emit-time-lda]]).
                continue
            disqualified.add(key)
            continue
        if isinstance(instr, (asm_ast.Inc, asm_ast.Dec,
                              asm_ast.ArithmeticShiftLeft,
                              asm_ast.LogicalShiftRight,
                              asm_ast.RotateLeft, asm_ast.RotateRight)):
            key = _op_key(instr.dst)
            if key is not None and key in candidates:
                disqualified.add(key)
            continue
        if isinstance(instr, (asm_ast.Add, asm_ast.Sub,
                              asm_ast.And, asm_ast.Or, asm_ast.Xor)):
            key = _op_key(instr.dst)
            if key is not None and key in candidates:
                disqualified.add(key)
            continue
        if isinstance(instr, asm_ast.Pop):
            key = _op_key(instr.dst)
            if key is not None and key in candidates:
                disqualified.add(key)
            continue
    final = candidates - disqualified
    if not final:
        return fn

    # Pass 3: rewrite reads of M to reads of X.
    #
    #   `Mov(M, Reg(A))`  → `Mov(Reg(X), Reg(A))` (LDA M → TXA).
    #   `Mov(M, D)` where D is `Data` or `ZP` (the addressing modes
    #   STX supports) → `Mov(Reg(X), D)` (the mem-to-mem
    #   `LDA M; STA D` lowering becomes a single `STX D`).
    #
    # Mem-to-mem `Mov(M, D)` for non-STX-addressable D (Frame /
    # Stack / Indirect / IndexedData) is left alone — STX has no
    # indirect-Y or abs,X form, so we can't fold these directly.
    # `Mov(M, Reg(X))` is already a TAX/LDX shape that copy-prop
    # / direct_index_load handle; leave it.
    reg_x = asm_ast.Reg(reg=asm_ast.X())
    new_instrs: list[asm_ast.Type_instruction] = []
    for instr in instrs:
        if isinstance(instr, asm_ast.Mov):
            src_key = _op_key(instr.src)
            if src_key in final:
                if _is_reg_a(instr.dst):
                    new_instrs.append(
                        asm_ast.Mov(src=reg_x, dst=instr.dst,
                                    is_volatile=instr.is_volatile),
                    )
                    continue
                if isinstance(instr.dst, (asm_ast.Data, asm_ast.ZP)):
                    new_instrs.append(
                        asm_ast.Mov(src=reg_x, dst=instr.dst,
                                    is_volatile=instr.is_volatile),
                    )
                    continue
        new_instrs.append(instr)
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=new_instrs,
    )
