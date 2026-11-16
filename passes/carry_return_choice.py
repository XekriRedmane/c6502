"""Returns a `bool` in the carry flag without the annotation, when every
call to the function gains by it.

`[[c6502::return_bool_in_carry]]` makes a function return `e != 0`
in C, and a caller then branches on it with `BCC` / `BCS`. A function
returning its `bool` in A leaves the flags as whatever its last
instruction set, so a caller that branches on the result tests it
first, `ORA #$00` or `CMP #$00`, 2 bytes and 2 cycles. But a caller
that keeps the result as a value pays for the carry, `LDA #0; ROL A`,
3 bytes, where the value in A costs nothing. So the compiler chooses
the carry itself only for a function whose every call it can see and
whose every call uses the result in a way the carry suits.

# Eligibility

`choose_carry_returns` picks each function that

- returns `bool` (`FunAttr.returns_bool`), has internal linkage, and
  is defined in this unit, so that every call to it is here;
- isn't already declared `return_bool_in_carry`, has no address, and
  never has its address taken, since an indirect call can't know the
  result is in C;
- uses each call's result in one of three ways: not at all, as the
  condition of a `JumpIfTrue` / `JumpIfFalse` right after the call
  (past any label nothing jumps to), or as the value of a `Ret`
  right after it in a function that is chosen too, which then passes
  the carry on;
- and uses at least one call's result in one of the last two ways.

The conditions are decided for all the candidates together, since
dropping one can drop the candidates whose result it returns.

# Where to run

On the optimized TAC, before `tac_to_asm`, which lowers the returns
of a function whose `FunAttr.carry_return` is set, and the calls to
it, the way it does for the annotation. Only under `--optimize`.
"""
from __future__ import annotations

import dataclasses

import tac_ast
from passes.optimization.var_visit import uses_in


def choose_carry_returns(prog: tac_ast.Program, symbols) -> set[str]:
    """Sets `carry_return` on each function of `prog` that is chosen,
    in `symbols`, and returns their names."""
    from passes.type_checking import FunAttr
    functions = [
        tl for tl in prog.top_level if isinstance(tl, tac_ast.Function)
    ]
    taken = _taken_names(prog)
    candidates: set[str] = set()
    for fn in functions:
        sym = symbols.get(fn.name)
        attrs = getattr(sym, "attrs", None)
        if (isinstance(attrs, FunAttr) and attrs.returns_bool
                and not attrs.is_global and not attrs.carry_return
                and attrs.address is None and fn.name not in taken):
            candidates.add(fn.name)
    if not candidates:
        return set()
    # Each call to a candidate: the function it's in, and how its
    # result is used.
    calls: dict[str, list[tuple[str, str]]] = {n: [] for n in candidates}
    for fn in functions:
        counts: dict[str, int] = {}
        for ins in fn.instructions:
            for u in uses_in(ins):
                counts[u.name] = counts.get(u.name, 0) + 1
        instrs = fn.instructions
        targets = _jump_targets(instrs)
        for i, ins in enumerate(instrs):
            if not (isinstance(ins, tac_ast.FunctionCall)
                    and ins.name in candidates):
                continue
            calls[ins.name].append((fn.name, _use_of_result(
                ins, _next_instruction(instrs, i + 1, targets), counts,
            )))
    # Start from every candidate whose calls all suit the carry, and
    # drop those that don't gain, until nothing changes: a candidate
    # stays when each call's result is unused, branched on, or
    # returned by a function that stays, and at least one call is
    # branched on or returned that way.
    chosen = {
        n for n in candidates
        if all(u != "other" for _, u in calls[n])
    }
    changed = True
    while changed:
        changed = False
        for name in list(chosen):
            uses = calls[name]
            fits = all(
                u in ("none", "branch") or (u == "return" and c in chosen)
                for c, u in uses
            )
            gains = any(
                u == "branch" or (u == "return" and c in chosen)
                for c, u in uses
            )
            if not (fits and gains):
                chosen.discard(name)
                changed = True
    for name in chosen:
        sym = symbols[name]
        sym.attrs = dataclasses.replace(sym.attrs, carry_return=True)
    return chosen


def _jump_targets(instrs) -> set[str]:
    """The labels that some jump in `instrs` goes to."""
    out: set[str] = set()
    for ins in instrs:
        target = getattr(ins, "target", None)
        if isinstance(target, str):
            out.add(target)
    return out


def _next_instruction(instrs, k: int, targets: set[str]):
    """The first instruction from `instrs[k]` on that isn't a label
    nothing jumps to, such as one SSA left between a call and the
    jump that tests its result, or None at the end."""
    while k < len(instrs):
        ins = instrs[k]
        if not (isinstance(ins, tac_ast.Label) and ins.name not in targets):
            return ins
        k += 1
    return None


def _use_of_result(
    call: tac_ast.FunctionCall, nxt, counts: dict[str, int],
) -> str:
    """How `call`'s result is used: "none", "branch" (tested by a jump
    right after the call), "return" (returned right after it), or
    "other"."""
    dst = call.dst
    if dst is None:
        return "none"
    if not isinstance(dst, tac_ast.Var):
        return "other"
    n = counts.get(dst.name, 0)
    if n == 0:
        return "none"
    if n != 1:
        return "other"
    if (isinstance(nxt, (tac_ast.JumpIfTrue, tac_ast.JumpIfFalse))
            and nxt.condition == dst):
        return "branch"
    if isinstance(nxt, tac_ast.Ret) and nxt.val == dst:
        return "return"
    return "other"


def _taken_names(prog: tac_ast.Program) -> set[str]:
    """Every name the program uses as a value or in a static's
    initializer. A function's name is a value only where its address
    is taken."""
    out: set[str] = set()
    for tl in prog.top_level:
        if isinstance(tl, tac_ast.Function):
            for ins in tl.instructions:
                out.update(u.name for u in uses_in(ins))
                if isinstance(ins, tac_ast.GetAddress) and isinstance(
                    ins.operand, tac_ast.Var,
                ):
                    out.add(ins.operand.name)
        elif isinstance(tl, tac_ast.StaticVariable):
            for init in tl.init:
                if isinstance(init, tac_ast.AddressInit):
                    out.add(init.name)
    return out
