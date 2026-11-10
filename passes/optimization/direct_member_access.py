"""TAC pass that turns an access through `&obj + k` into a direct
access to byte `k` of `obj`.

`c99_to_tac` lowers a struct member, and an array element at a
constant index, as pointer arithmetic on the object's address:

    GetAddress(g, %p)
    Binary(Add, %p, Constant(12), %q)
    Store(c, %q)                      # g.command = c

or, after `recognize_indirect_indexed`, an `IndirectIndexedStore(%p,
Constant(12), c)`. `tac_to_asm` then builds the address in a
zero-page pair and stores through it with `STA (ptr),Y`. This pass
rewrites the access to `IndexedSymbolStore(g, Constant(12), c)` (or
`IndexedLoad` for a read), which `tac_to_asm` lowers with a constant
index to plain `STA g+12` for an object with static storage, or to
the local's own byte for an automatic one. The `GetAddress` and the
`Add` are left for dead-store elimination, which drops them when
nothing else uses the address.

When every access to a local struct or array goes this way, nothing
takes its address any more, so the asm register allocator can give
its bytes zero-page slots instead of a soft-stack frame.

The rewritten forms:

    Load(&obj + k, dst)                    -> IndexedLoad(obj, k, dst)
    Store(src, &obj + k)                   -> IndexedSymbolStore(obj, k, src)
    IndirectIndexedLoad(&obj + k, j, dst)  -> IndexedLoad(obj, k + j, dst)
    IndirectIndexedStore(&obj + k, j, src) -> IndexedSymbolStore(obj, k + j, src)

where `&obj + k` is an SSA pointer defined by `GetAddress(obj)`,
possibly followed by one `Binary(Add, _, Constant(k))`, and `j` is a
Constant. The volatile bit carries over.

Eligibility:

  * `obj` has static storage (any type), or it's an automatic struct,
    union, or array that isn't a parameter. An automatic scalar is
    left alone: once its address isn't taken, SSA construction could
    rename it, and the rewritten access names the original.
    Aggregates are never renamed.
  * `0 <= k + j <= 255`, since the index is one byte. An access
    outside the object through `&obj + k` is undefined behavior in C,
    so no other bound is needed.
  * The value loaded or stored is a scalar. Copying a whole struct
    member that is itself a struct still goes through the pointer.
"""
from __future__ import annotations

import tac_ast
from passes.optimization.framework import FixedpointPass, PassContext
from passes.type_checking import StaticAttr
import c99_ast


_INT_CONSTS = (
    tac_ast.ConstInt, tac_ast.ConstUInt, tac_ast.ConstLong,
    tac_ast.ConstULong, tac_ast.ConstLongLong, tac_ast.ConstULongLong,
    tac_ast.ConstChar, tac_ast.ConstUChar,
)

_AGGREGATES = (c99_ast.Structure, c99_ast.Union, c99_ast.Array)


class DirectMemberAccess(FixedpointPass):
    name = "direct_member_access"

    def run(self, fn, ctx: PassContext):
        return direct_member_access(fn, ctx)


def _strip(t):
    while isinstance(t, (c99_ast.Const, c99_ast.Volatile)):
        t = t.referenced_type
    return t


def _const_value(v) -> int | None:
    if isinstance(v, tac_ast.Constant) and isinstance(v.const, _INT_CONSTS):
        return v.const.value
    return None


def direct_member_access(
    fn: tac_ast.Function, ctx: PassContext,
) -> tac_ast.Function:
    """Returns `fn` with each eligible access through `&obj + k`
    rewritten to a direct access. A no-op outside SSA form or without
    a symbol table."""
    ssa = ctx.ssa_dsts
    symbols = ctx.symbols
    if not ssa or symbols is None:
        return fn
    params = set(fn.params)

    def eligible_object(name: str) -> bool:
        sym = symbols.get(name)
        if sym is None or isinstance(sym.type, c99_ast.FunType):
            return False
        if isinstance(sym.attrs, StaticAttr):
            return True
        return (name not in params
                and isinstance(_strip(sym.type), _AGGREGATES))

    def is_scalar(v) -> bool:
        if isinstance(v, tac_ast.Constant):
            return True
        sym = symbols.get(v.name) if isinstance(v, tac_ast.Var) else None
        return sym is not None and not isinstance(
            _strip(sym.type), _AGGREGATES,
        )

    # SSA pointer name -> (object, byte offset).
    base: dict[str, tuple[str, int]] = {}
    adds: list[tac_ast.Binary] = []
    for instr in fn.instructions:
        if (isinstance(instr, tac_ast.GetAddress)
                and isinstance(instr.operand, tac_ast.Var)
                and isinstance(instr.dst, tac_ast.Var)
                and instr.dst.name in ssa
                and eligible_object(instr.operand.name)):
            base[instr.dst.name] = (instr.operand.name, 0)
        elif (isinstance(instr, tac_ast.Binary)
                and isinstance(instr.op, tac_ast.Add)
                and isinstance(instr.dst, tac_ast.Var)
                and instr.dst.name in ssa):
            adds.append(instr)
    for add in adds:
        for ptr, off in ((add.src1, add.src2), (add.src2, add.src1)):
            k = _const_value(off)
            if (k is not None and isinstance(ptr, tac_ast.Var)
                    and ptr.name in base):
                obj, k0 = base[ptr.name]
                base[add.dst.name] = (obj, k0 + k)
                break

    def target(ptr, extra: int = 0) -> tuple[str, tac_ast.Constant] | None:
        if not isinstance(ptr, tac_ast.Var) or ptr.name not in base:
            return None
        obj, k = base[ptr.name]
        k += extra
        if not 0 <= k <= 255:
            return None
        return obj, tac_ast.Constant(const=tac_ast.ConstUChar(value=k))

    changed = False
    out: list[tac_ast.Type_instruction] = []
    for instr in fn.instructions:
        new = None
        match instr:
            case tac_ast.Load(src_ptr=p, dst=d, is_volatile=v):
                t = target(p)
                if t is not None and is_scalar(d):
                    new = tac_ast.IndexedLoad(
                        name=t[0], index=t[1], dst=d, is_volatile=v,
                    )
            case tac_ast.Store(src=s, dst_ptr=p, is_volatile=v):
                t = target(p)
                if t is not None and is_scalar(s):
                    new = tac_ast.IndexedSymbolStore(
                        name=t[0], index=t[1], src=s, is_volatile=v,
                    )
            case tac_ast.IndirectIndexedLoad(
                ptr=p, index=i, dst=d, is_volatile=v,
            ):
                j = _const_value(i)
                t = target(p, j) if j is not None else None
                if t is not None and is_scalar(d):
                    new = tac_ast.IndexedLoad(
                        name=t[0], index=t[1], dst=d, is_volatile=v,
                    )
            case tac_ast.IndirectIndexedStore(
                ptr=p, index=i, src=s, is_volatile=v,
            ):
                j = _const_value(i)
                t = target(p, j) if j is not None else None
                if t is not None and is_scalar(s):
                    new = tac_ast.IndexedSymbolStore(
                        name=t[0], index=t[1], src=s, is_volatile=v,
                    )
        if new is not None:
            out.append(new)
            changed = True
        else:
            out.append(instr)
    if not changed:
        return fn
    return tac_ast.Function(
        name=fn.name, is_global=fn.is_global, params=list(fn.params),
        instructions=out,
    )
