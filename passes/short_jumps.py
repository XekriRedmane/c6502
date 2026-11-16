"""Jumps made shorter once branches have their final reach: a jump to a
stub that only jumps to another function goes there itself, the stub
goes, and a `JMP` right after a load of a constant becomes a branch
when its label is in reach.

# Pattern

A switch's default is often a call that never returns, which
`tail_call` leaves as a stub `.default@N: JMP internal_error`. The
bounds check of a jump table branches there, and
`expand_long_branches` makes that `BCC skip; JMP .default@N` when
the stub is far away, as it is at the end of a long switch:

    CMP   #$10                       CMP   #$10
    BCC   .lb_skip@0                 BCC   .lb_skip@0
    JMP   .default@33       ->       JMP   internal_error
    ...                              ...
    .jtab@1:                         .jtab@1:
    DC.W  .default@33-1              DC.W  internal_error-1
    ...                              ...
    .default@33:
    JMP   internal_error

The `JMP`s and the table's entries for the default go to the function
the stub jumps to, and the stub, which nothing reaches any more, goes,
3 bytes.

`tac_to_asm` lowers a comparison kept as a value as `Bcc t; LDA #0;
JMP end; t: LDA #1; end:`. `LDA #0` sets Z, so `JMP end` is `BEQ end`
when `end` is within a branch's reach, 1 byte shorter. A load of a
constant other than 0 clears Z, so its `JMP` is `BNE`. The same goes
for `LDX` and `LDY`.

# Rules

- A `JMP .L`, with no register arguments, where the first instruction
  at `.L` and the labels with it is a `JMP f` to a name outside the
  function (a tail call) with none either, becomes `JMP f`. So does an
  entry `.L` of a jump table.
- A label that no branch, jump, jump table, or operand names any
  more, after an instruction that doesn't fall through, goes, and so
  does the code after it up to the next label.
- `LDA #k`, `LDX #k`, or `LDY #k` and then `JMP .L` to a label in the
  function: the `JMP` becomes `BEQ .L` for `k` of 0 and `BNE .L`
  otherwise, when the branch reaches `.L` with a margin of a byte
  each way. The displacements come from the code as it stands, and
  every rewrite here only removes bytes, so a branch that reaches its
  label still does after the others.

# Where to run

Once, at the end of the `--optimize` asm pipeline, right after
`expand_long_branches`, when the code's layout is final apart from
what this pass removes.
"""
from __future__ import annotations

from dataclasses import replace

import asm_ast
from sim.assembler import instruction_size

_TERMINATORS = (
    asm_ast.Jump, asm_ast.Return, asm_ast.Ret, asm_ast.JumpTable,
)


def apply_short_jumps(prog: asm_ast.Program) -> asm_ast.Program:
    """Shortens the jumps of every function, as the module docstring
    describes."""
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            instrs = _thread_to_tail_calls(tl.instructions)
            instrs = _drop_dead_stubs(instrs)
            instrs = _constant_jumps_to_branches(instrs)
            if instrs != tl.instructions:
                tl = replace(tl, instructions=instrs)
        new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _tail_call_at(instrs, labels, name) -> str | None:
    """The function a stub at label `name` jumps to, when the first
    instruction there is a `JMP` out of the function, else None."""
    k = labels.get(name)
    if k is None:
        return None
    while k < len(instrs) and isinstance(instrs[k], asm_ast.Label):
        k += 1
    if k < len(instrs):
        ins = instrs[k]
        if (isinstance(ins, asm_ast.Jump) and not ins.reg_args
                and not ins.target.startswith(".")):
            return ins.target
    return None


def _thread_to_tail_calls(instrs):
    labels = {
        ins.name: k for k, ins in enumerate(instrs)
        if isinstance(ins, asm_ast.Label)
    }
    out = []
    for ins in instrs:
        if (isinstance(ins, asm_ast.Jump) and not ins.reg_args
                and ins.target.startswith(".")):
            f = _tail_call_at(instrs, labels, ins.target)
            if f is not None:
                ins = asm_ast.Jump(target=f)
        elif isinstance(ins, asm_ast.JumpTable):
            targets = [
                _tail_call_at(instrs, labels, t) or t for t in ins.targets
            ]
            if targets != ins.targets:
                ins = replace(ins, targets=targets)
        out.append(ins)
    return out


def _referenced(instrs) -> set[str]:
    """Every label a branch, jump, jump table, or operand names."""
    out: set[str] = set()
    for ins in instrs:
        if isinstance(ins, (asm_ast.Jump, asm_ast.Branch)):
            out.add(ins.target)
        elif isinstance(ins, asm_ast.JumpTable):
            out.update(ins.targets)
        for v in vars(ins).values():
            name = getattr(v, "name", None)
            if isinstance(name, str):
                out.add(name)
    return out


def _drop_dead_stubs(instrs):
    used = _referenced(instrs)
    out = []
    dropping = False
    for ins in instrs:
        if isinstance(ins, asm_ast.Label):
            dead = (ins.name not in used and out
                    and (dropping or isinstance(out[-1], _TERMINATORS)))
            if dead:
                dropping = True
                continue
            dropping = False
        if dropping:
            continue
        out.append(ins)
    return out


def _constant_jumps_to_branches(instrs):
    labels: dict[str, int] = {}
    addrs: list[int] = []
    addr = 0
    for ins in instrs:
        addrs.append(addr)
        if isinstance(ins, asm_ast.Label):
            labels[ins.name] = addr
        else:
            addr += instruction_size(ins)
    out = list(instrs)
    for k in range(1, len(instrs)):
        load, jump = instrs[k - 1], instrs[k]
        if not (isinstance(jump, asm_ast.Jump) and not jump.reg_args
                and jump.target in labels
                and isinstance(load, asm_ast.Mov)
                and isinstance(load.src, asm_ast.Imm)
                and isinstance(load.dst, asm_ast.Reg)):
            continue
        # The displacement as a 2-byte branch here, a byte to spare
        # each way for the byte it saves.
        disp = labels[jump.target] - (addrs[k] + 2)
        if not -127 <= disp <= 126:
            continue
        cond = asm_ast.EQ() if load.src.value & 0xFF == 0 else asm_ast.NE()
        out[k] = asm_ast.Branch(cond=cond, target=jump.target)
    return out
