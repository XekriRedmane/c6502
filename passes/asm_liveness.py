"""Shared `Reg(A)` liveness predicate for asm-level peepholes.

Three peepholes (`direct_index_load`, the SSA-time
`backward_copy_propagation`, the CPX/CPY rewriter) need to ask
"is the current value of `Reg(A)` observable past this point?".
The answer drives whether transferring through A (`LDA M; TAX`,
`LDA M; STA N`, `TXA; CMP K`) can be collapsed: if A's post-
sequence value is never read before being overwritten, the
intermediate-A version is observably equivalent to a direct
version that bypasses A.

# `a_dead_at(instrs, idx)` — CFG-wide forward search

Forward DFS from `idx`. Each instruction either:

  * Reads `Reg(A)` — A is live on this path → return False.
  * Kills `Reg(A)` (writes A without reading it first) — A is
    dead from this point on this path; terminate the path.
  * `Ret` / `Return` — A is dead iff `save_a=False` (a
    `save_a=True` epilogue does `PHA` first, which is a read);
    if False, terminate the path; if True, return False.
  * `Jump(target)` — follow target only.
  * `Branch(_, target)` — follow both fall-through and target.
  * `Label` — pass through (a label is a marker; the walk
    continues on the same path).
  * End of instructions — terminate the path (no observers
    possible).

A's dead at `idx` iff EVERY path terminates without observing a
read. Visited indices are tracked to keep the walk linear in
program size.

The CFG walk catches the common loop-tail shape

    TXA ; CMP K ; Branch cond, t   ; (idx points past Branch)
    ... fall-through path starts with TYA / LDA / similar (kills A)
    t: LDA M / TYA / similar       (kills A on the taken path)

which a within-block walk that bails at the Branch misses.

# Phi handling

Within asm-level SSA, `Phi` nodes may take `Reg(A)` as a source
(rare but possible during regalloc transitions). The conservative
treatment is "Phi reads its sources" — a Phi with `Reg(A)` source
counts as a read.

# Soundness notes

- `LoadAddress` is treated as a kill: its expansion in
  `asm_to_asm2` always writes A through immediates, never
  preserving the prior A value.
- `Call` is treated as both a read AND a kill of A iff the
  callee's reg-arg list (`Call.reg_args`) names A: the JSR reads A
  as the incoming arg before the callee clobbers it. Without "A"
  in `reg_args` it's only a kill (callees clobber A on return —
  1-byte returns leave the result there; HARGS-returning calls
  leave A unspecified). The read-before-kill check in
  `_path_dead_from` runs `reads_a` first, so when a Call reads A
  the path correctly returns "A live" before we'd otherwise
  declare it dead from the kill.
- `Pop(dst=A)` is a kill — fresh value from the hw stack.
- The ALU instructions (Add/Sub/And/Or/Xor) RMW A: they read A
  before writing it, so they're a read, not a kill.
- Shift/rotate on A (`ASL A` etc.) likewise RMW A.
"""

from __future__ import annotations

import asm_ast


def is_reg_a(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def is_reg_x(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.X)


def is_reg_y(op: asm_ast.Type_operand) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.Y)


def a_dead_at(
    instrs: list[asm_ast.Type_instruction], idx: int,
) -> bool:
    """True iff `Reg(A)`'s current value is dead at `instrs[idx]` —
    every forward path through the instruction stream encounters a
    kill of A before any read of A. Uses a CFG-wide forward DFS to
    handle inter-block paths."""
    return _reg_dead_at(instrs, idx, "A")


def x_dead_at(
    instrs: list[asm_ast.Type_instruction], idx: int,
) -> bool:
    """True iff `Reg(X)`'s current value is dead at `instrs[idx]`.
    Same CFG-walk machinery as `a_dead_at`; uses X-specific
    reads/kills predicates. The soft-stack epilogue scratches X (low
    byte of the reloaded caller FP routes through X) so Ret/Return
    is treated as a kill regardless of `save_a`. Tail-call Jumps
    read X iff the callee names X in `reg_args`."""
    return _reg_dead_at(instrs, idx, "X")


def y_dead_at(
    instrs: list[asm_ast.Type_instruction], idx: int,
) -> bool:
    """True iff `Reg(Y)`'s current value is dead at `instrs[idx]`.
    Same as `x_dead_at` for Y. The soft-stack epilogue scratches Y
    (the indirect-Y offset for the (FP),Y reload), so Ret/Return is
    a kill of Y."""
    return _reg_dead_at(instrs, idx, "Y")


def _reg_dead_at(
    instrs: list[asm_ast.Type_instruction], idx: int, reg: str,
) -> bool:
    label_to_index = _build_label_map(instrs)
    visited: set[int] = set()
    stack: list[int] = [idx]
    while stack:
        j = stack.pop()
        if j in visited:
            continue
        visited.add(j)
        if not _path_dead_from(instrs, j, label_to_index, stack, reg):
            return False
    return True


def _path_dead_from(
    instrs: list[asm_ast.Type_instruction],
    start: int,
    label_to_index: dict[str, int],
    stack: list[int],
    reg: str,
) -> bool:
    """Walk forward from `start` along the straight-line successor
    chain. Returns False if this path observes a read of `reg`
    before a kill. Returns True if the path terminates without
    observing one (kill, Ret-with-save_a=False for A / Ret for X/Y,
    end of instructions). At branches / jumps, push successors onto
    `stack` and return True (the dispatching loop checks each
    successor)."""
    idx = start
    while idx < len(instrs):
        instr = instrs[idx]
        if isinstance(instr, (asm_ast.Ret, asm_ast.Return)):
            if reg == "A":
                return not instr.save_a
            # X and Y are scratched by the soft-stack epilogue (low
            # byte of reloaded caller FP through X, indirect-Y
            # offset for the (FP),Y reload). For a bare Return the
            # epilogue is empty, but X/Y are still callee-clobberable
            # by convention — treat as kill.
            return True
        if isinstance(instr, asm_ast.Jump):
            # Tail-call Jumps (rewritten from `Call(reg_args); Return`
            # by the tail-call peephole) read `reg` iff the target
            # callee took `reg` as a reg-attributed arg. Detect via
            # `reg_args` rather than "target not in label_to_index"
            # so we don't also flag long-branch trampolines (which
            # target an in-function `.skip` label — `label_to_index`
            # would still resolve them).
            if reg in instr.reg_args:
                return False
            tgt = label_to_index.get(instr.target)
            if tgt is not None:
                stack.append(tgt)
            return True
        if isinstance(instr, asm_ast.Branch):
            if idx + 1 < len(instrs):
                stack.append(idx + 1)
            tgt = label_to_index.get(instr.target)
            if tgt is not None:
                stack.append(tgt)
            return True
        if isinstance(instr, asm_ast.Label):
            idx += 1
            continue
        if _reads_reg(instr, reg):
            return False
        if _kills_reg(instr, reg):
            return True
        idx += 1
    return True


def _reads_reg(instr: asm_ast.Type_instruction, reg: str) -> bool:
    if reg == "A":
        return reads_a(instr)
    if reg == "X":
        return reads_x(instr)
    return reads_y(instr)


def _kills_reg(instr: asm_ast.Type_instruction, reg: str) -> bool:
    if reg == "A":
        return kills_a(instr)
    if reg == "X":
        return kills_x(instr)
    return kills_y(instr)


def _build_label_map(
    instrs: list[asm_ast.Type_instruction],
) -> dict[str, int]:
    out: dict[str, int] = {}
    for i, ins in enumerate(instrs):
        if isinstance(ins, asm_ast.Label):
            out[ins.name] = i
    return out


def reads_a(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` reads `Reg(A)` (uses it as a source or
    read-modify-writes via `dst=A`)."""
    if isinstance(instr, asm_ast.Mov):
        return is_reg_a(instr.src)
    if isinstance(instr, asm_ast.Push):
        return is_reg_a(instr.src)
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
    )):
        return is_reg_a(instr.src) or is_reg_a(instr.dst)
    if isinstance(instr, asm_ast.Xor):
        return (
            is_reg_a(instr.src1) or is_reg_a(instr.src2)
            or is_reg_a(instr.dst)
        )
    if isinstance(instr, asm_ast.Compare):
        return is_reg_a(instr.left) or is_reg_a(instr.right)
    if isinstance(instr, (
        asm_ast.Inc, asm_ast.Dec,
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        return is_reg_a(instr.dst)
    if isinstance(instr, asm_ast.Phi):
        return any(is_reg_a(a.source) for a in instr.args)
    if isinstance(instr, asm_ast.Call):
        return "A" in instr.reg_args
    if isinstance(instr, asm_ast.Jump):
        # Tail-call Jump (target is a function, not an in-function
        # label) inherits the original Call's `reg_args`. In-function
        # gotos have `reg_args=[]` and don't show up as A-readers.
        return "A" in instr.reg_args
    return False


def kills_a(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` writes `Reg(A)` without reading it first."""
    if isinstance(instr, asm_ast.Mov):
        if is_reg_a(instr.dst) and not is_reg_a(instr.src):
            return True
    if isinstance(instr, asm_ast.Pop):
        if is_reg_a(instr.dst):
            return True
    if isinstance(instr, asm_ast.Call):
        return True
    if isinstance(instr, asm_ast.LoadAddress):
        return True
    return False


def _operand_indexes_with(op: asm_ast.Type_operand, reg_cls) -> bool:
    """True iff `op` references the named index register (X or Y)
    as part of its addressing mode."""
    if isinstance(op, asm_ast.IndexedData):
        return isinstance(op.index, reg_cls)
    if reg_cls is asm_ast.Y and isinstance(
        op, (asm_ast.IndirectY, asm_ast.IndirectZpY),
    ):
        return True
    return False


def _operand_setup_clobbers_y(op: asm_ast.Type_operand) -> bool:
    """True iff `op`'s emit-time addressing-mode setup writes Y.
    `Indirect(off)` and `IndirectZp(addr, off)` emit `LDY #off`
    before the underlying op, so any instruction whose src or dst
    is one of these clobbers Y as a side effect."""
    return isinstance(op, (asm_ast.Indirect, asm_ast.IndirectZp))


def reads_x(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` reads `Reg(X)` (uses it as a source, an
    index for `IndexedData(_, index=X)`, or RMW's it via INX/DEX)."""
    if isinstance(instr, asm_ast.Mov):
        if is_reg_x(instr.src):
            return True
        if _operand_indexes_with(instr.src, asm_ast.X):
            return True
        if _operand_indexes_with(instr.dst, asm_ast.X):
            return True
        return False
    if isinstance(instr, asm_ast.Push):
        return is_reg_x(instr.src)
    if isinstance(instr, asm_ast.Compare):
        if is_reg_x(instr.left) or is_reg_x(instr.right):
            return True
        return (
            _operand_indexes_with(instr.left, asm_ast.X)
            or _operand_indexes_with(instr.right, asm_ast.X)
        )
    if isinstance(instr, (asm_ast.Inc, asm_ast.Dec)):
        if is_reg_x(instr.dst):
            return True
        return _operand_indexes_with(instr.dst, asm_ast.X)
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
    )):
        return (
            _operand_indexes_with(instr.src, asm_ast.X)
            or _operand_indexes_with(instr.dst, asm_ast.X)
        )
    if isinstance(instr, asm_ast.Xor):
        return any(
            _operand_indexes_with(o, asm_ast.X)
            for o in (instr.src1, instr.src2, instr.dst)
        )
    if isinstance(instr, (
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        return _operand_indexes_with(instr.dst, asm_ast.X)
    if isinstance(instr, asm_ast.BitTest):
        return _operand_indexes_with(instr.operand, asm_ast.X)
    if isinstance(instr, asm_ast.Phi):
        return any(is_reg_x(a.source) for a in instr.args)
    if isinstance(instr, (asm_ast.Call, asm_ast.Jump)):
        return "X" in instr.reg_args
    return False


def kills_x(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` writes `Reg(X)` without reading it first."""
    if isinstance(instr, asm_ast.Mov):
        if isinstance(instr.dst, asm_ast.Reg) and isinstance(
            instr.dst.reg, asm_ast.X,
        ):
            return not is_reg_x(instr.src)
        return False
    if isinstance(instr, asm_ast.Call):
        return True
    return False


def reads_y(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` reads `Reg(Y)` — same shapes as `reads_x`
    plus the `IndirectY` / `IndirectZpY` operand forms that consume
    Y as the indirect-indexed offset."""
    if isinstance(instr, asm_ast.Mov):
        if is_reg_y(instr.src):
            return True
        if _operand_indexes_with(instr.src, asm_ast.Y):
            return True
        if _operand_indexes_with(instr.dst, asm_ast.Y):
            return True
        return False
    if isinstance(instr, asm_ast.Push):
        return is_reg_y(instr.src)
    if isinstance(instr, asm_ast.Compare):
        if is_reg_y(instr.left) or is_reg_y(instr.right):
            return True
        return (
            _operand_indexes_with(instr.left, asm_ast.Y)
            or _operand_indexes_with(instr.right, asm_ast.Y)
        )
    if isinstance(instr, (asm_ast.Inc, asm_ast.Dec)):
        if is_reg_y(instr.dst):
            return True
        return _operand_indexes_with(instr.dst, asm_ast.Y)
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
    )):
        return (
            _operand_indexes_with(instr.src, asm_ast.Y)
            or _operand_indexes_with(instr.dst, asm_ast.Y)
        )
    if isinstance(instr, asm_ast.Xor):
        return any(
            _operand_indexes_with(o, asm_ast.Y)
            for o in (instr.src1, instr.src2, instr.dst)
        )
    if isinstance(instr, (
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        return _operand_indexes_with(instr.dst, asm_ast.Y)
    if isinstance(instr, asm_ast.BitTest):
        return _operand_indexes_with(instr.operand, asm_ast.Y)
    if isinstance(instr, asm_ast.Phi):
        return any(is_reg_y(a.source) for a in instr.args)
    if isinstance(instr, (asm_ast.Call, asm_ast.Jump)):
        return "Y" in instr.reg_args
    return False


def kills_y(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` writes `Reg(Y)` without reading it first.
    Includes the implicit `LDY #off` setup that `Indirect(off)` and
    `IndirectZp(addr, off)` operands emit before the underlying op."""
    if isinstance(instr, asm_ast.Mov):
        if isinstance(instr.dst, asm_ast.Reg) and isinstance(
            instr.dst.reg, asm_ast.Y,
        ):
            return not is_reg_y(instr.src)
        # An indirect-Y addressing-mode setup with a non-zero offset
        # clobbers Y via the implicit `LDY #off` the emitter inserts.
        if (
            _operand_setup_clobbers_y(instr.src)
            or _operand_setup_clobbers_y(instr.dst)
        ):
            return True
        return False
    if isinstance(instr, asm_ast.Call):
        return True
    if isinstance(instr, asm_ast.LoadAddress):
        # The dst-write step may insert `LDY #1` for the high-byte
        # store if the dst is indirect-Y. Conservative: treat as a
        # Y kill (rare in practice — LoadAddress dst is usually a
        # Frame slot whose offset fits the same indirect-Y setup,
        # but we don't model that precisely).
        return _operand_setup_clobbers_y(instr.dst)
    return False


def flags_dead_at(
    instrs: list[asm_ast.Type_instruction], idx: int,
) -> bool:
    """True iff the N/Z flags' values at position `idx` are dead —
    no `Branch` reads them before some flag-setter overwrites them.
    Within-block walk: a `Jump` / `Ret` / `Return` / `Label`
    terminates safely (the N/Z status of subsequent paths is each
    block's responsibility; if the next block's first instruction
    is a Branch, the soundness of declaring flags dead here depends
    on c6502's emission convention always setting N/Z before any
    Branch the lowering needs to read — which it does).

    Only tracks N/Z. Callers that need to know whether C is also
    dead (e.g. dropping an Add/Sub whose C output threads into a
    later SBC chain) must use `all_flags_dead_at`."""
    while idx < len(instrs):
        instr = instrs[idx]
        if isinstance(instr, asm_ast.Branch):
            return False
        if isinstance(instr, (
            asm_ast.Jump, asm_ast.Ret, asm_ast.Return, asm_ast.Label,
        )):
            return True
        if sets_flags(instr):
            return True
        idx += 1
    return True


def all_flags_dead_at(
    instrs: list[asm_ast.Type_instruction], idx: int,
) -> bool:
    """True iff every flag (N/Z/C/V) at position `idx` is dead —
    no instruction reads any of them before they're overwritten.

    Stricter than `flags_dead_at`: also detects subsequent Add/Sub
    (which read C from the prior ADC/SBC) and RotateLeft/Right
    (which read C as the shift-in bit) as flag readers. These are
    common in multi-byte arithmetic chains where the codegen does
    NOT seed CLC/SEC between consecutive ADCs/SBCs because they're
    deliberately threading carry.

    Use this when dropping or rewriting an instruction whose C/V
    side-effects could be observed downstream."""
    while idx < len(instrs):
        instr = instrs[idx]
        if isinstance(instr, asm_ast.Branch):
            return False
        if isinstance(instr, (
            asm_ast.Add, asm_ast.Sub,
            asm_ast.RotateLeft, asm_ast.RotateRight,
        )):
            # Reads C before (possibly) overwriting it.
            return False
        if isinstance(instr, (
            asm_ast.Jump, asm_ast.Ret, asm_ast.Return, asm_ast.Label,
        )):
            return True
        if _overwrites_all_flags(instr):
            return True
        idx += 1
    return True


def _overwrites_all_flags(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` overwrites N, Z, AND C (V is harder to fully
    track but the common signed-compare V-correction pattern emits
    its own flag-setter right after the EOR, so we conservatively
    require N/Z/C to all be overwritten here).

    The N/Z-only setters from `sets_flags` aren't sufficient — they
    leave C unchanged, so a downstream ADC/SBC/BCC could still read
    the previous C. Strict overwriters of C:
      * `SetCarry` / `ClearCarry` — explicit C writes.
      * `Compare` (CMP/CPX/CPY) — sets N/Z/C from subtraction.
      * `ArithmeticShiftLeft` / `LogicalShiftRight` — set C from
        the shifted-out bit.
      * `Call` — callee defines all flags."""
    if isinstance(instr, (
        asm_ast.SetCarry, asm_ast.ClearCarry,
        asm_ast.Compare,
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.Call,
    )):
        return True
    return False


def sets_flags(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr`'s 6502 lowering fully overwrites the N/Z
    flags."""
    if isinstance(instr, asm_ast.Mov):
        # Mov(_, Reg(A|X|Y)) → LD* sets flags.
        if isinstance(instr.dst, asm_ast.Reg):
            return True
        # Mov(Reg(A), memory) → STA only — no flag change.
        if is_reg_a(instr.src):
            return False
        # Mov(non-A, memory) emits LDA src; STA dst — LDA sets flags.
        return True
    if isinstance(instr, (
        asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
        asm_ast.Xor,
    )):
        return True
    if isinstance(instr, (
        asm_ast.Inc, asm_ast.Dec,
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        return True
    if isinstance(instr, asm_ast.Compare):
        return True
    if isinstance(instr, asm_ast.Pop):
        return True
    if isinstance(instr, asm_ast.LoadAddress):
        return True
    if isinstance(instr, asm_ast.Call):
        # Callee-defined flag state — assume clobbered.
        return True
    return False
