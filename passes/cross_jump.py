"""Cross-jumping: two blocks that end in the same instructions and a
jump to the same place share one copy of those instructions.

# Pattern

    BNE   .x4
    ASL   n               ; the * 2 path
    ROL   n+1
    JMP   .end
.x4:
    ...
    ASL   n               ; the * 4 path
    ROL   n+1
    ASL   n
    ROL   n+1
    JMP   .end

becomes

    BNE   .x4
.xjump@0:
    ASL   n
    ROL   n+1
    JMP   .end
.x4:
    ...
    ASL   n
    ROL   n+1
    JMP   .xjump@0

The second path runs the same instructions it ran before, and then
the same jump, so nothing it computes changes. It takes the 3 cycles
of one more `JMP`, and the shared instructions' bytes go. Zork's
`instr_mul` and `instr_div` (examples/zork/interpreter/numbers.c)
have this shape, as `if (b == 2) a <<= 1; else if (b == 4) a <<= 2;`
leaves it, and the original lets the `* 4` path fall into the
`* 2` one.

# Rules

- Both blocks end in a `JMP` to the same place: the same function
  (a tail call, with the same register arguments), or labels in the
  function that name the same instruction.
- The shared instructions are the longest run before the two jumps
  that is equal instruction by instruction, with no label, branch,
  jump, or return in it, so no other path enters either run partway.
- The run is at least two instructions. One, such as the `LDA #1`
  that ends many of the comparisons in Zork's `do_2op`, saves 2 bytes
  for a 3-cycle `JMP`, and on the interpreter's hottest paths that
  came to 52,000 more cycles in a session for 59 bytes.
- The later block jumps into the earlier one, which gets a label in
  front of the run (`.xjump@N`) when it has none.

Each call shares the pair with the most instructions in a function,
then looks again, until no pair is left.

# Where to run

In the asm peephole fixed-point loop, after `tail_merge`, which
shares a repeated return tail by inverting a branch around it.
"""
from __future__ import annotations

import re

import asm_ast

_LABEL_RE = re.compile(r"\.xjump@(\d+)$")
_STOPS = (asm_ast.Label, asm_ast.Branch, asm_ast.Jump, asm_ast.Return,
          asm_ast.Ret)


def apply_cross_jump(prog: asm_ast.Program) -> asm_ast.Program:
    """Shares the longest repeated run before two jumps to the same
    place, in every function. Labels are numbered across the program,
    since the sim's assembler rejects one name in two functions."""
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


def _destination(instrs, labels: dict[str, int], jump: asm_ast.Jump):
    """Where `jump` goes: the index of the first instruction after a
    label in the function, or the name of a function it tail-calls,
    with its register arguments."""
    at = labels.get(jump.target)
    if at is None:
        return ("call", jump.target, tuple(jump.reg_args))
    while at < len(instrs) and isinstance(instrs[at], asm_ast.Label):
        at += 1
    return ("here", at)


def _rewrite_function(
    fn: asm_ast.Function, counter: int,
) -> tuple[asm_ast.Function, int]:
    """Shares pairs until none is left. Each one removes instructions,
    so this ends, and a function with many repeated tails, such as
    the `LDA #1; JMP` of a switch's comparisons in Zork's `do_2op`,
    doesn't take a sweep of the fixed-point loop for each."""
    while True:
        new, counter = _share_one(fn, counter)
        if new is fn:
            return fn, counter
        fn = new


def _share_one(
    fn: asm_ast.Function, counter: int,
) -> tuple[asm_ast.Function, int]:
    """Shares the pair of runs that has the most instructions, or
    returns `fn` itself when there's none."""
    instrs = fn.instructions
    labels = {
        ins.name: k for k, ins in enumerate(instrs)
        if isinstance(ins, asm_ast.Label)
    }
    jumps = [
        (k, _destination(instrs, labels, ins))
        for k, ins in enumerate(instrs) if isinstance(ins, asm_ast.Jump)
    ]
    best = None
    for a, (i, dest_i) in enumerate(jumps):
        for j, dest_j in jumps[a + 1:]:
            if dest_i != dest_j:
                continue
            n = 0
            while (i - 1 - n >= 0 and j - 1 - n > i
                   and not isinstance(instrs[i - 1 - n], _STOPS)
                   and instrs[i - 1 - n] == instrs[j - 1 - n]):
                n += 1
            if n >= 2 and (best is None or n > best[2]):
                best = (i, j, n)
    if best is None:
        return fn, counter
    i, j, n = best
    start = i - n
    if start > 0 and isinstance(instrs[start - 1], asm_ast.Label):
        name = instrs[start - 1].name
        new_label = None
    else:
        name = f".xjump@{counter}"
        counter += 1
        new_label = start
    out: list[asm_ast.Type_instruction] = []
    for k, ins in enumerate(instrs):
        if k == new_label:
            out.append(asm_ast.Label(name=name))
        if j - n <= k < j:
            continue
        if k == j:
            ins = asm_ast.Jump(target=name)
        out.append(ins)
    return asm_ast.Function(
        name=fn.name, is_global=fn.is_global,
        params=list(fn.params), instructions=out,
    ), counter
