"""Helpers for comparing hand-written 6502 routines with c6502's output.

A comparison test assembles the original routine with py65's line
assembler, runs it and the compiled function on the same inputs, and
times each from entry to its RTS. `count_function_instructions` gives
the compiled function's static instruction count from
`compile.py --codegen --optimize` output."""

from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path

from py65.assembler import Assembler
from py65.devices.mpu6502 import MPU

REPO = Path(__file__).parent.parent
RETURN_ADDR = 0x0F00     # a BRK the routine under test returns to


def assemble(mpu: MPU, lines: list[str], origin: int) -> None:
    """Assembles `lines` into `mpu.memory` at `origin`. A line may
    start with `label:`; later references to the label are replaced
    by its address. Two passes, so forward references work."""
    asm = Assembler(mpu)
    labels: dict[str, int] = {}
    names = [l.split(":", 1)[0].strip() for l in lines if ":" in l]
    for pass_ in range(2):
        pc = origin
        for line in lines:
            if ":" in line:
                label, line = (part.strip() for part in line.split(":", 1))
                labels[label] = pc
            for name in names:
                line = re.sub(rf"\b{re.escape(name)}\b",
                              f"${labels.get(name, pc):04X}", line)
            code = asm.assemble(line, pc)
            if code is None:
                raise ValueError(f"py65 can't assemble {line!r}")
            if pass_ == 1:
                mpu.memory[pc:pc + len(code)] = code
            pc += len(code)


def call(mpu: MPU, entry: int) -> int:
    """Runs the subroutine at `entry` until it returns; returns the
    cycles it took, including its RTS."""
    mpu.memory[RETURN_ADDR] = 0x00
    mpu.memory[0x01FF] = ((RETURN_ADDR - 1) >> 8) & 0xFF
    mpu.memory[0x01FE] = (RETURN_ADDR - 1) & 0xFF
    mpu.sp = 0xFD
    mpu.pc = entry
    start = mpu.processorCycles
    while mpu.pc != RETURN_ADDR:
        mpu.step()
    return mpu.processorCycles - start


def count_function_instructions(c_path: Path, fn: str) -> int:
    """Instructions in `fn`'s body in `compile.py --codegen
    --optimize` output (labels, comments, and directives excluded)."""
    asm_text = subprocess.run(
        [sys.executable, "compile.py", str(c_path), "--codegen", "--optimize"],
        capture_output=True, text=True, check=True, cwd=REPO,
    ).stdout
    body = asm_text.split(f"\n{fn}:")[1]
    end = re.search(r"\n[A-Za-z_]\w*:", body)
    if end:
        body = body[:end.start()]
    return sum(
        1 for line in body.splitlines()
        if re.match(r"^\s+[A-Z]{3}\b", line) and "SUBROUTINE" not in line
    )
