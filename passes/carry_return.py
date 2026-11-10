"""Peepholes for the carry-flag return of `return_bool_in_carry`.

A function declared `__attribute__((return_bool_in_carry))` returns
its truth value in the carry flag. `tac_to_asm` lowers both sides of
that convention plainly:

    callee:  LDA value ; CMP #1 ; RTS     C = (value >= 1)
    caller:  JSR f ; LDA #0 ; ROL A       A = C, so 0 or 1

and these rewrites tidy up what the rest of the pipeline leaves.

`apply_carry_return_tails` copies a `CMP #c; RTS` return tail into
each `LDA #k; JMP tail` that reaches it, and in front of the tail's
label when the code falling into it ends in `LDA #k`. A comparison
or a `return true;` inside a loop reaches the shared tail that way,
with a constant in A. The copy puts the constant next to the
compare, where `apply_carry_const_fold` can fold it.

`apply_carry_const_fold` turns `LDA #k; CMP #c` into `SEC` (when
k >= c, unsigned) or `CLC`, when A and N/Z are dead after it. Only C
reaches the next reader, and C is known at compile time.

`apply_carry_branch_fold` reads the caller's captured carry
directly:

    LDA #0 ; ROL A ; BEQ L    becomes    BCC L
    LDA #0 ; ROL A ; BNE L    becomes    BCS L

when A, N, Z, and C are dead on both sides of the branch. After `ROL
A`, A is the old C and Z is set iff A is 0, so BEQ is taken iff C
was clear. The rewrite leaves C as the callee set it and N/Z as they
were, instead of C = 0, N = 0, Z = !C, which is why those have to be
dead. It also drops

    LDA #0 ; ROL A ; CMP #1

when A and N/Z are dead after it, since `CMP #1` on the 0 / 1 in A
sets C back to the value `ROL` read. That's `return g();` in one
carry-flag function calling another.
"""

from __future__ import annotations

import asm_ast
from passes.asm_liveness import (
    a_dead_at, c_dead_at, flags_dead_at,
)

_A = asm_ast.Reg(reg=asm_ast.A())


def _is_imm_to_a(instr: asm_ast.Type_instruction) -> bool:
    return (
        isinstance(instr, asm_ast.Mov)
        and isinstance(instr.src, asm_ast.Imm)
        and instr.dst == _A
    )


def _is_cmp_a_imm(instr: asm_ast.Type_instruction) -> bool:
    return (
        isinstance(instr, asm_ast.Compare)
        and instr.left == _A
        and isinstance(instr.right, asm_ast.Imm)
    )


def _map_functions(prog: asm_ast.Program, fn) -> asm_ast.Program:
    top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            instrs = fn(tl.instructions)
            if instrs is not None:
                tl = asm_ast.Function(
                    name=tl.name, is_global=tl.is_global,
                    params=list(tl.params), instructions=instrs,
                )
        top.append(tl)
    return asm_ast.Program(top_level=top)


# ---------------------------------------------------------------------
# Tail copies
# ---------------------------------------------------------------------

def apply_carry_return_tails(prog: asm_ast.Program) -> asm_ast.Program:
    """Copies `CMP #c; RTS` carry-flag return tails into the paths
    that load a constant into A before reaching them."""
    return _map_functions(prog, _copy_tails)


def _tail_after(instrs, i):
    """The `[Compare, Return]` carry-flag tail at `instrs[i]`, skipping
    labels, or None. Only a bare `Return` that carries nothing but C
    qualifies: copying a `Ret` would copy its frame teardown, and the
    fold needs A to be dead at the return."""
    while i < len(instrs) and isinstance(instrs[i], asm_ast.Label):
        i += 1
    if i + 1 >= len(instrs):
        return None
    cmp, ret = instrs[i], instrs[i + 1]
    if not _is_cmp_a_imm(cmp):
        return None
    if not (isinstance(ret, asm_ast.Return) and ret.save_c
            and not ret.save_a and not ret.save_x):
        return None
    return [cmp, ret]


def _copy_tails(instrs):
    labels = {
        x.name: k for k, x in enumerate(instrs)
        if isinstance(x, asm_ast.Label)
    }
    out: list[asm_ast.Type_instruction] = []
    changed = False
    for i, instr in enumerate(instrs):
        prev_imm = bool(out) and _is_imm_to_a(out[-1])
        if (prev_imm and isinstance(instr, asm_ast.Jump)
                and instr.target in labels):
            tail = _tail_after(instrs, labels[instr.target])
            if tail is not None:
                out.extend(tail)
                changed = True
                continue
        if prev_imm and isinstance(instr, asm_ast.Label):
            tail = _tail_after(instrs, i)
            if tail is not None:
                # The fall-through path gets its own copy. The label
                # and the tail after it stay for the other paths.
                out.extend(tail)
                changed = True
        out.append(instr)
    return out if changed else None


# ---------------------------------------------------------------------
# Constant compare
# ---------------------------------------------------------------------

def apply_carry_const_fold(prog: asm_ast.Program) -> asm_ast.Program:
    """Folds `LDA #k; CMP #c` to `SEC` / `CLC` when only C is read
    after it."""
    return _map_functions(prog, _fold_const_compares)


def _fold_const_compares(instrs):
    out = list(instrs)
    changed = False
    i = 0
    while i + 1 < len(out):
        a, b = out[i], out[i + 1]
        if (_is_imm_to_a(a) and _is_cmp_a_imm(b)
                and a_dead_at(out, i + 2) and flags_dead_at(out, i + 2)):
            k = a.src.value & 0xFF
            c = b.right.value & 0xFF
            out[i:i + 2] = [
                asm_ast.SetCarry() if k >= c else asm_ast.ClearCarry(),
            ]
            changed = True
            continue
        i += 1
    return out if changed else None


# ---------------------------------------------------------------------
# Caller-side capture
# ---------------------------------------------------------------------

def apply_carry_branch_fold(prog: asm_ast.Program) -> asm_ast.Program:
    """Branches on a captured carry flag directly, and drops a
    capture that's turned straight back into C."""
    return _map_functions(prog, _fold_captures)


def _is_capture(instrs, i) -> bool:
    """`LDA #0; ROL A` at `instrs[i]`."""
    if i + 1 >= len(instrs):
        return False
    a, b = instrs[i], instrs[i + 1]
    return (
        _is_imm_to_a(a) and a.src.value & 0xFF == 0
        and isinstance(b, asm_ast.RotateLeft) and b.dst == _A
    )


def _dead_after_branch(instrs, idx) -> bool:
    """A, N/Z, and C are all dead at `instrs[idx]`."""
    return (
        a_dead_at(instrs, idx) and flags_dead_at(instrs, idx)
        and c_dead_at(instrs, idx)
    )


def _fold_captures(instrs):
    out = list(instrs)
    changed = False
    i = 0
    while i + 2 < len(out):
        if not _is_capture(out, i):
            i += 1
            continue
        nxt = out[i + 2]
        if (isinstance(nxt, asm_ast.Branch)
                and isinstance(nxt.cond, (asm_ast.EQ, asm_ast.NE))):
            labels = {
                x.name: k for k, x in enumerate(out)
                if isinstance(x, asm_ast.Label)
            }
            tgt = labels.get(nxt.target)
            if (tgt is not None and _dead_after_branch(out, i + 3)
                    and _dead_after_branch(out, tgt)):
                cond = (asm_ast.CC() if isinstance(nxt.cond, asm_ast.EQ)
                        else asm_ast.CS())
                out[i:i + 3] = [asm_ast.Branch(cond=cond, target=nxt.target)]
                changed = True
                continue
        if (_is_cmp_a_imm(nxt) and nxt.right.value & 0xFF == 1
                and a_dead_at(out, i + 3) and flags_dead_at(out, i + 3)):
            del out[i:i + 3]
            changed = True
            continue
        i += 1
    return out if changed else None
