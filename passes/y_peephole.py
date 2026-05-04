"""Post-emit Y-register tracking peephole.

The asm emit phase synthesizes `LDY #<off>` whenever it lowers an
indirect-Y addressing mode (Frame, Stack, Indirect operands) — one
per access. Two consecutive accesses with the same offset emit two
identical LDYs, the second of which is redundant. Two with adjacent
offsets emit two LDYs where the second could be a shorter `INY` /
`DEY`.

This pass walks the emitted text lines, tracks Y's known value
across instructions, and rewrites:

    * `LDY #<imm>` when Y is already known to equal `imm` → drop.
    * `LDY #<imm>` when Y is known to equal `(imm - 1) & 0xFF`
      → replace with `INY` (1 byte saved).
    * `LDY #<imm>` when Y is known to equal `(imm + 1) & 0xFF`
      → replace with `DEY` (1 byte saved).

State invalidation. Y's value is set unknown at:

    * Any `Label` line (basic-block boundary; a label could be
      reached from any predecessor).
    * `JSR` instructions (called functions are free to clobber Y;
      runtime helpers do).
    * Other Y-clobbering ops we don't model: `TAY`, `PHP`/`PLP` are
      irrelevant (don't touch Y), `PLA` doesn't touch Y. The only
      ones that matter are `INY`, `DEY`, `TAY`, `LDY`, `JSR`,
      label.

Branches and unconditional jumps don't modify Y themselves, so
fall-through state is preserved. Branch targets are reached via
their own `Label`, where Y resets — so the branched-to side doesn't
inherit state.

Pure textual peephole — operates on the line list `emit_program`
produces, parsing each line just enough to recognize LDY/INY/DEY/
TAY/JSR/label. Conservative: anything we can't classify leaves Y
alone (since most asm ops don't touch Y).
"""

from __future__ import annotations


def apply_y_peephole(lines: list[str]) -> list[str]:
    """Walk `lines` and rewrite redundant LDYs. Returns a new list.

    The peephole's state is single-block local: every label resets
    Y to unknown. Calls (JSR) also reset. Within a block the rules
    are mechanical.
    """
    out: list[str] = []
    y_value: int | None = None
    for line in lines:
        kind = _classify(line)
        if kind is _LABEL:
            y_value = None
            out.append(line)
            continue
        if kind is _BLANK or kind is _COMMENT or kind is _DIRECTIVE:
            out.append(line)
            continue
        if kind is _LDY_IMM:
            imm = _ldy_imm_value(line)
            if y_value is not None and imm == y_value:
                # Redundant — Y already holds the desired value.
                continue
            if y_value is not None and imm == (y_value + 1) & 0xFF:
                out.append("   INY")
                y_value = imm
                continue
            if y_value is not None and imm == (y_value - 1) & 0xFF:
                out.append("   DEY")
                y_value = imm
                continue
            out.append(line)
            y_value = imm
            continue
        if kind is _INY:
            out.append(line)
            if y_value is not None:
                y_value = (y_value + 1) & 0xFF
            continue
        if kind is _DEY:
            out.append(line)
            if y_value is not None:
                y_value = (y_value - 1) & 0xFF
            continue
        if kind is _Y_CLOBBER:
            # TAY, JSR, and any other recognized Y-clobbering op
            # invalidate the tracker.
            out.append(line)
            y_value = None
            continue
        # _OTHER: instruction we don't model; assume it doesn't
        # touch Y.
        out.append(line)
    return out


# ---------------------------------------------------------------
# Line classification.
# ---------------------------------------------------------------

_BLANK = object()
_COMMENT = object()
_LABEL = object()
_DIRECTIVE = object()
_LDY_IMM = object()
_INY = object()
_DEY = object()
_Y_CLOBBER = object()
_OTHER = object()


def _classify(line: str) -> object:
    """Return one of the marker objects above describing what `line`
    does to Y. Recognizes only the lines we care about; everything
    else is `_OTHER` (treated as Y-preserving)."""
    if line == "":
        return _BLANK
    stripped = line.strip()
    if not stripped:
        return _BLANK
    if stripped.startswith(";"):
        return _COMMENT
    if stripped.endswith(":"):
        return _LABEL
    # Instruction line: leading whitespace + opcode + (operand?).
    tokens = stripped.split(None, 1)
    if not tokens:
        return _BLANK
    opcode = tokens[0].upper()
    if opcode in _DIRECTIVES:
        return _DIRECTIVE
    if opcode == "LDY":
        operand = tokens[1] if len(tokens) > 1 else ""
        if operand.startswith("#"):
            return _LDY_IMM
        # LDY <addr> reads memory into Y; we don't statically know
        # the loaded value. Treat as Y-clobber.
        return _Y_CLOBBER
    if opcode == "INY":
        return _INY
    if opcode == "DEY":
        return _DEY
    if opcode in _Y_CLOBBERING_OPCODES:
        return _Y_CLOBBER
    return _OTHER


def _ldy_imm_value(line: str) -> int:
    """Parse `LDY #$XX` (hex) or `LDY #NN` (decimal) and return the
    immediate. Caller must have already classified the line as
    `_LDY_IMM`."""
    stripped = line.strip()
    operand = stripped.split(None, 1)[1].strip()
    assert operand.startswith("#")
    body = operand[1:]
    if body.startswith("$"):
        return int(body[1:], 16)
    return int(body, 10)


_DIRECTIVES = frozenset({
    "SUBROUTINE", "DC.B", "DC.W", "DC.L", "DS.B", "DS.W", "DS.L",
    "EQU", "ORG", "PROCESSOR",
})


# Ops that clobber Y. JSR is the big one — callees may use Y. TAY
# overwrites Y from A.
_Y_CLOBBERING_OPCODES = frozenset({
    "JSR",
    "TAY",
})
