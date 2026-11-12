"""Runs a linked program (the text `compile.py --link` writes) in the
simulator, by assembling it with dasm.

The in-process assembler (`sim/assembler.py`) works on the asm IR, and
a linked program only exists as text, so `build_linked_sim` writes the
text out with the runtime's helper addresses (and an `ORG`, unless a
linker script already placed it), runs dasm on it, and loads each
segment of the binary at its address, and the symbol table, into the
same simulator `build_sim` builds. `DASM` is None when dasm isn't installed,
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
    placed = re.search(r"^\s+ORG\s", text, re.M) is not None
    source = "\n".join([
        "\tPROCESSOR 6502", *helpers,
        *([] if placed else [f"\tORG ${origin:04X}"]), text,
    ])
    with tempfile.TemporaryDirectory() as tmp:
        src = Path(tmp) / "linked.asm"
        out = Path(tmp) / "linked.bin"
        sym = Path(tmp) / "linked.sym"
        src.write_text(source)
        done = subprocess.run(
            [DASM, str(src), "-f2", f"-o{out}", f"-s{sym}"],
            capture_output=True, text=True,
        )
        if done.returncode != 0:
            raise RuntimeError(f"dasm failed:\n{done.stdout}{done.stderr}")
        binary = out.read_bytes()
        symbols = _read_symbols(sym.read_text())
    image = bytearray(0x10000)
    end = origin
    pos = 0
    # dasm's -f2 format: each segment is its address and its length,
    # two little-endian bytes each, then its bytes.
    while pos + 4 <= len(binary):
        addr = binary[pos] | binary[pos + 1] << 8
        size = binary[pos + 2] | binary[pos + 3] << 8
        image[addr:addr + size] = binary[pos + 4:pos + 4 + size]
        pos += 4 + size
        end = max(end, addr + size)
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
        code_end=end,
    )


def _read_symbols(text: str) -> dict[str, int]:
    """dasm's symbol file: one `name  value  flags` line per symbol."""
    out: dict[str, int] = {}
    for line in text.splitlines():
        parts = line.split()
        if len(parts) >= 2 and re.fullmatch(r"[0-9a-fA-F]+", parts[1]):
            out[parts[0]] = int(parts[1], 16)
    return out
