"""DEY / DEX folding for `--counter` where the counter is pinned
to X or Y (including volatile pins).

When a c99 local with `__attribute__((reg("X")))` or `reg("Y")`
gets `--counter` (or `counter -= 1; counter != 0`), `tac_to_asm`
lowers the read-modify-write through A:

    Mov(Reg(Y), Reg(A))       ; TYA (volatile if y is volatile-pinned)
    SetCarry
    Sub(Imm(1), Reg(A))       ; A = Y - 1
    Mov(Reg(A), <tmp>)        ; STA tmp
    Mov(<tmp>, Reg(Y))        ; LDY tmp (writes y)
    Branch(NE|EQ, …)

Six instructions for what is semantically a single-byte register
decrement. The 6502's DEY (and DEX) instructions perform
`Y = Y - 1; flag <- N/Z based on result` in 1 byte / 2 cycles —
exactly what we want.

The fold:

    Dec(Reg(Y))
    Branch(NE|EQ, …)

Soundness for volatile:
  * `volatile uint8_t y __attribute__((reg("Y")))` pins y to the
    Y register; the register IS the storage. A volatile access of
    y is a read-or-write of Y, which any DEY instruction performs.
  * Original lowering: one volatile read (TYA) and one volatile
    write (the LDY that brings the post-decrement value back into
    Y). DEY does both in one instruction — one R-M-W of Y, which
    the C abstract machine sees as the same single read + single
    write that `--y` requires per C99 §6.5.2.4 (postfix decrement).

Soundness for A:
  * Original leaves A holding `y - 1` post-sequence. DEY leaves A
    unchanged from its prior value. The fold requires A dead at
    BOTH of the Branch's successors (taken AND fall-through); a
    conservative one-instruction-look ahead at each successor's
    head is enough for the typical inner-loop shape (every loop
    iteration starts with a TYA / TXA / Mov-into-A clobber). When
    we can't prove A dead the fold doesn't fire.

Soundness for the spill temp:
  * The STA + LDY pair routes the new value through a memory
    cell. Dropping both halves drops the cell's write, which
    means downstream readers (if any) see whatever was there
    before. We require the cell to be a known dead-after-temp:
    the simplest sufficient gate is "the next non-Branch
    instruction overwrites the cell, or the cell isn't read
    again on either successor path". For the snd_delay_up inner
    loop the cell `__local_..._0` is used only here, so the
    write is dead and the fold is safe.

Where to run: in the asm-peephole fixed-point loop, after
`replace_pseudoregisters` (so Pseudos are resolved to Reg / Data /
ZP). The fold is monotone-shrinking (6 atoms → 2), so it's safe in
the fixed point.
"""
from __future__ import annotations

import asm_ast


def apply_dec_register_peephole(prog: asm_ast.Program) -> asm_ast.Program:
    """Walk every Function and collapse `Mov(Reg(R), A); SetCarry;
    Sub(Imm(1), A); Mov(A, tmp); Mov(tmp, Reg(R)); Branch(NE|EQ, _)`
    sequences into `Dec(Reg(R)); Branch(NE|EQ, _)` when R is X / Y
    and the soundness gates pass."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    instrs = fn.instructions
    out: list[asm_ast.Type_instruction] = []
    label_to_index = _build_label_index(instrs)
    i = 0
    while i < len(instrs):
        match = _match_dec_register_pattern(instrs, i, label_to_index)
        if match is not None:
            reg_kind, branch = match
            out.append(asm_ast.Dec(dst=asm_ast.Reg(reg=reg_kind())))
            out.append(branch)
            i += 6
            continue
        out.append(instrs[i])
        i += 1
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _match_dec_register_pattern(
    instrs: list[asm_ast.Type_instruction],
    i: int,
    label_to_index: dict[str, int],
):
    """Return `(reg_kind_class, branch_instr)` when the 6-instr
    decrement pattern matches at index `i`, else None.

    reg_kind_class is `asm_ast.X` or `asm_ast.Y` (the constructor
    for the register kind, so the caller can build a fresh Reg).
    branch_instr is the trailing Branch atom verbatim."""
    if i + 5 >= len(instrs):
        return None
    a, b, c, d, e, f = (
        instrs[i], instrs[i + 1], instrs[i + 2],
        instrs[i + 3], instrs[i + 4], instrs[i + 5],
    )
    # a: Mov(Reg(R), Reg(A))
    if not isinstance(a, asm_ast.Mov):
        return None
    if not (
        isinstance(a.src, asm_ast.Reg)
        and isinstance(a.src.reg, (asm_ast.X, asm_ast.Y))
        and isinstance(a.dst, asm_ast.Reg)
        and isinstance(a.dst.reg, asm_ast.A)
    ):
        return None
    reg_kind = type(a.src.reg)
    # b: SetCarry()
    if not isinstance(b, asm_ast.SetCarry):
        return None
    # c: Sub(Imm(1), Reg(A))
    if not isinstance(c, asm_ast.Sub):
        return None
    if not (
        isinstance(c.src, asm_ast.Imm) and c.src.value == 1
        and isinstance(c.dst, asm_ast.Reg)
        and isinstance(c.dst.reg, asm_ast.A)
    ):
        return None
    # d: Mov(Reg(A), tmp)
    if not isinstance(d, asm_ast.Mov):
        return None
    if not (
        isinstance(d.src, asm_ast.Reg)
        and isinstance(d.src.reg, asm_ast.A)
    ):
        return None
    tmp = d.dst
    # e: Mov(tmp, Reg(R)) — same tmp, same R as `a`.
    if not isinstance(e, asm_ast.Mov):
        return None
    if e.src != tmp:
        return None
    if not (
        isinstance(e.dst, asm_ast.Reg)
        and type(e.dst.reg) is reg_kind
    ):
        return None
    # f: Branch(NE|EQ, _)
    if not isinstance(f, asm_ast.Branch):
        return None
    if not isinstance(f.cond, (asm_ast.NE, asm_ast.EQ)):
        return None
    # Soundness gates.
    # (1) A is dead at both of f's successors (the branch target's
    #     head and the fall-through head after the Branch). The
    #     simplest sufficient check: the first non-trivial
    #     instruction on each path is a write to A (any Mov whose
    #     dst is Reg(A), or an arithmetic op with dst=Reg(A), or
    #     a Call which clobbers everything).
    if not _a_dead_at_successors(instrs, i + 5, label_to_index):
        return None
    # (2) The spill temp is dead after the read at `e` — either
    #     overwritten before the next read on every path, or never
    #     read again. Conservatively gate on the simplest case:
    #     no read of `tmp` after `e` in the rest of the function.
    if _operand_read_after(tmp, instrs, i + 6):
        return None
    return reg_kind, f


def _a_dead_at_successors(
    instrs: list[asm_ast.Type_instruction],
    branch_idx: int,
    label_to_index: dict[str, int],
) -> bool:
    """True iff A is dead at both successors of `instrs[branch_idx]`.

    Heuristic: A is dead if the first meaningful instruction on
    each successor path is an A-write OR a Call (which clobbers
    everything). Skips Labels and self-Mov no-ops on the way. If
    no first instruction (end of function), A is dead at exit by
    convention (the soft-stack frame teardown clobbers it
    anyway)."""
    branch = instrs[branch_idx]
    assert isinstance(branch, asm_ast.Branch)
    # Fall-through start.
    if not _a_dead_at(instrs, branch_idx + 1, label_to_index):
        return False
    # Branch-taken start.
    tgt = label_to_index.get(branch.target)
    if tgt is None:
        # External (tail-call-style); the callee owns A.
        return True
    if not _a_dead_at(instrs, tgt + 1, label_to_index):
        return False
    return True


def _a_dead_at(
    instrs: list[asm_ast.Type_instruction],
    start: int,
    label_to_index: dict[str, int],
) -> bool:
    """Walk forward from `start` until A's state is decided.
    A is dead at start iff the first observable operation clobbers
    A before reading it."""
    i = start
    seen: set[int] = set()
    while i < len(instrs):
        if i in seen:
            return False  # loop without resolution — be safe
        seen.add(i)
        instr = instrs[i]
        if isinstance(instr, asm_ast.Label):
            i += 1
            continue
        # Self-Mov is transparent.
        if (
            isinstance(instr, asm_ast.Mov)
            and instr.src == instr.dst
        ):
            i += 1
            continue
        if _reads_reg_a(instr):
            return False
        if _writes_reg_a(instr):
            return True
        # An unconditional Jump moves to its target.
        if isinstance(instr, asm_ast.Jump):
            tgt = label_to_index.get(instr.target)
            if tgt is None:
                # External — callee owns A.
                return True
            i = tgt + 1
            continue
        # A conditional Branch needs both paths checked; defer
        # to the recursive `_a_dead_at_successors` if needed.
        # Here we just bail conservatively.
        if isinstance(instr, asm_ast.Branch):
            return False
        # Call / Return clobber everything (A definitely dies).
        if isinstance(instr, (
            asm_ast.Call, asm_ast.Ret, asm_ast.Return,
        )):
            return True
        # Other instructions: not A-touching, continue.
        i += 1
    # Fell off the end of the function: A dies at exit.
    return True


def _reads_reg_a(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` reads Reg(A)."""
    def is_a(op):
        return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)
    match instr:
        case asm_ast.Mov(src=s):
            if is_a(s):
                return True
        case asm_ast.Add(src=s, dst=d) | asm_ast.Sub(src=s, dst=d):
            if is_a(s) or is_a(d):
                return True
        case asm_ast.And(src1=a, src2=b) | asm_ast.Or(src1=a, src2=b) | asm_ast.Xor(src1=a, src2=b):
            if is_a(a) or is_a(b):
                return True
        case asm_ast.Compare(left=l, right=r):
            if is_a(l) or is_a(r):
                return True
        case asm_ast.Push(src=s):
            if is_a(s):
                return True
        case asm_ast.ArithmeticShiftLeft(dst=d) | asm_ast.LogicalShiftRight(dst=d):
            if is_a(d):
                return True
        case asm_ast.RotateLeft(dst=d) | asm_ast.RotateRight(dst=d):
            if is_a(d):
                return True
    return False


def _writes_reg_a(instr: asm_ast.Type_instruction) -> bool:
    """True iff `instr` overwrites Reg(A) (clobbers the prior
    value)."""
    def is_a(op):
        return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)
    match instr:
        case asm_ast.Mov(dst=d):
            if is_a(d):
                return True
        case asm_ast.Add(dst=d) | asm_ast.Sub(dst=d):
            if is_a(d):
                return True
        case asm_ast.And(dst=d) | asm_ast.Or(dst=d) | asm_ast.Xor(dst=d):
            if is_a(d):
                return True
        case asm_ast.Pop(dst=d):
            if is_a(d):
                return True
        case asm_ast.ArithmeticShiftLeft(dst=d) | asm_ast.LogicalShiftRight(dst=d):
            if is_a(d):
                return True
        case asm_ast.RotateLeft(dst=d) | asm_ast.RotateRight(dst=d):
            if is_a(d):
                return True
    return False


def _operand_read_after(
    target: asm_ast.Type_operand,
    instrs: list[asm_ast.Type_instruction],
    start: int,
) -> bool:
    """True iff `target` appears anywhere in a non-dst operand
    position in `instrs[start:]`. Used to gate the temp-dead
    check: if the temp is never read again, dropping the STA + LDY
    pair that wrote/read it is safe."""
    for instr in instrs[start:]:
        for op, is_dst in _operand_roles(instr):
            if is_dst:
                continue
            if op == target:
                return True
    return False


def _operand_roles(instr: asm_ast.Type_instruction):
    """Yield `(operand, is_dst)` pairs for every operand of
    `instr`. is_dst=True for the destination position of a Mov;
    every other position counts as a read."""
    match instr:
        case asm_ast.Mov(src=s, dst=d):
            yield s, False
            yield d, True
        case asm_ast.Add(src=s, dst=d) | asm_ast.Sub(src=s, dst=d):
            yield s, False
            yield d, False  # RMW
        case asm_ast.And(src1=a, src2=b, dst=d):
            yield a, False
            yield b, False
            yield d, False
        case asm_ast.Or(src1=a, src2=b, dst=d):
            yield a, False
            yield b, False
            yield d, False
        case asm_ast.Xor(src1=a, src2=b, dst=d):
            yield a, False
            yield b, False
            yield d, False
        case asm_ast.Inc(dst=d) | asm_ast.Dec(dst=d):
            yield d, False
        case asm_ast.ArithmeticShiftLeft(dst=d) | asm_ast.LogicalShiftRight(dst=d):
            yield d, False
        case asm_ast.RotateLeft(dst=d) | asm_ast.RotateRight(dst=d):
            yield d, False
        case asm_ast.Push(src=s):
            yield s, False
        case asm_ast.Pop(dst=d):
            yield d, True
        case asm_ast.Compare(left=l, right=r):
            yield l, False
            yield r, False
        case asm_ast.LoadAddress(src=s, dst=d):
            yield s, False
            yield d, True
        case asm_ast.BitTest(operand=op):
            yield op, False


def _build_label_index(
    instrs: list[asm_ast.Type_instruction],
) -> dict[str, int]:
    out: dict[str, int] = {}
    for i, instr in enumerate(instrs):
        if isinstance(instr, asm_ast.Label):
            out[instr.name] = i
    return out
