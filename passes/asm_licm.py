"""Asm-level loop-invariant store hoisting (LICM-lite).

Detects three shapes inside a natural loop and hoists them to the
loop's preheader when loop-invariant:

  1. `Mov(Imm, Data | ZP)` — single-instruction constant store.
  2. `Mov(Imm, Reg(A)); Mov(Reg(A), Data | ZP)` — pre-fusion
     lowered form of (1).
  3. `Mov(Data | ZP, Reg(A)); Mov(Reg(A), Data | ZP)` — read-
     then-write of a stable memory cell into another stable
     memory cell. The DPTR staging for a volatile pointer
     dereference (`LDA static_ptr; STA DPTR; LDA static_ptr+1;
     STA DPTR+1`) inside an outer loop is the motivating case:
     the static pointer doesn't change between iterations, so
     the staging belongs in the preheader.

The motivating case for shapes (1)/(2) is loop-invariant zp_abi
arg writes (e.g. `LDA #$01; STA __zpabi_callee_p0` inside a loop
that calls `callee` with a constant width arg) — without LICM the
constant is re-stored every iteration.

# What counts as a natural loop

A Jump or Branch whose target Label appears EARLIER in the
instruction stream is a back-edge. The loop's body is the
contiguous instruction range `[header_idx, back_edge_idx]`
inclusive, where `header_idx` is the index of the back-edge
target Label and `back_edge_idx` is the index of the Jump or
Branch.

A function may contain multiple back-edges; each is processed
independently. Nested loops are handled by running the pass to
fixed point — each inner-loop hoist may move an instruction to a
position where the outer loop can hoist it further.

# Eligibility

A candidate inside a loop body is eligible for hoisting when:

  * The dst (`Data` or `ZP`) is not written elsewhere in the
    loop body — only the to-be-hoisted Mov writes it.
  * For shape (3) — the read-then-write pair — additionally:
    the src (`Data` or `ZP`) must not be written anywhere in
    the loop body. Otherwise the loaded value could change
    between iterations.
  * No `Call` appears in the loop body. The conservative
    constraint avoids reasoning about whether a callee might
    clobber the dst: most importantly, calls to a `zp_abi`
    callee write to `__zpabi_<callee>_p<k>` slots, and the
    callee may further mutate them as locals — making it unsafe
    to assume the slot still holds our constant on the next
    iteration.
  * No instructions outside the loop body branch INTO the body
    past the header (the header is the single entry point).
    Verified by checking that the header Label is the only
    branch target inside `[header_idx, back_edge_idx]` referenced
    from outside.
  * For shape (3) and any Mov pair: neither half of the pair
    can carry `is_volatile=True`. A volatile Mov must remain at
    its source-order position so the access is observable on
    every iteration — hoisting one out of the loop would change
    the observable access count.

# Hoist mechanics

The eligible Mov (or LDA #c / STA M pair) is removed from the
body and inserted at the position immediately before
`header_idx`. The preheader is the run of instructions ending
just before the Label; if there's no convenient preheader
(e.g. the Label is the very first instruction), one is still a
valid insertion point — preceding instructions still flow
through it.

# Where to run

Best placement is after `replace_pseudoregisters_bare_exit`
(operands resolved to concrete Data / ZP), and before the
peephole fixed-point loop (so downstream peepholes see the
hoisted form). Currently invoked once before each peephole
fixed-point round in `compile.py`.
"""

from __future__ import annotations

import asm_ast


def apply_licm(prog: asm_ast.Program) -> asm_ast.Program:
    """Run loop-invariant constant-store hoisting on every Function
    top-level. Iterates to fixed point per function so nested loops
    can compose."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = list(fn.instructions)
    while True:
        new_instrs = _one_pass(instrs)
        if new_instrs is instrs:
            break
        instrs = new_instrs
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global, params=list(fn.params),
        instructions=instrs,
    )


def _one_pass(
    instrs: list[asm_ast.Type_instruction],
) -> list[asm_ast.Type_instruction]:
    """Find one hoistable Mov in the deepest available loop and
    perform the hoist. Returns a new instruction list, or `instrs`
    unchanged (same object identity) if no hoist fires."""
    label_idx = {
        inst.name: k for k, inst in enumerate(instrs)
        if isinstance(inst, asm_ast.Label)
    }
    # Collect back-edges: (header_idx, back_edge_idx, header_name).
    back_edges: list[tuple[int, int, str]] = []
    for k, inst in enumerate(instrs):
        target = _branch_or_jump_target(inst)
        if target is None:
            continue
        if target not in label_idx:
            continue
        header_idx = label_idx[target]
        if header_idx < k:
            back_edges.append((header_idx, k, target))
    # Process inner loops first (smallest range), so a hoist out of
    # an inner loop has a chance to also hoist out of any enclosing
    # outer loop on the next iteration.
    back_edges.sort(key=lambda t: t[1] - t[0])
    for header_idx, back_edge_idx, header_name in back_edges:
        hoisted = _try_hoist_one(
            instrs, header_idx, back_edge_idx, header_name, label_idx,
        )
        if hoisted is not None:
            return hoisted
    return instrs


def _try_hoist_one(
    instrs: list[asm_ast.Type_instruction],
    header_idx: int,
    back_edge_idx: int,
    header_name: str,
    label_idx: dict[str, int],
) -> list[asm_ast.Type_instruction] | None:
    """Try to find one hoistable Mov in `[header_idx, back_edge_idx]`.
    Returns a new instruction list with the hoist applied, or None
    if no hoist is possible."""
    body_range = range(header_idx, back_edge_idx + 1)
    # Single-entry check: every branch/jump into the body must
    # target `header_name`. A target Label INSIDE the body that's
    # referenced from outside the body would be a side entry.
    if not _is_single_entry(instrs, body_range, header_name, label_idx):
        return None
    # No Calls allowed in the body — conservative.
    if any(isinstance(instrs[k], asm_ast.Call) for k in body_range):
        return None
    # Tally write-counts to each Data/ZP key inside the body so we
    # can recognize "only this Mov writes M" (for the dst check) and
    # "no instruction writes this cell" (for the src check on the
    # read-then-write shape).
    write_count: dict[tuple, int] = {}
    for k in body_range:
        dst_key = _written_data_key(instrs[k])
        if dst_key is not None:
            write_count[dst_key] = write_count.get(dst_key, 0) + 1
    # Find a hoistable Mov.
    for k in body_range:
        candidate = _match_candidate(instrs, k, back_edge_idx)
        if candidate is None:
            continue
        span_len, dst_key, src_key = candidate
        if write_count.get(dst_key, 0) != 1:
            continue
        # For the read-then-write shape (src_key is not None), the
        # src cell must not be written anywhere in the body. Otherwise
        # the loaded value isn't loop-invariant.
        if src_key is not None and write_count.get(src_key, 0) != 0:
            continue
        # Hoist. The candidate occupies [k, k + span_len) — splice
        # those instructions out of the body and into the position
        # immediately before `header_idx`.
        hoisted_block = instrs[k : k + span_len]
        new_instrs = (
            instrs[:header_idx]
            + hoisted_block
            + instrs[header_idx:k]
            + instrs[k + span_len :]
        )
        return new_instrs
    return None


def _is_single_entry(
    instrs: list[asm_ast.Type_instruction],
    body_range: range,
    header_name: str,
    label_idx: dict[str, int],
) -> bool:
    """True iff the only branch target inside `body_range` that's
    referenced from OUTSIDE the body is `header_name`. (Targets
    referenced only from within the body are loop-internal control
    flow — fine.)"""
    body_label_names: set[str] = set()
    for k in body_range:
        inst = instrs[k]
        if isinstance(inst, asm_ast.Label):
            body_label_names.add(inst.name)
    for k, inst in enumerate(instrs):
        if k in body_range:
            continue
        target = _branch_or_jump_target(inst)
        if target is None:
            continue
        if target in body_label_names and target != header_name:
            return False
    return True


def _branch_or_jump_target(
    inst: asm_ast.Type_instruction,
) -> str | None:
    if isinstance(inst, (asm_ast.Jump, asm_ast.Branch)):
        return inst.target
    return None


def _written_data_key(inst: asm_ast.Type_instruction) -> tuple | None:
    """Return a key (kind, name|addr, offset) identifying the
    Data/ZP destination written by `inst`, or None if `inst` doesn't
    write to a Data/ZP cell."""
    dst = None
    if isinstance(inst, asm_ast.Mov):
        dst = inst.dst
    elif isinstance(inst, (asm_ast.Add, asm_ast.Sub, asm_ast.And,
                           asm_ast.Or, asm_ast.Inc, asm_ast.Dec)):
        dst = inst.dst
    elif isinstance(inst, asm_ast.Xor):
        dst = inst.dst
    elif isinstance(inst, (asm_ast.ArithmeticShiftLeft,
                           asm_ast.LogicalShiftRight,
                           asm_ast.RotateLeft,
                           asm_ast.RotateRight)):
        dst = inst.dst
    elif isinstance(inst, asm_ast.LoadAddress):
        dst = inst.dst
    if dst is None:
        return None
    return _operand_key(dst)


def _operand_key(op: asm_ast.Type_operand) -> tuple | None:
    if isinstance(op, asm_ast.Data):
        return ("data", op.name, op.offset)
    if isinstance(op, asm_ast.ZP):
        return ("zp", op.address, op.offset)
    return None


def _match_candidate(
    instrs: list[asm_ast.Type_instruction],
    start: int, last: int,
) -> tuple[int, tuple, tuple | None] | None:
    """If `instrs[start:]` is a hoistable store, return
    `(span_length, dst_key, src_key)` where `src_key` is the
    invariant-source key (for shape 3 — read-then-write) or None
    (for shapes 1 / 2 — Imm sources, automatically invariant).
    Otherwise return None.

    Three shapes:
      1. `Mov(Imm, Data | ZP)` — single instruction.
      2. `Mov(Imm, Reg(A)); Mov(Reg(A), Data | ZP)` — pair
         (the tac_to_asm-lowered form before any peephole fusion).
         The pair counts as one logical hoist; both instructions
         move together.
      3. `Mov(Data | ZP, Reg(A)); Mov(Reg(A), Data | ZP)` — pair
         that copies one stable memory cell into another. The
         src_key returned identifies the cell that must not be
         written anywhere in the body for the hoist to be sound.

    Volatile-flagged Movs (either half of a pair, or the
    single-instruction form) disqualify the candidate — the
    observable access must remain at its source-order position.
    """
    inst = instrs[start]
    if not isinstance(inst, asm_ast.Mov):
        return None
    if inst.is_volatile:
        return None
    # Shape 1: Mov(Imm, Data|ZP) directly.
    if isinstance(inst.src, asm_ast.Imm):
        key = _operand_key(inst.dst)
        if key is not None:
            return (1, key, None)
    # Shapes 2 and 3 both have the structure
    #   Mov(<src>, Reg(A))
    #   Mov(Reg(A), Data|ZP)
    # — the first instruction loads A from `<src>` and the second
    # writes A to a stable memory cell. The two pair shapes differ
    # only in what `<src>` is.
    if (isinstance(inst.dst, asm_ast.Reg)
        and isinstance(inst.dst.reg, asm_ast.A)
        and start + 1 <= last):
        nxt = instrs[start + 1]
        if (isinstance(nxt, asm_ast.Mov)
            and not nxt.is_volatile
            and isinstance(nxt.src, asm_ast.Reg)
            and isinstance(nxt.src.reg, asm_ast.A)):
            dst_key = _operand_key(nxt.dst)
            if dst_key is not None:
                # Shape 2: Mov(Imm, A); Mov(A, Data|ZP). No src
                # check needed (Imm is always invariant).
                if isinstance(inst.src, asm_ast.Imm):
                    return (2, dst_key, None)
                # Shape 3: Mov(Data|ZP, A); Mov(A, Data|ZP). The
                # caller must verify src isn't written in the body.
                src_key = _operand_key(inst.src)
                if src_key is not None:
                    return (2, dst_key, src_key)
    return None
