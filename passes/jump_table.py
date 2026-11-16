"""Jump tables: a chain of compares of A with constants, each followed
by a branch to its case, becomes a dispatch through a table of the
cases' addresses.

# Pattern

A `switch` on a byte becomes a chain like this one, from Zork's
`do_2op` (examples/zork/interpreter/decode.c), with 24 cases:

    CMP   #$01
    BEQ   .case@35
    CMP   #$02
    BEQ   .case@36
    ...
    CMP   #$18
    BEQ   .case@58
    JMP   .default@59

Most of the cases are too far for a branch, so each `BEQ` grows into
`BNE; JMP`, 7 bytes a case. The chain becomes

    CMP   #$19
    BCS   .default@59
    ASL
    TAY
    LDA   .jtab@0+1,Y
    PHA
    LDA   .jtab@0,Y
    PHA
    RTS
.jtab@0:
    DC.W  .default@59-1      ; 0
    DC.W  .case@35-1         ; 1
    ...

`RTS` pulls the address the two `PHA`s pushed and adds one, so it
goes to the case. That's 15 bytes and 2 a case, and the same 32
cycles or so for every case, where the chain took 5 for each compare
it passed. Zork's own code dispatches through tables the same way.

# Rules

- At least four compares of A with constants from 0 to 127, each
  right after the one before and followed by a `BEQ` to its case,
  with no label in the chain, and no constant twice. The case for 0
  may be a `BEQ` right after the load of A, with no compare.
- Then a `JMP` to the default, or code that is the default, which gets
  a label (`.jtdef@N`).
- At every case and the default, A, N, Z, and C are dead, since the
  dispatch leaves A and the flags as the table's load set them, and
  so is Y, the index, or else X, which takes its place.
- The table covers the values from the smallest constant to the
  largest, a value no compare names going to the default. When the
  smallest is 2 or less, it starts at 0, which costs at most two
  entries and saves the `CMP; BCC` of a lower bound.
- The table is shorter than the chain, counting each compare and
  branch as 2 bytes, as they are when the branch reaches its case.

# Where to run

Once, at the end of the `--optimize` asm pipeline, after
`carry_known` and before `expand_long_branches`. Every pass that
follows control flow has run by then, so none of them has to know
the table. `expand_long_branches` sizes it through `instruction_size`.
"""
from __future__ import annotations

import re

import asm_ast
from passes.asm_liveness import (
    a_dead_at, c_dead_at, flags_dead_at, x_dead_at, y_dead_at,
)

_LABEL_RE = re.compile(r"\.(?:jtab|jtdef)@(\d+)$")
# Compares in a chain worth a table, at the least.
_MIN_CASES = 4
# The dispatch: `ASL; TAY; LDA t+1,Y; PHA; LDA t,Y; PHA; RTS`.
_DISPATCH = 11


def apply_jump_tables(prog: asm_ast.Program) -> asm_ast.Program:
    """Turns each compare chain the rules allow into a jump table, in
    every function. Labels are numbered across the program, since the
    sim's assembler rejects one name in two functions."""
    counter = 1 + max(
        (int(m.group(1))
         for tl in prog.top_level if isinstance(tl, asm_ast.Function)
         for ins in tl.instructions
         if isinstance(ins, asm_ast.Label)
         and (m := _LABEL_RE.match(ins.name))),
        default=-1,
    )
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            tl, counter = _rewrite_function(tl, counter)
        new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _is_a(op) -> bool:
    return isinstance(op, asm_ast.Reg) and isinstance(op.reg, asm_ast.A)


def _chain_at(instrs, i):
    """The `(constant, target)` pairs of the chain starting at
    `instrs[i]`, and how many instructions the chain takes. A case for
    0 may come first as a `BEQ` right after a load into A, which sets
    Z from the value, with no `CMP #$00`."""
    start = i
    pairs = []
    if (0 < i < len(instrs) and isinstance(instrs[i], asm_ast.Branch)
            and isinstance(instrs[i].cond, asm_ast.EQ)
            and isinstance(instrs[i - 1], asm_ast.Mov)
            and _is_a(instrs[i - 1].dst)
            and not isinstance(instrs[i - 1].src, asm_ast.Reg)):
        pairs.append((0, instrs[i].target))
        i += 1
    while i + 1 < len(instrs):
        cmp, br = instrs[i], instrs[i + 1]
        if not (isinstance(cmp, asm_ast.Compare) and _is_a(cmp.left)
                and isinstance(cmp.right, asm_ast.Imm)
                and 0 <= cmp.right.value <= 127
                and isinstance(br, asm_ast.Branch)
                and isinstance(br.cond, asm_ast.EQ)):
            break
        pairs.append((cmp.right.value, br.target))
        i += 2
    return pairs, i - start


def _rewrite_function(
    fn: asm_ast.Function, counter: int,
) -> tuple[asm_ast.Function, int]:
    instrs = list(fn.instructions)
    i = 0
    while i < len(instrs):
        pairs, length = _chain_at(instrs, i)
        if len(pairs) < _MIN_CASES:
            i += 1
            continue
        new, counter = _table_for(instrs, i, i + length, pairs, counter)
        if new is None:
            i += length
            continue
        instrs = new
        i += 1
    if instrs == fn.instructions:
        return fn, counter
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=instrs,
    ), counter


def _table_for(instrs, i, end, pairs, counter):
    """`instrs` with the chain of `pairs` from `instrs[i]` to before
    `instrs[end]` made a jump table, or None when the rules don't allow
    it."""
    keys = [k for k, _ in pairs]
    if len(set(keys)) != len(keys):
        return None, counter
    labels = {
        ins.name: k for k, ins in enumerate(instrs)
        if isinstance(ins, asm_ast.Label)
    }
    after = instrs[end] if end < len(instrs) else None
    jump_default = (
        isinstance(after, asm_ast.Jump) and after.target in labels
        and not after.reg_args
    )
    if jump_default:
        default = after.target
        default_at = labels[default]
        rest = end + 1
    else:
        default = None
        default_at = end
        rest = end
    lo, hi = min(keys), max(keys)
    base = 0 if lo <= 2 else lo
    # Each `CMP #k` and `BEQ` is 2 bytes.
    chain_bytes = 2 * (end - i) + (3 if jump_default else 0)
    table_bytes = (4 if base else 0) + 4 + _DISPATCH + 2 * (hi - base + 1)
    if table_bytes >= chain_bytes:
        return None, counter
    # Every place the dispatch can go: A, N/Z, C, and the index dead.
    places = [labels.get(t) for _, t in pairs] + [default_at]
    if any(p is None for p in places):
        return None, counter
    if not all(a_dead_at(instrs, p) and flags_dead_at(instrs, p)
               and c_dead_at(instrs, p) for p in places):
        return None, counter
    if all(y_dead_at(instrs, p) for p in places):
        index = "Y"
    elif all(x_dead_at(instrs, p) for p in places):
        index = "X"
    else:
        return None, counter
    new_label = None
    if default is None:
        default = f".jtdef@{counter}"
        counter += 1
        new_label = asm_ast.Label(name=default)
    table = f".jtab@{counter}"
    counter += 1
    by_key = dict(pairs)
    targets = [by_key.get(v, default) for v in range(base, hi + 1)]
    code: list[asm_ast.Type_instruction] = []
    if base:
        code += [
            asm_ast.Compare(left=asm_ast.Reg(reg=asm_ast.A()),
                            right=asm_ast.Imm(value=base)),
            asm_ast.Branch(cond=asm_ast.CC(), target=default),
        ]
    code += [
        asm_ast.Compare(left=asm_ast.Reg(reg=asm_ast.A()),
                        right=asm_ast.Imm(value=hi + 1)),
        asm_ast.Branch(cond=asm_ast.CS(), target=default),
        asm_ast.JumpTable(
            table=table, index=index, base=base, targets=targets,
        ),
    ]
    if new_label is not None:
        code.append(new_label)
    return instrs[:i] + code + instrs[rest:], counter
