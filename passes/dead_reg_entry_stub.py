"""Targeted dead-store elimination for `__attribute__((reg(...)))`
parameter entry stubs.

When `tac_to_asm` lowers a function with one or more reg-attributed
parameters, it emits an entry stub `Mov(Reg(R), Data(__zpabi_<fn>__
<param>))` at the function head — copying the calling-convention's
incoming register into the param's ZP slot so the body can read the
param like any other zp_abi byte. After the asm-level regalloc pins
the param's body Pseudos to the same register R, the slot is never
read in the body and the entry stub becomes dead.

The general `apply_asm_dead_store` pass can't drop the stub when the
body contains an `Indirect` read through DPTR (which conservatively
aliases every byte). But we know more here:

  - The slot name follows the `__zpabi_<fn>__<param>` convention, so
    it sits in this function's calling-convention namespace.
  - User code can't construct the slot's address: there's no syntax
    to take `&__zpabi_*` directly, and `&param` for a reg-attributed
    parameter is a type-check error. For non-reg params an `&param`
    would force a frame copy upstream, so it still wouldn't yield a
    pointer to the slot.
  - Therefore any `Indirect` read in this function's body cannot
    alias the slot byte. If no `Data(slot_sym)` read appears in the
    body, the slot is dead and the entry stub can be dropped.

This pass walks each function once after `replace_pseudoregisters_
bare_exit` (operands concrete, regalloc decisions baked in) and
before the peephole fixedpoint's final `apply_asm_dead_store` so its
removal exposes any subsequent dead computations. Bookkeeping is
minimal: gather the function's own reg-attributed slot symbols, scan
read positions for any reference, drop the matching entry stub when
the slot is never read.
"""
from __future__ import annotations

import asm_ast
from passes.abi_selection import ZpLayout


def apply_dead_reg_entry_stub_drop(
    prog: asm_ast.Program,
    abi: dict[str, object] | None,
) -> asm_ast.Program:
    """Drop dead reg-attributed-param entry stubs.

    `abi` is the per-function `ParamLayout` dict from
    `passes.abi_selection.select_abi`. Each ZpLayout names the
    function's slot symbols and (when reg attributes apply) the
    per-byte `param_registers` list. When `abi` is None or a
    function has no reg-attributed params, the function passes
    through unchanged.
    """
    if abi is None:
        return prog
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_top.append(_rewrite_function(tl, abi))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rewrite_function(
    fn: asm_ast.Function, abi: dict[str, object],
) -> asm_ast.Function:
    layout = abi.get(fn.name)
    if not isinstance(layout, ZpLayout):
        return fn
    # Build the set of `(slot_sym, expected_register)` pairs for
    # the function's reg-attributed param bytes. Each reg-passed
    # byte sits at exactly one slot symbol (v1 only supports
    # 1-byte reg-attributed params, so it's a 1:1 mapping).
    reg_stubs: dict[str, str] = {}
    for i, slot_sym in enumerate(layout.slot_symbols):
        if i >= len(layout.param_registers):
            break
        reg = layout.param_registers[i]
        if reg is not None:
            reg_stubs[slot_sym] = reg
    if not reg_stubs:
        return fn
    # Find slots that ARE read somewhere in the body — those
    # entry stubs stay. A "read" is any operand position other
    # than the Mov.dst that references the slot's Data() form.
    slots_read: set[str] = set()
    for instr in fn.instructions:
        for op, is_write in _operand_roles_with_dst_flag(instr):
            if is_write:
                continue
            if isinstance(op, asm_ast.Data) and op.name in reg_stubs:
                slots_read.add(op.name)
    # Drop matching entry stubs. The pattern is exactly
    # `Mov(Reg(R), Data(slot, 0))` where R is the register the
    # layout names. Anything else stays — out of an abundance of
    # caution we don't drop a Mov whose src is something other
    # than the expected register, even if the slot is otherwise
    # dead.
    out: list[asm_ast.Type_instruction] = []
    for instr in fn.instructions:
        if _is_droppable_stub(instr, reg_stubs, slots_read):
            continue
        out.append(instr)
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    )


def _is_droppable_stub(
    instr: asm_ast.Type_instruction,
    reg_stubs: dict[str, str],
    slots_read: set[str],
) -> bool:
    if not isinstance(instr, asm_ast.Mov):
        return False
    if instr.is_volatile:
        return False
    if not isinstance(instr.src, asm_ast.Reg):
        return False
    if not isinstance(instr.dst, asm_ast.Data):
        return False
    if instr.dst.offset != 0:
        return False
    expected = reg_stubs.get(instr.dst.name)
    if expected is None:
        return False
    if instr.dst.name in slots_read:
        return False
    # Match the register the layout named.
    reg = instr.src.reg
    if expected == "A" and not isinstance(reg, asm_ast.A):
        return False
    if expected == "X" and not isinstance(reg, asm_ast.X):
        return False
    if expected == "Y" and not isinstance(reg, asm_ast.Y):
        return False
    return True


def _operand_roles_with_dst_flag(instr: asm_ast.Type_instruction):
    """Yield `(operand, is_write)` pairs for every operand of
    `instr`. `is_write=True` only for the destination slot of a
    Mov / Add / Sub / etc.; every other position is read. Used by
    the slot-read scan above to distinguish reads from writes."""
    match instr:
        case asm_ast.Mov(src=s, dst=d):
            yield s, False
            yield d, True
        case asm_ast.Add(src=s, dst=d) | asm_ast.Sub(src=s, dst=d):
            yield s, False
            yield d, False  # ALU dst is RMW (read + write)
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
            yield d, False  # RMW
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
        case _:
            # Branches / Jumps / Labels / SetCarry / ClearCarry /
            # FunctionPrologue / Ret / Return / Call / Phi /
            # IndirectCall / IndexedStore: no slot-relevant
            # Data operand reads to worry about, OR carry no
            # operands at this stage.
            return
