"""Drop unused `__local_<fn>_b<k>` EQU bindings from the slot
symbol table just before emit.

# Why

`zp_local_allocation` reserves a private ZP byte range for each
function's body locals BEFORE the asm peephole fixed-point loop
runs. Subsequent passes (asm_dead_store, redundant_load_elim,
adc_commute, …) may then drop all reads and writes to one of those
byte slots — its EQU binding then becomes useless ballast in the
output, declaring an address no instruction references.

This pass scans the post-peephole IR for every `Data(name, _)` /
`IndexedData(name, _, _)` reference, then filters the slot-symbol
dict down to symbols that are actually used. We only prune symbols
matching `__local_*` — `__zpabi_*` slots and any other entry stays,
because callers in other translation units or the runtime stub may
reference them.

# What `local_bytes` reports after pruning

The `; @zp-link-meta-begin` link metadata block records each
function's `local_bytes` count, which the multi-TU linker uses to
size that function's private body-local range. Pruning the EQU
doesn't reclaim the bytes from the link allocator — they're still
reserved by `local_bytes`. Reclaiming those bytes is a follow-up
that re-runs the allocator after peephole convergence. For now,
pruning is purely cosmetic / clarity, not a packing win.

# Where to run

Right before `emit_program`. After every peephole has converged
and before the EQU block is rendered.
"""
from __future__ import annotations

import asm_ast


def prune_unused_locals(
    prog: asm_ast.Program, slot_symbols: dict[str, int],
) -> dict[str, int]:
    """Return a copy of `slot_symbols` with `__local_*` entries
    that no instruction in `prog` references removed. Non-local
    symbols (notably `__zpabi_*`) are kept regardless of local
    references, since they're part of the calling convention and
    other TUs may reference them."""
    referenced = _collect_referenced_data_names(prog)
    out: dict[str, int] = {}
    for name, addr in slot_symbols.items():
        if name.startswith("__local_") and name not in referenced:
            continue
        out[name] = addr
    return out


def _collect_referenced_data_names(prog: asm_ast.Program) -> set[str]:
    """Walk every operand of every instruction in every function;
    collect the `name` of every `Data` or `IndexedData` operand
    seen. Returns the set of referenced symbol names."""
    referenced: set[str] = set()
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            for instr in tl.instructions:
                for op in _operands_in(instr):
                    if isinstance(op, asm_ast.Data):
                        referenced.add(op.name)
                    elif isinstance(op, asm_ast.IndexedData):
                        referenced.add(op.name)
    return referenced


def _operands_in(instr: asm_ast.Type_instruction):
    """Yield every operand of `instr` regardless of read/write role.
    Used for unreferenced-symbol detection — we don't distinguish
    reads from writes since either kind makes the symbol live."""
    match instr:
        case asm_ast.Mov(src=s, dst=d):
            yield s
            yield d
        case asm_ast.Add(src=s, dst=d) | asm_ast.Sub(src=s, dst=d) \
                | asm_ast.And(src=s, dst=d) | asm_ast.Or(src=s, dst=d):
            yield s
            yield d
        case asm_ast.Xor(src1=s1, src2=s2, dst=d):
            yield s1
            yield s2
            yield d
        case asm_ast.Inc(dst=d) | asm_ast.Dec(dst=d) \
                | asm_ast.ArithmeticShiftLeft(dst=d) \
                | asm_ast.LogicalShiftRight(dst=d) \
                | asm_ast.RotateLeft(dst=d) \
                | asm_ast.RotateRight(dst=d):
            yield d
        case asm_ast.Push(src=s):
            yield s
        case asm_ast.Pop(dst=d):
            yield d
        case asm_ast.Compare(left=l, right=r):
            yield l
            yield r
        case asm_ast.BitTest(src=s):
            yield s
        case asm_ast.LoadAddress(src=s, dst=d):
            yield s
            yield d
        # Label / Jump / Branch / Call / Ret / Return / ClearCarry /
        # SetCarry / FunctionPrologue / AllocateStack / Phi (asm-SSA
        # only — not present at this stage) carry no Data / IndexedData
        # operands.
