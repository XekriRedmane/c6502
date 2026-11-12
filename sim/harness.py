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
from passes.long_branches import expand_program as expand_long_branches
from passes.type_checking import check_program as type_check_program, StaticAttr
from passes.replace_pseudoregisters import replace_program as replace_pseudoregs
from c99_to_tac import translate_program as translate_to_tac
from passes.abi_selection import fixed_address_layouts
from tac_to_asm import translate_program as translate_to_asm
# The front-end passes and the TAC optimizer, which fuzz/reduce.py
# reaches through this module.
from parser import parse  # noqa: F401
from passes.identifier_resolution import (  # noqa: F401
    resolve_program as resolve_identifiers,
)
from passes.string_lifting import lift_program as lift_strings  # noqa: F401
from passes.label_resolution import (  # noqa: F401
    resolve_program as resolve_labels,
)
from passes.loop_labeling import label_program as label_loops  # noqa: F401
from passes.optimization import (  # noqa: F401
    optimize_program as optimize_tac,
)

from sim import assembler
from sim import runtime as rt_mod


DEFAULT_ORIGIN = 0x0800
DEFAULT_MAX_CYCLES = 10_000_000


# -------- compile pipeline (C source → asm_ast) --------


def compile_to_asm(
    source: str, *, optimize: bool = False,
) -> tuple[asm_ast.Program, dict, dict, dict[str, int]]:
    """Run the full pipeline up through asm_ast (post pseudo
    elimination). Returns
    `(asm_program, symbols, types, zp_slot_symbols)` — the symbol
    and type tables come back so the harness can pick the right
    return-value width based on `main`'s declared type, and
    `zp_slot_symbols` carries the zp_abi slot-name → address
    bindings produced by `passes.zp_slot_allocation` (empty when
    optimization is off or no zp_abi function is present).

    Both pipelines run the passes `compile.py --codegen` runs, so the
    simulator executes the code the CLI emits. `optimize=True` runs
    `compile.optimized_asm`, the `--optimize` pipeline."""
    from compile import _resolved
    ast5, syms, types = type_check_program(
        _resolved(preprocess(source), optimize=optimize),
    )
    from passes.zp_globals import plan_zero_page, zero_page_in_effect
    tac = translate_to_tac(ast5, syms, types)
    zp = plan_zero_page(tac, syms, types, promote=optimize)
    with zero_page_in_effect(zp):
        return _compile_tac(ast5, syms, types, optimize, zp, tac)


def _compile_tac(ast5, syms, types, optimize: bool, zp, tac):
    """The rest of `compile_to_asm`, after translation to TAC (`tac`),
    with the translation unit's zero-page globals and runtime layout
    `zp`. Returns the same tuple; the slot-symbol map includes the
    zero-page globals and the runtime's symbols, so the assembler
    binds them."""
    from passes.zp_globals import place_zp_globals
    statics = frozenset(
        n for n, s in syms.items() if isinstance(s.attrs, StaticAttr)
    )
    if optimize:
        # The same pipeline `compile.py --codegen --optimize` runs.
        from compile import optimized_asm
        opt = optimized_asm(ast5, syms, types, zp, tac, statics)
        return opt.asm, syms, types, {**opt.slot_symbols, **zp.equs()}
    # Unoptimized path: the same passes `compile.py --codegen` runs
    # without `--optimize`, including the full peephole fixedpoint,
    # so the sim executes the code the CLI emits.
    from compile import _peephole_fixedpoint
    # Calls to a zp_abi routine with a fixed address pass their
    # arguments in registers here too, since the routine's
    # convention can't depend on the pipeline.
    asm0 = translate_to_asm(
        tac, syms, types,
        abi=fixed_address_layouts(tac, ast5, types),
    )
    asm = expand_long_branches(_peephole_fixedpoint(
        replace_pseudoregs(
            asm0, extra_statics=statics, symbols=syms, types=types,
        ),
        zp_global_symbols=zp.addrs,
    ))
    return place_zp_globals(asm, zp), syms, types, zp.equs()


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
        """Return value as an unsigned 2-byte integer (Int / UInt):
        low byte in A, high byte in X per the calling convention (a
        non-pointer return of <=2 bytes rides in registers)."""
        return (self.a & 0xFF) | ((self.x & 0xFF) << 8)

    def return_pointer(self) -> int:
        """Return value as a 2-byte pointer: bytes read from
        HARGS+0..1. Pointers don't ride in registers (the register-
        passing rules exclude them), so a pointer-returning function
        leaves its result in HARGS like a wider return."""
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
        function_names: set[str] | None = None,
    ) -> None:
        self.mpu = mpu
        self.symbols = symbols
        self.runtime = runtime
        self.origin = origin
        self.code_end = code_end
        # Names of top-level `asm_ast.Function` entry points in the
        # assembled program (no static-variable labels, no internal
        # `.foo@N` block labels). Used by `run_bucketed` to attribute
        # cycles to source-level functions.
        self.function_names: set[str] = (
            set(function_names) if function_names is not None else set()
        )

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

    def run_bucketed(
        self, max_cycles: int = DEFAULT_MAX_CYCLES,
    ) -> tuple[SimResult, dict[str, int]]:
        """Like `run()` but also returns a per-function cycle-bucket
        map. Each step's cycle delta is attributed to the function
        whose entry-point address is the largest in `function_names`
        not exceeding the current PC. Cycles spent at the boot stub,
        inside runtime-helper hooks, or anywhere outside a named
        function fall into the `<other>` bucket.

        Useful for optimization measurement: the whole-program
        `SimResult.cycles` total mixes the function under test with
        test instrumentation (`record()`, stubbed callees, `main()`
        setup), which dilutes the optimization-relevant delta. The
        bucket map lets callers quote the function-under-test slice
        directly."""
        import bisect
        mpu = self.mpu
        boundaries = sorted(
            (self.symbols[name], name)
            for name in self.function_names
            if name in self.symbols
        )
        boundary_addrs = [a for a, _ in boundaries]
        boundary_names = [n for _, n in boundaries]
        cycles_by_fn: dict[str, int] = {n: 0 for n in boundary_names}
        cycles_by_fn["<other>"] = 0

        def _bucket_for(pc: int) -> str:
            # Largest boundary <= pc; "<other>" if no boundary fits
            # (pc before the first function or in static-data tail).
            idx = bisect.bisect_right(boundary_addrs, pc) - 1
            if idx < 0:
                return "<other>"
            return boundary_names[idx]

        timed_out = False
        prev_c = mpu.processorCycles
        while mpu.processorCycles < max_cycles:
            pc = mpu.pc
            if pc in self.runtime.hooks:
                self._hook_and_rts()
                cycles_by_fn["<other>"] += (
                    mpu.processorCycles - prev_c
                )
                prev_c = mpu.processorCycles
                continue
            if mpu.memory[pc] == 0x00:  # BRK
                break
            bucket = _bucket_for(pc)
            mpu.step()
            cycles_by_fn[bucket] += mpu.processorCycles - prev_c
            prev_c = mpu.processorCycles
        else:
            timed_out = True
        result = SimResult(
            a=mpu.a, x=mpu.x, y=mpu.y, pc=mpu.pc, sp=mpu.sp,
            cycles=mpu.processorCycles,
            memory=bytearray(mpu.memory),
            timed_out=timed_out,
        )
        return result, cycles_by_fn


# -------- top-level helpers --------


def build_sim(
    source: str, *,
    origin: int = DEFAULT_ORIGIN,
    optimize: bool = False,
) -> Simulation:
    """Compile, assemble, install runtime, set up MPU. Doesn't run.
    `optimize=True` runs the optimizer pipeline (TAC fixed-point
    opts, asm-SSA round-trip, byte-granular regalloc, late prologue
    synthesis, plus `__attribute__((zp_abi))` frame elimination)."""
    asm_prog, _syms, _types, zp_slot_symbols = compile_to_asm(
        source, optimize=optimize,
    )
    from passes.runtime_layout import CLASSIC, RuntimeLayout
    layout = RuntimeLayout(
        ssp=zp_slot_symbols.get("SSP", CLASSIC.ssp),
        fp=zp_slot_symbols.get("FP", CLASSIC.fp),
        hargs=zp_slot_symbols.get("HARGS", CLASSIC.hargs),
        dptr=zp_slot_symbols.get("DPTR", CLASSIC.dptr),
    )
    runtime = rt_mod.build_runtime(layout)
    extra_symbols = dict(runtime.symbols)
    extra_symbols.update(zp_slot_symbols)
    assembled = assembler.assemble(
        asm_prog, origin=origin, extra_symbols=extra_symbols,
    )
    if "main" not in assembled.symbols:
        raise ValueError("program has no `main` function")
    rt_mod.install_runtime(
        assembled.image, runtime, assembled.symbols["main"],
        init_addr=assembled.symbols.get("__zp_init"),
    )
    mpu = MPU()
    mpu.memory = assembled.image
    # Skip the reset() machinery and place PC at the boot stub directly
    # — same effect as resetting after writing $FFFC, but doesn't
    # depend on py65's `start_pc` default.
    mpu.pc = runtime.boot_addr
    mpu.processorCycles = 0
    function_names = {
        tl.name for tl in asm_prog.top_level
        if isinstance(tl, asm_ast.Function)
    }
    return Simulation(
        mpu=mpu, symbols=assembled.symbols, runtime=runtime,
        origin=assembled.origin, code_end=assembled.code_end,
        function_names=function_names,
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
