"""Moves a loop's test from its top to its bottom.

# Motivating shape

A `for` or `while` loop tests at the top and ends with a `JMP` back
to the test:

    L:      <test>          ; ends in Bcc EXIT
    B:      <body>
            JMP L
    EXIT:

Each pass runs the `JMP` (3 cycles) and the test's branch not taken
(2 cycles). With the test at the bottom, the loop is entered with a
`JMP` to it, and the test's branch, inverted, goes back to the body:

            JMP L           ; only when L is reached by falling in
    B:      <body>
    L:      <test>          ; ends in B!cc B
    EXIT:

The code is the same size, and each pass runs the branch taken (3
cycles) and no `JMP`, 2 cycles fewer. Entering costs the `JMP`, 3
cycles, and leaving saves 1, since the test's branch falls through
to EXIT where it used to be taken. A loop that runs `n` times saves
`2n - 2` cycles.

When the test is known to pass the first time, the entry `JMP` goes
too, 3 bytes and 3 cycles, and the loop is entered at the body. That
is the case of `for (uint8_t i = 0; i != 6; i++)`, whose test is
`CPX #$06; BEQ` right after `LDX #$00`. `_entry_passes` evaluates the
test on the constants that the straight-line code in front of the
loop leaves in the registers and in memory.

# Eligibility

- `JMP L` is followed, after any labels, by the label EXIT, and `L`
  is a label earlier in the function.
- The instructions from `L` up to the first branch, the test, contain
  no label, jump, or return, and that branch is a conditional one to
  EXIT.
- The backward branch from the end of the test to the body fits in
  the 8-bit range, with a margin, since `expand_long_branches` would
  otherwise turn it into a branch around a `JMP`, 3 bytes more.

Other jumps and branches to `L`, such as a `continue`, still go to
`L`, which moves with the test.

# Where to run

After the peephole fixed points and `loop_invariant_a`, which
expects the loop's test at its top, and before
`expand_long_branches`. Only under `-O`.
"""
from __future__ import annotations

from dataclasses import replace
from typing import Optional

import asm_ast
from passes.asm_aliasing import may_alias
from passes.asm_liveness import sets_up_y
from passes.long_branches import _invert
from sim.assembler import instruction_size

# The longest body and test, in bytes, that the backward branch is
# expected to reach. The 6502's branch reaches 128 bytes back, and
# the margin covers long branches that `expand_long_branches` may add
# inside the body.
_MAX_REACH = 112


def apply_rotate_loop_test(prog: asm_ast.Program) -> asm_ast.Program:
    """Moves each eligible loop's test to its bottom."""
    counter = [0]
    new_top: list[asm_ast.Type_top_level] = []
    for tl in prog.top_level:
        if isinstance(tl, asm_ast.Function):
            instrs = list(tl.instructions)
            while True:
                rotated = _rotate_one(instrs, tl.name, counter)
                if rotated is None:
                    break
                instrs = rotated
            new_top.append(replace(tl, instructions=instrs))
        else:
            new_top.append(tl)
    return asm_ast.Program(top_level=new_top)


def _rotate_one(
    instrs: list[asm_ast.Type_instruction], fn_name: str, counter: list[int],
) -> Optional[list[asm_ast.Type_instruction]]:
    """`instrs` with the first eligible loop rotated, or None when no
    loop is eligible."""
    label_at = {
        ins.name: i for i, ins in enumerate(instrs)
        if isinstance(ins, asm_ast.Label)
    }
    for j, jump in enumerate(instrs):
        if not (isinstance(jump, asm_ast.Jump) and not jump.reg_args):
            continue
        l = label_at.get(jump.target)
        if l is None or l >= j:
            continue
        after = _labels_from(instrs, j + 1)
        test_end = _test_end(instrs, l, j)
        if test_end is None:
            continue
        branch = instrs[test_end]
        if branch.target not in after:
            continue
        test = instrs[l + 1:test_end + 1]
        body = instrs[test_end + 1:j]
        if sum(map(_size, test + body)) > _MAX_REACH:
            continue
        # The body's first label, or a new one.
        if body and isinstance(body[0], asm_ast.Label):
            body_label = body[0].name
            head: list[asm_ast.Type_instruction] = []
        else:
            body_label = f".{fn_name}@rot@{counter[0]}"
            counter[0] += 1
            head = [asm_ast.Label(name=body_label)]
        # A label at the function's start is where it's entered.
        falls_in = l == 0 or not isinstance(
            instrs[l - 1], (asm_ast.Jump, asm_ast.Return, asm_ast.Ret),
        )
        entry: list[asm_ast.Type_instruction] = []
        if falls_in and not _entry_passes(instrs, l, test):
            entry = [asm_ast.Jump(target=jump.target)]
        bottom_test = [
            *test[:-1],
            asm_ast.Branch(cond=_invert(branch.cond), target=body_label),
        ]
        out = [
            *instrs[:l], *entry, *head, *body,
            instrs[l], *bottom_test, *instrs[j + 1:],
        ]
        # The loop's label stays where the test went, for the other
        # jumps to it, and goes when nothing jumps to it.
        if not any(
            isinstance(i, (asm_ast.Jump, asm_ast.Branch))
            and i.target == jump.target for i in out
        ):
            out = [
                i for i in out
                if not (isinstance(i, asm_ast.Label) and i.name == jump.target)
            ]
        return out
    return None


def _labels_from(instrs, k: int) -> set[str]:
    """The names of the labels in the run of labels at `instrs[k:]`."""
    out: set[str] = set()
    while k < len(instrs) and isinstance(instrs[k], asm_ast.Label):
        out.add(instrs[k].name)
        k += 1
    return out


def _test_end(instrs, l: int, j: int) -> Optional[int]:
    """The index of the conditional branch that ends the test starting
    after the label at `l`, when the test has no label, jump, or
    return before it and ends before `j`."""
    for k in range(l + 1, j):
        ins = instrs[k]
        if isinstance(ins, asm_ast.Branch):
            return k
        if isinstance(ins, (
            asm_ast.Label, asm_ast.Jump, asm_ast.Return, asm_ast.Ret,
            asm_ast.FunctionPrologue, asm_ast.AllocateStack,
        )):
            return None
    return None


def _size(ins: asm_ast.Type_instruction) -> int:
    return 0 if isinstance(ins, asm_ast.Label) else instruction_size(ins)


# ---- The test on entry ---------------------------------------------------

def _entry_passes(instrs, l: int, test) -> bool:
    """True when the straight-line code that falls into the label at
    `l` leaves values for which `test`'s branch isn't taken, so the
    first pass of the loop needs no test. Unknown values, and any
    instruction the evaluation doesn't model, give False."""
    start = l
    while start > 0 and not isinstance(instrs[start - 1], (
        asm_ast.Label, asm_ast.Jump, asm_ast.Branch, asm_ast.Return,
        asm_ast.Ret,
    )):
        start -= 1
    state = _State()
    for ins in instrs[start:l]:
        state.step(ins)
    for ins in test[:-1]:
        state.step(ins)
    taken = state.branch_taken(test[-1].cond)
    return taken is False


def _reg_name(op) -> Optional[str]:
    if isinstance(op, asm_ast.Reg):
        return type(op.reg).__name__
    return None


def _is_memory(op) -> bool:
    return isinstance(op, (asm_ast.Data, asm_ast.ZP))


class _State:
    """Known byte values of A, X, Y, and memory cells, and the N, Z,
    and C flags, through straight-line code. None is unknown."""

    def __init__(self) -> None:
        self.regs: dict[str, Optional[int]] = {"A": None, "X": None, "Y": None}
        self.mem: list[tuple[asm_ast.Type_operand, int]] = []
        self.n: Optional[bool] = None
        self.z: Optional[bool] = None
        self.c: Optional[bool] = None

    def _value(self, op) -> Optional[int]:
        if isinstance(op, asm_ast.Imm):
            return op.value & 0xFF
        r = _reg_name(op)
        if r is not None:
            return self.regs.get(r)
        if _is_memory(op):
            for cell, v in self.mem:
                if cell == op:
                    return v
        return None

    def _set_nz(self, v: Optional[int]) -> None:
        self.n = None if v is None else bool(v & 0x80)
        self.z = None if v is None else v == 0

    def _write(self, op, v: Optional[int]) -> None:
        r = _reg_name(op)
        if r is not None:
            self.regs[r] = v
            return
        if _is_memory(op):
            self.mem = [(c, x) for c, x in self.mem if not may_alias(c, op)]
            if v is not None:
                self.mem.append((op, v))
            return
        # A write through a pointer or an index may change any cell.
        self.mem = []

    def _forget(self) -> None:
        self.regs = {"A": None, "X": None, "Y": None}
        self.mem = []
        self.n = self.z = self.c = None

    def step(self, ins) -> None:
        if sets_up_y(ins):
            self.regs["Y"] = None
        if isinstance(ins, asm_ast.Mov):
            if ins.is_volatile:
                self._forget()
                return
            v = self._value(ins.src)
            src_reg, dst_reg = _reg_name(ins.src), _reg_name(ins.dst)
            if src_reg is None and dst_reg is None:
                # Memory to memory is LDA src; STA dst.
                self.regs["A"] = v
            self._write(ins.dst, v)
            # A load or a transfer sets N and Z. A store doesn't.
            if not (src_reg is not None and dst_reg is None):
                self._set_nz(v)
            return
        if isinstance(ins, asm_ast.Compare):
            a, b = self._value(ins.left), self._value(ins.right)
            if a is None or b is None:
                self.n = self.z = self.c = None
            else:
                self.c = a >= b
                self.z = a == b
                self.n = bool((a - b) & 0x80)
            return
        if isinstance(ins, (asm_ast.Inc, asm_ast.Dec)):
            v = self._value(ins.dst)
            if v is not None:
                v = (v + (1 if isinstance(ins, asm_ast.Inc) else -1)) & 0xFF
            self._write(ins.dst, v)
            self._set_nz(v)
            return
        if isinstance(ins, (asm_ast.SetCarry, asm_ast.ClearCarry)):
            self.c = isinstance(ins, asm_ast.SetCarry)
            return
        if isinstance(ins, asm_ast.Label):
            return
        # Anything else: forget what it may have changed.
        self._forget()

    def branch_taken(self, cond) -> Optional[bool]:
        flag = {
            asm_ast.EQ: (self.z, True), asm_ast.NE: (self.z, False),
            asm_ast.CS: (self.c, True), asm_ast.CC: (self.c, False),
            asm_ast.MI: (self.n, True), asm_ast.PL: (self.n, False),
        }.get(type(cond))
        if flag is None or flag[0] is None:
            return None
        return flag[0] == flag[1]
