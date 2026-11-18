"""Helpers for asm `DecimalArith`, the packed BCD add or subtract.

`DecimalArith(subtract, left, right, dst)` is one asm instruction for
the whole `SED; CLC; LDA left[0]; ADC right[0]; STA dst[0]; ...; CLD`
chain (asm.asdl). Keeping the chain in one instruction until
`asm_to_asm2` expands it is what confines the decimal flag. No pass
sees the SED or the CLD, so no pass can put an instruction between
them, and no pass can move the chain's ADC or SBC out from between
them. The asm2 check in `passes/ir_check.py` checks the expansion.

Each pass still has to know which operands the instruction reads and
writes, and this module gives them in one place:

  * `reads` are the bytes of `left` and `right`, and `writes` are the
    bytes of `dst`. A and the flags N, Z, C, and V are clobbered, and
    nothing in A or a flag is read.
  * `writes_y` is True when an operand's access begins with an
    `LDY #off` (a soft-stack slot), which clobbers Y. X is never
    touched.
  * `map_operands` rebuilds the instruction with each operand passed
    through a function, for the passes that rename or resolve
    operands.

Every operand is one of `SOURCE_FORMS` (for `left` and `right`) or
`DEST_FORMS` (for `dst`). Registers are excluded, because `ADC`
can't read X or Y, and so are the indexed and indirect forms, so
that no address computation is needed inside the chain.
"""
from __future__ import annotations

from collections.abc import Callable

import asm_ast

# Operand forms `left` and `right` may take. `Pseudo` is only present
# before `replace_pseudoregisters`.
SOURCE_FORMS = (
    asm_ast.Imm, asm_ast.ImmLabelLow, asm_ast.ImmLabelHigh,
    asm_ast.Pseudo, asm_ast.Data, asm_ast.ZP,
    asm_ast.Frame, asm_ast.Stack,
)

# Operand forms `dst` may take.
DEST_FORMS = (
    asm_ast.Pseudo, asm_ast.Data, asm_ast.ZP,
    asm_ast.Frame, asm_ast.Stack,
)

# Forms whose access starts with `LDY #off`.
_Y_SETUP_FORMS = (asm_ast.Frame, asm_ast.Stack)


def reads(instr: asm_ast.DecimalArith) -> list[asm_ast.Type_operand]:
    """The operands `instr` reads, `left` then `right`."""
    return [*instr.left, *instr.right]


def writes(instr: asm_ast.DecimalArith) -> list[asm_ast.Type_operand]:
    """The operands `instr` writes."""
    return list(instr.dst)


def operands(instr: asm_ast.DecimalArith) -> list[asm_ast.Type_operand]:
    """Every operand of `instr`, read or written."""
    return [*instr.left, *instr.right, *instr.dst]


def pseudo_reads(instr: asm_ast.DecimalArith) -> list[asm_ast.Pseudo]:
    return [op for op in reads(instr) if isinstance(op, asm_ast.Pseudo)]


def pseudo_writes(instr: asm_ast.DecimalArith) -> list[asm_ast.Pseudo]:
    return [op for op in writes(instr) if isinstance(op, asm_ast.Pseudo)]


def writes_y(instr: asm_ast.DecimalArith) -> bool:
    """True when the expansion loads Y to address a soft-stack slot."""
    return any(isinstance(op, _Y_SETUP_FORMS) for op in operands(instr))


def map_operands(
    instr: asm_ast.DecimalArith,
    fn: Callable[[asm_ast.Type_operand], asm_ast.Type_operand],
) -> asm_ast.DecimalArith:
    """`instr` with every operand replaced by `fn(operand)`."""
    return asm_ast.DecimalArith(
        subtract=instr.subtract,
        left=[fn(op) for op in instr.left],
        right=[fn(op) for op in instr.right],
        dst=[fn(op) for op in instr.dst],
    )


def map_reads(
    instr: asm_ast.DecimalArith,
    fn: Callable[[asm_ast.Type_operand], asm_ast.Type_operand],
) -> asm_ast.DecimalArith:
    """`instr` with every operand it reads replaced by `fn(operand)`
    and its `dst` left as it is."""
    return asm_ast.DecimalArith(
        subtract=instr.subtract,
        left=[fn(op) for op in instr.left],
        right=[fn(op) for op in instr.right],
        dst=list(instr.dst),
    )


def map_writes(
    instr: asm_ast.DecimalArith,
    fn: Callable[[asm_ast.Type_operand], asm_ast.Type_operand],
) -> asm_ast.DecimalArith:
    """`instr` with every `dst` operand replaced by `fn(operand)`."""
    return asm_ast.DecimalArith(
        subtract=instr.subtract,
        left=list(instr.left),
        right=list(instr.right),
        dst=[fn(op) for op in instr.dst],
    )


def later_sources(
    instr: asm_ast.DecimalArith, k: int,
) -> list[asm_ast.Type_operand]:
    """The source bytes read after `dst[k]` is written, which are
    `left[j]` and `right[j]` for j > k. `dst[k]` must not share a byte
    with any of them."""
    return [*instr.left[k + 1:], *instr.right[k + 1:]]


def early_write_pairs(
    instr: asm_ast.DecimalArith,
) -> list[tuple[str, str]]:
    """The `(dst, source)` Pseudo name pairs that must not share a
    byte, because `dst[k]` is written before the source byte (of
    index j > k) is read. Liveness after the instruction doesn't show
    these, so the interference graphs add them."""
    out: list[tuple[str, str]] = []
    for k, d in enumerate(instr.dst):
        if not isinstance(d, asm_ast.Pseudo):
            continue
        for s in later_sources(instr, k):
            if isinstance(s, asm_ast.Pseudo) and s.name != d.name:
                out.append((d.name, s.name))
    return out


def form_problems(instr: asm_ast.DecimalArith) -> list[str]:
    """What's wrong with `instr`'s shape, for the IR check: lists of
    different lengths, an empty chain, or an operand of a form the
    expansion can't take."""
    problems: list[str] = []
    n = len(instr.dst)
    if n == 0 or len(instr.left) != n or len(instr.right) != n:
        problems.append(
            f"DecimalArith has {len(instr.left)} left, {len(instr.right)} "
            f"right, and {n} dst bytes"
        )
    for op in reads(instr):
        if not isinstance(op, SOURCE_FORMS):
            problems.append(f"DecimalArith reads {type(op).__name__}")
    for op in writes(instr):
        if not isinstance(op, DEST_FORMS):
            problems.append(f"DecimalArith writes {type(op).__name__}")
    return problems
