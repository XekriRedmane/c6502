"""Top-level test harness: take C source, run the full compiler
pipeline, assemble the result with a runtime stub, and execute on a
py65 6502 simulator until BRK.

Two entry points:

  `run_c_program(source)` — convenience: compile, simulate, return a
  SimResult with the value in A / X / HARGS plus a memory snapshot.

  `build_sim(source)` — lower-level: returns a Simulation object
  bundling the MPU, memory map, and runtime hooks. Useful for tests
  that want to step manually, inspect mid-execution state, or assert
  on calling-convention invariants.

Helper-trap dispatch: before each step, the harness checks whether PC
matches a runtime trap address. If so, it runs the Python hook against
the MPU's memory and synthesizes an RTS (pop two bytes from the HW
stack and set PC = popped + 1). This is how `mul16` / `divmod16` /
shifts / FP conversions work without the asm helpers existing yet.

Exit condition: PC pointing at BRK ($00). The boot stub's trailing BRK
is what halts the simulator after `main` returns.
"""

from __future__ import annotations

from dataclasses import dataclass

from py65.devices.mpu6502 import MPU

import asm_ast
from preprocessor import preprocess
from parser import parse
from passes.identifier_resolution import resolve_program as resolve_identifiers
from passes.string_lifting import lift_program as lift_strings
from passes.label_resolution import resolve_program as resolve_labels
from passes.loop_labeling import label_program as label_loops
from passes.long_branches import expand_program as expand_long_branches
from passes.type_checking import check_program as type_check_program, StaticAttr
from passes.replace_pseudoregisters import replace_program as replace_pseudoregs
from c99_to_tac import translate_program as translate_to_tac
from passes.optimization import optimize_program as optimize_tac
from tac_to_asm import translate_program as translate_to_asm

from sim import assembler
from sim import runtime as rt_mod


DEFAULT_ORIGIN = 0x0800
DEFAULT_MAX_CYCLES = 10_000_000


# -------- compile pipeline (C source → asm_ast) --------


def compile_to_asm(
    source: str, *, optimize: bool = False,
) -> tuple[asm_ast.Program, dict, dict]:
    """Run the full pipeline up through asm_ast (post pseudo
    elimination). Returns (asm_program, symbols, types) — the symbol
    and type tables come back so the harness can pick the right
    return-value width based on `main`'s declared type.

    `optimize=True` runs the SSA-bracketed optimizer including
    register allocation; the resulting per-function colorings are
    threaded into `replace_pseudoregisters` so colored values lower
    to ZP operands. Default is False (matches today's pre-regalloc
    behavior)."""
    pp = preprocess(source)
    ast0 = parse(pp)
    ast1 = resolve_identifiers(ast0)
    ast2 = lift_strings(ast1)
    ast3 = resolve_labels(ast2)
    ast4 = label_loops(ast3)
    ast5, syms, types = type_check_program(ast4)
    tac = translate_to_tac(ast5, syms, types)
    colorings: dict = {}
    if optimize:
        tac, colorings = optimize_tac(tac, syms)
    asm0 = translate_to_asm(tac, syms, types)
    statics = frozenset(
        n for n, s in syms.items() if isinstance(s.attrs, StaticAttr)
    )
    asm = expand_long_branches(replace_pseudoregs(
        asm0, extra_statics=statics, symbols=syms, types=types,
        colorings=colorings,
    ))
    return asm, syms, types


# -------- Simulation state --------


@dataclass
class SimResult:
    """Outcome of running a program through the simulator until BRK
    (or hitting the cycle cap, in which case `timed_out=True`)."""
    a: int                  # accumulator at exit
    x: int                  # X register at exit
    y: int                  # Y register at exit
    pc: int                 # PC at exit (points at the halting BRK)
    sp: int                 # hardware-stack pointer at exit
    cycles: int             # py65 cycle count at exit
    memory: bytearray       # full 64KiB snapshot
    timed_out: bool = False

    def return_char(self) -> int:
        """Return value as an unsigned 1-byte integer (Char / SChar /
        UChar): the value sits in A on RTS per the calling
        convention."""
        return self.a & 0xFF

    def return_char_signed(self) -> int:
        """Return value as a signed 1-byte integer."""
        v = self.a & 0xFF
        return v - 0x100 if v & 0x80 else v

    def return_int(self) -> int:
        """Return value as an unsigned 2-byte integer (Int / UInt /
        Pointer): bytes read from HARGS+0..1 — the slot that 2-byte
        returns land in per the calling convention."""
        return (
            self.memory[rt_mod.HARGS + 0]
            | (self.memory[rt_mod.HARGS + 1] << 8)
        )

    def return_int_signed(self) -> int:
        v = self.return_int()
        return v - 0x10000 if v & 0x8000 else v

    def return_long(self) -> int:
        """Return value as an unsigned 4-byte integer (Long / ULong /
        Float bit pattern). Read from HARGS+8..11 — the FP-result
        slot, where 4-byte returns land per the calling convention."""
        v = 0
        for i in range(4):
            v |= self.memory[rt_mod.HARGS + 8 + i] << (i * 8)
        return v

    def return_long_signed(self) -> int:
        v = self.return_long()
        return v - 0x100000000 if v & 0x80000000 else v

    def return_longlong(self) -> int:
        """Return value as an unsigned 8-byte integer (LongLong /
        ULongLong / Double bit pattern). Read from HARGS+16..23 —
        the 8-byte slot per the calling convention."""
        v = 0
        for i in range(8):
            v |= self.memory[rt_mod.HARGS + 16 + i] << (i * 8)
        return v

    def return_longlong_signed(self) -> int:
        v = self.return_longlong()
        return v - (1 << 64) if v & (1 << 63) else v


class Simulation:
    """A loaded-and-ready-to-run simulator instance.

    Construct via `build_sim`. The `run` method drives the MPU until
    BRK or the cycle cap; tests that need finer control (e.g.
    inspecting state mid-execution) can call `step` themselves.

    `symbols` exposes the assembler's resolved labels so tests can
    refer to functions / statics by name."""

    def __init__(
        self,
        mpu: MPU,
        symbols: dict[str, int],
        runtime: rt_mod.Runtime,
        origin: int,
        code_end: int,
    ) -> None:
        self.mpu = mpu
        self.symbols = symbols
        self.runtime = runtime
        self.origin = origin
        self.code_end = code_end

    # The harness's step loop. Each iteration either fires a helper
    # hook (if PC matches a trap address) or asks py65 to run one
    # 6502 instruction.
    def _hook_and_rts(self) -> None:
        """Run the current PC's helper hook against memory, then
        synthesize an RTS to resume the caller. JSR pushes
        `return_address - 1` (high byte then low byte) onto the
        hardware stack at $0100 + sp; RTS pops two bytes (low first)
        and sets `pc = popped + 1`."""
        mpu = self.mpu
        hook = self.runtime.hooks[mpu.pc]
        hook(mpu.memory)
        sp = mpu.sp
        lo = mpu.memory[0x0100 + ((sp + 1) & 0xFF)]
        hi = mpu.memory[0x0100 + ((sp + 2) & 0xFF)]
        mpu.sp = (sp + 2) & 0xFF
        mpu.pc = (((hi << 8) | lo) + 1) & 0xFFFF

    def step(self) -> bool:
        """One simulator step. Returns True if execution should
        continue, False if a BRK was just observed (i.e. the program
        has halted). Helper traps run inline and don't observe a BRK."""
        mpu = self.mpu
        if mpu.pc in self.runtime.hooks:
            self._hook_and_rts()
            return True
        if mpu.memory[mpu.pc] == 0x00:   # BRK
            return False
        mpu.step()
        return True

    def run(self, max_cycles: int = DEFAULT_MAX_CYCLES) -> SimResult:
        """Drive the simulator forward until BRK or the cycle cap."""
        mpu = self.mpu
        timed_out = False
        while mpu.processorCycles < max_cycles:
            if not self.step():
                break
        else:
            timed_out = True
        return SimResult(
            a=mpu.a, x=mpu.x, y=mpu.y, pc=mpu.pc, sp=mpu.sp,
            cycles=mpu.processorCycles,
            memory=bytearray(mpu.memory),
            timed_out=timed_out,
        )


# -------- top-level helpers --------


def build_sim(
    source: str, *,
    origin: int = DEFAULT_ORIGIN,
    optimize: bool = False,
) -> Simulation:
    """Compile, assemble, install runtime, set up MPU. Doesn't run.
    `optimize=True` enables the SSA-bracketed optimizer including
    register allocation."""
    asm_prog, _syms, _types = compile_to_asm(source, optimize=optimize)
    runtime = rt_mod.build_runtime()
    assembled = assembler.assemble(
        asm_prog, origin=origin, extra_symbols=runtime.symbols,
    )
    if "main" not in assembled.symbols:
        raise ValueError("program has no `main` function")
    rt_mod.install_runtime(
        assembled.image, runtime, assembled.symbols["main"],
    )
    mpu = MPU()
    mpu.memory = assembled.image
    # Skip the reset() machinery and place PC at the boot stub directly
    # — same effect as resetting after writing $FFFC, but doesn't
    # depend on py65's `start_pc` default.
    mpu.pc = runtime.boot_addr
    mpu.processorCycles = 0
    return Simulation(
        mpu=mpu, symbols=assembled.symbols, runtime=runtime,
        origin=assembled.origin, code_end=assembled.code_end,
    )


def run_c_program(
    source: str, *,
    origin: int = DEFAULT_ORIGIN,
    max_cycles: int = DEFAULT_MAX_CYCLES,
) -> SimResult:
    """Compile, assemble, simulate. Returns the final state once
    execution halts at BRK (or the cycle cap fires)."""
    sim = build_sim(source, origin=origin)
    return sim.run(max_cycles=max_cycles)
