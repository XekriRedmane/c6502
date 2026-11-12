"""Runs a linked program (the text `compile.py --link` writes) in the
simulator, by assembling it with dasm.

The in-process assembler (`sim/assembler.py`) works on the asm IR, and
a linked program only exists as text, so `build_linked_sim` writes the
text out with an `ORG` and the runtime's helper addresses, runs dasm
on it, and loads the binary and the symbol table into the same
simulator `build_sim` builds. `DASM` is None when dasm isn't installed,
and tests that need it are skipped.
"""
from __future__ import annotations

import re
import shutil
import subprocess
import tempfile
from pathlib import Path

from py65.devices.mpu6502 import MPU

import sim.runtime as rt_mod
from sim.harness import DEFAULT_ORIGIN, Simulation

DASM = shutil.which("dasm")

_EQU = re.compile(r"^(\S+)\tEQU\t\$([0-9A-Fa-f]+)$", re.M)


def build_linked_sim(text: str, origin: int = DEFAULT_ORIGIN) -> Simulation:
    """Assembles the linked program `text` with dasm and returns a
    simulator ready to run it from the boot stub, which calls
    `__zp_init` (when the program has one) and then `main`."""
    if DASM is None:
        raise RuntimeError("dasm isn't installed")
    from passes.runtime_layout import CLASSIC, RuntimeLayout
    equs = {m.group(1): int(m.group(2), 16) for m in _EQU.finditer(text)}
    layout = RuntimeLayout(
        ssp=equs.get("SSP", CLASSIC.ssp),
        fp=equs.get("FP", CLASSIC.fp),
        hargs=equs.get("HARGS", CLASSIC.hargs),
        dptr=equs.get("DPTR", CLASSIC.dptr),
    )
    runtime = rt_mod.build_runtime(layout)
    helpers = [
        f"{name}\tEQU\t${addr:04X}"
        for name, addr in sorted(runtime.symbols.items())
        if name not in equs
    ]
    source = "\n".join([
        "\tPROCESSOR 6502", *helpers, f"\tORG ${origin:04X}", text,
    ])
    with tempfile.TemporaryDirectory() as tmp:
        src = Path(tmp) / "linked.asm"
        out = Path(tmp) / "linked.bin"
        sym = Path(tmp) / "linked.sym"
        src.write_text(source)
        done = subprocess.run(
            [DASM, str(src), "-f3", f"-o{out}", f"-s{sym}"],
            capture_output=True, text=True,
        )
        if done.returncode != 0:
            raise RuntimeError(f"dasm failed:\n{done.stdout}{done.stderr}")
        binary = out.read_bytes()
        symbols = _read_symbols(sym.read_text())
    image = bytearray(0x10000)
    image[origin:origin + len(binary)] = binary
    rt_mod.install_runtime(
        image, runtime, symbols["main"],
        init_addr=symbols.get("__zp_init"),
    )
    mpu = MPU()
    mpu.memory = image
    mpu.pc = runtime.boot_addr
    mpu.processorCycles = 0
    return Simulation(
        mpu=mpu, symbols=symbols, runtime=runtime, origin=origin,
        code_end=origin + len(binary),
    )


def _read_symbols(text: str) -> dict[str, int]:
    """dasm's symbol file: one `name  value  flags` line per symbol."""
    out: dict[str, int] = {}
    for line in text.splitlines():
        parts = line.split()
        if len(parts) >= 2 and re.fullmatch(r"[0-9a-fA-F]+", parts[1]):
            out[parts[0]] = int(parts[1], 16)
    return out
