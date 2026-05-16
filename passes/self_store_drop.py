"""Drop adjacent `LDA M; STA M` self-store, when the surrounding
state allows.

# Pattern

Two consecutive instructions:

    Mov(<mem M>, Reg(A))    # LDA M  — A = M, flags = bit7(M)/Zero(M)
    Mov(Reg(A), <mem M>)    # STA M  — M = A = M (no change)

The STA writes the same byte M already held, so the pair has no
effect on M. The LDA sets A and flags; the STA leaves both
untouched.

# What can be dropped

The STA is unconditionally redundant: M's value is unchanged, and
STA doesn't write A or flags. Drop it.

The LDA is dropped only when A AND flags are dead afterward. That
check is exactly what `dead_a_arith_elimination` already
performs — so we leave the LDA in place and let that pass collect
it in a subsequent fixed-point iteration.

# Why this isn't covered by other passes

  * `redundant_store_elimination` looks for memory-to-memory
    copies that re-establish a known equivalence. The first
    `LDA M; STA M` in a block establishes `known[M] = M`, so the
    pair stays — only a SUBSEQUENT identical pair would drop.
  * `redundant_load_elimination` drops `LDA M` after `STA M`
    when A already mirrors M, but here the LDA comes FIRST.
  * `asm_dead_store` would drop the STA only if M is dead
    afterward — but M IS live (consumed downstream).

The self-store case is its own pattern: STA writes the SAME
value back, so it's redundant regardless of M's downstream
liveness.

# Motivating case

c99_to_tac lowers `(uint16_t)hi << 8 | (uint16_t)lo` (the
two-byte-into-pointer composition) with byte-level operations
on each half. After the byte-aligned shift fold turns the
`<< 8`'s low byte into `Imm(0)` and the OR's per-byte high half
becomes `hi | 0 = hi`, the IR shape for the high byte is:

    LDA hi       ; A = hi
    Or(Imm(0), A) ; <dropped by const_arith_fold>
    STA hi       ; self-store
    (later: consume hi)

This pass drops the STA; downstream dead_a_arith may then drop
the LDA if A is dead.

# Where to run

Inside the asm-peephole fixed-point loop, after the other
arithmetic-simplifying passes (mem_const_prop / const_arith_fold)
have stripped intervening identity ops."""

from __future__ import annotations

import asm_ast


def apply_self_store_drop(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _is_reg_a(op) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _is_stable_mem(op) -> bool:
    return isinstance(op, (asm_ast.ZP, asm_ast.Data))


def _operands_equal(a, b) -> bool:
    if isinstance(a, asm_ast.Data) and isinstance(b, asm_ast.Data):
        return a.name == b.name and a.offset == b.offset
    if isinstance(a, asm_ast.ZP) and isinstance(b, asm_ast.ZP):
        return a.address == b.address and a.offset == b.offset
    return False


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    i = 0
    while i < len(instrs):
        # Drop trivial self-Movs `Mov(X, X)` (structurally identical
        # src and dst). These arise from SSA destruction emitting
        # parallel-copy moves where source and destination coalesce
        # to the same color. `asm_emit` already drops them at codegen
        # time, but dropping them at IR level lets downstream passes
        # (notably `asm_dead_store`) avoid mistaking the self-Mov's
        # src for a live read of its dst.
        cur = instrs[i]
        if (isinstance(cur, asm_ast.Mov)
                and not cur.is_volatile
                and _is_stable_mem(cur.src)
                and _is_stable_mem(cur.dst)
                and _operands_equal(cur.src, cur.dst)):
            i += 1
            continue
        if i + 1 < len(instrs):
            a, b = instrs[i], instrs[i + 1]
            # Neither Mov can be volatile — a volatile LDA must
            # re-read, and a volatile STA must always write.
            if (isinstance(a, asm_ast.Mov)
                    and not a.is_volatile
                    and _is_reg_a(a.dst)
                    and _is_stable_mem(a.src)
                    and isinstance(b, asm_ast.Mov)
                    and not b.is_volatile
                    and _is_reg_a(b.src)
                    and _is_stable_mem(b.dst)
                    and _operands_equal(a.src, b.dst)):
                # Keep the LDA; drop the STA (self-store).
                out.append(a)
                i += 2
                continue
        out.append(instrs[i])
        i += 1
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )
