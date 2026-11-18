"""Join store: a value that each arm of a branch leaves in a slot, and
that the join copies into memory, is stored into that memory by each
arm.

# Pattern

`OPERANDS[0] = (op & 0x40) ? get_var_content() : get_next_code_byte();`
in Zork's `do_instruction` (examples/zork/decode.c) keeps
the low byte in A across the join and the high byte in a slot:

       JSR   get_var_content
       STX   __local_do_instruction__v_1
       JMP   .cond_end@31
    .cond_else@30:
       JSR   get_next_code_byte
       LDY   #$00
       STY   __local_do_instruction__v_1
    .cond_end@31:
       STA   OPERANDS
       LDA   __local_do_instruction__v_1
       STA   OPERANDS+1

Each arm's store goes to `OPERANDS+1` instead, and the copy at the
join goes:

       JSR   get_var_content
       STX   OPERANDS+1
       JMP   .cond_end@31
    .cond_else@30:
       JSR   get_next_code_byte
       LDY   #$00
       STY   OPERANDS+1
    .cond_end@31:
       STA   OPERANDS

That saves the `LDA t; STA M` at the join, 4 or 5 bytes, and its 6
or 7 cycles on every path.

# Rules

- At a run of labels, within a few instructions, `LDA t; STA M`,
  with `t` one of the function's own `__local_*` slots and `M` a byte
  of memory with no index (`Data` or `ZP`), neither volatile.
- Nothing between the labels and the `LDA` reads or writes `t` or
  `M`, and nothing there is a call, a branch, or a label.
- `t` is dead after the `LDA`, and A and N/Z are dead after the `STA`,
  since they no longer hold `t`'s value.
- The labels are reached only by `JMP`s and by falling through, at
  most three of them, since a store to `M` may take a byte more
  than one to `t` when `M` is an absolute address. Each one's last
  write to `t` is a store from A, X, or Y, or of a constant (`LDA
  #k; STA t`, one `Mov` until `asm_emit`), after which its path to
  the labels has no call, branch, or label, and reads or writes
  neither `t` nor `M`. That store becomes a store to `M`.

`M` then takes its value earlier than it did, which no instruction
can see, since none between the arm's store and the join's reads
`M`.

# Where to run

In the asm peephole fixed point, after `redundant_store`. The store
to `t` it leaves is gone when the slot is dead, and `asm_dead_store`
needs nothing more.
"""
from __future__ import annotations

import asm_ast
from passes.asm_aliasing import _instruction_operands, may_alias
from passes.asm_dead_store import mem_dead_after
from passes.asm_liveness import a_dead_at, flags_dead_at

# Instructions between the labels and the `LDA t`, at the most.
_WINDOW = 8
# Jumps and fall-throughs into the labels, at the most.
_MAX_PREDS = 3

_STRAIGHT = (
    asm_ast.Mov, asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
    asm_ast.Xor, asm_ast.Compare, asm_ast.BitTest, asm_ast.ClearCarry,
    asm_ast.SetCarry, asm_ast.Inc, asm_ast.Dec, asm_ast.Push,
    asm_ast.Pop, asm_ast.ArithmeticShiftLeft,
    asm_ast.LogicalShiftRight, asm_ast.RotateLeft, asm_ast.RotateRight,
)
_TERMINATORS = (
    asm_ast.Jump, asm_ast.Return, asm_ast.Ret, asm_ast.JumpTable,
)
_NOT_MEMORY = (
    asm_ast.Reg, asm_ast.Imm, asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh,
)


def apply_join_store(
    prog: asm_ast.Program, *, zp_slot_symbols: dict[str, int] | None = None,
) -> asm_ast.Program:
    """Moves each join's `LDA t; STA M` into the stores to `t` before
    it, where the rules allow, in every function."""
    new_top = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            tl = _rewrite_function(tl, zp_slot_symbols)
        new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(fn, zp_slot_symbols):
    instrs = list(fn.instructions)
    changed = False
    i = 0
    while i < len(instrs):
        new = _rewrite_at(instrs, i, fn.name, zp_slot_symbols)
        if new is None:
            i += 1
            continue
        instrs = new
        changed = True
    if not changed:
        return fn
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    )


def _untouched(ins, cells) -> bool:
    """Whether `ins` is straight-line code that neither reads nor
    writes any of `cells`."""
    if not isinstance(ins, _STRAIGHT) or getattr(ins, "is_volatile", False):
        return False
    return not any(
        may_alias(op, c)
        for op in _instruction_operands(ins)
        if not isinstance(op, _NOT_MEMORY)
        for c in cells
    )


def _rewrite_at(instrs, i, fn_name, zp_slot_symbols):
    """`instrs` with the join store at the labels starting at
    `instrs[i]` moved into the stores before it, or None."""
    if not isinstance(instrs[i], asm_ast.Label):
        return None
    if i > 0 and isinstance(instrs[i - 1], asm_ast.Label):
        return None
    end = i
    while end < len(instrs) and isinstance(instrs[end], asm_ast.Label):
        end += 1
    names = {ins.name for ins in instrs[i:end]}
    # The `LDA t; STA M` within the window.
    k = end
    load = store = None
    while k + 1 < len(instrs) and k - end < _WINDOW:
        a, b = instrs[k], instrs[k + 1]
        if _join_pair(a, b, fn_name):
            load, store = a, b
            break
        k += 1
    if load is None:
        return None
    t, m = load.src, store.dst
    if not all(_untouched(ins, (t, m)) for ins in instrs[end:k]):
        return None
    if not (a_dead_at(instrs, k + 2) and flags_dead_at(instrs, k + 2)):
        return None
    if not mem_dead_after(
        instrs, k, t, fn_name=fn_name, zp_slot_symbols=zp_slot_symbols,
    ):
        return None
    # The jumps into the labels, and the fall-through.
    preds = []
    for j, ins in enumerate(instrs):
        if j in range(i, end):
            continue
        if isinstance(ins, asm_ast.Jump) and ins.target in names:
            if ins.reg_args:
                return None
            preds.append(j)
        elif _names_label(ins, names):
            return None
    if i > 0 and not isinstance(instrs[i - 1], _TERMINATORS):
        preds.append(i)
    if not preds or len(preds) > _MAX_PREDS:
        return None
    stores = []
    for p in preds:
        s = _last_store(instrs, p, t, m)
        if s is None:
            return None
        stores.append(s)
    out = list(instrs)
    for s in stores:
        out[s] = asm_ast.Mov(src=instrs[s].src, dst=m)
    del out[k:k + 2]
    return out


def _join_pair(a, b, fn_name) -> bool:
    """Whether `a; b` is `LDA t; STA M` with `t` one of `fn_name`'s
    own slots and `M` another byte of memory with no index."""
    return (
        isinstance(a, asm_ast.Mov) and isinstance(b, asm_ast.Mov)
        and not a.is_volatile and not b.is_volatile
        and isinstance(a.src, asm_ast.Data)
        and a.src.name.startswith(f"__local_{fn_name}__")
        and isinstance(a.dst, asm_ast.Reg)
        and isinstance(a.dst.reg, asm_ast.A)
        and isinstance(b.src, asm_ast.Reg)
        and isinstance(b.src.reg, asm_ast.A)
        and isinstance(b.dst, (asm_ast.Data, asm_ast.ZP))
        and b.dst != a.src
    )


def _names_label(ins, names) -> bool:
    """Whether `ins` refers to one of `names` other than as a `JMP`'s
    target: a branch, a jump table, or an operand."""
    if isinstance(ins, asm_ast.Branch):
        return ins.target in names
    if isinstance(ins, asm_ast.JumpTable):
        return any(t in names for t in ins.targets)
    if isinstance(ins, asm_ast.Label):
        return False
    return any(
        getattr(op, "name", None) in names
        for op in _instruction_operands(ins)
    )


def _last_store(instrs, end, t, m):
    """The index of the store from a register or of a constant into
    `t` that the path ending before `instrs[end]` last makes, with
    nothing after it that touches `t` or `m` or leaves the path, or
    None."""
    j = end - 1
    while j >= 0:
        ins = instrs[j]
        if (isinstance(ins, asm_ast.Mov) and ins.dst == t
                and not ins.is_volatile
                and isinstance(ins.src, (asm_ast.Reg, asm_ast.Imm))):
            return j
        if not _untouched(ins, (t, m)):
            return None
        j -= 1
    return None
