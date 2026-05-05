"""Dead static elimination.

A program-level pass that drops internal-linkage `StaticVariable`
top-levels nothing references. Composes naturally with the
const-fold pipeline:

  * The TAC-level `_fold_indexed_load` rewrites
    `IndexedLoad(name, ConstInt(idx), dst)` to `Copy(Constant(v),
    dst)` when `name` is a const array — every USE of the array's
    bytes becomes an immediate, but the `StaticVariable` storage
    is left in place.
  * `fold_const_statics` does the same for scalar const statics
    AND drops the storage when the rewrite covers every USE. It
    deliberately skips arrays (whose USE-rewriting lives at the
    TAC level instead).
  * After the unroll pass, every loop-iteration array reference
    has its index folded to a constant, so the TAC-level pass
    folds them all away. The array's `StaticVariable` then
    survives unreferenced — until this pass drops it.

Eligibility:

  * `is_global == False` — internal linkage. External-linkage
    statics (today: anything not declared `static`) might be read
    by another translation unit at link time, so we conservatively
    keep them. (c6502 only models a single TU, but the
    `is_global` bit rides through from the type checker, and
    respecting it now is cheap insurance for future multi-TU
    support.)
  * No function instruction references the static's name (via any
    operand kind: `Pseudo` / `Data` / `IndexedData` / `ImmLabelLow`
    / `ImmLabelHigh`).
  * No other static's `init` references the static (via
    `AddressInit`).

Where it runs: inside `optimize_program`, after the per-function
SSA round-trip. The per-function pass can drop loads via DCE
which sometimes removes the last reference to a static — if we
ran before, we'd miss those. Running after sees the final state.
"""
from __future__ import annotations

import asm_ast


def apply_dead_static_elimination(
    prog: asm_ast.Program,
) -> asm_ast.Program:
    referenced = _collect_referenced_names(prog)
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.StaticVariable):
            if tl.is_global or tl.name in referenced:
                new_top.append(tl)
            continue
        new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _collect_referenced_names(prog: asm_ast.Program) -> set[str]:
    names: set[str] = set()
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            for instr in tl.instructions:
                _scan_instruction(instr, names)
        elif isinstance(tl, asm_ast.StaticVariable):
            for init in tl.init:
                _scan_init(init, names)
    return names


def _scan_instruction(
    instr: asm_ast.Type_instruction, names: set[str],
) -> None:
    """Walk each dataclass field of `instr`; for any that hold an
    operand, scan it. Generic over instruction kinds — adding a
    new asm instruction with operand-typed fields will work
    automatically."""
    if not hasattr(instr, "__dataclass_fields__"):
        return
    for fname in instr.__dataclass_fields__:
        val = getattr(instr, fname)
        if isinstance(val, asm_ast.Type_operand):
            _scan_operand(val, names)


def _scan_operand(
    op: asm_ast.Type_operand, names: set[str],
) -> None:
    if isinstance(op, (asm_ast.Pseudo, asm_ast.Data,
                       asm_ast.IndexedData,
                       asm_ast.ImmLabelLow,
                       asm_ast.ImmLabelHigh)):
        names.add(op.name)


def _scan_init(
    init: asm_ast.Type_static_init, names: set[str],
) -> None:
    if isinstance(init, asm_ast.AddressInit):
        names.add(init.name)
