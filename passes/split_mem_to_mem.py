"""Split `Mov(mem, mem)` into `Mov(mem, Reg(A)); Mov(Reg(A), mem)`.

The asm IR allows a `Mov` atom whose src AND dst are both memory
operands. The 6502 has no `MOV mem, mem` opcode, so `asm_emit`
lowers the atom to two opcodes — `LDA src; STA dst` — using A as
the staging register. One IR atom, two emitted instructions.

That compound shape hides the implicit `LDA src` and the implicit
A-clobber from every instruction-stream peephole and from the
A-tracker dataflow passes. Concrete consequences logged in
`project_mem_to_mem_mov_hides_emit_lda`:

  * `redundant_load_elimination` sees a `Mov(M, dst_mem)` atom
    but won't rewrite its src even when its CFG-aware A-tracker
    has proven A already mirrors M (it only drops bare `LDA M`
    atoms).
  * `round_trip_load` needed bespoke patterns (the "Pattern B"
    extension) to recognize the hidden LDA inside the mem-to-mem.
  * The wider class of "load is redundant because some earlier
    computation already established A == M" patterns can't fire
    on mem-to-mem atoms at all.

This pass lowers the compound shape into the two atoms it actually
emits as, so every downstream peephole and tracker sees the
explicit LDA + STA pair. After splitting, the existing
`redundant_load_elimination` drops redundant LDAs, `asm_dead_store`
drops redundant STAs, etc. — no special-case handling required.

# What gets split

A `Mov(src, dst)` atom where BOTH src and dst are memory operands
in the sense of `asm_emit._is_memory_operand`'s set:

    Data | ZP | Frame | Stack | Indirect | IndirectY |
    IndirectZp | IndirectZpY

plus `IndexedData` (absolute,X / absolute,Y) — also a memory
operand from the emit perspective; `asm_emit` lowers
`Mov(IndexedData, mem)` as `LDA name,X; STA mem`, identical to
the mem-to-mem shape.

The split form is:

    Mov(src, Reg(A))     # LDA src
    Mov(Reg(A), dst)     # STA dst

The `is_volatile` bit propagates to both atoms — a volatile
mem-to-mem must perform a real load AND a real store, which the
split preserves.

# Self-Mov drop

`Mov(M, M)` (src == dst byte-identically) is semantically a
no-op: emit-time peephole `asm_emit:513` drops the entire atom
without producing any LDA or STA. This pass mirrors that — when
src == dst, drop the atom entirely rather than producing a
useless `LDA M; STA M` pair. SSA destruction emits self-Movs
when a Phi src and dst land at the same byte after coalescing;
without this drop, splitting would un-do the emit-time win.

# Volatile skip

`Mov(src_mem, dst_mem, is_volatile=True)` is NOT split. The
`is_volatile` bit on a Mov atom is conservative: it's set when
EITHER operand is a volatile-typed cell, but the bit itself
doesn't say which. Splitting would force both halves to inherit
`is_volatile=True` (because we can't tell which half is the
volatile one), which makes the LDA half look fully volatile to
`redundant_load_elimination` and blocks the optimization we
came here for. The existing `redundant_load._update_for_mov`
volatile-mem-to-mem branch handles this case correctly without
splitting — it adds the (presumed-non-volatile) src to state.a
so downstream explicit `LDA src` atoms still elide.

# Non-targets

Everything outside the (mem, mem) shape stays unchanged:

  * `Mov(Imm, mem)`, `Mov(ImmLabel*, mem)` — already a
    load-immediate-then-store pair from the emit's perspective,
    and `apply_remat` / `apply_memory_value_propagation` rewrites
    are easier to spot at the original shape.
  * `Mov(Reg, mem)` and `Mov(mem, Reg)` — single-atom loads /
    stores; nothing to split.
  * `Mov(Reg, Reg)` — register transfer; nothing to split.

# Where it runs

Inside `_peephole_fixedpoint`, near the top. Earlier passes in
the fixedpoint (e.g. `apply_memory_value_propagation`) can
synthesize new mem-to-mem atoms when they substitute a stage
cell's tracked value into an operand slot — putting the split
inside the fixedpoint ensures the next iteration catches and
lowers those too.
"""
from __future__ import annotations

import asm_ast


_MEM_KINDS = (
    asm_ast.Data, asm_ast.ZP, asm_ast.Frame, asm_ast.Stack,
    asm_ast.Indirect, asm_ast.IndirectY,
    asm_ast.IndirectZp, asm_ast.IndirectZpY,
    asm_ast.IndexedData,
)


def _is_mem(op) -> bool:
    return isinstance(op, _MEM_KINDS)


def apply_split_mem_to_mem(
    prog: asm_ast.Program,
) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    out: list[asm_ast.Type_instruction] = []
    changed = False
    for instr in fn.instructions:
        if (isinstance(instr, asm_ast.Mov)
                and not instr.is_volatile
                and _is_mem(instr.src)
                and _is_mem(instr.dst)):
            if instr.src == instr.dst:
                # Self-Mov: drop entirely (mirrors emit-time
                # peephole).
                changed = True
                continue
            a = asm_ast.Reg(reg=asm_ast.A())
            out.append(asm_ast.Mov(
                src=instr.src, dst=a,
                is_volatile=instr.is_volatile,
            ))
            out.append(asm_ast.Mov(
                src=a, dst=instr.dst,
                is_volatile=instr.is_volatile,
            ))
            changed = True
            continue
        out.append(instr)
    if not changed:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )
