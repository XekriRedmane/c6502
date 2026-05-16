"""Asm-level basic-block forward propagation of memory immediates.

Tracks the value of named memory cells (`Data(name, offset)` and
`ZP(addr, offset)`) within a basic block when their content is
provably a known immediate, and substitutes that immediate at any
downstream use whose operand slot accepts `Imm`.

# Motivating case

Post-integer-promotion `(uchar & 0x80) != 0` lowers (after the
shift-byte-shuffle, the `Mov(Imm, A); And(Imm, A)` constant fold,
and the high-byte truncation) to:

    LDA #$00
    STA M_hi        ; M_hi = 0
    LDA M_lo
    ORA M_hi        ; sees the stored 0

`ORA M_hi` accepts `Imm` as its source. Knowing `M_hi = 0` lets us
rewrite to `ORA #$00`, which `const_arith_fold` then drops as an
identity op. The store to `M_hi` becomes dead and falls to
`asm_dead_store`.

# Tracked state (basic-block scope)

  * `a_value` — Python int when A's value is a known immediate;
    `None` otherwise. Updated by `Mov(Imm, A)` (set), any other
    write to A (cleared).
  * `mem_value` — dict mapping `(MemKey, name|addr, offset)` to
    a known immediate Python int. Updated by `Mov(Reg(A), <mem>)`
    when `a_value` is known, by `Mov(Imm(c), <mem>)` directly
    (if such a shape exists), and invalidated by any write to a
    cell that may alias `<mem>`.

# Substitution

For each instruction, before updating state, look at its source
operand(s) and substitute `Data(name, k)` / `ZP(addr, k)` with
the corresponding `Imm` if known. Only slots that accept `Imm` —
`Mov.src`, `And.src`, `Or.src`, `Add.src`, `Sub.src`,
`Compare.right` — are substitution candidates.

# Basic-block boundaries

Reset all state at:

  * `Label` — incoming control flow can arrive from anywhere.
  * `Jump` / `Branch` / `Return` / `Ret` — outgoing.
  * `Call` — helper / user call may write through any memory.
  * `LoadAddress` / `FunctionPrologue` / `AllocateStack` —
    compound atoms that asm_to_asm2 expands later; their
    in-between state isn't reasoned about here.

# Aliasing

The model is conservative: any write whose target we can't
classify as a definite `Data` / `ZP` cell invalidates every
tracked memory cell. Writes through `IndexedData(name, off, X)`
invalidate every `Data(name, *)` (the index can hit any byte in
the range). Writes through `Frame` / `Stack` / `Indirect` — the
soft-stack and indirect-Y modes — invalidate everything tracked
(we can't pin the effective address).

# Where to run

Inside `_peephole_fixedpoint`, before `const_arith_fold` so the
identity-drop has the substituted operand to recognize. After
`replace_pseudoregisters` (Pseudos are gone). Before
`expand_long_branches` (we don't touch branches).

# Idempotent

Substitutions and state updates are functions of the input IR
alone, so a second run produces the same IR. Fixed-point loop
safe."""

from __future__ import annotations

from dataclasses import dataclass, field

import asm_ast


def apply_mem_const_prop(prog: asm_ast.Program) -> asm_ast.Program:
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


@dataclass
class _BlockState:
    a_value: int | None = None
    mem_value: dict[tuple, int] = field(default_factory=dict)

    def reset(self) -> None:
        self.a_value = None
        self.mem_value.clear()


def _mem_key(op) -> tuple | None:
    """Return a hashable key for a tracked memory cell, or None if
    `op` isn't a `Data` or `ZP` operand."""
    if isinstance(op, asm_ast.Data):
        return ("data", op.name, op.offset)
    if isinstance(op, asm_ast.ZP):
        return ("zp", op.address, op.offset)
    return None


def _substitute_source(op, state: _BlockState):
    """If `op` is a `Data` / `ZP` whose value is in `state.mem_value`,
    return the corresponding `Imm`. Otherwise return `op` unchanged."""
    key = _mem_key(op)
    if key is None:
        return op
    v = state.mem_value.get(key)
    if v is None:
        return op
    return asm_ast.Imm(value=v)


def _is_reg_a(op) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _is_block_boundary(instr) -> bool:
    return isinstance(instr, (
        asm_ast.Label, asm_ast.Jump, asm_ast.Branch,
        asm_ast.Ret, asm_ast.Return, asm_ast.Call,
        asm_ast.LoadAddress, asm_ast.FunctionPrologue,
        asm_ast.AllocateStack,
    ))


def _invalidate_alias(state: _BlockState, dst) -> None:
    """Remove tracked entries that may alias `dst`. For `Data(name,
    off)` and `ZP(addr, off)` writes, only the exact-match cell is
    invalidated. For `IndexedData(name, off, X|Y)` writes, every
    `Data(name, *)` entry in the same name is invalidated (the
    index ranges over all `[name+off..name+off+0xFF]` bytes). For
    `Frame` / `Stack` / `Indirect` writes, every tracked entry is
    invalidated."""
    if isinstance(dst, asm_ast.Data):
        state.mem_value.pop(("data", dst.name, dst.offset), None)
        return
    if isinstance(dst, asm_ast.ZP):
        state.mem_value.pop(("zp", dst.address, dst.offset), None)
        return
    if isinstance(dst, asm_ast.IndexedData):
        to_drop = [
            k for k in state.mem_value
            if k[0] == "data" and k[1] == dst.name
        ]
        for k in to_drop:
            state.mem_value.pop(k, None)
        return
    # Frame / Stack / Indirect / IndexedX / IndexedY / unknown:
    # conservatively drop everything (we can't bound the effective
    # address).
    state.mem_value.clear()


def _rewrite_function(fn: asm_ast.Function) -> asm_ast.Function:
    out: list[asm_ast.Type_instruction] = []
    state = _BlockState()
    for instr in fn.instructions:
        if _is_block_boundary(instr):
            state.reset()
            out.append(instr)
            continue
        # Step 1: substitute sources at any Imm-accepting slot.
        instr = _substitute(instr, state)
        # Step 2: update state from the (possibly rewritten) instr.
        _update_state(instr, state)
        out.append(instr)
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _substitute(instr, state: _BlockState):
    """Return a (possibly rewritten) copy of `instr` with any
    `Data`/`ZP` source operand replaced by the corresponding `Imm`
    if its value is known. Slots that accept `Imm` only:
    `Mov.src` (when dst is `Reg(A)` — LDA imm is the only
    Imm-accepting Mov shape), `And.src`, `Or.src`, `Add.src`,
    `Sub.src`, `Compare.right`. Volatile Movs are left alone —
    a volatile load must re-read the memory cell every time, not
    use a previously-cached immediate."""
    if (isinstance(instr, asm_ast.Mov)
            and not instr.is_volatile
            and _is_reg_a(instr.dst)):
        new_src = _substitute_source(instr.src, state)
        if new_src is not instr.src:
            return asm_ast.Mov(src=new_src, dst=instr.dst)
        return instr
    if isinstance(instr, (asm_ast.And, asm_ast.Or,
                          asm_ast.Add, asm_ast.Sub)):
        new_src = _substitute_source(instr.src, state)
        if new_src is not instr.src:
            return type(instr)(src=new_src, dst=instr.dst)
        return instr
    if isinstance(instr, asm_ast.Compare):
        new_right = _substitute_source(instr.right, state)
        if new_right is not instr.right:
            return asm_ast.Compare(left=instr.left, right=new_right)
        return instr
    return instr


def _update_state(instr, state: _BlockState) -> None:
    """Update `state.a_value` and `state.mem_value` based on
    `instr`'s effects. Substitution has already happened, so an
    `instr.src == Imm(c)` here may be a freshly-substituted value
    or an original literal — either way it's a Python int we can
    track."""
    if isinstance(instr, asm_ast.Mov):
        # Volatile Mov: the source can yield a different value
        # between reads, and a volatile write to a memory cell
        # doesn't make the memory's value knowable to the prop
        # tracker (the program — or hardware — may overwrite it
        # asynchronously). Drop any tracking related to A and
        # invalidate any cached value for the dst.
        if instr.is_volatile:
            state.a_value = None
            if not _is_reg_a(instr.dst):
                _invalidate_alias(state, instr.dst)
            return
        if _is_reg_a(instr.dst):
            # Mov(<src>, A): A becomes <src>'s value.
            if isinstance(instr.src, asm_ast.Imm):
                state.a_value = instr.src.value & 0xFF
            else:
                # Other src — A's value isn't a known immediate.
                state.a_value = None
            return
        # Mov(<src>, <mem>): a write to memory.
        dst = instr.dst
        key = _mem_key(dst)
        if key is None:
            # Mov to a register other than A (X/Y): no mem change,
            # no A change.
            if isinstance(dst, asm_ast.Reg):
                return
            # Mov to Frame / Stack / Indirect / IndexedData: drop
            # everything we can't bound (handled by _invalidate_alias).
            _invalidate_alias(state, dst)
            return
        # Mov(Imm(c), <data|zp>) or Mov(A, <data|zp>) when A is known.
        if isinstance(instr.src, asm_ast.Imm):
            state.mem_value[key] = instr.src.value & 0xFF
            return
        if _is_reg_a(instr.src) and state.a_value is not None:
            state.mem_value[key] = state.a_value
            return
        # Mov from X/Y or some unknown source: invalidate this cell.
        state.mem_value.pop(key, None)
        return

    if isinstance(instr, (asm_ast.And, asm_ast.Or)):
        if _is_reg_a(instr.dst):
            if (state.a_value is not None
                    and isinstance(instr.src, asm_ast.Imm)):
                c = instr.src.value & 0xFF
                if isinstance(instr, asm_ast.And):
                    state.a_value = state.a_value & c
                else:
                    state.a_value = state.a_value | c
            else:
                state.a_value = None
            return
        # Dst is memory — invalidate it.
        _invalidate_alias(state, instr.dst)
        return

    if isinstance(instr, (asm_ast.Add, asm_ast.Sub)):
        # Reads carry; result is not statically known by this pass.
        if _is_reg_a(instr.dst):
            state.a_value = None
            return
        _invalidate_alias(state, instr.dst)
        return

    if isinstance(instr, asm_ast.Xor):
        # Xor's IR shape: src1, src2, dst.
        if _is_reg_a(instr.dst):
            state.a_value = None
            return
        _invalidate_alias(state, instr.dst)
        return

    # In-place memory ops: Inc, Dec, ArithmeticShiftLeft,
    # LogicalShiftRight, RotateLeft, RotateRight. Each writes its
    # dst.
    if isinstance(instr, (
        asm_ast.Inc, asm_ast.Dec,
        asm_ast.ArithmeticShiftLeft, asm_ast.LogicalShiftRight,
        asm_ast.RotateLeft, asm_ast.RotateRight,
    )):
        dst = instr.dst
        if _is_reg_a(dst):
            state.a_value = None
            return
        _invalidate_alias(state, dst)
        return

    # SetCarry / ClearCarry / Compare / Push / Pop: no A or memory
    # value change relevant here (Compare reads but doesn't write;
    # Push / Pop touch the hardware stack, not our tracked cells).
    return
