"""Moves a run of instructions that several places in a unit repeat into
a function of its own, which each place calls.

# Pattern

Functions that call the same routine with the same arguments repeat
the code that sets the arguments up. Zork's `instr_mul`, `instr_div`,
and `instr_mod` (examples/zork/interpreter/numbers.c) each start

    LDA   OPERANDS+2
    STA   __zpabi_check_sign__b_0
    LDA   OPERANDS+3
    STA   __zpabi_check_sign__b_1
    LDA   OPERANDS
    STA   __zpabi_check_sign__a_0
    LDA   OPERANDS+1
    STA   __zpabi_check_sign__a_1
    JSR   check_sign

19 bytes in each. The pass moves such a run into a function of its
own, `F@run<n>`, that ends with `RTS`, and puts `JSR F@run<n>` in each
place the run was, which keeps each function's code around it as it
was. A run that ends with a call ends with a `JMP` to the callee
instead, which returns to the run's caller. `JSR` and `RTS` change no register and no flag, so the code after
the call sees what it saw after the run. The cost is 12 cycles at each
place and two bytes of the hardware stack while the run executes.

[Shared function tails](../docs/optimizations/shared_tails.md) share
code that ends in a `JMP` or `RTS`. This pass takes code in the middle
of a function, which can't end in one.

# Conditions

A run is a sequence of loads, stores, arithmetic and logic
instructions, compares, `SEC`, `CLC`, `BIT`, and calls, with no label,
branch, jump, return, push, or pull, no soft-stack operand (`Frame`,
`Stack`, a compound atom), and no name of a function's own
`__local_*` slot. The places must not overlap. A function with a
soft-stack frame is left alone, and so is a run inside a loop of at
most 64 bytes, where the call's 12 cycles a pass would be a large
share of the loop's time. In Zork's interpreter that rule costs 2
bytes.

A run of `s` bytes at `n` places takes `s + 1 + 3n` bytes in place of
`n * s`, and the pass makes the change only when that's smaller,
which for two places means a run of 8 bytes or more. It takes the run
that saves the most first and repeats.

The new function has no parameters or locals. A call it makes is one
the functions it came from made, and the link metadata's call graph,
built from the TAC, already has it as theirs, so the zero-page
allocation of every function's bytes is unchanged. Its symbol is a
copy of the first user's, with internal linkage, so it goes in the
same section.

# Where it runs

Last in `compile.optimized_asm`, after `apply_shared_tails`, since a
tail is cheaper to share with a `JMP` than a run with a `JSR`. Every
change only removes bytes from a function, so no branch falls out of
reach.
"""
from __future__ import annotations

from dataclasses import replace

import asm_ast
from sim.assembler import instruction_size

_RUN_KINDS = (
    asm_ast.Mov, asm_ast.Add, asm_ast.Sub, asm_ast.And, asm_ast.Or,
    asm_ast.Xor, asm_ast.Inc, asm_ast.Dec, asm_ast.ArithmeticShiftLeft,
    asm_ast.LogicalShiftRight, asm_ast.RotateLeft, asm_ast.RotateRight,
    asm_ast.Compare, asm_ast.BitTest, asm_ast.SetCarry,
    asm_ast.ClearCarry, asm_ast.Call,
)
_MAX_LEN = 24


def apply_outline_runs(
    prog: asm_ast.Program, symbols, *, zp_names=None,
) -> asm_ast.Program:
    """`prog` with each repeated run in a function of its own (see the
    module docstring). Each new function's symbol, a copy of the first
    user's with internal linkage, is added to `symbols`. `zp_names`
    are the names of zero-page bytes, which take one-byte operands, for
    the sizes of the runs (`sim.assembler.instruction_size`)."""
    top = list(prog.top_level)
    count = 0
    while True:
        best = _best_run(top, zp_names)
        if best is None:
            return asm_ast.Program(top_level=top)
        length, places = best
        first_fn = top[places[0][0]]
        name = f"{first_fn.name}@run{count}"
        count += 1
        body = first_fn.instructions[places[0][1]:places[0][1] + length]
        by_fn: dict[int, list[int]] = {}
        for fi, start in places:
            by_fn.setdefault(fi, []).append(start)
        for fi, starts in by_fn.items():
            fn = top[fi]
            instrs = list(fn.instructions)
            for start in sorted(starts, reverse=True):
                instrs[start:start + length] = [asm_ast.Call(name=name)]
            top[fi] = asm_ast.Function(
                name=fn.name, is_global=fn.is_global,
                params=list(fn.params), instructions=instrs,
            )
        sym = symbols.get(first_fn.name)
        if sym is not None:
            symbols[name] = replace(
                sym, attrs=replace(sym.attrs, is_global=False),
            )
        at = next(i for i, tl in enumerate(top)
                  if isinstance(tl, asm_ast.Function)
                  and tl.name == first_fn.name)
        # A run that ends with a call ends with a jump to the callee,
        # which returns to the run's caller.
        if isinstance(body[-1], asm_ast.Call):
            end = [*body[:-1], asm_ast.Jump(target=body[-1].name)]
        else:
            end = [*body, asm_ast.Return()]
        top.insert(at + 1, asm_ast.Function(
            name=name, is_global=False, params=[], instructions=end,
        ))


def _runnable(ins) -> bool:
    if not isinstance(ins, _RUN_KINDS):
        return False
    for op in vars(ins).values():
        if isinstance(op, (asm_ast.Frame, asm_ast.Stack, asm_ast.Pseudo)):
            return False
        if (isinstance(op, (asm_ast.Data, asm_ast.IndexedData))
                and op.name.startswith("__local_")):
            return False
    if isinstance(ins, asm_ast.Call) and (ins.reg_args or ins.preserves):
        # A call that takes arguments in registers, or keeps them, is
        # left where its registers are set.
        return False
    return True


def _best_run(top, zp_names=None):
    """`(length, [(function index, start), ...])` for the run that
    saves the most, or None."""
    keys: list[tuple[int, int, str]] = []
    sizes: dict[tuple[int, int], int] = {}
    for fi, tl in enumerate(top):
        if not isinstance(tl, asm_ast.Function):
            continue
        if any(isinstance(i, asm_ast.FunctionPrologue)
               for i in tl.instructions):
            continue
        loops = _loop_spans(tl.instructions)
        for k, ins in enumerate(tl.instructions):
            if any(lo <= k <= hi for lo, hi in loops):
                continue
            if _runnable(ins):
                keys.append((fi, k, repr(ins)))
                sizes[(fi, k)] = instruction_size(ins, zp_names=zp_names)
    ok = {(fi, k): r for fi, k, r in keys}
    best = None
    best_gain = 0
    for length in range(2, _MAX_LEN + 1):
        groups: dict[tuple[str, ...], list[tuple[int, int]]] = {}
        for fi, k, _ in keys:
            seq = []
            for j in range(k, k + length):
                r = ok.get((fi, j))
                if r is None:
                    break
                seq.append(r)
            else:
                groups.setdefault(tuple(seq), []).append((fi, k))
        any_repeat = False
        for seq, starts in groups.items():
            places = _non_overlapping(starts, length)
            n = len(places)
            if n < 2:
                continue
            any_repeat = True
            fi, k = places[0]
            s = sum(sizes[(fi, j)] for j in range(k, k + length))
            # The function adds an `RTS`, or none when it ends with a
            # call, which becomes a jump.
            ret = 0 if seq[-1].startswith("Call(") else 1
            gain = n * s - (s + ret + 3 * n)
            if gain > best_gain:
                best_gain = gain
                best = (length, places)
        if not any_repeat:
            break
    return best


# A loop of at most this many bytes keeps its runs in place, since the
# call costs 12 cycles a pass, which in a short loop is a large share.
_SHORT_LOOP = 64


def _loop_spans(instrs) -> list[tuple[int, int]]:
    """`(label index, branch index)` for each branch or jump back to a
    label earlier in the function, when the instructions between, the
    loop, take at most `_SHORT_LOOP` bytes."""
    at = {ins.name: k for k, ins in enumerate(instrs)
          if isinstance(ins, asm_ast.Label)}
    spans = []
    for k, ins in enumerate(instrs):
        if isinstance(ins, (asm_ast.Branch, asm_ast.Jump)):
            lab = at.get(ins.target)
            if lab is None or lab >= k:
                continue
            size = sum(instruction_size(x) for x in instrs[lab:k + 1]
                       if not isinstance(x, asm_ast.Label))
            if size <= _SHORT_LOOP:
                spans.append((lab, k))
    return spans


def _non_overlapping(starts, length):
    out = []
    last: dict[int, int] = {}
    for fi, k in sorted(starts):
        if fi in last and k < last[fi] + length:
            continue
        out.append((fi, k))
        last[fi] = k
    return out
