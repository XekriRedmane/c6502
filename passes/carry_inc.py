"""Carry-INC peephole: add a carry into a byte, or take a borrow out
of one, in place with BCC / INC or BCS / DEC.

The high byte of an add whose other operand's high byte is zero,
such as `t + (uint16_t)c` or `p + 53`, is lowered by `tac_to_asm` as

    Mov(M, A); Add(Imm(0), A); Mov(A, M)      LDA M; ADC #0; STA M

which adds the carry out of the low byte into M. When M is written
in place, hand-written 6502 code does the same thing with

    Branch(CC, skip); Inc(M); Label(skip)     BCC skip; INC M; skip:

A subtract's high byte, `LDA M; SBC #0; STA M`, takes 1 - C out of
M, and becomes `BCS skip; DEC M; skip:` the same way.

A wider add has one carry byte per byte above the lowest, each
reading the carry out of the one before. When all of them are in
place, the run becomes the carry chain `inc_peephole` uses for
`x += 1`:

    BCC done; INC M1; BNE done; INC M2; BNE done; INC M3; done:

since a byte that INC wraps to zero is the one case that carries
into the next. A subtract's run only has its top byte rewritten,
because a byte that DEC wraps to $FF can't be detected with one
branch.

That's 2 instructions instead of 3, and 4 bytes instead of 6 in zero
page (5 instead of 8 at an absolute address). It takes 3 cycles when
the branch is taken and 7 when it isn't (8 for an absolute INC or
DEC), against 8 (10 absolute) for the ADC form. The rewrite also
applies to `Reg(X)` and `Reg(Y)`, where `TXA; ADC #0; TAX` becomes
`BCC skip; INX`.

Move coalescing gives M the same byte on both sides where it can
(see "Carry bytes" in `passes/optimization_asm/coalescing.py`), and
an add to a global in place, `g += c`, has this shape already.

The two forms leave different values behind, so the rewrite needs
all of these to be dead after the store:

  * A. The ADC form leaves M's new value in A, and the INC form
    leaves A as it was before the LDA.
  * C. The ADC form sets C from the carry out of M, and the INC form
    leaves C as it came in. A following ADC that continues the chain
    into another byte reads C, so a wider add only has its top byte
    rewritten.
  * N, Z, and V. They come from the ADC in one form, and from the
    INC or the low byte's ADC in the other.

`asm_liveness.a_dead_at` and `flags_dead_at` (N and Z) check those
conditions along every path from the store, and `c_dead_at` and
`v_dead_at` check C and V the same way, each on its own. A `BEQ` after the `LDA`/`ORA`
that tests a 16-bit counter for zero reads N/Z, which the LDA set,
and doesn't make C or V live. A volatile M is left
alone, the same as in `inc_peephole`, since INC is one read-modify-
write access where the ADC form reads and writes separately.

The pass runs in the asm peephole fixed-point loop, after
`replace_pseudoregisters`, when operands are concrete and it's known
whether M is a zero-page or absolute byte that INC can address.
"""

from __future__ import annotations

import re

import asm_ast
from passes.asm_liveness import a_dead_at, c_dead_at, flags_dead_at, v_dead_at
from passes.inc_peephole import _is_inc_eligible_operand, _operands_equal

_REG_A = asm_ast.Reg(reg=asm_ast.A())
_LABEL_RE = re.compile(r"\.carry_inc@(\d+)$")


def apply_carry_inc_peephole(prog: asm_ast.Program) -> asm_ast.Program:
    """Rewrites every eligible run of `LDA M; ADC #0; STA M` in every
    Function to a `BCC` / `INC` chain, and every eligible `LDA M;
    SBC #0; STA M` to `BCS skip; DEC M; skip:`. Other top-levels pass
    through unchanged.

    Labels are numbered across the whole program, past any from an
    earlier run of the fixed-point loop. dasm scopes a `.` label to
    its SUBROUTINE, but the sim's assembler (`sim/assembler.py`)
    rejects the same label in two functions."""
    counter = 1 + max(
        (int(m.group(1))
         for tl in prog.top_level if isinstance(tl, asm_ast.Function)
         for ins in tl.instructions
         if isinstance(ins, asm_ast.Label)
         and (m := _LABEL_RE.match(ins.name))),
        default=-1,
    )
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            tl, counter = _rewrite_function(tl, counter)
        new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(
    fn: asm_ast.Function, counter: int,
) -> tuple[asm_ast.Function, int]:
    """Rewrites the runs in one function, numbering labels from
    `counter`. Returns the function and the next free number."""
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    changed = False
    i = 0
    while i < len(instrs):
        match = _match(instrs, i)
        if match is None:
            out.append(instrs[i])
            i += 1
            continue
        mems, is_add = match
        skip = f".carry_inc@{counter}"
        counter += 1
        if is_add:
            out.append(asm_ast.Branch(cond=asm_ast.CC(), target=skip))
            for k, mem in enumerate(mems):
                if k:
                    out.append(
                        asm_ast.Branch(cond=asm_ast.NE(), target=skip),
                    )
                out.append(asm_ast.Inc(dst=mem))
        else:
            out.append(asm_ast.Branch(cond=asm_ast.CS(), target=skip))
            out.append(asm_ast.Dec(dst=mems[0]))
        out.append(asm_ast.Label(name=skip))
        changed = True
        i += 3 * len(mems)
    if not changed:
        return fn, counter
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    ), counter


def _match(
    instrs: list[asm_ast.Type_instruction], i: int,
) -> tuple[list[asm_ast.Type_operand], bool] | None:
    """Returns `([M1, ...], is_add)` for the run of in-place carry
    bytes starting at `instrs[i]`, with A and every flag dead after
    the run. An add's run takes every carry byte in a row, and a
    subtract's run is one byte. Returns None when there's no such
    run."""
    first = _carry_byte(instrs, i)
    if first is None:
        return None
    mems = [first[0]]
    is_add = first[1]
    if is_add:
        while True:
            nxt = _carry_byte(instrs, i + 3 * len(mems))
            if nxt is None or not nxt[1]:
                break
            mems.append(nxt[0])
    end = i + 3 * len(mems)
    if not a_dead_at(instrs, end):
        return None
    if not flags_dead_at(instrs, end):
        return None
    if not c_dead_at(instrs, end):
        return None
    if not v_dead_at(instrs, end):
        return None
    return mems, is_add


def _carry_byte(
    instrs: list[asm_ast.Type_instruction], i: int,
) -> tuple[asm_ast.Type_operand, bool] | None:
    """Returns `(M, is_add)` when `instrs[i:i+3]` is `Mov(M, A);
    Add(Imm(0), A); Mov(A, M)` (is_add True) or the same with `Sub`
    (is_add False), with M INC-addressable and not volatile. Returns
    None otherwise."""
    if i + 3 > len(instrs):
        return None
    load, op, store = instrs[i:i + 3]
    if not (
        isinstance(load, asm_ast.Mov)
        and not load.is_volatile
        and load.dst == _REG_A
        and _is_inc_eligible_operand(load.src)
    ):
        return None
    if not (
        isinstance(op, (asm_ast.Add, asm_ast.Sub))
        and op.src == asm_ast.Imm(value=0)
        and op.dst == _REG_A
    ):
        return None
    if not (
        isinstance(store, asm_ast.Mov)
        and not store.is_volatile
        and store.src == _REG_A
        and _operands_equal(store.dst, load.src)
    ):
        return None
    return load.src, isinstance(op, asm_ast.Add)
