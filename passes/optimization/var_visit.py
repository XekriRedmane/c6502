"""Structural Var/Val walkers for `tac_ast.Type_instruction`.

Three pure helpers shared by SSA construction, liveness analysis,
interference graph construction, and dead-store elimination:

  * `vals_in(instr)`  — every `Type_val` operand (use OR def), in
                         roughly source order. Used for "what Vars
                         appear anywhere in this instr".
  * `uses_in(instr)`  — Var operands that are READ. Excludes
                         `GetAddress.operand` (it names a storage cell,
                         not a value being read). Returns Phi sources
                         as uses for callers that want a flat
                         structural walk; liveness analysis
                         special-cases Phis to handle them as
                         per-edge predecessor uses instead.
  * `defs_in(instr)`  — Var operands that are WRITTEN.
  * `count_uses(instrs)` — Counter of USE-position Var names across an
                         instruction sequence (built on `uses_in`).

These helpers are intentionally signature-stable and SSA-agnostic —
both SSA-form and non-SSA-form TAC pass through unchanged.
"""

from __future__ import annotations

from collections import Counter
from typing import Callable, Iterable

import tac_ast


def rewrite_uses_in(
    instr: tac_ast.Type_instruction,
    mapper: Callable[[tac_ast.Type_val], tac_ast.Type_val],
) -> tac_ast.Type_instruction:
    """Rewrite every USE-position `Type_val` operand in `instr`
    via `mapper`. DEF positions and non-Val fields (e.g.
    `IndexedLoad.name`, `IndexedStore.address`, `GetAddress.operand`
    which names a storage cell rather than reads a value) are
    untouched.

    Returns a NEW instruction with rewritten operands when at least
    one operand changed; returns the original `instr` instance
    unchanged when no operand differs (caller can use `is` to detect
    changes cheaply)."""
    match instr:
        case tac_ast.Ret(val=v) if v is not None:
            nv = mapper(v)
            if nv is v:
                return instr
            return tac_ast.Ret(val=nv)
        case tac_ast.SignExtend(src=s, dst=d):
            ns = mapper(s)
            if ns is s:
                return instr
            return tac_ast.SignExtend(src=ns, dst=d)
        case tac_ast.ZeroExtend(src=s, dst=d):
            ns = mapper(s)
            if ns is s:
                return instr
            return tac_ast.ZeroExtend(src=ns, dst=d)
        case tac_ast.Truncate(src=s, dst=d):
            ns = mapper(s)
            if ns is s:
                return instr
            return tac_ast.Truncate(src=ns, dst=d)
        case tac_ast.IntToFloat(src=s, dst=d):
            ns = mapper(s)
            if ns is s:
                return instr
            return tac_ast.IntToFloat(src=ns, dst=d)
        case tac_ast.IntToDouble(src=s, dst=d):
            ns = mapper(s)
            if ns is s:
                return instr
            return tac_ast.IntToDouble(src=ns, dst=d)
        case tac_ast.FloatToInt(src=s, dst=d):
            ns = mapper(s)
            if ns is s:
                return instr
            return tac_ast.FloatToInt(src=ns, dst=d)
        case tac_ast.DoubleToInt(src=s, dst=d):
            ns = mapper(s)
            if ns is s:
                return instr
            return tac_ast.DoubleToInt(src=ns, dst=d)
        case tac_ast.FloatToDouble(src=s, dst=d):
            ns = mapper(s)
            if ns is s:
                return instr
            return tac_ast.FloatToDouble(src=ns, dst=d)
        case tac_ast.DoubleToFloat(src=s, dst=d):
            ns = mapper(s)
            if ns is s:
                return instr
            return tac_ast.DoubleToFloat(src=ns, dst=d)
        case tac_ast.Unary(op=op, src=s, dst=d):
            ns = mapper(s)
            if ns is s:
                return instr
            return tac_ast.Unary(op=op, src=ns, dst=d)
        case tac_ast.Binary(op=op, src1=s1, src2=s2, dst=d):
            ns1 = mapper(s1)
            ns2 = mapper(s2)
            if ns1 is s1 and ns2 is s2:
                return instr
            return tac_ast.Binary(op=op, src1=ns1, src2=ns2, dst=d)
        case tac_ast.Copy(src=s, dst=d):
            ns = mapper(s)
            if ns is s:
                return instr
            return tac_ast.Copy(src=ns, dst=d)
        case tac_ast.GetAddress():
            # GetAddress.operand names a storage cell — not a value read.
            return instr
        case tac_ast.Load(src_ptr=p, dst=d, is_volatile=v):
            np = mapper(p)
            if np is p:
                return instr
            return tac_ast.Load(src_ptr=np, dst=d, is_volatile=v)
        case tac_ast.Store(src=s, dst_ptr=p, is_volatile=v):
            ns = mapper(s)
            np = mapper(p)
            if ns is s and np is p:
                return instr
            return tac_ast.Store(src=ns, dst_ptr=np, is_volatile=v)
        case tac_ast.IndexedLoad(name=n, index=idx, dst=d, is_volatile=v):
            # IndexedLoad.name is the array's symbol identifier, not a
            # value — leave it alone. Only the index is a USE.
            nidx = mapper(idx)
            if nidx is idx:
                return instr
            return tac_ast.IndexedLoad(name=n, index=nidx, dst=d, is_volatile=v)
        case tac_ast.IndexedStore(address=a, index=idx, src=s, is_volatile=v):
            nidx = mapper(idx)
            ns = mapper(s)
            if nidx is idx and ns is s:
                return instr
            return tac_ast.IndexedStore(
                address=a, index=nidx, src=ns, is_volatile=v,
            )
        case tac_ast.IndexedSymbolStore(name=n, index=idx, src=s, is_volatile=v):
            nidx = mapper(idx)
            ns = mapper(s)
            if nidx is idx and ns is s:
                return instr
            return tac_ast.IndexedSymbolStore(
                name=n, index=nidx, src=ns, is_volatile=v,
            )
        case tac_ast.IndexedConstLoad(address=a, index=idx, dst=d, is_volatile=v):
            # IndexedConstLoad.address is an int constant — not a val.
            # Only the index is a USE.
            nidx = mapper(idx)
            if nidx is idx:
                return instr
            return tac_ast.IndexedConstLoad(
                address=a, index=nidx, dst=d, is_volatile=v,
            )
        case tac_ast.IndirectIndexedLoad(ptr=p, index=idx, dst=d, is_volatile=v):
            np = mapper(p)
            nidx = mapper(idx)
            if np is p and nidx is idx:
                return instr
            return tac_ast.IndirectIndexedLoad(
                ptr=np, index=nidx, dst=d, is_volatile=v,
            )
        case tac_ast.IndirectIndexedStore(ptr=p, index=idx, src=s, is_volatile=v):
            np = mapper(p)
            nidx = mapper(idx)
            ns = mapper(s)
            if np is p and nidx is idx and ns is s:
                return instr
            return tac_ast.IndirectIndexedStore(
                ptr=np, index=nidx, src=ns, is_volatile=v,
            )
        case tac_ast.JumpIfTrue(condition=c, target=t):
            nc = mapper(c)
            if nc is c:
                return instr
            return tac_ast.JumpIfTrue(condition=nc, target=t)
        case tac_ast.JumpIfFalse(condition=c, target=t):
            nc = mapper(c)
            if nc is c:
                return instr
            return tac_ast.JumpIfFalse(condition=nc, target=t)
        case tac_ast.JumpIfCmp(op=op, src1=s1, src2=s2, target=t):
            ns1 = mapper(s1)
            ns2 = mapper(s2)
            if ns1 is s1 and ns2 is s2:
                return instr
            return tac_ast.JumpIfCmp(op=op, src1=ns1, src2=ns2, target=t)
        case tac_ast.JumpIfMasked(
            val=v, mask=mask, jump_when_nonzero=jnz, target=t,
        ):
            nv = mapper(v)
            if nv is v:
                return instr
            return tac_ast.JumpIfMasked(
                val=nv, mask=mask, jump_when_nonzero=jnz, target=t,
            )
        case tac_ast.FunctionCall(name=n, args=args, dst=d):
            new_args = [mapper(a) for a in args]
            if all(na is a for na, a in zip(new_args, args)):
                return instr
            return tac_ast.FunctionCall(name=n, args=new_args, dst=d)
        case tac_ast.IndirectCall(ptr=p, args=args, dst=d):
            np = mapper(p)
            new_args = [mapper(a) for a in args]
            if np is p and all(na is a for na, a in zip(new_args, args)):
                return instr
            return tac_ast.IndirectCall(ptr=np, args=new_args, dst=d)
        case tac_ast.Phi(dst=d, args=phi_args):
            new_phi_args = [
                tac_ast.PhiArg(pred_label=a.pred_label, source=mapper(a.source))
                for a in phi_args
            ]
            if all(na.source is a.source for na, a in zip(new_phi_args, phi_args)):
                return instr
            return tac_ast.Phi(dst=d, args=new_phi_args)
    return instr


def vals_in(instr: tac_ast.Type_instruction) -> Iterable[tac_ast.Type_val]:
    """Every `Type_val` operand of `instr`, in roughly source order."""
    match instr:
        case tac_ast.Ret(val=v) if v is not None:
            yield v
        case tac_ast.Ret():
            return
        case tac_ast.SignExtend(src=s, dst=d) | tac_ast.ZeroExtend(src=s, dst=d) \
                | tac_ast.Truncate(src=s, dst=d) \
                | tac_ast.IntToFloat(src=s, dst=d) \
                | tac_ast.IntToDouble(src=s, dst=d) \
                | tac_ast.FloatToInt(src=s, dst=d) \
                | tac_ast.DoubleToInt(src=s, dst=d) \
                | tac_ast.FloatToDouble(src=s, dst=d) \
                | tac_ast.DoubleToFloat(src=s, dst=d) \
                | tac_ast.Unary(src=s, dst=d) \
                | tac_ast.Copy(src=s, dst=d):
            yield s
            yield d
        case tac_ast.GetAddress(operand=o, dst=d):
            yield o
            yield d
        case tac_ast.Load(src_ptr=p, dst=d):
            yield p
            yield d
        case tac_ast.Store(src=s, dst_ptr=p):
            yield s
            yield p
        case tac_ast.IndexedLoad(index=i, dst=d):
            yield i
            yield d
        case tac_ast.IndexedStore(index=i, src=s):
            yield i
            yield s
        case tac_ast.IndexedSymbolStore(index=i, src=s):
            yield i
            yield s
        case tac_ast.IndexedConstLoad(index=i, dst=d):
            yield i
            yield d
        case tac_ast.IndirectIndexedLoad(ptr=p, index=i, dst=d):
            yield p
            yield i
            yield d
        case tac_ast.IndirectIndexedStore(ptr=p, index=i, src=s):
            yield p
            yield i
            yield s
        case tac_ast.Binary(src1=s1, src2=s2, dst=d):
            yield s1
            yield s2
            yield d
        case tac_ast.JumpIfTrue(condition=c) | tac_ast.JumpIfFalse(condition=c):
            yield c
        case tac_ast.JumpIfCmp(src1=s1, src2=s2):
            yield s1
            yield s2
        case tac_ast.JumpIfMasked(val=v):
            yield v
        case tac_ast.FunctionCall(args=args, dst=d):
            yield from args
            if d is not None:
                yield d
        case tac_ast.IndirectCall(ptr=p, args=args, dst=d):
            yield p
            yield from args
            if d is not None:
                yield d
        case tac_ast.Phi(dst=d, args=args):
            yield d
            for a in args:
                yield a.source


def defs_in(instr: tac_ast.Type_instruction) -> list[tac_ast.Var]:
    """Var operands of `instr` that are *defined* (written)."""
    match instr:
        case tac_ast.SignExtend(dst=d) | tac_ast.ZeroExtend(dst=d) \
                | tac_ast.Truncate(dst=d) \
                | tac_ast.IntToFloat(dst=d) | tac_ast.IntToDouble(dst=d) \
                | tac_ast.FloatToInt(dst=d) | tac_ast.DoubleToInt(dst=d) \
                | tac_ast.FloatToDouble(dst=d) | tac_ast.DoubleToFloat(dst=d) \
                | tac_ast.Unary(dst=d) | tac_ast.Binary(dst=d) \
                | tac_ast.Copy(dst=d) \
                | tac_ast.GetAddress(dst=d) \
                | tac_ast.Load(dst=d) \
                | tac_ast.IndexedLoad(dst=d) \
                | tac_ast.IndexedConstLoad(dst=d) \
                | tac_ast.Phi(dst=d):
            return [d] if isinstance(d, tac_ast.Var) else []
        case tac_ast.FunctionCall(dst=d) | tac_ast.IndirectCall(dst=d):
            return [d] if d is not None and isinstance(d, tac_ast.Var) else []
    return []


def uses_in(instr: tac_ast.Type_instruction) -> list[tac_ast.Var]:
    """Var operands of `instr` that are *read*. Excludes
    `GetAddress.operand` (its name names a storage cell, not a value
    being read). Phi sources ARE returned as uses by this flat walk;
    liveness analysis special-cases Phis to attribute their sources
    to predecessor edges instead of the Phi's own block."""
    out: list[tac_ast.Var] = []
    match instr:
        case tac_ast.Ret(val=v) if v is not None:
            if isinstance(v, tac_ast.Var):
                out.append(v)
        case tac_ast.SignExtend(src=s) | tac_ast.ZeroExtend(src=s) \
                | tac_ast.Truncate(src=s) \
                | tac_ast.IntToFloat(src=s) | tac_ast.IntToDouble(src=s) \
                | tac_ast.FloatToInt(src=s) | tac_ast.DoubleToInt(src=s) \
                | tac_ast.FloatToDouble(src=s) | tac_ast.DoubleToFloat(src=s) \
                | tac_ast.Unary(src=s) | tac_ast.Copy(src=s):
            if isinstance(s, tac_ast.Var):
                out.append(s)
        case tac_ast.Binary(src1=s1, src2=s2):
            if isinstance(s1, tac_ast.Var):
                out.append(s1)
            if isinstance(s2, tac_ast.Var):
                out.append(s2)
        case tac_ast.Load(src_ptr=p):
            if isinstance(p, tac_ast.Var):
                out.append(p)
        case tac_ast.Store(src=s, dst_ptr=p):
            if isinstance(s, tac_ast.Var):
                out.append(s)
            if isinstance(p, tac_ast.Var):
                out.append(p)
        case tac_ast.IndexedLoad(index=i):
            if isinstance(i, tac_ast.Var):
                out.append(i)
        case tac_ast.IndexedStore(index=i, src=s):
            if isinstance(i, tac_ast.Var):
                out.append(i)
            if isinstance(s, tac_ast.Var):
                out.append(s)
        case tac_ast.IndexedSymbolStore(index=i, src=s):
            if isinstance(i, tac_ast.Var):
                out.append(i)
            if isinstance(s, tac_ast.Var):
                out.append(s)
        case tac_ast.IndexedConstLoad(index=i):
            if isinstance(i, tac_ast.Var):
                out.append(i)
        case tac_ast.IndirectIndexedLoad(ptr=p, index=i):
            if isinstance(p, tac_ast.Var):
                out.append(p)
            if isinstance(i, tac_ast.Var):
                out.append(i)
        case tac_ast.IndirectIndexedStore(ptr=p, index=i, src=s):
            if isinstance(p, tac_ast.Var):
                out.append(p)
            if isinstance(i, tac_ast.Var):
                out.append(i)
            if isinstance(s, tac_ast.Var):
                out.append(s)
        case tac_ast.JumpIfTrue(condition=c) | tac_ast.JumpIfFalse(condition=c):
            if isinstance(c, tac_ast.Var):
                out.append(c)
        case tac_ast.JumpIfCmp(src1=s1, src2=s2):
            if isinstance(s1, tac_ast.Var):
                out.append(s1)
            if isinstance(s2, tac_ast.Var):
                out.append(s2)
        case tac_ast.JumpIfMasked(val=v):
            if isinstance(v, tac_ast.Var):
                out.append(v)
        case tac_ast.FunctionCall(args=args):
            for a in args:
                if isinstance(a, tac_ast.Var):
                    out.append(a)
        case tac_ast.IndirectCall(ptr=p, args=args):
            if isinstance(p, tac_ast.Var):
                out.append(p)
            for a in args:
                if isinstance(a, tac_ast.Var):
                    out.append(a)
        case tac_ast.Phi(args=args):
            for a in args:
                if isinstance(a.source, tac_ast.Var):
                    out.append(a.source)
    return out


def count_uses(
    instrs: Iterable[tac_ast.Type_instruction],
) -> Counter[str]:
    """Count how many times each Var name appears in a USE position
    (per `uses_in`) across `instrs`. Returns a Counter, so an absent
    name reads as 0 — usable both as a single-use gate
    (`counts[name] == 1`) and a `.get`-style lookup."""
    counts: Counter[str] = Counter()
    for instr in instrs:
        for v in uses_in(instr):
            counts[v.name] += 1
    return counts
