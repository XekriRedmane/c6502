"""Expand over-long branches into inverted-branch + JMP pairs.

The 6502's `Bxx` family (`BCC`/`BCS`/`BEQ`/`BMI`/`BNE`/`BPL`/`BVC`/
`BVS`) carries a signed 8-bit displacement, so a branch can only
reach a target within `-128..+127` bytes of the instruction
following it. Functions large enough to push a label past that
window from a branch site can't encode the original branch in one
instruction.

This pass walks each function's instruction list, computes label
addresses, identifies over-long branches, and rewrites each one
into a 5-byte sequence:

    Branch(cond, target)
        becomes
    Branch(inverted_cond, .lb_skip@N)
    Jump(target)
    Label(.lb_skip@N)

The inverted-branch skips over the 3-byte JMP when the original
condition is *false* (so falling through to the JMP corresponds to
the original taken-branch case). `JMP` is absolute, 3 bytes, and
can reach anywhere in 64KB.

Iteration. Each expansion grows the function by 3 bytes, which
might push other still-short branches over their windows. So the
pass iterates per-function until no more expansions are needed.
Termination is guaranteed because the function only grows; once a
branch is expanded it stays expanded.

Per-function. Branches always target labels within the same
function — the codegen never crosses function boundaries with a
`Bxx` (cross-function transfers go through `JSR` / `RTS` / `JMP`).
The pass takes advantage of that and works on each `Function` in
isolation; cross-function symbol references in `Branch.target`
would raise.

Where to run. After `replace_pseudoregisters` (so all `Pseudo`
operands are resolved to `Frame` / `Stack` / `Data` and per-instr
sizes are well-defined) and before either `asm_emit.emit_program`
or `sim.assembler.assemble`. Both consumers see the same expanded
program; the pass does the heavy lifting once.

The minted skip-labels follow the dasm-local-label convention used
elsewhere (`.lb_skip@<N>`, where `N` is a global counter). The `@`
keeps them disjoint from user labels (`.<funcname>@<orig>`) and
from the rest of the translator-minted family (`.if_end@<N>` etc.).
"""

from __future__ import annotations

from dataclasses import replace

import asm_ast
from sim.assembler import instruction_size


# Inverted-condition map. Each pair flips the sense of a Bxx —
# BCC ↔ BCS, BEQ ↔ BNE, BMI ↔ BPL, BVC ↔ BVS — so the new branch
# falls through to the JMP when the original would have taken.
_INVERTED: dict[type, type] = {
    asm_ast.CC: asm_ast.CS,
    asm_ast.CS: asm_ast.CC,
    asm_ast.EQ: asm_ast.NE,
    asm_ast.NE: asm_ast.EQ,
    asm_ast.MI: asm_ast.PL,
    asm_ast.PL: asm_ast.MI,
    asm_ast.VC: asm_ast.VS,
    asm_ast.VS: asm_ast.VC,
}


def _invert(cond: asm_ast.Type_condition) -> asm_ast.Type_condition:
    return _INVERTED[type(cond)]()


def expand_program(prog: asm_ast.Program) -> asm_ast.Program:
    """Return a new `asm_ast.Program` with no over-long branches.
    `StaticVariable` top-levels pass through unchanged; each
    `Function` is rewritten in-place (a fresh dataclass copy with
    the expanded instruction list)."""
    counter = 0
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            new_instrs, counter = _expand_function(tl.instructions, counter)
            new_top.append(replace(tl, instructions=new_instrs))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _expand_function(
    instrs: list[asm_ast.Type_instruction], counter: int,
) -> tuple[list[asm_ast.Type_instruction], int]:
    """Iterate to fixed point, expanding any branch whose target is
    more than 127 bytes from the instruction following the branch.
    Returns the expanded instruction list and the updated label
    counter."""
    while True:
        label_addr = _label_addresses(instrs)
        too_far = _find_too_far_branches(instrs, label_addr)
        if not too_far:
            return instrs, counter
        instrs, counter = _expand_indices(instrs, too_far, counter)


def _label_addresses(
    instrs: list[asm_ast.Type_instruction],
) -> dict[str, int]:
    """Map each `Label.name` in `instrs` to its byte offset within
    the function. The function's start is offset 0; each non-Label
    instruction advances the cursor by its `instruction_size`."""
    addr_map: dict[str, int] = {}
    addr = 0
    for instr in instrs:
        if isinstance(instr, asm_ast.Label):
            addr_map[instr.name] = addr
        else:
            addr += instruction_size(instr)
    return addr_map


def _find_too_far_branches(
    instrs: list[asm_ast.Type_instruction],
    label_addr: dict[str, int],
) -> list[int]:
    """Indices of `Branch` instructions in `instrs` whose displacement
    to their target exceeds the 8-bit signed range. The 6502's PC
    has already advanced past the 2-byte branch when the displacement
    is computed, so `disp = target - (branch_addr + 2)`."""
    out: list[int] = []
    addr = 0
    for i, instr in enumerate(instrs):
        if isinstance(instr, asm_ast.Branch):
            if instr.target not in label_addr:
                raise ValueError(
                    f"Branch target {instr.target!r} not found in this "
                    "function — branches must stay within their "
                    "containing function"
                )
            disp = label_addr[instr.target] - (addr + 2)
            if not -128 <= disp <= 127:
                out.append(i)
        if not isinstance(instr, asm_ast.Label):
            addr += instruction_size(instr)
    return out


def _expand_indices(
    instrs: list[asm_ast.Type_instruction],
    too_far: list[int],
    counter: int,
) -> tuple[list[asm_ast.Type_instruction], int]:
    """Replace each `Branch` at the given indices with the long-branch
    triple. Process from the back so earlier indices stay valid as
    later ones are spliced in. Each expansion mints a fresh
    `.lb_skip@<N>` label."""
    new = list(instrs)
    for i in reversed(too_far):
        br = new[i]
        skip_label = f".lb_skip@{counter}"
        counter += 1
        new[i:i + 1] = [
            asm_ast.Branch(cond=_invert(br.cond), target=skip_label),
            asm_ast.Jump(target=br.target),
            asm_ast.Label(name=skip_label),
        ]
    return new, counter
