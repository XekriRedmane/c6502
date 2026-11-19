"""Runs a linked program (the ELF executable that linking with
`compile.py` writes) in the simulator.

`build_linked_sim` loads each segment of the executable at its load
address and reads the symbol table, then installs the runtime (the
boot stub and the helpers, `sim/runtime.py`) for the program's
runtime layout, which the executable's SSP, FP, HARGS, and DPTR
symbols give.

A program can get the runtime's helpers two ways. Linked without
`--defsym`, it holds the helpers it calls from the runtime library
(passes/runtime_library.py), and they run where the linker put them.
Linked with each helper's address given as a `--defsym`
(`runtime_defsyms`, `defsym_args`), it calls the simulator's copies:
the 6502 helpers at $E000 and the Python hooks after them, for the
helpers that have no 6502 code, such as the floating-point ones. The
simulator's copies sit at the same addresses for every runtime
layout, and `build_linked_sim` checks that the executable's absolute
addresses for them are the runtime's.
"""
from __future__ import annotations

from pathlib import Path

from py65.devices.mpu6502 import MPU

import sim.runtime as rt_mod
from binutils import elf as E
from sim.harness import DEFAULT_ORIGIN, Simulation


def runtime_defsyms() -> dict[str, int]:
    """The address of every runtime helper, for linking a program the
    simulator runs (`compile.py --defsym`, or `link_objects`'s
    `defsyms`)."""
    return dict(rt_mod.build_runtime().symbols)


def defsym_args() -> list[str]:
    """`runtime_defsyms` as `compile.py` arguments."""
    return [f"--defsym={name}=0x{addr:04X}"
            for name, addr in sorted(runtime_defsyms().items())]


def build_linked_sim(
    exe: E.ElfFile | bytes | str | Path, origin: int = DEFAULT_ORIGIN,
) -> Simulation:
    """A simulator ready to run the executable `exe` (an `ElfFile`,
    its bytes, or its path) from the boot stub, which calls
    `__zp_init` (when the program has one) and then `main`."""
    if isinstance(exe, (str, Path)):
        exe = E.read_file(exe)
    elif isinstance(exe, (bytes, bytearray)):
        exe = E.read(bytes(exe), "executable")
    if exe.type != E.ET_EXEC:
        raise RuntimeError("build_linked_sim needs a linked executable")
    from passes.runtime_layout import CLASSIC, RuntimeLayout
    symbols: dict[str, int] = {}
    # The names the program defines in its own sections, which include
    # the helpers it holds from the runtime library.
    own: set[str] = set()
    for s in exe.symbols:
        if not s.defined or s.type == E.STT_FILE:
            continue
        if s.is_global or s.name not in symbols:
            symbols[s.name] = s.value
        if s.section != E.SHN_ABS:
            own.add(s.name)
    layout = RuntimeLayout(
        ssp=symbols.get("SSP", CLASSIC.ssp),
        fp=symbols.get("FP", CLASSIC.fp),
        hargs=symbols.get("HARGS", CLASSIC.hargs),
        dptr=symbols.get("DPTR", CLASSIC.dptr),
    )
    runtime = rt_mod.build_runtime(layout)
    for name, addr in runtime.symbols.items():
        if name in own:
            continue
        if name in symbols and symbols[name] != addr:
            raise RuntimeError(
                f"the executable has `{name}` at ${symbols[name]:04X}, "
                f"and the runtime has it at ${addr:04X}"
            )
        symbols.setdefault(name, addr)
    image = bytearray(0x10000)
    end = origin
    for seg in exe.segments:
        image[seg.paddr:seg.paddr + len(seg.data)] = seg.data
        end = max(end, seg.paddr + len(seg.data))
    if "main" not in symbols:
        raise RuntimeError("the executable has no `main`")
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
