"""Shared recognizer core for the `extend → add → load/store` chain.

The three addressing-mode recognizers — `recognize_indexed_load`,
`recognize_indexed_store`, and `recognize_indirect_indexed` — all match
the same upstream shape from a Load/Store's address operand `%addr`:

    %ext  := ZeroExtend|SignExtend(idx_var)     # idx_var is a 1-byte type
    %addr := Binary(Add, base, %ext)            # base = Constant or pointer Var
    Load(%addr, …)  /  Store(…, %addr)

and rewrite it to an addressing-mode-specific node. They differ only in:

  * the use-site pattern (`Load` vs `Store` vs either),
  * whether `base` must be a `Constant` (absolute,X) or a pointer Var
    ((zp),Y),
  * the emitted instruction, and
  * whether the intermediate defs are dropped eagerly (`Rewrite.drop_defs`,
    indirect) or left for SSA-DCE (the absolute,X pair).

`recognize_extend_add_chain` captures the identical walk-back and the
gates every recognizer applies — symbol table present; `%addr` and `%ext`
both SSA-renamed and single-use; the Add recognized; `idx_var` a 1-byte
Var. Each caller post-checks `base`'s kind and supplies its own emit /
drop. `is_1_byte_var` / `is_1_byte_val` are the shared width predicates
(formerly copy-pasted into all three passes)."""
from __future__ import annotations

from dataclasses import dataclass

import c99_ast
import tac_ast
from passes.optimization.framework import DefUseEnv, PassContext


@dataclass
class ExtendAddChain:
    """A recognized `extend → add → use` chain.

      * `base` — the non-`%ext` operand of the Add (a `Constant` for the
        absolute,X recognizers, a pointer `Var` for the indirect one).
      * `idx_var` — the 1-byte index (the extend's source).
      * `addr_var` / `ext_var` — the Add's dst and the extend's dst, both
        single-use; droppable (eagerly, or by a later DSE sweep)."""
    base: tac_ast.Type_val
    idx_var: tac_ast.Var
    addr_var: tac_ast.Var
    ext_var: tac_ast.Var


def recognize_extend_add_chain(
    addr_var: tac_ast.Var, env: DefUseEnv, ctx: PassContext,
) -> ExtendAddChain | None:
    """Walk back from `addr_var` (a Load/Store address operand) through
    `Binary(Add, base, %ext)` and `%ext := ZeroExtend|SignExtend(idx)`,
    applying the gates common to all three recognizers. Returns the
    `ExtendAddChain`, or None if any gate fails. Callers post-check
    `base`'s kind and emit their own node."""
    if ctx.symbols is None:
        return None
    if ctx.ssa_dsts is None or addr_var.name not in ctx.ssa_dsts:
        return None
    if env.extra.get(addr_var.name, 0) != 1:  # %addr single-use
        return None

    binary = env.def_of(addr_var)
    if not isinstance(binary, tac_ast.Binary) or not isinstance(
        binary.op, tac_ast.Add,
    ):
        return None

    base, ext_var = _split_extend_operand(binary.src1, binary.src2, env)
    if ext_var is None:
        return None
    if ext_var.name not in ctx.ssa_dsts:
        return None
    if env.extra.get(ext_var.name, 0) != 1:  # %ext single-use
        return None

    # Accept both ZeroExtend (uchar promotion) and SignExtend (signed
    # 1-byte promotion). The 6502's indexed addressing reads only the
    # index's low byte, so the extend's high bytes don't affect the
    # byte address; SignExtend is sound because a negative index would
    # address outside the array, which C99 §6.5.6 leaves undefined.
    # The dead high-byte extension residue is cleaned up by SSA-DCE /
    # byte-DCE once this rewrite removes the chain's only consumer.
    ext = env.def_of(ext_var)
    if not isinstance(ext, (tac_ast.ZeroExtend, tac_ast.SignExtend)):
        return None
    if not isinstance(ext.src, tac_ast.Var):
        return None
    idx_var = ext.src
    if not is_1_byte_var(idx_var, ctx.symbols):
        return None

    return ExtendAddChain(
        base=base, idx_var=idx_var, addr_var=addr_var, ext_var=ext_var,
    )


def _split_extend_operand(
    a: tac_ast.Type_val, b: tac_ast.Type_val, env: DefUseEnv,
) -> tuple[tac_ast.Type_val, tac_ast.Var | None]:
    """Of the Add's two operands, return `(base, ext_var)` where
    `ext_var` is the operand that is a Var defined by a ZeroExtend /
    SignExtend and `base` is the other. Prefer `b` (src2) as the extend
    side. Returns `(a, None)` if neither operand is extend-defined.

    This unifies the historical `_split_const_var` (base = Constant) and
    `_split_var_var` (base = pointer Var): the caller decides which
    `base` kind it accepts."""
    if _defined_by_extend(b, env):
        return a, b
    if _defined_by_extend(a, env):
        return b, a
    return a, None


def _defined_by_extend(v: tac_ast.Type_val, env: DefUseEnv) -> bool:
    return isinstance(v, tac_ast.Var) and isinstance(
        env.def_of(v), (tac_ast.ZeroExtend, tac_ast.SignExtend),
    )


def is_1_byte_var(v: tac_ast.Var, symbols) -> bool:
    """True iff `v`'s symbol-table c99 type is a 1-byte scalar
    (Char / SChar / UChar), looking through Const / Volatile wrappers."""
    sym = symbols.get(v.name) if hasattr(symbols, "get") else None
    if sym is None:
        return False
    t = sym.type
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return isinstance(t, (c99_ast.Char, c99_ast.SChar, c99_ast.UChar))


def is_1_byte_val(v: tac_ast.Type_val, symbols) -> bool:
    """True iff `v` is a 1-byte typed value: a Var with a 1-byte c99
    type, or a Constant with a 1-byte variant (ConstChar / ConstUChar)."""
    if isinstance(v, tac_ast.Constant):
        return isinstance(v.const, (tac_ast.ConstChar, tac_ast.ConstUChar))
    if isinstance(v, tac_ast.Var):
        return is_1_byte_var(v, symbols)
    return False


def all_dsts(fn: tac_ast.Function) -> set[str]:
    """The set of all Var dst names in `fn`. Used as a stand-in for
    `ssa_dsts` when a recognizer's standalone entry point is called on
    synthetic TAC (the recognizers run post-`to_ssa` in the real
    pipeline, so every named temp is single-def in practice)."""
    out: set[str] = set()
    for instr in fn.instructions:
        if hasattr(instr, 'dst') and isinstance(instr.dst, tac_ast.Var):
            out.add(instr.dst.name)
    return out
