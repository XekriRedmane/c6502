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

These helpers are intentionally signature-stable and SSA-agnostic —
both SSA-form and non-SSA-form TAC pass through unchanged.
"""

from __future__ import annotations

from typing import Iterable

import tac_ast


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
        case tac_ast.Binary(src1=s1, src2=s2, dst=d):
            yield s1
            yield s2
            yield d
        case tac_ast.JumpIfTrue(condition=c) | tac_ast.JumpIfFalse(condition=c):
            yield c
        case tac_ast.JumpIfCmp(src1=s1, src2=s2):
            yield s1
            yield s2
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
        case tac_ast.JumpIfTrue(condition=c) | tac_ast.JumpIfFalse(condition=c):
            if isinstance(c, tac_ast.Var):
                out.append(c)
        case tac_ast.JumpIfCmp(src1=s1, src2=s2):
            if isinstance(s1, tac_ast.Var):
                out.append(s1)
            if isinstance(s2, tac_ast.Var):
                out.append(s2)
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
