"""Common-subexpression elimination over the dominator tree.

When an instruction computes the same value as an earlier one, it's
replaced by a `Copy` from the earlier one's dst, which copy
propagation and dead-store elimination then remove. Two kinds of
instructions are matched.

  * Pure computations. `Binary`, `Unary`, `ZeroExtend`, `SignExtend`,
    and `Truncate` whose operands are SSA values or Constants. In SSA
    form such an instruction's result depends only on its operands,
    so an identical instruction whose block dominates this one has
    the same result. A walk of the dominator tree carries a table of
    them from each block to the blocks it dominates.
  * Loads. `Load`, `IndexedLoad`, `IndexedConstLoad`, and
    `IndirectIndexedLoad` that aren't volatile. A load's result also
    depends on memory, so an earlier identical load is reused only
    when nothing could have written memory in between. The table of
    loads is carried into a block only from its single predecessor,
    so every path to the reuse passes through the earlier load. Any
    instruction that could write memory empties it: a store, a call,
    a volatile access, and any write to a name that isn't an SSA
    value (a static, or an address-taken local).

The typical source is C that repeats a subscript or a test, such as

    if (key[1] < entry[0]) return 0;
    if (key[1] == entry[0]) { ... }

where the second test reloads both bytes, and `dict[n + 1]`,
`dict[n + 2]`, `dict[n + 3]`, which each zero-extend `n`.

A dst must be an SSA value too, so that replacing its one definition
with a Copy changes nothing else. The key includes the dst's type,
so `ZeroExtend(c)` to `int` and to `long` aren't merged. Operands of
the commutative operators are put in a fixed order, so `a + b` and
`b + a` match.
"""

from __future__ import annotations

import tac_ast
from passes.optimization.cfg import (
    build_cfg, cfg_to_function, dominator_tree_children,
    immediate_dominators, ENTRY_ID,
)
from passes.optimization.framework import FixedpointPass, PassContext

_PURE = (
    tac_ast.Binary, tac_ast.Unary,
    tac_ast.ZeroExtend, tac_ast.SignExtend, tac_ast.Truncate,
)
_LOADS = (
    tac_ast.Load, tac_ast.IndexedLoad, tac_ast.IndexedConstLoad,
    tac_ast.IndirectIndexedLoad,
)
_COMMUTATIVE = (
    tac_ast.Add, tac_ast.Multiply, tac_ast.BitwiseAnd, tac_ast.BitwiseOr,
    tac_ast.BitwiseXor, tac_ast.Equal, tac_ast.NotEqual,
)
# Instructions that never write memory. Any other instruction, and
# any of these whose dst isn't an SSA value, empties the load table.
_NO_MEMORY_WRITE = _PURE + _LOADS + (
    tac_ast.Copy, tac_ast.GetAddress, tac_ast.Label, tac_ast.Phi,
    tac_ast.Jump, tac_ast.JumpIfTrue, tac_ast.JumpIfFalse,
    tac_ast.JumpIfCmp, tac_ast.JumpIfMasked, tac_ast.Ret,
)


class EliminateCommonSubexpressions(FixedpointPass):
    name = "eliminate_common_subexpressions"

    def run(self, fn, ctx):
        return eliminate_common_subexpressions(fn, ctx)


def eliminate_common_subexpressions(
    fn: tac_ast.Function, ctx: PassContext,
) -> tac_ast.Function:
    """Returns `fn` with each repeated computation replaced by a Copy
    from its first occurrence. A no-op outside SSA form."""
    ssa = ctx.ssa_dsts
    if not ssa or ctx.symbols is None:
        return fn
    cfg = build_cfg(fn)
    idom = immediate_dominators(cfg)
    children = dominator_tree_children(idom)
    changed = False

    def operand_key(v):
        if isinstance(v, tac_ast.Constant):
            # The whole node, since the FP variants keep their value
            # in `bits` instead of `value`.
            return ("c", repr(v.const))
        if isinstance(v, tac_ast.Var) and v.name in ssa:
            return ("v", v.name)
        return None

    def type_key(name: str):
        sym = ctx.symbols.get(name)
        return repr(sym.type) if sym is not None else None

    def key_of(instr):
        dst = getattr(instr, "dst", None)
        if not (isinstance(dst, tac_ast.Var) and dst.name in ssa):
            return None, None
        if isinstance(instr, _PURE):
            if isinstance(instr, tac_ast.Binary):
                a, b = operand_key(instr.src1), operand_key(instr.src2)
                if a is None or b is None:
                    return None, None
                if isinstance(instr.op, _COMMUTATIVE):
                    a, b = sorted((a, b), key=repr)
                ops = (type(instr.op).__name__, a, b)
            else:
                s = operand_key(instr.src)
                if s is None:
                    return None, None
                op = getattr(instr, "op", None)
                ops = (type(op).__name__ if op else None, s)
            return "pure", (type(instr).__name__, ops, type_key(dst.name))
        if isinstance(instr, _LOADS) and not instr.is_volatile:
            match instr:
                case tac_ast.Load(src_ptr=p):
                    ops = (operand_key(p),)
                case tac_ast.IndexedLoad(name=n, index=i):
                    ops = (n, operand_key(i))
                case tac_ast.IndexedConstLoad(address=a, index=i):
                    ops = (a, operand_key(i))
                case tac_ast.IndirectIndexedLoad(ptr=p, index=i):
                    ops = (operand_key(p), operand_key(i))
            if None in ops:
                return None, None
            return "load", (type(instr).__name__, ops, type_key(dst.name))
        return None, None

    def writes_memory(instr) -> bool:
        if not isinstance(instr, _NO_MEMORY_WRITE):
            return True
        if getattr(instr, "is_volatile", False):
            return True
        dst = getattr(instr, "dst", None)
        return isinstance(dst, tac_ast.Var) and dst.name not in ssa

    # Iterative dominator-tree walk. Each entry carries the pure table
    # inherited from the block's immediate dominator and the load
    # table from its predecessor (empty unless that's its only one).
    stack = [(ENTRY_ID, {}, {})]
    while stack:
        bid, pure, loads = stack.pop()
        block = cfg.blocks[bid]
        instrs = block.instructions
        for idx, instr in enumerate(instrs):
            kind, key = key_of(instr)
            if kind is not None:
                table = pure if kind == "pure" else loads
                prev = table.get(key)
                if prev is not None:
                    instrs[idx] = tac_ast.Copy(
                        src=tac_ast.Var(name=prev), dst=instr.dst,
                    )
                    changed = True
                    continue
                table[key] = instr.dst.name
            if writes_memory(instr):
                loads = {}
        for child in children.get(bid, []):
            preds = cfg.blocks[child].predecessors
            child_loads = dict(loads) if preds == [bid] else {}
            stack.append((child, dict(pure), child_loads))
    if not changed:
        return fn
    return cfg_to_function(fn, cfg)
