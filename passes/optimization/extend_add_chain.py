"""Shared recognizer core for the `extend → add → load/store` chain.

The three addressing-mode recognizers — `recognize_indexed_load`,
`recognize_indexed_store`, and `recognize_indirect_indexed` — all match
the same upstream shape from a Load/Store's address operand `%addr`:

    %ext  := ZeroExtend(idx_var)                # idx_var is a 1-byte type
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
both SSA-renamed, and `%addr` single-use; the Add recognized; the extend a
ZeroExtend; `idx_var` a 1-byte Var. Each caller post-checks `base`'s kind and supplies its own emit /
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
      * `addr_var` / `ext_var` — the Add's dst and the extend's dst.
        `addr_var` is single-use and droppable.
      * `ext_droppable` — True when the Add is `ext_var`'s only use, so
        its def can go too. Common-subexpression elimination shares one
        extend among several subscripts with the same index, and then
        each chain is rewritten but the extend stays for the others
        (DSE drops it once the last one is gone)."""
    base: tac_ast.Type_val
    idx_var: tac_ast.Var
    addr_var: tac_ast.Var
    ext_var: tac_ast.Var
    ext_droppable: bool = True

    def drop_defs(self) -> tuple[tac_ast.Var, ...]:
        """The defs a recognizer drops along with the rewrite."""
        if self.ext_droppable:
            return (self.addr_var, self.ext_var)
        return (self.addr_var,)


def recognize_extend_add_chain(
    addr_var: tac_ast.Var, env: DefUseEnv, ctx: PassContext,
) -> ExtendAddChain | None:
    """Walk back from `addr_var` (a Load/Store address operand) through
    `Binary(Add, base, %ext)` and `%ext := ZeroExtend(idx)`,
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

    # Accept only a ZeroExtend. X and Y are unsigned, so `base,X` and
    # `(base),Y` address `base + idx` with `idx` in 0..255, which is
    # what `base + ZeroExtend(idx)` computes. A SignExtend of a
    # negative byte gives `base - 1` for -1 where the addressing mode
    # gives `base + 255`, and C allows the negative offset when the
    # pointer points into the middle of an object (`p[-1]`). The loop
    # rotation pass rewrites the SignExtend of a counter it proves
    # non-negative into a ZeroExtend, so such a counter still folds.
    # The dead high-byte extension residue is cleaned up by SSA-DCE /
    # byte-DCE once this rewrite removes the chain's only consumer.
    ext = _extend_def(ext_var, env)
    if not isinstance(ext, tac_ast.ZeroExtend):
        return None
    if not isinstance(ext.src, tac_ast.Var):
        return None
    idx_var = ext.src
    if not is_1_byte_var(idx_var, ctx.symbols):
        return None

    return ExtendAddChain(
        base=base, idx_var=idx_var, addr_var=addr_var, ext_var=ext_var,
        ext_droppable=env.extra.get(ext_var.name, 0) == 1,
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
    return isinstance(v, tac_ast.Var) and _extend_def(v, env) is not None


# How many Copies `_extend_def` follows back to the extend.
_MAX_COPY_HOPS = 4


def _extend_def(
    v: tac_ast.Var, env: DefUseEnv,
) -> tac_ast.ZeroExtend | tac_ast.SignExtend | None:
    """Returns the ZeroExtend / SignExtend that defines `v`, looking
    through a few `Copy`s, or None. A `Copy` between same-width types
    of different signedness (the lowering of an `int` to `unsigned`
    cast) isn't propagated by copy propagation, so after common-
    subexpression elimination shares one extend among several
    subscripts, each subscript's index can be a Copy of it. The
    addressing modes read only the index's low byte, which the Copy
    doesn't change."""
    for _ in range(_MAX_COPY_HOPS + 1):
        instr = env.def_of(v)
        if isinstance(instr, (tac_ast.ZeroExtend, tac_ast.SignExtend)):
            return instr
        if not (isinstance(instr, tac_ast.Copy)
                and isinstance(instr.src, tac_ast.Var)):
            return None
        v = instr.src
    return None


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
